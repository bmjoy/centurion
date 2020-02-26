#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#if defined( _MSC_VER ) && _MSC_VER >= 1400
#pragma warning (push)
#pragma warning (disable:4100)  // Disable unreferenced formal parameter
#endif

#include "lmxuser.h"

// To help MS intellisense
#ifndef LMX_GDECL
#define LMX_GDECL
#endif

LMX_GDECL extern const lmx::s_ns_map ns_map_reader[];
LMX_GDECL extern const lmx::s_ns_map ns_map_writer_1[];

enum
{
    e_error = lmx::EXE_LAST_XML_EVENT,
    e_NONE_settings,
    e_NONE_windowWidth,
    e_NONE_windowHeight,
    e_NONE_cameraMovespeed,
    e_NONE_cameraMaxZoom,
    e_NONE_language,
    e_NONE_debug,
    e_NONE_fullScreen,
    e_any
};

enum elmx_enums
{
    e_e_unknown
};

#ifdef LMX_USER_VALIDATE
lmx::elmx_error user_validator_1( const lmx::tlmx_uns16 &, const char * );
#endif // LMX_USER_VALIDATE

#ifdef LMX_USER_VALIDATE
lmx::elmx_error user_validator_2( const lmx::tlmx_uns8 &, const char * );
#endif // LMX_USER_VALIDATE

#ifdef LMX_USER_VALIDATE
lmx::elmx_error user_validator_3( const lmx::tlmx_unicode_string &, const char * );
#endif // LMX_USER_VALIDATE

#ifdef LMX_USER_VALIDATE
lmx::elmx_error user_validator_4( const bool &, const char * );
#endif // LMX_USER_VALIDATE

//-----------------------------------------------------------------------------
// Generated in Evaluation Mode by Codalogic LMX C++ XML Databinder.           |
//     Upgrade to Professional Edition to enable full functionality.           |
//         Visit http://codalogic.com/lmx/buy.php                              |
//-----------------------------------------------------------------------------

// From: C:\Users\utente\Documents\github rattlesmake\centurion\centurion-utils\lmx\settings.xsd(3)
class c_settings
{
private:
    // Element(s)
    lmx::ct_simple_pod_single< lmx::tlmx_uns16 > m_windowWidth;
    lmx::ct_simple_pod_single< lmx::tlmx_uns16 > m_windowHeight;
    lmx::ct_simple_pod_single< lmx::tlmx_uns8 > m_cameraMovespeed;
    lmx::ct_simple_pod_single< lmx::tlmx_uns8 > m_cameraMaxZoom;
    lmx::ct_simple_non_pod_single< lmx::tlmx_unicode_string > m_language;
    lmx::ct_simple_pod_single< bool > m_debug;
    lmx::ct_simple_pod_single< bool > m_fullScreen;

    static const lmx::s_event_map attr_event_map[];
    static const lmx::s_event_map elem_event_map[];

public:
    LMX_GDECL c_settings();
    LMX_GDECL c_settings( const c_settings & r_rhs );
    LMX_GDECL c_settings & operator = ( const c_settings & r_rhs );
    LMX_GDECL virtual c_settings * clone() const;
    LMX_GDECL void swap( c_settings & r_rhs );
    LMX_GDECL c_settings( lmx::c_xml_reader & r_reader, lmx::elmx_error * p_error );
    // Convenience constructors
    LMX_GDECL c_settings( const char c_file_name[], lmx::elmx_error * p_error, lmx::s_debug_error * p_debug_error = LMXNULL );
    #if LMX_WANT_WIDE_FILE_NAMES == 1
        LMX_GDECL c_settings( const wchar_t c_file_name[], lmx::elmx_error * p_error, lmx::s_debug_error * p_debug_error = LMXNULL );
    #endif
    LMX_GDECL c_settings( const char * p_memory, size_t memory_size, lmx::elmx_error * p_error, lmx::s_debug_error * p_debug_error = LMXNULL );
    LMX_GDECL c_settings( const std::string & r_string, lmx::elmx_error * p_error, lmx::s_debug_error * p_debug_error = LMXNULL );
    LMX_GDECL c_settings( const lmx::c_any_info & r_any_info, lmx::elmx_error * p_error, lmx::s_debug_error * p_debug_error = LMXNULL );
    LMX_GDECL virtual ~c_settings();

    // Element(s)

    //    windowWidth --> xs:unsignedShort{0<=x<=65535}
    LMX_GDECL const lmx::tlmx_uns16 & get_windowWidth() const { return m_windowWidth.get(); }
    LMX_GDECL lmx::elmx_error set_windowWidth( const lmx::tlmx_uns16 & value );

    //    windowHeight --> xs:unsignedShort{0<=x<=65535}
    LMX_GDECL const lmx::tlmx_uns16 & get_windowHeight() const { return m_windowHeight.get(); }
    LMX_GDECL lmx::elmx_error set_windowHeight( const lmx::tlmx_uns16 & value );

