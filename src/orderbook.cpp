#include "Orderbook.h"
#include "GlobalVariables.h"
#include "Order.h"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <numeric>
#include <chrono>
#include <ctime>


void OrderBook::PruneGoodForDayOrder()
{
    using namespace std::chrono;
    const auto end = hours(16);

    while(true)
    {
        const auto now = system_clock::now();
        const auto now_c = system_clock::to_time_t(now);

        std::tm now_parts;

        localtime_r(&now_c, &now_parts); // convert the current time into human readable time

        if(now_parts.tm_hour >= end.count())
            now_parts.tm_mday += 1;

        now_parts.tm_hour = end.count();
        now_parts.tm_min = 0;
        now_parts.tm_sec = 0;

        auto next = system_clock::from_time_t(mktime(&now_parts));
        auto till = next - now + milliseconds(100);


        {
            std::unique_lock ordersLock{ ordersMutex_};

            if (shutdown_.load(std::memory_order_acquire) || shutdownConditionVariable_.wait_for(ordersLock, till) == std::cv_status::no_timeout)
                return;
        }

        OrderIds orderIds;

        {
            std::scoped_lock ordersLock{ ordersMutex_}; 

            for(const auto& [orderId, entry] : orders_)
            {
                const auto& [order, location] = entry;
                if (order->GetOrderType() != OrderType::GoodForDay)
                    continue;

                orderIds.push_back(order->GetOrderId());
            }
        }
        CancelOrders(orderIds);

    }
}

void OrderBook::CancelOrders(OrderIds orderIds)
{
    std::scoped_lock ordersLock{ ordersMutex_};

    for(const auto& orderId : orderIds)
        CancelOrderInternal(orderId);
}

void OrderBook::CancelOrderInternal(OrderId orderId)
{
    if(!orders_.contains(orderId))
        return;

    const auto& [order, iterator] = orders_.at(orderId);
    orders_.erase(orderId);
    //orders_ is a unordered map of OrderPointers, 
    // which can Shared Pointer to each OrderPointer, 
    // and that can access to Order class, and then order class can access to GetPrice()
    if (order->GetSide() == Side::Sell) 
    {
        auto price = order->GetPrice();
        auto& orders = asks_.at(price);
        orders.erase(iterator);
        if(orders.empty())
            asks_.erase(price);
    }
    else
    {
        auto price = order->GetPrice();
        auto& orders = bids_.at(price);
        orders.erase(iterator);
        if(orders.empty())
            bids_.erase(price);
    }

    OnOrderCancelled(order);
}