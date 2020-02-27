#if defined( _MSC_VER ) && _MSC_VER > 1000
#pragma warning (disable:4786)  // Disable truncated debug info warning
#pragma warning (disable:4100)  // Disable unreferenced formal parameter
#pragma warning (disable:4702)  // Disable unreachable code
#endif

#include <algorithm>
#include <memory>
#ifndef LMX_GEN_SOURCE
#define LMX_GEN_SOURCE
#endif

#include "xml-settings.h"
#include <lmx/lmxparse.h>

#if defined( _MSC_VER ) && _MSC_VER <= 1200
using namespace lmx;
#endif

const lmx::s_ns_map ns_map_reader[] =
{
    { 0,    LMXNULL,    LMXNULL }
};

const lmx::s_ns_map ns_map_writer_1[] =
{
    { 0,    LMXNULL,    LMXNULL }
};

const lmx::s_event_map c_settings::attr_event_map[] =
{
    { 0,                LMXNULL,                                0,  lmx::EXE_UNKNOWN }
};

const lmx::s_event_map c_settings::elem_event_map[] =
{
    { lmx::EXKN_NONE,   "windowWidth",                          11, e_NONE_windowWidth },
    { lmx::EXKN_NONE,   "windowHeight",                         12, e_NONE_windowHeight },
    { lmx::EXKN_NONE,   "cameraMovespeed",                      15, e_NONE_cameraMovespeed },
    { lmx::EXKN_NONE,   "cameraMaxZoom",                        13, e_NONE_cameraMaxZoom },
    { lmx::EXKN_NONE,   "language",                             8,  e_NONE_language },
    { lmx::EXKN_NONE,   "debug",                                5,  e_NONE_debug },
    { lmx::EXKN_NONE,   "fullScreen",                           10, e_NONE_fullScreen },
    { 0,                LMXNULL,                                0,  lmx::EXE_UNKNOWN }
};

const lmx::c_big_int constant_1( "0" );

const lmx::c_big_int constant_2( "65535" );

#ifdef LMX_USER_VALIDATE
lmx::elmx_error user_validator_1( const lmx::tlmx_uns16 & r_value, const char * p_name )
{
    LMX_NO_WARN_UNUSED( r_value );
    LMX_NO_WARN_UNUSED( p_name );
    return lmx::ELMX_OK;
}
#endif // LMX_USER_VALIDATE

lmx::elmx_error lexical_validator_1( lmx::c_xml_reader & r_reader, const std::string & r_value )
{
    lmx::elmx_error error = lmx::ELMX_OK;
    if( ! lmx::is_valid_integer( r_value ) &&
            (error = r_reader.handle_error( lmx::ELMX_VALUE_BAD_FORMAT )) != lmx::ELMX_OK )
        return error;
    if( ! ( r_value >= constant_1 ) &&
            (error = r_reader.handle_error( lmx::ELMX_VALUE_EXCEEDS_MIN )) != lmx::ELMX_OK )
        return error;
    if( ! ( r_value <= constant_2 ) &&
            (error = r_reader.handle_error( lmx::ELMX_VALUE_EXCEEDS_MAX )) != lmx::ELMX_OK )
        return error;
    return lmx::ELMX_OK;
}

lmx::elmx_error value_validator_1( lmx::c_xml_reader & r_reader, const lmx::tlmx_uns16 & r_value )
{
    LMX_NO_WARN_UNUSED( r_reader );
    LMX_NO_WARN_UNUSED( r_value );
    return lmx::ELMX_OK;
}

const lmx::ct_typed_validation_spec< lmx::tlmx_uns16 > validation_spec_1(
                lmx::EXWS_COLLAPSE,     // Whitespace handling
                "",                     // Default value (if any)
                &lexical_validator_1,   // Lexical validator function
                &value_validator_1,     // Value validator function
                &lmx::v_to_o< lmx::tlmx_uns16 >, // Lexical to object converter
                &lmx::o_to_v< lmx::tlmx_uns16 > ); // Object to lexical converter

const lmx::c_big_int constant_3( "255" );

