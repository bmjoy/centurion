//------------------------------------------------------------------------------
//                   Copyright (c) 2003-2011 Codalogic Ltd.
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
//      This file consists of a number of separately generated files
//      concatenated together.  The intent is that this should make the
//      supporting software source code easier to compile.
//
// Version: 5.6
//      - Updated to include pointer to s_debug_error in convenience methods.
//
// Version: 5
//      - Regenerated with LMX v5.
//      - Added warning(disable:4786) for VC6.
//
// Version: 3.10
//      - Updated code to remove unused parameter warnings.
//
// Version: 3.8
//      - First inclusion in LMX.
//------------------------------------------------------------------------------

#if defined( _MSC_VER ) && _MSC_VER > 1000
#pragma once
#pragma warning(disable:4786)
#endif

#include <algorithm>
#include <memory>
#ifndef LMX_PARSE_SOURCE
#define LMX_PARSE_SOURCE
#endif

#include "lmxsoap.h"
#include "lmxparse.h"

// None of the code will be exported for this file
#undef LMX_GDECL
#define LMX_GDECL

namespace lmx {

//------------------------------------------------------------------------------
// Enhanced reader and writer classes containing the link to the SOAP classes.
//------------------------------------------------------------------------------

class c_xml_writer_soap : public c_xml_writer
{
private:
    const c_soap_base & r_soap_base;

public:
    c_xml_writer_soap( const c_soap_base & ar_soap_base, std::ostream & ar_os, t_writer_options a_options ) :
            c_xml_writer( ar_os, a_options ),
            r_soap_base( ar_soap_base )
    {}
    const c_soap_base & get_soap_base() const { return r_soap_base; }
};

class c_xml_reader_soap : public c_xml_reader
{
private:
    c_soap_base & r_soap_base;

public:
    c_xml_reader_soap( c_soap_base & ar_soap_base, c_read & ar_low_reader ) :
            c_xml_reader( ar_low_reader ),
            r_soap_base( ar_soap_base )
    {}
    const c_soap_base & get_soap_base() const { return r_soap_base; }
    c_soap_base & get_soap_base() { return r_soap_base; }
};

}  // end of namespace lmx

#if defined( _MSC_VER ) && _MSC_VER >= 1400
#pragma warning (push)
#pragma warning (disable:4100)  // Disable unreferenced formal parameter
#endif


// To help MS intellisense
#ifndef LMX_GDECL
#define LMX_GDECL
#endif

namespace lmx {

LMX_GDECL extern const lmx::s_ns_map ns_map_reader[];
LMX_GDECL extern const lmx::s_ns_map ns_map_writer_1[];

enum
{
    e_error = lmx::EXE_LAST_XML_EVENT,
    e_NS7_Envelope,
    e_NS7_Header,
    e_NS7_Body,
    e_NS7_Fault,
    e_NONE_faultcode,
    e_NONE_faultstring,
    e_NONE_faultactor,
    e_NONE_detail,
    e_any
};

enum elmx_enums
{
    e_e_unknown
};

// From: lmxsoap-env.xsd(56)
class c_Header
{
    //:snippet:start Header
    elmx_error on_lmx_marshal_body_start( c_xml_writer_soap &ar_writer ) const;
    elmx_error on_lmx_unmarshal_body_start( c_xml_reader_soap &ar_reader );
    //:snippet:end Header

private:
    // Attribute(s)
    lmx::t_any_info_container any_attribute;

    // Element(s)
    lmx::ct_simple_non_pod_multi< lmx::c_any_info > m_any;

    static const lmx::s_event_map attr_event_map[];
    static const lmx::s_event_map elem_event_map[];

public:
    LMX_GDECL c_Header();
    LMX_GDECL c_Header( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error );
    LMX_GDECL virtual ~c_Header();

    // Attribute(s)
    LMX_GDECL lmx::t_any_info_container & getany_attributes() { return any_attribute; }
    LMX_GDECL void getany_attribute( size_t a_index, std::string * ap_namespace, std::string * ap_name, std::string * ap_value ) const;
    LMX_GDECL size_t sizeany_attribute() const { return any_attribute.size(); }
    LMX_GDECL void appendany_attribute( const std::string & ar_name, const std::string & ar_value ) { any_attribute.push_back_w_autop( new lmx::c_any_info( ar_name, ar_value ) ); }
    LMX_GDECL void insertany_attribute( size_t a_index, const std::string & ar_name, const std::string & ar_value ) { any_attribute.insert_w_autop( a_index, new lmx::c_any_info( ar_name, ar_value ) ); }
    LMX_GDECL void deleteany_attribute( size_t a_index ) { any_attribute.erase( a_index ); }
    LMX_GDECL void clearany_attribute() { any_attribute.clear(); }
    // Element(s)

    //    {any} --> xs:any[0..*]
    LMX_GDECL lmx::elmx_error append_any( const std::string & ar_value )
    {
        m_any.append( lmx::c_any_info( ar_value ) );
        return lmx::ELMX_OK;
    }
    LMX_GDECL lmx::elmx_error insert_any( size_t a_index, const std::string & ar_value )
    {
        m_any.insert( a_index, lmx::c_any_info( ar_value ) );
        return lmx::ELMX_OK;
    }
    LMX_GDECL void delete_any( size_t a_index ) { m_any.erase( a_index ); }
    LMX_GDECL void clear_any() { m_any.clear(); }
    LMX_GDECL void get_any( size_t a_index, std::string * ap_namespace, std::string * ap_name, std::string * ap_value ) const
    {
        if( ap_namespace ) *ap_namespace = m_any.get( a_index ).get_namespace();
        if( ap_name ) *ap_name = m_any.get( a_index ).get_name();
        *ap_value = m_any.get( a_index ).get_value();
    }
    LMX_GDECL const lmx::c_any_info & get_any( size_t a_index ) const { return  m_any.get( a_index ); }
    LMX_GDECL lmx::c_any_info & get_any( size_t a_index ) { return  m_any.get( a_index ); }
    LMX_GDECL size_t size_any() const { return m_any.size(); }

    LMX_GDECL bool is_occurs_ok() const { return true; }

    LMX_GDECL static const lmx::s_ns_map *ns_map() { return ns_map_reader; }
    // General marshal/unmarshal functions
    LMX_GDECL lmx::elmx_error marshal( c_xml_writer_soap & ar_writer, const char * ap_name = "SOAP-ENV:Header" ) const;
    LMX_GDECL lmx::elmx_error unmarshal( c_xml_reader_soap & ar_reader );
    LMX_GDECL lmx::elmx_error unmarshal( c_xml_reader_soap & ar_reader, const std::string & ar_name );

protected:
    LMX_GDECL void marshal_attributes( c_xml_writer_soap & ar_writer ) const;
    LMX_GDECL lmx::elmx_error marshal_child_elements( c_xml_writer_soap & ar_writer ) const;
    LMX_GDECL bool unmarshal_attributes( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error );
    LMX_GDECL lmx::elmx_error unmarshal_attributes_check( c_xml_reader_soap & ar_reader );
    LMX_GDECL void unmarshal_anyattribute( c_xml_reader_soap & ar_reader );
    LMX_GDECL bool unmarshal_body( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error );

private:
    LMX_GDECL void init();
    LMX_GDECL void release();
    c_Header( const c_Header & );
    c_Header & operator =( const c_Header & );

    friend class c_Header_unmarshal_helper;
};

// From: lmxsoap-env.xsd(65)
class c_Body
{
    //:snippet:start Body
    elmx_error on_lmx_marshal_body_start( c_xml_writer_soap &ar_writer ) const;
    elmx_error on_lmx_unmarshal_body_start( c_xml_reader_soap &ar_reader );
    //:snippet:end Body

private:
    // Attribute(s)
    lmx::t_any_info_container any_attribute;

    // Element(s)
    lmx::ct_simple_non_pod_multi< lmx::c_any_info > m_any;

    static const lmx::s_event_map attr_event_map[];
    static const lmx::s_event_map elem_event_map[];

public:
    LMX_GDECL c_Body();
    LMX_GDECL c_Body( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error );
    LMX_GDECL virtual ~c_Body();

    // Attribute(s)
    LMX_GDECL lmx::t_any_info_container & getany_attributes() { return any_attribute; }
    LMX_GDECL void getany_attribute( size_t a_index, std::string * ap_namespace, std::string * ap_name, std::string * ap_value ) const;
    LMX_GDECL size_t sizeany_attribute() const { return any_attribute.size(); }
    LMX_GDECL void appendany_attribute( const std::string & ar_name, const std::string & ar_value ) { any_attribute.push_back_w_autop( new lmx::c_any_info( ar_name, ar_value ) ); }
    LMX_GDECL void insertany_attribute( size_t a_index, const std::string & ar_name, const std::string & ar_value ) { any_attribute.insert_w_autop( a_index, new lmx::c_any_info( ar_name, ar_value ) ); }
    LMX_GDECL void deleteany_attribute( size_t a_index ) { any_attribute.erase( a_index ); }
    LMX_GDECL void clearany_attribute() { any_attribute.clear(); }
    // Element(s)

