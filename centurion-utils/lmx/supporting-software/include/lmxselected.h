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

#ifndef LMXSELECTED_H
#define LMXSELECTED_H

#if defined( _MSC_VER ) && _MSC_VER > 1000
#pragma once
#pragma warning(disable:4786)
#endif

#if defined( _MSC_VER ) && _MSC_VER >= 1400
#pragma warning (push)
#pragma warning (disable:4996)  // Disable VC8 'safe' warnings
#endif

#include "lmxparse.h"

namespace lmx {

typedef struct s_elements_to_select
{
    const char *p_namespace;
    const char *p_local_name;
} tc_elements_to_select[];    // Last member marked by NULL values

class c_selected_element
{
public:
    enum elmx_unmarshal_select_result { E_STOP, E_PARSED, E_SKIP };

    virtual elmx_unmarshal_select_result element_found(
        c_xml_reader & ar_reader,
        elmx_error *ap_error,
        const std::string &ar_sought_namespace,
        const std::string &ar_sought_local_name,
        const std::string &ar_full_element_name ) = 0;
};

bool /*is_complete*/ unmarshal_select(
        c_xml_reader & ar_reader,
        elmx_error * ap_error,
        const s_ns_map ac_ns_map[],
        tc_elements_to_select ac_elements_to_select,    // Set members to NULL in final entry
        c_selected_element * ap_handler );

bool /*is_complete*/ unmarshal_select(
        elmx_error * ap_error,
        const char ac_file_name[],
        const s_ns_map ac_ns_map[],
        tc_elements_to_select ac_elements_to_select,    // Set members to NULL in final entry
        c_selected_element * ap_handler );

#if LMX_WANT_WIDE_FILE_NAMES == 1
bool /*is_complete*/ unmarshal_select(
        elmx_error * ap_error,
        const wchar_t ac_file_name[],
        const s_ns_map ac_ns_map[],
        tc_elements_to_select ac_elements_to_select,    // Set members to NULL in final entry
        c_selected_element * ap_handler );
#endif

bool /*is_complete*/ unmarshal_select(
        elmx_error * ap_error,
        const char * ap_memory,
        size_t a_memory_size,
        const s_ns_map ac_ns_map[],
        tc_elements_to_select ac_elements_to_select,    // Set members to NULL in final entry
        c_selected_element * ap_handler );

bool /*is_complete*/ unmarshal_select(
        elmx_error * ap_error,
        const std::string & ar_string,
        const s_ns_map ac_ns_map[],
        tc_elements_to_select ac_elements_to_select,    // Set members to NULL in final entry
        c_selected_element * ap_handler );

} // End of namespace lmx

#if defined( _MSC_VER ) && _MSC_VER >= 1400
#pragma warning (pop)  // Restore VC8 'safe' warnings
#endif

#endif // LMXSELECTED_H

//------------------------------------------------------------------------------
//                   Copyright (c) 2003-2011 Codalogic Ltd.
//------------------------------------------------------------------------------