#ifdef LMX_USER_VALIDATE
lmx::elmx_error user_validator_2( const lmx::tlmx_uns8 & r_value, const char * p_name )
{
    LMX_NO_WARN_UNUSED( r_value );
    LMX_NO_WARN_UNUSED( p_name );
    return lmx::ELMX_OK;
}
#endif // LMX_USER_VALIDATE

lmx::elmx_error lexical_validator_2( lmx::c_xml_reader & r_reader, const std::string & r_value )
{
    lmx::elmx_error error = lmx::ELMX_OK;
    if( ! lmx::is_valid_integer( r_value ) &&
            (error = r_reader.handle_error( lmx::ELMX_VALUE_BAD_FORMAT )) != lmx::ELMX_OK )
        return error;
    if( ! ( r_value >= constant_1 ) &&
            (error = r_reader.handle_error( lmx::ELMX_VALUE_EXCEEDS_MIN )) != lmx::ELMX_OK )
        return error;
    if( ! ( r_value <= constant_3 ) &&
            (error = r_reader.handle_error( lmx::ELMX_VALUE_EXCEEDS_MAX )) != lmx::ELMX_OK )
        return error;
    return lmx::ELMX_OK;
}

lmx::elmx_error value_validator_2( lmx::c_xml_reader & r_reader, const lmx::tlmx_uns8 & r_value )
{
    LMX_NO_WARN_UNUSED( r_reader );
    LMX_NO_WARN_UNUSED( r_value );
    return lmx::ELMX_OK;
}

const lmx::ct_typed_validation_spec< lmx::tlmx_uns8 > validation_spec_2(
                lmx::EXWS_COLLAPSE,     // Whitespace handling
                "",                     // Default value (if any)
                &lexical_validator_2,   // Lexical validator function
                &value_validator_2,     // Value validator function
                &lmx::v_to_o< lmx::tlmx_uns8 >, // Lexical to object converter
                &lmx::o_to_v< lmx::tlmx_uns8 > ); // Object to lexical converter

#ifdef LMX_USER_VALIDATE
lmx::elmx_error user_validator_3( const lmx::tlmx_unicode_string & r_value, const char * p_name )
{
    LMX_NO_WARN_UNUSED( r_value );
    LMX_NO_WARN_UNUSED( p_name );
    return lmx::ELMX_OK;
}
#endif // LMX_USER_VALIDATE

lmx::elmx_error lexical_validator_3( lmx::c_xml_reader & r_reader, const std::string & r_value )
{
    LMX_NO_WARN_UNUSED( r_reader );
    LMX_NO_WARN_UNUSED( r_value );
    return lmx::ELMX_OK;
}

lmx::elmx_error value_validator_3( lmx::c_xml_reader & r_reader, const lmx::tlmx_unicode_string & r_value )
{
    LMX_NO_WARN_UNUSED( r_reader );
    LMX_NO_WARN_UNUSED( r_value );
    return lmx::ELMX_OK;
}

const lmx::ct_typed_validation_spec< lmx::tlmx_unicode_string > validation_spec_3(
                lmx::EXWS_PRESERVE,     // Whitespace handling
                "",                     // Default value (if any)
                &lexical_validator_3,   // Lexical validator function
                &value_validator_3,     // Value validator function
                &lmx::v_to_o< lmx::tlmx_unicode_string >, // Lexical to object converter
                &lmx::o_to_v< lmx::tlmx_unicode_string > ); // Object to lexical converter

#ifdef LMX_USER_VALIDATE
lmx::elmx_error user_validator_4( const bool & r_value, const char * p_name )
{
    LMX_NO_WARN_UNUSED( r_value );
    LMX_NO_WARN_UNUSED( p_name );
    return lmx::ELMX_OK;
}
#endif // LMX_USER_VALIDATE

lmx::elmx_error lexical_validator_4( lmx::c_xml_reader & r_reader, const std::string & r_value )
{
    lmx::elmx_error error = lmx::ELMX_OK;
    if( ! lmx::is_valid_bool( r_value ) &&
            (error = r_reader.handle_error( lmx::ELMX_VALUE_BAD_FORMAT )) != lmx::ELMX_OK )
        return error;
    return lmx::ELMX_OK;
}

