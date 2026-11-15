#include <iostream>
#include <chrono>
#include <string>
#include <unordered_map>

#include "./order.cpp"
#include "./redblacktree.cpp"
#include "./limit.cpp"

#pragma once

class Orderbook{
private:
    Redblacktree buy_tree;
    Redblacktree sell_tree;

    std::unordered_map<int, Order*> order_bucket;

    Order* build_order(int _price, int _quantity, OrderSide _side){
        int order_id = generateID();
        Order* new_order = new Order(order_id, _price, _quantity, _side);
        order_bucket.insert({ order_id, new_order });

        std::cout << "Order Building Success! \n";
        return new_order;
    }

public:

    void fetch_order(int _id){
        auto it = order_bucket.find(_id);

        if (it == order_bucket.end()){
            std::cout << "Order not found.\n";
            return;
        }

        Order* temp = it->second;

        std::cout << "Order ID: " << temp->order_id
                << " Price: " << temp->price
                << " Quantity: " << temp->quantity
                << " Side: " << static_cast<int>(temp->side)
                << " Timestamp: "
                << std::chrono::system_clock::to_time_t(temp->created_at)
                << "\n";
    }

    void submit_limit_order(int _price, int _quantity, OrderSide _side){
        Order* new_order = this->build_order(_price, _quantity, _side);

        if (_side == OrderSide::buy){
            Redblacktree& tree = buy_tree;

            Limit* target_limit = tree.search_limit(_price);

            if (target_limit == nullptr){
                target_limit = new Limit(_price);
                tree.insert_value(_price, target_limit);
            }
            target_limit->enlist_order(*new_order);

        } else if (_side == OrderSide::sell){

            Redblacktree& tree = sell_tree;

            Limit* target_limit = tree.search_limit(_price);

            if (target_limit == nullptr){
                target_limit = new Limit(_price);
                tree.insert_value(_price, target_limit);
            }

            target_limit->enlist_order(*new_order);
        }
        fetch_order(new_order->order_id);

        std::cout << "Order Submission Success! \n";
    }

    void cancel_limit_order(int _id){
        Order* target_order = order_bucket.at(_id);

        if (target_order->side == OrderSide::buy){
            Redblacktree& tree = buy_tree;

            Limit* target_limit = tree.search_limit(target_order->price);
            target_limit->remove_order(*target_order);

        } else if (target_order->side == OrderSide::sell){
            Redblacktree& tree = sell_tree;

            Limit* target_limit = tree.search_limit(target_order->price);
                target_limit->remove_order(*target_order);
        }

        order_bucket.erase(_id);
        delete target_order;

        std::cout << "Order Cancellation Success! \n";
    }

    void modify_limit_order(int _id, int _new_price, int _new_quantity){
        Order* target_order = order_bucket.at(_id);

        if (_new_price == target_order->price){

        }
    }

    void submit_market_order();

    ~Orderbook(){
        for (auto& entry : order_bucket){
            delete entry.second;
        }

        buy_tree.destroy_limits();
        sell_tree.destroy_limits();
    }
};
