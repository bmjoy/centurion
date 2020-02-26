//------------------------------------------------------------------------------
//                     Copyright (c) 2003-2011 Codalogic Ltd.
//
// This file forms part of THE SUPPORTING SOFTWARE of Codalogic's LMX W3C
// Schema to C++ code generator product.  It may not be used independent of
// files legitimately generated and used by the LMX product.  See the LMX
// license agreement for further restrictions on the use of this file.
//
// Users may edit the contents of this file to suit their platform needs
// subject to the aforementioned restrictions remaining in effect on the
// derived work.
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
//
// Documentation:
//      c_soap is the main class in this file.  The other major class is
//      c_soap_fault which is a member of c_soap.  These two classes are
//      templates that have generic base classes that contain the majority of
//      the SOAP functionality.  As such, the contents of this file leads up to
//      the definition of c_soap.
//
// Version: 5.6
//      - Updated to include pointer to s_debug_error in convenience methods.
//
// Version: 5
//      - Updated to use LMX version 5 generated code.
//
// Version: 3.10
//      - Updated code to remove unused parameter warnings.
//
// Version: 3.9
//      - Removed LMX_PDECL from isset_headers() in c_soap template class and
//        pure virtuals in c_soap_base.
//
// Version: 3.8
//      - First inclusion in LMX.
//------------------------------------------------------------------------------

#ifndef LMXSOAP_H
#define LMXSOAP_H

#if defined( _MSC_VER ) && _MSC_VER > 1000
#pragma once
#pragma warning(disable:4786)
#endif

#include <vector>
#include <fstream>

#include "lmxuser.h"

#define LMX_SOAP_DEFINED

namespace lmx {

LMX_PDECL void soap_reenable_ns_map( c_xml_writer & ar_writer );

//------------------------------------------------------------------------------
//                                c_soap_empty
// If a SOAP exchange does not need a header and/or fault type, then this class
// is used to specify the header and/or fault in the instantiation of the SOAP
// template class.
//------------------------------------------------------------------------------

class c_soap_empty
{
private:
    static const s_ns_map ns_map_reader[];

public:
    // In principle we should never call these methods, but it's
    // -not a disaster if we do.
    LMX_PDECL elmx_error marshal( c_xml_writer & LMX_UNUSED_PARAM(ar_writer) ) const
    {
        lmx_assert(0);
        return ELMX_OK;
    }
    LMX_PDECL elmx_error unmarshal( c_xml_reader & ar_reader, const std::string & ar_name );
    static const s_ns_map *ns_map();
};

//------------------------------------------------------------------------------
//                             c_soap_fault_iface
// This fault interface class is the base of the templatized c_soap_fault
// instances and allows the core generic SOAP code to get the c_soap_fault
// information.
//------------------------------------------------------------------------------

class c_soap_fault_iface
{
public:
    LMX_PDECL c_soap_fault_iface() {}
    LMX_PDECL virtual ~c_soap_fault_iface() {}

    // Called by the SOAP code
    LMX_PDECL virtual const tc_qname & get_fault_code() const = 0;
    LMX_PDECL virtual void set_fault_code( const tc_qname & ar_fault_code ) = 0;
    LMX_PDECL virtual const tlmx_unicode_string & get_fault_string() const = 0;
    LMX_PDECL virtual void set_fault_string( const tlmx_unicode_string & ar_fault_string ) = 0;
    LMX_PDECL virtual const tlmx_uri_string & get_fault_actor() const = 0;
    LMX_PDECL virtual void set_fault_actor( const tlmx_uri_string & ar_fault_actor ) = 0;
    LMX_PDECL virtual bool isset_fault_actor() const = 0;
    LMX_PDECL virtual void unset_fault_actor() = 0;
    LMX_PDECL virtual bool isset_details() const = 0;

    LMX_PDECL virtual elmx_error marshal_details( c_xml_writer & ar_writer ) const = 0;
    LMX_PDECL virtual elmx_error unmarshal_detail( c_xml_reader & ar_reader, const std::string & ar_name ) = 0;
};

//------------------------------------------------------------------------------
//                               c_soap_fault
// Captures the details related to a SOAP fault.
//------------------------------------------------------------------------------

template< class Tfault_detail >
class c_soap_fault : public c_soap_fault_iface
{
private:
    bool is_delete_on_destruct_required;
    tc_qname fault_code;
    tlmx_unicode_string fault_string;
    tlmx_uri_string fault_actor;
    bool present_fault_actor;
    std::vector< Tfault_detail * > o_details;

public:
    c_soap_fault() : is_delete_on_destruct_required( true ), present_fault_actor( false )
    {}
    virtual ~c_soap_fault()
    {
        if( is_delete_on_destruct_required )
            for( size_t l_i=0; l_i<o_details.size(); ++l_i )
                delete o_details[l_i];
    }

