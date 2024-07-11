#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>


using json = nlohmann::json;
using time_p = std::chrono::system_clock::time_point;

namespace data_service
{
struct circuit_info {
    std::string circuit_name;
    std::string location;
    std::string country;
    std::string url;
    std::string circuit_id;
    std::string latitute;
    std::string longitude;
};


struct race_info {
    std::string race_name;
    std::string round;
    std::string year;
    std::string url;

    std::vector< time_p > sessions;
    bool is_sprint_weekend;

    circuit_info circuit;
};


std::string get_current_f1_calendar();
std::vector< race_info > parse_f1_calendar( const std::string& data );


void from_json( const json& j, race_info& info );
void from_json( const json& j, circuit_info& info );
} // namespace data_service