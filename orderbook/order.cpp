
#pragma once

#include <iostream>
#include <chrono>
#include <string>
#include <random>

enum class OrderSide : bool {
    buy,
    sell
};

inline int generateID(){
     std::random_device rd;
     std::mt19937 gen(rd());
     int min = 1000000;
     int max = 9999999;
     std::uniform_int_distribution<int> distr(min, max);

     return distr(gen);
}

struct Order{
    int order_id;
    int price;
    int quantity;
    OrderSide side;
    std::chrono::system_clock::time_point created_at = std::chrono::system_clock::now();

    Order* next;
    Order* prev;

    Order(int _id, int _price, int _quantity, OrderSide _side) : order_id(_id),
                price(_price), quantity(_quantity), side(_side), next(nullptr), prev(nullptr){}
};