lmx::elmx_error value_validator_4( lmx::c_xml_reader & r_reader, const bool & r_value )
{
    LMX_NO_WARN_UNUSED( r_reader );
    LMX_NO_WARN_UNUSED( r_value );
    return lmx::ELMX_OK;
}

const lmx::ct_typed_validation_spec< bool > validation_spec_4(
                lmx::EXWS_COLLAPSE,     // Whitespace handling
                "",                     // Default value (if any)
                &lexical_validator_4,   // Lexical validator function
                &value_validator_4,     // Value validator function
                &lmx::v_to_o< bool >,   // Lexical to object converter
                &lmx::o_to_v< bool > ); // Object to lexical converter

c_settings::c_settings()
{
    init();
}

c_settings::c_settings( lmx::c_xml_reader & r_reader, lmx::elmx_error * p_error )
{
    *p_error = lmx::ELMX_UNDEFINED_ERROR;
    init();
    *p_error = unmarshal( r_reader );
}

c_settings::c_settings( const char c_file_name[], lmx::elmx_error * p_error, lmx::s_debug_error * p_debug_error /*= LMXNULL*/ )
{
    *p_error = lmx::ELMX_UNDEFINED_ERROR;
    init();
    *p_error = unmarshal( c_file_name, p_debug_error );
}

#if LMX_WANT_WIDE_FILE_NAMES == 1
c_settings::c_settings( const wchar_t c_file_name[], lmx::elmx_error * p_error, lmx::s_debug_error * p_debug_error /*= LMXNULL*/ )
{
    *p_error = lmx::ELMX_UNDEFINED_ERROR;
    init();
    *p_error = unmarshal( c_file_name, p_debug_error );
}
#endif

c_settings::c_settings( const char * p_memory, size_t memory_size, lmx::elmx_error * p_error, lmx::s_debug_error * p_debug_error /*= LMXNULL*/ )
{
    *p_error = lmx::ELMX_UNDEFINED_ERROR;
    init();
    *p_error = unmarshal( p_memory, memory_size, p_debug_error );
}

c_settings::c_settings( const std::string & r_string, lmx::elmx_error * p_error, lmx::s_debug_error * p_debug_error /*= LMXNULL*/ )
{
    *p_error = lmx::ELMX_UNDEFINED_ERROR;
    init();
    *p_error = unmarshal( r_string.data(), r_string.size(), p_debug_error );
}

c_settings::c_settings( const lmx::c_any_info & r_any_info, lmx::elmx_error * p_error, lmx::s_debug_error * p_debug_error /*= LMXNULL*/ )
{
    *p_error = lmx::ELMX_UNDEFINED_ERROR;
    init();
    *p_error = unmarshal( r_any_info, p_debug_error );
}

void c_settings::init()
{
}

void c_settings::reset()
{
    c_settings l_temp;
    swap( l_temp );
}

c_settings::~c_settings()
{
}

c_settings::c_settings( const c_settings & r_rhs )
{
    LMX_NO_WARN_UNUSED( r_rhs );
    init();
    m_windowWidth = r_rhs.m_windowWidth;
    m_windowHeight = r_rhs.m_windowHeight;
    m_cameraMovespeed = r_rhs.m_cameraMovespeed;
    m_cameraMaxZoom = r_rhs.m_cameraMaxZoom;
    m_language = r_rhs.m_language;
    m_debug = r_rhs.m_debug;
    m_fullScreen = r_rhs.m_fullScreen;
}

void c_settings::swap( c_settings & r_rhs )
{
    m_windowWidth.swap( r_rhs.m_windowWidth );
    m_windowHeight.swap( r_rhs.m_windowHeight );
    m_cameraMovespeed.swap( r_rhs.m_cameraMovespeed );
    m_cameraMaxZoom.swap( r_rhs.m_cameraMaxZoom );
    m_language.swap( r_rhs.m_language );
    m_debug.swap( r_rhs.m_debug );
    m_fullScreen.swap( r_rhs.m_fullScreen );
}

c_settings & c_settings::operator = ( const c_settings & r_rhs )
{
    c_settings l_temp( r_rhs );
    swap( l_temp );
    return *this;
}

c_settings * c_settings::clone() const
{
    return new c_settings( *this );
}

