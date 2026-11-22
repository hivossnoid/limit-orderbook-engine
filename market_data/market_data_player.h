#ifndef MARKET_DATA_PLAYER_H
#define MARKET_DATA_PLAYER_H

#include "market_data.h"
#include <vector>
#include <cstddef>

class MarketDataPlayer {
private:
    std::vector<MarketData> snapshots;
    std::size_t current_index;

public:
    explicit MarketDataPlayer(std::vector<MarketData> _snapshots);

    bool has_next() const;
    const MarketData& next();
    void reset();
};

#endif