    //    {any} --> xs:any[0..*]
    LMX_GDECL lmx::elmx_error append_any( const std::string & ar_value )
    {
        m_any.append( lmx::c_any_info( ar_value ) );
        return lmx::ELMX_OK;
    }
    LMX_GDECL lmx::elmx_error insert_any( size_t a_index, const std::string & ar_value )
    {
        m_any.insert( a_index, lmx::c_any_info( ar_value ) );
        return lmx::ELMX_OK;
    }
    LMX_GDECL void delete_any( size_t a_index ) { m_any.erase( a_index ); }
    LMX_GDECL void clear_any() { m_any.clear(); }
    LMX_GDECL void get_any( size_t a_index, std::string * ap_namespace, std::string * ap_name, std::string * ap_value ) const
    {
        if( ap_namespace ) *ap_namespace = m_any.get( a_index ).get_namespace();
        if( ap_name ) *ap_name = m_any.get( a_index ).get_name();
        *ap_value = m_any.get( a_index ).get_value();
    }
    LMX_GDECL const lmx::c_any_info & get_any( size_t a_index ) const { return  m_any.get( a_index ); }
    LMX_GDECL lmx::c_any_info & get_any( size_t a_index ) { return  m_any.get( a_index ); }
    LMX_GDECL size_t size_any() const { return m_any.size(); }

    LMX_GDECL bool is_occurs_ok() const { return true; }

    LMX_GDECL static const lmx::s_ns_map *ns_map() { return ns_map_reader; }
    // General marshal/unmarshal functions
    LMX_GDECL lmx::elmx_error marshal( c_xml_writer_soap & ar_writer, const char * ap_name = "SOAP-ENV:Body" ) const;
    LMX_GDECL lmx::elmx_error unmarshal( c_xml_reader_soap & ar_reader );
    LMX_GDECL lmx::elmx_error unmarshal( c_xml_reader_soap & ar_reader, const std::string & ar_name );

protected:
    LMX_GDECL void marshal_attributes( c_xml_writer_soap & ar_writer ) const;
    LMX_GDECL lmx::elmx_error marshal_child_elements( c_xml_writer_soap & ar_writer ) const;
    LMX_GDECL bool unmarshal_attributes( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error );
    LMX_GDECL lmx::elmx_error unmarshal_attributes_check( c_xml_reader_soap & ar_reader );
    LMX_GDECL void unmarshal_anyattribute( c_xml_reader_soap & ar_reader );
    LMX_GDECL bool unmarshal_body( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error );

private:
    LMX_GDECL void init();
    LMX_GDECL void release();
    c_Body( const c_Body & );
    c_Body & operator =( const c_Body & );

    friend class c_Body_unmarshal_helper;
};

// From: lmxsoap-env.xsd(123)
class c_detail
{
    //:snippet:start detail
    elmx_error on_lmx_marshal_body_start( c_xml_writer_soap &ar_writer ) const;
    elmx_error on_lmx_unmarshal_body_start( c_xml_reader_soap &ar_reader );
    //:snippet:end detail

private:
    // Attribute(s)
    lmx::t_any_info_container any_attribute;

    // Element(s)
    lmx::ct_simple_non_pod_multi< lmx::c_any_info > m_any;

    static const lmx::s_event_map attr_event_map[];
    static const lmx::s_event_map elem_event_map[];

public:
    LMX_GDECL c_detail();
    LMX_GDECL virtual ~c_detail();

    // Attribute(s)
    LMX_GDECL lmx::t_any_info_container & getany_attributes() { return any_attribute; }
    LMX_GDECL void getany_attribute( size_t a_index, std::string * ap_namespace, std::string * ap_name, std::string * ap_value ) const;
    LMX_GDECL size_t sizeany_attribute() const { return any_attribute.size(); }
    LMX_GDECL void appendany_attribute( const std::string & ar_name, const std::string & ar_value ) { any_attribute.push_back_w_autop( new lmx::c_any_info( ar_name, ar_value ) ); }
    LMX_GDECL void insertany_attribute( size_t a_index, const std::string & ar_name, const std::string & ar_value ) { any_attribute.insert_w_autop( a_index, new lmx::c_any_info( ar_name, ar_value ) ); }
    LMX_GDECL void deleteany_attribute( size_t a_index ) { any_attribute.erase( a_index ); }
    LMX_GDECL void clearany_attribute() { any_attribute.clear(); }
    // Element(s)

    //    {any} --> xs:any[0..*]
    LMX_GDECL lmx::elmx_error append_any( const std::string & ar_value )
    {
        m_any.append( lmx::c_any_info( ar_value ) );
        return lmx::ELMX_OK;
    }
    LMX_GDECL lmx::elmx_error insert_any( size_t a_index, const std::string & ar_value )
    {
        m_any.insert( a_index, lmx::c_any_info( ar_value ) );
        return lmx::ELMX_OK;
    }
    LMX_GDECL void delete_any( size_t a_index ) { m_any.erase( a_index ); }
    LMX_GDECL void clear_any() { m_any.clear(); }
    LMX_GDECL void get_any( size_t a_index, std::string * ap_namespace, std::string * ap_name, std::string * ap_value ) const
    {
        if( ap_namespace ) *ap_namespace = m_any.get( a_index ).get_namespace();
        if( ap_name ) *ap_name = m_any.get( a_index ).get_name();
        *ap_value = m_any.get( a_index ).get_value();
    }
    LMX_GDECL const lmx::c_any_info & get_any( size_t a_index ) const { return  m_any.get( a_index ); }
    LMX_GDECL lmx::c_any_info & get_any( size_t a_index ) { return  m_any.get( a_index ); }
    LMX_GDECL size_t size_any() const { return m_any.size(); }

    LMX_GDECL bool is_occurs_ok() const { return true; }

    // General marshal/unmarshal functions
    LMX_GDECL lmx::elmx_error marshal( c_xml_writer_soap & ar_writer, const char * ap_name = "SOAP-ENV:detail" ) const;
    LMX_GDECL lmx::elmx_error unmarshal( c_xml_reader_soap & ar_reader, const std::string & ar_name );

protected:
    LMX_GDECL void marshal_attributes( c_xml_writer_soap & ar_writer ) const;
    LMX_GDECL lmx::elmx_error marshal_child_elements( c_xml_writer_soap & ar_writer ) const;
    LMX_GDECL bool unmarshal_attributes( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error );
    LMX_GDECL lmx::elmx_error unmarshal_attributes_check( c_xml_reader_soap & ar_reader );
    LMX_GDECL void unmarshal_anyattribute( c_xml_reader_soap & ar_reader );
    LMX_GDECL bool unmarshal_body( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error );

private:
    LMX_GDECL void init();
    LMX_GDECL void release();
    c_detail( const c_detail & );
    c_detail & operator =( const c_detail & );

    friend class c_detail_unmarshal_helper;
};

// From: lmxsoap-env.xsd(45)
class c_Envelope
{
    //:snippet:start Envelope
    elmx_error on_lmx_marshal_start( c_xml_writer_soap &ar_writer, const char *ap_name ) const;
    //:snippet:end Envelope

private:
    // Attribute(s)
    lmx::t_any_info_container any_attribute;

    // Element(s)
    lmx::ct_complex_optional< c_Header > m_Header;
    lmx::ct_complex_single< c_Body > m_Body;
    lmx::ct_simple_non_pod_multi< lmx::c_any_info > m_any;

    static const lmx::s_event_map attr_event_map[];
    static const lmx::s_event_map elem_event_map[];

public:
    LMX_GDECL c_Envelope();
    LMX_GDECL c_Envelope( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error );
    LMX_GDECL virtual ~c_Envelope();

    // Attribute(s)
    LMX_GDECL lmx::t_any_info_container & getany_attributes() { return any_attribute; }
    LMX_GDECL void getany_attribute( size_t a_index, std::string * ap_namespace, std::string * ap_name, std::string * ap_value ) const;
    LMX_GDECL size_t sizeany_attribute() const { return any_attribute.size(); }
    LMX_GDECL void appendany_attribute( const std::string & ar_name, const std::string & ar_value ) { any_attribute.push_back_w_autop( new lmx::c_any_info( ar_name, ar_value ) ); }
    LMX_GDECL void insertany_attribute( size_t a_index, const std::string & ar_name, const std::string & ar_value ) { any_attribute.insert_w_autop( a_index, new lmx::c_any_info( ar_name, ar_value ) ); }
    LMX_GDECL void deleteany_attribute( size_t a_index ) { any_attribute.erase( a_index ); }
    LMX_GDECL void clearany_attribute() { any_attribute.clear(); }
    // Element(s)

    //    SOAP-ENV:Header --> Header[0..1]
    LMX_GDECL const c_Header & get_Header() const { return m_Header.get(); }
    LMX_GDECL c_Header & get_Header() { return m_Header.get(); } // For read/write access
    LMX_GDECL c_Header & assign_Header( c_Header * p ) { return m_Header.assign( p ); }
    LMX_GDECL bool isset_Header() const { return m_Header.is_value_set(); }
    LMX_GDECL void unset_Header() { m_Header.unset(); }

    //    SOAP-ENV:Body --> Body
    LMX_GDECL const c_Body & get_Body() const { return m_Body.get(); }
    LMX_GDECL c_Body & get_Body() { return m_Body.get(); } // For read/write access
    LMX_GDECL c_Body & assign_Body( c_Body * p ) { return m_Body.assign( p ); }

