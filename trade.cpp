#include <iostream>
#include <chrono>

#pragma once

struct Trade{
    int buy_order_id;
    int sell_order_id;

    int exec_price;
    int exec_quantity;

    std::chrono::system_clock::time_point executed_at;

    Trade(int _buy_id, int _sell_id, int _price, int _quantity)
        : buy_order_id(_buy_id), sell_order_id(_sell_id), exec_price(_price),
       exec_quantity(_quantity), executed_at(std::chrono::system_clock::now()) {}
};
