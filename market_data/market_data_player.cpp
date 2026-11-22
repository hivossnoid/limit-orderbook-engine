#include "market_data_player.h"

#include <stdexcept>
#include <utility>

MarketDataPlayer::MarketDataPlayer(
    std::vector<MarketData> _snapshots
)
    : snapshots(std::move(_snapshots)),
      current_index(0) {
}

bool MarketDataPlayer::has_next() const {
    return current_index < snapshots.size();
}

const MarketData& MarketDataPlayer::next() {
    if (!has_next()) {
        throw std::out_of_range("No more market data snapshots");
    }

    return snapshots[current_index++];
}

void MarketDataPlayer::reset() {
    current_index = 0;
}
