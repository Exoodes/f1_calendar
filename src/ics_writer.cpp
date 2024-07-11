#include "ics_writer.hpp"
#include "data_service.hpp"

#include <chrono>
#include <format>
#include <fstream>
#include <iostream>


std::chrono::hours one_hour( 1 );
std::chrono::hours two_hours( 2 );

std::chrono::hours get_session_length( ics_writer::session_type type )
{
    switch ( type ) {
        case ics_writer::PRACTICE_1:;
        case ics_writer::PRACTICE_2:;
        case ics_writer::PRACTICE_3:;
        case ics_writer::SPRINT_QUALIFYING:;
        case ics_writer::SPRINT:;
        case ics_writer::QUALIFYING: return std::chrono::hours( 1 );
        case ics_writer::RACE: return std::chrono::hours( 2 );
    }
}


std::string get_session_name( ics_writer::session_type type )
{
    switch ( type ) {
        case ics_writer::PRACTICE_1: return "🏎️ Practice 1";
        case ics_writer::PRACTICE_2: return "🏎️ Practice 2";
        case ics_writer::PRACTICE_3: return "🏎️ Practice 3";
        case ics_writer::SPRINT_QUALIFYING: return "⏱️ Sprint Qualifying";
        case ics_writer::SPRINT: return "🏁 Sprint";
        case ics_writer::QUALIFYING: return "⏱️ Qualifying";
        case ics_writer::RACE: return "🏁 Race";
    }
}


void ics_writer::write_ics_header( std::ofstream& file )
{
    file << "BEGIN:VCALENDAR\n";
    file << "VERSION:2.0\n";
    file << "PRODID:-//F1 Calendar// x //EN\n";
    file << "CALSCALE:GREGORIAN\n";
    file << "METHOD:PUBLISH\n";
    file << "X-WR-CALNAME:F1 Calendar\n";
    file << "X-WR-TIMEZONE:UTC\n";
}


void ics_writer::write_ics_footer( std::ofstream& file ) { file << "END:VCALENDAR\n"; }


void ics_writer::write_race_info( std::ofstream& file, const data_service::race_info& info )
{
    std::vector< ics_writer::session_type > classical_sessions = { ics_writer::PRACTICE_1,
                                                                   ics_writer::PRACTICE_2,
                                                                   ics_writer::PRACTICE_2,
                                                                   ics_writer::QUALIFYING,
                                                                   ics_writer::RACE };

    std::vector< ics_writer::session_type > sprint_sessions = { ics_writer::PRACTICE_1,
                                                                ics_writer::SPRINT_QUALIFYING,
                                                                ics_writer::SPRINT,
                                                                ics_writer::QUALIFYING,
                                                                ics_writer::RACE };


    for ( auto i = 0; i < info.sessions.size(); i++ ) {
        write_session_info( file,
                            info,
                            info.is_sprint_weekend ? sprint_sessions[ i ] : classical_sessions[ i ],
                            info.sessions[ i ] );
    }
}


void write_alarm( std::ofstream& file, const int& minutes )
{
    file << "BEGIN:VALARM\n";
    file << "TRIGGER:-P0DT0H" << minutes << "M0S\n";
    file << "ACTION:DISPLAY\n";
    file << "DESCRIPTION:F1 Reminder\n";
    file << "END:VALARM\n";
}


void ics_writer::write_session_info( std::ofstream& file,
                                     const data_service::race_info& info,
                                     ics_writer::session_type type,
                                     const time_p& date )
{
    file << "BEGIN:VEVENT\n";
    file << "UID:" << std::format( "{:%Y%m%dT%H%M00}", date ) << "@f1calendar.com\n";
    file << "DTSTAMP:" << std::format( "{:%Y%m%dT%H%M00Z}", std::chrono::system_clock::now() ) << "\n";
    file << "DTSTART:" << std::format( "{:%Y%m%dT%H%M00Z}", date ) << "\n";
    file << "DTEND:" << std::format( "{:%Y%m%dT%H%M00Z}", date + get_session_length( type ) ) << "\n";
    file << "SUMMARY:" << get_session_name( type ) << " " << info.race_name << "\n";
    file << "DESCRIPTION:"
         << std::format( "{} at the {} circuit in {}, {}\n",
                         get_session_name( type ),
                         info.circuit.circuit_name,
                         info.circuit.location,
                         info.circuit.country );
    file << "LOCATION:" << info.circuit.location << ", " << info.circuit.country << "\n";
    file << "TRANSP:OPAQUE\n";
    file << "CLASS:PUBLIC\n";
    write_alarm( file, 30 );
    file << "END:VEVENT\n";
}