    //    {any} --> xs:any[0..*]
    LMX_GDECL lmx::elmx_error append_any( const std::string & ar_value )
    {
        m_any.append( lmx::c_any_info( ar_value ) );
        return lmx::ELMX_OK;
    }
    LMX_GDECL lmx::elmx_error insert_any( size_t a_index, const std::string & ar_value )
    {
        m_any.insert( a_index, lmx::c_any_info( ar_value ) );
        return lmx::ELMX_OK;
    }
    LMX_GDECL void delete_any( size_t a_index ) { m_any.erase( a_index ); }
    LMX_GDECL void clear_any() { m_any.clear(); }
    LMX_GDECL void get_any( size_t a_index, std::string * ap_namespace, std::string * ap_name, std::string * ap_value ) const
    {
        if( ap_namespace ) *ap_namespace = m_any.get( a_index ).get_namespace();
        if( ap_name ) *ap_name = m_any.get( a_index ).get_name();
        *ap_value = m_any.get( a_index ).get_value();
    }
    LMX_GDECL const lmx::c_any_info & get_any( size_t a_index ) const { return  m_any.get( a_index ); }
    LMX_GDECL lmx::c_any_info & get_any( size_t a_index ) { return  m_any.get( a_index ); }
    LMX_GDECL size_t size_any() const { return m_any.size(); }

    LMX_GDECL bool is_occurs_ok() const { return true; }

    LMX_GDECL static const lmx::s_ns_map *ns_map() { return ns_map_reader; }
    // General marshal/unmarshal functions
    LMX_GDECL lmx::elmx_error marshal( c_xml_writer_soap & ar_writer, const char * ap_name = "SOAP-ENV:Envelope" ) const;
    LMX_GDECL lmx::elmx_error unmarshal( c_xml_reader_soap & ar_reader );
    LMX_GDECL lmx::elmx_error unmarshal( c_xml_reader_soap & ar_reader, const std::string & ar_name );

protected:
    LMX_GDECL void marshal_attributes( c_xml_writer_soap & ar_writer ) const;
    LMX_GDECL lmx::elmx_error marshal_child_elements( c_xml_writer_soap & ar_writer ) const;
    LMX_GDECL bool unmarshal_attributes( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error );
    LMX_GDECL lmx::elmx_error unmarshal_attributes_check( c_xml_reader_soap & ar_reader );
    LMX_GDECL void unmarshal_anyattribute( c_xml_reader_soap & ar_reader );
    LMX_GDECL bool unmarshal_body( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error );

private:
    LMX_GDECL void init();
    LMX_GDECL void release();
    c_Envelope( const c_Envelope & );
    c_Envelope & operator =( const c_Envelope & );

    friend class c_Envelope_unmarshal_helper;
};

#ifdef LMX_USER_VALIDATE
lmx::elmx_error user_validator_1( const lmx::tc_qname &, const char * );
#endif // LMX_USER_VALIDATE

#ifdef LMX_USER_VALIDATE
lmx::elmx_error user_validator_2( const lmx::tlmx_unicode_string &, const char * );
#endif // LMX_USER_VALIDATE

#ifdef LMX_USER_VALIDATE
lmx::elmx_error user_validator_3( const lmx::tlmx_uri_string &, const char * );
#endif // LMX_USER_VALIDATE

// From: lmxsoap-env.xsd(107)
class c_Fault
{
    //:snippet:start Fault
    elmx_error on_lmx_marshal_start( c_xml_writer_soap &ar_writer, const char *ap_name ) const;
    //:snippet:end Fault

private:
    // Element(s)
    lmx::ct_simple_non_pod_single< lmx::tc_qname, lmx::c_handling_qname > m_faultcode;
    lmx::ct_simple_non_pod_single< lmx::tlmx_unicode_string > m_faultstring;
    lmx::ct_simple_non_pod_optional< lmx::tlmx_uri_string > m_faultactor;
    lmx::ct_complex_optional< c_detail > m_detail;

    static const lmx::s_event_map attr_event_map[];
    static const lmx::s_event_map elem_event_map[];

public:
    LMX_GDECL c_Fault();
    LMX_GDECL c_Fault( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error );
    LMX_GDECL virtual ~c_Fault();

    // Element(s)

    //    faultcode --> xs:QName
    LMX_GDECL const lmx::tc_qname & get_faultcode() const { return m_faultcode.get(); }
    LMX_GDECL lmx::elmx_error set_faultcode( const lmx::tc_qname & a ) { return m_faultcode.set( a ); }

    //    faultstring --> xs:string
    LMX_GDECL const lmx::tlmx_unicode_string & get_faultstring() const { return m_faultstring.get(); }
    LMX_GDECL lmx::elmx_error set_faultstring( const lmx::tlmx_unicode_string & a ) { return m_faultstring.set( a ); }

    //    faultactor --> xs:anyURI[0..1]
    LMX_GDECL const lmx::tlmx_uri_string & get_faultactor() const { return m_faultactor.get(); }
    LMX_GDECL lmx::elmx_error set_faultactor( const lmx::tlmx_uri_string & a ) { return m_faultactor.set( a ); }
    LMX_GDECL bool isset_faultactor() const { return m_faultactor.is_value_set(); }
    LMX_GDECL void unset_faultactor() { m_faultactor.unset(); }

    //    detail --> detail[0..1]
    LMX_GDECL const c_detail & get_detail() const { return m_detail.get(); }
    LMX_GDECL c_detail & get_detail() { return m_detail.get(); } // For read/write access
    LMX_GDECL c_detail & assign_detail( c_detail * p ) { return m_detail.assign( p ); }
    LMX_GDECL bool isset_detail() const { return m_detail.is_value_set(); }
    LMX_GDECL void unset_detail() { m_detail.unset(); }

    LMX_GDECL bool is_occurs_ok() const;

    LMX_GDECL static const lmx::s_ns_map *ns_map() { return ns_map_reader; }
    // General marshal/unmarshal functions
    LMX_GDECL lmx::elmx_error marshal( c_xml_writer_soap & ar_writer, const char * ap_name = "SOAP-ENV:Fault" ) const;
    LMX_GDECL lmx::elmx_error unmarshal( c_xml_reader_soap & ar_reader );
    LMX_GDECL lmx::elmx_error unmarshal( c_xml_reader_soap & ar_reader, const std::string & ar_name );

protected:
    LMX_GDECL void marshal_attributes( c_xml_writer_soap & LMX_UNUSED_PARAM(ar_writer) ) const {}
    LMX_GDECL lmx::elmx_error marshal_child_elements( c_xml_writer_soap & ar_writer ) const;
    LMX_GDECL bool unmarshal_attributes( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error );
    LMX_GDECL lmx::elmx_error unmarshal_attributes_check( c_xml_reader_soap & ar_reader );
    LMX_GDECL bool unmarshal_body( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error );

private:
    LMX_GDECL void init();
    LMX_GDECL void release();
    c_Fault( const c_Fault & );
    c_Fault & operator =( const c_Fault & );

    friend class c_Fault_unmarshal_helper;
};

}   // end of namespace lmx

#if defined( _MSC_VER ) && _MSC_VER >= 1400
#pragma warning (pop)  // Restore disabled warnings
#endif


