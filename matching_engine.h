
#pragma once

#include <iostream>
#include <algorithm>
#include <chrono>

#include "./order.cpp"
#include "./limit.cpp"
#include "./trade.cpp"
#include "./orderbook.h"

class MatchingEngine {
private:
    Orderbook& orderbook;

    int execute_trade(
        Order& buy_order,
        Order& sell_order,
        int exec_price
    );

    bool check_price_cross(
        Order& buy_order,
        Order& sell_order
    );

public:
    MatchingEngine(Orderbook& _orderbook);

    void match_limit_order(Order& incoming_order);

    void match_market_order(Order& incoming_order);
};