lmx::elmx_error c_settings::set_windowWidth( const lmx::tlmx_uns16 & value )
{
    return m_windowWidth.set( value );
}

lmx::elmx_error c_settings::set_windowHeight( const lmx::tlmx_uns16 & value )
{
    return m_windowHeight.set( value );
}

lmx::elmx_error c_settings::set_cameraMovespeed( const lmx::tlmx_uns8 & value )
{
    return m_cameraMovespeed.set( value );
}

lmx::elmx_error c_settings::set_cameraMaxZoom( const lmx::tlmx_uns8 & value )
{
    return m_cameraMaxZoom.set( value );
}

bool c_settings::is_occurs_ok( lmx::c_check_tracker * p_tracker_in /*= LMXNULL*/ ) const
{
    lmx::c_check_tracker check_tracker( p_tracker_in );
    if( check_tracker.named_check( ! m_windowWidth.is_occurs_ok( true ), "windowWidth" ) ||
            check_tracker.named_check( ! m_windowHeight.is_occurs_ok( true ), "windowHeight" ) ||
            check_tracker.named_check( ! m_cameraMovespeed.is_occurs_ok( true ), "cameraMovespeed" ) ||
            check_tracker.named_check( ! m_cameraMaxZoom.is_occurs_ok( true ), "cameraMaxZoom" ) ||
            check_tracker.named_check( ! m_language.is_occurs_ok( true ), "language" ) ||
            check_tracker.named_check( ! m_debug.is_occurs_ok( true ), "debug" ) ||
            check_tracker.named_check( ! m_fullScreen.is_occurs_ok( true ), "fullScreen" ) )
        return check_tracker.report( __FILE__, __LINE__ );
    return true;
}

bool c_settings::check( lmx::c_check_tracker * p_tracker_in /*= LMXNULL*/, const char * p_name_in ) const
{
    lmx::c_check_tracker check_tracker( p_tracker_in, p_name_in );
    if( ! is_occurs_ok( &check_tracker ) )
        return false;
    return true;
}

lmx::elmx_error c_settings::marshal( const char c_file_name[], lmx::s_debug_error * p_debug_error /*= LMXNULL*/ ) const
{
    return lmx::marshal( *this, c_file_name, p_debug_error );
}

#if defined( _MSC_VER ) && _MSC_VER >= 1400
lmx::elmx_error c_settings::marshal( const wchar_t c_file_name[], lmx::s_debug_error * p_debug_error /*= LMXNULL*/ ) const
{
    return lmx::marshal( *this, c_file_name, p_debug_error );
}
#endif

lmx::elmx_error c_settings::marshal( std::string * p_string, lmx::s_debug_error * p_debug_error /*= LMXNULL*/ ) const
{
    return lmx::marshal( *this, p_string, p_debug_error );
}

lmx::elmx_error c_settings::marshal( std::ostream & r_sos, lmx::s_debug_error * p_debug_error /*= LMXNULL*/ ) const
{
    return lmx::marshal( *this, r_sos, p_debug_error );
}

lmx::elmx_error c_settings::marshal( lmx::c_xml_writer & r_writer, const char * p_name ) const
{
    lmx_assert_is_occurs_ok( is_occurs_ok() );
    lmx::c_xml_writer_local l_xml_writer_local( r_writer );
    r_writer.start_element( p_name );
    r_writer.conditionally_select_ns_map( ns_map_writer_1 );
    r_writer.conditionally_write_ns_attrs( false );
    lmx::elmx_error error;
    if( (error = marshal_child_elements( r_writer )) != lmx::ELMX_OK )
        return error;
    r_writer.end_element( p_name );
    return lmx::ELMX_OK;
}

lmx::elmx_error c_settings::marshal_child_elements( lmx::c_xml_writer & r_writer ) const
{
    LMX_NO_WARN_UNUSED( r_writer );
    lmx::elmx_error error = lmx::ELMX_OK;
    r_writer.marshal_element( "windowWidth", m_windowWidth, validation_spec_1 );
    r_writer.marshal_element( "windowHeight", m_windowHeight, validation_spec_1 );
    r_writer.marshal_element( "cameraMovespeed", m_cameraMovespeed, validation_spec_2 );
    r_writer.marshal_element( "cameraMaxZoom", m_cameraMaxZoom, validation_spec_2 );
    r_writer.marshal_element( "language", m_language, validation_spec_3 );
    r_writer.marshal_element( "debug", m_debug, validation_spec_4 );
    r_writer.marshal_element( "fullScreen", m_fullScreen, validation_spec_4 );
    return error;
}

