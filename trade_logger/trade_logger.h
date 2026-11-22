#ifndef TRADE_LOGGER_H
#define TRADE_LOGGER_H

#include "../trade/trade.h"
#include <string>
#include <fstream>

class TradeLogger {
private:
    std::ofstream log_file;

public:
    explicit TradeLogger(const std::string& filename);
    ~TradeLogger();

    void log_trade(const Trade& trade);
};

#endif