    virtual void do_delete_on_destruct( bool a_is_delete_on_destruct_required = true )
            { is_delete_on_destruct_required = a_is_delete_on_destruct_required; }

    // Methods for setting and getting the data
    virtual const tc_qname & get_fault_code() const { return fault_code; }
    virtual void set_fault_code( const tc_qname & ar_fault_code )
            { fault_code = ar_fault_code; }

    virtual const tlmx_unicode_string & get_fault_string() const { return fault_string; }
    virtual void set_fault_string( const tlmx_unicode_string & ar_fault_string )
            { fault_string = ar_fault_string; }

    virtual const tlmx_uri_string & get_fault_actor() const { return fault_actor; }
    virtual void set_fault_actor( const tlmx_uri_string & ar_fault_actor )
            { fault_actor = ar_fault_actor; present_fault_actor = true; }
    virtual bool isset_fault_actor() const { return present_fault_actor; }
    virtual void unset_fault_actor() { present_fault_actor = false; }

    // For example - use: details().push_back( my_detail );
    std::vector< Tfault_detail * > &details() { return o_details; }
    const std::vector< Tfault_detail * > &details() const { return o_details; }
    virtual bool isset_details() const { return ! o_details.empty(); }

    // Called by the SOAP code
    virtual elmx_error marshal_details( c_xml_writer & ar_writer ) const
    {
        elmx_error l_error;
        for( size_t l_i=0; l_i<o_details.size(); ++l_i )
            {
            soap_reenable_ns_map( ar_writer );
            if( (l_error = o_details[l_i]->marshal( ar_writer )) != ELMX_OK )
                return l_error;
            }
        return ELMX_OK;
    }
    virtual elmx_error unmarshal_detail( c_xml_reader & ar_reader, const std::string & ar_name )
    {
        elmx_error l_error;
        typename uniq_ptr< Tfault_detail >::type lap_detail( new Tfault_detail );
        if( (l_error = lap_detail->unmarshal( ar_reader, ar_name )) != ELMX_OK )
            return l_error;
        o_details.push_back( lap_detail.get() );
        lap_detail.release();
        return ELMX_OK;
    }
};

//------------------------------------------------------------------------------
//                              c_soap_base
// This class captures all the non-templatised parts of the SOAP processing.
//------------------------------------------------------------------------------

class c_read;

class c_soap_base
{
private:
    // The soap_action parameter is not explicitly used by this class but is
    // -included here as a convenient way to bundle all SOAP transaction
    // -related information together.
    tlmx_uri_string soap_action;

public:
    LMX_PDECL c_soap_base() {}
    LMX_PDECL virtual ~c_soap_base() {}

    // These are the main methods for initiating marshalling and unmarshalling
    LMX_PDECL elmx_error marshal( const char ac_file_name[], s_debug_error * ap_debug_error = LMXNULL ) const
    {
        std::ofstream l_fout( ac_file_name );
        if( ! l_fout.is_open() )
            return ELMX_NO_FILE;
        elmx_error l_error = marshal( l_fout, ap_debug_error );
    #ifndef LMX_NO_REMOVE_FILE_ON_FAILED_MARSHAL
        if( l_error != ELMX_OK )
            LMXSTDCRT::remove( ac_file_name );
    #endif
        return l_error;
    }
    LMX_PDECL elmx_error marshal( std::string * ap_string, s_debug_error * ap_debug_error = LMXNULL ) const;
    LMX_PDECL elmx_error marshal( std::ostream & ar_sos, s_debug_error * ap_debug_error = LMXNULL ) const;
    LMX_PDECL elmx_error unmarshal( const char ac_file_name[], s_debug_error * ap_debug_error = LMXNULL );
    LMX_PDECL elmx_error unmarshal( const std::string & ar_string, s_debug_error * ap_debug_error = LMXNULL );
    LMX_PDECL elmx_error unmarshal( const char * ap_memory, size_t a_memory_size, s_debug_error * ap_debug_error = LMXNULL );
    LMX_PDECL elmx_error unmarshal( c_read & ar_low_level_reader, s_debug_error * ap_debug_error = LMXNULL );

