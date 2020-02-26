//------------------------------------------------------------------------------
//                   Copyright (c) 2003-2018 Codalogic Ltd.
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
//
// A note on version numbers: We often make builds of the code in response to
//      customer requests.  Such builds may not be formally released as an
//      official build, but are still given a version number.  Hence, in the
//      version history below you will see version numbers that do not
//      directly correspond to formal releases.
//
// Version 6.5:
//      - !C131206_1452: Improved handling of serialisation of floating
//        point numbers on non-Windows platforms.
//
// Version: 6.3.1
//      - See C130425_1106 in lmxtypes.h.
//
// Version 6.3:
//      - Updated variable naming convention.
//
// Version: 6.1
//      - !C120312_1049: Some CSTA implementations require escaping of the '>'
//        character even though this is non-standard XML behavior.  To always
//        escape '>' to '&gt;' define LMX_XML_ALWAYS_ESCAPE_GT to be 1.
//
// Version: 5.1.1
//      - !C100702_1244: Replaced occurences of ?.at(?) with ?[?]
//
// Version: 5.1
//      - See lmxtypes.h.
//
// Version: 5
//      - Reformatted.
//
// Version: 4.2
//      - Added additional ()s to (a && b || c && d) type expressions to suppress
//        over zealous GCC 4.3.3+ warnings.  See gcc_433_brackets.
//
// Version: 3.11
//      - Prevented ]]> appearing in XML output
//
// Version: 3.9
//      - Replaced some of the old-style casts
//      - Modified the output converter functions that they could more easily
//        be overridden when using the Standard Edition.
//
// Version: 3.8
//      - Temp storage value of os.precision() is now correctly std::streamsize
//        rather than int.
//      - Added #ifndef LMX_NO_WSTRING sections to allow conditional removal
//        of std::wstring from a build.
//
// Version: 3.7
//      - Added manipulator to enable output of SOAP like boolean values that
//        are either 0 or 1.
//
// Version: 3.3
//      - Added QName helper functions.
//
// Version: 3.2.1
//      - Fixed handling of fractional seconds less than 10.0 in time and
//        datetime types.
//
// Version: 3.1
//      - Added function to return the number of Unicode characters encoded
//        into a UTF-8 encoded string.
//
// Version: 3.0
//      - Used std:: versions of a number of C utility functions.
//
// Version: 2.11
//      - History log in lmxtypes.h
//
// Version: 2.10
//      - History log in lmxtypes.h
//
// Version: 2.9
//      - Changed #include <ostream> to #include <iostream> for better
//        portability.
//
// Version: 2.8
//      - History log in lmxtypes.h
//
// Version: 2.7
//      - History log in lmxtypes.h
//
// Version: 2.6
//
// Version: 2.5
//
//------------------------------------------------------------------------------


#if defined( _MSC_VER ) && _MSC_VER > 1000
#pragma warning (disable:4786)  // Disable truncated debug info warning
#endif

#if defined( _MSC_VER ) && _MSC_VER >= 1400
#pragma warning (disable:4996)  // Disable VC8 'safe' warnings
#pragma warning (disable:4100)  // Disable unreferenced formal parameter
#pragma warning (disable:4702)  // Disable unreachable code
#pragma warning (disable:4127)  // Disable conditional expression is constant [for if( sizeof( wchar_t ) == 2 )]
#endif

#include <cctype>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include <cstring>

#ifndef LMX_TYPES_SOURCE
#define LMX_TYPES_SOURCE
#endif

#include "lmxuser.h"    // lmxuser.h will include lmxtypes.h at the right place