namespace lmx {

//------------------------------------------------------------------------------
//                       soap_select_ns_map function
//              Outside of other classes to ease dependecies.
//------------------------------------------------------------------------------

void soap_reenable_ns_map( c_xml_writer & ar_writer )
{
    ar_writer.reenable_ns_map_output();
}

//------------------------------------------------------------------------------
//                        c_soap_empty components
//------------------------------------------------------------------------------

const s_ns_map c_soap_empty::ns_map_reader[] =
{
    { 0,    LMXNULL,    LMXNULL }
};

const s_ns_map *c_soap_empty::ns_map()
{
    return ns_map_reader;
}

elmx_error c_soap_empty::unmarshal(
                    lmx::c_xml_reader & ar_reader,
                    const std::string & LMX_UNUSED_PARAM(ar_name) )
{
    elmx_error l_error;
    ar_reader.skip_rest_of_element( ar_reader.get_full_name() );
    ar_reader.get_element_event( LMXNULL, &l_error, &ar_reader.name );
    return l_error;
}

//------------------------------------------------------------------------------
//                      Main Soap marshaling methods
//------------------------------------------------------------------------------

elmx_error c_soap_base::marshal( std::string * ap_string, s_debug_error * ap_debug_error /*= LMXNULL*/ ) const
{
    std::ostringstream l_sout;
    elmx_error l_error = marshal( l_sout, ap_debug_error );
    if( l_error == ELMX_OK )
        *ap_string = l_sout.str();
    return l_error;
}

elmx_error c_soap_base::marshal( std::ostream & ar_sos, s_debug_error * ap_debug_error /*= LMXNULL*/ ) const
{
    c_xml_writer_soap l_writer( *this, ar_sos, lmx::c_xml_writer::include_xml_decl );
    c_Envelope l_soap_envelope;
    elmx_error l_error = l_soap_envelope.marshal( l_writer );
    if( ap_debug_error )        // C110512_0912
        *ap_debug_error = l_writer.get_debug_error();
    return l_error;
}

//------------------------------------------------------------------------------
//                      Main Soap unmarshaling methods
//------------------------------------------------------------------------------

elmx_error c_soap_base::unmarshal( const char ac_file_name[], s_debug_error * ap_debug_error /*= LMXNULL*/ )
{
    c_read_file l_reader( ac_file_name );
    if( ! l_reader.is_open() )
        return ELMX_NO_FILE;
    return unmarshal( l_reader, ap_debug_error );
}

elmx_error c_soap_base::unmarshal( const std::string & ar_string, s_debug_error * ap_debug_error /*= LMXNULL*/ )
{
    return unmarshal( ar_string.data(), ar_string.size(), ap_debug_error );
}

elmx_error c_soap_base::unmarshal( const char * ap_memory, size_t a_memory_size, s_debug_error * ap_debug_error /*= LMXNULL*/ )
{
    c_read_memory l_reader( ap_memory, a_memory_size );
    return unmarshal( l_reader, ap_debug_error );
}

elmx_error c_soap_base::unmarshal( c_read & ar_low_level_reader, s_debug_error * ap_debug_error /*= LMXNULL*/ )
{
    c_xml_reader_soap l_reader( *this, ar_low_level_reader );
    c_Envelope l_soap_envelope;
    l_reader.add_namespace( "http://schemas.xmlsoap.org/soap/envelope/", EXKN_SOAP_ENV );
    l_reader.add_namespace( "http://schemas.xmlsoap.org/soap/encoding/", EXKN_SOAP_ENC );
    l_reader.set_ns_map( get_ns_map() );
    elmx_error l_error = l_soap_envelope.unmarshal( l_reader );
    if( ap_debug_error )        // C110512_0912
        *ap_debug_error = l_reader.get_debug_error();
    return l_error;
}

//------------------------------------------------------------------------------
//                      Marshaling snippet event handlers
//------------------------------------------------------------------------------

elmx_error c_Envelope::on_lmx_marshal_start(
                            c_xml_writer_soap & ar_writer,
                            const char * LMX_UNUSED_PARAM(ap_name) ) const
{
    c_Envelope *lp_mutable_envelope = const_cast< c_Envelope * >( this );
    if( ar_writer.get_soap_base().isset_headers() )
        lp_mutable_envelope->get_Header();  // Force creation of Header object
    return ELMX_OK;
}

elmx_error c_Header::on_lmx_marshal_body_start( c_xml_writer_soap & ar_writer ) const
{
    ar_writer.prepare_for_simple_body_data();   // Allows v5 code to call pre-v5 generated code
    return ar_writer.get_soap_base().marshal_headers( ar_writer );
}

elmx_error c_Body::on_lmx_marshal_body_start( c_xml_writer_soap & ar_writer ) const
{
    elmx_error l_error = ELMX_OK;

    ar_writer.prepare_for_simple_body_data();   // Allows v5 code to call pre-v5 generated code
    if( ar_writer.get_soap_base().is_fault() )
    {
        const c_soap_fault_iface &lr_fault_iface = ar_writer.get_soap_base().get_fault_iface();
        c_Fault l_fault;
        l_fault.set_faultcode( lr_fault_iface.get_fault_code() );
        l_fault.set_faultstring( lr_fault_iface.get_fault_string() );
        if( lr_fault_iface.isset_fault_actor() )
            l_fault.set_faultactor( lr_fault_iface.get_fault_actor() );

        l_error = l_fault.marshal( ar_writer );
    }

    else
    {
        l_error = ar_writer.get_soap_base().marshal_bodies( ar_writer );
    }

    return l_error;
}

elmx_error c_Fault::on_lmx_marshal_start(
                            c_xml_writer_soap & ar_writer,
                            const char * LMX_UNUSED_PARAM(ap_name) ) const
{
    c_Fault *lp_mutable_fault = const_cast< c_Fault * >( this );
    if( ar_writer.get_soap_base().get_fault_iface().isset_details() )
        lp_mutable_fault->get_detail(); // Force creation of detail object
    return ELMX_OK;
}

elmx_error c_detail::on_lmx_marshal_body_start( c_xml_writer_soap & ar_writer ) const
{
    ar_writer.prepare_for_simple_body_data();   // Allows v5 code to call pre-v5 generated code
    return ar_writer.get_soap_base().get_fault_iface().marshal_details( ar_writer );
}

//------------------------------------------------------------------------------
//                      Unmarshaling snippet event handlers
//------------------------------------------------------------------------------

elmx_error c_Header::on_lmx_unmarshal_body_start( c_xml_reader_soap & ar_reader )
{
    // ns_map installed in outer-most method
    while( ar_reader.get_current_event() != EXE_ELEMENT_END )
    {
        elmx_error l_error = ar_reader.get_soap_base().unmarshal_header( ar_reader, ar_reader.get_full_name() );
        if( l_error != ELMX_OK )
            return l_error;

        ar_reader.get_element_event( LMXNULL, &l_error, &ar_reader.name );
        if( l_error != lmx::ELMX_OK && (l_error = ar_reader.handle_error( l_error, ar_reader.name, __FILE__, __LINE__ )) != lmx::ELMX_OK )
            return l_error;
    }

    return ELMX_OK;
}

elmx_error c_Body::on_lmx_unmarshal_body_start( c_xml_reader_soap & ar_reader )
{
    // ns_map installed in outer-most method
    // If fault, unmarshal fault, else unmarshal body
    if( ar_reader.get_element_ns_id() == EXKN_SOAP_ENV && ar_reader.get_local_name() == "Fault" )
    {
        elmx_error l_error;
        c_Fault l_fault;
        l_error = l_fault.unmarshal( ar_reader, ar_reader.get_full_name() );
        if( l_error != ELMX_OK )
            return l_error;

        c_soap_fault_iface &lr_fault_iface = ar_reader.get_soap_base().get_fault_iface();
        lr_fault_iface.set_fault_code( l_fault.get_faultcode() );
        lr_fault_iface.set_fault_string( l_fault.get_faultstring() );
        if( l_fault.isset_faultactor() )
            lr_fault_iface.set_fault_actor( l_fault.get_faultactor() );

        ar_reader.get_element_event( LMXNULL, &l_error, &ar_reader.name );
        if( l_error != lmx::ELMX_OK && (l_error = ar_reader.handle_error( l_error, ar_reader.name, __FILE__, __LINE__ )) != lmx::ELMX_OK )
            return l_error;

        return ELMX_OK;
    }

    while( ar_reader.get_current_event() != EXE_ELEMENT_END )
    {
        elmx_error l_error = ar_reader.get_soap_base().unmarshal_body( ar_reader, ar_reader.get_full_name() );
        if( l_error != ELMX_OK )
            return l_error;

        ar_reader.get_element_event( LMXNULL, &l_error, &ar_reader.name );
        if( l_error != lmx::ELMX_OK && (l_error = ar_reader.handle_error( l_error, ar_reader.name, __FILE__, __LINE__ )) != lmx::ELMX_OK )
            return l_error;
    }

    return ELMX_OK;
}

elmx_error c_detail::on_lmx_unmarshal_body_start( c_xml_reader_soap & ar_reader )
{
    // ns_map installed in outer-most method
    while( ar_reader.get_current_event() != EXE_ELEMENT_END )
    {
        elmx_error l_error = ar_reader.get_soap_base().get_fault_iface().unmarshal_detail( ar_reader, ar_reader.get_full_name() );
        if( l_error != ELMX_OK )
            return l_error;

        ar_reader.get_element_event( LMXNULL, &l_error, &ar_reader.name );
        if( l_error != lmx::ELMX_OK && (l_error = ar_reader.handle_error( l_error, ar_reader.name, __FILE__, __LINE__ )) != lmx::ELMX_OK )
            return l_error;
    }

    return ELMX_OK;
}

}  // end of namespace lmx

#if defined( _MSC_VER ) && _MSC_VER > 1000
#pragma warning (disable:4786)  // Disable truncated debug info warning
#pragma warning (disable:4100)  // Disable unreferenced formal parameter
#pragma warning (disable:4702)  // Disable unreachable code
#endif


#if defined( _MSC_VER ) && _MSC_VER <= 1200
using namespace lmx;
#endif

