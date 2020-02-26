//                 Copyright (c) 2020 Codalogic Ltd.
//
// You are permitted to modify this file as part of your evaluation of LMX.
// See the LMX license agreement for further restrictions on the use of this file.

// THIS FILE AND THE SOFTWARE CONTAINED HEREIN IS PROVIDED "AS IS", WITHOUT
// WARRANTY OF ANY KIND.
//
// CODALOGIC LTD SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY ANYONE
// OR ANYTHING DUE TO THE USE OF THIS FILE HOWEVER THEY MAY BE CAUSED.
//
// For more information on the LMX compiler, go to:
// 
//     http://www.xml2cpp.com
//
// Also see:
//
//     http://www.codalogic.com
//

#include "params.h"
#include "lmxparse.h"
#include <fstream>
#include <sstream>
#include <iostream>

int test_it( const char file_in[], const char file_out[] )
{
    // Allocate somewhere to store any unmarshalling error code
    lmx::elmx_error lmx_error;
    lmx::s_debug_error lmx_error_detail;        // lmx_error_detail is an optional argument

    // Allocate an object and unmarshal the contents of file_in to it.
    //     We want the option to modify the unmarshalled data, so we don't make 
    //     the object 'const'.  If we were only reading from the object it would 
    //     be better to do (assuming we don't want to populate lmx_error_detail):
    //         const c_Params item( file_in, &lmx_error );
    c_Params item( file_in, &lmx_error, &lmx_error_detail );        // lmx_error_detail is an optional argument

    // Check if everything went OK
    if( lmx_error != lmx::ELMX_OK )
    {
        std::cout << "Unable to unmarshal " << file_in << "\n";

        std::cout << lmx_error_detail;
        // Or:
        LMX_OUTPUT_DEBUG_ERROR( std::cout );    // Only outputs if LMX_DEBUG_CAPTURE_ERROR == 1
        // Or:
        #if defined( LMX_DEBUG_CAPTURE_ERROR ) && LMX_DEBUG_CAPTURE_ERROR == 1
            std::cout << lmx::debug_error;
            // Or:
            std::string lmx_error_detail_message( lmx::debug_error.to_string() );
        #endif

        return 1;
    }

    else
    {
        // TODO - Add code to print and/or manipulate the unmarshalled data.
        // See the documentation generated in C:\Users\utente\Documents\github rattlesmake\centurion\centurion-utils\lmx\output\params.html
        // for help creating code.
        // (Try file:C:\Users\utente\Documents\github%20rattlesmake\centurion\centurion-utils\lmx\output\params.html)

        // Marshal the modified data out to file.
        if( item.marshal( file_out ) != lmx::ELMX_OK )
            return 2;
    }

    return 0;

    // Also: If we had:
    //      std::string my_string;
    // we could have done:
    //      item.unmarshal( my_string );
    //      item.marshal( & my_string );
    //
    // There are also other ways to marshal and unmarshal data which are more flexible
    // than this.  See the documentation for more information.
}

int main( int argc, char * argv[] )
{
    int result = 0;

    if( argc >= 3 )
        result = test_it( argv[1], argv[2] );
    else
        std::cout << "Insufficient program arguments\nUsage: trylmx in_file.xml out_file.xml\n";

    return result;
}
