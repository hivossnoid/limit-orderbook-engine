
#include "./matching_engine.h"

// Constructor
MatchingEngine::MatchingEngine(Orderbook& _orderbook)
    : orderbook(_orderbook) {}

// Private methods

int MatchingEngine::execute_trade(
    Order& buy_order,
    Order& sell_order,
    int exec_price
) {
    int execution_quantity = std::min(
        buy_order.quantity,
        sell_order.quantity
    );

    Trade new_trade(
        buy_order.order_id,
        sell_order.order_id,
        exec_price,
        execution_quantity
    );

    return execution_quantity;
}

bool MatchingEngine::check_price_cross(
    Order& buy_order,
    Order& sell_order
) {
    return buy_order.price >= sell_order.price;
}

// Public methods

void MatchingEngine::match_limit_order(Order& incoming_order) {

    while (incoming_order.quantity > 0) {

        int best_price = 0;
        OrderSide resting_side;

        if (incoming_order.side == OrderSide::buy) {
            best_price = orderbook.get_best_ask();
            resting_side = OrderSide::sell;
        }
        else {
            best_price = orderbook.get_best_bid();
            resting_side = OrderSide::buy;
        }

        if (best_price == 0) break;

        Limit* target_limit =
            orderbook.fetch_limit(resting_side, best_price);

        if (target_limit == nullptr) break;

        Order* resting_order = target_limit->get_head_order();

        if (resting_order == nullptr) break;

        bool crossed;

        if (incoming_order.side == OrderSide::buy) {
            crossed = check_price_cross(incoming_order, *resting_order);
        }
        else {
            crossed = check_price_cross(*resting_order, incoming_order);
        }

        if (!crossed) break;

        int execution_quantity;

        if (incoming_order.side == OrderSide::buy) {
            execution_quantity = execute_trade(
                incoming_order,
                *resting_order,
                resting_order->price
            );
        }
        else {
            execution_quantity = execute_trade(
                *resting_order,
                incoming_order,
                resting_order->price
            );
        }

        // Update both order quantities.
        incoming_order.quantity -= execution_quantity;

        // Update resting order and its limit volume together.
        target_limit->reduce_order_quantity(
            *resting_order,
            execution_quantity
        );

        if (resting_order->quantity == 0) {

            int resting_order_id = resting_order->order_id;

            target_limit->remove_order(*resting_order);

            orderbook.remove_order_from_bucket(resting_order_id);

            if (target_limit->is_empty()) {
                orderbook.remove_limit(resting_side, best_price);
            }
        }
    }

    if (incoming_order.quantity > 0) {
        orderbook.add_order_to_book(incoming_order);
    }
    else {
        orderbook.remove_order_from_bucket(incoming_order.order_id);
    }
}

void MatchingEngine::match_market_order(Order& incoming_order) {

    while (incoming_order.quantity > 0) {

        int best_price = 0;
        OrderSide resting_side;

        if (incoming_order.side == OrderSide::buy) {
            best_price = orderbook.get_best_ask();
            resting_side = OrderSide::sell;
        }
        else {
            best_price = orderbook.get_best_bid();
            resting_side = OrderSide::buy;
        }

        if (best_price == 0) break;

        Limit* target_limit =
            orderbook.fetch_limit(resting_side, best_price);

        if (target_limit == nullptr) break;

        Order* resting_order = target_limit->get_head_order();

        if (resting_order == nullptr) break;

        int execution_quantity;

        if (incoming_order.side == OrderSide::buy) {
            execution_quantity = execute_trade(
                incoming_order,
                *resting_order,
                resting_order->price
            );
        }
        else {
            execution_quantity = execute_trade(
                *resting_order,
                incoming_order,
                resting_order->price
            );
        }

        // Update incoming order quantity.
        incoming_order.quantity -= execution_quantity;

        // Update resting order and limit volume.
        target_limit->reduce_order_quantity(
            *resting_order,
            execution_quantity
        );

        if (resting_order->quantity == 0) {

            int resting_order_id = resting_order->order_id;

            target_limit->remove_order(*resting_order);

            orderbook.remove_order_from_bucket(resting_order_id);

            if (target_limit->is_empty()) {
                orderbook.remove_limit(resting_side, best_price);
            }
        }
    }

    // Market orders cannot rest in the order book.
    orderbook.remove_order_from_bucket(incoming_order.order_id);
}
