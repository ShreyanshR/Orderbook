#pragma once

#include <functional>
#include <map>
#include <unordered_map>
#include <thread>
#include <condition_variable>
#include <mutex>

#include "GlobalVariables.h"
#include "Order.h"
#include "OrderbookLevelInfos.h"
#include "Trade.h"
#include "OrderModify.h"


class OrderBook
{
    private:
        struct OrderEntry
        {
            OrderPointer order_{ nullptr }; // Actual order object
            OrderPointers::iterator location_; // Iterator pointing to its positin in bids_/askç
        };

        struct LevelData
        {
            Quantity quantity_{ }; //Total Quantity as this price
            Quantity count_{ }; //Numbers of orders at this price

            enum class Action
            {
                Add, //Order Added
                Remove, // Order Removed
                Match, // Order Matched/Filled
            };
        };

        std::unordered_map<Price, LevelData> data_;
        std::map<Price, OrderPointers, std::greater<Price>> bids_; //best bid on the top, highest bid at top
        std::map<Price, OrderPointers, std::less<Price>> asks_; // Best ask, lowest bid at top
        std::unordered_map<OrderId, OrderEntry> orders_;
        mutable std::mutex ordersMutex_;
        std::thread ordersPruneThread_;
        std::condition_variable shutdownConditionVariable_;
        std::atomic<bool> shutdown_{ false };

        void PruneGoodForDayOrder();
        void CancelOrders(OrderIds orderIds);
        void CancelOrderInternal(OrderId orderId);

        void OnOrderCancelled(OrderPointer order);
        void OnOrderAdded(OrderPointer order);
        void OnOrderMatched(Price price, Quantity quantity, bool isFullyFilled);
        void UpdateLevelData(Price price, Quantity quantity, LevelData::Action action);

        bool CanFullyFill(Side side, Price price, Quantity quantity) const;
        bool CanMatch(Side side, Price price) const;
        Trades MatchOrders();



    public:
        OrderBook();
        OrderBook(const OrderBook&) = delete;
        void operator=(const OrderBook&) = delete;
        OrderBook(OrderBook&&) = delete;
        void operator=(OrderBook&&) = delete;
        ~OrderBook();

        Trades AddOrder(OrderPointer order);
        void CancelOrder(OrderId orderId);
        Trades ModifyOrder(OrderModify order);

        std::size_t Size() const;
        OrderBookLevelInfos GetOrderInfos() const;
};