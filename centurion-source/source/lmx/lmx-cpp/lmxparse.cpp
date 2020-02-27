#if defined( _MSC_VER ) && _MSC_VER > 1000
#pragma warning (disable:4786)  // Disable truncated debug info warning
#endif

#if defined( _MSC_VER ) && _MSC_VER >= 1400
#pragma warning (disable:4996)  // Disable VC8 'safe' warnings
#pragma warning (disable:4100)  // Disable unreferenced formal parameter
#pragma warning (disable:4702)  // Disable unreachable code
#endif

#include <cctype>
#include <string>
#if defined( _MSC_VER ) && ! defined( UNDER_CE ) || defined( __BORLANDC__ ) || defined( __GNUC__ ) && __GNUC__ >= 3
    #include <locale>
#endif

#ifndef LMX_PARSE_SOURCE
#define LMX_PARSE_SOURCE
#endif

#include <lmx/lmxparse.h>

//------------------------------------------------------------------------------
//                       Functions for parsing XML
//------------------------------------------------------------------------------

namespace lmx {

//------------------------------------------------------------------------------
//           Try to help user get the correct #defines for declspecs
//------------------------------------------------------------------------------

#if LMX_WANT_DLL == 0 && ! defined( LMX_MAKE_DLL ) && (defined( _MSC_VER ) || defined( __BORLANDC__ ))
    // Try to warn user if they have not setup DLL declspecs correctly
    namespace NOTE_TO_USER_____
    {
        const int set_hash_define_LMX_WANT_DLL_to_1_to_use_DLL_versions_of_LMX_libraries___See_comments_near_LMX_WANT_DLL_in_lmxuser_h_file = 0;
    }
#endif

//------------------------------------------------------------------------------
//                 c_entity_name_reader specializations
// These classes are specializations of c_entity_name_reader and allow us to
// use a common function to read the name of entities from different input
// sources such as c_get_for_xml, strings and the stored attribute data.
//------------------------------------------------------------------------------

c_entity_name_reader_for_c_get::c_entity_name_reader_for_c_get( c_get_for_xml & r_get_in )
    : mr_get( r_get_in )
{}

int c_entity_name_reader_for_c_get::get()   // Returns c_read::k_eof for end of text
{
    return mr_get.get();
}

void c_entity_name_reader_for_c_get::unget( int c_in )
{
    mr_get.unget( c_in );
}

c_entity_name_reader_for_attribute_data::c_entity_name_reader_for_attribute_data( c_attribute_data & r_attribute_data_in )
    : mr_attribute_data( r_attribute_data_in )
{}

int c_entity_name_reader_for_attribute_data::get()   // Returns c_read::k_eof for end of text
{
    if( mr_attribute_data.peek() == c_read::k_eof )
        return c_read::k_eof;
    return mr_attribute_data.get();
}

void c_entity_name_reader_for_attribute_data::unget( int c_in )
{
    if( c_in != c_read::k_eof )
        mr_attribute_data.unget( c_in );
}

c_entity_name_reader_for_entity::c_entity_name_reader_for_entity( const char ** pp_entity_value_in )
    :
    mp_entity_value_start_value( *pp_entity_value_in ),
    mpp_entity_value( pp_entity_value_in )
{}

int c_entity_name_reader_for_entity::get()   // Returns c_read::k_eof for end of text
{
    unsigned char c = static_cast<unsigned char>(**mpp_entity_value);   // Avoid sign extension
    if( c == '\0' )
        return c_read::k_eof;
    ++(*mpp_entity_value);
    return c;
}

void c_entity_name_reader_for_entity::unget( int c_in )
{
    if( c_in != c_read::k_eof && mp_entity_value_start_value < *mpp_entity_value )
        --(*mpp_entity_value);
}

c_entity_name_reader_for_c_str::c_entity_name_reader_for_c_str( const char * p_string_in, size_t index_in )
    : mp_string( p_string_in ), m_index( index_in )
{}

int c_entity_name_reader_for_c_str::get()   // Returns c_read::k_eof for end of text
{
    if( mp_string[m_index] == '\0' )
        return c_read::k_eof;
    return static_cast<unsigned char>( mp_string[m_index++] );    // Control sign extension
}

void c_entity_name_reader_for_c_str::unget( int c_in )
{
    if( c_in != c_read::k_eof )
    {
        if( m_index > 0 ) --m_index;
        lmx_assert( c_in == static_cast<unsigned char>( mp_string[m_index] ) );    // Control sign extension
    }
}

//------------------------------------------------------------------------------
//                      Error handling implementation
//------------------------------------------------------------------------------

const c_get_as_utf8 * c_error::register_getter( c_getter_error_binder * p_binder_in )
{
    const c_get_as_utf8 * p_old_getter = mp_getter;
    mp_getter = p_binder_in->get_new_getter();
    return p_old_getter;
}

void c_error::unregister_getter( c_getter_error_binder * p_binder_in )
{
    mp_getter = p_binder_in->get_old_getter();
}

void c_error::error(
                elmx_error_type type_in,
                elmx_error code_in,
                const char * p_file_in,
                int line_in,
                const char * p_msg_in )
{
    if( m_fatals > 0 )    // Don't report errors after fatal error (likely to be eof)
        return;

    m_last_error_line = line_in;
    m_last_error_code = code_in;
    m_last_error_type = type_in;

    if( type_in == EET_FATAL )
    {
        ++m_errors;
        ++m_fatals;
    }
    else if( type_in == EET_ERROR )
        ++m_errors;
    else if( type_in == EET_WARNING )
        ++m_warnings;
    else if( type_in == EET_NOTE )
    {
        ++m_warnings;
        ++m_notes;
    }

    report_error( type_in, code_in, p_file_in, line_in, p_msg_in );
}

void c_error::error( elmx_error_type type_in, elmx_error code_in, const char * p_msg_in )
{
    const char * p_file = LMXNULL;
    int line = -1;

    if( mp_getter )
    {
        p_file = mp_getter->get_source_id();
        line = mp_getter->get_line_num();
    }

    error( type_in, code_in, p_file, line, p_msg_in );
}

bool c_error::get_error_info( elmx_error_type * p_type_out, elmx_error * p_code_out, int * p_line_out )
{
    if( m_last_error_code == ELMX_OK )
        return false;

    *p_type_out = m_last_error_type;
    *p_code_out = m_last_error_code;
    *p_line_out = m_last_error_line;
    return true;
}

//------------------------------------------------------------------------------
//                            c_getter_error_binder
//            Implementation of binding between c_error and getter.
//   By making this the only way a getter can register with the error class
//       we ensure that the binding is always done and undone correctly.
// Note error_binding - Potentially only one instance of c_error might be used
// to parse multiple XML files.  To allow easier error reporting,
// c_error::error() can be called without line numbers etc., and the c_error
// object will ask the current getter instance for the line number information.
// Hence c_error needs to be informed of the active getter, and when that getter
// has completed, the previous getter (if any) has to be reverted to.
// c_getter_error_binder ensures this works smoothly.
//------------------------------------------------------------------------------

c_getter_error_binder::c_getter_error_binder( c_error & r_error_in ) :
    m_is_bound( false ), mr_error( r_error_in ), mp_new_getter( LMXNULL ), mp_old_getter( LMXNULL )
{
}

void c_getter_error_binder::bind( const c_get_as_utf8 * p_getter_in )
{
    if( ! m_is_bound )    // Prevent binding multiple times to avoid invalidating p_old_getter
    {
        mp_new_getter = p_getter_in;
        mp_old_getter = mr_error.register_getter( this );
        m_is_bound = true;
    }
}

c_getter_error_binder::~c_getter_error_binder()
{
    assert( m_is_bound );     // We forgot to call bind() if this occurs
    if( m_is_bound )
        mr_error.unregister_getter( this );
}

//------------------------------------------------------------------------------
//                             Utility functions
//------------------------------------------------------------------------------

//   Latin 1 (ISO-8859-1) maps directly to Unicode
//   UCS-2 range (hex.)     UTF-8 octet sequence (binary)
//   0000-007F              0xxxxxxx
//   0080-07FF              110xxxxx 10xxxxxx
//   0800-FFFF              1110xxxx 10xxxxxx 10xxxxxx
//
// With UCS-4 range
//   0001 0000-001F FFFF   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
//
// The Unicode range is 0000 0000-0010 FFFF

LMX_PDECL bool utf8_to_unicode_char( tc_utf8_buf c_utf8_in, t_unicode_char * p_unicode_in )
{
    if( (c_utf8_in[0] & 0x80) == 0 )
    {
        *p_unicode_in = c_utf8_in[0];   // Also detects '\0' case
        return true;
    }

    char msb;
    char size;
    if( (c_utf8_in[0] & 0xe0) == 0xc0 )
    {
        // 2 bytes
        msb = c_utf8_in[0] & 0x1f;
        size = 2;
    }

    else if( (c_utf8_in[0] & 0xf0) == 0xe0 )
    {
        // 3 bytes
        msb = c_utf8_in[0] & 0x0f;
        size = 3;
    }

    else if( (c_utf8_in[0] & 0xf8) == 0xf0 )
    {
        // 4 bytes
        msb = c_utf8_in[0] & 0x07;
        size = 4;
    }

    else
    {
        *p_unicode_in = -1;
        return false;
    }

    *p_unicode_in = msb;
    int i;
    for( i=1; i<size; ++i )
    {
        if( (c_utf8_in[i] & 0xc0) != 0x80 )
            return false;   // Also detects unexpected '\0' case
        *p_unicode_in <<= 6;
        *p_unicode_in += c_utf8_in[i] & 0x3f;
    }

    return true;
}

LMX_PDECL char *unicode_char_to_utf8( t_unicode_char c_in, tc_utf8_buf c_utf8_in )
{
    int n_bytes;
    int id_mask;

    if( c_in <= 0x7f )
    {
        c_utf8_in[0] = static_cast<char>( c_in );
        c_utf8_in[1] = '\0';
        return c_utf8_in;
    }

    else if( c_in <= 0x7ff )
    {
        // 2 bytes
        n_bytes = 2;
        id_mask = 0xc0;
    }

    else if( c_in <= 0x0000ffff )
    {
        // 3 bytes
        n_bytes = 3;
        id_mask = 0xe0;
    }

    else // if( a_c <= 0x1fffff )
    {
        // 4 bytes
        n_bytes = 4;
        id_mask = 0xf0;
    }

    c_utf8_in[n_bytes] = '\0';
    for( int i = n_bytes - 1; i>=1; i-- )
    {
        c_utf8_in[i] = static_cast<char>(0x80 | (c_in & 0x3f));
        c_in >>= 6;
    }

    c_utf8_in[0] = static_cast<char>((c_in & ((~id_mask)>>1)) | id_mask);
                    // For reason for >>1 see banner at start of function

    return c_utf8_in;
}

LMX_PDECL bool is_valid_utf8_char_sequence( const char c_utf8_in[] )      // V3.2.4 - Added
{
    // Values from ABNF in RFC-3629
    unsigned char utf8_0 = c_utf8_in[0];
    unsigned char utf8_1 = c_utf8_in[1];

    // Tests need to be against 0xc2 etc rather than '\xc2' to avoid sign extension issues.
    if( utf8_0 <= 0x7f ||         // v4.2 - gcc_433_brackets
            (utf8_0 >= 0xc2 && utf8_0 <= 0xdf) ||
            (utf8_0 == 0xe0 && (utf8_1 >= 0xa0 && utf8_1 <= 0xbf)) ||
            (utf8_0 >= 0xe1 && utf8_0 <= 0xec) ||
            (utf8_0 == 0xed && (utf8_1 >= 0x80 && utf8_1 <= 0x9f)) ||
            (utf8_0 >= 0xee && utf8_0 <= 0xef) ||
            (utf8_0 == 0xf0 && (utf8_1 >= 0x90 && utf8_1 <= 0xbf)) ||
            (utf8_0 >= 0xf1 && utf8_0 <= 0xf3) ||
            (utf8_0 == 0xf4 && (utf8_1 >= 0x80 && utf8_1 <= 0x8f)) )
        return true;        // All OK

    return false;
}

inline bool is_xml_name_start_char( int c_in )
{
    // Put a_c > 0x7f first to allow for safe use of isalpha()
    return c_in > 0x7f || LMXSTDCRT::isalpha( c_in ) || c_in == '_' || c_in == ':';
}

inline bool is_xml_name_char( int c_in )
{
    // is_xml_name_start_char() includes test for a_c > 0x7f to allow for safe use of isdigit()
    return is_xml_name_start_char( c_in ) || LMXSTDCRT::isdigit( c_in ) || c_in == '.' || c_in == '-';
}

inline bool is_xml_NCname_char( int c_in )   // V3.2.4 - Added
{
    return c_in != ':' && is_xml_name_char( c_in );
}

//------------------------------------------------------------------------------
//                                c_read_file
//------------------------------------------------------------------------------

#if LMX_WANT_WIDE_FILE_NAMES == 1
c_read_file::c_read_file( const wchar_t c_file_in[] ) :
    m_n_chars( 0 ), m_char_index( 0 ), m_is_file_to_be_closed_on_delete( true )
{
    tc_utf8_buf c_utf8;
    for( size_t i=0; c_file_in[i] != L'\0'; ++i )
        m_file += unicode_char_to_utf8( c_file_in[i], c_utf8 );

    mh_input = _wfopen( c_file_in, L"rb" );
}
#endif

//------------------------------------------------------------------------------
//                                 c_get_as_utf8
//             Class for reading 'any' input and converting to UTF-8
//------------------------------------------------------------------------------

const c_external_transcoder_factory * c_get_as_utf8::mp_default_external_transcoder_factory = LMXNULL;  // C20140124_1118

c_get_as_utf8::c_get_as_utf8( c_read & r_reader_in, c_error & r_error_in )
    :
    mr_reader( r_reader_in ),
    mr_error( r_error_in ),
    m_getter_error_binder( r_error_in ),
    mp_external_transcoder_factory( mp_default_external_transcoder_factory ),   // C20140124_1118
    mp_external_transcoder( LMXNULL )                                           // C20140124_1118
{
    m_getter_error_binder.bind( this );   // Can't call bind() until object fully constructed
    p_init();
}

void c_get_as_utf8::p_init()
{
    m_encoding_mode = EXR_INIT;
    m_last_get = '\0';
    m_unget_buffer.clear();
    m_line_num = 1;
    mc_utf8_expansion_buffer[0] = '\0';
    mp_utf8_expansion = LMXNULL;
}

c_get_as_utf8::~c_get_as_utf8()
{
    delete mp_external_transcoder;  // C20140124_1118
}

void c_get_as_utf8::set_default_external_transcoder_factory(        // C20140124_1118
                                const c_external_transcoder_factory & r_default_external_transcoder_factory_in )
{
    mp_default_external_transcoder_factory = &r_default_external_transcoder_factory_in;
}

void c_get_as_utf8::clear_default_external_transcoder_factory() // C20140124_1118
{
    mp_default_external_transcoder_factory = LMXNULL;
}

void c_get_as_utf8::set_external_transcoder_factory(                // C20140124_1118
                                const c_external_transcoder_factory & r_external_transcoder_factory_in )
{
    mp_external_transcoder_factory = &r_external_transcoder_factory_in;
}

void c_get_as_utf8::clear_external_transcoder_factory()         // C20140124_1118
{
    mp_external_transcoder_factory = LMXNULL;
}

void c_get_as_utf8::rewind()
{
    p_init();
    if( mp_external_transcoder )    // C20140124_1118
    {
        delete mp_external_transcoder;
        mp_external_transcoder = LMXNULL;
    }
    mr_reader.rewind();
}

int c_get_as_utf8::p_resolve_initial_character_encoding( int initial_c_in )
{
    // Resolves the character encoding and returns the first character suitable
    // for parsing in UTF-8 encoding.  Will typically be white space or '<'.

    // A complete coded character must be read so that the next read operation
    // starts at the boundary of a valid character code.  (This is mainly an
    // issue for the 16-bit character codes.)

    int c = initial_c_in;

    // Test for multibyte code - Set appropriate mode if found, UTF8 otherwise
    if( c == 0xff )
    {
        // FF FE 00 00 UCS-4, little-endian machine (4321 order) - Not supported
        // FF FE ## ## UTF-16, little-endian
        if( p_read() != 0xfe )
        {
            error( EET_FATAL, ELMX_BAD_CHAR_ENCODING, "Unrecognised character encoding" );
            return c_read::k_eof;
        }

        int extra = p_read();
        int extra2 = p_read();
        if( extra != 0 && extra <= 0x7f && extra2 == 0 )  // First char must be in ASCII range
        {
            m_encoding_mode = EXR_UTF16_LE;
            return extra;
        }
        else
        {
            // Direct UCS-4 unsupported
            error( EET_FATAL, ELMX_BAD_CHAR_ENCODING, "Unrecognised character encoding" );
            return c_read::k_eof;
        }
    }

    else if( c == 0xfe )
    {
        // FE FF 00 00 UCS-4, unusual octet order (3412)
        // FE FF ## ## UTF-16, big-endian
        if( p_read() != 0xff )
        {
            error( EET_FATAL, ELMX_BAD_CHAR_ENCODING, "Unrecognised character encoding" );
            return c_read::k_eof;
        }

        int extra = p_read();
        int extra2 = p_read();
        if( extra == 0 && extra2 != 0 && extra2 <= 0x7f ) // First char must be in ASCII range
        {
            m_encoding_mode = EXR_UTF16_BE;
            return extra2;
        }
        else
        {
            // Direct UCS-4 unsupported
            error( EET_FATAL, ELMX_BAD_CHAR_ENCODING, "Unrecognised character encoding" );
            return c_read::k_eof;
        }
    }

    else if( c == 0xef )
    {
        // ef bb bf UTF-8
        if( p_read() != 0xbb || p_read() != 0xbf )
        {
            error( EET_FATAL, ELMX_BAD_CHAR_ENCODING, "Unrecognised character encoding" );
            return c_read::k_eof;
        }

        m_encoding_mode = EXR_UTF8;
        return p_read();
    }

    else if( xisspace(c) || c == '<' )
    {
        // 3C 00 3F 00 UTF-16LE or little-endian ISO-10646-UCS-2 or other encoding with
        // -a 16-bit code unit in little-endian order and ASCII characters encoded as
        // -ASCII values (the encoding declaration must be read to determine which)

        // 3C 3F 78 6D UTF-8, ISO 646, ASCII, some part of ISO 8859, Shift-JIS, EUC,
        // -or any other 7-bit, 8-bit, or mixed-width encoding which ensures that the
        // -characters of ASCII have their normal positions, width, and values; the
        // -actual encoding declaration must be read to detect which of these applies,
        // -but since all of these encodings use the same bit patterns for the relevant
        // -ASCII characters, the encoding declaration itself may be read reliably.

        int extra = p_read();
        if( extra != 0 )
        {
            m_encoding_mode = EXR_UTF8;
            // We've read a byte from the next UTF-8 character and can't easily push it
            // back, so read in the whole compound character for later reading.
            if( extra != c_read::k_eof )
            {
                p_read_utf8_compound_char_sequence( extra );
                mp_utf8_expansion = mc_utf8_expansion_buffer;
            }
        }
        else
            m_encoding_mode = EXR_UTF16_LE;
        return c;
    }

    else if( c == 0 )
    {
        // 00 3C 00 3F UTF-16BE or big-endian ISO-10646-UCS-2 or other encoding with a 16-bit code unit in big-endian order and ASCII characters encoded as ASCII values (the encoding declaration must be read to determine which)
        int extra = p_read();
        if( xisspace( extra ) || extra == '<' )
        {
            m_encoding_mode = EXR_UTF16_BE;
            return extra;
        }
    }

    error( EET_FATAL, ELMX_BAD_CHAR_ENCODING, "Unrecognised character encoding" );

    return c_read::k_eof;
}

int c_get_as_utf8::p_map_unicode_to_utf8( t_unicode_char c_in )
{
    if( c_in == 0x85 || c_in == 0x2028 )  // XML1.1: Map NEL (New End of Line) and
    {                                   // -Unicode Line sep to 0x0a to ease processing
        mc_utf8_expansion_buffer[0] = 0x0a;
        mc_utf8_expansion_buffer[1] = '\0';
    }
    else
        unicode_char_to_utf8( c_in, mc_utf8_expansion_buffer );

    if( mc_utf8_expansion_buffer[1] != '\0' )
        mp_utf8_expansion = mc_utf8_expansion_buffer + 1;
    else
        mp_utf8_expansion = LMXNULL;

    return static_cast<unsigned char>( mc_utf8_expansion_buffer[0] );   // Control sign extension
}

bool c_get_as_utf8::set_encoding( const char * p_encoding_in )
{
    bool incompatible_encoding = false;

    if( xstricmp( "ISO-10646-UCS-2", p_encoding_in ) == 0 )
    {
        if( m_encoding_mode == EXR_UTF16_BE )
            m_encoding_mode = EXR_UCS_2_BE;

        else if( m_encoding_mode == EXR_UTF16_LE )
            m_encoding_mode = EXR_UCS_2_LE;

        else
            incompatible_encoding = true;
    }

    else if( xstricmp( "ISO-8859-1", p_encoding_in ) == 0 )
    {
        if( m_encoding_mode != EXR_UTF8 )
            incompatible_encoding = true;
        m_encoding_mode = EXR_ISO8859_1;
    }

    else if( xstricmp( "US-ASCII", p_encoding_in ) == 0 )
    {
        if( m_encoding_mode != EXR_UTF8 )
            incompatible_encoding = true;
        // Slightly non-standard, but allow as a subset of Latin-1
        m_encoding_mode = EXR_ISO8859_1;
    }

    else if( xstricmp( "UTF-8", p_encoding_in ) == 0 )
    {
        if( m_encoding_mode != EXR_UTF8 )
            incompatible_encoding = true;
    }

    else if( xstricmp( "UTF-16", p_encoding_in ) == 0 )
    {
        if( m_encoding_mode != EXR_UTF16_BE && m_encoding_mode != EXR_UTF16_LE )
            incompatible_encoding = true;
    }

    else
    {
        if( mp_external_transcoder_factory )    // C20140124_1118
        {
            mp_external_transcoder =
                    mp_external_transcoder_factory->get_transcoder(
                                            mr_reader,
                                            mr_error,
                                            m_encoding_mode,
                                            p_encoding_in );
            if( mp_external_transcoder )
            {
                m_encoding_mode = EXR_EXTERNAL;
                return true;
            }

        }
        error( EET_FATAL, ELMX_BAD_CHAR_ENCODING, "Unknown character encoding specified" );
        return false;
    }

    if( incompatible_encoding )
    {
        error( EET_FATAL, ELMX_CHAR_ENCODING_MISMATCH, "Mis-match between specfied encoding and byte format" );
        return false;
    }

    return true;
}

t_unicode_char c_get_as_utf8::p_convert_utf16_bytes( int b1, int b2 )
{
    if( b1 == c_read::k_eof || b2 == c_read::k_eof )
        return c_read::k_eof;

    switch( m_encoding_mode )
    {
    case EXR_UTF16_BE:
        return (b1 << 8) + b2;

    case EXR_UTF16_LE:
        return b1 + (b2 << 8);

    default:
        assert(0);      // Should not happen
        return 0;
    }

    lmx_assert_code_not_reached();  // Should not get here

    return c_read::k_eof;
}

bool p_utf8_char_in_range( char c_in, char lower_c_in, char upper_c_in )
{
    if( static_cast<unsigned char>(c_in) >= static_cast<unsigned char>(lower_c_in) &&
            static_cast<unsigned char>(c_in) <= static_cast<unsigned char>(upper_c_in) )
        return true;
    return false;
}

void c_get_as_utf8::p_read_single_utf8_compound_char_sequence( int first_c_in, char c_utf8_in[] )
{
    // Read in all UTF-8 chars corresponding to the Unicode char so we can look for
    // -NEL and Unicode Line Sep

    c_utf8_in[0] = static_cast<char>( first_c_in );

    int n_bytes;
    if( first_c_in >= 0xfc )
        n_bytes = 6;
    else if( first_c_in >= 0xf8 )
        n_bytes = 5;
    else if( first_c_in >= 0xf0 )
        n_bytes = 4;
    else if( first_c_in >= 0xe0 )
        n_bytes = 3;
    else if( first_c_in >= 0xc0 )
        n_bytes = 2;
    else
        n_bytes = 1;

    // Add check for illegal UTF-8 sequence

    int i;
    int c = '\0';
    for( i=1; i<n_bytes; ++i )
        c_utf8_in[i] = static_cast<char>( c = p_read() );
    c_utf8_in[i] = '\0';

    if( n_bytes > 1 && c == c_read::k_eof )
        c_utf8_in[0] = '\0';

    else if( (c_utf8_in[0] == '\xc2' &&       // NEL
                    c_utf8_in[1] == '\x85')
                || (c_utf8_in[0] == '\xe2' &&     // Line sep
                    c_utf8_in[1] == '\x80' &&
                    c_utf8_in[2] == '\xa8') )     // v4.2 - gcc_433_brackets
    {
        c_utf8_in[0] = '\n';
        c_utf8_in[1] = '\0';
    }

    #if defined( LMX_XML_STRICT ) && LMX_XML_STRICT != 0    // Added in V3.2.4
    if( ! is_valid_utf8_char_sequence( c_utf8_in ) )
        c_utf8_in[0] = '\0';  // Indicates error condition
    #endif
}

int c_get_as_utf8::p_read_utf8_compound_char_sequence( int first_c_in )
{
    // This function handles UTF-8 characters that represent UTF-16 surrogates.
    // If this case is encountered, the surrogates are converted into their
    // corresponding Unicode character.

    //D800: 1110|xxxx 10xx|xxxx 10xx|xxxx
    //          |1101   10|0000   00|0000
    //      E    D    A    0    8    0
    //DBFF: 1110|xxxx 10xx|xxxx 10xx|xxxx
    //          |1101   10|1111   11|1111
    //      E    D    A    F    B    F
    //DC00: 1110|xxxx 10xx|xxxx 10xx|xxxx
    //          |1101   11|0000   00|0000
    //      E    D    B    0    8    0
    //DFFF: 1110|xxxx 10xx|xxxx 10xx|xxxx
    //           1101   11|1111   11|1111
    //      E    D    B    F    B    F
    // U+DC00 = ed b0 80
    // U+DFFF = ed bf bf
    // U+D800 = ed a0 80
    // U+DBFF = ed af bf

    p_read_single_utf8_compound_char_sequence( first_c_in, mc_utf8_expansion_buffer );

    if( mc_utf8_expansion_buffer[0] == '\0' )
        return c_read::k_eof;

    else if( mc_utf8_expansion_buffer[0] == '\xed' &&
                p_utf8_char_in_range( mc_utf8_expansion_buffer[1], '\xa0', '\xaf' ) )
    {
        // This is a UTF-16 surrogate encoded as UTF-8
        tc_utf8_buf c_utf8_low_surrogate;
        p_read_single_utf8_compound_char_sequence( p_read(), c_utf8_low_surrogate );
        if( c_utf8_low_surrogate[0] == '\0' )
            return c_read::k_eof;
        if( c_utf8_low_surrogate[0] != '\xed' ||
                ! p_utf8_char_in_range( c_utf8_low_surrogate[1], '\xb0', '\xbf' ) )
            return c_read::k_eof;

        t_unicode_char high_surrogate, low_surrogate;
        if( ! utf8_to_unicode_char( mc_utf8_expansion_buffer, &high_surrogate ) ||
                ! utf8_to_unicode_char( c_utf8_low_surrogate, &low_surrogate ) )
            return c_read::k_eof;       // V3.2.4 - tested for successful return result

        p_map_unicode_to_utf8( ((high_surrogate & 0x3ff)<<10) + (low_surrogate & 0x3ff) + 0x10000 );
    }

    if( mc_utf8_expansion_buffer[0] == '\0' || mc_utf8_expansion_buffer[1] == '\0' )
        mp_utf8_expansion = LMXNULL;
    else
        mp_utf8_expansion = mc_utf8_expansion_buffer + 1;

    return static_cast<unsigned char>( mc_utf8_expansion_buffer[0] );   // Control sign extension
}

int c_get_as_utf8::p_read_local_transcoded()    // C20140124_1118
{
    int c = p_read();

    if( c == c_read::k_eof )
        return c_read::k_eof;

    switch( m_encoding_mode )
    {
        case EXR_INIT:
            return p_resolve_initial_character_encoding( c );
        break;

        case EXR_ISO8859_1:
            // Latin-1 maps directly to Unicode
            if( c > 0x7e )
                return p_map_unicode_to_utf8( c );
            return c;
        break;

        case EXR_UTF8:
            if( c <= 0x7f )
                return c;
            else
                return p_read_utf8_compound_char_sequence( c );
        break;

        case EXR_UTF16_BE:
        case EXR_UTF16_LE:
        {
            // Read 2 chars.  If 0xd800 (or whatever), read next UTF-16 char also
            // -and convert to UTF-8
            t_unicode_char unicode_c;
            if( (unicode_c = p_convert_utf16_bytes( c, p_read() )) == c_read::k_eof )
                return c_read::k_eof;

            if( unicode_c >= 0x0000d800 && unicode_c <= 0x0000dbff )
            {
                c = p_read();
                // Don't assume calling order - i.e. NOT p_conv...( p_read(), p_read() );
                t_unicode_char unicode_extra = p_convert_utf16_bytes( c, p_read() );

                if( unicode_extra == c_read::k_eof )
                    return c_read::k_eof;

                else if( unicode_extra >= 0x0000dc00 && unicode_extra <= 0x0000dfff )
                    return p_map_unicode_to_utf8( ((unicode_c & 0x3ff)<<10) + (unicode_extra & 0x3ff) + 0x10000 );

                else
                {
                    error( EET_FATAL, ELMX_BAD_CHAR_ENCODING, "Unrecognised character encoding" );
                    return c_read::k_eof;    // This is an error condition
                }
            }

            return p_map_unicode_to_utf8( unicode_c );
        }
        break;

        case EXR_UCS_2_BE:
        {
            int extra = p_read();
            if( extra == c_read::k_eof )
                return c_read::k_eof;
            return p_map_unicode_to_utf8( (static_cast<t_unicode_char>(c) << 8) + extra );
        }
        break;

        case EXR_UCS_2_LE:
        {
            int extra = p_read();
            if( extra == c_read::k_eof )
                return c_read::k_eof;
            return p_map_unicode_to_utf8( c + (static_cast<t_unicode_char>(extra) << 8) );
        }
        break;

        case EXR_EXTERNAL:  // C20140124_1118
        {
            return c;   // All trancoding is done externally
        }
        break;

        default:
            assert(0);  // Should not happen
            return c_read::k_eof;
    }

    lmx_assert_code_not_reached();  // Should not get here

    return c_read::k_eof;
}

int c_get_as_utf8::p_read_external_transcoded() // C20140124_1118
{
    c_external_transcoder::s_result result = mp_external_transcoder->read();
    mp_utf8_expansion = result.mp_utf8_expansion;
    return result.c;
}

int c_get_as_utf8::p_get()
{

    if( mr_error.is_fatal_errored() )
        return c_read::k_eof;

    else if( ! m_unget_buffer.empty() )
        return m_unget_buffer.get();      // Don't submit to further processing (it will have already been done the first time the character was read)

    else if( mp_utf8_expansion )
    {
        int result = static_cast<unsigned char>( *mp_utf8_expansion++ );   // Control sign extension
        if( *mp_utf8_expansion == '\0' )
            mp_utf8_expansion = LMXNULL;
        return result;
    }

    if( m_encoding_mode != EXR_EXTERNAL )   // C20140124_1118
        return p_read_local_transcoded();

    return p_read_external_transcoded();
}

int c_get_as_utf8::get()
{
    int c = p_get();

        // Complete end of line normalisation - NEL and Unicode line sep already
        // -converted to 0x0a
    if( c == 0x0a && m_last_get == 0x0d )
        c = p_get();

    m_last_get = static_cast<char>( c );

    if( c == 0x0d )
        c = 0x0a;

    if( c == 0x0a )
        ++m_line_num;

    return c;
}

void c_get_as_utf8::error( elmx_error_type type_in, elmx_error code_in, const char * p_format_in )
{
    mr_error.error( type_in, code_in, mr_reader.get_source_id(), m_line_num,
                        p_format_in );
}

void c_get_as_utf8::error( elmx_error_type type_in, elmx_error code_in, const std::string & r_s_in )
{
    mr_error.error( type_in, code_in, mr_reader.get_source_id(), m_line_num,
                        r_s_in.c_str() );
}

//------------------------------------------------------------------------------
//                        c_get_with_entities class
//------------------------------------------------------------------------------

c_get_with_entities::c_get_with_entities( c_read & r_reader_in, c_error & r_error_in, bool is_reading_xsd_in /*=true*/ )
    :
    c_get_as_utf8( r_reader_in, r_error_in ),
    m_is_reading_xsd( is_reading_xsd_in ),
    m_ws_mode( EXWS_PRESERVE ),
    m_ws_proc_got_white( false )
{
}

c_get_with_entities::~c_get_with_entities()
{
}

int c_get_with_entities::get()
{
    // Mostly user defined entities will not be used, so fast path this option
    if( m_entity_input_stack.empty() )
        return c_get_as_utf8::get();


    // In code that uses the returned characters, we need to know if the character comes
    // from the entity stack or not.  Therefore, we wait until the next character read
    // after the entity has been fully read to pop entity from the top of the stack.
    // It also allows us to push back a character to the right entity.
    while( ! m_entity_input_stack.empty() && m_entity_input_stack.back().empty() )
        m_entity_input_stack.pop_back();

    if( ! m_entity_input_stack.empty() )
        return m_entity_input_stack.back().get();

    else
        return c_get_as_utf8::get();
}

void c_get_with_entities::unget( int c_in )
{
    if( m_entity_input_stack.empty() )
        c_get_as_utf8::unget( c_in );
    else
        m_entity_input_stack.back().unget( c_in );
}

bool c_get_with_entities::set_input_entity_value( const char * p_input_text_in )
{
    for( size_t i=0; i<m_entity_input_stack.size(); ++i )
    {
        if( m_entity_input_stack[i].p_base == p_input_text_in )
        {
            error( EET_FATAL, ELMX_BAD_DTD, std::string( "Recursive entity definition for: " ) + p_input_text_in );
            return false;
        }
    }

    m_entity_input_stack.push_back( c_dtd_active_entity( p_input_text_in ) );

    return true;
}

bool c_get_with_entities::select_general_entity( c_entity_name_reader & r_entity_tag_reader_in, std::string * p_value_out )      // C101112_1316
{
    // We have read '&'

    int c = r_entity_tag_reader_in.get();

    if( c == '#' )
    {
        if( ! insert_character_entity( r_entity_tag_reader_in, p_value_out ) )
            return false;
    }

    else
    {
        m_entity_tag.erase();

        m_entity_tag += static_cast<char>( c );

        while( (c = r_entity_tag_reader_in.get()) != c_read::k_eof && is_xml_name_char( c ) )
            m_entity_tag += static_cast<char>( c );

        if( c != ';' )
            r_entity_tag_reader_in.unget( c );

        // Fast path the standard entities
        if( m_entity_tag.size() == 2 )
        {
            if( m_entity_tag == "lt" )
            {
                *p_value_out += "<";
                return true;
            }
            if( m_entity_tag == "gt" )
            {
                *p_value_out += ">";
                return true;
            }
        }

        if( m_entity_tag.size() == 3 )
        {
            if( m_entity_tag == "amp" )
            {
                *p_value_out += "&";
                return true;
            }
        }

        if( m_entity_tag.size() == 4 )
        {
            if( m_entity_tag == "apos" )
            {
                *p_value_out += "'";
                return true;
            }

            else if( m_entity_tag == "quot" )
            {
                *p_value_out += "\"";
                return true;
            }
        }

        const char * p_entity_value;

        if( get_general_entity( m_entity_tag.c_str(), &p_entity_value ) )    // C101112_2112 - Refactored the order of this code
        {
            if( r_entity_tag_reader_in.is_markup_possible_in_entity() )
            {
                // If the entity can contain markup, then after selecting the entity
                // -we need to return to the main calling loop to do further parsing
                set_input_entity_value( p_entity_value );
            }

            else
            {
                if( ! p_append_entity( p_entity_value, p_value_out ) )
                    return false;
            }
        }

        else
        {
            // This is being liberal
            *p_value_out += "&";
            *p_value_out += m_entity_tag;
            if( c == ';' )
                *p_value_out += static_cast<char>( c );

            #if defined( LMX_XML_STRICT ) && LMX_XML_STRICT != 0    // Added in V3.2.4
            return false;
            #endif
        }
    }

    return true;
}

bool c_get_with_entities::p_append_entity( const char * p_entity_value_in, std::string * p_value_out )       // C101112_1316
{
    while( *p_entity_value_in != '\0' )
    {
        if( *p_entity_value_in == '&' || *p_entity_value_in == '%' )
        {
            c_entity_name_reader_for_entity entity_tag_reader( &p_entity_value_in );
            ++p_entity_value_in;

            if( ! select_general_entity( entity_tag_reader, p_value_out ) )
                return false;
        }
        else
            add_char_wsp( p_value_out, *p_entity_value_in );

        if( *p_entity_value_in != '\0' )  // May have been moved on by p_select_entity
            ++p_entity_value_in;
    }

    return true;
}

bool c_get_with_entities::insert_character_entity( c_entity_name_reader & r_entity_tag_reader_in, std::string * p_value_out )      // C101112_1316
{
    // Have read '&#'

    m_entity_tag.erase();
    int c;
    bool is_hex = false;

    c = r_entity_tag_reader_in.get();

    if( c == 'x' )
        is_hex = true;
    else if( xisdigit( c ) )
        m_entity_tag += static_cast<char>( c );
    else
        return false;

    while( (c = r_entity_tag_reader_in.get()) != c_read::k_eof && is_hex ? xisxdigit( c ) : xisdigit( c ) )
        m_entity_tag += static_cast<char>( c );

    if( c != ';' )
        r_entity_tag_reader_in.unget( c );

    if( m_entity_tag.empty() )
        return false;

    tlmx_uns32 entity_value;

    if( is_hex )
        entity_value = LMXSTDCRT::strtoul( m_entity_tag.c_str(), LMXNULL, 16 );
    else
        entity_value = LMXSTDCRT::atoi( m_entity_tag.c_str() );

    if( entity_value > 0x100000 )
        return false;


    if( entity_value == 0x85 || entity_value == 0x2028 )        // XML1.1: Map NEL (New End of Line) and
        add_char_wsp( p_value_out, static_cast<char>('\x0a'), true );  // -Unicode Line sep to 0x0a to ease processing

    else if( entity_value <= 0x7f )
        add_char_wsp( p_value_out, static_cast<char>(entity_value), true );

    else
    {
        tc_utf8_buf c_utf8;
        *p_value_out += unicode_char_to_utf8( entity_value, c_utf8 );   // Can't be white space so can just add it.
    }

    return true;
}

void c_get_with_entities::add_char_wsp( std::string * p_value_out, int c_in, bool is_from_char_entity_in/*=false*/ )  // C101112_1316
{
    // Add char with white space processing

    switch( m_ws_mode )
    {
    case EXWS_PRESERVE:
        *p_value_out += static_cast<char>( c_in );
    return;

    case EXWS_REPLACE:
        if( ( ! is_from_char_entity_in || m_is_reading_xsd ) && xisspace( c_in ) )
            c_in = ' ';
        *p_value_out += static_cast<char>( c_in );
    return;

    case EXWS_COLLAPSE:
        if( ( ! is_from_char_entity_in || m_is_reading_xsd ) && xisspace( c_in ) )
            c_in = ' ';

        if( c_in == ' ' )
            m_ws_proc_got_white = true;
        else
        {
            if( m_ws_proc_got_white )
            {
                if( ! p_value_out->empty() )
                    *p_value_out += ' ';
                m_ws_proc_got_white = false;
            }
            *p_value_out += static_cast<char>( c_in );
        }
    return;

    default:
        lmx_assert( 0 );
    }

    lmx_assert_code_not_reached();    // Should not get here
}

void c_get_with_entities::set_ws_mode( elmx_whitespace ws_mode_in )      // C101112_1316
{
    m_ws_mode = ws_mode_in;
    m_ws_proc_got_white = false;
}

//------------------------------------------------------------------------------
//                           c_get_for_xml class
//------------------------------------------------------------------------------

c_get_for_xml::c_get_for_xml( c_read & r_reader_in, c_error & r_error_in, bool is_reading_xsd_in /*=true*/ )
    :
    c_get_with_entities( r_reader_in, r_error_in, is_reading_xsd_in ),
    m_has_pi( false )     // For LMX_WANT_COMMENT_AND_PI
{
    p_init();
}

c_get_for_xml::~c_get_for_xml()
{
}

void c_get_for_xml::p_init()
{
    set_ws_mode( EXWS_PRESERVE );
}

void c_get_for_xml::rewind()
{
    p_init();
    c_get_with_entities::rewind();
}

bool c_get_for_xml::read_specified_string_from_input( const char * p_input_string_to_be_read_in )
{
    // Read from the input, characters corresponding to those contained in ap_input_string_to_be_read, or fail

    while( *p_input_string_to_be_read_in )
        if( get() != *p_input_string_to_be_read_in++ )
            return false;
    return true;
}

bool c_get_for_xml::get_simple_quoted_value( std::string * p_value_out )
{
    // Does not do entity expansion

    p_value_out->erase();
    int c = get_non_ws();

    if( c != '\'' && c != '"' )
        return false;

    int end = c;

    while( (c = get()) != end && c != c_read::k_eof )
        *p_value_out += static_cast<char>( c );

    if( c == c_read::k_eof )
        return false;

    return true;
}

bool c_get_for_xml::read_xml_decl()
{
    // VersionInfo EncodingDecl? SDDecl?

    std::string value;
    int c = get_non_ws();

    if( c == 'v' && read_specified_string_from_input( "ersion" ) &&
            (c = get_non_ws()) == '=' && get_simple_quoted_value( &value ) )
    {
        if( value != "1.0" && value != "1.1" )
        {
            error( EET_ERROR, ELMX_BAD_XML_VERSION, std::string( "XML version " ) + value + " not understood" );
            return false;
        }

        c = get_non_ws();
    }
    else
    {
        error( EET_ERROR, ELMX_BAD_XML_DECL, "Version not present in XML Decl" );
        return false;
    }


    if( c == 'e' && read_specified_string_from_input( "ncoding" ) &&
            (c = get_non_ws()) == '=' && get_simple_quoted_value( &value ) )
    {
        if( ! set_encoding( value.c_str() ) )
            return false;   // Error message set in set_encoding()

        c = get_non_ws();
    }

    if( c == 's' && read_specified_string_from_input( "tandalone" ) &&
            (c = get_non_ws()) == '=' && get_simple_quoted_value( &value ) )
    {
        // v5 (C100307_1217) - standalone="no" only means there MAY be external markup declarations, so it's not conclusive at this stage
        //if( l_value == "no" )
        //    {
        //    error( EET_ERROR, ELMX_BAD_XML_DECL, "XML Document is not standalone" );
        //    return false;
        //    }

        c = get_non_ws();
    }

    if( c == '?' )
    {
        if( (c = get()) == '>' )
            return true;
    }

    return false;
}

bool c_get_for_xml::skip_comment()
{
    int c;
    size_t dash_count = 0;
    #ifdef LMX_WANT_COMMENT_AND_PI
        std::string comment;
    #endif // LMX_WANT_COMMENT_AND_PI

    while( (c = get()) != c_read::k_eof )
    {
        if( c == '>' && dash_count >= 2 )
        {
            if( dash_count > 2 )
                error( EET_WARNING, ELMX_BAD_COMMENT, "End of comment not Well-Formed" );

            #ifdef LMX_WANT_COMMENT_AND_PI
                m_comment_collection.push_back( comment );
            #endif // LMX_WANT_COMMENT_AND_PI

            return true;
        }

        else if( c == '-' )
            ++dash_count;

        else
        {
            #ifdef LMX_WANT_COMMENT_AND_PI
                for( size_t i=0; i<dash_count; ++i )
                    comment += '-';
                comment += c;
            #endif // LMX_WANT_COMMENT_AND_PI

            dash_count = 0;
        }
    }

    return false;
}

bool c_get_for_xml::skip_PI()
{
    #ifdef LMX_WANT_COMMENT_AND_PI
        m_has_pi = true;
    #endif // LMX_WANT_COMMENT_AND_PI

    int last = '\0';
    int c;

    while( (c = get()) != c_read::k_eof )
    {
        if( c == '>' && last == '?' )
            return true;
        last = c;
    }

    return false;
}

bool c_get_for_xml::move_to_start_tag_end( bool * p_end_of_element_in /* = LMXNULL */ )
{
    // Skip element tag, allowing > to appear within an attribute value
    // Assumes that name of tag has already been read

    // Attributes may or may _not_ have been scanned into attribute stores
    // when this function is called.  The function handles both cases equally.

    int c;
    int string_end = '\0';

    while( (c = get()) != c_read::k_eof )
    {
        if( string_end == '\0' && c == '>' )
        {
            if( p_end_of_element_in )
                *p_end_of_element_in = false;
            return true;
        }

        else if( string_end == '\0' && c == '/' )
        {
            bool error_reported = false;
            while( (c = get()) != c_read::k_eof && c != '>' )
            {
                if( ! xisspace( c ) && ! error_reported )
                {
                    error( EET_ERROR,
                           ELMX_BAD_END_TAG,
                           std::string( "Illegal character(s) after / at end of tag - " ) +
                                        static_cast<char>(c) );
                    error_reported = true;
                }
            }

            if( p_end_of_element_in )
                *p_end_of_element_in = true;

            return error_reported ? false : true;
        }

        else if( c == '\'' || c == '"' )
        {
            if( string_end == '\0' )
                string_end = c;
            else if( string_end == c )
                string_end = '\0';
        }
    }

    return false;
}

//------------------------------------------------------------------------------
//                           c_dtd_util class
//              Parts common to both internal and external DTDs.
//------------------------------------------------------------------------------

c_dtd_util::c_dtd_util( c_get_for_xml & r_get_in )
    : mr_get( r_get_in )
{
}

bool c_dtd_util::get_entity_value( std::string * p_value_out )
{
    int c = mr_get.get();

    while( xisspace( c ) )
        c = mr_get.get();

    if( c == c_read::k_eof )
        return false;   // Allow higher level read operation to report error

    while( c != c_read::k_eof && ! xisspace( c ) && c != '>' )
    {
        if( c == '%' )
        {
            if( ! select_parameter_entity() )
                return false;
        }
        else
            *p_value_out += static_cast<char>( c );

        c = mr_get.get();
    }

    mr_get.unget( c );

    return true;
}

bool c_dtd_util::get_quoted_entity_value( std::string * p_value_out )
{
    // Reads an attribute and does suitable expansion
    // - Also does attribute normalisation

    int c;

    while( (c = mr_get.get()) != c_read::k_eof && xisspace( c ) )
        {}

    if( c == c_read::k_eof )
        return false;   // Allow higher level read operation to report error

    if( c == '"' || c == '\'' )
    {
        p_value_out->erase();

        int end_c = c;

        size_t start_entity_stack_depth = mr_get.get_entity_stack_depth();

        while( (c = mr_get.get()) != c_read::k_eof &&
                ( c != end_c || start_entity_stack_depth != mr_get.get_entity_stack_depth() ) )
        {
            if( c == '%' )
            {
                if( ! select_parameter_entity() )
                    return false;
            }

            else if( c == '&' )
            {
                int extra = mr_get.get();
                if( extra == '#' )
                {
                    c_entity_name_reader_for_c_get entity_reader( mr_get );

                    if( ! mr_get.insert_character_entity( entity_reader, p_value_out ) )
                        return false;
                }
                else
                {
                    // General entity - not expanded here
                    *p_value_out += static_cast<char>( c );
                    if( extra != c_read::k_eof )
                        *p_value_out += static_cast<char>( extra );
                }
            }

            else
                *p_value_out += static_cast<char>( c );
        }

        return true;
    }

    else
    {
        return false;
    }
}

bool c_dtd_util::select_parameter_entity()
{
    std::string entity_tag;
    int c;

    while( (c = mr_get.get()) != c_read::k_eof && is_xml_name_char( c ) )
        entity_tag += static_cast<char>( c );

    if( c != ';' )
        mr_get.unget( c );

    const char * p_entity_value;

    if( mr_get.get_parameter_entity( entity_tag.c_str(), &p_entity_value ) )
    {
        if( ! mr_get.set_input_entity_value( p_entity_value ) )
            return false;
    }
    else
    {
        mr_get.error( EET_ERROR, ELMX_ENTITY_NOT_FOUND, std::string( "Entity '%" ) + entity_tag + "' not found" );
    }

    return true;
}

//------------------------------------------------------------------------------
//                           c_internal_dtd class
//------------------------------------------------------------------------------

c_internal_dtd::c_internal_dtd( c_get_for_xml & r_get_in )
    : m_dtd_util( r_get_in )
{
}

void c_internal_dtd::reset()
{
}

bool c_internal_dtd::read_doctype_decl()
{
    // doctypedecl ::= '<!DOCTYPE' S Name (S ExternalID)? S?
    //      ('[' (markupdecl | DeclSep)* ']' S?)? '>'
    // markupdecl ::= elementdecl | AttlistDecl | EntityDecl | NotationDecl |
    //      PI | Comment
    // DeclSep ::= PEReference | S
    // PEReference ::= '%' Name ';'     ; Parameter entity - when expand may map to
    //                                  ; -markupdecl

    // We have already read the '<!DOCTYPE' part

    int c;

    while( (c = m_dtd_util.get()) != c_read::k_eof && c != '[' && c != '>' )
        {}

    if( c == c_read::k_eof )
        return false;   // Allow higher level read operation to report error

    else if( c == '>' )
        {}  // No markupDecl - Just exit

    else
    {
        std::string name;

        for( ;; )
        {
            while( (c = m_dtd_util.get()) != c_read::k_eof && xisspace( c ) )
                {}

            if( c == '<' )
            {
                name.erase();

                p_get_markupdecl_name( &name );

                if( name == "!--" )
                {
                    if( ! m_dtd_util.m_getter().skip_comment() )
                        return false;
                }

                else if( name == "!ENTITY" )
                {
                    if( ! p_read_dtd_entity() )
                        return false;
                }

                else if( ! name.empty() && name[0] == '?' ) // C100928_1704
                {
                    if( ! m_dtd_util.m_getter().skip_PI() )
                        return false;
                }

                else
                {
                    // e.g. !NOTATION
                    if( ! p_skip_to_end_of_markupdecl() )
                        return false;
                }
            }

            else if( c == ']' )
            {
                while( (c = m_dtd_util.get()) != c_read::k_eof && xisspace( c ) )
                    {}

                if( c != '>' )
                {
                    m_dtd_util.m_getter().error(
                                EET_FATAL,
                                ELMX_BAD_DTD,
                                std::string( "DTD not Well Formed (expected '>', got " ) +
                                        static_cast<char>( c ) + ")" );
                    return false;
                }

                break;  // Main exit point
            }

            else if( c == '%' )
            {
                if( ! m_dtd_util.select_parameter_entity() )
                    return false;   // V3.2.4 - previously did not return false on error.
            }

            else
            {
                m_dtd_util.m_getter().error(
                            EET_FATAL,
                            ELMX_BAD_DTD,
                            std::string( "DTD not Well Formed (got " ) + static_cast<char>(c) + ")" );
                return false;
            }
        }
    }

    return true;
}

void c_internal_dtd::p_get_markupdecl_name( std::string * p_name_out )
{
    p_name_out->erase();

    int c;

    c = m_dtd_util.get();

    if( c == c_read::k_eof )
        return;     // Allow higher level read operation to report error

    while( c != c_read::k_eof &&
                ! xisspace( c ) && c != '[' && c != '>' )
    {
        *p_name_out += static_cast<char>( c );
        if( p_name_out->size() == 3 && *p_name_out == "!--" )
            return;         // Start of a comment
        c = m_dtd_util.get();
    }

    m_dtd_util.unget( c );
}

bool c_internal_dtd::p_read_dtd_entity()
{
    // EntityDecl::=    GEDecl | PEDecl
    // GEDecl    ::=    '<!ENTITY' S Name S EntityDef S? '>'
    // PEDecl    ::=    '<!ENTITY' S '%' S Name S PEDef S? '>'
    // EntityDef ::=    EntityValue | (ExternalID NDataDecl?)
    // PEDef     ::=    EntityValue | ExternalID

    // EntityValue ::= '"' ([^%&"] | PEReference | Reference)* '"'
    //    |  "'" ([^%&'] | PEReference | Reference)* "'"
    // ExternalID::=    'SYSTEM' S SystemLiteral |
    //    'PUBLIC' S PubidLiteral S SystemLiteral
    // NDataDecl ::=    S 'NDATA' S Name

    // PEReference ::= '%' Name ';'     ; Parameter entity - when expand may map to
    //                                  ; -markupdecl
    // Reference ::= EntityRef | CharRef
    // EntityRef ::= '&' Name ';'
    // CharRef ::= '&#' [0-9]+ ';'
    //    | '&#x' [0-9a-fA-F]+ ';'

    // Skip ExternalID references after emitting a warning

    // We've already read '!ENTITY'

    std::string name;
    std::string value;
    bool is_parameter_entity = false;
    bool success;
    int c;

    while( (c = m_dtd_util.get()) != c_read::k_eof && xisspace( c ) )
        {}

    if( c == '%' )
        is_parameter_entity = true;
    else
        m_dtd_util.unget( c );

    success = m_dtd_util.get_entity_value( &name );

    if( success )
    {
        // Check what sort of entity definition we have before trying to read it

        while( (c = m_dtd_util.get()) != c_read::k_eof && xisspace( c ) )
            {}

        m_dtd_util.unget( c );

        if( c == '"' || c == '\'' )
            success = m_dtd_util.get_quoted_entity_value( &value );

        else
        {
            success = m_dtd_util.get_entity_value( &value );

            if( value == "SYSTEM" || value == "PUBLIC" )
                m_dtd_util.m_getter().error( EET_WARNING, ELMX_EXTERNAL_ENTITY, "External Entity definitions not supported" );
            else
                m_dtd_util.m_getter().error( EET_ERROR, ELMX_UNEXPECTED_ENTITY, std::string( "Unexpected value in Entity definition: " ) + value );
        }
    }


    if( success )
    {
        if( is_parameter_entity )
            m_dtd_util.m_getter().add_parameter_entity( name.c_str(), value.c_str() );
        else
            m_dtd_util.m_getter().add_general_entity( name.c_str(), value.c_str() );
    }


    p_skip_to_end_of_markupdecl();   // IF EOF encountered, next read will return it

    return success;
}

bool c_internal_dtd::p_skip_to_end_of_markupdecl()
{
    // Skip markupdecl, allowing > to appear within an attribute value
    // Assumes that name of tag has already been read

    int c;
    int string_end = '\0';

    while( (c = m_dtd_util.get()) != c_read::k_eof )
    {
        if( string_end == '\0' && c == '>' )
            return true;

        else if( c == '\'' || c == '"' )
        {
            if( string_end == '\0' )
                string_end = c;
            else if( string_end == c )
                string_end = '\0';
        }
    }

    return false;
}

//------------------------------------------------------------------------------
//                           c_xml_list_tokeniser
//------------------------------------------------------------------------------

c_xml_list_tokeniser::c_xml_list_tokeniser( const std::string & r_string_in )
{
    m_string = r_string_in;
    mi_position = m_string.begin();
}

void c_xml_list_tokeniser::set( const std::string & r_string_in )
{
    m_string = r_string_in;
    mi_position = m_string.begin();
}

bool c_xml_list_tokeniser::get( std::string * p_sub_string_in )
{
    std::string::const_iterator i_end = m_string.end();

    while( mi_position != i_end && xisspace( *mi_position ) )
        ++mi_position;

    if( mi_position == i_end )
        return false;

    std::string::const_iterator i_start = mi_position;

    while( mi_position != i_end && ! xisspace( *mi_position ) )
        ++mi_position;

    p_sub_string_in->assign( i_start, mi_position );

    return true;
}

size_t c_xml_list_tokeniser::count( const std::string & r_string_in )
{
    size_t size = 0;
    c_xml_list_tokeniser list_tok( r_string_in );
    std::string list_value;

    while( list_tok.get( &list_value ) )
        ++size;

    return size;
}

//------------------------------------------------------------------------------
//                               c_enumerated_namespaces
//------------------------------------------------------------------------------

c_enumerated_namespaces::c_enumerated_namespaces()
{
    clear();
}

void c_enumerated_namespaces::clear()
{
    m_namespaces.clear();

    m_next_enumeration = EXKN_DYNAMIC_START;
    m_namespaces.set( "http://www.w3.org/2001/XMLSchema-instance", EXKN_XSI );
    m_namespaces.set( "http://www.w3.org/XML/1998/namespace", EXKN_XML );
}

int c_enumerated_namespaces::find_namespace( const std::string & r_namespace_uri_in ) const
{
    int namespace_enumeration;

    if( m_namespaces.get( r_namespace_uri_in.c_str(), &namespace_enumeration ) )
        return namespace_enumeration;
    return EXKN_UNKNOWN;
}

const char * c_enumerated_namespaces::find_namespace_from_id( int namespace_id_in ) const
{
    if( namespace_id_in == EXKN_NONE )   // v4.1 - Now allow find_namespace_from_id() to handle EXKN_NONE
        return c_xml::k_non_namespace.c_str();
    return m_namespaces.reverse_lookup( namespace_id_in );
}

int c_enumerated_namespaces::add_namespace( const char * p_namespace_uri_in )
{
    int namespace_id;

    // If not already enumerated, then enumerate it...
    if( ! m_namespaces.get( p_namespace_uri_in, &namespace_id ) )
    {
        namespace_id = m_next_enumeration++;
        m_namespaces.set( p_namespace_uri_in, namespace_id );
    }

    return namespace_id;
}

void c_enumerated_namespaces::add_namespace( const char * p_namespace_uri_in, int value_in )
{
    if( ! m_namespaces.is_defined( p_namespace_uri_in ) )
        m_namespaces.set( p_namespace_uri_in, value_in );

    if( value_in >= m_next_enumeration )
        m_next_enumeration = value_in + 1;
}

//------------------------------------------------------------------------------
//                               c_xml_local
//------------------------------------------------------------------------------

// Note: Active_prev - To speed things up, we attempt to only look for namespace
// information in c_xml_local instances that have had namespace information
// added to them.  This is done using the p_active member, which points to the
// next member containing namespace information.  Note that if a c_xml_local
// instance contains namespace information, then p_active will point to itself
// (p_active == this).  Thus some caution is required when moving from one
// active instance of c_xml_local to another, especially as p_previous may be
// NULL.
int c_xml_local::find_namespace_id( const std::string & r_namespace_prefix_in ) const
{
    if( r_namespace_prefix_in == "xml" )
        return EXKN_XML;

    const c_xml_local * p_local = mp_active;
    while( p_local )
    {
        int namespace_id;
        if( p_local->m_namespaces.get( r_namespace_prefix_in.c_str(), &namespace_id ) )
            return namespace_id;

        // See note Active_prev
        p_local = p_local->mp_previous;
        if( p_local )
            p_local = p_local->mp_active;
    }

    if( r_namespace_prefix_in.empty() )   // v4.1
        return EXKN_NONE;

    return EXKN_UNKNOWN;
}

const char *c_xml_local::find_namespace_prefix( int namespace_id_in ) const
{
    if( namespace_id_in == EXKN_XML )
        return "xml";

    const c_xml_local * p_local = mp_active;
    while( p_local )
    {
        const char * p_namespace_prefix = p_local->m_namespaces.reverse_lookup( namespace_id_in );
        if( p_namespace_prefix )
            return p_namespace_prefix;

        // See note Active_prev
        p_local = p_local->mp_previous;
        if( p_local )
            p_local = p_local->mp_active;
    }

    return LMXNULL;
}

const char *c_xml_local::find_first_namespace_prefix( int namespace_id_in ) const
{
    if( namespace_id_in == EXKN_XML )
        return "xml";

    const char * p_first_namespace_prefix = LMXNULL;
    const c_xml_local * p_local = mp_active;
    while( p_local )
    {
        const char * p_namespace_prefix = p_local->m_namespaces.reverse_lookup( namespace_id_in );
        if( p_namespace_prefix )
            p_first_namespace_prefix = p_namespace_prefix;

        // See note Active_prev
        p_local = p_local->mp_previous;
        if( p_local )
            p_local = p_local->mp_active;
    }

    return p_first_namespace_prefix;
}

void c_xml_local::add_namespace( const char * p_namespace_prefix_in, int namespace_id_in )
{
    m_namespaces.set( p_namespace_prefix_in, namespace_id_in );
    mp_active = this;
}

void c_xml_local::extract_namespace_context( c_namespace_context * p_namespace_context_in, const c_xml & r_xml_in ) const // v4.1.2
{
    // We want more recently defined namespace to have priority over deeper namespaces,
    // so go deep first and then actually extract the namespace info on the return path.
    if( mp_active && mp_active != this )
        mp_active->extract_namespace_context( p_namespace_context_in, r_xml_in );
    else if( mp_previous && mp_previous->mp_active )
        mp_previous->mp_active->extract_namespace_context( p_namespace_context_in, r_xml_in );

    for( t_namespaces::const_iterator i_ns_info( m_namespaces.begin() ), i_ns_info_end( m_namespaces.end() );
            i_ns_info != i_ns_info_end;
            ++i_ns_info )
    {
        std::string ns_prefix = m_namespaces.get_key( i_ns_info );
        int ns_id = m_namespaces.get_value( i_ns_info );
        std::string ns_uri = r_xml_in.find_namespace_from_id( ns_id );
        p_namespace_context_in->add( ns_prefix, ns_uri );
    }
}

//------------------------------------------------------------------------------
//                                c_xml
//                     This class understands XML things
//                     such as namespaces but not Schema.
//------------------------------------------------------------------------------

const std::string c_xml::k_non_namespace( "" );
const std::string c_xml::k_namespace_not_found( " " );  // This can not match any valid namespace string

c_xml::c_xml( c_read & r_reader_in, c_error & r_error_in, bool is_reading_xsd_in /*=true*/ )
    :
    m_getter( r_reader_in, r_error_in, is_reading_xsd_in ),
    m_dtd( m_getter ),
    m_dtd_read( false ),
    m_element_read( false )
{
}

void c_xml::rewind()
{
    m_enumerated_namespaces.clear();
    m_dtd.reset();
    m_dtd_read = false;
    m_element_read = false;
    m_getter.rewind();
}

bool c_xml::p_select_entity( c_entity_name_reader & r_entity_tag_reader_in, std::string * p_value_out )
{
    // We have read '&'
    return m_getter.select_general_entity( r_entity_tag_reader_in, p_value_out );
}

bool c_xml::p_capture_attribute_value( c_attribute_data * p_attribute_data_in, size_t * p_initial_index_in /*=LMXNULL*/ )
{
    // Reads an attribute but does not do expansion

    int c;

    c = m_getter.get_non_ws();

    if( c == c_read::k_eof )
    {
        m_getter.error( EET_FATAL, ELMX_UNEXPECTED_EOM, "Unexpected end of file" );
        return false;
    }

    else if( c != '=' )
    {
        m_getter.error(
                    EET_FATAL,
                    ELMX_NOT_WELL_FORMED,
                    std::string( "Schema is not Well Formed (expected '=', got '" ) + static_cast<char>(c) + "')" );
        return false;
    }

    c = m_getter.get_non_ws();

    if( c == '"' || c == '\'' )
    {
        int end_c = c;
        p_attribute_data_in->put( static_cast<char>( end_c ) );  // Store the attribute quotes (see attribute_data_note for more info.)

        if( p_initial_index_in )
            *p_initial_index_in = p_attribute_data_in->size();

        while( (c = m_getter.get()) != c_read::k_eof && c != end_c )
        {
            // Do not expand entities at this stage.  See attribute_data_note for more info.
            p_attribute_data_in->put( static_cast<char>( c ) );
        }

        p_attribute_data_in->mark_segment_end();

        if( c == end_c )
            return true;
    }

    m_getter.error(
            EET_FATAL,
            ELMX_NOT_WELL_FORMED,
            std::string( "Schema is not Well Formed (expected '\"' or ''', got " ) + static_cast<char>(c) + ")" );

    return false;
}

bool c_xml::p_capture_attributes( c_xml_local * p_local_control_in /*=LMXNULL*/ )
{
    attribute_data.clear();
    m_namespace_attribute_data.clear();

    int c;
    const size_t k_length_of_xmlns = 5; // Length of "xmlns" string
    char c_attr_name_start[k_length_of_xmlns + 1];
    size_t attr_name_start_length;

    for( ;; )
    {
        c = m_getter.get_non_ws();

        if( ! is_xml_name_start_char( c ) )
        {
            m_getter.unget( c );
            break;
        }

        for( attr_name_start_length=0;
                attr_name_start_length < k_length_of_xmlns && c == "xmlns"[attr_name_start_length];
                ++attr_name_start_length )
        {
            c_attr_name_start[attr_name_start_length] = static_cast<char>( c );
            c = m_getter.get();
        }
        c_attr_name_start[attr_name_start_length] = '\0';

        if( p_local_control_in && attr_name_start_length == k_length_of_xmlns &&
                (c == ':' || ! is_xml_name_char( c )) )     // Test ap_local_control because we may be in xml_decl or other PI
        {
            m_namespace_attribute_data.append( c_attr_name_start, &c_attr_name_start[attr_name_start_length] );

            size_t ns_prefix_index = m_namespace_attribute_data.size(); // Will end up pointing to empty string if no prefix

            // Have "xmlns".  Following character can be ":", but not anything else
            if( c == ':' )
            {
                m_namespace_attribute_data.put( static_cast<char>( c ) );

                ns_prefix_index = m_namespace_attribute_data.size();

                c = m_getter.get();
                while( is_xml_name_char( c ) )
                {
                    m_namespace_attribute_data.put( static_cast<char>( c ) );
                    c = m_getter.get();
                }
            }

            m_getter.unget( c );
            m_namespace_attribute_data.mark_segment_end();

            size_t ns_uri_index = ~static_cast<size_t>( 0 );

            if( ! p_capture_attribute_value( &m_namespace_attribute_data, &ns_uri_index ) )
                return false;

            const char * p_ns_uri = m_namespace_attribute_data.get_str( ns_uri_index );

            // Optimistically we won't have entities in the URI, so only do entity expansion if we detect one
            if( LMXSTDCRT::strchr( p_ns_uri, '&' ) )
            {
                std::string entity_expanded_namespace_uri;
                for( size_t i=0; p_ns_uri[i] != '\0'; ++i )
                {
                    if( p_ns_uri[i] == '&' )
                    {
                        c_entity_name_reader_for_c_str entity_reader( p_ns_uri, i );

                        if( ! p_select_entity( entity_reader, &entity_expanded_namespace_uri ) )
                            return false;

                        i = entity_reader.get_index() - 1;  // -1 allows for ++l_i on next loop iteration
                    }
                    else
                        entity_expanded_namespace_uri += p_ns_uri[i];
                }

                int new_namespace_id = m_enumerated_namespaces.add_namespace( entity_expanded_namespace_uri.c_str() );

                p_local_control_in->add_namespace( m_namespace_attribute_data.get_str( ns_prefix_index ), new_namespace_id );
            }

            else
            {   // Reworked in v4.1
                const char * p_ns_prefix = m_namespace_attribute_data.get_str( ns_prefix_index );
                if( *p_ns_uri == '\0' )
                {
                    // See xml_1_1_namespace_disabling
                    if( *p_ns_prefix == '\0' ) // No prefix specified
                        p_local_control_in->add_namespace( "", EXKN_NONE );
                    else // prefix specified
                        p_local_control_in->add_namespace( p_ns_prefix, EXKN_UNKNOWN );
                }
                else
                {
                    int new_namespace_id = m_enumerated_namespaces.add_namespace( p_ns_uri );

                    p_local_control_in->add_namespace( p_ns_prefix, new_namespace_id );
                }
            }
        }
        else
        {
            attribute_data.append( c_attr_name_start, &c_attr_name_start[attr_name_start_length] );

            while( is_xml_name_char( c ) )
            {
                attribute_data.put( static_cast<char>( c ) );
                c = m_getter.get();
            }
            m_getter.unget( c );

            attribute_data.mark_segment_end();

            if( ! p_capture_attribute_value( &attribute_data ) )
                return false;
        }
    }

    attribute_data.mark_segment_end();  // Mark the end of the attribute data

    if( c == c_read::k_eof )
        return false;

    return true;
}

bool c_xml::p_read_cdata( std::string * p_value_out )
{
    // Return true if CDATA successfully read, or false otherwise.

    // Only called if have already read ![

    if( ! m_getter.read_specified_string_from_input( "CDATA[" ) )
    {
        m_getter.error( EET_ERROR, ELMX_NOT_WELL_FORMED, "XML not Well-Formed" );
        return false;
    }

    size_t sq_close_count = 0;
    int c;
    size_t i;
    while( (c = m_getter.get()) != c_read::k_eof && (sq_close_count < 2 || c != '>') )
    {
        if( c == ']' )
            ++sq_close_count;
        else
            {
            for( i=0; i<sq_close_count; ++i )
                *p_value_out += ']';
            sq_close_count = 0;
            *p_value_out += static_cast<char>( c );
        }
    }

    // If we have more than 2 ] characters at the end, add them to the value
    // N.B. Don't subtract 2 from l_sq_close_count to avoid unsigned problems.
    for( i=2; i<sq_close_count; ++i )
        *p_value_out += ']';

    return true;
}

bool c_xml::p_get_xml_text( std::string * p_text_in )  // Read text part of mixed content
{
    // Do _not_ erase ap_text as it may already have desired text in it

    m_getter.reset_ws_mode();

    int c = m_getter.get();

    while( c != '<' && c != c_read::k_eof )
    {
        if( c == '&' )
        {
            c_entity_name_reader_for_c_get entity_reader( m_getter );
            entity_reader.is_markup_possible_in_entity( true );

            if( ! p_select_entity( entity_reader, p_text_in ) )
                return false;

            c = m_getter.get();
        }

        else
        {
            m_getter.add_char_wsp( p_text_in, c );
            c = m_getter.get();
        }
    }

    m_getter.unget( c );

    return true;
}

void c_xml::add_namespace( const char * p_namespace_uri_in, int value_in )
{
    m_enumerated_namespaces.add_namespace( p_namespace_uri_in, value_in );
}

int c_xml::add_namespace( const char * p_namespace_uri_in )
{
    return m_enumerated_namespaces.add_namespace( p_namespace_uri_in );
}

int c_xml::find_enumerated_namespace( const std::string & r_namespace_in ) const
{
    return m_enumerated_namespaces.find_namespace( r_namespace_in );
}

const char * c_xml::find_namespace_from_id( int namespace_id_in ) const
{
    return m_enumerated_namespaces.find_namespace_from_id( namespace_id_in );
}

bool c_xml::p_handle_lt_qmark()     // Have read "<?"
{
    // Look for ?xml
    if( m_getter.read_specified_string_from_input( "xml" ) && xisspace( m_getter.get() ) )  // v3.8 - ensured space after 'xml'
    {
        if( m_element_read )
        {
            m_getter.error( EET_FATAL, ELMX_BAD_XML_DECL, "XML declaration not at start of document" );
            return false;
        }

        else if( ! m_getter.read_xml_decl() )
            return false;

        // loop round to get next element
    }

    else
    {
        if( ! m_getter.skip_PI() )
            return false;
    }

    return true;
}

bool c_xml::p_handle_lt_pling( std::string * p_text_in, elxp_xml_event * p_returnable_event_in )      // Have read "<!"
{
    // Returns true if calling function should keep on processing input, and false if
    // -calling function should return ap_returnable event.

    int c = m_getter.get();

    if( c == '[' )
    {
        // Look for CDSect (<![CDATA[blah]]>)
        p_text_in->erase();

        if( p_read_cdata( p_text_in ) )
            *p_returnable_event_in = EXE_CDATA;
        else
        {
            m_getter.error( EET_ERROR, ELMX_NOT_WELL_FORMED, "XML not Well-Formed" );
            *p_returnable_event_in = EXE_BADLY_FORMED;
        }

        return false;
    }

    // Look for !--
    else if( c == '-' && m_getter.get() == '-' )
    {
        // When complete, loop round to get next element
        if( ! m_getter.skip_comment() )
        {
            *p_returnable_event_in = EXE_BADLY_FORMED;
            return false;
        }
    }

    // Look for !DOCTYPE
    else if( c == 'D' && m_getter.read_specified_string_from_input( "OCTYPE" ) )
    {
        // When complete, loop round to get next element
        if( m_element_read )
        {
            m_getter.error( EET_ERROR, ELMX_BAD_DTD, "Document Type Declaration not at start of document" );
            *p_returnable_event_in = EXE_BADLY_FORMED;
            return false;
        }

        else if( m_dtd_read )
        {
            m_getter.error( EET_ERROR, ELMX_BAD_DTD, "Document Type Declaration already read" );
            *p_returnable_event_in = EXE_BADLY_FORMED;
            return false;
        }

        else if( ! m_dtd.read_doctype_decl() )
        {
            *p_returnable_event_in = EXE_BADLY_FORMED;
            return false;
        }

        m_dtd_read = true;
    }

    else
    {
        m_getter.move_to_start_tag_end();
        *p_returnable_event_in = EXE_BADLY_FORMED;
        return false;
    }

    return true;
}

std::string & c_xml::get_local_name( const std::string & r_name_in, std::string * p_local_name_out_in )
{
    *p_local_name_out_in = r_name_in;
    std::string::size_type colon_pos = p_local_name_out_in->find( ':' );
    if( colon_pos != std::string::npos )
        p_local_name_out_in->erase( 0, colon_pos + 1 );
    return *p_local_name_out_in;
}

std::string & c_xml::get_namespace_prefix( const std::string & r_name_in, std::string * p_namespace_prefix_out_in )
{
    std::string::size_type colon_pos = r_name_in.find( ':' );

    if( colon_pos != std::string::npos )
    {
        p_namespace_prefix_out_in->erase();   // For VC6 bug
        *p_namespace_prefix_out_in = r_name_in;
        p_namespace_prefix_out_in->erase( colon_pos );
    }
    else
        *p_namespace_prefix_out_in = "";

    return *p_namespace_prefix_out_in;
}

int c_xml::get_ns_id( const c_xml_local * p_xml_local_in, const std::string & r_name_in ) const
{
    get_namespace_prefix( r_name_in, &m_prefix_for_get_ns_id );

    return p_xml_local_in->find_namespace_id( m_prefix_for_get_ns_id );
}

bool c_xml::is_xsi_attribute( const c_xml_local * p_xml_local_in, const std::string & r_name_in ) const
{
    return get_ns_id( p_xml_local_in, r_name_in ) == EXKN_XSI;
}

elxp_xml_event c_xml::get_element_name( c_xml_local * p_local_control_in, std::string * p_name_out )
{
    // Reads an element name. Tokenisation is done by (Schema aware) calling function.

    elxp_xml_event event = EXE_UNKNOWN;

    for( ;; )
    {
        p_name_out->erase();

        int c;

        c = m_getter.get_non_ws();

        if( c == '<' )
        {
            // A possible start tag
            #if defined( LMX_XML_STRICT ) && LMX_XML_STRICT != 0    // Added in V3.3
            c = m_getter.get();
            #else
            c = m_getter.get_non_ws();
            #endif

            if( is_xml_name_start_char( c ) )
            {
                *p_name_out += static_cast<char>( c );
                c = m_getter.get();

                while( is_xml_name_char( c ) )
                {
                    *p_name_out += static_cast<char>( c );
                    c = m_getter.get();
                }

                if( c == c_read::k_eof )
                {
                    m_getter.error( EET_ERROR, ELMX_NOT_WELL_FORMED, "XML not Well-Formed" );
                    return EXE_BADLY_FORMED;
                }

                m_getter.unget( c );       // Might be '/' or something

                if( ! p_capture_attributes( p_local_control_in ) )
                    return EXE_BADLY_FORMED;

                event = EXE_TOKEN;    // Exit via function end
                break;
            }

            else if( c == '/' )   // End of element
            {
                #if defined( LMX_XML_STRICT ) && LMX_XML_STRICT != 0    // Added in V3.3
                c = m_getter.get();
                #else
                c = m_getter.get_non_ws();
                #endif

                while( is_xml_name_char( c ) )
                {
                    *p_name_out += static_cast<char>( c );
                    c = m_getter.get();
                }

                if( p_name_out->empty() )
                    return EXE_BADLY_FORMED;

                // Move past the closing '>'
                while( xisspace( c ) )
                    c = m_getter.get();

                if( c != '>' )
                {
                    m_getter.error( EET_ERROR, ELMX_NOT_WELL_FORMED, "XML not Well-Formed" );
                    return EXE_BADLY_FORMED;
                }

                event = EXE_ELEMENT_END;  // Exit via function end
                break;
            }

            else if( c == '?' )
            {
                if( ! p_handle_lt_qmark() )
                    return EXE_BADLY_FORMED;

                // else loop round to get next element
            }

            else if( c == '!' )
            {
                if( ! p_handle_lt_pling( p_name_out, &event ) )
                    break;

                // else loop round to get next element
            }

            else
            {
                m_getter.error( EET_ERROR, ELMX_NOT_WELL_FORMED, "XML not Well-Formed" );

                m_getter.move_to_start_tag_end();   // For user compilation

                return EXE_BADLY_FORMED;
            }
        }       // End of if( l_c == '<' )

        else if( c == c_read::k_eof )
        {
            event = EXE_EOM;  // Exit via function end
            break;
        }

        else if( c == '&' )
        {
            c_entity_name_reader_for_c_get entity_reader( m_getter );
            entity_reader.is_markup_possible_in_entity( true );

            if( ! p_select_entity( entity_reader, p_name_out ) )
                return EXE_BADLY_FORMED;

            if( ! p_name_out->empty() )
            {
                event = EXE_CDATA;
                if( ! p_get_xml_text( p_name_out ) )
                    event = EXE_BADLY_FORMED;
                break;
            }
        }

        else
        {
            // Mixed content - Read CDATA
            event = EXE_CDATA;
            *p_name_out += static_cast<char>( c );
            if( ! p_get_xml_text( p_name_out ) )
                event = EXE_BADLY_FORMED;
            break;
        }
    }

    m_element_read = true;

    assert( event != EXE_UNKNOWN );

    return event;
}

void c_xml::add_general_entity( const std::string & r_name_in, const std::string & r_value_in )
{
    m_getter.add_general_entity( r_name_in.c_str(), r_value_in.c_str() );
}

void c_xml::add_parameter_entity( const std::string & r_name_in, const std::string & r_value_in )
{
    m_getter.add_parameter_entity( r_name_in.c_str(), r_value_in.c_str() );
}

bool c_xml::skip_rest_of_element( const std::string & r_name_in )
{
    // Skips remainder of start tag, element body (if any) and
    // element end tag.

    bool end_of_element;

    if( ! skip_start_tag( &end_of_element ) )
        return false;

    if( ! end_of_element  )
         return skip_element_body( r_name_in );

    return true;
}

bool c_xml::skip_rest_of_empty_element( const std::string & r_name_in )
{
    // Skips remainder of start tag.  If it's not a empty element <empty/>
    // then check that it is immediately followed by an end tag <empty></empty>.
    // This is an XML spec item.

    bool end_of_element;

    if( ! skip_start_tag( &end_of_element ) )
        return false;

    if( ! end_of_element && ! is_element_end_tag( r_name_in ) )
        return false;

    return true;
}

// Skips remainder of element body
bool c_xml::skip_element_body( const std::string & r_name_in )
{
    std::string name;
    c_xml_local local_control;
    elxp_xml_event event;

    for( ;; )
    {
        event = get_element_name( &local_control, &name );

        if( event == EXE_CDATA )
            {}  // Simply go round again

        else if( event == EXE_TOKEN )
        {
            if( ! skip_rest_of_element( name ) )
                return false;
        }

        else if( event == EXE_ELEMENT_END )
        {
            if( r_name_in != name )
            {
                m_getter.error( EET_ERROR, ELMX_NOT_WELL_FORMED,
                            std::string( "Element start tag (" ) + r_name_in + ") "
                                            "and end tag (" + name + ") do not match" );
                return false;
            }

            break;  // v4.1 - To keep HP-UX aCC happy (formerly: return true;)
        }

        else  // For other reasons (e.g. EXE_EOM, EXE_BADLY_FORMED etc)
            return false;
    }

    return true;
}

bool c_xml::get_element_value( std::string * p_value_out,
                                    elmx_whitespace ws_mode_in /*= EXWS_PRESERVE*/,
                                    bool expand_entities_in /*=true*/ )
{
    p_value_out->erase();      // Allow reuse of buffer

    m_getter.set_ws_mode( ws_mode_in );

    int c;

    for( ;; )
    {
        // Accumulate to start of tag
        while( (c = m_getter.get()) != c_read::k_eof && c != '<' )
        {
            if( c == '&' && expand_entities_in )
            {
                c_entity_name_reader_for_c_get entity_reader( m_getter );
                entity_reader.is_markup_possible_in_entity( true );

                if( ! p_select_entity( entity_reader, p_value_out ) )
                    return false;
            }
            else
                m_getter.add_char_wsp( p_value_out, c );
        }

        if( c == c_read::k_eof )
        {
            m_getter.error( EET_ERROR, ELMX_UNEXPECTED_EOM, "Premature end of file encountered" );
            return false;
        }

        // Could be end of element, but may be start of CDSect, comment or PI

        c = m_getter.get();
        if( c == '!' )
        {
            c = m_getter.get();
            if( c == '-' && m_getter.get() == '-' )
                m_getter.skip_comment();   // Go round again...

            else if( c == '[' )
            {
                if( ! p_read_cdata( p_value_out ) )
                {
                    m_getter.error( EET_ERROR, ELMX_NOT_WELL_FORMED, "XML not well-formed" );
                    return false;
                }
            }

            else
            {
                m_getter.error( EET_ERROR, ELMX_NOT_WELL_FORMED, "XML not well-formed" );
                return false;
            }
        }

        else if( c == '?' )
            m_getter.skip_PI();

        else
        {
            m_getter.unget( c );    // Double-unget
            m_getter.unget( '<' );

            break;
        }
    }

    return true;
}

bool c_xml::is_element_end_tag( const std::string & r_name_in )
{
    int c = m_getter.get_non_ws();
    if( c != '<' )
        return false;

    #if defined( LMX_XML_STRICT ) && LMX_XML_STRICT != 0    // Added in V3.3
    c = m_getter.get();
    #else
    c = m_getter.get_non_ws();
    #endif

    if( c != '/' )
        return false;

    #if defined( LMX_XML_STRICT ) && LMX_XML_STRICT != 0    // Added in V3.3
    c = m_getter.get();
    #else
    c = m_getter.get_non_ws();
    #endif

    for( size_t i=0; i<r_name_in.size(); ++i )
    {
        if( static_cast<unsigned char>(r_name_in[i]) != c )   // Avoid sign extension issues
            return false;
        c = m_getter.get();
    }

    if( xisspace( c ) )
        c = m_getter.get_non_ws();

    if( c != '>' )
        return false;

    return true;
}

//------------------------------------------------------------------------------
//                   Functions for handling 'any' content
//------------------------------------------------------------------------------

// Returns true if element has a body, false otherwise

void c_xml::p_add_any_attribute_data( std::string * p_value_out, c_attribute_data & r_attribute_data_in )
{
    // See attribute_data_note in lmxparse.h for information stored in attribute_data element.

    if( r_attribute_data_in.empty() )
        return;

    while( r_attribute_data_in.peek() != c_read::k_eof )
    {
        *p_value_out += ' ';
        // Transfer the attribute name
        for( ; r_attribute_data_in.peek() != c_read::k_eof; r_attribute_data_in.next() )
            *p_value_out += static_cast<char>( r_attribute_data_in.peek() );

        r_attribute_data_in.next_segment();

        *p_value_out += "=";

        char attribute_quote_char = static_cast<char>( r_attribute_data_in.peek() );
        r_attribute_data_in.next();

        *p_value_out += attribute_quote_char;

        // Transfer the attribute value
        for( ; r_attribute_data_in.peek() != c_read::k_eof; r_attribute_data_in.next() )
            *p_value_out += static_cast<char>( r_attribute_data_in.peek() );

        *p_value_out += attribute_quote_char;

        r_attribute_data_in.next_segment();
    }

    r_attribute_data_in.clear();
}

bool c_xml::p_add_any_attributes( std::string * p_value_out )
{
    // Depending on the context in which this function is called, there may or may not
    // be captured attribute data.  Hence this function extracts both captured (using
    // attribute_data etc.) and non captured attributes.

    p_add_any_attribute_data( p_value_out, m_namespace_attribute_data );

    p_add_any_attribute_data( p_value_out, attribute_data );

    int c;
    bool is_element_with_body = true;
    int attr_value_end = '\0';

    while( (c = m_getter.get()) != c_read::k_eof &&     // v4.2 - gcc_433_brackets
                    ((c != '>' && c != '/') || attr_value_end != '\0' ) )
    {
        *p_value_out += static_cast<char>( c );

        if( attr_value_end == c )
            attr_value_end = '\0';

        else if( attr_value_end == '\0' )
        {
            if( c == '\'' || c == '"' )
                attr_value_end = c;
        }
    }


    if( c == '>' )
        is_element_with_body = true;

    else if( c == '/' )
    {
        *p_value_out += '/';
        while( (c = m_getter.get()) != c_read::k_eof && c != '>' )
            {}

        is_element_with_body = false;
    }

    *p_value_out += '>';

    return is_element_with_body;
}

c_xml::e_tag_type c_xml::p_get_any_tag_data( std::string * p_value_out )
{
    p_value_out->erase();

    int c;

    while( (c = m_getter.get()) != lmx::c_read::k_eof && xisspace( c ) )
        m_getter.add_char_wsp( p_value_out, c );

    if( c == lmx::c_read::k_eof )
        return ETM_ERROR;

    else if( c == '<' )
    {
        p_value_out->erase();

        #if defined( LMX_XML_STRICT ) && LMX_XML_STRICT != 0    // Added in V3.3
        c = m_getter.get();
        #else
        c = m_getter.get_non_ws();
        #endif

        e_tag_type result = ETM_START;

        if( c == lmx::c_read::k_eof )
            return ETM_ERROR;

        else if( c == '!' )
        {
            c = m_getter.get();
            if( c == '[' )
            {
                // Assume its a CDSect
                std::string cdata;
                if( ! p_read_cdata( &cdata ) )  // C140521_1259 - Checked return code
                    return ETM_ERROR;

                *p_value_out += "<![CDATA[";
                *p_value_out += cdata;
                *p_value_out += "]]>";
            }

            else if( c == '-' && m_getter.get() == '-' )
                m_getter.skip_comment();

            return ETM_BODY;
        }

        else if( c == '?' )
        {
            m_getter.skip_PI();

            return ETM_BODY;
        }

        else
        {
            #if defined( LMX_XML_STRICT ) && LMX_XML_STRICT != 0    // Added in V3.3
            if( xisspace( c ) )
                return ETM_ERROR;
            #endif

            if( c == '/' )
            {
                result = ETM_END;
                #if defined( LMX_XML_STRICT ) && LMX_XML_STRICT != 0    // Added in V3.3
                c = m_getter.get();
                #else
                c = m_getter.get_non_ws();
                #endif
            }

            #if defined( LMX_XML_STRICT ) && LMX_XML_STRICT != 0    // Added in V3.3
            if( xisspace( c ) )
                return ETM_ERROR;
            #endif

            *p_value_out += static_cast<char>( c );

            while( (c = m_getter.get()) != lmx::c_read::k_eof &&
                    ! xisspace( c ) &&
                    c != '>' &&
                    c != '/' )
                m_getter.add_char_wsp( p_value_out, c );

            if( result == ETM_END && c != '>')  // Consume up to '>'
            {
                c = m_getter.get_non_ws();

                if( c != '>' )
                    return ETM_ERROR;
            }

            if( c == lmx::c_read::k_eof )
                return ETM_ERROR;

            if( result == ETM_START )
                m_getter.unget( c );

            return result;
        }
    }

    else
    {
        m_getter.add_char_wsp( p_value_out, c );
        while( (c = m_getter.get()) != lmx::c_read::k_eof && c != '<' )
            m_getter.add_char_wsp( p_value_out, c );

        m_getter.unget( c );

        return ETM_BODY;
    }

    lmx_assert_code_not_reached();  // Should not get here

    return ETM_END;
}

bool c_xml::p_get_any_element( const std::string & r_name_in, std::string * p_value_out )
{
    *p_value_out += "<";
    *p_value_out += r_name_in;

    if( p_add_any_attributes( p_value_out ) )
    {
        for(;;)
        {
            std::string tag_data;
            e_tag_type tag_type = p_get_any_tag_data( &tag_data );

            if( tag_type == ETM_BODY )
            {
                *p_value_out += tag_data;
                continue;
            }

            else if( tag_type == ETM_START )
            {
                if( ! p_get_any_element( tag_data, p_value_out ) )
                    return false;
            }

            else if( tag_type == ETM_END )
            {
                if( tag_data != r_name_in )
                    return false;
                *p_value_out += "</";
                *p_value_out += r_name_in;
                *p_value_out += ">";
                break;
            }

            else if( tag_type == ETM_ERROR )
                return false;
        }
    }

    return true;
}

// Reads nested elements and checks that final end tag name matches ar_name

bool c_xml::get_any_element( const std::string & r_name_in, std::string * p_value_out,
                                        elmx_whitespace ws_mode_in /*= EXWS_PRESERVE*/,
                                        bool LMX_UNUSED_PARAM(expand_entities_in) /*=false*/ )
{
    p_value_out->erase();      // Allow reuse of buffer
    m_getter.set_ws_mode( ws_mode_in );

    return p_get_any_element( r_name_in, p_value_out );
}

bool c_xml::get_nested_element_value( std::string * p_value_out, std::string * p_end_tag_name_out,
                                        elmx_whitespace ws_mode_in /*= EXWS_PRESERVE*/,
                                        bool LMX_UNUSED_PARAM(expand_entities_in) /*=false*/ )
{
    p_value_out->erase();      // Allow reuse of buffer
    p_end_tag_name_out->erase();

    m_getter.set_ws_mode( ws_mode_in );

    // Clear any stored attributes from previous element
    attribute_data.clear();
    m_namespace_attribute_data.clear();

    for(;;)
    {
        std::string tag_data;
        e_tag_type tag_type = p_get_any_tag_data( &tag_data );

        if( tag_type == ETM_BODY )
        {
            *p_value_out += tag_data;
            continue;
        }

        else if( tag_type == ETM_START )
        {
            if( ! p_get_any_element( tag_data, p_value_out ) )
                return false;
        }

        else if( tag_type == ETM_END )
        {
            *p_end_tag_name_out = tag_data;
            break;
        }

        else if( tag_type == ETM_ERROR )
            return false;
    }

    return true;
}

//------------------------------------------------------------------------------
//                   Functions for handling attributes
//------------------------------------------------------------------------------

elxp_xml_event c_xml::get_attribute_name( std::string * p_name_out, bool in_xml_decl_in /*=false*/ )
{
    // Reads an attribute name. Tokenisation is done by (Schema aware) calling function.

    p_name_out->erase();       // Allow reuse of buffer

    if( ! attribute_data.is_eof() )
    {
        *p_name_out = attribute_data.get_str();

        attribute_data.next_segment();      // Move to the start of the value part

        return EXE_TOKEN;
    }

    else
    {
        int c;

        while( (c = m_getter.get()) != c_read::k_eof && xisspace( c ) )
            {}

        if( c == c_read::k_eof )
        {
            m_getter.error( EET_ERROR, ELMX_NOT_WELL_FORMED, "XML not well-formed" );

            return EXE_BADLY_FORMED;
        }


        // Check end of empty element start tag (and xml decl)

        if( (c == '/' && ! in_xml_decl_in) || (c == '?' && in_xml_decl_in) )  // v4.2 - gcc_433_brackets
        {
            while( (c = m_getter.get()) != c_read::k_eof && xisspace( c ) )
                {}

            if( c == '>' )
                return EXE_ELEMENT_END;

            m_getter.error( EET_ERROR, ELMX_NOT_WELL_FORMED, "XML not well-formed" );

            return EXE_BADLY_FORMED;
        }

        else if( c == '>' )
            return EXE_START_TAG_END;

        else
        {
            m_getter.error( EET_ERROR, ELMX_NOT_WELL_FORMED, "XML not well-formed" );

            return EXE_BADLY_FORMED;
        }
    }
}

bool c_xml::get_attribute_value( std::string * p_value_out,
                                    elmx_whitespace ws_mode_in /*= EXWS_PRESERVE*/ )
{
    p_value_out->erase();      // Allow reuse of buffer

    m_getter.set_ws_mode( ws_mode_in );

    attribute_data.next();      // Skip over the ' or ".  We don't need it in this case.

    c_entity_name_reader_for_attribute_data  entity_reader( attribute_data );

    while( attribute_data.peek() != c_read::k_eof )
    {
        if( attribute_data.peek() == '&' )
        {
            attribute_data.next();
            if( ! p_select_entity( entity_reader, p_value_out ) )
                return false;
        }
        else
            m_getter.add_char_wsp( p_value_out, attribute_data.get() );
    }

    attribute_data.next_segment();      // Move to the start of the next name part

    return true;
}

bool c_xml::skip_attribute_value()
{
    attribute_data.next_segment();      // Move to the start of the next name part

    return true;
}

bool c_xml::get_namespace_attribute_name( std::string * p_name_out )
{
    // Reads an attribute name. Tokenisation is done by (Schema aware) calling function.

    p_name_out->erase();       // Allow reuse of buffer

    if( ! m_namespace_attribute_data.is_eof() )
    {
        *p_name_out = m_namespace_attribute_data.get_str();

        m_namespace_attribute_data.next_segment();      // Move to the start of the value part

        return true;
    }

    return false;   // Signals end of namespace attribute data
}

bool c_xml::get_namespace_attribute_value( std::string * p_value_out )
{
    p_value_out->erase();      // Allow reuse of buffer

    m_getter.set_ws_mode( EXWS_COLLAPSE );

    m_namespace_attribute_data.next();      // Skip over the ' or ".  We don't need it in this case.

    c_entity_name_reader_for_attribute_data  entity_reader( m_namespace_attribute_data );

    while( m_namespace_attribute_data.peek() != c_read::k_eof )
    {
        if( m_namespace_attribute_data.peek() == '&' )
        {
            m_namespace_attribute_data.next();
            if( ! p_select_entity( entity_reader, p_value_out ) )
                return false;
        }
        else
            m_getter.add_char_wsp( p_value_out, m_namespace_attribute_data.get() );
    }

    m_namespace_attribute_data.next_segment();      // Move to the start of the next name part

    return true;
}

bool c_xml::skip_namespace_attribute_value()
{
    m_namespace_attribute_data.next_segment();      // Move to the start of the next name part

    return true;
}

bool c_xml::get_named_attribute(
        const c_xml_local * p_xml_local_in,
        int ns_id_in,
        const std::string & r_local_name_in,
        std::string * p_value_out )
{
    bool is_attribute_found = false;

    while( ! attribute_data.is_eof() )
    {
        p_value_out->erase();
        *p_value_out = attribute_data.get_str();

        if( ns_id_in == EXKN_NONE )  // V4.1 - Named_attr_fix
        {
            if( p_value_out->find( ':' ) == std::string::npos && *p_value_out == r_local_name_in )
                is_attribute_found = true;
        }

        else
        {
            if( get_ns_id( p_xml_local_in, *p_value_out ) == ns_id_in && get_local_name( *p_value_out, p_value_out ) == r_local_name_in )
                is_attribute_found = true;
        }

        if( is_attribute_found )
        {
            attribute_data.next_segment();      // Move to value
            get_attribute_value( p_value_out );    // This will also do entity expansion and move to next name
                // Ignore get_attribute_value() return code.  We want to know if attribute is found, not if value is OK.

            attribute_data.rewind();

            return true;
        }

        attribute_data.next_segment();  // Move to value
        attribute_data.next_segment();  // Move to next name
    }

    attribute_data.rewind();

    return false;
}

// Note: get_xsi_type_is_non_const - While conceptually get_xsi_type() looks like it should
// be const, internally it does change the state of the object (which we then fix up before
// exiting).  We could make things mutable and const_cast appropriately, but it really only
// makes sense to call the function in a non-const context (during parsing) so it might as
// well advertise that it can change things.
bool c_xml::get_xsi_type( const c_xml_local * p_xml_local_in, int * p_type_ns_id_in, std::string * p_type_local_name_in )
{
    if( get_named_attribute( p_xml_local_in, EXKN_XSI, "type", p_type_local_name_in ) )
    {
        *p_type_ns_id_in = get_ns_id( p_xml_local_in, *p_type_local_name_in );
        get_local_name( *p_type_local_name_in, p_type_local_name_in );

        return true;
    }

    return false;
}

//------------------------------------------------------------------------------
//                  Reader and writer classes common components
//------------------------------------------------------------------------------

s_debug_error::s_debug_error()
{
    clear();
}

void s_debug_error::clear()
{
    error = ELMX_OK;
    item_name = "";
    p_prog_file = "(not set)";
    prog_line = -1;
    xml_line = -1;
}

void s_debug_error::set( elmx_error error_in, const std::string & r_item_name_in,
                const char * p_prog_file_in, int prog_line_in, int xml_line_in /*=-1*/ )
{
    error = error_in;
    item_name.erase();  // VC6 Bug
    item_name = r_item_name_in;
    p_prog_file = p_prog_file_in;
    prog_line = prog_line_in;
    xml_line = xml_line_in;
}

const char * s_debug_error::get_description() const
{
    return get_error_description( error );
}

const char * s_debug_error::get_description( const s_custom_error_description * p_custom_error_description ) const
{
    if( p_custom_error_description )
    {
        for( size_t i=0; p_custom_error_description[i].p_description; ++i )
        {
            if( p_custom_error_description[i].code == error )
                return p_custom_error_description[i].p_description;
        }
    }

    return get_error_description( error );
}

std::string s_debug_error::to_string() const
{
    return to_string( LMXNULL );
}

std::string s_debug_error::to_string( const s_custom_error_description * p_custom_error_description ) const
{
    std::ostringstream result;
    to_stream( result, p_custom_error_description );
    return result.str();
}

std::ostream & s_debug_error::to_stream( std::ostream & r_os_in ) const
{
    return to_stream( r_os_in, LMXNULL );
}

std::ostream & s_debug_error::to_stream( std::ostream & r_os_in, const s_custom_error_description * p_custom_error_description ) const
{
    r_os_in << "Error: " << get_description( p_custom_error_description ) << "\n";

    if( error == ELMX_OK )
        return r_os_in;

    r_os_in << "Item: " << item_name << "\n" <<
                "Program file: " << p_prog_file << "\n" <<
                "Program line: " << prog_line << "\n";
    if( xml_line != -1 )
        r_os_in << "XML line: " << xml_line << "\n";
    return r_os_in;
}

std::ostream & operator << ( std::ostream & r_os_in, const s_debug_error & r_debug_error_in )
{
    return r_debug_error_in.to_stream( r_os_in );
}

#if defined( LMX_DEBUG_CAPTURE_ERROR ) && LMX_DEBUG_CAPTURE_ERROR == 1
    s_debug_error debug_error;
#endif

//------------------------------------------------------------------------------
//                              Reader classes
//------------------------------------------------------------------------------

c_xml_reader_local::c_xml_reader_local()
    :
    c_xml_local(),
    mp_reader( LMXNULL ),
    mp_prev( LMXNULL )
{
}

c_xml_reader_local::c_xml_reader_local( c_xml_reader * p_reader_in )
    :
    c_xml_local( *(p_reader_in->get_reader_local()) ),
    mp_reader( p_reader_in ),
    mp_prev( p_reader_in->get_reader_local() )
{
    mp_reader->set_reader_local( this );
}

void c_xml_reader_local::set_reader( c_xml_reader * p_reader_in )
{
    mp_reader = p_reader_in;
}

c_xml_reader_local::~c_xml_reader_local()
{
    if( mp_reader )  // C101103_0904: Can be NULL if bad_alloc occurs before object fully configured
        mp_reader->set_reader_local( mp_prev );
}

void c_cracked_name::set( c_xml_local * p_reader_local_in, const std::string & r_name_in )
{
    m_full_name.erase();  // For VC6
    m_full_name.append( r_name_in );    // Use append to avoid VC6 releasing already allocated memory

    m_ns_prefix.erase();
    m_local_name.erase();

    std::string::size_type colon_pos = m_full_name.find( ':' );

    if( colon_pos != std::string::npos )
    {
        m_local_name.assign( m_full_name, colon_pos+1, std::string::npos );

        m_ns_prefix.assign( m_full_name, 0, colon_pos );

        m_ns_id = p_reader_local_in->find_namespace_id( m_ns_prefix );

        m_has_ns_prefix = true;
    }

    else
    {
        m_local_name.append( m_full_name ); // Use append to avoid VC6 releasing already allocated memory

        m_ns_id = p_reader_local_in->find_namespace_id( "" );   // Could be default namespace

        m_has_ns_prefix = false;
    }
}

void c_cracked_name::set_for_attribute( c_xml_local * p_reader_local_in, const std::string & r_name_in )  // v4.1
{
    set( p_reader_local_in, r_name_in );

    if( ! m_has_ns_prefix )   // If has no namespace prefix, ignore any default namespace mapping
        m_ns_id = EXKN_NONE;
}

int c_cracked_name::get_element_ns_id() const   // V3.7.5
{
    if( m_ns_id == EXKN_UNKNOWN && ! m_has_ns_prefix )
        return EXKN_NONE;
    return m_ns_id;
}

c_known_elements_selector::c_known_elements_selector( // V3.7.5
        c_xml_reader & r_xml_reader_in,
        const s_known_elements * p_known_elements_in )
    :
    mr_xml_reader( r_xml_reader_in ),
    mp_old_known_elements( r_xml_reader_in.get_known_elements() )
{
    mr_xml_reader.set_known_elements( p_known_elements_in );
}

c_known_elements_selector::~c_known_elements_selector() // V3.7.5
{
    mr_xml_reader.set_known_elements( mp_old_known_elements );
}

//------------------------------------------------------------------------------
//                              validation_spec_empty
//------------------------------------------------------------------------------

elmx_error lexical_validator_empty( c_xml_reader & r_reader_in, const std::string & r_v_in )
{
    LMX_NO_WARN_UNUSED( r_reader_in );
    elmx_error error = ELMX_OK;       // C110316_1024 - Error now reported to error handler
    if( ! r_v_in.empty() && (error = r_reader_in.handle_error( ELMX_VALUE_BAD_FORMAT )) != ELMX_OK )
        return error;
    return ELMX_OK;
}

elmx_error value_validator_empty( c_xml_reader & r_reader_in, const c_empty & r_v_in )
{
    LMX_NO_WARN_UNUSED( r_reader_in );
    LMX_NO_WARN_UNUSED( r_v_in );
    return ELMX_OK;
}

const ct_typed_validation_spec< c_empty > validation_spec_empty(
                EXWS_PRESERVE,                      // Whitespace handling
                "",                                 // Default value (if any)
                &lexical_validator_empty,           // Lexical validator function
                &value_validator_empty,             // Value validator function
                &lmx::v_to_o_not_used< c_empty >,   // Lexical to object converter
                &lmx::o_to_v_not_used< c_empty > ); // Object to lexical converter

//------------------------------------------------------------------------------
//                              c_xml_reader
//------------------------------------------------------------------------------

void c_xml_reader::add_namespace_mapping( const std::string & r_ns_prefix_in, const std::string & r_namespace_in )
{
    m_user_namespaces.push_back( s_user_ns_prefix_map( r_ns_prefix_in, r_namespace_in ) );
}

void c_xml_reader::add_namespace_mappings( const c_namespace_context & r_namespace_context_in )
{
    for( size_t i=0; i<r_namespace_context_in.size(); ++i )
        add_namespace_mapping( r_namespace_context_in.get_ns_prefix( i ), r_namespace_context_in.get_ns_uri( i ) );
}

void c_xml_reader::add_namespace_mappings( const c_any_info & r_any_info_in )
{
    add_namespace_mappings( r_any_info_in.get_namespace_context() );
}

std::string & c_xml_reader::get_namespace( const std::string & r_name_in, std::string * p_namespace_out_in ) const
{
    // Rather than create a temporary string, use the output string to temporarily store the prefix
    get_namespace_prefix( r_name_in, p_namespace_out_in );

    int namespace_id = mp_reader_local->find_namespace_id( *p_namespace_out_in );

    p_namespace_out_in->erase();
    if( namespace_id == EXKN_NONE )   // 3.6.1
        *p_namespace_out_in = k_non_namespace;
    else if( namespace_id == EXKN_UNKNOWN )
        *p_namespace_out_in = k_namespace_not_found;
    else
        *p_namespace_out_in = find_namespace_from_id( namespace_id );

    return *p_namespace_out_in;
}

elmx_error c_xml_reader::capture_error(
                                elmx_error error_in,
                                const std::string & r_item_name_in,
                                const char * p_prog_file_in,
                                int prog_line_in )
{
    if( error_in == ELMX_UNEXPECTED_EOM && m_error.is_fatal_errored() )    // Added v4.2
        error_in = m_error.get_last_error();

    m_error_info.set( error_in, r_item_name_in, p_prog_file_in, prog_line_in, get_line_no() );  // C100806_1136

    return error_in;
}

std::string & c_xml_reader::get_error_message( std::string * p_message_in ) const
{
    *p_message_in = m_error_info.to_string();   // C100806_1136

    return *p_message_in;
}

std::string c_xml_reader::get_error_message() const
{
    std::string error_message;
    return get_error_message( & error_message );
}

void c_xml_reader::set_ns_map( const s_ns_map * p_ns_map_in )
{
    for( ; p_ns_map_in->p_ns; ++p_ns_map_in )
        add_namespace( p_ns_map_in->p_ns, p_ns_map_in->id );

    t_user_namespaces::const_iterator i_user_namespace( m_user_namespaces.begin() );
    t_user_namespaces::const_iterator i_user_namespace_end( m_user_namespaces.end() );

    while( i_user_namespace != i_user_namespace_end )
    {
        int new_namespace_id = add_namespace( i_user_namespace->ns_uri.c_str() );

        m_bottom_reader_local.add_namespace( i_user_namespace->m_ns_prefix.c_str(), new_namespace_id );

        ++i_user_namespace;
    }
}

void c_xml_reader::tokenise( const s_event_map c_event_map_in[], elxp_attr_elem type_in )
{
    if( ! is_current_event_tokenisable() )
        return;

    int enumerated_namespace = EXKN_UNKNOWN;

    if( m_cracked_name.get_has_ns_prefix() )
        enumerated_namespace = m_cracked_name.get_ns_id();
    else
    {
        if( type_in == EXNT_ATTR )
            enumerated_namespace = EXKN_NONE;

        else    // Element case - default namespace can override 'None'
            enumerated_namespace = m_cracked_name.get_ns_id();
    }

    const std::string & r_local_name = m_cracked_name.get_local_name();

    if( enumerated_namespace == EXKN_XSI )
    {
        if( r_local_name[0] == 't' && r_local_name == "type" )
            m_event = EXE_XSI_TYPE;

        else if( r_local_name[0] == 'n' && r_local_name == "nil" )
            m_event = EXE_XSI_NIL;

        else if( r_local_name[0] == 'n' && r_local_name == "noNamespaceSchemaLocation" )
            m_event = EXE_XSI_NONAMESPACESCHEMALOCATION;

        else if( r_local_name[0] == 's' && r_local_name == "schemaLocation" )
            m_event = EXE_XSI_SCHEMALOCATION;

        else
            m_event = EXE_UNKNOWN;
        return;
    }

    else
    {
        // Look through token table for relevant event
        for( int i=0; c_event_map_in[i].p_name; ++i )
        {
            if( enumerated_namespace == c_event_map_in[i].known_namespace &&
                    r_local_name[0] == c_event_map_in[i].p_name[0] &&
                    r_local_name.size() == c_event_map_in[i].size &&
                    stringcompare(
                            r_local_name,
                            c_event_map_in[i].p_name,
                            c_event_map_in[i].size ) == 0 )
            {
                m_event = c_event_map_in[i].m_event;
                return;
            }
        }
    }

    m_event = EXE_UNKNOWN;
}

bool c_xml_reader::get_attribute_event( const s_event_map c_event_map_in[], elmx_error * p_error_out,
                                                    std::string * p_name_out )
{
    *p_error_out = ELMX_OK;

    m_event = get_attribute_name( p_name_out );

    if( is_error_event( m_event ) )
    {
        *p_error_out = get_error();    // Option to override error is in generated code so that
        return true;                // -breakpoint stack traces don't go into unknown DLL code.
    }

    else if( m_event == EXE_EOM )
    {
        *p_error_out = ELMX_UNEXPECTED_EOM;
        return true;
    }

    else if( m_event == EXE_TOKEN )
    {
        m_cracked_name.set_for_attribute( mp_reader_local, *p_name_out );

        if( c_event_map_in )
            tokenise( c_event_map_in, EXNT_ATTR );

        // We can't read the attribute value here as we don't yet know the white space normalisation to be applied

        return true;
    }

    return false;
}

void c_xml_reader::get_element_event( elmx_error * p_error_out, std::string * p_name_out )  // V4.3
{
    *p_error_out = ELMX_OK;    // v5

    bool is_unknown_element_found = false;  // v5.8 - C110707_1155

    for( ;; )
    {
        mp_reader_local->clear();

        m_event = get_element_name( mp_reader_local, p_name_out );

        if( m_event == EXE_TOKEN )
            m_cracked_name.set( mp_reader_local, *p_name_out );

        else
        {
            if( is_error_event( m_event ) )
                *p_error_out = get_error();    // Option to override error is in generated code so that
                                            // -breakpoint stack traces don't go into unknown DLL code.

            else if( m_event == EXE_EOM )
                *p_error_out = ELMX_UNEXPECTED_EOM;

            else if( m_event == EXE_ELEMENT_END && is_unknown_element_found )
                m_event = EXE_HAS_SKIPPED_UNKNOWN_ELEMENTS;

            return;
        }

        if( ! mp_known_elements )
            return;

        int enumerated_namespace = m_cracked_name.get_element_ns_id();
        const std::string & r_local_name = m_cracked_name.get_local_name();

        for( const s_known_elements * p_known_elements = mp_known_elements;
                p_known_elements->p_local_name != LMXNULL;
                ++p_known_elements )
        {
            if( enumerated_namespace == p_known_elements->known_namespace &&
                    r_local_name[0] == p_known_elements->p_local_name[0] &&
                    r_local_name.size() == p_known_elements->local_name_size &&
                    stringcompare(
                            r_local_name,
                            p_known_elements->p_local_name,
                            p_known_elements->local_name_size ) == 0 )
            {
                return;
            }
        }

        is_unknown_element_found = true;  // v5.8 - C110707_1155

        // V7.1 - C150417_1659
        if( (*p_error_out = found_unknown_element( *p_name_out, m_code_location.mp_file, m_code_location.m_line )) != ELMX_OK )
            return;

        if( ! skip_rest_of_element( *p_name_out ) )
        {
            *p_error_out = ELMX_BADLY_FORMED;
            return;
        }
    }
}

void c_xml_reader::get_element_event( const s_event_map c_event_map_in[], elmx_error * p_error_out,
                                                std::string * p_name_out )  // Refactored in V4.3 // C110707_1155
{
    get_element_event( p_error_out, p_name_out );

    if( m_event == EXE_TOKEN && c_event_map_in )
        tokenise( c_event_map_in, EXNT_ELEM );
}

void c_xml_reader::get_known_element_event( elmx_error * p_error_out, std::string * p_name_out )  // C110707_1155
{
    lmx_assert( mp_known_elements );

    get_element_event( p_error_out, p_name_out ); // Now identical to get_element_event().  Presence of p_known_elements indicates if looking for known events
}

void c_xml_reader::get_known_element_event( const s_event_map c_event_map_in[], elmx_error * p_error_out,
                                                std::string * p_name_out )  // V3.7.5  // Refactored in V4.3 // C110707_1155
{
    lmx_assert( mp_known_elements );

    get_element_event( c_event_map_in, p_error_out, p_name_out );   // Now identical to get_element_event().  Presence of p_known_elements indicates if looking for known events
}

bool c_xml_reader::skip_start_tag( bool * p_is_end_of_element_in /*= LMXNULL*/ )   // V5
{
    bool is_end_of_element;
    bool result = c_xml::skip_start_tag( &is_end_of_element );
    if( is_end_of_element )
        m_event = EXE_ELEMENT_END;
    else
        m_event = EXE_START_TAG_END;
    if( p_is_end_of_element_in )
        *p_is_end_of_element_in = is_end_of_element;
    return result;
}

bool c_xml_reader::get_element_value( std::string * p_value_out, elmx_whitespace ws_mode_in /*= EXWS_PRESERVE*/ )    // V5
{
    if( m_event == EXE_ELEMENT_END )
    {
        p_value_out->erase();
        return true;
    }
    bool result = c_xml::get_element_value( p_value_out, ws_mode_in );
    m_event = EXE_CDATA;
    return result;
}

bool c_xml_reader::get_nested_element_value( std::string * p_value_out, std::string * p_end_tag_name_out,
                                elmx_whitespace ws_mode_in/* = EXWS_PRESERVE*/,
                                bool expand_entities_in /*= false*/ )
{
    p_value_out->erase();      // Allow reuse of buffer
    if( m_event == EXE_ELEMENT_END )  // No value if already at element end
        return true;
    if( c_xml::get_nested_element_value( p_value_out, p_end_tag_name_out, ws_mode_in, expand_entities_in ) )
    {
        m_event = EXE_ELEMENT_END;
        return true;
    }

    return false;
}

bool c_xml_reader::get_simple_type_value(
                        const std::string & LMX_UNUSED_PARAM(r_name_in),
                        elmx_whitespace ws_mode_in,
                        std::string * p_value_out,
                        elmx_error * p_error_out,
                        bool * p_is_empty_element_out )
{
    // Skip to end of start tag, read value and then read end tag
    p_value_out->erase();      // Allow reuse of buffer
    if( m_event == EXE_ELEMENT_END )  // No value if already at element end
        return true;
    if( ! skip_start_tag( p_is_empty_element_out ) )
    {
        *p_error_out = ELMX_BAD_END_OF_START_TAG;
        return false;
    }

    if( ! *p_is_empty_element_out )
    {
        if( ! get_element_value( p_value_out, ws_mode_in ) )
        {
            *p_error_out = ELMX_UNABLE_TO_READ_ELEMENT_VALUE;
            return false;
        }

        if( ! is_element_end_tag() )
        {
            *p_error_out = ELMX_BAD_ELEMENT_END;
            return false;
        }
    }

    return true;
}

bool c_xml_reader::get_element_ur_value(
                        const std::string & r_name_in,
                        std::string * p_value_out,
                        elmx_error * p_error_out,
                        bool * p_is_empty_element_out )
{
    // Skip to end of start tag, read value and then read end tag
    p_value_out->erase();      // Allow reuse of buffer
    if( m_event == EXE_ELEMENT_END )  // No value if already at element end
        return true;
    if( ! skip_start_tag( p_is_empty_element_out ) )
    {
        *p_error_out = ELMX_BAD_END_OF_START_TAG;
        return false;
    }

    if( ! *p_is_empty_element_out )
    {
        std::string element_end_name;
        if( ! get_nested_element_value( p_value_out, &element_end_name ) )
        {
            *p_error_out = ELMX_UNABLE_TO_READ_ELEMENT_VALUE;
            return false;
        }

        if( element_end_name != r_name_in )
        {
            *p_error_out = ELMX_BAD_ELEMENT_END;
            return false;
        }
    }

    return true;
}

// V3.2.4
bool c_xml_reader::is_valid_qname( const std::string & r_value_in ) const
{
    c_cracked_name cracked_name;
    cracked_name.set( mp_reader_local, r_value_in );

    if( cracked_name.get_ns_id() == EXKN_UNKNOWN )
        return false;

    if( cracked_name.get_has_ns_prefix() )
    {
        if( ! is_xml_name_start_char( cracked_name.get_ns_prefix()[0] ) )
            return false;

        for( size_t i=1; i<cracked_name.get_ns_prefix().size(); ++i )
            if( ! is_xml_NCname_char( cracked_name.get_ns_prefix()[i] ) )
                return false;
    }

    if( cracked_name.get_local_name().empty() )
        return false;

    if( ! is_xml_name_start_char( cracked_name.get_local_name()[0] ) )
        return false;

    for( size_t i=1; i<cracked_name.get_local_name().size(); ++i )
        if( ! is_xml_NCname_char( cracked_name.get_local_name()[i] ) )
            return false;

    return true;
}

elmx_error c_xml_reader::ignore_element( const s_event_map c_event_map_in[] )     // C110317_1715
{
    if( ! skip_rest_of_element( name ) )
        return ELMX_BADLY_FORMED;
    elmx_error error = lmx::ELMX_OK;
    get_element_event( c_event_map_in, &error, &name );
    return error;
}

// Deprecated - For backwards compatibility - these methods are no longer preferred
t_xml_event c_xml_reader::tokenise( const s_event_map c_event_map_in[], const std::string & r_name_in, elxp_attr_elem type_in )
{
    m_cracked_name.set( mp_reader_local, r_name_in );
    tokenise( c_event_map_in, type_in );
    return m_event;
}

t_xml_event c_xml_reader::get_attribute_event( const s_event_map c_event_map_in[], std::string * p_name_out )
{
    elmx_error error;
    get_attribute_event( c_event_map_in, &error, p_name_out );
    return m_event;
}

t_xml_event c_xml_reader::get_element_event( const s_event_map c_event_map_in[], std::string * p_name_out )
{
    elmx_error error;
    get_element_event( c_event_map_in, &error, p_name_out );
    return m_event;
}

bool c_xml_reader::get_end_tag( const s_event_map c_event_map_in[], const std::string & r_name_in,
                                    t_xml_event * p_event_out, elmx_error * p_error_out )
{
    std::string element_end_name;

    *p_event_out = get_element_event( c_event_map_in, &element_end_name );

    if( *p_event_out == EXE_EOM )
    {
        handle_error( ELMX_UNEXPECTED_EOM, r_name_in, __FILE__, __LINE__ );
        *p_error_out = ELMX_UNEXPECTED_EOM;
        return false;
    }

    else if( (*p_event_out != EXE_ELEMENT_END || element_end_name != r_name_in) &&
            (*p_error_out = handle_error( ELMX_BAD_ELEMENT_END, r_name_in, __FILE__, __LINE__ )) != ELMX_OK )
        return false;

    return true;
}

elmx_error c_xml_reader::unmarshal_attribute_value_impl(
                        c_untyped_unmarshal_bridge & r_bridge_in,
                        const c_untyped_validation_spec & r_item_spec_in )
{
    elmx_error error = ELMX_OK;
    if( r_bridge_in.is_already_unmarshalled() && (error = handle_attribute_error( ELMX_ATTRIBUTE_ALREADY_READ )) != ELMX_OK )
        return error;
    if( ! get_attribute_value( &value, r_item_spec_in.get_whitespace_handling() ) &&
             (error = handle_attribute_error( ELMX_UNABLE_TO_READ_ATTRIBUTE_VALUE )) != ELMX_OK )
        return error;
    error = r_item_spec_in.is_lexical_valid( *this, value );
    if( error != ELMX_OK )
        return error;
    return r_bridge_in.convert_store_and_check_value( value );
}

elmx_error c_xml_reader::unmarshal_attribute_list_value_impl(
                        c_untyped_unmarshal_list_bridge & r_bridge_in,
                        const c_untyped_validation_spec & r_item_spec_in,
                        tpf_list_lexical_validator pf_list_lexical_validator )
{
    elmx_error error;
    if( r_bridge_in.is_already_unmarshalled() && (error = handle_attribute_error( ELMX_ATTRIBUTE_ALREADY_READ )) != ELMX_OK )
        return error;
    if( ! get_attribute_value( &value, r_item_spec_in.get_whitespace_handling() ) &&
             (error = handle_attribute_error( ELMX_UNABLE_TO_READ_ATTRIBUTE_VALUE )) != ELMX_OK )
        return error;
    if( (error = (*pf_list_lexical_validator)( *this, value )) != ELMX_OK )
        return error;
    r_bridge_in.start_new_list();
    lmx::c_xml_list_tokeniser list( value );
    std::string value_inst;
    while( list.get( &value_inst ) )
    {
        error = r_item_spec_in.is_lexical_valid( *this, value_inst );
        if( error != ELMX_OK )
            return error;
        error = r_bridge_in.convert_store_and_check_value( value_inst );
        if( error != ELMX_OK )
            return error;
    }
    return ELMX_OK;
}

elmx_error c_xml_reader::unmarshal_child_element_impl(
                        c_untyped_unmarshal_bridge & r_bridge_in,
                        const c_untyped_validation_spec & r_item_spec_in,
                        const s_event_map * p_event_map_in /*= LMXNULL*/ )    // C101015_1032
{
    elmx_error error;
    if( ! skip_start_tag() && (error = handle_error( ELMX_BAD_END_OF_START_TAG )) != ELMX_OK )
        return error;
    error = unmarshal_element_value_impl( r_bridge_in, r_item_spec_in );
    if( error != ELMX_OK )
        return error;
    if( p_event_map_in )
        get_element_event( p_event_map_in, &error, &name );
    return error;
}

elmx_error c_xml_reader::unmarshal_element_value_impl(
                        c_untyped_unmarshal_bridge & r_bridge_in,
                        const c_untyped_validation_spec & r_item_spec_in )
{
    elmx_error error = r_bridge_in.get_element_value();
    if( error != ELMX_OK )
        return error;
    if( ! is_element_end_tag() && (error = handle_error( ELMX_BAD_ELEMENT_END )) != ELMX_OK )
        return error;
    m_event = EXE_ELEMENT_END;
    if( value.empty() )
        value = r_item_spec_in.get_default();
    error = r_item_spec_in.is_lexical_valid( *this, value );
    if( error != ELMX_OK )
        return error;
    return r_bridge_in.convert_store_and_check_value( value );
}

elmx_error c_xml_reader::unmarshal_child_element_list_impl(
                        c_untyped_unmarshal_list_bridge & r_bridge_in,
                        const c_untyped_validation_spec & r_item_spec_in,
                        tpf_list_lexical_validator pf_list_lexical_validator,
                        const s_event_map * p_event_map_in /*= LMXNULL*/ )    // C101015_1032
{
    elmx_error error;
    if( ! skip_start_tag() && (error = handle_error( ELMX_BAD_END_OF_START_TAG )) != ELMX_OK )
        return error;
    error = unmarshal_element_list_value_impl( r_bridge_in, r_item_spec_in, pf_list_lexical_validator );
    if( error != ELMX_OK )
        return error;
    if( p_event_map_in )
        get_element_event( p_event_map_in, &error, &name );
    return error;
}

elmx_error c_xml_reader::unmarshal_element_list_value_impl(
                        c_untyped_unmarshal_list_bridge & r_bridge_in,
                        const c_untyped_validation_spec & r_item_spec_in,
                        tpf_list_lexical_validator pf_list_lexical_validator )
{
    elmx_error error = r_bridge_in.get_element_value();
    if( error != ELMX_OK )
        return error;
    if( ! is_element_end_tag() && (error = handle_error( ELMX_BAD_ELEMENT_END )) != ELMX_OK )
        return error;
    m_event = EXE_ELEMENT_END;
    r_bridge_in.start_new_list();
    if( value.empty() && ! r_item_spec_in.get_default().empty() )
        value = r_item_spec_in.get_default();
    if( (error = (*pf_list_lexical_validator)( *this, value )) != ELMX_OK )
        return error;
    lmx::c_xml_list_tokeniser list( value );
    std::string value_inst;
    while( list.get( &value_inst ) )
    {
        error = r_item_spec_in.is_lexical_valid( *this, value_inst );
        if( error != ELMX_OK )
            return error;
        error = r_bridge_in.convert_store_and_check_value( value_inst );
        if( error != ELMX_OK )
            return error;
    }
    return ELMX_OK;
}

//------------------------------------------------------------------------------
//                              c_xml_writer_local
//------------------------------------------------------------------------------

c_xml_writer_local::c_xml_writer_local()
    :
    mp_writer( LMXNULL ),
    mp_parent( LMXNULL ),
    m_has_attributes( false ),
    m_has_body( true ),   // Invoked for bottom member in c_xml_writer.  This prevents outputting undesired > character
    m_has_children( false )
{
}

c_xml_writer_local::c_xml_writer_local( c_xml_writer & r_writer_in )
    :
    mp_writer( &r_writer_in ),
    mp_parent( r_writer_in.get_xml_writer_local() ),
    m_has_attributes( false ),
    m_has_body( false ),
    m_has_children( false )
{
    mp_writer->set_xml_writer_local( this );
}

void c_xml_writer_local::set_writer( c_xml_writer * p_writer_in )
{
    mp_writer = p_writer_in;
}

c_xml_writer_local::~c_xml_writer_local()
{
    mp_writer->set_xml_writer_local( mp_parent );
}

//------------------------------------------------------------------------------
//                              c_xml_writer
//------------------------------------------------------------------------------

std::string c_xml_writer::m_default_tab( LMX_WRITER_DEFAULT_TAB );    // May be modified by user  // C101014_1423
std::string c_xml_writer::m_default_nl( LMX_WRITER_DEFAULT_NL );      // May be modified by user
std::string c_xml_writer::m_default_attribute_tab( LMX_WRITER_DEFAULT_ATTRIBUTE_TAB );    // May be modified by user  // C110426_1836
std::string c_xml_writer::m_default_attribute_nl( LMX_WRITER_DEFAULT_ATTRIBUTE_NL );      // May be modified by user
c_xml_writer::t_writer_options c_xml_writer::m_convenience_options = LMX_WRITER_DEFAULT_CONVENIENCE_OPTIONS;    // C150616_1455

const c_xml_writer::t_writer_options c_xml_writer::include_xml_decl = 1 << 0;
const c_xml_writer::t_writer_options c_xml_writer::include_standalone = 1 << 1;

void c_xml_writer::set_default_tab( const std::string & r_default_tab_in ) // C101014_1423
{
#if LMX_WRITER_LOCK_FORMAT == 0
    if( m_default_attribute_tab == m_default_tab && m_default_attribute_nl == m_default_nl )    // See Writer_format_compatibility
        m_default_attribute_tab = r_default_tab_in;
    m_default_tab = r_default_tab_in;
#endif
}

const std::string & c_xml_writer::get_default_tab()
{
    return m_default_tab;
}

void c_xml_writer::set_default_nl( const std::string & r_default_nl_in )
{
#if LMX_WRITER_LOCK_FORMAT == 0
    if( m_default_attribute_tab == m_default_tab && m_default_attribute_nl == m_default_nl )    // See Writer_format_compatibility
        m_default_attribute_nl = r_default_nl_in;
    m_default_nl = r_default_nl_in;
#endif
}

const std::string & c_xml_writer::get_default_nl()
{
    return m_default_nl;
}

void c_xml_writer::set_default_attribute_tab( const std::string & r_default_attribute_tab_in ) // C110426_1836
{
#if LMX_WRITER_LOCK_FORMAT == 0
    m_default_attribute_tab = r_default_attribute_tab_in;
#endif
}

const std::string & c_xml_writer::get_default_attribute_tab()
{
    return m_default_attribute_tab;
}

void c_xml_writer::set_default_attribute_nl( const std::string & r_default_attribute_nl_in )
{
#if LMX_WRITER_LOCK_FORMAT == 0
    m_default_attribute_nl = r_default_attribute_nl_in;
#endif
}

const std::string & c_xml_writer::get_default_attribute_nl()
{
    return m_default_attribute_nl;
}

void c_xml_writer::set_convenience_options( t_writer_options options_in )   // C150616_1455
{
#if LMX_WRITER_LOCK_FORMAT == 0
    m_convenience_options = options_in;
#endif
}

c_xml_writer::t_writer_options c_xml_writer::get_convenience_options()  // C150616_1455
{
    return m_convenience_options;
}

c_xml_writer::c_xml_writer(
                    std::ostream & r_os_in,
                    const char * p_tab_in /*=LMXNULL*/, const char * p_nl_in /*=LMXNULL*/,
                    const char * p_attribute_tab_in /*=LMXNULL*/, const char * p_attribute_nl_in /*=LMXNULL*/ )
    :
    m_os( r_os_in ),
    mp_xml_writer_local( LMXNULL ),
    m_bottom_xml_writer_local(),
    m_indent( 0 ),
    m_tab( p_tab_in ? p_tab_in : LMX_WRITER_DEFAULT_TAB_VAR ),
    m_nl( p_nl_in ? p_nl_in : LMX_WRITER_DEFAULT_NL_VAR ),
    m_attribute_tab( p_attribute_tab_in ? p_attribute_tab_in : p_tab_in ? p_tab_in : LMX_WRITER_DEFAULT_ATTRIBUTE_TAB_VAR ),
    m_attribute_nl( p_attribute_nl_in ? p_attribute_nl_in : p_nl_in ? p_nl_in : LMX_WRITER_DEFAULT_ATTRIBUTE_NL_VAR ),
    m_is_ns_attr_written( false ),
    mp_ns_map_to_use( LMXNULL ),
    m_expected_class_identity( LMXNULL )
{
    m_bottom_xml_writer_local.set_writer( this );
    mp_xml_writer_local = &m_bottom_xml_writer_local;

#if defined( _MSC_VER ) && ! defined( UNDER_CE ) || defined( __BORLANDC__ ) || defined( __GNUC__ ) && __GNUC__ >= 3
    m_os.imbue( std::locale::classic() ); // Make sure decimal point comes out as .
#endif
}

c_xml_writer::c_xml_writer(
                    std::ostream & r_os_in,
                    t_writer_options options_in,
                    const char * p_tab_in /*=LMXNULL*/, const char * p_nl_in /*=LMXNULL*/,
                    const char * p_attribute_tab_in /*=LMXNULL*/, const char * p_attribute_nl_in /*=LMXNULL*/ )
    :
    m_os( r_os_in ),
    mp_xml_writer_local( LMXNULL ),
    m_bottom_xml_writer_local(),
    m_indent( 0 ),
    m_tab( p_tab_in ? p_tab_in : LMX_WRITER_DEFAULT_TAB_VAR ),
    m_nl( p_nl_in ? p_nl_in : LMX_WRITER_DEFAULT_NL_VAR ),
    m_attribute_tab( p_attribute_tab_in ? p_attribute_tab_in : p_tab_in ? p_tab_in : LMX_WRITER_DEFAULT_ATTRIBUTE_TAB_VAR ),
    m_attribute_nl( p_attribute_nl_in ? p_attribute_nl_in : p_nl_in ? p_nl_in : LMX_WRITER_DEFAULT_ATTRIBUTE_NL_VAR ),
    m_is_ns_attr_written( false ),
    mp_ns_map_to_use( LMXNULL ),
    m_expected_class_identity( LMXNULL )
{
    m_bottom_xml_writer_local.set_writer( this );
    mp_xml_writer_local = &m_bottom_xml_writer_local;

#if defined( _MSC_VER ) && ! defined( UNDER_CE ) || defined( __BORLANDC__ ) || defined( __GNUC__ ) && __GNUC__ >= 3
    m_os.imbue( std::locale::classic() ); // Make sure decimal point comes out as .
#endif

    if( (options_in & include_xml_decl) || (options_in & include_standalone) )
    {
        m_os << "<?xml version=\"1.0\" encoding=\"UTF-8\"";
        if( options_in & include_standalone ) m_os << " standalone=\"yes\"";
        m_os << "?>" << m_nl;
    }
}

elmx_error c_xml_writer::capture_error(
                                elmx_error error_in,
                                const std::string & r_item_name_in,
                                const char * p_prog_file_in,
                                int prog_line_in )
{
    m_error_info.set( error_in, r_item_name_in, p_prog_file_in, prog_line_in ); // C100806_1136

    return error_in;
}

std::string & c_xml_writer::get_error_message( std::string * p_message_in ) const
{
    *p_message_in = m_error_info.to_string();   // C100806_1136

    return *p_message_in;
}

std::string c_xml_writer::get_error_message() const
{
    std::string error_message;
    return get_error_message( & error_message );
}

void c_xml_writer::add_namespace( const std::string & r_ns_prefix_in, const std::string & r_namespace_in )
{
    m_user_namespaces.push_back( s_user_ns_prefix_map( r_ns_prefix_in, r_namespace_in ) );
}

void c_xml_writer::add_schema_location( const std::string & r_ns_uri_in, const std::string & r_location_in )  // v3.1
{
    m_schema_locations.push_back( s_schema_location_item( r_ns_uri_in, r_location_in ) );
}

void c_xml_writer::conditionally_select_ns_map( const s_ns_map * p_ns_map_in )
{
    // Store the identity of the first ns_map encountered
    if( ! mp_ns_map_to_use )
        mp_ns_map_to_use = p_ns_map_in;
}

void c_xml_writer::conditionally_write_ns_attrs( bool want_xsi_in )
{
    if( m_is_ns_attr_written )
        return;

    const s_ns_map * p_ns_map_to_use = mp_ns_map_to_use;    // V3.2.4 - local variable introduced

    while( p_ns_map_to_use->p_ns )
    {
        position_attribute();   // v5 - C100316_1735
        m_os << "xmlns";

        if( p_ns_map_to_use->p_ns_prefix[0] != '\0' )
            m_os << ":" << p_ns_map_to_use->p_ns_prefix;

        m_os << "=\"" << p_ns_map_to_use->p_ns << "\"";

        ++p_ns_map_to_use;
    }

    t_user_namespaces::const_iterator i_user_namespace( m_user_namespaces.begin() );
    t_user_namespaces::const_iterator i_user_namespace_end( m_user_namespaces.end() );

    while( i_user_namespace != i_user_namespace_end )
    {
        position_attribute();   // v5 - C100316_1735
        m_os << "xmlns";

        if( ! i_user_namespace->m_ns_prefix.empty() )
            m_os << ":" << i_user_namespace->m_ns_prefix;

        m_os << "=\"" << i_user_namespace->ns_uri << "\"";

        ++i_user_namespace;
    }

    if( want_xsi_in || ! m_schema_locations.empty() )
    {
        position_attribute();   // v5 - C100316_1735
        m_os << "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"";
    }

    if( ! m_schema_locations.empty() )    // v3.1 (and check for schema_locations.empty() above)
    {
        if( m_schema_locations[0].ns_uri.empty() )
        {
            position_attribute();   // v5 - C100316_1735
            m_os << "xsi:noNamespaceSchemaLocation=\"" << m_schema_locations[0].location << "\"";
        }

        else
        {
            position_attribute();   // v5 - C100316_1735
            m_os << "xsi:schemaLocation=\"";
            for( size_t i=0; i<m_schema_locations.size(); ++i )
                {
                if( i != 0 )
                    m_os << " ";
                m_os << m_schema_locations[i].ns_uri << " " << m_schema_locations[i].location;
                }
            m_os << "\"";
        }
    }

    m_is_ns_attr_written = true;
}

// V3.2.4
const char * c_xml_writer::get_namespace_prefix( const std::string & r_namespace_in ) const
{
    const s_ns_map * p_ns_map_to_use = mp_ns_map_to_use;

    while( p_ns_map_to_use->p_ns )
    {
        if( r_namespace_in == p_ns_map_to_use->p_ns )
            return p_ns_map_to_use->p_ns_prefix;

        ++p_ns_map_to_use;
    }

    t_user_namespaces::const_iterator i_user_namespace( m_user_namespaces.begin() );
    t_user_namespaces::const_iterator i_user_namespace_end( m_user_namespaces.end() );

    while( i_user_namespace != i_user_namespace_end )
    {
        if( r_namespace_in == i_user_namespace->ns_uri )
            return i_user_namespace->m_ns_prefix.c_str();

        ++i_user_namespace;
    }

    return LMXNULL;
}

void c_xml_writer::marshal_attribute_impl(
                    const char c_name_in[],
                    c_untyped_marshal_bridge & r_bridge_in )
{
    if( r_bridge_in.count() != 0 )
    {
        position_attribute();
        m_os << c_name_in << "=\"";
        r_bridge_in.output_value( 0 );
        m_os << "\"";
    }
}

void c_xml_writer::marshal_attribute_or_default_impl(
                    const char c_name_in[],
                    c_untyped_marshal_bridge & r_bridge_in,
                    const c_untyped_validation_spec & r_item_spec_in )
{
    if( r_bridge_in.count() == 0 )
        marshal_attribute( c_name_in, r_item_spec_in.get_default() );
    else
        marshal_attribute_impl( c_name_in, r_bridge_in );
}

void c_xml_writer::marshal_attribute_list_impl(
                    const char c_name_in[],
                    c_untyped_marshal_list_bridge & r_bridge_in )
{
    if( r_bridge_in.list_count() != 0 )
    {
        position_attribute();
        m_os << c_name_in << "=\"";
        size_t count = r_bridge_in.count( 0 );
        for( size_t j=0; j<count; ++j )
        {
            if( j != 0 )
                m_os << " ";
            r_bridge_in.output_value( 0, j );  // C101015_1032
        }
        m_os << "\"";
    }
}

void c_xml_writer::marshal_attribute_list_or_default_impl(
                    const char c_name_in[],
                    c_untyped_marshal_list_bridge & r_bridge_in,
                    const c_untyped_validation_spec & r_item_spec_in )
{
    if( r_bridge_in.list_count() == 0 )
        marshal_attribute( c_name_in, r_item_spec_in.get_default() );
    else
        marshal_attribute_list_impl( c_name_in, r_bridge_in );
}

void c_xml_writer::marshal_element_impl(       // For single simple global element case
                    const char c_name_in[],
                    c_untyped_marshal_bridge & r_bridge_in,
                    const lmx::s_ns_map * p_ns_map_writer_in /*= LMXNULL*/ )  // C101015_1032
{
    size_t count = r_bridge_in.count();
    for( size_t i=0; i<count; ++i )
    {
        c_xml_writer_local xml_writer_local( *this );
        start_element( c_name_in );
        if( p_ns_map_writer_in )
        {
            conditionally_select_ns_map( p_ns_map_writer_in );
            conditionally_write_ns_attrs( false );
        }
        if( ! r_bridge_in.is_value_empty( i ) )
        {
            prepare_for_simple_body_data();
            r_bridge_in.output_value( i );
        }
        end_element( c_name_in );
    }
}

void c_xml_writer::marshal_element_or_default_impl(
                    const char c_name_in[],
                    c_untyped_marshal_bridge & r_bridge_in,
                    const c_untyped_validation_spec & r_item_spec_in )
{
    if( r_bridge_in.count() == 0 )
        marshal_element( c_name_in, r_item_spec_in.get_default() );
    else
        marshal_element_impl( c_name_in, r_bridge_in );
}

void c_xml_writer::marshal_element_body_impl(
                    const char c_name_in[],
                    c_untyped_marshal_bridge & r_bridge_in )
{
    LMX_NO_WARN_UNUSED( c_name_in );
    if( ! r_bridge_in.is_value_empty( 0 ) )
    {
        prepare_for_simple_body_data();
        r_bridge_in.output_value( 0 ); // C101015_1032
    }
}

void c_xml_writer::marshal_element_list_impl(      // For single simple global element case
                    const char c_name_in[],
                    c_untyped_marshal_list_bridge & r_bridge_in,
                    const lmx::s_ns_map * p_ns_map_writer_in /*= LMXNULL*/ )  // C101015_1032
{
    size_t list_count = r_bridge_in.list_count();
    for( size_t i=0; i<list_count; ++i )
    {
        c_xml_writer_local xml_writer_local( *this );
        start_element( c_name_in );
        if( p_ns_map_writer_in )
        {
            conditionally_select_ns_map( p_ns_map_writer_in );
            conditionally_write_ns_attrs( false );
        }
        size_t count = r_bridge_in.count( i );
        for( size_t j=0; j<count; ++j )
        {
            if( j != 0 )
                m_os << " ";
            prepare_for_simple_body_data(); // Preparing for body data here allows for <empty/> elements
            r_bridge_in.output_value( i, j );
        }
        end_element( c_name_in );
    }
}

void c_xml_writer::marshal_element_list_or_default_impl(
                    const char c_name_in[],
                    c_untyped_marshal_list_bridge & r_bridge_in,
                    const c_untyped_validation_spec & r_item_spec_in )
{
    if( r_bridge_in.list_count() == 0 )
        marshal_element( c_name_in, r_item_spec_in.get_default() );
    else
        marshal_element_list_impl( c_name_in, r_bridge_in );
}

void c_xml_writer::marshal_element_body_list_impl(
                    const char c_name_in[],
                    c_untyped_marshal_list_bridge & r_bridge_in )
{
    LMX_NO_WARN_UNUSED( c_name_in );
    size_t count = r_bridge_in.count( 0 );
    for( size_t j=0; j<count; ++j )
    {
        if( j != 0 )
            m_os << " ";
        prepare_for_simple_body_data(); // Preparing for body data here allows for <empty/> elements
        r_bridge_in.output_value( 0, j );
    }
}

//------------------------------------------------------------------------------
//                      c_unmarshal_helper - v4.3
//------------------------------------------------------------------------------

c_unmarshal_helper::c_unmarshal_helper( c_xml_reader & r_reader_in )
    :
    mr_reader( r_reader_in ),
    m_element_name_stack_node( r_reader_in.get_element_name_stack(), r_reader_in.name ),
    mp_cpp_file_name( LMXNULL ),
    m_line_num( 0 ),
    mp_anyattributes( LMXNULL ),
    m_can_body_be_empty( false )
{
}

elmx_error c_unmarshal_helper::unmarshal_empty_content( const char c_cpp_file_name_in[], int line_num_in )
{
    mp_cpp_file_name = c_cpp_file_name_in;
    m_line_num = line_num_in;

    elmx_error error = ELMX_OK;

    if( (error = unmarshal_all_attributes()) != ELMX_OK )
        return error;

    if( (error = unmarshal_empty_content_case()) != ELMX_OK )
        return error;

    return ELMX_OK;
}

elmx_error c_unmarshal_helper::unmarshal_simple_content( const char c_cpp_file_name_in[], int line_num_in )
{
    mp_cpp_file_name = c_cpp_file_name_in;
    m_line_num = line_num_in;

    elmx_error error = ELMX_OK;

    if( (error = unmarshal_all_attributes()) != ELMX_OK )
        return error;

    if( (error = unmarshal_simple_content_case()) != ELMX_OK )
        return error;

    return ELMX_OK;
}

elmx_error c_unmarshal_helper::unmarshal_complex_content( const char c_cpp_file_name_in[], int line_num_in )
{
    mp_cpp_file_name = c_cpp_file_name_in;
    m_line_num = line_num_in;

    elmx_error error = ELMX_OK;

    if( (error = unmarshal_all_attributes()) != ELMX_OK )
        return error;

    if( (error = unmarshal_complex_content_case()) != ELMX_OK )
        return error;

    return ELMX_OK;
}

elmx_error c_unmarshal_helper::unmarshal_all_attributes()
{
    elmx_error error = ELMX_OK;
    while( mr_reader.get_attribute_event( LMXNULL, &error, &mr_reader.name ) )
    {
        if( error != ELMX_OK && (error = mr_reader.handle_error( error, mr_reader.name, mp_cpp_file_name, m_line_num )) != ELMX_OK )
            return error;

        bool is_attribute_processed = unmarshal_an_attribute( &error );
        if( error != ELMX_OK )
            return error;

        if( ! is_attribute_processed )
        {
            if( mr_reader.is_xsi_attribute() )
            {
                mr_reader.skip_attribute_value();
            }
            else
            {
                if( mp_anyattributes )
                {
                    if( ! mr_reader.get_attribute_value( &mr_reader.value ) )
                    {
                        if( (error = mr_reader.handle_error( ELMX_UNABLE_TO_READ_ATTRIBUTE_VALUE, mr_reader.name, mp_cpp_file_name, m_line_num )) != ELMX_OK )
                            return error;
                    }
                    else
                    {
                        std::string attribute_namespace;
                        mr_reader.get_namespace( mr_reader.name, &attribute_namespace );
                        mp_anyattributes->push_back_w_autop( new c_any_info( attribute_namespace, mr_reader.name, mr_reader.value ) ); // Will make a copy of c_any_info
                    }
                }
                else
                {
                    if( ! mr_reader.is_auto_versioning() )
                    {
                        if( (error = mr_reader.handle_error( ELMX_UNEXPECTED_ATTRIBUTE, mr_reader.name, mp_cpp_file_name, m_line_num )) != ELMX_OK )
                            return error;
                    }
                    else        // V7.1 - C150417_1659
                    {
                        if( (error = mr_reader.found_unknown_attribute( mr_reader.name, mp_cpp_file_name, m_line_num )) != ELMX_OK )
                            return error;
                    }
                    mr_reader.skip_attribute_value();
                }
            }
        }
    }

    if( (error = unmarshal_attributes_check()) != ELMX_OK )
        return error;

    return ELMX_OK;
}

elmx_error c_unmarshal_helper::unmarshal_empty_content_case()
{
    c_xml_reader_local reader_local( &mr_reader );
    elmx_error error = ELMX_OK;

    if( mr_reader.get_current_event() == EXE_ELEMENT_END )
        return ELMX_OK;

    mr_reader.get_element_event( &error, &mr_reader.name );
    if( error != ELMX_OK && (error = mr_reader.handle_error( error, m_element_name_stack_node.get_str(), mp_cpp_file_name, m_line_num )) != ELMX_OK )
        return error;

    if( mr_reader.is_auto_versioning() && mr_reader.get_current_event() != EXE_ELEMENT_END )
    {
        while( mr_reader.is_current_event_tokenisable() )
        {
            if( ! mr_reader.skip_rest_of_element( mr_reader.name ) && (error = mr_reader.handle_error( ELMX_BAD_ELEMENT_END, mr_reader.name, mp_cpp_file_name, m_line_num )) != ELMX_OK )
                return error;
            mr_reader.get_element_event( &error, &mr_reader.name );
            if( error != ELMX_OK && (error = mr_reader.handle_error( error, m_element_name_stack_node.get_str(), mp_cpp_file_name, m_line_num )) != ELMX_OK )
                return error;
        }
    }

    if( (mr_reader.get_current_event() != EXE_ELEMENT_END || m_element_name_stack_node != mr_reader.name) &&
            (error = mr_reader.handle_error( ELMX_BAD_ELEMENT_END, m_element_name_stack_node.get_str(), mp_cpp_file_name, m_line_num )) != ELMX_OK )
        return error;

    return ELMX_OK;
}

elmx_error c_unmarshal_helper::unmarshal_simple_content_case()
{
    elmx_error error = ELMX_OK;
    mr_reader.name.erase();      // Set r_reader.name for simpleContent to get names in error reports
    mr_reader.name = m_element_name_stack_node.get_str();
    if( ! is_nil() )
    {
        unmarshal_simple_body( &error );
        if( error != ELMX_OK )
            return error;
    }

    else
    {
        if( ! mr_reader.is_element_end_tag() &&  // C110316_1000 - Error now reported to error handler
                (error = mr_reader.handle_error( ELMX_BAD_ELEMENT_END, m_element_name_stack_node.get_str(), mp_cpp_file_name, m_line_num )) != ELMX_OK )
            return error;
    }

    // reading ur-type procedure already consumes the end tag, so allow for that case
    // For nillable case, we can check the current event again even if end tag has been read during ur-type reading
    // is_nillable case allows for <elem xsi:nil=true></elem>
    return ELMX_OK;
}

elmx_error c_unmarshal_helper::unmarshal_complex_content_case()
{
    elmx_error error = ELMX_OK;
    c_xml_reader_local reader_local( &mr_reader );

    if( mr_reader.get_current_event() == EXE_ELEMENT_END && m_can_body_be_empty )
        return ELMX_OK;

    if( mr_reader.get_current_event() != EXE_ELEMENT_END )
    {
        mr_reader.get_element_event( &error, &mr_reader.name );
        if( error != ELMX_OK && (error = mr_reader.handle_error( error, m_element_name_stack_node.get_str(), mp_cpp_file_name, m_line_num )) != ELMX_OK )
            return error;
    }

    if( is_nil() )
    {
        // Allow for <elem xsi:nil=true></elem>
        return verify_element_end();
    }

    else if( mr_reader.get_current_event() == EXE_ELEMENT_END )
    {
        if( ! m_can_body_be_empty )
        {
            if( (error = mr_reader.handle_error( ELMX_UNEXPECTED_EMPTY_ELEMENT, m_element_name_stack_node.get_str(), mp_cpp_file_name, m_line_num )) != ELMX_OK )
                return error;
        }

        if( m_element_name_stack_node != mr_reader.name )
        {
            if( (error = mr_reader.handle_error( ELMX_BAD_ELEMENT_END, m_element_name_stack_node.get_str(), mp_cpp_file_name, m_line_num )) != ELMX_OK )
                return error;
        }

        return ELMX_OK;
    }

    else
    {
        unmarshal_complex_body( &error );
        if( error != ELMX_OK )
            return error;

        if( mr_reader.get_current_event() == EXE_HAS_SKIPPED_UNKNOWN_ELEMENTS )
            mr_reader.restore_event( EXE_ELEMENT_END );      // Can now treat it as element end

        if( mr_reader.is_auto_versioning() && mr_reader.get_current_event() != EXE_ELEMENT_END )
        {
            if( is_not_choice_or_choice_is_not_set() )
            {
                while( mr_reader.is_current_event_tokenisable() )
                {
                    if( (error = mr_reader.found_unknown_element( mr_reader.name, mp_cpp_file_name, m_line_num )) != ELMX_OK )  // V7.1 - C150417_1659
                        return error;
                    if( ! mr_reader.skip_rest_of_element( mr_reader.name ) && (error = mr_reader.handle_error( ELMX_BAD_ELEMENT_END, mr_reader.name, mp_cpp_file_name, m_line_num )) != ELMX_OK )
                        return error;
                    mr_reader.get_element_event( &error, &mr_reader.name );
                    if( error != ELMX_OK && (error = mr_reader.handle_error( error, m_element_name_stack_node.get_str(), mp_cpp_file_name, m_line_num )) != ELMX_OK )
                        return error;
                }

            if( mr_reader.get_current_event() == EXE_HAS_SKIPPED_UNKNOWN_ELEMENTS )
                mr_reader.restore_event( EXE_ELEMENT_END );      // Can now treat it as element end
            }
        }

        if( (mr_reader.get_current_event() != EXE_ELEMENT_END || m_element_name_stack_node != mr_reader.name) &&
                (error = mr_reader.handle_error( ELMX_BAD_ELEMENT_END, m_element_name_stack_node.get_str(), mp_cpp_file_name, m_line_num )) != ELMX_OK )
            return error;
    }

    return ELMX_OK;
}

// Default virtual handler - can be overridden by derived class
bool c_unmarshal_helper::unmarshal_simple_body(
                                    elmx_error * LMX_UNUSED_PARAM( p_error_out ) )
{
    lmx_assert( 0 );    // Should always be overridden if used
    return false;
}

// Default virtual handler - can be overridden by derived class
bool c_unmarshal_helper::unmarshal_complex_body(
                                    elmx_error * LMX_UNUSED_PARAM( p_error_out ) )
{
    lmx_assert( 0 );    // Should always be overridden if used
    return false;
}

elmx_error c_unmarshal_helper::verify_element_end()
{
    elmx_error error = ELMX_OK;
    if( mr_reader.get_current_event() != EXE_ELEMENT_END )
    {
        mr_reader.get_element_event( &error, &mr_reader.name );
        if( error != ELMX_OK && (error = mr_reader.handle_error( error, m_element_name_stack_node.get_str(), mp_cpp_file_name, m_line_num )) != ELMX_OK )
            return error;
        if( (mr_reader.get_current_event() != EXE_ELEMENT_END || m_element_name_stack_node != mr_reader.name) &&
                (error = mr_reader.handle_error( ELMX_BAD_ELEMENT_END, m_element_name_stack_node.get_str(), mp_cpp_file_name, m_line_num )) != ELMX_OK )
            return error;
    }
    return ELMX_OK;
}

//------------------------------------------------------------------------------
//                              Global check_exception
//------------------------------------------------------------------------------

c_check_exception global_check_exception;

//------------------------------------------------------------------------------
//                         Error code interpretation
//------------------------------------------------------------------------------

#define ERROR_MAP( c, s ) c, #c, s

s_error_description mc_error_description[] = {
    { ERROR_MAP( ELMX_OK,                                  "No Error" ) },
    { ERROR_MAP( ELMX_NO_FILE,                             "Unable to open file" ) },
    { ERROR_MAP( ELMX_ALL_CARDINALITY_VIOLATION,           "Violation of xs:all cardinality constraints" ) },
    { ERROR_MAP( ELMX_ATTRIBUTE_ALREADY_READ,              "Attribute has already been read" ) },
    { ERROR_MAP( ELMX_ATTRIBUTES_IN_ANONYMOUS_COMPOSITOR,  "Attributes found in anonymous compositor" ) },
    { ERROR_MAP( ELMX_BADLY_FORMED,                        "XML badly formed" ) },
    { ERROR_MAP( ELMX_BAD_CHAR_ENCODING,                   "XML character encoding not supported" ) },
    { ERROR_MAP( ELMX_BAD_CHOICE,                          "Inappropriate choice option selected" ) },
    { ERROR_MAP( ELMX_BAD_COMMENT,                         "Badly formed comment" ) },
    { ERROR_MAP( ELMX_BAD_DTD,                             "Badly formed DTD" ) },
    { ERROR_MAP( ELMX_BAD_ELEMENT_END,                     "Bad end of element" ) },
    { ERROR_MAP( ELMX_BAD_END_OF_START_TAG,                "Bad start tag end" ) },
    { ERROR_MAP( ELMX_BAD_END_TAG,                         "Syntax error in end tag" ) },
    { ERROR_MAP( ELMX_BAD_XML_DECL,                        "Bad XML Decl spec" ) },
    { ERROR_MAP( ELMX_BAD_XML_VERSION,                     "Unsupported XML version" ) },
    { ERROR_MAP( ELMX_CHAR_ENCODING_MISMATCH,              "Mis-match between specified char encoding and read bytes" ) },
    { ERROR_MAP( ELMX_ELEMENT_NOT_FOUND,                   "Element not found" ) },
    { ERROR_MAP( ELMX_EXTERNAL_ENTITY,                     "Reference to external entity" ) },
    { ERROR_MAP( ELMX_FRACTION_DIGITS_EXCEEDED,            "Fraction digits exceeded" ) },
    { ERROR_MAP( ELMX_LENGTH_TOO_LONG,                     "Length too long" ) },
    { ERROR_MAP( ELMX_LENGTH_TOO_SHORT,                    "Length too short" ) },
    { ERROR_MAP( ELMX_MANDATORY_ELEMENT_MISSING,           "Mandatory element missing" ) },
    { ERROR_MAP( ELMX_NO_PATTERN_MATCHED,                  "No pattern matched" ) },
    { ERROR_MAP( ELMX_NOT_WELL_FORMED,                     "Not well formed" ) },
    { ERROR_MAP( ELMX_RECURSIVE_ENTITY_DEFINITION,         "Recursive entity definition" ) },
    { ERROR_MAP( ELMX_REQUIRED_ATTRIBUTES_MISSING,         "Required attribute(s) missing" ) },
    { ERROR_MAP( ELMX_TOO_MANY_ITEMS,                      "Element instances exceeds maxOccurs" ) },
    { ERROR_MAP( ELMX_TOTAL_DIGITS_EXCEEDED,               "Total digits exceeded" ) },
    { ERROR_MAP( ELMX_UNABLE_TO_READ_ATTRIBUTE_VALUE,      "Unable to read attribute value" ) },
    { ERROR_MAP( ELMX_UNABLE_TO_READ_ELEMENT_VALUE,        "Unable to read element value" ) },
    { ERROR_MAP( ELMX_UNEXPECTED_ALL_EVENT,                "Unknown element in xs:all" ) },
    { ERROR_MAP( ELMX_UNEXPECTED_ELEMENT_EVENT,            "Unknown element in complexContent" ) },
    { ERROR_MAP( ELMX_UNEXPECTED_EMPTY_ELEMENT,            "Unexpected empty element" ) },
    { ERROR_MAP( ELMX_UNEXPECTED_ENTITY,                   "Unexpected entity" ) },
    { ERROR_MAP( ELMX_UNEXPECTED_EOM,                      "Unexpected end of message" ) },
    { ERROR_MAP( ELMX_VALUE_BAD_FORMAT,                    "Invalid value format for this type" ) },
    { ERROR_MAP( ELMX_VALUE_EXCEEDS_MAX,                   "Value greater than specified maximum" ) },
    { ERROR_MAP( ELMX_VALUE_EXCEEDS_MIN,                   "Value less than specified minimum" ) },
    { ERROR_MAP( ELMX_VALUE_NOT_ENUMERATED,                "Value does not match enumerations" ) },
    // V 2.1 addition
    { ERROR_MAP( ELMX_UNEXPECTED_ATTRIBUTE,                "Unexpected attribute" ) },
    { ERROR_MAP( ELMX_NILLED_ELEMENT_NOT_EMPTY,            "Nilled element body not empty" ) },
    // V2.7 addition
    { ERROR_MAP( ELMX_ENTITY_NOT_FOUND,                    "Entity not found" ) },
    // V3.0 addition
    { ERROR_MAP( ELMX_UNDEFINED_ERROR,                     "Undefined error" ) },
    // V3.2 addition
    { ERROR_MAP( ELMX_VALUE_DOES_NOT_MATCH_FIXED_CONSTRAINT, "Value does not match specified fixed constraint" ) },
    // V3.4 addition
    { ERROR_MAP( ELMX_UNKNOWN_XSI_TYPE,                    "Unknown xsi:type" ) },
    { ERROR_MAP( ELMX_TYPE_NOT_SET,                        "Type not set" ) },
    // V3.8 addition
    { ERROR_MAP( ELMX_OCCURRENCE_ERROR,                    "min/max occurs error on marshaling" ) },

    // V3.8 addition - primarily for use in snippet event handlers
    { ERROR_MAP( ELMX_USER_DEFINED_1,                      "User defined error 1" ) },
    { ERROR_MAP( ELMX_USER_DEFINED_2,                      "User defined error 2" ) },
    { ERROR_MAP( ELMX_USER_DEFINED_3,                      "User defined error 3" ) },
    { ERROR_MAP( ELMX_USER_DEFINED_4,                      "User defined error 4" ) },
    { ERROR_MAP( ELMX_USER_DEFINED_5,                      "User defined error 5" ) },
    { ERROR_MAP( ELMX_USER_DEFINED_6,                      "User defined error 6" ) },
    { ERROR_MAP( ELMX_USER_DEFINED_7,                      "User defined error 7" ) },
    { ERROR_MAP( ELMX_USER_DEFINED_8,                      "User defined error 8" ) },
    { ERROR_MAP( ELMX_USER_DEFINED_9,                      "User defined error 9" ) }
};

LMX_PDECL const s_error_description * get_error_mapping( elmx_error error_code_in )
{
    for( size_t i=0;
                i<sizeof( mc_error_description )/sizeof( mc_error_description[0] );
                ++i )
    {
        if( mc_error_description[i].code == error_code_in )
            return &(mc_error_description[i]);
    }

    return LMXNULL;
}

LMX_PDECL const char * to_string( elmx_error error_code_in )
{
    const s_error_description * lp_error_map = get_error_mapping( error_code_in );

    if( lp_error_map )
        return lp_error_map->to_string;

    return "<Code Not Found>";
}

LMX_PDECL const char * get_error_description( elmx_error error_code_in )
{
    const s_error_description * lp_error_map = get_error_mapping( error_code_in );

    if( lp_error_map )
        return lp_error_map->p_description;

    return "";
}

//------------------------------------------------------------------------------
//                             Must Understand
//------------------------------------------------------------------------------

LMX_PDECL elmx_must_understand bool_must_understand( const std::string & r_value_in )
{
    if( r_value_in == "1" || r_value_in == "true" )
        return EMU_MUST_UNDERSTAND;

    else if( r_value_in == "0" || r_value_in == "false" )
        return EMU_OK;

    return EMU_ERROR;
}

elmx_must_understand p_has_must_understand( c_xml_reader & r_reader_in,
                            const std::string & r_namespace_in,
                            const std::string & r_name_in,
                            t_must_understand_test_function function_in,
                            const std::string & r_element_name_in )
{
    std::string name;
    elmx_error error;

    while( r_reader_in.get_attribute_event( LMXNULL, &error, &name ) )
    {
        if( error != ELMX_OK )
            return EMU_ERROR;

        else if( r_reader_in.get_current_event() == EXE_TOKEN )
        {
            bool check_value = false;
            std::string local_name;
            c_xml::get_local_name( name, &local_name );
            if( local_name == r_name_in )
            {
                std::string attribute_namespace;
                r_reader_in.get_namespace( name, &attribute_namespace );
                if( attribute_namespace == r_namespace_in )
                    check_value = true;
            }

            if( check_value )
            {
                std::string value;
                if( ! r_reader_in.get_attribute_value( &value ) )
                    return EMU_ERROR;

                elmx_must_understand result = function_in( value );
                if( result != EMU_OK )
                    return result;
            }

            else
                r_reader_in.skip_attribute_value();
        }
    }

    if( r_reader_in.get_current_event() == EXE_ELEMENT_END )
        return EMU_OK;

    for(;;)
    {
        c_xml_reader_local reader_local( &r_reader_in );
        r_reader_in.get_element_event( LMXNULL, &error, &name );

        if( error != ELMX_OK )
            return EMU_ERROR;

        else if( r_reader_in.get_current_event() == EXE_TOKEN )
        {
            elmx_must_understand result = p_has_must_understand( r_reader_in, r_namespace_in, r_name_in, function_in, name );
            if( result != EMU_OK )
                return result;
        }

        else if( r_reader_in.get_current_event() == EXE_ELEMENT_END )
        {
            if( name == r_element_name_in )
                return EMU_OK;

            return EMU_ERROR;
        }

        else if( r_reader_in.get_current_event() == EXE_CDATA )
            {}  // No action

        else
            return EMU_ERROR;
    }

    lmx_assert_code_not_reached();  // Shouldn't get to here

    return EMU_OK;
}

LMX_PDECL elmx_must_understand has_must_understand( c_xml_reader & r_reader_in,
                                                    const std::string & r_namespace_in,
                                                    const std::string & r_name_in,
                                                    t_must_understand_test_function function_in )
{
    c_xml_reader_local reader_local( &r_reader_in );
    std::string name;
    elmx_error error;
    r_reader_in.get_element_event( LMXNULL, &error, &name );

    if( r_reader_in.get_current_event() != EXE_TOKEN )
        return EMU_ERROR;

    elmx_must_understand must_understand = p_has_must_understand( r_reader_in, r_namespace_in, r_name_in, function_in, name );
    r_reader_in.rewind();

    return must_understand;
}

//------------------------------------------------------------------------------
//                            get_instance_namespace
//------------------------------------------------------------------------------

LMX_PDECL bool get_instance_namespace( c_xml_reader & r_reader_in, std::string * p_namespace_out_in )
{
    c_xml_reader_local reader_local( &r_reader_in );
    std::string name;
    elmx_error error;
    r_reader_in.get_element_event( LMXNULL, &error, &name );

    if( r_reader_in.get_current_event() != EXE_TOKEN )
    {
        r_reader_in.rewind();     // Will clear the stored namespace info, so can't be unconditionally called before get_namespace()
        return false;
    }

    r_reader_in.get_namespace( name, p_namespace_out_in );
    r_reader_in.rewind();     // Will clear the stored namespace info, so must be after get_namespace()

    return *p_namespace_out_in != c_xml_reader::k_namespace_not_found;
}

//------------------------------------------------------------------------------
//                            Utility functions
//------------------------------------------------------------------------------

char * xstrdup( const char * p_string_in )
{
    // For portablity, make sure we know whether we're using new or malloc (N.B. _not_ new[])
    char * p_new_string = static_cast<char *>( operator new( LMXSTDCRT::strlen( p_string_in ) + 1 ) );
    if( p_new_string )
        LMXSTDCRT::strcpy( p_new_string, p_string_in );
    return p_new_string;
}

#if ! defined( _MSC_VER ) && ! defined( __BORLANDC__ )
int xstricmp( const char * p_1_in, const char * p_2_in )
{
    for(;;)
    {
        if( LMXSTDCRT::tolower( static_cast<unsigned char>(*p_1_in) ) != LMXSTDCRT::tolower( static_cast<unsigned char>(*p_2_in) ) )
            return LMXSTDCRT::tolower( static_cast<unsigned char>(*p_1_in) ) - LMXSTDCRT::tolower( static_cast<unsigned char>(*p_2_in) );
        if( *p_1_in == '\0' )
            return 0;
        ++p_1_in; ++p_2_in;
    }
}
#endif

#if defined( __GNUC__ ) && __GNUC__ < 4
LMX_PDECL double float_zero()   // Added in V2.9
{
    return 0.0;
}
#endif

LMX_PDECL void bswap( void * p_lhs_in, void * p_rhs_in, size_t size )
{
    char * p_clhs = reinterpret_cast<char*>(p_lhs_in);
    char * p_crhs = reinterpret_cast<char*>(p_rhs_in);
    for( size_t i=0; i<size; ++i )
        std::swap( *(p_clhs+i), *(p_crhs+i) );
}

} // End of namepsace lmx

//------------------------------------------------------------------------------
//                   Copyright (c) 2003-2013 Codalogic Ltd.
//------------------------------------------------------------------------------
