#include <cstdint>
#include <format>
#include <functional>
#include <list>
#include <memory>
#include <stdatomic.h>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <map>

enum class OrderType
{
    GoodTillCancel,
    FillAndKill
};

enum class Side
{
    Buy,
    Sell
};

using Price = std::int32_t;
using Quantity = std::uint32_t;
using OrderId = std::uint64_t;

struct LevelInfo
{
    Price price_;
    Quantity quantity_;
};

using LevelInfos = std::vector<LevelInfo>;

class OrderBookLevelInfos
{
private:
    LevelInfo bids_;
    LevelInfos asks_;

public:
    OrderBookLevelInfos(const LevelInfo& bids, const LevelInfos& asks):
        bids_{bids}, asks_{asks}
    { }

    const LevelInfo& GetBids() const{return bids_;}
    const LevelInfos& GetAsks() const{return asks_;}
};

class Order
{
public:
    Order(OrderType orderType, OrderId orderId, Side side, Price price, Quantity quantity)
        : orderType_{ orderType }
        , orderId_{ orderId }
        , side_{ side }
        , price_{ price }
        , initialQuantity_ { quantity }
        , remaningQuantity_{ quantity }
    { }

    OrderType GetOrderType() const { return orderType_; }
    OrderId GetOrderId() const { return orderId_;}
    Side GetSide() const { return side_; }
    Price GetPrice() const { return price_;}
    Quantity GetInitialQuantity() const {return initialQuantity_; }
    Quantity GetRemaningQuantity() const {return remaningQuantity_; }
    Quantity GetFilledQuantity() const {return GetInitialQuantity() - GetRemaningQuantity(); }

    void Fill(Quantity quantity)
    {
        if (quantity > GetFilledQuantity())
            throw std::logic_error(std::format("Order ({}) cannot be filled for more than it's remaning Quantity.", GetOrderId()));
        remaningQuantity_ -= quantity;
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

struct TradeInfo
{
    OrderId orderId_;
    Price price_;
    Quantity quantity_;
};

class Trade
{
    public:
        Trade(const TradeInfo& bidTrade, const TradeInfo& askTrade)
            : bidTrade_{ bidTrade }
            , askTrade_{ askTrade }
        { }

        const TradeInfo& GetBidTrade() const { return bidTrade_; }
        const TradeInfo& GetAskTrade() const { return askTrade_; }

    private:
        TradeInfo bidTrade_;
        TradeInfo askTrade_;

};

using Trades = std::vector<Trade>;

class Orderbook
{
    private:
        struct OrderEntry
        {
            OrderPointer order_{ nullptr };
            OrderPointers::iterator location_;
        };

        std::map<Price, OrderPointers, std::greater<Price>> bids_;
        std::map<Price, OrderPointers, std::less<Price>> asks_;
        std::unordered_map<OrderId, OrderEntry> orders_;

        bool CanMatch(Side side, Price price) const
        {
            if (side == Side::Buy)
            {
                if (asks_.empty())
                    return false;

                const auto& [bestAsk, _] = *asks_.begin();
                return price >= bestAsk;
            }
            else {
            {
                if (bids_.empty())
                    return false;
                const auto& [bestBid, _] = *bids_.begin();
                return price <= bestBid;
            }
            }
        }

        Trades MatchOrders()
        {
            Trades trades;
            trades.reserve(orders_.size()); 

            while (true)
            {

            }
        }


    
};