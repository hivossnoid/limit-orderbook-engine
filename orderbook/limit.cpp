
#pragma once

#include "./order.cpp"

class Limit {
private:
    int limit_price;
    int size;
    int total_volume;

    Order* head_order;
    Order* tail_order;

public:
    Limit(int _price)
        : limit_price(_price),
          size(0),
          total_volume(0),
          head_order(nullptr),
          tail_order(nullptr) {}

    bool is_empty() const {
        return size == 0;
    }

    int get_total_volume() const {
        return total_volume;
    }

    int get_size() const {
        return size;
    }

    int get_limit_price() const {
        return limit_price;
    }

    Order* get_head_order() const {
        return head_order;
    }

    void enlist_order(Order& _order) {
        Order* new_order = &_order;

        new_order->next = nullptr;
        new_order->prev = tail_order;

        if (head_order == nullptr) {
            head_order = new_order;
        }
        else {
            tail_order->next = new_order;
        }

        tail_order = new_order;

        size++;
        total_volume += new_order->quantity;

        std::cout << "Order Enlisting Success!\n";
    }

    void remove_order(Order& _order) {
        Order* target = &_order;

        // Unlink from the doubly linked list.
        if (target->prev != nullptr) {
            target->prev->next = target->next;
        }
        else {
            head_order = target->next;
        }

        if (target->next != nullptr) {
            target->next->prev = target->prev;
        }
        else {
            tail_order = target->prev;
        }

        // Remove the order's remaining volume.
        total_volume -= target->quantity;

        target->next = nullptr;
        target->prev = nullptr;

        size--;

        std::cout << "Order Removed From Limit.\n";
    }

    void modify_order_quantity(Order& _order, int _new_quantity) {
        if (_new_quantity < 0) {
            throw std::invalid_argument("Order quantity cannot be negative.");
        }

        total_volume -= _order.quantity;
        _order.quantity = _new_quantity;
        total_volume += _order.quantity;
    }

    // Call this after a trade has reduced an order's quantity.
    void reduce_order_quantity(Order& _order, int _executed_quantity) {
        if (_executed_quantity < 0 ||
            _executed_quantity > _order.quantity) {
            throw std::invalid_argument("Invalid executed quantity.");
        }

        _order.quantity -= _executed_quantity;
        total_volume -= _executed_quantity;
    }

    void print_orders_at_limit() const {
        Order* temp = head_order;

        if (temp == nullptr) {
            std::cout << "No Orders\n";
            return;
        }

        while (temp != nullptr) {
            std::cout << " {";
            std::cout << " OrderID: " << temp->order_id
                      << ", Order Price: " << temp->price
                      << ", Order Quantity: " << temp->quantity
                      << ", Order Side: " << static_cast<int>(temp->side)
                      << ", Created at: "
                      << std::chrono::system_clock::to_time_t(temp->created_at)
                      << " }\n";

            temp = temp->next;
        }
    }
};