lmx::elmx_error c_settings::unmarshal( const char c_file_name[], lmx::s_debug_error * p_debug_error /*= LMXNULL*/ )
{
    return lmx::unmarshal( this, c_file_name, p_debug_error );
}

#if LMX_WANT_WIDE_FILE_NAMES == 1
lmx::elmx_error c_settings::unmarshal( const wchar_t c_file_name[], lmx::s_debug_error * p_debug_error /*= LMXNULL*/ )
{
    return lmx::unmarshal( this, c_file_name, p_debug_error );
}
#endif

lmx::elmx_error c_settings::unmarshal( const char * p_memory, size_t memory_size, lmx::s_debug_error * p_debug_error /*= LMXNULL*/ )
{
    return lmx::unmarshal( this, p_memory, memory_size, p_debug_error );
}

lmx::elmx_error c_settings::unmarshal( const lmx::c_any_info & r_any_info, lmx::s_debug_error * p_debug_error /*= LMXNULL*/ )
{
    return lmx::unmarshal( this, r_any_info, p_debug_error );
}

lmx::elmx_error c_settings::unmarshal( lmx::c_xml_reader & r_reader )
{
    r_reader.set_code_file( __FILE__ );
    r_reader.set_ns_map( ns_map_reader );
    lmx::elmx_error error = lmx::ELMX_OK;
    lmx::c_xml_reader_local l_reader_local( &r_reader );
    r_reader.get_element_event( &error );
    if( error != lmx::ELMX_OK && (error = r_reader.handle_error( error, __LINE__ )) != lmx::ELMX_OK )
        return error;
    if( (r_reader.get_element_ns_id() != lmx::EXKN_NONE || r_reader.get_local_name() != "settings") && (error = r_reader.handle_error( lmx::ELMX_ELEMENT_NOT_FOUND, "settings", __FILE__, __LINE__ )) != lmx::ELMX_OK )
        return error;
    error = unmarshal( r_reader, r_reader.name );
    return error;
}

lmx::elmx_error c_settings::unmarshal( lmx::c_xml_reader & r_reader, const std::string & r_name )
{
    if( &r_reader.name != &r_name )
        r_reader.name = r_name;
    lmx::elmx_error error;
    class c_settings_unmarshal_helper : public lmx::c_unmarshal_helper
    {
    private:
        c_settings * p_parent;
        lmx::c_xml_reader & r_reader;
    public:
        c_settings_unmarshal_helper( c_settings * p_parent_in, lmx::c_xml_reader & r_reader_in )
            : lmx::c_unmarshal_helper( r_reader_in ), p_parent( p_parent_in ), r_reader( r_reader_in ) {}
        virtual bool unmarshal_an_attribute( lmx::elmx_error * p_error )
            { return p_parent->unmarshal_attributes( r_reader, p_error ); }
        virtual lmx::elmx_error unmarshal_attributes_check()
            { return p_parent->unmarshal_attributes_check( r_reader ); }
        virtual bool unmarshal_complex_body( lmx::elmx_error * p_error )
            { return p_parent->unmarshal_body( r_reader, p_error ); }
    };
    // If your compiler generates errors concerning access to private/protected methods not allowed, 
    // compile schema using LMX's -no-local-classes flag.
    c_settings_unmarshal_helper helper( this, r_reader );
    error = helper.unmarshal_complex_content( __FILE__, __LINE__ );
    return error;
}

bool c_settings::unmarshal_attributes( lmx::c_xml_reader & r_reader, lmx::elmx_error * p_error )
{
    LMX_NO_WARN_UNUSED( r_reader );
    LMX_NO_WARN_UNUSED( p_error );
    return false;
}

