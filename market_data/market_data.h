

#ifndef MARKET_DATA_H
#define MARKET_DATA_H

#include <chrono>

struct MarketData {
    std::chrono::system_clock::time_point timestamp;

    int bid_price;
    int ask_price;

    int bid_volume;
    int ask_volume;
};

#endif