namespace lmx {

//------------------------------------------------------------------------------
//                     Basic converters and validators
//------------------------------------------------------------------------------

#ifndef LMX_NO_WSTRING

LMX_TDECL std::wstring & vtostring( std::wstring & r_dest_out, const std::string & r_src_in )
{
    r_dest_out.erase();
    t_unicode_char high_surrogate = '\0';
    char size;
    for( std::string::size_type i=0; i<r_src_in.length(); i += size )
    {
        char msb;
        if( (r_src_in[i] & 0x80) == 0 )
        {
            msb = r_src_in[i];
            size = 1;
        }
        else if( (r_src_in[i] & 0xe0) == 0xc0 )
        {
            // 2 bytes
            msb = r_src_in[i] & 0x1f;
            size = 2;
        }
        else if( (r_src_in[i] & 0xf0) == 0xe0 )
        {
            // 3 bytes
            msb = r_src_in[i] & 0x0f;
            size = 3;
        }
        else if( (r_src_in[i] & 0xf8) == 0xf0 )
        {
            // 4 bytes
            msb = r_src_in[i] & 0x07;
            size = 4;
        }
        else
        {
            break;
        }

        if( i + size > r_src_in.length() )
            return r_dest_out;

        t_unicode_char unicode_char = msb;
        int j;
        for( j=1; j<size; ++j )
        {
            if( (r_src_in[i+j] & 0xc0) != 0x80 )
                return r_dest_out;
            unicode_char <<= 6;
            unicode_char += r_src_in[i+j] & 0x3f;
        }

        if( high_surrogate != '\0' )
        {
            if( unicode_char < 0x0000dc00 || unicode_char > 0x0000dfff )
                break;
            if( sizeof( wchar_t ) == 2 )    // Rely on the compiler to optimize this out!
            {
                r_dest_out += static_cast<wchar_t>( high_surrogate );
                r_dest_out += static_cast<wchar_t>( unicode_char );
            }
            else
            {
                unicode_char = ((high_surrogate & 0x3ff)<<10) + (unicode_char & 0x3ff) + 0x10000;
                r_dest_out += static_cast<wchar_t>( unicode_char );
            }
            high_surrogate = '\0';
        }
        else if( unicode_char >= 0x0000d800 && unicode_char <= 0x0000dbff )
        {
            high_surrogate = unicode_char;
        }
        else if( unicode_char <= 0x0000ffff )
            r_dest_out += static_cast<wchar_t>( unicode_char );
        else
        {
            if( sizeof( wchar_t ) == 2 )    // Rely on the compiler to optimize this out!
            {
                unicode_char -= 0x10000;
                r_dest_out += static_cast<wchar_t>( ((unicode_char >> 10) & 0x03ff) + 0x0000d800 );
                r_dest_out += static_cast<wchar_t>( (unicode_char & 0x03ff) + 0x0000dc00 );
            }
            else
                r_dest_out += static_cast<wchar_t>( unicode_char );
        }
    }
    return r_dest_out;
}

LMX_TDECL size_t length( const std::wstring & r_utf16_string_in )  // v6.2 - C121028_1228
{
    size_t length = 0;
    for( size_t i=0; i<r_utf16_string_in.size(); ++i )
    {
        ++length;
        if( r_utf16_string_in[i] >= 0x0000d800 && r_utf16_string_in[i] <= 0x0000dbff )
            ++i;  // 2 UTF-16 codes per Unicode char
    }

    return length;
}
#endif

LMX_TDECL std::string & vtostring( std::string & r_dest_out, const std::string & r_src_in )
{
    r_dest_out.erase();    // Avoid VC bug
    r_dest_out = r_src_in;
    return r_dest_out;
}

LMX_TDECL size_t length( const std::string & r_utf8_string_in )   // v3.1 - Returns length in Unicode characters
{
    size_t length = 0;
    size_t step;
    for( size_t i=0; i<r_utf8_string_in.size(); i += step )
    {
        ++length;
        if( (r_utf8_string_in[i] & 0x80) == 0 )
        {
            step = 1;
        }
        else if( (r_utf8_string_in[i] & 0xe0) == 0xc0 )
        {
            // 2 bytes
            step = 2;
        }
        else if( (r_utf8_string_in[i] & 0xf0) == 0xe0 )
        {
            // 3 bytes - or 6 bytes if surrogate
            if( r_utf8_string_in[i] == '\xed' && r_utf8_string_in.size() > i+1 && (r_utf8_string_in[i+1] & 0xe0) == 0xa0 )
                step = 6;
            else
                step = 3;
        }
        else if( (r_utf8_string_in[i] & 0xf8) == 0xf0 )
        {
            // 4 bytes
            step = 4;
        }
        else
        {
            break;
        }
    }

    return length;
}

LMX_TDECL bool is_min_length_ok( const std::string & r_utf8_string_in, size_t length_in )  // v3.1
{
    if( r_utf8_string_in.size() >= 4 * length_in ) // Worst case, each Unicode character may be encoded as 4 bytes
        return true;    // Definitely greater than or equal

    // May have multibyte chars so need to look more carefully
    if( length( r_utf8_string_in ) >= length_in )
        return true;

    return false;
}

LMX_TDECL bool is_max_length_ok( const std::string & r_utf8_string_in, size_t length_in )  // v3.1
{
    if( r_utf8_string_in.size() <= length_in )
        return true;    // Definitely less than or equal

    if( r_utf8_string_in.size() > 4 * length_in )  // Worst case, each Unicode character may be encoded as 4 bytes
        return false;   // Definitely greater than

    // May have multibyte chars so need to look more carefully
    if( length( r_utf8_string_in ) <= length_in )
        return true;

    return false;
}

LMX_TDECL bool is_valid_bool( const std::string & r_s_in )
{
    //true, false, 1, 0
    if( r_s_in == "true" || r_s_in == "1" || r_s_in == "false" || r_s_in == "0" )
        return true;
    return false;
}

LMX_TDECL bool is_valid_float( const std::string & r_s_in )
{
    if( r_s_in == "INF" || r_s_in == "-INF" || r_s_in == "NaN" )
        return true;
    const char * p_s = r_s_in.c_str();
    skip_sign( p_s );
    if( skip_decimal( p_s ) )
    {
        if( skip_const( p_s, "e" ) || skip_const( p_s, "E" ) )
        {
            skip_sign( p_s );
            if( ! skip_digits( p_s ) )
                return false;
        }
        if( *p_s == '\0' )
            return true;
    }
    return false;
}

LMX_TDECL bool is_valid_integer( const std::string & r_v_in )
{
    const char * p_s = r_v_in.c_str();
    skip_sign( p_s );
    if( skip_digits( p_s ) > 0 && *p_s == '\0' )
        return true;
    return false;
}

LMX_TDECL int total_digits( const std::string & r_value_in )
{
    // Assumes that a check has already been made that this is a valid integer
    int digits = 0;
    for( std::string::size_type i=0; i<r_value_in.size(); ++i )
        if( xisdigit( r_value_in[i] ) ) ++digits;
    return digits;
}

LMX_TDECL int fraction_digits( const std::string & r_value_in )
{
    // Assumes that a check has already been made that this is a valid integer
    int digits = 0;
    std::string::size_type i = r_value_in.find( '.' );
    if( i != std::string::npos )
        for( ++i; i<r_value_in.size(); ++i )
            if( xisdigit( r_value_in[i] ) ) ++digits;
    return digits;
}

//------------------------------------------------------------------------------
//                                   Helpers
//------------------------------------------------------------------------------

template <typename T>
std::string as_string( const T & r_in )
{
    std::ostringstream t;
    t << r_in;
    return t.str();
}

class c_to_long
{
private:
    long m_value;
    bool m_is_neg;
    bool m_is_overflowed;

public:
    c_to_long() : m_value( 0 ), m_is_neg( false ), m_is_overflowed( false ) {}
    void update( int c_in )
    {
        if( c_in == '-' )
            m_is_neg = true;
        else if( c_in != '+' )
        {
            const long max = ((~(static_cast<unsigned long>(0)))>>1);
            if( m_value > max / 10 )
                m_is_overflowed = true;
            m_value *= 10;
            int new_units = c_in - '0';
            if( max - m_value < new_units )
                m_is_overflowed = true;
            m_value += new_units;
        }
    }
    bool is_overflowed() const { return m_is_overflowed; }
    long get() const
    {
        if( m_is_neg )
            return -m_value;
        return m_value;
    }
};

class c_to_uns_long
{
private:
    unsigned long m_value;
    bool m_is_overflowed;

public:
    c_to_uns_long() : m_value( 0 ), m_is_overflowed( false ) {}
    void update( int c_in )
    {
        if( c_in != '+' )
        {
            const unsigned long max = ~(static_cast<unsigned long>(0));
            if( m_value > max / 10 )
                m_is_overflowed = true;
            m_value *= 10;
            unsigned int new_units = c_in - '0';
            if( max - m_value < new_units )
                m_is_overflowed = true;
            m_value += new_units;
        }
    }
    bool is_overflowed() const { return m_is_overflowed; }
    unsigned long get() const
    {
        return m_value;
    }
};

template< typename Tfloat >
class c_non_scientific_float
{
private:
    Tfloat f;

public:
    c_non_scientific_float( Tfloat f_in ) : f( f_in ) {}
    friend std::ostream & operator << ( std::ostream & r_os_in, const c_non_scientific_float & r_formatter_in )
    {
        // This method will output values less than 0 as 0.???
        if( r_formatter_in.f != 0.0 && r_formatter_in.f < 0.001 )
        {
            std::ostringstream t;       // C130507_1908 - Better handling of small valued fractional seconds
            t.precision( (sizeof( Tfloat ) == sizeof( float )) ? LMX_FLOAT_PRECISION : LMX_DOUBLE_PRECISION ); // C170614_1145
            t << (r_formatter_in.f + 1.0);  // Prevent conversion to something like 5e-5
            std::string result( t.str() );
            result[0] = '0';        // Replace initial 1 with 0
            r_os_in << result;
        }

        else
        {
            std::streamsize old_precision =
                    r_os_in.precision( (sizeof( Tfloat ) == sizeof( float )) ? LMX_FLOAT_PRECISION : LMX_DOUBLE_PRECISION ); // C170614_1145
            r_os_in << r_formatter_in.f;
            r_os_in.precision( old_precision );
        }

        return r_os_in;
    }
};

template< typename Tfloat >
c_non_scientific_float< Tfloat > non_scientific_float( Tfloat f_in )
{
    return c_non_scientific_float< Tfloat >( f_in );
}

//------------------------------------------------------------------------------
//                    Implementation of schema type classes
//------------------------------------------------------------------------------

//                         class c_big_int implementation
//------------------------------------------------------------------------------

c_big_int::c_big_int( const c_big_int & r_big_int_in )
    :
    m_value( r_big_int_in.m_value )
{
}


c_big_int::c_big_int()
{
    m_value = "0";
}

c_big_int::c_big_int( const char * p_string_in )
{
    p_init( p_string_in );
}

c_big_int::c_big_int( const std::string & r_string_in )
{
    p_init( r_string_in.c_str() );
}

c_big_int::c_big_int( long long_in )
{
    char c_buffer[20];
    LMXSTDCRT::sprintf( c_buffer, "%ld", long_in );
    p_init( c_buffer );
}

void c_big_int::p_init( const char * p_string_in )
{
    m_value.reserve( LMXSTDCRT::strlen( p_string_in ) + 1 );

    if( *p_string_in == '-' )
        m_value = '-';

    // Remove unwanted leading characters
    if( *p_string_in == '-' || *p_string_in == '+' )
        ++p_string_in;

    while( *p_string_in == '0' )
        ++p_string_in;

    if( ! xisdigit( *p_string_in ) )    // \0 or other - Value is zero
        m_value = "0";
    else
    {
        while( xisdigit( *p_string_in ) )
            m_value += *p_string_in++;
    }
}

c_big_int & c_big_int::operator =( const c_big_int & r_big_int_in )
{
    c_big_int temp( r_big_int_in );
    swap( temp );
    return *this;
}

c_big_int & c_big_int::operator =( const char * p_string_in )
{
    c_big_int temp( p_string_in );
    swap( temp );
    return *this;
}

c_big_int & c_big_int::operator =( const std::string & r_string_in )
{
    c_big_int temp( r_string_in );
    swap( temp );
    return *this;
}


c_big_int & c_big_int::operator =( long long_in )
{
    c_big_int temp( long_in );
    swap( temp );
    return *this;
}

std::string c_big_int::get_as_string() const
{
    return m_value;
}

// This routine is unable to convert to the maximum negative value
long c_big_int::get( bool * p_is_overflowed_out /*= LMXNULL*/ ) const
{
    c_to_long to_long;
    for( size_t i=0; i<m_value.size(); ++i )
        to_long.update( m_value[i] );
    if( p_is_overflowed_out )
        *p_is_overflowed_out = to_long.is_overflowed();
    return to_long.get();
}

double c_big_int::get_as_float() const
{
    size_t i=0;
    bool is_neg = false;

    if( m_value[0] == '-' )
    {
        is_neg = true;
        ++i;
    }
    else if( ! LMXSTDCRT::isdigit( m_value[0] ) )
        ++i;

    double value = 0.0;
    for( ; i<m_value.size(); ++i )
    {
        value *= 10.0;
        value += m_value[i] - '0';
    }

    if( is_neg )
        value = -value;

    return value;
}

// Note: big_int_ambiguity - In addition to allowing comparisons of the type
//          (big_int == "12") which the LMX compiler needs, it would be handy
//          to allow users to be able to do (big_int == 12).  The latter is fine
//          (because the C++ compiler can create a temporary big_int object from the 12)
//          until the user tries (big_int == 0).  Here the C++ compiler does not
//          know whether the 0 is const char * or int.  Hence providing one
//          set of comparison functions that allows const char * and another set
//          that allows int would cause ambiguity in the compilation.  This could
//          be resolved by providing both sets and requiring the user to explicitly
//          state the type to avoid the ambiguity (e.g. big_int == s_c<int>(12)),
//          but this is not attractive.  Not providing an ints version means
//          that the const char * set is called.  Without special measures this
//          results in *LMXNULL being used, which is bad.  To resolve this, if a null
//          pointer is received in the comparison functions, it is assumed that it is
//          a comparison to 0, and the pointer ammended accordingly.

bool operator == ( const c_big_int & r_lhs_in, const char * p_rhs_in )
{
    if( p_rhs_in == LMXNULL )    // See note big_int_ambiguity
        p_rhs_in = "0";

    if( *p_rhs_in == '\0' )
        return false;

    else if( (r_lhs_in.m_value[0] == '-') == (*p_rhs_in == '-') )
    {
        // Signs are the same
        if( *p_rhs_in == '-' || *p_rhs_in == '+' ) ++p_rhs_in;
        while( *p_rhs_in == '0' )
            ++p_rhs_in;   // Ignore leading zeros
        if( *p_rhs_in == '\0' )
        {
            if( r_lhs_in.m_value == "0" )
                return true;
        }
        else
        {
            if( stringncompare( r_lhs_in.m_value, p_rhs_in,
                                        r_lhs_in.m_value[0] == '-' ? 1 : 0,
                                        std::string::npos ) == 0 )
                return true;
        }
    }

    else if( *p_rhs_in == '-' && *++p_rhs_in == '0' && r_lhs_in == "0" && c_big_int( p_rhs_in ) == "0" )
        return true;    // Allow for 0 == -0 comparison

    return false;
}

bool operator != ( const c_big_int & r_lhs_in, const char * p_rhs_in )
{
    return ! ( r_lhs_in == p_rhs_in );
}

bool operator > ( const c_big_int & r_lhs_in, const char * p_rhs_in )
{
    if( p_rhs_in == 0 )   // See note big_int_ambiguity
        p_rhs_in = "0";

    if( *p_rhs_in == '\0' )
        return false;

    else if( (r_lhs_in.m_value[0] == '-') != (*p_rhs_in == '-') )
    {
        if( *p_rhs_in == '-' && *++p_rhs_in == '0' && r_lhs_in == "0" && c_big_int( p_rhs_in ) == "0" )
            return false;   // Allow for 0 == -0 comparison
        // Different sign - Positive one is bigger
        else if( r_lhs_in.m_value[0] != '-')
            return true;
    }
    else
    {
        // Signs are the same
        if( *p_rhs_in == '-' || *p_rhs_in == '+' ) ++p_rhs_in;
        while( *p_rhs_in == '0' )
            ++p_rhs_in;   // Ignore leading zeros
        if( *p_rhs_in == '\0' )   // rhs is zero
        {
            if( r_lhs_in.m_value[0] != '-' && r_lhs_in.m_value != "0" )
                return true;
        }
        else
        {
            std::string::size_type rhs_length = LMXSTDCRT::strlen( p_rhs_in );
            if( r_lhs_in.m_value[0] != '-' )
            {
                // Both positive
                if( r_lhs_in.m_value.length() > rhs_length )
                    return true;
                else if( r_lhs_in.m_value.length() < rhs_length )
                    return false;
                else
                    return r_lhs_in.m_value.compare( p_rhs_in ) > 0;
            }

            else
            {
                // Both negative
                if( r_lhs_in.m_value.length() - 1 < rhs_length )
                    return true;
                else if( r_lhs_in.m_value.length() - 1 > rhs_length )
                    return false;
                else
                    return stringncompare( r_lhs_in.m_value, p_rhs_in, 1, std::string::npos ) < 0;
            }
        }
    }
    return false;
}

bool operator >= ( const c_big_int & r_lhs_in, const char * p_rhs_in )
{
    return r_lhs_in == p_rhs_in || r_lhs_in > p_rhs_in;
}

bool operator < ( const c_big_int & r_lhs_in, const char * p_rhs_in )
{
    return ! ( r_lhs_in == p_rhs_in ) && ! ( r_lhs_in > p_rhs_in );
}

bool operator <= ( const c_big_int & r_lhs_in, const char * p_rhs_in )
{
    return ! ( r_lhs_in > p_rhs_in );
}

bool operator == ( const c_big_int & r_lhs_in, const std::string & r_value_in )
{
    return r_lhs_in == r_value_in.c_str();
}

bool operator != ( const c_big_int & r_lhs_in, const std::string & r_value_in )
{
    return r_lhs_in != r_value_in.c_str();
}

bool operator > ( const c_big_int & r_lhs_in, const std::string & r_value_in )
{
    return r_lhs_in > r_value_in.c_str();
}

bool operator >= ( const c_big_int & r_lhs_in, const std::string & r_value_in )
{
    return r_lhs_in >= r_value_in.c_str();
}

bool operator < ( const c_big_int & r_lhs_in, const std::string & r_value_in )
{
    return r_lhs_in < r_value_in.c_str();
}

bool operator <= ( const c_big_int & r_lhs_in, const std::string & r_value_in )
{
    return r_lhs_in <= r_value_in.c_str();
}

bool operator == ( const std::string & r_lhs_in, const c_big_int & r_rhs_in )
{
    return r_rhs_in == r_lhs_in;
}

bool operator != ( const std::string & r_lhs_in, const c_big_int & r_rhs_in )
{
    return r_rhs_in != r_lhs_in;
}

bool operator > ( const std::string & r_lhs_in, const c_big_int & r_rhs_in )
{
    return r_rhs_in < r_lhs_in;
}

bool operator >= ( const std::string & r_lhs_in, const c_big_int & r_rhs_in )
{
    return r_rhs_in <= r_lhs_in;
}

bool operator < ( const std::string & r_lhs_in, const c_big_int & r_rhs_in )
{
    return r_rhs_in > r_lhs_in;
}

bool operator <= ( const std::string & r_lhs_in, const c_big_int & r_rhs_in )
{
    return r_rhs_in >= r_lhs_in;
}

bool operator == ( const char * p_lhs_in, const c_big_int & r_rhs_in )
{
    return r_rhs_in == p_lhs_in;
}

bool operator != ( const char * p_lhs_in, const c_big_int & r_rhs_in )
{
    return r_rhs_in != p_lhs_in;
}

bool operator > ( const char * p_lhs_in, const c_big_int & r_rhs_in )
{
    return r_rhs_in < p_lhs_in;
}

bool operator >= ( const char * p_lhs_in, const c_big_int & r_rhs_in )
{
    return r_rhs_in <= p_lhs_in;
}

bool operator < ( const char * p_lhs_in, const c_big_int & r_rhs_in )
{
    return r_rhs_in > p_lhs_in;
}

bool operator <= ( const char * p_lhs_in, const c_big_int & r_rhs_in )
{
    return r_rhs_in >= p_lhs_in;
}

bool operator == ( const c_big_int & r_lhs_in, const c_big_int & r_rhs_in )
{
    return r_lhs_in.m_value == r_rhs_in.m_value.c_str();
}

bool operator != ( const c_big_int & r_lhs_in, const c_big_int & r_rhs_in )
{
    return ! (r_lhs_in.m_value == r_rhs_in.m_value);
}

bool operator > ( const c_big_int & r_lhs_in, const c_big_int & r_rhs_in )
{
    return r_lhs_in > r_rhs_in.m_value.c_str();
}

bool operator >= ( const c_big_int & r_lhs_in, const c_big_int & r_rhs_in )
{
    return r_lhs_in == r_rhs_in || r_lhs_in > r_rhs_in;
}

bool operator < ( const c_big_int & r_lhs_in, const c_big_int & r_rhs_in )
{
    return ! ( r_lhs_in == r_rhs_in ) && ! ( r_lhs_in > r_rhs_in );
}

bool operator <= ( const c_big_int & r_lhs_in, const c_big_int & r_rhs_in )
{
    return ! ( r_lhs_in > r_rhs_in );
}

std::ostream & operator << ( std::ostream & r_os_in, const c_big_int & r_rhs_in )
{
    r_os_in << r_rhs_in.m_value;
    return r_os_in;
}

//                    class c_decimal implementation
//------------------------------------------------------------------------------

c_decimal::c_decimal() :
        m_is_neg( false )
{
}

c_decimal::c_decimal( const c_decimal & r_in ) :
        m_is_neg( r_in.m_is_neg ),
        m_left( r_in.m_left ),
        m_right( r_in.m_right )
{
}

c_decimal::c_decimal( long v_in, int scale_in /*= 0*/ )
{
    m_right.erase();
    if( v_in < 0 )
    {
        m_is_neg = true;
        v_in = -v_in;
    }
    else
        m_is_neg = false;

    if( scale_in <= 0 )
    {
        scale_in = -scale_in;
        char c_buffer[30];
        LMXSTDCRT::sprintf( c_buffer, "%lu", v_in );
        size_t valid_chars = LMXSTDCRT::strlen( c_buffer );
        for( int i=0; i < scale_in; ++i )
            c_buffer[valid_chars + i] = '0';
        c_buffer[valid_chars + scale_in] = '\0';
        m_left = c_buffer;
    }
    else
    {
        unsigned long scaler = 1;
        unsigned long fractional = 0;
        for( int i=0; i<scale_in; ++i )
        {
            int modulo = v_in % 10;
            fractional += scaler * modulo;
            scaler *= 10;
            v_in /= 10;
        }
        m_left =  v_in;
        set_fractional( fractional, scale_in );
    }
}

c_decimal::c_decimal( const char * p_in_in )
{
    p_init( p_in_in );
}

c_decimal::c_decimal( const std::string & r_in )
{
    p_init( r_in.c_str() );
}

c_decimal::c_decimal( double value_in ) // Added v6.3 - C130115_0930
{
    char c_temp[DBL_MAX_10_EXP+1+DBL_DIG+2+1]; // +1 extra left space, +2 sign and ., +1 \0
    LMXSTDCRT::sprintf( c_temp, "%f", value_in );
    p_init( c_temp, INCLUDE_SYSTEM_LOCALE );  // C150320_1524
    p_cull_right_hand_zeros();  // C140606_1347
}

c_decimal::c_decimal( double value_in, unsigned int precision_in )
{
    if( precision_in > DBL_DIG )
        precision_in = DBL_DIG;
    char c_converter[30];
    LMXSTDCRT::sprintf( c_converter, "%%.%uf", precision_in );
    char c_temp[DBL_MAX_10_EXP+1+DBL_DIG+2+1]; // +1 extra left space, +2 sign and ., +1 \0
    LMXSTDCRT::sprintf( c_temp, c_converter, value_in );
    p_init( c_temp, INCLUDE_SYSTEM_LOCALE );  // C150320_1524
    p_cull_right_hand_zeros();  // C140918_1252
}

void c_decimal::p_init( const char * p_in_in, e_decimal_separator_handling decimal_separator_handling /*= DOT_ONLY*/ )   // C150320_1524
{
    get_sign( p_in_in, m_is_neg );
    m_left = p_in_in;    // This will suck in digits up to the '.', if any
    skip_digits( p_in_in );       // May not have leading digits e.g. ".7"
    if( p_skip_decimal_separator( p_in_in, decimal_separator_handling ) )  // C150320_1524
        while( xisdigit( *p_in_in ) )
            m_right += *p_in_in++;
}

bool c_decimal::p_skip_decimal_separator( const char * & rp_s_in, e_decimal_separator_handling decimal_separator_handling )   // C150320_1524
{
    return skip_const( rp_s_in, "." ) ||
                (decimal_separator_handling == INCLUDE_SYSTEM_LOCALE &&
                    skip_const( rp_s_in, LMXSTDCRT::localeconv()->decimal_point ) );
}

void c_decimal::p_cull_right_hand_zeros()   // C140606_1347
{
    size_t last_zero_index = m_right.size();
    while( last_zero_index > 0 && m_right[last_zero_index - 1] == '0' )
            --last_zero_index;
    if( last_zero_index < m_right.size() )
        m_right.erase( last_zero_index );
}

bool c_decimal::is_valid( const std::string & r_value_in )
{
    // [+-]?\d*(.\d*)?  .5 seems valid
    const char * p_s = r_value_in.c_str();
    if( skip_opt_sign( p_s ) && skip_decimal( p_s ) && *p_s == '\0' )
        return true;
    return false;
}

c_decimal & c_decimal::operator = ( const char * p_v_in )
{
    c_decimal temp( p_v_in );
    swap( temp );
    return *this;
}

c_decimal & c_decimal::operator = ( const c_decimal & r_v_in )
{
    c_decimal temp( r_v_in );
    swap( temp );
    return *this;
}

c_decimal & c_decimal::operator = ( double value_in )
{
    set( value_in );
    return *this;
}

void c_decimal::swap( c_decimal & r_v_in )
{
    std::swap( m_is_neg, r_v_in.m_is_neg );
    m_left.swap( r_v_in.m_left );
    m_right.swap( r_v_in.m_right );
}

std::string c_decimal::get_as_string() const
{
    return as_string( *this );
}

long c_decimal::get_integral( bool * p_is_overflowed_out /*= LMXNULL*/ ) const
{
    long result = m_left.get( p_is_overflowed_out );
    if( m_is_neg )
        return -result;
    return result;
}

void c_decimal::set_integral( long v_in )
{
    if( v_in < 0 )
    {
        m_is_neg = true;
        v_in = -v_in;
    }
    else
        m_is_neg = false;
    m_left = v_in;
}

unsigned long c_decimal::get_fractional( int precision_in, bool * p_is_overflowed_out /*= LMXNULL*/ ) const
{
    c_to_uns_long to_long;
    for( size_t i=0; precision_in > 0 && i<m_right.size(); ++i, --precision_in )
        to_long.update( m_right[i] );
    for( ; precision_in > 0; --precision_in )
        to_long.update( '0' );
    if( p_is_overflowed_out )
        *p_is_overflowed_out = to_long.is_overflowed();
    return to_long.get();
}

void c_decimal::set_fractional( unsigned long v_in, unsigned int precision_in )
{
    // If a_v = 5 & a_precision = 2, want to set right to "05"
    char c_buffer[20];
    LMXSTDCRT::sprintf( c_buffer, "%lu", v_in );
    size_t valid_chars = LMXSTDCRT::strlen( c_buffer );
    if( precision_in >= valid_chars )
    {
        m_right.erase();
        for( unsigned int i=0; i < precision_in - valid_chars; ++i )
            m_right += '0';
        m_right += c_buffer;
    }
    else
    {
        c_buffer[precision_in] = '\0';
        m_right.erase();
        m_right = c_buffer;
    }
}

long c_decimal::get_scaled( int scale_in, bool * p_is_overflowed_out /*= LMXNULL*/ ) const
{
    std::string left( m_left.get_as_string() );
    c_to_long to_long;
    if( m_is_neg )
        to_long.update( '-' );
    if( scale_in <= 0 )
    {
        for( size_t i=0; i<left.size() - (-scale_in); ++i )
            to_long.update( left[i] );
    }
    else
    {
        size_t i;
        for( i=0; i<left.size(); ++i )
            to_long.update( left[i] );
        for( i=0; scale_in > 0 && i<m_right.size(); ++i, --scale_in )
            to_long.update( m_right[i] );
        for( ; scale_in > 0; --scale_in )
            to_long.update( '0' );
    }
    if( p_is_overflowed_out )
        *p_is_overflowed_out = to_long.is_overflowed();
    return to_long.get();
}

void c_decimal::set_scaled( long v_in, int scale_in /*= 0*/ )
{
    c_decimal temp( v_in, scale_in );
    swap( temp );
}

double c_decimal::get_as_float() const  // v6.3 - algorithm modified C130115_0930
{
    double value = m_left.get_as_float();
    if( ! m_right.empty() )
    {
        double scale = 1.0;
        for( size_t i=0; i<m_right.size(); ++i )
        {
            value *= 10.0;
            value += m_right[i] - '0';
            scale *= 10.0;
        }
        value /= scale;
    }
    if( m_is_neg )
        value = -value;
    return value;
}

void c_decimal::set( double value_in, unsigned int precision_in /*= DBL_DIG*/ )
{
    c_decimal temp( value_in, precision_in );
    swap( temp );
}

bool operator == ( const c_decimal & r_lhs_in, const c_decimal & r_rhs_in )
{
    return r_lhs_in.m_is_neg == r_rhs_in.m_is_neg &&
            r_lhs_in.m_left == r_rhs_in.m_left &&
            c_decimal::p_fraction_comp( r_lhs_in.m_right, r_rhs_in.m_right ) == 0;
}

bool operator != ( const c_decimal & r_lhs_in, const c_decimal & r_rhs_in )
{
    return ! ( r_lhs_in == r_rhs_in );
}

bool operator > ( const c_decimal & r_lhs_in, const c_decimal & r_rhs_in )
{
    if( r_lhs_in.m_is_neg == r_rhs_in.m_is_neg )
    {
        // Same sign
        if( r_lhs_in.m_is_neg )
            return r_lhs_in.m_left < r_rhs_in.m_left ||         // v4.2 - gcc_433_brackets
                    (r_lhs_in.m_left == r_rhs_in.m_left && c_decimal::p_fraction_comp( r_lhs_in.m_right, r_rhs_in.m_right ) < 0);
        else
            return r_lhs_in.m_left > r_rhs_in.m_left ||         // v4.2 - gcc_433_brackets
                    (r_lhs_in.m_left == r_rhs_in.m_left && c_decimal::p_fraction_comp( r_lhs_in.m_right, r_rhs_in.m_right ) > 0);
    }

    else if( ! r_lhs_in.m_is_neg )
        return true;

    return false;
}

bool operator >= ( const c_decimal & r_lhs_in, const c_decimal & r_rhs_in )
{
    return r_lhs_in == r_rhs_in || r_lhs_in > r_rhs_in;
}

bool operator < ( const c_decimal & r_lhs_in, const c_decimal & r_rhs_in )
{
    return ! ( r_lhs_in == r_rhs_in ) && ! ( r_lhs_in > r_rhs_in );
}

bool operator <= ( const c_decimal & r_lhs_in, const c_decimal & r_rhs_in )
{
    return ! ( r_lhs_in > r_rhs_in );
}

// Returns 1 if lhs > rhs, 0 if lhs == rhs, -1 if lhs < rhs
int c_decimal::p_fraction_comp( const std::string & r_lhs_in, const std::string & r_rhs_in )
{
    const char * p_left = r_lhs_in.c_str();
    const char * p_right = r_rhs_in.c_str();

    for(;;)
    {
        if( *p_right == '\0' )
        {
            while( *p_left == '0' )
                ++p_left;
            if( *p_left == '\0' )
                return 0;
            return 1;   // C100415_1549 - Changed sign
        }
        else if( *p_left == '\0' )
        {
            while( *p_right == '0' )
                ++p_right;
            if( *p_right == '\0' )
                return 0;
            return -1;  // C100415_1549 - Changed sign
        }
        else if( *p_left > *p_right )
            return 1;
        else if( *p_left < *p_right )
            return -1;
        ++p_left; ++p_right;
    }
}

std::ostream & operator << ( std::ostream & r_os_in, const c_decimal & r_dec_in )
{
    if( r_dec_in.m_is_neg )
        r_os_in << '-';
    r_os_in << r_dec_in.m_left;     // C140606_1441 - Always output left hand side, even if zero.
    if( r_dec_in.m_right != "" )
        r_os_in << '.' << r_dec_in.m_right;
    return r_os_in;
}

//                    class c_duration implementation
//------------------------------------------------------------------------------

// [+-]?P(\d+Y)?(\d+M)?(\d+D)?(T(\d+H)?(\d+M)?(\d+(.\d*)?S)?)?
// In comparing duration values with minInclusive, minExclusive,
// -maxInclusive and maxExclusive facet values indeterminate comparisons
// -should be considered as "false".
// Y1!=Y2: (Y + M/12)*366 + maxmon[M%12] + D < (Y + M/12)*365 + minmon[M%12] + D
// Only do above if (Y1+M1/12) and (Y2+M2/12) and M1 and M2 are different.
// Y1==Y2: (M/12)*366 + maxmon[M%12] + D < (M/12)*365 + minmon[M%12] + D
// Y1==Y2, M1==M2: D < D

const unsigned int c_duration::m_day_mons_mins[] = { 0, 28, 59, 89, 120, 150, 181, 212, 242, 273, 303, 334 };
const unsigned int c_duration::m_day_mons_maxs[] = { 0, 31, 62, 92, 123, 153, 184, 215, 245, 276, 306, 337 };

c_duration::c_duration() :
            m_is_neg( false ),
            m_years( 0 ),
            m_months( 0 ),
            m_days( 0 ),
            m_hours( 0 ),
            m_mins( 0 ),
            m_secs( 0.0 )
{
}

c_duration::c_duration( const std::string & r_s_in )
{
    operator = ( r_s_in.c_str() );
}

c_duration::c_duration( const char * p_s_in )
{
    operator = ( p_s_in );
}

bool c_duration::p_is_indeterminate( const c_duration & r_rhs_in ) const
{
    if( m_is_neg != r_rhs_in.m_is_neg )
        return false;
    if( m_months % 12 == r_rhs_in.m_months % 12 ) // Only unequal months cause indeterminancy
        return false;
    c_duration left( *this );
    c_duration right( r_rhs_in );
    left.normalize();
    right.normalize();
    int left_year_offset;
    int right_year_offset;
    if( left.m_years > right.m_years )
    {
        left_year_offset = left.m_years - right.m_years;
        right_year_offset = 0;
    }
    else
    {
        left_year_offset = 0;
        right_year_offset = right.m_years - left.m_years;
    }
    // Only consider whole (normalized) days to determine determinancy and ignore sign
    int left_max = left_year_offset*366 + m_day_mons_maxs[left.m_months] + left.m_days;
    int left_min = left_year_offset*365 + m_day_mons_mins[left.m_months] + left.m_days;
    int right_max = right_year_offset*366 + m_day_mons_maxs[right.m_months] + right.m_days;
    int right_min = right_year_offset*365 + m_day_mons_mins[right.m_months] + right.m_days;
    if( left_max > right_min && left_min < right_max )
        return true;
    return false;
}

bool c_duration::p_is_equal( const c_duration & r_rhs_in ) const
{
    c_duration left( *this );
    c_duration right( r_rhs_in );
    left.normalize();
    right.normalize();
    if( left.m_is_neg == right.m_is_neg &&
            left.m_years == right.m_years &&
            left.m_months == right.m_months &&
            left.m_days == right.m_days &&
            left.m_hours == right.m_hours &&
            left.m_mins == right.m_mins &&
            left.m_secs == right.m_secs )
        return true;
    return false;
}

bool c_duration::p_is_greater_than( const c_duration & r_rhs_in ) const
{
    // Assumes that the input is not indeterminate
    c_duration left( *this );
    c_duration right( r_rhs_in );
    left.normalize();
    right.normalize();
    if( left.m_years > right.m_years ||         // v4.2 - gcc_433_brackets
        (left.m_years == right.m_years && (left.m_months > right.m_months ||
            (left.m_months == right.m_months && (left.m_days > right.m_days ||
                (left.m_days == right.m_days && (left.m_hours > right.m_hours ||
                    (left.m_hours == right.m_hours && (left.m_mins > right.m_mins ||
                        (left.m_mins == right.m_mins && left.m_secs > right.m_secs))))))))) )
        return true;
    return false;
}

void c_duration::normalize()
{
    m_mins += static_cast<int>(m_secs / 60);
    m_secs -= static_cast<int>(m_secs/60) * 60; // secs %= 60 for float
    m_hours += m_mins / 60;
    m_mins %= 60;
    m_days += m_hours / 24;
    m_hours %= 24;
    // We can not normalize days to months
    m_years += m_months / 12;
    m_months %= 12;
}


bool c_duration::is_valid( const char * p_s_in )
{
    // [+-]?P(\d+Y)?(\d+M)?(\d+D)?(T(\d+H)?(\d+M)?(\d+(.\d*)?S)?)?
    bool got_digits = false;
    if( skip_opt_sign( p_s_in ) && skip_const( p_s_in, "P" ) )
    {
        if( skip_digits( p_s_in ) )
            got_digits = true;
        if( got_digits && skip_const( p_s_in, "Y" ) )
            got_digits = skip_digits( p_s_in ) != 0;
        if( got_digits && skip_const( p_s_in, "M" ) )
            got_digits = skip_digits( p_s_in ) != 0;
        if( got_digits && skip_const( p_s_in, "D" ) )
            got_digits = false;
        if( *p_s_in == '\0' )
        {
            if( ! got_digits )
                return true;
        }
        else if( skip_const( p_s_in, "T" ) && *p_s_in != '\0' ) // Must be digits after T
        {
            if( skip_digits( p_s_in ) )
                got_digits = true;
            if( got_digits && skip_const( p_s_in, "H" ) )
                got_digits = skip_digits( p_s_in ) != 0;
            if( got_digits && skip_const( p_s_in, "M" ) )
                got_digits = skip_digits( p_s_in ) != 0;
            if( skip_const( p_s_in, "." ) )   // Needn't have collected any digits prior to .
            {
                if( *p_s_in == '\0' )
                    return false;
                else if( skip_digits( p_s_in ) || got_digits )
                    got_digits = true;
            }
            if( got_digits && skip_const( p_s_in, "S" ) )
                got_digits = false;
            if( ! got_digits && *p_s_in == '\0' )
                return true;
        }
    }
    return false;
}

c_duration & c_duration::operator = ( const char * p_s_in )
{
    // Assumes that is_valid has already been successful
    // [+-]?P(\d+Y)?(\d+M)?(\d+D)?(T(\d+H)?(\d+M)?(\d+(.\d*)?S)?)?
    m_is_neg = false;
    m_years = m_months = m_days = m_hours = m_mins = 0;
    m_secs = 0.0;
    get_sign( p_s_in, m_is_neg );
    if( skip_const( p_s_in, "P" ) )
    {
        bool got_digits = false;
        const char * p_last_digits = p_s_in;
        if( skip_digits( p_s_in ) )
            got_digits = true;
        if( got_digits && skip_const( p_s_in, "Y" ) )
        {
            m_years = LMXSTDCRT::atoi( p_last_digits );
            p_last_digits = p_s_in;
            got_digits = skip_digits( p_s_in ) != 0;
        }
        if( got_digits && skip_const( p_s_in, "M" ) )
        {
            m_months = LMXSTDCRT::atoi( p_last_digits );
            p_last_digits = p_s_in;
            got_digits = skip_digits( p_s_in ) != 0;
        }
        if( got_digits && skip_const( p_s_in, "D" ) )
        {
            m_days = LMXSTDCRT::atoi( p_last_digits );
            got_digits = false;
        }
        if( skip_const( p_s_in, "T" ) )   // Must be digits after T
        {
            p_last_digits = p_s_in;
            if( skip_digits( p_s_in ) )
                got_digits = true;
            if( got_digits && skip_const( p_s_in, "H" ) )
            {
                m_hours = LMXSTDCRT::atoi( p_last_digits );
                p_last_digits = p_s_in;
                got_digits = skip_digits( p_s_in ) != 0;
            }
            if( got_digits && skip_const( p_s_in, "M" ) )
            {
                m_mins = LMXSTDCRT::atoi( p_last_digits );
                p_last_digits = p_s_in;
                got_digits = skip_digits( p_s_in ) != 0;
            }
            if( skip_const( p_s_in, "." ) )   // Needn't have collected any digits prior to .
            {
                if( skip_digits( p_s_in ) || got_digits )
                    got_digits = true;
            }
            if( got_digits && skip_const( p_s_in, "S" ) )
                m_secs = static_cast<tlmx_precision_seconds>( LMXSTDCRT::atof( p_last_digits ) );
        }
    }
    return *this;
}

std::string c_duration::get_as_string() const
{
    return as_string( *this );
}

bool operator == ( const c_duration & r_lhs_in, const c_duration & r_rhs_in )
{
    if( r_lhs_in.p_is_indeterminate( r_rhs_in ) )
        return false;
    return r_lhs_in.p_is_equal( r_rhs_in );
}

bool operator != ( const c_duration & r_lhs_in, const c_duration & r_rhs_in )
{
    return ! ( r_lhs_in == r_rhs_in );
}

bool operator > ( const c_duration & r_lhs_in, const c_duration & r_rhs_in )
{
    if( r_lhs_in.p_is_indeterminate( r_rhs_in ) )
        return false;
    return r_lhs_in.p_is_greater_than( r_rhs_in );
}

bool operator >= ( const c_duration & r_lhs_in, const c_duration & r_rhs_in )
{
    if( r_lhs_in.p_is_indeterminate( r_rhs_in ) )
        return false;
    return r_lhs_in.p_is_greater_than( r_rhs_in ) || r_lhs_in.p_is_equal( r_rhs_in );
}

bool operator < ( const c_duration & r_lhs_in, const c_duration & r_rhs_in )
{
    if( r_lhs_in.p_is_indeterminate( r_rhs_in ) )
        return false;
    return ! r_lhs_in.p_is_equal( r_rhs_in ) && ! r_lhs_in.p_is_greater_than( r_rhs_in );
}

bool operator <= ( const c_duration & r_lhs_in, const c_duration & r_rhs_in )
{
    if( r_lhs_in.p_is_indeterminate( r_rhs_in ) )
        return false;
    return ! r_lhs_in.p_is_greater_than( r_rhs_in );
}

std::ostream & operator << ( std::ostream & r_os_in, const c_duration & r_d_in )
{
    // [+-]?P(\d+Y)?(\d+M)?(\d+D)?(T(\d+H)?(\d+M)?(\d+(.\d*)?S)?)?
    lmx_assert( r_d_in.m_secs >= 0.0 );
    if( r_d_in.m_is_neg )
        r_os_in << '-';
    r_os_in << 'P';
    if( r_d_in.m_years )
        r_os_in << r_d_in.m_years << 'Y';
    if( r_d_in.m_months )
        r_os_in << r_d_in.m_months << 'M';
    if( r_d_in.m_days )
        r_os_in << r_d_in.m_days << 'D';
    if( r_d_in.m_hours || r_d_in.m_mins || r_d_in.m_secs )
    {
        r_os_in << 'T';
        if( r_d_in.m_hours )
            r_os_in << r_d_in.m_hours << 'H';
        if( r_d_in.m_mins )
            r_os_in << r_d_in.m_mins << 'M';
        if( r_d_in.m_secs )
        {
            r_os_in << non_scientific_float( r_d_in.m_secs ) << 'S';
        }
    }
    return r_os_in;
}

//                class c_datetime_base helper implementation
//------------------------------------------------------------------------------

c_datetime_base::c_datetime_base()
    :
    m_year( 1 ),     // Year 0 is illegal
    m_month( 0 ),
    m_day( 0 ),
    m_hours( 0 ),
    m_mins( 0 ),
    m_secs( 0.0 ),
    m_off_mins( 0 ),  // Default is 'local' time
    m_is_utc( false )
{
}

bool c_datetime_base::is_equal( const c_datetime_base & r_rhs_in ) const
{
    if( m_year == r_rhs_in.m_year && m_month == r_rhs_in.m_month && m_day == r_rhs_in.m_day &&
            m_hours == r_rhs_in.m_hours && m_mins == r_rhs_in.m_mins && m_secs == r_rhs_in.m_secs )
        return true;
    return false;
}

bool c_datetime_base::is_greater_than( const c_datetime_base & r_rhs_in ) const
{
    if( m_year > r_rhs_in.m_year ||       // v4.2 - gcc_433_brackets
        (m_year == r_rhs_in.m_year && (m_month > r_rhs_in.m_month ||
            (m_month == r_rhs_in.m_month && (m_day > r_rhs_in.m_day ||
                (m_day == r_rhs_in.m_day && (m_hours > r_rhs_in.m_hours ||
                    (m_hours == r_rhs_in.m_hours && (m_mins > r_rhs_in.m_mins ||
                        (m_mins == r_rhs_in.m_mins && m_secs > r_rhs_in.m_secs ))))))))) )
        return true;
    return false;
}

bool c_datetime_base::is_valid_tz( const char * & rp_s_in )
{
    if( *rp_s_in == '\0' )
        return true;
    else if( skip_const( rp_s_in, "Z" ) )
    {
        if( *rp_s_in == '\0' )
            return true;
    }
    else if( skip_sign( rp_s_in ) &&
                skip_digits( rp_s_in ) == 2 &&
                skip_const( rp_s_in, ":" ) &&
                skip_digits( rp_s_in ) == 2 &&
                *rp_s_in == '\0' )
        return true;
    return false;
}

void c_datetime_base::set_tz( const char * p_s_in )
{
    bool is_off_neg;
    int off_hours;
    int off_mins;
    if( *p_s_in == '\0' )
    {
        m_is_utc = false;
        m_off_mins = 0;
    }
    else if( skip_const( p_s_in, "Z" ) && *p_s_in == '\0' )
    {
        m_is_utc = true;
        m_off_mins = 0;
    }
    else if( get_sign( p_s_in, is_off_neg ) &&
                get_digits( p_s_in, off_hours ) == 2 &&
                skip_const( p_s_in, ":" ) &&
                get_digits( p_s_in, off_mins ) == 2 &&
                *p_s_in == '\0' )
    {
        m_is_utc = false;
        m_off_mins = off_hours * 60 + off_mins;
        if( is_off_neg )
            m_off_mins = -m_off_mins;
    }
}

std::ostream & c_datetime_base::output_seconds( std::ostream & r_os_in ) const  // C130425_1106
{
    if( m_secs < 10.0 )  // Ensure leading zero for small seconds
        r_os_in << "0";
    r_os_in << non_scientific_float( m_secs );
    return r_os_in;
}

std::ostream & c_datetime_base::output_tz( std::ostream & r_os_in ) const
{
    if( ! m_is_utc && m_off_mins == 0 )
        {}  // Do nothing
    else if( m_is_utc )
        r_os_in << 'Z';
    else    // ! is_utc && off_mins != 0
    {
        int abs_off_mins = m_off_mins;
        if( m_off_mins < 0 )
        {
            r_os_in << '-';
            abs_off_mins = -abs_off_mins;
        }
        else
            r_os_in << '+';
        r_os_in.fill( '0' );
        r_os_in << std::setw(2) << abs_off_mins / 60 << ':' <<
                std::setw(2) << abs_off_mins % 60;
    }
    return r_os_in;
}

//                   class c_datetime implementation
//------------------------------------------------------------------------------
    // [+/-]CCYY-MM-DDThh:mm:ss[.ss][Z/((+/-)hh:mm]

c_datetime::c_datetime()
{
}

// c_datetime::c_datetime( const c_datetime & ar_v )    // Default constructor OK

c_datetime::c_datetime( const char * p_v_in )
{
    operator=( p_v_in );
}

c_datetime::c_datetime( const std::string & r_v_in )
{
    operator=( r_v_in.c_str() );
}

bool c_datetime::is_valid( const std::string & r_v_in )
{
    // [+/-]CCYY-MM-DDThh:mm:ss[.ss][Z/((+/-)hh:mm]
    const char * p_s = r_v_in.c_str();
    skip_sign( p_s );
    if( skip_digits( p_s ) >= 4 &&         // Year
             skip_const( p_s, "-" )  &&
             skip_digits( p_s ) == 2 &&    // Month
             skip_const( p_s, "-" ) &&
             skip_digits( p_s ) == 2 &&    // Day
             skip_const( p_s, "T" ) &&
             skip_digits( p_s ) == 2 &&    // Hours
             skip_const( p_s, ":" ) &&
             skip_digits( p_s ) == 2 &&    // Mins
             skip_const( p_s, ":" ) &&
             skip_digits( p_s ) == 2 )     // Secs integer part
    {
        if( skip_const( p_s, "." ) )
            skip_digits( p_s );  // Secs fractional part
        if( is_valid_tz( p_s ) && *p_s == '\0' )
            return true;
    }
    return false;
}

// c_datetime & c_datetime::operator = ( const c_datetime & ar_v )  // Default assign OK

c_datetime & c_datetime::operator = ( const char * p_v_in )
{
    bool is_neg = false;
    get_sign( p_v_in, is_neg );
    if( get_digits( p_v_in, m_year ) >= 4 &&
             skip_const( p_v_in, "-" ) &&
             get_digits( p_v_in, m_month ) == 2 &&
             skip_const( p_v_in, "-" ) &&
             get_digits( p_v_in, m_day ) == 2 &&
             skip_const( p_v_in, "T" ) &&
             get_digits( p_v_in, m_hours ) == 2 &&
             skip_const( p_v_in, ":" ) &&
             get_digits( p_v_in, m_mins ) == 2 &&
             skip_const( p_v_in, ":" ) &&
             get_decimal( p_v_in, m_secs ) >= 2 )
    {
        if( is_neg )
            m_year = -m_year;
        set_tz( p_v_in );
    }
    return *this;
}

c_datetime & c_datetime::operator = ( const std::string & r_v_in )
{
    return operator = ( r_v_in.c_str() );
}

std::string c_datetime::get_as_string() const
{
    return as_string( *this );
}

bool operator == ( const c_datetime & r_lhs_in, const c_datetime & r_rhs_in )
{
    return r_lhs_in.is_equal( r_rhs_in );
}

bool operator != ( const c_datetime & r_lhs_in, const c_datetime & r_rhs_in )
{
    return ! ( r_lhs_in == r_rhs_in );
}

bool operator > ( const c_datetime & r_lhs_in, const c_datetime & r_rhs_in )
{
    return r_lhs_in.is_greater_than( r_rhs_in );
}

bool operator >= ( const c_datetime & r_lhs_in, const c_datetime & r_rhs_in )
{
    return r_lhs_in == r_rhs_in || r_lhs_in > r_rhs_in;
}

bool operator < ( const c_datetime & r_lhs_in, const c_datetime & r_rhs_in )
{
    return ! ( r_lhs_in == r_rhs_in ) && ! ( r_lhs_in > r_rhs_in );
}

bool operator <= ( const c_datetime & r_lhs_in, const c_datetime & r_rhs_in )
{
    return ! ( r_lhs_in > r_rhs_in );
}

std::ostream & operator << ( std::ostream & r_os_in, const c_datetime & r_v_in )
{
    // [+/-]CCYY-MM-DDThh:mm:ss[.ss][Z/((+/-)hh:mm]
    lmx_assert( r_v_in.m_secs >= 0.0 );
    r_os_in.fill( '0' );
    r_os_in << std::setw(4) << r_v_in.m_year <<
            '-' <<
            std::setw(2) << r_v_in.m_month <<
            '-' <<
            std::setw(2) << r_v_in.m_day <<
            'T' <<
            std::setw(2) << r_v_in.m_hours <<
            ':' <<
            std::setw(2) << r_v_in.m_mins <<
            ':';
    r_v_in.output_seconds( r_os_in );   // C130425_1106
    r_v_in.output_tz( r_os_in );
    return r_os_in;
}

//                  class c_time implementation
//------------------------------------------------------------------------------

c_time::c_time()
{
}

// c_time::c_time( const c_time & ar_v )    // Default constructor OK

c_time::c_time( const char * p_v_in )
{
    operator=( p_v_in );
}

c_time::c_time( const std::string & r_v_in )
{
    operator=( r_v_in.c_str() );
}

bool c_time::is_valid( const std::string & r_v_in )
{
    // hh:mm:ss[.ss][Z/((+/-)hh:mm]
    const char * p_s = r_v_in.c_str();
    skip_sign( p_s );
    if( skip_digits( p_s ) == 2 &&         // Hours
             skip_const( p_s, ":" ) &&
             skip_digits( p_s ) == 2 &&    // Mins
             skip_const( p_s, ":" ) &&
             skip_digits( p_s ) == 2 )     // Secs integer part
    {
        if( skip_const( p_s, "." ) )
            skip_digits( p_s );  // Secs fractional part
        if( is_valid_tz( p_s ) )
            return true;
    }
    return false;
}

// c_time & c_time::operator = ( const c_time & ar_v )  // Default assign OK

c_time & c_time::operator = ( const char * p_v_in )
{
    // hh:mm:ss[.ss][Z/((+/-)hh:mm]
    if( get_digits( p_v_in, m_hours ) == 2 &&
             skip_const( p_v_in, ":" ) &&
             get_digits( p_v_in, m_mins ) == 2 &&
             skip_const( p_v_in, ":" ) &&
             get_decimal( p_v_in, m_secs ) >= 2 )
    {
        set_tz( p_v_in );
    }
    return *this;
}

c_time & c_time::operator = ( const std::string & r_v_in )
{
    return operator = ( r_v_in.c_str() );
}

std::string c_time::get_as_string() const
{
    return as_string( *this );
}

bool operator == ( const c_time & r_lhs_in, const c_time & r_rhs_in )
{
    return r_lhs_in.is_equal( r_rhs_in );
}

bool operator != ( const c_time & r_lhs_in, const c_time & r_rhs_in )
{
    return ! ( r_lhs_in == r_rhs_in );
}

bool operator > ( const c_time & r_lhs_in, const c_time & r_rhs_in )
{
    return r_lhs_in.is_greater_than( r_rhs_in );
}

bool operator >= ( const c_time & r_lhs_in, const c_time & r_rhs_in )
{
    return r_lhs_in == r_rhs_in || r_lhs_in > r_rhs_in;
}

bool operator < ( const c_time & r_lhs_in, const c_time & r_rhs_in )
{
    return ! ( r_lhs_in == r_rhs_in ) && ! ( r_lhs_in > r_rhs_in );
}

bool operator <= ( const c_time & r_lhs_in, const c_time & r_rhs_in )
{
    return ! ( r_lhs_in > r_rhs_in );
}

std::ostream & operator << ( std::ostream & r_os_in, const c_time & r_v_in )
{
    lmx_assert( r_v_in.m_secs >= 0.0 );
    r_os_in.fill( '0' );
    r_os_in << std::setw(2) << r_v_in.m_hours <<
            ':' <<
            std::setw(2) << r_v_in.m_mins <<
            ':';
    r_v_in.output_seconds( r_os_in );   // C130425_1106
    r_v_in.output_tz( r_os_in );
    return r_os_in;
}

//                   class c_date implementation
//------------------------------------------------------------------------------

c_date::c_date()
{
}

// c_date::c_date( const c_date & ar_v )    // Default constructor OK

c_date::c_date( const char * p_v_in )
{
    operator=( p_v_in );
}

c_date::c_date( const std::string & r_v_in )
{
    operator=( r_v_in.c_str() );
}

bool c_date::is_valid( const std::string & r_v_in )
{
    // [+/-]CCYY-MM-DD[Z/((+/-)hh:mm]
    const char * p_s = r_v_in.c_str();
    skip_sign( p_s );
    if( skip_digits( p_s ) >= 4 &&                 // Year
            skip_const( p_s, "-" )  &&
            skip_digits( p_s ) == 2 &&             // Month
            skip_const( p_s, "-" ) &&
            skip_digits( p_s ) == 2  &&            // Day
            is_valid_tz( p_s ) )
        return true;
    return false;
}

// c_date & c_date::operator = ( const c_date & ar_v )  // Default assign OK

c_date & c_date::operator = ( const char * p_v_in )
{
    // [+/-]CCYY-MM-DD[Z/((+/-)hh:mm]
    bool is_neg = false;
    get_sign( p_v_in, is_neg );
    if( get_digits( p_v_in, m_year ) >= 4 &&
             skip_const( p_v_in, "-" ) &&
             get_digits( p_v_in, m_month ) == 2 &&
             skip_const( p_v_in, "-" ) &&
             get_digits( p_v_in, m_day ) == 2 )
    {
        set_tz( p_v_in );
        if( is_neg ) m_year = -m_year;
    }
    return *this;
}

c_date & c_date::operator = ( const std::string & r_v_in )
{
    return operator = ( r_v_in.c_str() );
}

std::string c_date::get_as_string() const
{
    return as_string( *this );
}

bool operator == ( const c_date & r_lhs_in, const c_date & r_rhs_in )
{
    return r_lhs_in.is_equal( r_rhs_in );
}

bool operator != ( const c_date & r_lhs_in, const c_date & r_rhs_in )
{
    return ! ( r_lhs_in == r_rhs_in );
}

bool operator > ( const c_date & r_lhs_in, const c_date & r_rhs_in )
{
    return r_lhs_in.is_greater_than( r_rhs_in );
}

bool operator >= ( const c_date & r_lhs_in, const c_date & r_rhs_in )
{
    return r_lhs_in == r_rhs_in || r_lhs_in > r_rhs_in;
}

bool operator < ( const c_date & r_lhs_in, const c_date & r_rhs_in )
{
    return ! ( r_lhs_in == r_rhs_in ) && ! ( r_lhs_in > r_rhs_in );
}

bool operator <= ( const c_date & r_lhs_in, const c_date & r_rhs_in )
{
    return ! ( r_lhs_in > r_rhs_in );
}

std::ostream & operator << ( std::ostream & r_os_in, const c_date & r_v_in )
{
    r_os_in.fill( '0' );
    r_os_in << std::setw(4) << r_v_in.m_year <<
            '-' <<
            std::setw(2) << r_v_in.m_month <<
            '-' <<
            std::setw(2) << r_v_in.m_day;
    r_v_in.output_tz( r_os_in );
    return r_os_in;
}


//                   class c_gyearmonth implementation
//------------------------------------------------------------------------------

c_gyearmonth::c_gyearmonth()
{
}

// c_gyearmonth::c_gyearmonth( const c_gyearmonth & ar_v )  // Default constructor OK

c_gyearmonth::c_gyearmonth( const char * p_v_in )
{
    operator=( p_v_in );
}

c_gyearmonth::c_gyearmonth( const std::string & r_v_in )
{
    operator=( r_v_in.c_str() );
}

bool c_gyearmonth::is_valid( const std::string & r_v_in )
{
    // [+/-]CCYY-MM[Z/((+/-)hh:mm]
    const char * p_s = r_v_in.c_str();
    skip_sign( p_s );
    if( skip_digits( p_s ) >= 4 &&         // Year
            skip_const( p_s, "-" )  &&
            skip_digits( p_s ) == 2 &&     // Month
            is_valid_tz( p_s ) )
        return true;
    return false;
}

// c_gyearmonth & c_gyearmonth::operator = ( const c_gyearmonth & ar_v )    // Default assign OK

c_gyearmonth & c_gyearmonth::operator = ( const char * p_v_in )
{
    // [+/-]CCYY-MM[Z/((+/-)hh:mm]
    bool is_neg = false;
    get_sign( p_v_in, is_neg );
    if( get_digits( p_v_in, m_year ) >= 4 &&
             skip_const( p_v_in, "-" ) &&
             get_digits( p_v_in, m_month ) == 2 )
    {
        set_tz( p_v_in );
        if( is_neg ) m_year = -m_year;
    }
    return *this;
}

c_gyearmonth & c_gyearmonth::operator = ( const std::string & r_v_in )
{
    return operator = ( r_v_in.c_str() );
}

std::string c_gyearmonth::get_as_string() const
{
    return as_string( *this );
}

bool operator == ( const c_gyearmonth & r_lhs_in, const c_gyearmonth & r_rhs_in )
{
    return r_lhs_in.is_equal( r_rhs_in );
}

bool operator != ( const c_gyearmonth & r_lhs_in, const c_gyearmonth & r_rhs_in )
{
    return ! ( r_lhs_in == r_rhs_in );
}

bool operator > ( const c_gyearmonth & r_lhs_in, const c_gyearmonth & r_rhs_in )
{
    return r_lhs_in.is_greater_than( r_rhs_in );
}

bool operator >= ( const c_gyearmonth & r_lhs_in, const c_gyearmonth & r_rhs_in )
{
    return r_lhs_in == r_rhs_in || r_lhs_in > r_rhs_in;
}

bool operator < ( const c_gyearmonth & r_lhs_in, const c_gyearmonth & r_rhs_in )
{
    return ! ( r_lhs_in == r_rhs_in ) && ! ( r_lhs_in > r_rhs_in );
}

bool operator <= ( const c_gyearmonth & r_lhs_in, const c_gyearmonth & r_rhs_in )
{
    return ! ( r_lhs_in > r_rhs_in );
}

std::ostream & operator << ( std::ostream & r_os_in, const c_gyearmonth & r_v_in )
{
    // [+/-]CCYY-MM[Z/((+/-)hh:mm]
    r_os_in.fill( '0' );
    r_os_in << std::setw(4) << r_v_in.m_year <<
            '-' <<
            std::setw(2) << r_v_in.m_month;
    r_v_in.output_tz( r_os_in );
    return r_os_in;
}

//                class c_gyear implementation
//------------------------------------------------------------------------------

c_gyear::c_gyear()
{
}

// c_gyear::c_gyear( const c_gyear & ar_v ) // Default constructor OK

c_gyear::c_gyear( const char * p_v_in )
{
    operator=( p_v_in );
}

c_gyear::c_gyear( const std::string & r_v_in )
{
    operator=( r_v_in.c_str() );
}

bool c_gyear::is_valid( const std::string & r_v_in )
{
    // [+/-]CCYY[Z/((+/-)hh:mm]
    const char * p_s = r_v_in.c_str();
    skip_sign( p_s );
    if( skip_digits( p_s ) >= 4 &&
            is_valid_tz( p_s ) )
        return true;
    return false;
}

// c_gyear & c_gyear::operator = ( const c_gyear & ar_v )   // Default assign OK

c_gyear & c_gyear::operator = ( const char * p_v_in )
{
    // [+/-]CCYY[Z/((+/-)hh:mm]
    bool is_neg = false;
    get_sign( p_v_in, is_neg );
    if( get_digits( p_v_in, m_year ) >= 4 )
    {
        set_tz( p_v_in );
        if( is_neg ) m_year = -m_year;
    }
    return *this;
}

c_gyear & c_gyear::operator = ( const std::string & r_v_in )
{
    return operator = ( r_v_in.c_str() );
}

std::string c_gyear::get_as_string() const
{
    return as_string( *this );
}

bool operator == ( const c_gyear & r_lhs_in, const c_gyear & r_rhs_in )
{
    return r_lhs_in.is_equal( r_rhs_in );
}

bool operator != ( const c_gyear & r_lhs_in, const c_gyear & r_rhs_in )
{
    return ! ( r_lhs_in == r_rhs_in );
}

bool operator > ( const c_gyear & r_lhs_in, const c_gyear & r_rhs_in )
{
    return r_lhs_in.is_greater_than( r_rhs_in );
}

bool operator >= ( const c_gyear & r_lhs_in, const c_gyear & r_rhs_in )
{
    return r_lhs_in == r_rhs_in || r_lhs_in > r_rhs_in;
}

bool operator < ( const c_gyear & r_lhs_in, const c_gyear & r_rhs_in )
{
    return ! ( r_lhs_in == r_rhs_in ) && ! ( r_lhs_in > r_rhs_in );
}

bool operator <= ( const c_gyear & r_lhs_in, const c_gyear & r_rhs_in )
{
    return ! ( r_lhs_in > r_rhs_in );
}

std::ostream & operator << ( std::ostream & r_os_in, const c_gyear & r_v_in )
{
    // [+/-]CCYY[Z/((+/-)hh:mm]
    r_os_in.fill( '0' );
    r_os_in << std::setw(4) << r_v_in.m_year;
    r_v_in.output_tz( r_os_in );
    return r_os_in;
}

//                class c_gmonthday implementation
//------------------------------------------------------------------------------

c_gmonthday::c_gmonthday()
{
}

// c_gmonthday::c_gmonthday( const c_gmonthday & ar_v ) // Default constructor OK

c_gmonthday::c_gmonthday( const char * p_v_in )
{
    operator=( p_v_in );
}

c_gmonthday::c_gmonthday( const std::string & r_v_in )
{
    operator=( r_v_in.c_str() );
}

bool c_gmonthday::is_valid( const std::string & r_v_in )
{
    // --MM-DD[Z/((+/-)hh:mm]
    const char * p_s = r_v_in.c_str();
    if( skip_const( p_s, "--" )  &&
            skip_digits( p_s ) == 2 &&     // Month
            skip_const( p_s, "-" ) &&
            skip_digits( p_s ) == 2 &&     // Day
            is_valid_tz( p_s ) )
        return true;
    return false;
}

// c_gmonthday & c_gmonthday::operator = ( const c_gmonthday & ar_v )   // Default assign OK

c_gmonthday & c_gmonthday::operator = ( const char * p_v_in )
{
    // --MM-DD[Z/((+/-)hh:mm]
    if( skip_const( p_v_in, "--" ) &&
             get_digits( p_v_in, m_month ) == 2 &&
             skip_const( p_v_in, "-" ) &&
             get_digits( p_v_in, m_day ) )
        {
        set_tz( p_v_in );
        }
    return *this;
}

c_gmonthday & c_gmonthday::operator = ( const std::string & r_v_in )
{
    return operator = ( r_v_in.c_str() );
}

std::string c_gmonthday::get_as_string() const
{
    return as_string( *this );
}

bool operator == ( const c_gmonthday & r_lhs_in, const c_gmonthday & r_rhs_in )
{
    return r_lhs_in.is_equal( r_rhs_in );
}

bool operator != ( const c_gmonthday & r_lhs_in, const c_gmonthday & r_rhs_in )
{
    return ! ( r_lhs_in == r_rhs_in );
}

bool operator > ( const c_gmonthday & r_lhs_in, const c_gmonthday & r_rhs_in )
{
    return r_lhs_in.is_greater_than( r_rhs_in );
}

bool operator >= ( const c_gmonthday & r_lhs_in, const c_gmonthday & r_rhs_in )
{
    return r_lhs_in == r_rhs_in || r_lhs_in > r_rhs_in;
}

bool operator < ( const c_gmonthday & r_lhs_in, const c_gmonthday & r_rhs_in )
{
    return ! ( r_lhs_in == r_rhs_in ) && ! ( r_lhs_in > r_rhs_in );
}

bool operator <= ( const c_gmonthday & r_lhs_in, const c_gmonthday & r_rhs_in )
{
    return ! ( r_lhs_in > r_rhs_in );
}

std::ostream & operator << ( std::ostream & r_os_in, const c_gmonthday & r_v_in )
{
    // --MM-DD[Z/((+/-)hh:mm]
    r_os_in.fill( '0' );
    r_os_in << "--" <<
            std::setw(2) << r_v_in.m_month <<
            '-' <<
            std::setw(2) << r_v_in.m_day;
    r_v_in.output_tz( r_os_in );
    return r_os_in;
}

//                   class c_gday implementation
//------------------------------------------------------------------------------
    // ---DD[Z/((+/-)hh:mm]

c_gday::c_gday()
{
}

// c_gday::c_gday( const c_gday & ar_v )    // Default constructor OK

c_gday::c_gday( const char * p_v_in )
{
    operator=( p_v_in );
}

c_gday::c_gday( const std::string & r_v_in )
{
    operator=( r_v_in.c_str() );
}

bool c_gday::is_valid( const std::string & r_v_in )
{
    // ---DD[Z/((+/-)hh:mm]
    const char * p_s = r_v_in.c_str();
    if( skip_const( p_s, "---" ) &&
            skip_digits( p_s ) == 2 &&
            is_valid_tz( p_s ) )
        return true;
    return false;
}

// c_gday & c_gday::operator = ( const c_gday & ar_v )  // Default assign OK

c_gday & c_gday::operator = ( const char * p_v_in )
{
    // ---DD[Z/((+/-)hh:mm]
    if( skip_const( p_v_in, "---" ) && get_digits( p_v_in, m_day ) )
        set_tz( p_v_in );
    return *this;
}

c_gday & c_gday::operator = ( const std::string & r_v_in )
{
    return operator = ( r_v_in.c_str() );
}

std::string c_gday::get_as_string() const
{
    return as_string( *this );
}

bool operator == ( const c_gday & r_lhs_in, const c_gday & r_rhs_in )
{
    return r_lhs_in.is_equal( r_rhs_in );
}

bool operator != ( const c_gday & r_lhs_in, const c_gday & r_rhs_in )
{
    return ! ( r_lhs_in == r_rhs_in );
}

bool operator > ( const c_gday & r_lhs_in, const c_gday & r_rhs_in )
{
    return r_lhs_in.is_greater_than( r_rhs_in );
}

bool operator >= ( const c_gday & r_lhs_in, const c_gday & r_rhs_in )
{
    return r_lhs_in == r_rhs_in || r_lhs_in > r_rhs_in;
}

bool operator < ( const c_gday & r_lhs_in, const c_gday & r_rhs_in )
{
    return ! ( r_lhs_in == r_rhs_in ) && ! ( r_lhs_in > r_rhs_in );
}

bool operator <= ( const c_gday & r_lhs_in, const c_gday & r_rhs_in )
{
    return ! ( r_lhs_in > r_rhs_in );
}

std::ostream & operator << ( std::ostream & r_os_in, const c_gday & r_v_in )
{
    // ---DD[Z/((+/-)hh:mm]
    r_os_in.fill( '0' );
    r_os_in << "---" <<
            std::setw(2) << r_v_in.m_day;
    r_v_in.output_tz( r_os_in );
    return r_os_in;
}

//                    class c_gmonth implementation
//------------------------------------------------------------------------------

c_gmonth::c_gmonth()
{
}

// c_gmonth::c_gmonth( const c_gmonth & ar_v )  // Default constructor OK

c_gmonth::c_gmonth( const char * p_v_in )
{
    operator=( p_v_in );
}

c_gmonth::c_gmonth( const std::string & r_v_in )
{
    operator=( r_v_in.c_str() );
}

bool c_gmonth::is_valid( const std::string & r_v_in )
{
    // --MM--[Z/((+/-)hh:mm]
    const char * p_s = r_v_in.c_str();
    if( skip_const( p_s, "--" )  &&
            skip_digits( p_s ) == 2 &&     // Month
            skip_const( p_s, "--" ) &&
            is_valid_tz( p_s ) )
        return true;
    return false;
}

// c_gmonth & c_gmonth::operator = ( const c_gmonth & ar_v )    // Default assign OK

c_gmonth & c_gmonth::operator = ( const char * p_v_in )
{
    // --MM--[Z/((+/-)hh:mm]
    if( skip_const( p_v_in, "--" ) &&
             get_digits( p_v_in, m_month ) == 2 &&
             skip_const( p_v_in, "--" ) )
    {
        set_tz( p_v_in );
    }
    return *this;
}

c_gmonth & c_gmonth::operator = ( const std::string & r_v_in )
{
    return operator = ( r_v_in.c_str() );
}

std::string c_gmonth::get_as_string() const
{
    return as_string( *this );
}

bool operator == ( const c_gmonth & r_lhs_in, const c_gmonth & r_rhs_in )
{
    return r_lhs_in.is_equal( r_rhs_in );
}

bool operator != ( const c_gmonth & r_lhs_in, const c_gmonth & r_rhs_in )
{
    return ! ( r_lhs_in == r_rhs_in );
}

bool operator > ( const c_gmonth & r_lhs_in, const c_gmonth & r_rhs_in )
{
    return r_lhs_in.is_greater_than( r_rhs_in );
}

bool operator >= ( const c_gmonth & r_lhs_in, const c_gmonth & r_rhs_in )
{
    return r_lhs_in == r_rhs_in || r_lhs_in > r_rhs_in;
}

bool operator < ( const c_gmonth & r_lhs_in, const c_gmonth & r_rhs_in )
{
    return ! ( r_lhs_in == r_rhs_in ) && ! ( r_lhs_in > r_rhs_in );
}

bool operator <= ( const c_gmonth & r_lhs_in, const c_gmonth & r_rhs_in )
{
    return ! ( r_lhs_in > r_rhs_in );
}

std::ostream & operator << ( std::ostream & r_os_in, const c_gmonth & r_v_in )
{
    // --MM--[Z/((+/-)hh:mm]
    r_os_in.fill( '0' );
    r_os_in << "--" <<
            std::setw(2) << r_v_in.m_month <<
            "--";
    r_v_in.output_tz( r_os_in );
    return r_os_in;
}

//                   class c_binary implementation
//------------------------------------------------------------------------------

c_binary::c_binary( const c_binary & r_v_in ) : m_value( r_v_in.m_value )
{
}

c_binary::c_binary( const unsigned char * p_v_in, size_t len_in ) : m_value( p_v_in, p_v_in+len_in )
{
}

c_binary & c_binary::operator = ( const c_binary & r_v_in )
{
    c_binary temp( r_v_in );
    swap( temp );
    return *this;
}

void c_binary::swap( c_binary & r_v_in )
{
    m_value.swap( r_v_in.m_value );
}

bool c_binary::is_valid_hex( const std::string & r_v_in )
{
    if( r_v_in.size() % 2 != 0 )
        return false;
    for( std::string::size_type i=0; i<r_v_in.size(); ++i )
    {
        int c = r_v_in[i];
        if( ! ( xisdigit( c ) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') ) )
            return false;           // v4.2 - gcc_433_brackets
    }
    return true;
}

bool c_binary::is_valid_base64( const std::string & r_v_in )
{
    unsigned int length = 0;
    std::string::size_type i;
    if( r_v_in.size() == 0 )
        return true;
    for( i=0; i<r_v_in.size(); ++i )
    {
        int c = r_v_in[i];
        if( ! ( xisalnum( c ) || c == '+' || c == '/' || xisspace( c ) ) )
            break;
        if( ! xisspace( c ) )
            ++length;
    }
    if( i < 2 || i < r_v_in.size() - 2 )
        return false;
    if( (length + (r_v_in.size() - i)) % 4 != 0 )
        return false;
    for( ; i<r_v_in.size() && ! xisspace( r_v_in[i] ); ++i )
        if( r_v_in[i] != '=' )
            return false;
    return true;
}

unsigned char * c_binary::get( unsigned char * p_buffer_out, size_t buffer_size_in ) const
{
    if( m_value.size() < buffer_size_in )
        buffer_size_in = m_value.size();
    unsigned char * p_buffer = p_buffer_out;
    for( size_t i=0; i<buffer_size_in; ++i )
        *p_buffer++ = m_value[ i ];
    return p_buffer_out;
}

const unsigned char & c_binary::operator[] ( size_t i_in ) const    // C130117_1505 - Returning ref allows use with fwrite() etc.
{
    assert( i_in < m_value.size() );
    return m_value[ i_in ];
}

unsigned char & c_binary::operator[] ( size_t i_in )
{
    assert( i_in < m_value.size() );
    return m_value[ i_in ];
}

#if defined( _MSC_VER ) ||  defined( __GNUC__ ) && __GNUC__ >= 3
// pre-GCC v3 does not have vector::at (at() throws an exception if out of bounds)

    const unsigned char & c_binary::at( size_t i_in ) const // C130117_1505 - Returning ref allows use with fwrite() etc.
    {
        return m_value.at( i_in );
    }

    unsigned char & c_binary::at( size_t i_in )
    {
        return m_value.at( i_in );
    }
#endif

size_t c_binary::size() const
{
    return m_value.size();
}

bool c_binary::empty() const    // v5
{
    return m_value.empty();
}

std::string c_binary::get_as_hex_string() const     //C141222_1154
{
    std::ostringstream t;
    out_hex( t );
    return t.str();
}

std::string c_binary::get_as_base64_string() const  //C141222_1154
{
    std::ostringstream t;
    out_base64( t );
    return t.str();
}

void c_binary::set( const unsigned char * p_v_in, size_t len_in )
{
    c_binary temp( p_v_in, len_in );
    swap( temp );
}

void c_binary::append( unsigned char c_in )
{
    m_value.push_back( c_in );
}

void c_binary::operator += ( unsigned char c_in )
{
    m_value.push_back( c_in );
}

void c_binary::from_hex( const std::string & r_src_in )
{
    m_value.clear();
    if( r_src_in.length() < 2 )
        return;
    m_value.reserve( r_src_in.length() / 2 );
    int c;
    int c1;
    int c2;
    for( std::string::size_type i=0; i<r_src_in.length()-1; i+=2 )
    {
        c = r_src_in[i];
        c1 = xisdigit( c )?c-'0':(c>='a'&&c<='f')?c-'a'+10:c-'A'+10;
        c = r_src_in[i+1];
        c2 = xisdigit( c )?c-'0':(c>='a'&&c<='f')?c-'a'+10:c-'A'+10;
        m_value.push_back( static_cast<unsigned char>( (c1 << 4) + c2 ) );
    }
}

unsigned char c_binary::p_base64_char_to_6_bits( int c_in )
{
    if( c_in >= 'A' && c_in <= 'Z' )
        return static_cast<unsigned char>( c_in - 'A' );
    else if( c_in >= 'a' && c_in <= 'z' )
        return static_cast<unsigned char>( c_in - 'a' + 26 );
    else if( xisdigit( c_in ) )
        return static_cast<unsigned char>( c_in - '0' + 52 );
    else if( c_in == '+' )
        return 62;
    // ( a_c == '/' )
    return 63;
}

void c_binary::p_base64_convert_24bits_to_bytes( tlmx_uns32 accumulated_in, size_t number_accumulated_in )
{
    unsigned char c_data[3];
    c_data[2] = static_cast<char>( accumulated_in & 0xff );
    accumulated_in >>= 8;
    c_data[1] = static_cast<char>( accumulated_in & 0xff );
    accumulated_in >>= 8;
    c_data[0] = static_cast<char>( accumulated_in & 0xff );
    m_value.push_back( c_data[0] );
    if( number_accumulated_in >= 3 ) m_value.push_back( c_data[1] );
    if( number_accumulated_in >= 4 ) m_value.push_back( c_data[2] );
}

void c_binary::from_base64( const std::string & r_src_in )
{
    m_value.clear();
    m_value.reserve( (3 * r_src_in.length()) / 4 );
    tlmx_uns32 accumulated = 0;
    size_t number_accumulated = 0;
    for( std::string::size_type i=0; i<r_src_in.length(); ++i )
    {
        int c = r_src_in[i];
        if( xisalnum( c ) || c == '+' || c == '/' )
        {
            accumulated <<= 6;
            accumulated |= p_base64_char_to_6_bits( c );
            if( ++number_accumulated == 4 )
            {
                p_base64_convert_24bits_to_bytes( accumulated, number_accumulated );
                number_accumulated = accumulated = 0;
            }
        }
    }
    if( number_accumulated != 0 )
    {
        accumulated <<= 6 * (4 - number_accumulated);
        p_base64_convert_24bits_to_bytes( accumulated, number_accumulated );
    }
}

bool operator == ( const c_binary & r_lhs_in, const c_binary & r_rhs_in )
{
    return r_lhs_in.m_value == r_rhs_in.m_value;
}

bool operator != ( const c_binary & r_lhs_in, const c_binary & r_rhs_in )
{
    return ! ( r_lhs_in == r_rhs_in );
}

bool c_binary::equal( const unsigned char c_rhs_in[], size_t rhs_len_in ) const
{
    return m_value.size() == rhs_len_in && std::equal( m_value.begin(), m_value.end(), c_rhs_in );
}

void c_binary::out_hex( std::ostream & r_os_in ) const
{
    std::ios::fmtflags old_ios_flags = r_os_in.setf( std::ios::hex | std::ios::uppercase,
                                        std::ios::basefield | std::ios::uppercase );
    r_os_in.fill( '0' );
    for( std::vector<unsigned char>::size_type i=0; i<m_value.size(); ++i )
        r_os_in << std::setw(2) << static_cast<int>(m_value[i]);
    r_os_in.flags( old_ios_flags );
}

char c_binary::p_6_bits_to_base64_char( int c_in )
{
    if( c_in >= 0 && c_in <= 25 )
        return static_cast<char>( c_in + 'A' );
    else if( c_in >= 26 && c_in <= 51 )
        return static_cast<char>( c_in - 26 + 'a' );
    else if( c_in >= 52 && c_in <= 61 )
        return static_cast<char>( c_in - 52 + '0' );
    else if( c_in == 62 )
        return '+';
    return '/'; // a_c == 63
}

void c_binary::p_convert_24bits_to_4x6bits( std::ostream & r_os_in,
                                                tlmx_uns32 accumulated_in,
                                                size_t number_accumulated_in )
{
    if( number_accumulated_in == 1 )
        accumulated_in <<= 2 * 8;
    else if( number_accumulated_in == 2 )
        accumulated_in <<= 8;
    char c_data[4];
    c_data[3] = p_6_bits_to_base64_char( accumulated_in & 0x3f );
    accumulated_in >>= 6;
    c_data[2] = p_6_bits_to_base64_char( accumulated_in & 0x3f );
    accumulated_in >>= 6;
    c_data[1] = p_6_bits_to_base64_char( accumulated_in & 0x3f );
    accumulated_in >>= 6;
    c_data[0] = p_6_bits_to_base64_char( accumulated_in & 0x3f );
    if( number_accumulated_in == 3 )
        r_os_in << c_data[0] << c_data[1] << c_data[2] << c_data[3];
    else if( number_accumulated_in == 2 )
        r_os_in << c_data[0] << c_data[1] << c_data[2] << '=';
    else if( number_accumulated_in == 1 )
        r_os_in << c_data[0] << c_data[1] << "==";
}


void c_binary::out_base64( std::ostream & r_os_in ) const
{
    tlmx_uns32 accumulated = 0;
    int number_accumulated = 0;
    for( std::vector<unsigned char>::size_type i=0; i<m_value.size(); ++i )
    {
        accumulated <<= 8;
        accumulated |= static_cast<int>(m_value[i]);
        if( ++number_accumulated == 3 )
        {
            p_convert_24bits_to_4x6bits( r_os_in, accumulated, number_accumulated );
            accumulated = number_accumulated = 0;
            if( i != 0 && (i % 3 * 76 / 4) == 0 )
                r_os_in << '\n';
        }
    }
    p_convert_24bits_to_4x6bits( r_os_in, accumulated, number_accumulated );
}

std::ostream & operator << ( std::ostream & r_os_in, const c_binary & r_v_in )   // C170425_1618
{
    for( size_t i=0; i<r_v_in.size(); ++i )
        r_os_in << r_v_in[i];
    return r_os_in;
}

//------------------------------------------------------------------------------
//                              QName helpers - V3.3
//------------------------------------------------------------------------------

const std::string k_qname_user_explicit_ns_prefix_not_set( " " );   // V3.8

template< class T >     // Split string with format {qname} {local name} {namespace} into three strings
static void make_qname_from_default_spec( T * p_value_out, const std::string & r_qname_default_spec_in )
{
    std::string qname;
    std::string namespace_uri;
    std::string local_name;

    size_t i=0;
    while( i < r_qname_default_spec_in.size() && ! xisspace( r_qname_default_spec_in[i] ) )
        qname += r_qname_default_spec_in[i++];

    while( i < r_qname_default_spec_in.size() && xisspace( r_qname_default_spec_in[i] ) )
        ++i;

    while( i < r_qname_default_spec_in.size() && ! xisspace( r_qname_default_spec_in[i] ) )
        local_name += r_qname_default_spec_in[i++];

    while( i < r_qname_default_spec_in.size() && xisspace( r_qname_default_spec_in[i] ) )
        ++i;

    while( i < r_qname_default_spec_in.size() )
        namespace_uri += r_qname_default_spec_in[i++];

    make_qname( p_value_out, qname, namespace_uri, local_name );
}

LMX_TDECL void make_qname(
                    ct_qname< std::string > * p_value_out,
                    const std::string & LMX_UNUSED_PARAM(r_qname_in),
                    const std::string & r_namespace_in,
                    const std::string & r_local_name_in )
{
    p_value_out->set( r_namespace_in, r_local_name_in );
}

LMX_TDECL void make_qname(
                    std::string * p_value_out,
                    const std::string & r_qname_in,
                    const std::string & LMX_UNUSED_PARAM(r_namespace_in),
                    const std::string & LMX_UNUSED_PARAM(r_local_name_in) )
{
    *p_value_out = r_qname_in;
}

void make_qname( ct_qname< std::string > * p_value_out, const std::string & r_qname_default_spec_in )
{
    make_qname_from_default_spec( p_value_out, r_qname_default_spec_in );
}

void make_qname( std::string * p_value_out, const std::string & r_qname_default_spec_in )
{
    make_qname_from_default_spec( p_value_out, r_qname_default_spec_in );
}

#ifndef LMX_NO_WSTRING

LMX_TDECL void make_qname(
                    ct_qname< std::wstring > * p_value_out,
                    const std::string & LMX_UNUSED_PARAM(r_qname_in),
                    const std::string & r_namespace_in,
                    const std::string & r_local_name_in )
{
    std::wstring wide_local_name;
    convert( &wide_local_name, r_local_name_in );
    p_value_out->set( r_namespace_in, wide_local_name );
}

LMX_TDECL void make_qname(
                    std::wstring * p_value_out,
                    const std::string & r_qname_in,
                    const std::string & LMX_UNUSED_PARAM(r_namespace_in),
                    const std::string & LMX_UNUSED_PARAM(r_local_name_in) )
{
    convert( p_value_out, r_qname_in );
}

LMX_TDECL void make_qname( ct_qname< std::wstring > * p_value_out, const std::string & r_qname_default_spec_in )
{
    make_qname_from_default_spec( p_value_out, r_qname_default_spec_in );
}

LMX_TDECL void make_qname( std::wstring * p_value_out, const std::string & r_qname_default_spec_in )
{
    make_qname_from_default_spec( p_value_out, r_qname_default_spec_in );
}
#endif

//------------------------------------------------------------------------------
//                       Output Converters - renamed in v3.8.2
//------------------------------------------------------------------------------

#ifndef LMX_NO_WSTRING

LMX_TDECL std::ostream & output_convert_to_xml( std::ostream & r_os_in, const std::wstring & r_v_in )
{
    //   UCS-2 range (hex.)     UTF-8 octet sequence (binary)
    //   0000-007F              0xxxxxxx
    //   0080-07FF              110xxxxx 10xxxxxx
    //   0800-FFFF              1110xxxx 10xxxxxx 10xxxxxx
    //   0001 0000-001F FFFF    11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    for( std::wstring::size_type i=0; i<r_v_in.size(); ++i )
    {
        t_unicode_char c = r_v_in[i];     // C100702_1244
        if( sizeof( wchar_t ) == 2 )    // Make sure we don't get sign extension problems
            c &= 0x0000ffff;          // Compiler will optimise this out if not required

        if( c >= 0x0000d800 && c <= 0x0000dbff )    // UTF-16 Surrogates
        {
            if( ++i<r_v_in.size() )
            {
                t_unicode_char c2 = r_v_in[i];        // C100702_1244
                if( sizeof( wchar_t ) == 2 )    // Make sure we don't get sign extension problems
                    c2 &= 0x0000ffff;         // Compiler will optimise this out if not required
                if( c2 >= 0x0000dc00 && c2 <= 0x0000dfff )
                    c = ((c & 0x3ff)<<10) + (c2 & 0x3ff) + 0x10000;
                else
                    return r_os_in;
            }
            else
                return r_os_in;
        }

        if( c == '"' )
            r_os_in << "&quot;";
        else if( c == '\'' )
            r_os_in << "&apos;";
        else if( c == '&' )
            r_os_in << "&amp;";
        else if( c == '<' )
            r_os_in << "&lt;";
        #if defined( LMX_XML_ALWAYS_ESCAPE_GT ) && LMX_XML_ALWAYS_ESCAPE_GT != 0    // C120312_1049
            else if( c == '>' )
                r_os_in << "&gt;";
        #else
            else if( c == '>' && i >= 2 && r_v_in[i-1] == ']' )  // Prevent ]]> in output v3.11     // C100702_1244
                r_os_in << "&gt;";
        #endif
        else if( c <= 0x7f )
            r_os_in << static_cast<char>(c);
        else if( c <= 0x7ff )
        {
            // 2 bytes
            char tc1 = static_cast<char>((c & 0x3f) | 0x80);
            c >>= 6;
            r_os_in << static_cast<char>((c & 0x1f) | 0xc0) << tc1;
        }
        else if( c <= 0x0000ffff )
        {
            // 3 bytes
            char tc2 = static_cast<char>((c & 0x3f) | 0x80);
            c >>= 6;
            char tc1 = static_cast<char>((c & 0x3f) | 0x80);
            c >>= 6;
            r_os_in << static_cast<char>((c & 0x0f) | 0xe0) << tc1 << tc2;
        }
        else
        {
            // 4 bytes
            char tc3 = static_cast<char>((c & 0x3f) | 0x80);
            c >>= 6;
            char tc2 = static_cast<char>((c & 0x3f) | 0x80);
            c >>= 6;
            char tc1 = static_cast<char>((c & 0x3f) | 0x80);
            c >>= 6;
            r_os_in << static_cast<char>((c & 0x07) | 0xf0) << tc1 << tc2 << tc3;
        }
    }
    return r_os_in;
}
#endif

LMX_TDECL std::ostream & output_convert_to_xml( std::ostream & r_os_in, const std::string & r_v_in )
{
    for( std::string::size_type i=0; i<r_v_in.size(); ++i )
    {
        char c = r_v_in[i];       // C100702_1244
        if( c == '"' )
            r_os_in << "&quot;";
        else if( c == '\'' )
            r_os_in << "&apos;";
        else if( c == '&' )
            r_os_in << "&amp;";
        else if( c == '<' )
            r_os_in << "&lt;";
        #if defined( LMX_XML_ALWAYS_ESCAPE_GT ) && LMX_XML_ALWAYS_ESCAPE_GT != 0    // C120312_1049
            else if( c == '>' )
                r_os_in << "&gt;";
        #else
            else if( c == '>' && i >= 2 && r_v_in[i-1] == ']' )  // Prevent ]]> in output v3.11     // C100702_1244
                r_os_in << "&gt;";
        #endif
        else
            r_os_in << c;
    }
    return r_os_in;
}

LMX_TDECL std::ostream & output_convert_to_xml( std::ostream & r_os_in, bool v_in )
{
    if( v_in )
        r_os_in << "true";
    else
        r_os_in << "false";
    return r_os_in;
}

LMX_TDECL std::ostream & output_convert_to_bool_digit( std::ostream & r_os_in, bool v_in )    // V3.7
{
    if( v_in )
        r_os_in << "1";
    else
        r_os_in << "0";
    return r_os_in;
}

LMX_TDECL std::ostream & output_convert_to_xml( std::ostream & r_os_in, unsigned char v_in )
{
    r_os_in << static_cast<unsigned int>( v_in );
    return r_os_in;
}

LMX_TDECL std::ostream & output_convert_to_xml( std::ostream & r_os_in, signed char v_in )
{
    r_os_in << static_cast<int>( v_in );
    return r_os_in;
}

LMX_TDECL std::ostream & output_convert_to_xml( std::ostream & r_os_in, float v_in )
{
#if defined( _MSC_VER )

    int float_class = _fpclass( v_in );  // See portability note: IEEE754

    if( float_class & _FPCLASS_SNAN || float_class & _FPCLASS_QNAN )
        r_os_in << "NaN";
    else if( float_class & _FPCLASS_NINF )
        r_os_in << "-INF";
    else if( float_class & _FPCLASS_NZ )
        r_os_in << "-0.0";
    else if( float_class & _FPCLASS_PZ )
        r_os_in << "0.0";
    else if( float_class & _FPCLASS_PINF )
        r_os_in << "INF";
    else
    {
        std::streamsize old_precision = r_os_in.precision( LMX_FLOAT_PRECISION ); // v3.8 // C170614_1145
        r_os_in << v_in;
        r_os_in.precision( old_precision );
    }
#else
    // C131206_1452 - Revised attempt at a portable implementation.
    #if defined( __GNUC__ ) && __GNUC__ < 4
        if( v_in != 0.0 && v_in == v_in * 0.5F )
        {
            if( v_in < 0.0 )
                r_os_in << "-INF";
            else
                r_os_in << "INF";
        }
    #else
        if( v_in == std::numeric_limits<float>::infinity() )    // C181024_1014
            r_os_in << "INF";
        else if( v_in == -std::numeric_limits<float>::infinity() )
            r_os_in << "-INF";
    #endif
    else if( v_in != v_in )      // Required behaviour for NaN by IEEE754
        r_os_in << "NaN";
    else
    {
        std::streamsize old_precision = r_os_in.precision( LMX_FLOAT_PRECISION ); // v3.8 // C170614_1145
        r_os_in << v_in;
        r_os_in.precision( old_precision );
    }
#endif
    return r_os_in;
}

LMX_TDECL std::ostream & output_convert_to_xml( std::ostream & r_os_in, double v_in )
{
#if defined( _MSC_VER )

    int float_class = _fpclass( v_in );  // See portability note: IEEE754

    if( float_class & _FPCLASS_SNAN || float_class & _FPCLASS_QNAN )
        r_os_in << "NaN";
    else if( float_class & _FPCLASS_NINF )
        r_os_in << "-INF";
    else if( float_class & _FPCLASS_NZ )
        r_os_in << "-0.0";
    else if( float_class & _FPCLASS_PZ )
        r_os_in << "0.0";
    else if( float_class & _FPCLASS_PINF )
        r_os_in << "INF";
    else
    {
        std::streamsize old_precision = r_os_in.precision( LMX_DOUBLE_PRECISION );    // v3.8 // C170614_1145
        r_os_in << v_in;
        r_os_in.precision( old_precision );
    }
#else
    // C131206_1452 - Revised attempt at a portable implementation.
    #if defined( __GNUC__ ) && __GNUC__ < 4
        if( v_in != 0.0 && v_in == v_in * 0.5F )
        {
            if( v_in < 0.0 )
                r_os_in << "-INF";
            else
                r_os_in << "INF";
        }
    #else
        if( v_in == std::numeric_limits<double>::infinity() )    // C181024_1014
            r_os_in << "INF";
        else if( v_in == -std::numeric_limits<double>::infinity() )
            r_os_in << "-INF";
    #endif
    else if( v_in != v_in )      // Required behaviour for NaN by IEEE754
        r_os_in << "NaN";
    else
    {
        std::streamsize old_precision = r_os_in.precision( LMX_DOUBLE_PRECISION ); // v3.8 // C170614_1145
        r_os_in << v_in;
        r_os_in.precision( old_precision );
    }
#endif
    return r_os_in;
}

LMX_TDECL std::ostream & output_convert_to_hex( std::ostream & r_os_in, const c_binary & r_v_in )
{
    r_v_in.out_hex( r_os_in );
    return r_os_in;
}

LMX_TDECL std::ostream & output_convert_to_base64( std::ostream & r_os_in, const c_binary & r_v_in )
{
    r_v_in.out_base64( r_os_in );
    return r_os_in;
}

// See lmxparse.h for c_as_xml<char> and c_as_xml<unsigned char> versions.

LMX_TDECL std::ostream & output_convert_to_escaped( std::ostream & r_os_in, const std::string & r_v_in )
{
    return output_convert_to_xml( r_os_in, r_v_in );
}

#ifndef LMX_NO_WSTRING

// This version is primarily for user convenience
LMX_TDECL std::ostream & output_convert_to_ascii( std::ostream & r_os_in, const std::wstring & r_v_in )
{
    for( std::wstring::size_type i=0; i<r_v_in.size(); ++i )
    {
        wchar_t c = r_v_in[i];        // C100702_1244
        if( c <= 0x7f )
            r_os_in << static_cast<char>(c);
        else
            r_os_in << static_cast<char>('.');
    }
    return r_os_in;
}
#endif

//------------------------------------------------------------------------------
//                            Conversion functions
//------------------------------------------------------------------------------

#ifndef LMX_NO_WSTRING

// N.B. This is cut and pasted from lmxparse.cpp to ease linking issues.
//----------------------------------------------------------------------
static char * unicode_char_to_utf8( t_unicode_char c_in, tc_utf8_buf c_utf8_in )
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

LMX_TDECL std::string & convert( std::string * p_out, const std::wstring & r_in )
{
    p_out->erase();
    tc_utf8_buf c_buf;

    for( size_t i=0; i < r_in.size(); ++i )
    {
        t_unicode_char c = r_in[i];
        if( c >= 0x0000d800 &&
                c <= 0x0000dbff &&
                ++i < r_in.size() &&
                r_in[i] >= 0x0000dc00 &&
                r_in[i] <= 0x0000dfff )
            c = ((c & 0x3ff)<<10) + (r_in[i] & 0x3ff) + 0x10000;
        *p_out += unicode_char_to_utf8( c, c_buf );
        }

    return *p_out;
}

LMX_TDECL std::wstring & convert( std::wstring * p_out, const std::string & r_in )
{
    vtostring( *p_out, r_in );
    return *p_out;
}
#endif

//------------------------------------------------------------------------------
//                       Parsing helper functions
// (C120315_1330: added LMX_TDECL)
//------------------------------------------------------------------------------

LMX_TDECL bool get_sign( const char * & rp_s_in, bool & r_is_neg_out )
{
    r_is_neg_out = false;
    if( *rp_s_in == '-' )
    {
        r_is_neg_out = true;
        ++rp_s_in;
        return true;
    }
    else if( *rp_s_in == '+' )
    {
        ++rp_s_in;
        return true;
    }
    return false;
}

LMX_TDECL bool get_opt_sign( const char * & rp_s_in, bool & r_is_neg_out )
{
    get_sign( rp_s_in, r_is_neg_out );
    return true;
}

LMX_TDECL int get_digits( const char * & rp_s_in, unsigned int & r_out )
{
    int count=0;
    r_out = 0;
    while( xisdigit( *rp_s_in ) )
    {
        r_out *= 10;
        r_out += *rp_s_in - '0';
        ++count;
        ++rp_s_in;
    }
    return count;
}

LMX_TDECL int get_digits( const char * & rp_s_in, int & r_out )
{
    unsigned int out;
    int count = get_digits( rp_s_in, out );
    r_out = out;
    return count;
}

LMX_TDECL int get_decimal( const char * & rp_s_in, float & r_float_out )
{
    double value = 0.0;
    int count = get_decimal( rp_s_in, value );
    r_float_out = static_cast< float >( value );
    return count;
}

LMX_TDECL int get_decimal( const char * & rp_s_in, double & r_double_out )   // For C120927_1127
{
    int count = 0;
    r_double_out = 0;
    while( xisdigit( *rp_s_in ) )
    {
        r_double_out *= 10.0;
        r_double_out += *rp_s_in - '0';
        ++count;
        ++rp_s_in;
    }
    if( *rp_s_in == '.' )
    {
        ++rp_s_in; ++count;
        double scaler = 1.0;
        while( xisdigit( *rp_s_in ) )
        {
            r_double_out *= 10.0;
            r_double_out += *rp_s_in - '0';
            scaler *= 10.0;
            ++count;
            ++rp_s_in;
        }
        r_double_out /= scaler;
    }
    return count;
}

LMX_TDECL bool get_non_ws( const char * & rp_s_in, std::string * p_string_out )
{
    p_string_out->erase();

    while( *rp_s_in != '\0' && ! xisspace( *rp_s_in ) )
        *p_string_out += *rp_s_in++;

    if( ! p_string_out->empty() )
        return true;

    return false;
}

LMX_TDECL bool skip_sign( const char * & rp_s_in )
{
    if( *rp_s_in == '-' || *rp_s_in == '+' )
    {
        ++rp_s_in;
        return true;
    }
    return false;
}

LMX_TDECL bool skip_opt_sign( const char * & rp_s_in )
{
    skip_sign( rp_s_in );
    return true;
}

LMX_TDECL int skip_digits( const char * & rp_s_in )
{
    int count=0;
    while( xisdigit( *rp_s_in ) )
    {
        ++count;
        ++rp_s_in;
    }
    return count;
}

LMX_TDECL int skip_decimal( const char * & rp_s_in )
{
    // ?\d*(.\d*)?  .5 seems valid
    // N.B. Does not skip a leading sign
    int count = 0;
    const char * p_start = rp_s_in;
    while( xisdigit( *rp_s_in ) )
    {
        ++count;
        ++rp_s_in;
    }
    if( *rp_s_in == '.' )
    {
        ++rp_s_in; ++count;
        while( xisdigit( *rp_s_in ) )
        {
            ++count;
            ++rp_s_in;
        }
    }
    if( *p_start == '.' && p_start + 1 == rp_s_in )
        return 0;   // . on its own is not considered a decimal value
    return count;
}

LMX_TDECL bool skip_const( const char * & rp_s_in, const char * p_const_in )
{
    const char * p_start = rp_s_in;
    while( *p_const_in != '\0' && *rp_s_in == *p_const_in )
    {
        ++rp_s_in;
        ++p_const_in;
    }
    if( *p_const_in != '\0' )
    {
        rp_s_in = p_start;
        return false;
    }
    return true;
}

LMX_TDECL bool skip_ws( const char * & rp_s_in )
{
    if( ! xisspace( *rp_s_in ) )
        return false;
    while( xisspace( *rp_s_in ) )
        ++rp_s_in;
    return true;
}

LMX_TDECL bool skip_opt_ws( const char * & rp_s_in )
{
    skip_ws( rp_s_in );
    return true;
}

LMX_TDECL size_t /*n_skipped*/ skip_non_ws( const char * & rp_s_in )
{
    size_t n_skipped = 0;
    while( ! xisspace( *rp_s_in ) )
    {
        ++rp_s_in;
        ++n_skipped;
    }
    return n_skipped;
}

LMX_TDECL bool skip_opt_non_ws( const char * & rp_s_in )
{
    skip_non_ws( rp_s_in );
    return true;
}


} // End of namespace lmx

//------------------------------------------------------------------------------
//                   Copyright (c) 2003-2013 Codalogic Ltd.
//------------------------------------------------------------------------------
