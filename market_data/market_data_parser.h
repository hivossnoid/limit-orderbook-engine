

#ifndef MARKET_DATA_PARSER_H
#define MARKET_DATA_PARSER_H

#include "market_data.h"
#include <string>
#include <vector>

class MarketDataParser {
public:
    static std::vector<MarketData> load_csv(
        const std::string& filename
    );
};

#endif
