#pragma once

#include <list>
#include <format>
#include <memory>
#include <stdexcept>

#include "OrderType.h"
#include "GlobalVariables.h"
#include "Side.h"
#include "Constants.h"

class Order
{
public:
    Order(OrderType orderType, OrderId orderId, Side side, Price price, Quantity quantity)
        : orderType_{ orderType }
        , orderId_{ orderId }
        , side_ { side }
        , price_ { price }
        , initialQuantity_{ quantity }
        , remaningQuantity_ { quantity }
    { }

    Order(OrderId orderId, Side side, Quantity quantity)
        : Order(OrderType::Market, orderId, side, Constants::InvalidPrice, quantity)
    { }

    OrderType GetOrderType() const { return orderType_; }
    OrderId GetOrderId() const { return orderId_; }
    Side GetSide() const { return side_; }
    Price GetPrice() const { return price_;}
    Quantity GetInitialQuantity() const { return initialQuantity_; }
    Quantity GetRemaningQuantity() const { return remaningQuantity_; }
    Quantity GetFilledQuantity() const { return GetInitialQuantity() - GetRemaningQuantity(); }

    bool IsFilled() const { return GetRemaningQuantity() == 0;}
    
    void Fill(Quantity quantity)
    {
        if (quantity > GetRemaningQuantity())
            throw std::logic_error(std::format("Order ({}) cannot be filled for more than the remaning Quantity", GetOrderId()));

        remaningQuantity_ -= quantity;
    }

    void ToGoodTillCancel(Price price){
        if (GetOrderType() != OrderType::GoodTillCancel)
            throw std::logic_error(std::format("Order ({}) cannot have it price adjusted, only markets order can do that", GetOrderId()));

        price_ = price;
        orderType_ = OrderType::GoodTillCancel;
    }

private:
    OrderType orderType_;
    OrderId orderId_;
    Side side_;
    Price price_;
    Quantity initialQuantity_;
    Quantity remaningQuantity_;
};

using OrderPointer = std::shared_ptr<Order>;
using OrderPointers = std::list<OrderPointer>;

