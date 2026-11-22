
#include <iostream>
#include <chrono>
#include <string>

#include "./orderbook/orderbook.h"
#include "./matching_engine/matching_engine.h"
#include "./orderbook/limit.cpp"
#include "./orderbook/order.cpp"
#include "./market_data/market_data_parser.h"
#include "./market_data/market_data_player.h"

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

    {
        std::cout << "\nTEST 10: Trade History\n";

        Orderbook book;

        book.submit_limit_order(100, 3, OrderSide::sell);
        book.submit_limit_order(100, 5, OrderSide::buy);

        book.submit_limit_order(100, 3, OrderSide::sell);

        book.submit_limit_order(106, 3, OrderSide::sell);
        book.submit_limit_order(102, 2, OrderSide::sell);

        book.submit_limit_order(110, 5, OrderSide::buy);

        const auto& history = book.fetch_trade_history();

        std::cout << "Recorded trades: " << history.size() << '\n';

        for (const Trade& trade : history) {
            std::cout << "Buy ID: " << trade.buy_order_id
                      << " | Sell ID: " << trade.sell_order_id
                      << " | Price: " << trade.exec_price
                      << " | Quantity: " << trade.exec_quantity
                      << '\n';
        }
    }

    {
        std::cout << "\nTEST 11: Historical Market Data Parsing\n";

        std::vector<MarketData> data =
            MarketDataParser::load_csv("market_data_test.csv");

        std::cout << "Loaded snapshots: " << data.size() << '\n';

        for (const MarketData& snapshot : data) {
            std::cout << "Bid: " << snapshot.bid_price
                      << " | Ask: " << snapshot.ask_price
                      << " | Bid Volume: " << snapshot.bid_volume
                      << " | Ask Volume: " << snapshot.ask_volume
                      << '\n';
        }
    }

    {
        std::cout << "\nTEST 12: Historical Market Data Playback\n";

        std::vector<MarketData> data =
            MarketDataParser::load_csv("market_data_test.csv");

        MarketDataPlayer player(std::move(data));

        while (player.has_next()) {
            const MarketData& snapshot = player.next();

            std::cout << "Timestamp: "
                      << std::chrono::system_clock::to_time_t(
                             snapshot.timestamp
                         )
                      << " | Bid: " << snapshot.bid_price
                      << " | Ask: " << snapshot.ask_price
                      << " | Bid Volume: " << snapshot.bid_volume
                      << " | Ask Volume: " << snapshot.ask_volume
                      << '\n';
        }
    }
    std::cout << "\n========== ALL TESTS COMPLETED ==========\n";

    return 0;
}