lmx::elmx_error c_settings::unmarshal_attributes_check( lmx::c_xml_reader & r_reader )
{
    LMX_NO_WARN_UNUSED( r_reader );
    return lmx::ELMX_OK;
}

bool c_settings::unmarshal_body( lmx::c_xml_reader & r_reader, lmx::elmx_error * p_error )
{
    r_reader.tokenise( elem_event_map, lmx::EXNT_ELEM );
    if( r_reader.get_current_event() == e_NONE_windowWidth )
    {
        r_reader.set_code_line( __LINE__ );
        *p_error = r_reader.unmarshal_child_element( m_windowWidth, validation_spec_1, &elem_event_map[0] );
        if( *p_error != lmx::ELMX_OK )
            return false;
    }
    else if( (*p_error = r_reader.handle_error( lmx::ELMX_MANDATORY_ELEMENT_MISSING, "windowWidth", __FILE__, __LINE__ )) != lmx::ELMX_OK )
        return false;
    if( r_reader.get_current_event() == e_NONE_windowHeight )
    {
        r_reader.set_code_line( __LINE__ );
        *p_error = r_reader.unmarshal_child_element( m_windowHeight, validation_spec_1, &elem_event_map[1] );
        if( *p_error != lmx::ELMX_OK )
            return false;
    }
    else if( (*p_error = r_reader.handle_error( lmx::ELMX_MANDATORY_ELEMENT_MISSING, "windowHeight", __FILE__, __LINE__ )) != lmx::ELMX_OK )
        return false;
    if( r_reader.get_current_event() == e_NONE_cameraMovespeed )
    {
        r_reader.set_code_line( __LINE__ );
        *p_error = r_reader.unmarshal_child_element( m_cameraMovespeed, validation_spec_2, &elem_event_map[2] );
        if( *p_error != lmx::ELMX_OK )
            return false;
    }
    else if( (*p_error = r_reader.handle_error( lmx::ELMX_MANDATORY_ELEMENT_MISSING, "cameraMovespeed", __FILE__, __LINE__ )) != lmx::ELMX_OK )
        return false;
    if( r_reader.get_current_event() == e_NONE_cameraMaxZoom )
    {
        r_reader.set_code_line( __LINE__ );
        *p_error = r_reader.unmarshal_child_element( m_cameraMaxZoom, validation_spec_2, &elem_event_map[3] );
        if( *p_error != lmx::ELMX_OK )
            return false;
    }
    else if( (*p_error = r_reader.handle_error( lmx::ELMX_MANDATORY_ELEMENT_MISSING, "cameraMaxZoom", __FILE__, __LINE__ )) != lmx::ELMX_OK )
        return false;
    if( r_reader.get_current_event() == e_NONE_language )
    {
        r_reader.set_code_line( __LINE__ );
        *p_error = r_reader.unmarshal_child_element( m_language, validation_spec_3, &elem_event_map[4] );
        if( *p_error != lmx::ELMX_OK )
            return false;
    }
    else if( (*p_error = r_reader.handle_error( lmx::ELMX_MANDATORY_ELEMENT_MISSING, "language", __FILE__, __LINE__ )) != lmx::ELMX_OK )
        return false;
    if( r_reader.get_current_event() == e_NONE_debug )
    {
        r_reader.set_code_line( __LINE__ );
        *p_error = r_reader.unmarshal_child_element( m_debug, validation_spec_4, &elem_event_map[5] );
        if( *p_error != lmx::ELMX_OK )
            return false;
    }
    else if( (*p_error = r_reader.handle_error( lmx::ELMX_MANDATORY_ELEMENT_MISSING, "debug", __FILE__, __LINE__ )) != lmx::ELMX_OK )
        return false;
    if( r_reader.get_current_event() == e_NONE_fullScreen )
    {
        r_reader.set_code_line( __LINE__ );
        *p_error = r_reader.unmarshal_child_element( m_fullScreen, validation_spec_4, &elem_event_map[6] );
        if( *p_error != lmx::ELMX_OK )
            return false;
    }
    else if( (*p_error = r_reader.handle_error( lmx::ELMX_MANDATORY_ELEMENT_MISSING, "fullScreen", __FILE__, __LINE__ )) != lmx::ELMX_OK )
        return false;
    return true;
}

