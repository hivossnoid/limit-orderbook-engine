
#pragma once

#include <iostream>
#include <chrono>
#include <string>
#include <unordered_map>
#include <memory>

#include "./order.cpp"
#include "./redblacktree.cpp"
#include "./limit.cpp"

class MatchingEngine;


class Orderbook {
private:
    std::unique_ptr<MatchingEngine> matching_engine;

    Redblacktree buy_tree;
    Redblacktree sell_tree;

    std::unordered_map<int, Order*> order_bucket;

    Order* build_order(int _price, int _quantity, OrderSide _side);

    int fetch_maxval();
    int fetch_minval();

public:
    Orderbook();
    ~Orderbook();

    int get_best_bid();

    int get_best_ask();

    Limit* fetch_limit(OrderSide _side, int _price);

    Order* fetch_order(int _id);

    void remove_order_from_bucket(int _id);

    void remove_limit(OrderSide _side, int _price);

    void add_order_to_book(Order& new_order);

    void submit_limit_order(
        int _price,
        int _quantity,
        OrderSide _side
    );

    void cancel_limit_order(int _id);

    void modify_limit_order(
        int _id,
        int _new_price,
        int _new_quantity
    );

    void submit_market_order(
        int _quantity,
        OrderSide _side
    );

    void print_all();
    void bucket_cleanup();
};
