#include <orderbook/OrderBook.hpp>
#include <iostream>

int main() {
    orderbook::OrderBook book;
    
    // Add some orders
    orderbook::Order bid1{"bid1", orderbook::Side::BUY, orderbook::OrderType::LIMIT, 100.0, 100, 0};
    orderbook::Order bid2{"bid2", orderbook::Side::BUY, orderbook::OrderType::LIMIT, 99.5, 200, 0};
    orderbook::Order ask1{"ask1", orderbook::Side::SELL, orderbook::OrderType::LIMIT, 101.0, 150, 0};
    orderbook::Order ask2{"ask2", orderbook::Side::SELL, orderbook::OrderType::LIMIT, 101.5, 75, 0};
    
    book.add_order(bid1);
    book.add_order(bid2);
    book.add_order(ask1);
    book.add_order(ask2);
    
    std::cout << "Order Book Status:" << std::endl;
    std::cout << "Best Bid: " << book.get_best_bid() << std::endl;
    std::cout << "Best Ask: " << book.get_best_ask() << std::endl;
    std::cout << "Spread: " << book.get_spread() << std::endl;
    std::cout << "Bid Volume: " << book.get_bid_volume() << std::endl;
    std::cout << "Ask Volume: " << book.get_ask_volume() << std::endl;
    std::cout << "Total Orders: " << book.size() << std::endl;
    
    return 0;
}