    //    cameraMovespeed --> xs:unsignedByte{0<=x<=255}
    LMX_GDECL const lmx::tlmx_uns8 & get_cameraMovespeed() const { return m_cameraMovespeed.get(); }
    LMX_GDECL lmx::elmx_error set_cameraMovespeed( const lmx::tlmx_uns8 & value );

    //    cameraMaxZoom --> xs:unsignedByte{0<=x<=255}
    LMX_GDECL const lmx::tlmx_uns8 & get_cameraMaxZoom() const { return m_cameraMaxZoom.get(); }
    LMX_GDECL lmx::elmx_error set_cameraMaxZoom( const lmx::tlmx_uns8 & value );

    //    language --> xs:string
    LMX_GDECL const lmx::tlmx_unicode_string & get_language() const { return m_language.get(); }
    LMX_GDECL lmx::elmx_error set_language( const lmx::tlmx_unicode_string & value ) { return m_language.set( value ); }

    //    debug --> xs:boolean
    LMX_GDECL const bool & get_debug() const { return m_debug.get(); }
    LMX_GDECL lmx::elmx_error set_debug( const bool & value ) { return m_debug.set( value ); }

    //    fullScreen --> xs:boolean
    LMX_GDECL const bool & get_fullScreen() const { return m_fullScreen.get(); }
    LMX_GDECL lmx::elmx_error set_fullScreen( const bool & value ) { return m_fullScreen.set( value ); }

    LMX_GDECL bool is_occurs_ok( lmx::c_check_tracker * p_tracker_in = LMXNULL ) const;

    LMX_GDECL bool check( lmx::c_check_tracker * p_tracker_in = LMXNULL, const char * p_name_in = "settings" ) const;

    LMX_GDECL static const lmx::s_ns_map * ns_map() { return ns_map_reader; }
    // Convenience marshal/unmarshal functions
    LMX_GDECL lmx::elmx_error marshal( const char c_file_name[], lmx::s_debug_error * p_debug_error = LMXNULL ) const;
    #if defined( _MSC_VER ) && _MSC_VER >= 1400
        LMX_GDECL lmx::elmx_error marshal( const wchar_t c_file_name[], lmx::s_debug_error * p_debug_error = LMXNULL ) const;
    #endif
    LMX_GDECL lmx::elmx_error marshal( std::string * p_string, lmx::s_debug_error * p_debug_error = LMXNULL ) const;
    LMX_GDECL lmx::elmx_error marshal( std::ostream & r_sos, lmx::s_debug_error * p_debug_error = LMXNULL ) const;
    LMX_GDECL lmx::elmx_error unmarshal( const char c_file_name[], lmx::s_debug_error * p_debug_error = LMXNULL );
    #if LMX_WANT_WIDE_FILE_NAMES == 1
        LMX_GDECL lmx::elmx_error unmarshal( const wchar_t c_file_name[], lmx::s_debug_error * p_debug_error = LMXNULL );
    #endif
    LMX_GDECL lmx::elmx_error unmarshal( const char * p_memory, size_t memory_size, lmx::s_debug_error * p_debug_error = LMXNULL );
    LMX_GDECL lmx::elmx_error unmarshal( const std::string & r_string, lmx::s_debug_error * p_debug_error = LMXNULL )
    {
        return unmarshal( r_string.data(), r_string.size(), p_debug_error );
    }
    LMX_GDECL lmx::elmx_error unmarshal( const lmx::c_any_info & r_any_info, lmx::s_debug_error * p_debug_error = LMXNULL );

    // General marshal/unmarshal functions
    LMX_GDECL lmx::elmx_error marshal( lmx::c_xml_writer & r_writer, const char * p_name = "settings" ) const;
    LMX_GDECL lmx::elmx_error unmarshal( lmx::c_xml_reader & r_reader );
    LMX_GDECL lmx::elmx_error unmarshal( lmx::c_xml_reader & r_reader, const std::string & r_name );
    LMX_GDECL void reset();

protected:
    LMX_GDECL void marshal_attributes( lmx::c_xml_writer & LMX_UNUSED_PARAM(r_writer) ) const {}
    LMX_GDECL lmx::elmx_error marshal_child_elements( lmx::c_xml_writer & r_writer ) const;
    LMX_GDECL bool unmarshal_attributes( lmx::c_xml_reader & r_reader, lmx::elmx_error * p_error );
    LMX_GDECL lmx::elmx_error unmarshal_attributes_check( lmx::c_xml_reader & r_reader );
    LMX_GDECL bool unmarshal_body( lmx::c_xml_reader & r_reader, lmx::elmx_error * p_error );

private:
    LMX_GDECL void init();
    LMX_GDECL void release();
};

#if defined( _MSC_VER ) && _MSC_VER >= 1400
#pragma warning (pop)  // Restore disabled warnings
#endif


#endif      // End of include guard
