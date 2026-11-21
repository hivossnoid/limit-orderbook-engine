
#include "./orderbook.h"
#include "./matching_engine.h"

// Constructor
Orderbook::Orderbook()
    : matching_engine(std::make_unique<MatchingEngine>(*this)) {}

// Private methods

Order* Orderbook::build_order(int _price, int _quantity, OrderSide _side) {
    int order_id = generateID();

    Order* new_order = new Order(order_id, _price, _quantity, _side);
    order_bucket.insert({order_id, new_order});

    std::cout << "Order Building Success! \n";
    return new_order;
}

int Orderbook::fetch_maxval() {
    return buy_tree.max_val();
}

int Orderbook::fetch_minval() {
    return sell_tree.min_val();
}

// Public methods

int Orderbook::get_best_bid() {
    return fetch_maxval();
}

int Orderbook::get_best_ask() {
    return fetch_minval();
}

Limit* Orderbook::fetch_limit(OrderSide _side, int _price) {
    if (_side == OrderSide::buy) {
        return buy_tree.search_limit(_price);
    }
    else if (_side == OrderSide::sell) {
        return sell_tree.search_limit(_price);
    }

    return nullptr;
}

Order* Orderbook::fetch_order(int _id) {
    auto it = order_bucket.find(_id);

    if (it == order_bucket.end()) {
        std::cout << "Order not found.\n";
        return nullptr;
    }

    Order* temp = it->second;

    std::cout << "Order ID: " << temp->order_id
              << " Price: " << temp->price
              << " Quantity: " << temp->quantity
              << " Side: " << static_cast<int>(temp->side)
              << " Timestamp: "
              << std::chrono::system_clock::to_time_t(temp->created_at)
              << "\n";

    return temp;
}

void Orderbook::remove_order_from_bucket(int _id) {
    auto it = order_bucket.find(_id);

    if (it != order_bucket.end()) {
        delete it->second;
        order_bucket.erase(it);
    }
}

void Orderbook::remove_limit(OrderSide _side, int _price) {
    if (_side == OrderSide::buy) {
        Limit* target_limit = buy_tree.search_limit(_price);

        buy_tree.delete_value(_price);
        delete target_limit;
    }
    else if (_side == OrderSide::sell) {
        Limit* target_limit = sell_tree.search_limit(_price);

        sell_tree.delete_value(_price);
        delete target_limit;
    }
}

void Orderbook::add_order_to_book(Order& new_order) {
    Limit* target_limit = fetch_limit(new_order.side, new_order.price);

    if (target_limit == nullptr) {
        target_limit = new Limit(new_order.price);

        if (new_order.side == OrderSide::buy) {
            buy_tree.insert_value(new_order.price, target_limit);
        }
        else if (new_order.side == OrderSide::sell) {
            sell_tree.insert_value(new_order.price, target_limit);
        }
    }

    target_limit->enlist_order(new_order);

    std::cout << "Remaining Unfilled Orders Successfully Enlisted! \n";
}

void Orderbook::submit_limit_order(
    int _price,
    int _quantity,
    OrderSide _side
) {
    Order* new_order = build_order(_price, _quantity, _side);
    fetch_order(new_order->order_id);

    matching_engine->match_limit_order(*new_order);
    if (_side == OrderSide::buy) {
        Redblacktree& tree = buy_tree;

        Limit* target_limit = tree.search_limit(_price);

        if (target_limit == nullptr) {
            target_limit = new Limit(_price);
            tree.insert_value(_price, target_limit);
        }
    }
    else if (_side == OrderSide::sell) {
        Redblacktree& tree = sell_tree;

        Limit* target_limit = tree.search_limit(_price);

        if (target_limit == nullptr) {
            target_limit = new Limit(_price);
            tree.insert_value(_price, target_limit);
        }
    }
    std::cout << "Order Submission Success! \n";
}

void Orderbook::cancel_limit_order(int _id) {
    auto it = order_bucket.find(_id);

    if (it == order_bucket.end()) {
        std::cout << "Order Not Found! Cancellation Failed! \n";
        return;
    }

    Order* target_order = it->second;

    if (target_order->side == OrderSide::buy) {
        Limit* target_limit = buy_tree.search_limit(target_order->price);
        target_limit->remove_order(*target_order);
    }
    else if (target_order->side == OrderSide::sell) {
        Limit* target_limit = sell_tree.search_limit(target_order->price);
        target_limit->remove_order(*target_order);
    }

    order_bucket.erase(_id);
    delete target_order;

    std::cout << "Order Cancellation Success! \n";
}

void Orderbook::modify_limit_order(
    int _id,
    int _new_price,
    int _new_quantity
) {
    auto iterator = order_bucket.find(_id);

    if (iterator == order_bucket.end()) {
        std::cout << "Order Not Found! Modification Process Failed! \n";
        return;
    }

    Order* target_order = iterator->second;

    if (_new_price == target_order->price) {
        Limit* target_limit = fetch_limit(
            target_order->side,
            target_order->price
        );

        target_limit->modify_order_quantity(
            *target_order,
            _new_quantity
        );

        std::cout << "Order Modification Success! \n";
    }
    else {
        Redblacktree& tree =
            (target_order->side == OrderSide::buy)
            ? buy_tree
            : sell_tree;

        Limit* old_limit = tree.search_limit(target_order->price);

        old_limit->remove_order(*target_order);

        if (old_limit->is_empty()) {
            tree.delete_value(target_order->price);
            delete old_limit;
        }

        target_order->price = _new_price;
        target_order->quantity = _new_quantity;

        Limit* new_limit = tree.search_limit(_new_price);

        if (new_limit == nullptr) {
            new_limit = new Limit(_new_price);
            tree.insert_value(_new_price, new_limit);
        }

        new_limit->enlist_order(*target_order);

        std::cout << "Order Modification Success! \n";
    }
}

void Orderbook::submit_market_order(int _quantity, OrderSide _side) {
    Order* new_order = build_order(0, _quantity, _side);

    matching_engine->match_market_order(*new_order);
}

void Orderbook::print_all(){

    std::cout << " Printing All Existing Orders \n";
    auto it = order_bucket.begin();
    for (; it != order_bucket.end(); it++){
        Order* temp = it->second;

        std::cout << "  { Order ID: " << temp->order_id
            << " Price: " << temp->price
            << " Quantity: " << temp->quantity
            << " Side: " << static_cast<int>(temp->side)
            << " Timestamp: "
            << std::chrono::system_clock::to_time_t(temp->created_at)
            << " }  "
            << "\n";
    }
}

void Orderbook::bucket_cleanup(){
    auto it = order_bucket.begin();
    for (; it != order_bucket.end();){
        Order* temp = it->second;
        if (temp->quantity == 0){
            order_bucket.erase(it);
        }
        it++;
    }
}

// Destructor
Orderbook::~Orderbook() {
    std::cout << "1. Destroying buy limits\n";
    buy_tree.destroy_limits();

    std::cout << "2. Destroying sell limits\n";
    sell_tree.destroy_limits();

    std::cout << "3. Destroying orders\n";
    for (auto& entry : order_bucket) {
        std::cout << "Deleting order: " << entry.first << '\n';
        delete entry.second;
    }

    std::cout << "4. Orderbook cleanup complete\n";
}
