#include "./order.cpp"

#pragma once

class Limit{
private:
    int limit_price;
    int size; // number of orders in a limit_price.
    int total_volume; // sum of all quantities.

    Order* head_order; // the first priority in the queue.
    Order* tail_order; // the last priority in the queue.

public:
    Limit(int _price) : limit_price(_price), size(0), total_volume(0), head_order(nullptr), tail_order(nullptr) {}

    bool is_empty(){
        return size == 0;
    }

    void enlist_order(Order& _order){
        Order* new_order = &_order;

        new_order->next = nullptr;
        new_order->prev = tail_order;

        if (head_order == nullptr){
            head_order = new_order;
        } else {
            tail_order->next = new_order;
        }

        tail_order = new_order;

        size += 1;
        total_volume += new_order->quantity;

        std::cout << "Order Enlisting Success! \n";
    }

    void remove_order(Order& _order){
        Order* target = &_order;

        if (target->prev != nullptr){
            target->prev->next = target->next;
        } else {
            head_order = target->next;
        }

        if (target->next != nullptr){
            target->next->prev = target->prev;
        } else {
            tail_order = target->prev;
        }

        target->next = nullptr;
        target->prev = nullptr;

        size -= 1;
        total_volume -= target->quantity;

        std::cout << "Order Removed From Limit. \n";
    }

    void modify_order_quantity(Order& _order, int _new_quantity){

    }


    void print_orders_at_limit(){
        Order* temp = head_order;

        if (temp == nullptr){
            std::cout << "No Orders \n";
            return;
        }

        while (temp != nullptr){
            std::cout << " {";
            std::cout << " " << temp->order_id << " " << temp->price << " ";
            std::cout << temp->quantity << " " << static_cast<int>(temp->side) << " " <<
                std::chrono::system_clock::to_time_t(temp->created_at) << " ";
            std::cout << "} \n";

            temp = temp->next;
        }
    }

};
