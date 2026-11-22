
#include "./trade_logger.h"
#include <stdexcept>

TradeLogger::TradeLogger(const std::string& filename)
    : log_file(filename, std::ios::app) {
    if (!log_file.is_open()) {
        throw std::runtime_error("Failed to open trade log file");
    }

    if (log_file.tellp() == 0) {
        log_file << "timestamp,buy_order_id,sell_order_id,price,quantity\n";
    }
}

TradeLogger::~TradeLogger() {
    if (log_file.is_open()) {
        log_file.close();
    }
}

void TradeLogger::log_trade(const Trade& trade) {
    auto timestamp =
        std::chrono::system_clock::to_time_t(trade.executed_at);

    log_file << timestamp << ","
             << trade.buy_order_id << ","
             << trade.sell_order_id << ","
             << trade.exec_price << ","
             << trade.exec_quantity << "\n";

    if (!log_file) {
        throw std::runtime_error("Failed to write trade to log file");
    }
}
