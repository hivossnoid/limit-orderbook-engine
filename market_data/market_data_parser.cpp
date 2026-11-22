

#include "market_data_parser.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

std::vector<MarketData> MarketDataParser::load_csv(
    const std::string& filename
) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open market data CSV");
    }

    std::vector<MarketData> market_data;
    std::string line;

    // Skip CSV header
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);
        std::string timestamp;
        std::string bid_price;
        std::string ask_price;
        std::string bid_volume;
        std::string ask_volume;

        std::getline(ss, timestamp, ',');
        std::getline(ss, bid_price, ',');
        std::getline(ss, ask_price, ',');
        std::getline(ss, bid_volume, ',');
        std::getline(ss, ask_volume, ',');

        MarketData snapshot;

        snapshot.timestamp =
            std::chrono::system_clock::from_time_t(
                std::stoll(timestamp)
            );

        snapshot.bid_price = std::stoi(bid_price);
        snapshot.ask_price = std::stoi(ask_price);
        snapshot.bid_volume = std::stoi(bid_volume);
        snapshot.ask_volume = std::stoi(ask_volume);

        market_data.push_back(snapshot);
    }

    return market_data;
}