namespace lmx {

const lmx::tlmx_unicode_string limit_31_e0( lmx::inittowstring( "default" ) );
const lmx::tlmx_unicode_string limit_31_e1( lmx::inittowstring( "preserve" ) );

const lmx::s_ns_map ns_map_reader[] =
{
    { 7,    "SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/" },
    { 0,    LMXNULL,    LMXNULL }
};

const lmx::s_ns_map ns_map_writer_1[] =
{
    { 7,    "SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/" },
    { 0,    LMXNULL,    LMXNULL }
};

const lmx::s_event_map c_Header::attr_event_map[] =
{
    { 0,                LMXNULL,                                0,  lmx::EXE_UNKNOWN }
};

const lmx::s_event_map c_Header::elem_event_map[] =
{
    { 0,                LMXNULL,                                0,  lmx::EXE_UNKNOWN }
};

c_Header::c_Header()
{
    init();
}

c_Header::c_Header( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error )
{
    *ap_error = lmx::ELMX_UNDEFINED_ERROR;
    init();
    *ap_error = unmarshal( ar_reader );
}

void c_Header::init()
{
}

c_Header::~c_Header()
{
}

void c_Header::getany_attribute( size_t a_index, std::string * ap_namespace, std::string * ap_name, std::string * ap_value ) const
{
    lmx_assert( a_index < any_attribute.size() );
    if( ap_namespace )
        *ap_namespace = any_attribute[ a_index ].get_namespace();
    *ap_name = any_attribute[ a_index ].get_name();
    *ap_value = any_attribute[ a_index ].get_value();
}

lmx::elmx_error c_Header::marshal( c_xml_writer_soap & ar_writer, const char * ap_name ) const
{
    lmx_assert( is_occurs_ok() );
    lmx::c_xml_writer_local l_xml_writer_local( ar_writer );
    ar_writer.start_element( ap_name );
    ar_writer.conditionally_select_ns_map( ns_map_writer_1 );
    ar_writer.conditionally_write_ns_attrs( false );
    marshal_attributes( ar_writer );
    lmx::elmx_error l_error;
    if( (l_error = marshal_child_elements( ar_writer )) != lmx::ELMX_OK )
        return l_error;
    ar_writer.end_element( ap_name );
    return lmx::ELMX_OK;
}

void c_Header::marshal_attributes( c_xml_writer_soap & ar_writer ) const
{
    ar_writer.marshal_any_attribute( any_attribute );
}

lmx::elmx_error c_Header::marshal_child_elements( c_xml_writer_soap & ar_writer ) const
{
    LMX_NO_WARN_UNUSED( ar_writer );
    {
    lmx::elmx_error l_event_error = on_lmx_marshal_body_start( ar_writer );
    if( l_event_error != lmx::ELMX_OK )
        return l_event_error;
    }
    lmx::elmx_error l_error = lmx::ELMX_OK;
    ar_writer.marshal_any_element( m_any );
    return l_error;
}

lmx::elmx_error c_Header::unmarshal( c_xml_reader_soap & ar_reader )
{
    ar_reader.set_code_file( __FILE__ );
    ar_reader.set_ns_map( ns_map_reader );
    lmx::elmx_error l_error = lmx::ELMX_OK;
    lmx::c_xml_reader_local l_reader_local( &ar_reader );
    ar_reader.get_element_event( &l_error );
    if( l_error != lmx::ELMX_OK && (l_error = ar_reader.handle_error( l_error, __LINE__ )) != lmx::ELMX_OK )
        return l_error;
    if( (ar_reader.get_element_ns_id() != 7 || ar_reader.get_local_name() != "Header") && (l_error = ar_reader.handle_error( lmx::ELMX_ELEMENT_NOT_FOUND, __LINE__ )) != lmx::ELMX_OK )
        return l_error;
    return unmarshal( ar_reader, ar_reader.name );
}

class c_Header_unmarshal_helper : public lmx::c_unmarshal_helper
{
private:
    c_Header *p_parent;
    c_xml_reader_soap & r_reader;
public:
    c_Header_unmarshal_helper( c_Header * ap_parent, c_xml_reader_soap & ar_reader )
        : lmx::c_unmarshal_helper( ar_reader ), p_parent( ap_parent ), r_reader( ar_reader ) {}
    virtual bool unmarshal_an_attribute( lmx::elmx_error * ap_error )
            { return p_parent->unmarshal_attributes( r_reader, ap_error ); }
    virtual lmx::elmx_error unmarshal_attributes_check()
           { return p_parent->unmarshal_attributes_check( r_reader ); }
    virtual bool unmarshal_complex_body( lmx::elmx_error * ap_error )
           { return p_parent->unmarshal_body( r_reader, ap_error ); }
};

lmx::elmx_error c_Header::unmarshal( c_xml_reader_soap & ar_reader, const std::string & ar_name )
{
    if( &ar_reader.name != &ar_name )
        ar_reader.name = ar_name;
    lmx::elmx_error l_error;
    c_Header_unmarshal_helper l_helper( this, ar_reader );
    l_helper.set_anyattributes_store( & getany_attributes() );
    l_helper.set_body_can_be_empty();
    l_error = l_helper.unmarshal_complex_content( __FILE__, __LINE__ );
    return l_error;
}

bool c_Header::unmarshal_attributes( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error )
{
    ar_reader.tokenise( attr_event_map, lmx::EXNT_ATTR );
    switch( ar_reader.get_current_event() )
    {
    case -1:    // Avoid warning saying only default: present
    break;
    }
    LMX_NO_WARN_UNUSED( ar_reader );
    LMX_NO_WARN_UNUSED( ap_error );
    return false;
}

lmx::elmx_error c_Header::unmarshal_attributes_check( c_xml_reader_soap & ar_reader )
{
    LMX_NO_WARN_UNUSED( ar_reader );
    return lmx::ELMX_OK;
}

void c_Header::unmarshal_anyattribute( c_xml_reader_soap & ar_reader )
{
    std::string l_namespace;
    ar_reader.get_namespace( ar_reader.name, &l_namespace );
    any_attribute.push_back_w_autop( new lmx::c_any_info( l_namespace, ar_reader.name, ar_reader.value ) );
}

bool c_Header::unmarshal_body( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error )
{
    if( (*ap_error = on_lmx_unmarshal_body_start( ar_reader )) != lmx::ELMX_OK )
        return false;
    if( ar_reader.get_current_event() == lmx::EXE_ELEMENT_END )
        return true;
    ar_reader.tokenise( elem_event_map, lmx::EXNT_ELEM );
    while( (ar_reader.is_current_event_tokenisable() &&
            ar_reader.get_namespace( ar_reader.name, &ar_reader.value ) != "http://schemas.xmlsoap.org/soap/envelope/") )
    {
        ar_reader.set_code_line( __LINE__ );
        *ap_error = ar_reader.unmarshal_any_child_element( m_any, elem_event_map );
        if( *ap_error != lmx::ELMX_OK )
            return false;
    }
    return true;
}

const lmx::s_event_map c_Body::attr_event_map[] =
{
    { 0,                LMXNULL,                                0,  lmx::EXE_UNKNOWN }
};

const lmx::s_event_map c_Body::elem_event_map[] =
{
    { 0,                LMXNULL,                                0,  lmx::EXE_UNKNOWN }
};

c_Body::c_Body()
{
    init();
}

c_Body::c_Body( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error )
{
    *ap_error = lmx::ELMX_UNDEFINED_ERROR;
    init();
    *ap_error = unmarshal( ar_reader );
}

void c_Body::init()
{
}

c_Body::~c_Body()
{
}

void c_Body::getany_attribute( size_t a_index, std::string * ap_namespace, std::string * ap_name, std::string * ap_value ) const
{
    lmx_assert( a_index < any_attribute.size() );
    if( ap_namespace )
        *ap_namespace = any_attribute[ a_index ].get_namespace();
    *ap_name = any_attribute[ a_index ].get_name();
    *ap_value = any_attribute[ a_index ].get_value();
}

lmx::elmx_error c_Body::marshal( c_xml_writer_soap & ar_writer, const char * ap_name ) const
{
    lmx_assert( is_occurs_ok() );
    lmx::c_xml_writer_local l_xml_writer_local( ar_writer );
    ar_writer.start_element( ap_name );
    ar_writer.conditionally_select_ns_map( ns_map_writer_1 );
    ar_writer.conditionally_write_ns_attrs( false );
    marshal_attributes( ar_writer );
    lmx::elmx_error l_error;
    if( (l_error = marshal_child_elements( ar_writer )) != lmx::ELMX_OK )
        return l_error;
    ar_writer.end_element( ap_name );
    return lmx::ELMX_OK;
}

void c_Body::marshal_attributes( c_xml_writer_soap & ar_writer ) const
{
    ar_writer.marshal_any_attribute( any_attribute );
}

lmx::elmx_error c_Body::marshal_child_elements( c_xml_writer_soap & ar_writer ) const
{
    LMX_NO_WARN_UNUSED( ar_writer );
    {
    lmx::elmx_error l_event_error = on_lmx_marshal_body_start( ar_writer );
    if( l_event_error != lmx::ELMX_OK )
        return l_event_error;
    }
    lmx::elmx_error l_error = lmx::ELMX_OK;
    ar_writer.marshal_any_element( m_any );
    return l_error;
}

lmx::elmx_error c_Body::unmarshal( c_xml_reader_soap & ar_reader )
{
    ar_reader.set_code_file( __FILE__ );
    ar_reader.set_ns_map( ns_map_reader );
    lmx::elmx_error l_error = lmx::ELMX_OK;
    lmx::c_xml_reader_local l_reader_local( &ar_reader );
    ar_reader.get_element_event( &l_error );
    if( l_error != lmx::ELMX_OK && (l_error = ar_reader.handle_error( l_error, __LINE__ )) != lmx::ELMX_OK )
        return l_error;
    if( (ar_reader.get_element_ns_id() != 7 || ar_reader.get_local_name() != "Body") && (l_error = ar_reader.handle_error( lmx::ELMX_ELEMENT_NOT_FOUND, __LINE__ )) != lmx::ELMX_OK )
        return l_error;
    return unmarshal( ar_reader, ar_reader.name );
}

class c_Body_unmarshal_helper : public lmx::c_unmarshal_helper
{
private:
    c_Body *p_parent;
    c_xml_reader_soap & r_reader;
public:
    c_Body_unmarshal_helper( c_Body * ap_parent, c_xml_reader_soap & ar_reader )
        : lmx::c_unmarshal_helper( ar_reader ), p_parent( ap_parent ), r_reader( ar_reader ) {}
    virtual bool unmarshal_an_attribute( lmx::elmx_error * ap_error )
            { return p_parent->unmarshal_attributes( r_reader, ap_error ); }
    virtual lmx::elmx_error unmarshal_attributes_check()
           { return p_parent->unmarshal_attributes_check( r_reader ); }
    virtual bool unmarshal_complex_body( lmx::elmx_error * ap_error )
           { return p_parent->unmarshal_body( r_reader, ap_error ); }
};

lmx::elmx_error c_Body::unmarshal( c_xml_reader_soap & ar_reader, const std::string & ar_name )
{
    if( &ar_reader.name != &ar_name )
        ar_reader.name = ar_name;
    lmx::elmx_error l_error;
    c_Body_unmarshal_helper l_helper( this, ar_reader );
    l_helper.set_anyattributes_store( & getany_attributes() );
    l_helper.set_body_can_be_empty();
    l_error = l_helper.unmarshal_complex_content( __FILE__, __LINE__ );
    return l_error;
}

bool c_Body::unmarshal_attributes( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error )
{
    ar_reader.tokenise( attr_event_map, lmx::EXNT_ATTR );
    switch( ar_reader.get_current_event() )
    {
    case -1:    // Avoid warning saying only default: present
    break;
    }
    LMX_NO_WARN_UNUSED( ar_reader );
    LMX_NO_WARN_UNUSED( ap_error );
    return false;
}

lmx::elmx_error c_Body::unmarshal_attributes_check( c_xml_reader_soap & ar_reader )
{
    LMX_NO_WARN_UNUSED( ar_reader );
    return lmx::ELMX_OK;
}

void c_Body::unmarshal_anyattribute( c_xml_reader_soap & ar_reader )
{
    std::string l_namespace;
    ar_reader.get_namespace( ar_reader.name, &l_namespace );
    any_attribute.push_back_w_autop( new lmx::c_any_info( l_namespace, ar_reader.name, ar_reader.value ) );
}

bool c_Body::unmarshal_body( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error )
{
    if( (*ap_error = on_lmx_unmarshal_body_start( ar_reader )) != lmx::ELMX_OK )
        return false;
    if( ar_reader.get_current_event() == lmx::EXE_ELEMENT_END )
        return true;
    ar_reader.tokenise( elem_event_map, lmx::EXNT_ELEM );
    while( ar_reader.is_current_event_tokenisable() )
    {
        ar_reader.set_code_line( __LINE__ );
        *ap_error = ar_reader.unmarshal_any_child_element( m_any, elem_event_map );
        if( *ap_error != lmx::ELMX_OK )
            return false;
    }
    return true;
}

const lmx::s_event_map c_detail::attr_event_map[] =
{
    { 0,                LMXNULL,                                0,  lmx::EXE_UNKNOWN }
};

const lmx::s_event_map c_detail::elem_event_map[] =
{
    { 0,                LMXNULL,                                0,  lmx::EXE_UNKNOWN }
};

c_detail::c_detail()
{
    init();
}

void c_detail::init()
{
}

c_detail::~c_detail()
{
}

void c_detail::getany_attribute( size_t a_index, std::string * ap_namespace, std::string * ap_name, std::string * ap_value ) const
{
    lmx_assert( a_index < any_attribute.size() );
    if( ap_namespace )
        *ap_namespace = any_attribute[ a_index ].get_namespace();
    *ap_name = any_attribute[ a_index ].get_name();
    *ap_value = any_attribute[ a_index ].get_value();
}

lmx::elmx_error c_detail::marshal( c_xml_writer_soap & ar_writer, const char * ap_name ) const
{
    lmx_assert( is_occurs_ok() );
    lmx::c_xml_writer_local l_xml_writer_local( ar_writer );
    ar_writer.start_element( ap_name );
    ar_writer.conditionally_select_ns_map( ns_map_writer_1 );
    ar_writer.conditionally_write_ns_attrs( false );
    marshal_attributes( ar_writer );
    lmx::elmx_error l_error;
    if( (l_error = marshal_child_elements( ar_writer )) != lmx::ELMX_OK )
        return l_error;
    ar_writer.end_element( ap_name );
    return lmx::ELMX_OK;
}

void c_detail::marshal_attributes( c_xml_writer_soap & ar_writer ) const
{
    ar_writer.marshal_any_attribute( any_attribute );
}

lmx::elmx_error c_detail::marshal_child_elements( c_xml_writer_soap & ar_writer ) const
{
    LMX_NO_WARN_UNUSED( ar_writer );
    {
    lmx::elmx_error l_event_error = on_lmx_marshal_body_start( ar_writer );
    if( l_event_error != lmx::ELMX_OK )
        return l_event_error;
    }
    lmx::elmx_error l_error = lmx::ELMX_OK;
    ar_writer.marshal_any_element( m_any );
    return l_error;
}

class c_detail_unmarshal_helper : public lmx::c_unmarshal_helper
{
private:
    c_detail *p_parent;
    c_xml_reader_soap & r_reader;
public:
    c_detail_unmarshal_helper( c_detail * ap_parent, c_xml_reader_soap & ar_reader )
        : lmx::c_unmarshal_helper( ar_reader ), p_parent( ap_parent ), r_reader( ar_reader ) {}
    virtual bool unmarshal_an_attribute( lmx::elmx_error * ap_error )
            { return p_parent->unmarshal_attributes( r_reader, ap_error ); }
    virtual lmx::elmx_error unmarshal_attributes_check()
           { return p_parent->unmarshal_attributes_check( r_reader ); }
    virtual bool unmarshal_complex_body( lmx::elmx_error * ap_error )
           { return p_parent->unmarshal_body( r_reader, ap_error ); }
};

lmx::elmx_error c_detail::unmarshal( c_xml_reader_soap & ar_reader, const std::string & ar_name )
{
    if( &ar_reader.name != &ar_name )
        ar_reader.name = ar_name;
    lmx::elmx_error l_error;
    c_detail_unmarshal_helper l_helper( this, ar_reader );
    l_helper.set_anyattributes_store( & getany_attributes() );
    l_helper.set_body_can_be_empty();
    l_error = l_helper.unmarshal_complex_content( __FILE__, __LINE__ );
    return l_error;
}

bool c_detail::unmarshal_attributes( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error )
{
    ar_reader.tokenise( attr_event_map, lmx::EXNT_ATTR );
    switch( ar_reader.get_current_event() )
    {
    case -1:    // Avoid warning saying only default: present
    break;
    }
    LMX_NO_WARN_UNUSED( ar_reader );
    LMX_NO_WARN_UNUSED( ap_error );
    return false;
}

lmx::elmx_error c_detail::unmarshal_attributes_check( c_xml_reader_soap & ar_reader )
{
    LMX_NO_WARN_UNUSED( ar_reader );
    return lmx::ELMX_OK;
}

void c_detail::unmarshal_anyattribute( c_xml_reader_soap & ar_reader )
{
    std::string l_namespace;
    ar_reader.get_namespace( ar_reader.name, &l_namespace );
    any_attribute.push_back_w_autop( new lmx::c_any_info( l_namespace, ar_reader.name, ar_reader.value ) );
}

bool c_detail::unmarshal_body( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error )
{
    if( (*ap_error = on_lmx_unmarshal_body_start( ar_reader )) != lmx::ELMX_OK )
        return false;
    if( ar_reader.get_current_event() == lmx::EXE_ELEMENT_END )
        return true;
    ar_reader.tokenise( elem_event_map, lmx::EXNT_ELEM );
    while( ar_reader.is_current_event_tokenisable() )
    {
        ar_reader.set_code_line( __LINE__ );
        *ap_error = ar_reader.unmarshal_any_child_element( m_any, elem_event_map );
        if( *ap_error != lmx::ELMX_OK )
            return false;
    }
    return true;
}

const lmx::s_event_map c_Envelope::attr_event_map[] =
{
    { 0,                LMXNULL,                                0,  lmx::EXE_UNKNOWN }
};

const lmx::s_event_map c_Envelope::elem_event_map[] =
{
    { 7,                "Header",                               6,  e_NS7_Header },
    { 7,                "Body",                                 4,  e_NS7_Body },
    { 0,                LMXNULL,                                0,  lmx::EXE_UNKNOWN }
};

c_Envelope::c_Envelope()
{
    init();
}

c_Envelope::c_Envelope( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error )
{
    *ap_error = lmx::ELMX_UNDEFINED_ERROR;
    init();
    *ap_error = unmarshal( ar_reader );
}

void c_Envelope::init()
{
}

c_Envelope::~c_Envelope()
{
}

void c_Envelope::getany_attribute( size_t a_index, std::string * ap_namespace, std::string * ap_name, std::string * ap_value ) const
{
    lmx_assert( a_index < any_attribute.size() );
    if( ap_namespace )
        *ap_namespace = any_attribute[ a_index ].get_namespace();
    *ap_name = any_attribute[ a_index ].get_name();
    *ap_value = any_attribute[ a_index ].get_value();
}

lmx::elmx_error c_Envelope::marshal( c_xml_writer_soap & ar_writer, const char * ap_name ) const
{
    {
    lmx::elmx_error l_event_error = on_lmx_marshal_start( ar_writer, ap_name );
    if( l_event_error != lmx::ELMX_OK )
        return l_event_error;
    }
    lmx_assert( is_occurs_ok() );
    lmx::c_xml_writer_local l_xml_writer_local( ar_writer );
    ar_writer.start_element( ap_name );
    ar_writer.conditionally_select_ns_map( ns_map_writer_1 );
    ar_writer.conditionally_write_ns_attrs( false );
    marshal_attributes( ar_writer );
    lmx::elmx_error l_error;
    if( (l_error = marshal_child_elements( ar_writer )) != lmx::ELMX_OK )
        return l_error;
    ar_writer.end_element( ap_name );
    return lmx::ELMX_OK;
}

void c_Envelope::marshal_attributes( c_xml_writer_soap & ar_writer ) const
{
    ar_writer.marshal_any_attribute( any_attribute );
}

lmx::elmx_error c_Envelope::marshal_child_elements( c_xml_writer_soap & ar_writer ) const
{
    LMX_NO_WARN_UNUSED( ar_writer );
    lmx::elmx_error l_error = lmx::ELMX_OK;
    if( m_Header.is_value_set() )
    {
        if( (l_error = m_Header.get().marshal( ar_writer, "SOAP-ENV:Header" )) != lmx::ELMX_OK )
            return l_error;
    }
    if( (l_error = m_Body.get().marshal( ar_writer, "SOAP-ENV:Body" )) != lmx::ELMX_OK )
        return l_error;
    ar_writer.marshal_any_element( m_any );
    return l_error;
}

lmx::elmx_error c_Envelope::unmarshal( c_xml_reader_soap & ar_reader )
{
    ar_reader.set_code_file( __FILE__ );
    ar_reader.set_ns_map( ns_map_reader );
    lmx::elmx_error l_error = lmx::ELMX_OK;
    lmx::c_xml_reader_local l_reader_local( &ar_reader );
    ar_reader.get_element_event( &l_error );
    if( l_error != lmx::ELMX_OK && (l_error = ar_reader.handle_error( l_error, __LINE__ )) != lmx::ELMX_OK )
        return l_error;
    if( (ar_reader.get_element_ns_id() != 7 || ar_reader.get_local_name() != "Envelope") && (l_error = ar_reader.handle_error( lmx::ELMX_ELEMENT_NOT_FOUND, __LINE__ )) != lmx::ELMX_OK )
        return l_error;
    return unmarshal( ar_reader, ar_reader.name );
}

class c_Envelope_unmarshal_helper : public lmx::c_unmarshal_helper
{
private:
    c_Envelope *p_parent;
    c_xml_reader_soap & r_reader;
public:
    c_Envelope_unmarshal_helper( c_Envelope * ap_parent, c_xml_reader_soap & ar_reader )
        : lmx::c_unmarshal_helper( ar_reader ), p_parent( ap_parent ), r_reader( ar_reader ) {}
    virtual bool unmarshal_an_attribute( lmx::elmx_error * ap_error )
            { return p_parent->unmarshal_attributes( r_reader, ap_error ); }
    virtual lmx::elmx_error unmarshal_attributes_check()
           { return p_parent->unmarshal_attributes_check( r_reader ); }
    virtual bool unmarshal_complex_body( lmx::elmx_error * ap_error )
           { return p_parent->unmarshal_body( r_reader, ap_error ); }
};

lmx::elmx_error c_Envelope::unmarshal( c_xml_reader_soap & ar_reader, const std::string & ar_name )
{
    if( &ar_reader.name != &ar_name )
        ar_reader.name = ar_name;
    lmx::elmx_error l_error;
    c_Envelope_unmarshal_helper l_helper( this, ar_reader );
    l_helper.set_anyattributes_store( & getany_attributes() );
    l_error = l_helper.unmarshal_complex_content( __FILE__, __LINE__ );
    return l_error;
}

bool c_Envelope::unmarshal_attributes( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error )
{
    ar_reader.tokenise( attr_event_map, lmx::EXNT_ATTR );
    switch( ar_reader.get_current_event() )
    {
    case -1:    // Avoid warning saying only default: present
    break;
    }
    LMX_NO_WARN_UNUSED( ar_reader );
    LMX_NO_WARN_UNUSED( ap_error );
    return false;
}

lmx::elmx_error c_Envelope::unmarshal_attributes_check( c_xml_reader_soap & ar_reader )
{
    LMX_NO_WARN_UNUSED( ar_reader );
    return lmx::ELMX_OK;
}

void c_Envelope::unmarshal_anyattribute( c_xml_reader_soap & ar_reader )
{
    std::string l_namespace;
    ar_reader.get_namespace( ar_reader.name, &l_namespace );
    any_attribute.push_back_w_autop( new lmx::c_any_info( l_namespace, ar_reader.name, ar_reader.value ) );
}

bool c_Envelope::unmarshal_body( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error )
{
    ar_reader.tokenise( elem_event_map, lmx::EXNT_ELEM );
    if( ar_reader.get_current_event() == e_NS7_Header )
    {
        ar_reader.set_code_line( __LINE__ );
        if( (*ap_error = m_Header.get().unmarshal( ar_reader, ar_reader.name ) ) != lmx::ELMX_OK )
            return false;
        ar_reader.get_element_event( &elem_event_map[0], ap_error );
        if( *ap_error != lmx::ELMX_OK && (*ap_error = ar_reader.handle_error( *ap_error, __LINE__ )) != lmx::ELMX_OK )
            return false;
    }
    if( ar_reader.get_current_event() == e_NS7_Body )
    {
        ar_reader.set_code_line( __LINE__ );
        if( (*ap_error = m_Body.get().unmarshal( ar_reader, ar_reader.name ) ) != lmx::ELMX_OK )
            return false;
        ar_reader.get_element_event( &elem_event_map[1], ap_error );
        if( *ap_error != lmx::ELMX_OK && (*ap_error = ar_reader.handle_error( *ap_error, __LINE__ )) != lmx::ELMX_OK )
            return false;
    }
    else if( (*ap_error = ar_reader.handle_error( lmx::ELMX_MANDATORY_ELEMENT_MISSING, __LINE__ )) != lmx::ELMX_OK )
        return false;
    while( (ar_reader.is_current_event_tokenisable() &&
            ar_reader.get_namespace( ar_reader.name, &ar_reader.value ) != "http://schemas.xmlsoap.org/soap/envelope/") )
    {
        ar_reader.set_code_line( __LINE__ );
        *ap_error = ar_reader.unmarshal_any_child_element( m_any, elem_event_map );
        if( *ap_error != lmx::ELMX_OK )
            return false;
    }
    return true;
}

const lmx::s_event_map c_Fault::attr_event_map[] =
{
    { 0,                LMXNULL,                                0,  lmx::EXE_UNKNOWN }
};

const lmx::s_event_map c_Fault::elem_event_map[] =
{
    { lmx::EXKN_NONE,   "faultcode",                            9,  e_NONE_faultcode },
    { lmx::EXKN_NONE,   "faultstring",                          11, e_NONE_faultstring },
    { lmx::EXKN_NONE,   "faultactor",                           10, e_NONE_faultactor },
    { lmx::EXKN_NONE,   "detail",                               6,  e_NONE_detail },
    { 0,                LMXNULL,                                0,  lmx::EXE_UNKNOWN }
};

#ifdef LMX_USER_VALIDATE
lmx::elmx_error user_validator_1( const lmx::tc_qname & ar_v, const char * ap_name )
{
    LMX_NO_WARN_UNUSED( ar_v );
    LMX_NO_WARN_UNUSED( ap_name );
    return lmx::ELMX_OK;
}
#endif // LMX_USER_VALIDATE

lmx::elmx_error lexical_validator_1( lmx::c_xml_reader & ar_reader, const std::string & ar_v )
{
    lmx::elmx_error l_error = lmx::ELMX_OK;
    if( ! ar_reader.is_valid_qname( ar_v ) && (l_error = ar_reader.handle_error( lmx::ELMX_VALUE_BAD_FORMAT )) != lmx::ELMX_OK )
        return l_error;
    return lmx::ELMX_OK;
}

lmx::elmx_error value_validator_1( lmx::c_xml_reader & ar_reader, const lmx::tc_qname & ar_v )
{
    LMX_NO_WARN_UNUSED( ar_reader );
    LMX_NO_WARN_UNUSED( ar_v );
    return lmx::ELMX_OK;
}

const lmx::ct_typed_validation_spec< lmx::tc_qname > validation_spec_1(
                lmx::EXWS_COLLAPSE,     // Whitespace handling
                "",                     // Default value (if any)
                &lexical_validator_1,   // Lexical validator function
                &value_validator_1,     // Value validator function
                &lmx::v_to_o_not_used< lmx::tc_qname >, // Lexical to object converter
                &lmx::o_to_v_not_used< lmx::tc_qname > ); // Object to lexical converter

#ifdef LMX_USER_VALIDATE
lmx::elmx_error user_validator_2( const lmx::tlmx_unicode_string & ar_v, const char * ap_name )
{
    LMX_NO_WARN_UNUSED( ar_v );
    LMX_NO_WARN_UNUSED( ap_name );
    return lmx::ELMX_OK;
}
#endif // LMX_USER_VALIDATE

lmx::elmx_error lexical_validator_2( lmx::c_xml_reader & ar_reader, const std::string & ar_v )
{
    LMX_NO_WARN_UNUSED( ar_reader );
    LMX_NO_WARN_UNUSED( ar_v );
    return lmx::ELMX_OK;
}

lmx::elmx_error value_validator_2( lmx::c_xml_reader & ar_reader, const lmx::tlmx_unicode_string & ar_v )
{
    LMX_NO_WARN_UNUSED( ar_reader );
    LMX_NO_WARN_UNUSED( ar_v );
    return lmx::ELMX_OK;
}

const lmx::ct_typed_validation_spec< lmx::tlmx_unicode_string > validation_spec_2(
                lmx::EXWS_PRESERVE,     // Whitespace handling
                "",                     // Default value (if any)
                &lexical_validator_2,   // Lexical validator function
                &value_validator_2,     // Value validator function
                &lmx::v_to_o< lmx::tlmx_unicode_string >, // Lexical to object converter
                &lmx::o_to_v< lmx::tlmx_unicode_string > ); // Object to lexical converter

#ifdef LMX_USER_VALIDATE
lmx::elmx_error user_validator_3( const lmx::tlmx_uri_string & ar_v, const char * ap_name )
{
    LMX_NO_WARN_UNUSED( ar_v );
    LMX_NO_WARN_UNUSED( ap_name );
    return lmx::ELMX_OK;
}
#endif // LMX_USER_VALIDATE

lmx::elmx_error lexical_validator_3( lmx::c_xml_reader & ar_reader, const std::string & ar_v )
{
    LMX_NO_WARN_UNUSED( ar_reader );
    LMX_NO_WARN_UNUSED( ar_v );
    return lmx::ELMX_OK;
}

lmx::elmx_error value_validator_3( lmx::c_xml_reader & ar_reader, const lmx::tlmx_uri_string & ar_v )
{
    LMX_NO_WARN_UNUSED( ar_reader );
    LMX_NO_WARN_UNUSED( ar_v );
    return lmx::ELMX_OK;
}

const lmx::ct_typed_validation_spec< lmx::tlmx_uri_string > validation_spec_3(
                lmx::EXWS_COLLAPSE,     // Whitespace handling
                "",                     // Default value (if any)
                &lexical_validator_3,   // Lexical validator function
                &value_validator_3,     // Value validator function
                &lmx::v_to_o< lmx::tlmx_uri_string >, // Lexical to object converter
                &lmx::o_to_v< lmx::tlmx_uri_string > ); // Object to lexical converter

c_Fault::c_Fault()
{
    init();
}

c_Fault::c_Fault( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error )
{
    *ap_error = lmx::ELMX_UNDEFINED_ERROR;
    init();
    *ap_error = unmarshal( ar_reader );
}

void c_Fault::init()
{
}

c_Fault::~c_Fault()
{
}

bool c_Fault::is_occurs_ok() const
{
    if( ! m_faultcode.is_occurs_ok( true ) ||
            ! m_faultstring.is_occurs_ok( true )  )
        return false;
    return true;
}

lmx::elmx_error c_Fault::marshal( c_xml_writer_soap & ar_writer, const char * ap_name ) const
{
    {
    lmx::elmx_error l_event_error = on_lmx_marshal_start( ar_writer, ap_name );
    if( l_event_error != lmx::ELMX_OK )
        return l_event_error;
    }
    lmx_assert( is_occurs_ok() );
    lmx::c_xml_writer_local l_xml_writer_local( ar_writer );
    ar_writer.start_element( ap_name );
    ar_writer.conditionally_select_ns_map( ns_map_writer_1 );
    ar_writer.conditionally_write_ns_attrs( false );
    lmx::elmx_error l_error;
    if( (l_error = marshal_child_elements( ar_writer )) != lmx::ELMX_OK )
        return l_error;
    ar_writer.end_element( ap_name );
    return lmx::ELMX_OK;
}

lmx::elmx_error c_Fault::marshal_child_elements( c_xml_writer_soap & ar_writer ) const
{
    LMX_NO_WARN_UNUSED( ar_writer );
    lmx::elmx_error l_error = lmx::ELMX_OK;
    ar_writer.marshal_element( "faultcode", m_faultcode, validation_spec_1 );
    ar_writer.marshal_element( "faultstring", m_faultstring, validation_spec_2 );
    ar_writer.marshal_element( "faultactor", m_faultactor, validation_spec_3 );
    if( m_detail.is_value_set() )
    {
        if( (l_error = m_detail.get().marshal( ar_writer, "detail" )) != lmx::ELMX_OK )
            return l_error;
    }
    return l_error;
}

lmx::elmx_error c_Fault::unmarshal( c_xml_reader_soap & ar_reader )
{
    ar_reader.set_code_file( __FILE__ );
    ar_reader.set_ns_map( ns_map_reader );
    lmx::elmx_error l_error = lmx::ELMX_OK;
    lmx::c_xml_reader_local l_reader_local( &ar_reader );
    ar_reader.get_element_event( &l_error );
    if( l_error != lmx::ELMX_OK && (l_error = ar_reader.handle_error( l_error, __LINE__ )) != lmx::ELMX_OK )
        return l_error;
    if( (ar_reader.get_element_ns_id() != 7 || ar_reader.get_local_name() != "Fault") && (l_error = ar_reader.handle_error( lmx::ELMX_ELEMENT_NOT_FOUND, __LINE__ )) != lmx::ELMX_OK )
        return l_error;
    return unmarshal( ar_reader, ar_reader.name );
}

class c_Fault_unmarshal_helper : public lmx::c_unmarshal_helper
{
private:
    c_Fault *p_parent;
    c_xml_reader_soap & r_reader;
public:
    c_Fault_unmarshal_helper( c_Fault * ap_parent, c_xml_reader_soap & ar_reader )
        : lmx::c_unmarshal_helper( ar_reader ), p_parent( ap_parent ), r_reader( ar_reader ) {}
    virtual bool unmarshal_an_attribute( lmx::elmx_error * ap_error )
            { return p_parent->unmarshal_attributes( r_reader, ap_error ); }
    virtual lmx::elmx_error unmarshal_attributes_check()
           { return p_parent->unmarshal_attributes_check( r_reader ); }
    virtual bool unmarshal_complex_body( lmx::elmx_error * ap_error )
           { return p_parent->unmarshal_body( r_reader, ap_error ); }
};

lmx::elmx_error c_Fault::unmarshal( c_xml_reader_soap & ar_reader, const std::string & ar_name )
{
    if( &ar_reader.name != &ar_name )
        ar_reader.name = ar_name;
    lmx::elmx_error l_error;
    c_Fault_unmarshal_helper l_helper( this, ar_reader );
    l_error = l_helper.unmarshal_complex_content( __FILE__, __LINE__ );
    return l_error;
}

bool c_Fault::unmarshal_attributes( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error )
{
    LMX_NO_WARN_UNUSED( ar_reader );
    LMX_NO_WARN_UNUSED( ap_error );
    return false;
}

lmx::elmx_error c_Fault::unmarshal_attributes_check( c_xml_reader_soap & ar_reader )
{
    LMX_NO_WARN_UNUSED( ar_reader );
    return lmx::ELMX_OK;
}

bool c_Fault::unmarshal_body( c_xml_reader_soap & ar_reader, lmx::elmx_error * ap_error )
{
    ar_reader.tokenise( elem_event_map, lmx::EXNT_ELEM );
    if( ar_reader.get_current_event() == e_NONE_faultcode )
    {
        ar_reader.set_code_line( __LINE__ );
        *ap_error = ar_reader.unmarshal_child_element( m_faultcode, validation_spec_1, &elem_event_map[0] );
        if( *ap_error != lmx::ELMX_OK )
            return false;
    }
    else if( (*ap_error = ar_reader.handle_error( lmx::ELMX_MANDATORY_ELEMENT_MISSING, __LINE__ )) != lmx::ELMX_OK )
        return false;
    if( ar_reader.get_current_event() == e_NONE_faultstring )
    {
        ar_reader.set_code_line( __LINE__ );
        *ap_error = ar_reader.unmarshal_child_element( m_faultstring, validation_spec_2, &elem_event_map[1] );
        if( *ap_error != lmx::ELMX_OK )
            return false;
    }
    else if( (*ap_error = ar_reader.handle_error( lmx::ELMX_MANDATORY_ELEMENT_MISSING, __LINE__ )) != lmx::ELMX_OK )
        return false;
    if( ar_reader.get_current_event() == e_NONE_faultactor )
    {
        ar_reader.set_code_line( __LINE__ );
        *ap_error = ar_reader.unmarshal_child_element( m_faultactor, validation_spec_3, &elem_event_map[2] );
        if( *ap_error != lmx::ELMX_OK )
            return false;
    }
    if( ar_reader.get_current_event() == e_NONE_detail )
    {
        ar_reader.set_code_line( __LINE__ );
        if( (*ap_error = m_detail.get().unmarshal( ar_reader, ar_reader.name ) ) != lmx::ELMX_OK )
            return false;
        ar_reader.get_element_event( &elem_event_map[3], ap_error );
        if( *ap_error != lmx::ELMX_OK && (*ap_error = ar_reader.handle_error( *ap_error, __LINE__ )) != lmx::ELMX_OK )
            return false;
    }
    return true;
}

}   // end of namespace lmx


//------------------------------------------------------------------------------
//                     Copyright (c) 2003-2011 Codalogic Ltd.
//------------------------------------------------------------------------------
