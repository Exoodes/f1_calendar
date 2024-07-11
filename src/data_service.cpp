#include "data_service.hpp"

#include <chrono>
#include <curl/curl.h>
#include <format>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


std::chrono::system_clock::time_point parseDateTime( const std::string& date, const std::string& time )
{
    std::tm tm = {};
    std::istringstream ss( date + " " + time );

    ss >> std::get_time( &tm, "%Y-%m-%d %H:%M:%SZ" );

    std::time_t timeT = std::mktime( &tm );

    return std::chrono::system_clock::from_time_t( timeT );
}


void data_service::from_json( const json& j, race_info& info )
{
    j.at( "raceName" ).get_to( info.race_name );
    j.at( "round" ).get_to( info.round );
    j.at( "season" ).get_to( info.year );
    j.at( "url" ).get_to( info.url );

    auto add_to_sessions = [ & ]( const json& j ) {
        info.sessions.push_back( parseDateTime( j.at( "date" ), j.at( "time" ) ) +
                                 std::chrono::hours( 1 ) );
    };

    add_to_sessions( j.at( "FirstPractice" ) );
    add_to_sessions( j.at( "SecondPractice" ) );

    info.is_sprint_weekend = j.contains( "Sprint" );
    info.is_sprint_weekend ? add_to_sessions( j.at( "Sprint" ) ) :
                             add_to_sessions( j.at( "ThirdPractice" ) );

    add_to_sessions( j.at( "Qualifying" ) );
    add_to_sessions( j );

    info.circuit = j.at( "Circuit" ).template get< circuit_info >();
}


void data_service::from_json( const json& j, circuit_info& info )
{
    auto location = j.at( "Location" );
    location.at( "country" ).get_to( info.country );
    location.at( "lat" ).get_to( info.latitute );
    location.at( "locality" ).get_to( info.location );
    location.at( "long" ).get_to( info.longitude );

    j.at( "circuitId" ).get_to( info.circuit_id );
    j.at( "circuitName" ).get_to( info.circuit_name );
    j.at( "url" ).get_to( info.url );
}


std::vector< data_service::race_info > data_service::parse_f1_calendar( const std::string& data )
{
    std::vector< race_info > race_infos;
    json j = json::parse( data );
    std::cout << j.dump( 4 ) << std::endl;

    for ( auto race : j[ "MRData" ][ "RaceTable" ][ "Races" ] )
        race_infos.push_back( race.template get< race_info >() );

    return race_infos;
}


size_t writeCallback( char* ptr, size_t size, size_t nmemb, std::string* data )
{
    data->append( ptr, size * nmemb );
    return size * nmemb;
}


std::string data_service::get_current_f1_calendar()
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init( CURL_GLOBAL_DEFAULT );
    curl = curl_easy_init();
    if ( curl ) {
        curl_easy_setopt( curl, CURLOPT_URL, "http://ergast.com/api/f1/current.json" );
        curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, writeCallback );
        curl_easy_setopt( curl, CURLOPT_WRITEDATA, &readBuffer );

        res = curl_easy_perform( curl );

        if ( res != CURLE_OK )
            throw std::runtime_error( "Failed to fetch data" );

        curl_easy_cleanup( curl );
    }

    curl_global_cleanup();
    return readBuffer;
}