    LMX_PDECL const tlmx_uri_string & get_soap_action() const { return soap_action; }
    LMX_PDECL void set_soap_action( const tlmx_uri_string & ar_soap_action ) { soap_action = ar_soap_action; }

    // Callbacks called by the SOAP code
    virtual bool is_fault() const = 0;
    virtual elmx_error marshal_headers( c_xml_writer & ar_writer ) const = 0;
    virtual elmx_error unmarshal_header( c_xml_reader & ar_reader, const std::string & ar_name ) = 0;
    virtual bool isset_headers() const = 0;
    virtual elmx_error marshal_bodies( c_xml_writer & ar_writer ) const = 0;
    virtual elmx_error unmarshal_body( c_xml_reader & ar_reader, const std::string & ar_name ) = 0;
    virtual const c_soap_fault_iface & get_fault_iface() const = 0;
    virtual c_soap_fault_iface & get_fault_iface() = 0;
    virtual const s_ns_map * get_ns_map() const = 0;
};

//------------------------------------------------------------------------------
//                              c_soap
// The main SOAP class.  Instantiate an instance of this to perform SOAP
// operations.
//------------------------------------------------------------------------------

template< class Tbody, class Theader = c_soap_empty, class Tfault_detail = c_soap_empty >
class c_soap : public c_soap_base
{
private:
    bool is_delete_on_destruct_required;
    std::vector< Theader * > o_headers;
    std::vector< Tbody * > o_bodies;
    c_soap_fault< Tfault_detail > fault;

public:
    c_soap() : is_delete_on_destruct_required( true ) {}
    c_soap( const char ac_file_name[], elmx_error * ap_error, s_debug_error * ap_debug_error = LMXNULL )
        : is_delete_on_destruct_required( true )
        { *ap_error = unmarshal( ac_file_name ); }
    c_soap( const std::string &ar_string, elmx_error * ap_error, s_debug_error * ap_debug_error = LMXNULL )
        : is_delete_on_destruct_required( true )
        { *ap_error = unmarshal( ar_string ); }
    c_soap( const char *ap_memory, size_t a_memory_size, elmx_error * ap_error, s_debug_error * ap_debug_error = LMXNULL )
        : is_delete_on_destruct_required( true )
        { *ap_error = unmarshal( ap_memory, a_memory_size ); }
    c_soap( c_read &ar_low_level_reader, elmx_error * ap_error, s_debug_error * ap_debug_error = LMXNULL )
        : is_delete_on_destruct_required( true )
        { *ap_error = unmarshal( ar_low_level_reader ); }
    virtual ~c_soap()
    {
        if( is_delete_on_destruct_required )
            {
            size_t l_i;
            for( l_i=0; l_i<o_headers.size(); ++l_i )
                delete o_headers[l_i];
            for( l_i=0; l_i<o_bodies.size(); ++l_i )
                delete o_bodies[l_i];
            }
    }
    void do_delete_on_destruct( bool a_is_delete_on_destruct_required = true )
    {
        is_delete_on_destruct_required = a_is_delete_on_destruct_required;
    }

    // Methods for setting and getting the data
    // For example - use: headers().push_back( p_my_header );
    // Or: get_fault().details().push_back( p_my_detail );
    const std::vector< Theader * > &headers() const { return o_headers; }
    std::vector< Theader * > &headers() { return o_headers; }
    const std::vector< Tbody * > &bodies() const { return o_bodies; }
    std::vector< Tbody * > &bodies() { return o_bodies; }
    const c_soap_fault< Tfault_detail > &get_fault() const { return fault; }
    c_soap_fault< Tfault_detail > &get_fault() { return fault; }

    // These methods indicate whether unmarshalled data is an envelope or fault
    virtual bool is_fault() const { return ! fault.get_fault_code().empty(); }  // Presence of faultCode implies its a fault message

    // The following methods are in the base class and are the main methods for
    // -initiating marshalling
    // elmx_error marshal( const char ac_file_name[] ) const;
    // elmx_error marshal( std::string * ap_string ) const;
    // elmx_error marshal( std::ostream & ar_sos ) const;

