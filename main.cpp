#include "include/data_service.hpp"
#include "include/ics_writer.hpp"

#include <fstream>


int main()
{
    std::string data = data_service::get_current_f1_calendar();
    auto race_infos = data_service::parse_f1_calendar( data );
    std::ofstream file( "f1_calendar.ics" );
    ics_writer::write_ics_header( file );

    for ( const auto& race : race_infos )
        ics_writer::write_race_info( file, race );

    // ics_writer::write_race_info( file, race_infos.back() );
    ics_writer::write_ics_footer( file );

    return 0;
}