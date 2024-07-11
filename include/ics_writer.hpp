#pragma once

#include "data_service.hpp"

#include <fstream>

namespace ics_writer
{
enum session_type {
    PRACTICE_1,
    PRACTICE_2,
    PRACTICE_3,
    SPRINT_QUALIFYING,
    SPRINT,
    QUALIFYING,
    RACE
};


void write_ics_header( std::ofstream& file );
void write_ics_footer( std::ofstream& file );
void write_race_info( std::ofstream& file, const data_service::race_info& info );
void write_session_info( std::ofstream& file,
                         const data_service::race_info& info,
                         ics_writer::session_type type,
                         const time_p& date );
} // namespace ics_writer