    // The following methods are in the base class and are the main methods for
    // -initiating unmarshalling
    // elmx_error unmarshal( const char ac_file_name[] );
    // elmx_error unmarshal( const std::string & ar_string );
    // elmx_error unmarshal( const char * ap_memory, size_t a_memory_size );
    // elmx_error unmarshal( c_read & ar_low_level_reader );

    // The following convenience methods are in the base class
    // const tlmx_uri_string & get_soap_action() const;
    // void set_soap_action( const tlmx_uri_string & ar_soap_action );

    // Callbacks called by the SOAP code
    virtual elmx_error marshal_headers( c_xml_writer & ar_writer ) const
    {
        elmx_error l_error;
        for( size_t l_i=0; l_i<o_headers.size(); ++l_i )
            {
            soap_reenable_ns_map( ar_writer );
            if( (l_error = o_headers[l_i]->marshal( ar_writer )) != ELMX_OK )
                return l_error;
            }
        return ELMX_OK;
    }
    virtual elmx_error unmarshal_header( c_xml_reader & ar_reader, const std::string & ar_name )
    {
        // Called each time a header needs to be unmarshalled
        elmx_error l_error;
        typename uniq_ptr< Theader >::type lap_header( new Theader );
        if( (l_error = lap_header->unmarshal( ar_reader, ar_name )) != ELMX_OK )
            return l_error;
        o_headers.push_back( lap_header.get() );
        lap_header.release();
        return ELMX_OK;
    }
    virtual bool isset_headers() const { return ! o_headers.empty(); }
    virtual elmx_error marshal_bodies( c_xml_writer & ar_writer ) const
    {
        elmx_error l_error;
        for( size_t l_i=0; l_i<o_bodies.size(); ++l_i )
            {
            soap_reenable_ns_map( ar_writer );
            if( (l_error = o_bodies[l_i]->marshal( ar_writer )) != ELMX_OK )
                return l_error;
            }
        return ELMX_OK;
    }
    virtual elmx_error unmarshal_body( c_xml_reader & ar_reader, const std::string & ar_name )
    {
        // Called each time a body needs to be unmarshalled
        elmx_error l_error;
        typename uniq_ptr< Tbody >::type lap_body( new Tbody );
        if( (l_error = lap_body->unmarshal( ar_reader, ar_name )) != ELMX_OK )
            return l_error;
        o_bodies.push_back( lap_body.get() );
        lap_body.release();
        return ELMX_OK;
    }

    // VC6 does not like virtual methods to have different return types so we use
    // -these special forms instead of making get_fault() virtual.
    virtual const c_soap_fault_iface & get_fault_iface() const { return fault; }
    virtual c_soap_fault_iface & get_fault_iface() { return fault; }
    virtual const s_ns_map * get_ns_map() const { return Tbody::ns_map(); }
};

//------------------------------------------------------------------------------
//                          c_soap_no_header
// A convenience template for when the SOAP exchange does not have a header.
//------------------------------------------------------------------------------

template< class Tbody, class Tfault_detail = c_soap_empty >
class c_soap_no_header : public c_soap< Tbody, c_soap_empty, Tfault_detail >
{
public:
    c_soap_no_header() : c_soap< Tbody, c_soap_empty, Tfault_detail >() {}
    c_soap_no_header( const char ac_file_name[], elmx_error * ap_error ) :
            c_soap< Tbody, c_soap_empty, Tfault_detail >( ac_file_name, ap_error ) {}
    c_soap_no_header( const std::string & ar_string, elmx_error * ap_error ) :
            c_soap< Tbody, c_soap_empty, Tfault_detail >( ar_string, ap_error ) {}
    c_soap_no_header( const char * ap_memory, size_t a_memory_size, elmx_error * ap_error ) :
            c_soap< Tbody, c_soap_empty, Tfault_detail >( ap_memory, a_memory_size, ap_error ) {}
    c_soap_no_header( c_read & ar_low_level_reader, elmx_error * ap_error ) :
            c_soap< Tbody, c_soap_empty, Tfault_detail >( ar_low_level_reader, ap_error ) {}
};

}   // End of namespace lmx

#endif // LMXSOAP_H

//------------------------------------------------------------------------------
//                     Copyright (c) 2003-2011 Codalogic Ltd.
//------------------------------------------------------------------------------
