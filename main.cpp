
#include <iostream>
#include <chrono>
#include <string>

#include "./orderbook.h"
#include "./matching_engine.h"
#include "./limit.cpp"
#include "./order.cpp"

int main() {

    // =====================================================
    // TEST 1: PARTIAL ORDER FILL
    // =====================================================

    {
        std::cout << "\n========== TEST 1: PARTIAL ORDER FILL ==========\n";

        Orderbook book;

        book.submit_limit_order(100, 10, OrderSide::sell);
        book.submit_limit_order(100, 5, OrderSide::buy);

        std::cout << "\nExpected: Sell 5 remaining at 100.\n";
        book.print_all();
    }

    // =====================================================
    // TEST 2: FULL ORDER FILL
    // =====================================================

    {
        std::cout << "\n========== TEST 2: FULL ORDER FILL ==========\n";

        Orderbook book;

        book.submit_limit_order(100, 5, OrderSide::sell);
        book.submit_limit_order(100, 5, OrderSide::buy);

        std::cout << "\nExpected: Both orders completely consumed.\n";
        book.print_all();
    }


    {
        std::cout << "\n========== TEST 3: FIFO PRIORITY ==========\n";

        Orderbook book;

        book.submit_limit_order(100, 5, OrderSide::sell);
        book.submit_limit_order(100, 5, OrderSide::sell);

        book.submit_limit_order(100, 7, OrderSide::buy);

        std::cout << "\nExpected:\n";
        std::cout << "First sell completely filled.\n";
        std::cout << "Second sell has 3 remaining.\n";

        book.print_all();
    }

    {
        std::cout << "\n========== TEST 4: PRICE PRIORITY ==========\n";

        Orderbook book;

        book.submit_limit_order(105, 10, OrderSide::sell);
        book.submit_limit_order(100, 10, OrderSide::sell);

        book.submit_limit_order(110, 15, OrderSide::buy);

        std::cout << "\nExpected:\n";
        std::cout << "Sell 10 at 100 completely filled.\n";
        std::cout << "Sell 5 at 105 filled.\n";
        std::cout << "Sell 5 at 105 remains.\n";

        book.print_all();
    }


    {
        std::cout << "\n========== TEST 5: MULTIPLE PRICE LEVELS ==========\n";

        Orderbook book;

        book.submit_limit_order(100, 5, OrderSide::sell);
        book.submit_limit_order(105, 5, OrderSide::sell);
        book.submit_limit_order(110, 5, OrderSide::sell);

        book.submit_limit_order(110, 12, OrderSide::buy);

        std::cout << "\nExpected:\n";
        std::cout << "Sell 5 at 100 completely filled.\n";
        std::cout << "Sell 5 at 105 completely filled.\n";
        std::cout << "Sell 2 at 110 filled.\n";
        std::cout << "Sell 3 at 110 remains.\n";

        book.print_all();
    }


    {
        std::cout << "\n========== TEST 6: MARKET BUY ==========\n";

        Orderbook book;

        book.submit_limit_order(100, 5, OrderSide::sell);
        book.submit_limit_order(105, 5, OrderSide::sell);

        book.submit_market_order(8, OrderSide::buy);

        std::cout << "\nExpected:\n";
        std::cout << "Sell 5 at 100 completely filled.\n";
        std::cout << "Sell 3 at 105 filled.\n";
        std::cout << "Sell 2 at 105 remains.\n";

        book.print_all();
    }


    {
        std::cout << "\n========== TEST 7: MARKET SELL ==========\n";

        Orderbook book;

        book.submit_limit_order(105, 5, OrderSide::buy);
        book.submit_limit_order(100, 5, OrderSide::buy);

        book.submit_market_order(8, OrderSide::sell);

        std::cout << "\nExpected:\n";
        std::cout << "Buy 5 at 105 completely filled.\n";
        std::cout << "Buy 3 at 100 filled.\n";
        std::cout << "Buy 2 at 100 remains.\n";

        book.print_all();
    }


    {
        std::cout << "\n========== TEST 8: INSUFFICIENT LIQUIDITY ==========\n";

        Orderbook book;

        book.submit_limit_order(100, 3, OrderSide::sell);

        book.submit_market_order(10, OrderSide::buy);

        std::cout << "\nExpected:\n";
        std::cout << "Sell 3 at 100 completely filled.\n";
        std::cout << "Remaining market quantity discarded.\n";
        std::cout << "Order book empty.\n";

        book.print_all();
    }

    // =====================================================
    // TEST 9: EMPTY ORDER BOOK
    // =====================================================

    {
        std::cout << "\n========== TEST 9: EMPTY ORDER BOOK ==========\n";

        Orderbook book;

        book.submit_market_order(10, OrderSide::buy);
        book.submit_market_order(10, OrderSide::sell);

        std::cout << "\nExpected: No crashes and no resting orders.\n";

        book.print_all();
    }

    std::cout << "\n========== ALL TESTS COMPLETED ==========\n";

    return 0;
}
