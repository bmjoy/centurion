//------------------------------------------------------------------------------
//                   Copyright (c) 2003-2011 Codalogic Ltd.
//
// This file forms part of THE SUPPORTING SOFTWARE of Codalogic's LMX W3C
// Schema to C++ code generator product.  It may not be used independent of
// files legitimately generated and used by the LMX product.  See the LMX
// licence agreement for further restrictions on the use of this file.
//
// THIS FILE AND THE SOFTWARE CONTAINED HEREIN IS PROVIDED 'AS IS' AND COMES
// WITH NO WARRANTIES OF ANY KIND.
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
//------------------------------------------------------------------------------

#include "lmxselected.h"

namespace lmx {

bool /*continue*/ p_unmarshal_select(
        c_xml_reader & ar_reader,
        elmx_error * ap_error,
        tc_elements_to_select ac_elements_to_select,
        c_selected_element * ap_handler,
        const std::string & ar_element_name )
{
    std::string l_element_namespace;
    std::string l_element_local_name;
    ar_reader.get_namespace( ar_element_name, &l_element_namespace );
    ar_reader.get_local_name( ar_element_name, &l_element_local_name );
    for( size_t l_i=0; ac_elements_to_select[l_i].p_local_name; ++l_i )
        {
        if( l_element_local_name == ac_elements_to_select[l_i].p_local_name &&
                l_element_namespace == ac_elements_to_select[l_i].p_namespace )
            {
            c_selected_element::elmx_unmarshal_select_result l_result =
                    ap_handler->element_found(
                            ar_reader,
                            ap_error,
                            l_element_namespace,
                            l_element_local_name,
                            ar_element_name );
            if( *ap_error != ELMX_OK || l_result == c_selected_element::E_STOP )
                return false;
            else if( l_result == c_selected_element::E_PARSED )
                return true;
            break;
            }
        }

    bool l_is_end_of_element;

    if( ! ar_reader.skip_start_tag( &l_is_end_of_element ) )
        {
        *ap_error = ELMX_BAD_END_OF_START_TAG;
        return false;
        }

    if( l_is_end_of_element )
        return true;

    std::string l_name;

    for(;;)
        {
        c_xml_reader_local l_reader_local( &ar_reader );
        ar_reader.get_element_event( NULL, ap_error, &l_name );

        if( *ap_error != ELMX_OK )
            return false;
        else if( ar_reader.get_current_event() == lmx::EXE_TOKEN )
            {
            if( ! p_unmarshal_select(
                    ar_reader,
                    ap_error,
                    ac_elements_to_select,
                    ap_handler,
                    l_name ) )
                return false;
            else if( *ap_error != ELMX_OK )
                return false;
            }
        else if( ar_reader.get_current_event() == EXE_ELEMENT_END )
            {
            if( l_name != ar_element_name )
                {
                *ap_error = ELMX_BAD_ELEMENT_END;
                return false;
                }
            return true;
            }
        else if( ar_reader.get_current_event() == EXE_CDATA )
            {}  // No action
        else
            {
            *ap_error = ELMX_BADLY_FORMED;
            return false;
            }
        }

    lmx_assert_code_not_reached();  // Shouldn't get to here
    return false;
}

bool /*is_complete*/ unmarshal_select(
        c_xml_reader & ar_reader,
        elmx_error * ap_error,
        const s_ns_map ac_ns_map[],
        tc_elements_to_select ac_elements_to_select,    // Set members to NULL in final entry
        c_selected_element * ap_handler )
{
    ar_reader.set_ns_map( ac_ns_map );
    c_xml_reader_local l_reader_local( &ar_reader );
    std::string l_name;
    ar_reader.get_element_event( NULL, ap_error, &l_name );
    if( ar_reader.get_current_event() != EXE_TOKEN )
        return false;
    *ap_error = ELMX_OK;
    return p_unmarshal_select(
                ar_reader,
                ap_error,
                ac_elements_to_select,
                ap_handler,
                l_name );
}

bool /*is_complete*/ unmarshal_select(
        elmx_error * ap_error,
        const char ac_file_name[],
        const s_ns_map ac_ns_map[],
        tc_elements_to_select ac_elements_to_select,    // Set members to NULL in final entry
        c_selected_element * ap_handler )
{
    c_xml_reader_file l_reader( ac_file_name );
    if( ! l_reader.is_open() )
        return ELMX_NO_FILE;
    return unmarshal_select( l_reader, ap_error, ac_ns_map, ac_elements_to_select, ap_handler );
}

#if LMX_WANT_WIDE_FILE_NAMES == 1
bool /*is_complete*/ unmarshal_select(
        elmx_error * ap_error,
        const wchar_t ac_file_name[],
        const s_ns_map ac_ns_map[],
        tc_elements_to_select ac_elements_to_select,    // Set members to NULL in final entry
        c_selected_element * ap_handler )
{
    c_xml_reader_file l_reader( ac_file_name );
    if( ! l_reader.is_open() )
        return ELMX_NO_FILE;
    return unmarshal_select( l_reader, ap_error, ac_ns_map, ac_elements_to_select, ap_handler );
}
#endif

bool /*is_complete*/ unmarshal_select(
        elmx_error * ap_error,
        const char * ap_memory,
        size_t a_memory_size,
        const s_ns_map ac_ns_map[],
        tc_elements_to_select ac_elements_to_select,    // Set members to NULL in final entry
        c_selected_element * ap_handler )
{
    c_xml_reader_memory l_reader( ap_memory, a_memory_size );
    return unmarshal_select( l_reader, ap_error, ac_ns_map, ac_elements_to_select, ap_handler );
}

bool /*is_complete*/ unmarshal_select(
        elmx_error * ap_error,
        const std::string & ar_string,
        const s_ns_map ac_ns_map[],
        tc_elements_to_select ac_elements_to_select,    // Set members to NULL in final entry
        c_selected_element * ap_handler )
{
    return unmarshal_select( ap_error, ar_string.data(), ar_string.size(), ac_ns_map, ac_elements_to_select, ap_handler );
}

} // End of namespace lmx

//------------------------------------------------------------------------------
//                   Copyright (c) 2003-2011 Codalogic Ltd.
//------------------------------------------------------------------------------
