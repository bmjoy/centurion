//------------------------------------------------------------------------------
//                   Copyright (c) 2003-2017 Codalogic Ltd.
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
// Version: 7.2
//      - !C170425_1618: Added operator <<( std::ostream &, const c_binary & )
//        to facilitate output of raw binary data.
//
// Version: 7.1
//      - !C140918_1252: Added call to p_cull_right_hand_zeros() in
//        c_decimal::c_decimal( double value_in, unsigned int precision_in ).
//      - !C141222_1154: Added c_binary::get_as_hex_string() and
//        c_binary::get_as_base64_string().
//      - !C150320_1524: Allow sprintf() etc., to be used to convert floats
//        to c_decimal in non-C locales which may have comma as the decimal
//        point.
//
// Version: 7
//      - !C140606_1347: Culled right-hand digits when setting c_decimal from
//        floating point number.
//
// Version: 6.4
//      - !C130507_1908: Better handling of small valued fractional seconds.
//
// Version: 6.3.1
//      - !C130425_1106: Added c_datetime_base::output_seconds() and modified
//        various iostreams output functions to use it.  This better handles
//        high precision fractional seconds, and small valued fractional
//        seconds.
//
// Version: 6.3
//      - !C130115_0930: Fixed precision of round-tripping double values
//        through c_decimal back to double via c_decimal::get_as_float().
//      - !C130117_1505: c_binary::at( size_t a_i ) const and
//        c_binary::operator[] ( size_t a_i ) const now return a ref to
//        allow use with fwrite() etc.
//      - Updated variable naming convention.
//
//
// Version: 6.1
//      - !C120315_1330: Made internal parser support functions accessible to
//        external code to help with user defined micro formats.
//
// Version: 5.1
//      - !C100415_1549: Corrected sign returned by
//        c_decimal::p_fraction_comp().
//
// Version: 5
//      - Reformatted.
//      - Added inline swap( left, right ) functions.
//      - Added empty() method to c_binary.
//      - Added grow() and non-const back() methods to ct_simple_list.
//
// Version: 4.1
//      - Added operator == () into ct_simple_list.
//
// Version: 3.10
//      - In ct_qname, changed type of o_namespace and user_explicit_ns_prefix
//        to tlmx_string.
//
// Version: 3.9
//      - Modified the output converter functions that they could more easily
//        be overridden when using the Standard Edition.
//
// Version: 3.8.1
//      - On the date and time classes, doing set_offset( 0, 0 ) now stores
//        that the time is UTC.
//
// Version: 3.8
//      - Added an empty() method to ct_qname.
//      - Added #ifndef LMX_NO_WSTRING sections to allow conditional removal
//        of std::wstring from a build.
//      - Added additional wide/narrow string conversion functions.
//
// Version: 3.6
//      - Added support for regular expressions in the pattern class.
//
// Version: 3.3
//      - Added QName helper functions.
//
// Version: 3.2.4
//      - Added the ct_qname template to handle QNames better.
//
// Version: 3.2.3
//      - Added ct_simple_list template for when handling multiple
//        element occurences with lists of simple types.
//      - Fixed handling of fractional seconds less than 10.0 in time,
//        datetime and duration types.
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
// Version: 3.0 (Beta)
//      - Moved xisxxxx() functions from this header file to lmxinternals.h
//      - Modified #include structure so that user can do #include "lmxtype.h"
//        without having to worry about including lmxuser.h instead.
//
// Version: 2.11
//      - No changes
//
// Version: 2.10
//      - c_big_int now returns correct results when compared against "-0"
//      - Now enforce the "C" locale for string to float conversions.
//      - Unicode handling extended beyond the Basic Multi-lingual Plane
//        to full Unicode range (0x-0x10ffff).
//
// Version: 2.9.2
//      - Added switches for MS VC 2005
//
// Version: 2.9
//      - No changes
//
// Version: 2.8
//      - Added string converters for std::string to std::string and
//        std::wstring to std::wstring.  These non-converters make it easier
//        to operate in a environment where _TCHAR type mechanisms are being
//        used.
//      - Added definition of LMX_TDECL if it is not already defined.  During
//        normal compilation this is defined in lmxuser.h, but the extra
//        definition helps MS intellisense work correctly.
//
// Version: 2.7
//      - Moved inittowstring from lmxtypes.cpp to lmxinternals.h to allow
//        control of wide vs. narrow string usage for customers without
//        supporting software source code.
//
// Version: 2.6
//
// Version: 2.5
//
// Acknowledgements:
//      Borland additions kindly provided by Jeff Hay of HYTEC, Inc.,
//      http://www.hytecinc.com.  Find 'BORLANDC' to see additions.
//------------------------------------------------------------------------------


#ifndef LMXUSER_H
// Due to dependencies between lmxuser.h and lmxtypes.h (mainly to do with
// 64-bit values, and c_big_int) lmxtypes.h should be included via lmxuser.h.
// This allows setting up of declspec and user preferences.  Including lmxuser.h
// outside the LMXTYPES_H inclusion guards allows lmxtypes.h to be included by
// lmxuser.h at the right place.  When this include returns LMXTYPES_H will be
// defined, and the remainder of this file will not be included again.
#include "lmxuser.h"
#endif

#ifndef LMXTYPES_H
#define LMXTYPES_H

#if defined( _MSC_VER ) && _MSC_VER > 1000
#pragma once
#pragma warning(disable:4786)
#endif

#if defined( _MSC_VER ) && _MSC_VER >= 1400
#pragma warning (push)
#pragma warning (disable:4996)  // Disable VC8 'safe' warnings
#pragma warning (disable:4100)  // Disable unreferenced formal parameter
#pragma warning (disable:4702)  // Disable unreachable code
#endif

// To help MS intellisense
#if ! defined( LMX_TDECL )
#define LMX_TDECL
#endif

#include <string>
#include <cstdio>
#include <vector>
#include <deque>
#include <iosfwd>
#include <cfloat>
#include <cassert>

#if defined( LMX_VALIDATE_PATTERNS ) && LMX_VALIDATE_PATTERNS == 1
#include "lmxregex.h"
#endif

namespace lmx {

//------------------------------------------------------------------------------
//                      Basic validation prototypes
//------------------------------------------------------------------------------

#ifndef LMX_NO_WSTRING
LMX_TDECL std::wstring & vtostring( std::wstring & r_dest_out, const std::string & r_src_in );
LMX_TDECL size_t length( const std::wstring & r_utf16_string_in );  // v6.2 - C121028_1228
#endif
LMX_TDECL std::string & vtostring( std::string & r_dest_out, const std::string & r_src_in );
LMX_TDECL size_t length( const std::string & r_utf8_string_in );  // v3.1
LMX_TDECL bool is_min_length_ok( const std::string & r_utf8_string_in, size_t length_in );  // v3.1
LMX_TDECL bool is_max_length_ok( const std::string & r_utf8_string_in, size_t length_in );  // v3.1
LMX_TDECL bool is_valid_bool( const std::string & r_s_in );
LMX_TDECL bool is_valid_float( const std::string & r_s_in );
LMX_TDECL bool is_valid_integer( const std::string & m_value );
LMX_TDECL int total_digits( const std::string & r_value_in );
LMX_TDECL int fraction_digits( const std::string & r_value_in );

//------------------------------------------------------------------------------
//                  Prototypes for parser support functions
// Included here so they can be used in custom user micro formats. C120315_1330
//------------------------------------------------------------------------------

LMX_TDECL bool get_sign( const char * & rp_s_in, bool & r_is_neg_out );
LMX_TDECL bool get_opt_sign( const char * & rp_s_in, bool & r_is_neg_out );
LMX_TDECL int get_digits( const char * & rp_s_in, unsigned int & r_int_out );
LMX_TDECL int get_digits( const char * & rp_s_in, int & r_int_out );
LMX_TDECL int get_decimal( const char * & rp_s_in, float & r_float_out );
LMX_TDECL int get_decimal( const char * & rp_s_in, double & r_float_out );
LMX_TDECL bool get_non_ws( const char * & rp_s_in, std::string * p_string_out );
LMX_TDECL bool skip_sign( const char * & rp_s_in );
LMX_TDECL bool skip_opt_sign( const char * & rp_s_in );
LMX_TDECL int skip_digits( const char * & rp_s_in );
LMX_TDECL int skip_decimal( const char * & rp_s_in );
LMX_TDECL bool skip_const( const char * & rp_s_in, const char * p_const_in );
LMX_TDECL bool skip_ws( const char * & rp_s_in );
LMX_TDECL bool skip_opt_ws( const char * & rp_s_in );
LMX_TDECL size_t /*n_skipped*/ skip_non_ws( const char * & rp_s_in );
LMX_TDECL bool skip_opt_non_ws( const char * & rp_s_in );

//------------------------------------------------------------------------------
//        Template for multiple occurrence of simple type lists - V3.2.3
//------------------------------------------------------------------------------

template< typename T >
class ct_simple_list
{
private:
    std::deque< T > m_store;  // Base on deque so that it can contain bool members

public:
    ct_simple_list()
        {}
    ct_simple_list( const ct_simple_list<T> & r_rhs_in )
        : m_store( r_rhs_in.m_store )
        {}
    virtual ~ct_simple_list()
        {}
    size_t size() const
        { return m_store.size(); }
    const T & get( size_t i ) const
        { return m_store[i]; }
    const T & back() const
        { return m_store.back(); }
    T & back()      // v5
        { return m_store.back(); }
    void set( size_t i, const T & a )
        { return m_store[i] = a; }
    void append( const T & a )
        { m_store.push_back( a ); }
    void grow()     // v5
        { m_store.resize( m_store.size() + 1 ); }
    void clear()
        { m_store.erase(); }
    ct_simple_list<T> & operator = ( const ct_simple_list<T> & r_rhs_in )
        { ct_simple_list<T> temp( r_rhs_in ); swap( temp ); return *this; }
    bool operator == ( const ct_simple_list<T> & r_rhs_in ) const // v4.1
    {
        if( m_store.size() != r_rhs_in.m_store.size() )
            return false;
        for( size_t i=0; i<m_store.size(); ++i )
            if( m_store[i] != r_rhs_in.m_store[i] )
                return false;
        return true;
    }
    bool operator != ( const ct_simple_list<T> & r_rhs_in ) const // v4.1
        { return ! (*this == r_rhs_in); }
    void swap( ct_simple_list<T> & r_rhs_in )
        { m_store.swap( r_rhs_in.m_store ); }
};

template< class T >
inline void swap( ct_simple_list<T> & r_lhs_in, ct_simple_list<T> & r_rhs_in ) { r_lhs_in.swap( r_rhs_in ); }   // Added V5

//------------------------------------------------------------------------------
//                       Schema type specific classes
//------------------------------------------------------------------------------

class c_big_int
{
private:
    std::string m_value;

    void p_init( const char * p_string_in );

public:
    LMX_TDECL c_big_int();
    LMX_TDECL c_big_int( const c_big_int & r_big_int_in );
    LMX_TDECL c_big_int( const char * p_string_in );
    LMX_TDECL c_big_int( const std::string & r_string_in );
    LMX_TDECL c_big_int( long long_in );
    LMX_TDECL c_big_int & operator = ( const c_big_int & r_big_int_in );
    LMX_TDECL c_big_int & operator = ( const char * p_string_in );
    LMX_TDECL c_big_int & operator = ( const std::string & r_string_in );
    LMX_TDECL c_big_int & operator = ( long long_in );
    LMX_TDECL ~c_big_int() {}
    LMX_TDECL void swap( c_big_int & r_big_int_in ) { m_value.swap( r_big_int_in.m_value ); }

    // This routine is unable to convert to the maximum negative value
    LMX_TDECL long get( bool * p_is_overflowed_out = LMXNULL ) const;
    LMX_TDECL double get_as_float() const;
    LMX_TDECL std::string get_as_string() const;

    // Includes versions to prevent the need to create temporary big_ints and strings
    LMX_TDECL friend bool operator == ( const c_big_int & r_lhs_in, const char * p_rhs_in );
    LMX_TDECL friend bool operator != ( const c_big_int & r_lhs_in, const char * p_rhs_in );
    LMX_TDECL friend bool operator > ( const c_big_int & r_lhs_in, const char * p_rhs_in );
    LMX_TDECL friend bool operator >= ( const c_big_int & r_lhs_in, const char * p_rhs_in );
    LMX_TDECL friend bool operator < ( const c_big_int & r_lhs_in, const char * p_rhs_in );
    LMX_TDECL friend bool operator <= ( const c_big_int & r_lhs_in, const char * p_rhs_in );
    LMX_TDECL friend bool operator == ( const c_big_int & r_lhs_in, const std::string & r_value_in );
    LMX_TDECL friend bool operator != ( const c_big_int & r_lhs_in, const std::string & r_value_in );
    LMX_TDECL friend bool operator > ( const c_big_int & r_lhs_in, const std::string & r_value_in );
    LMX_TDECL friend bool operator >= ( const c_big_int & r_lhs_in, const std::string & r_value_in );
    LMX_TDECL friend bool operator < ( const c_big_int & r_lhs_in, const std::string & r_value_in );
    LMX_TDECL friend bool operator <= ( const c_big_int & r_lhs_in, const std::string & r_value_in );
    LMX_TDECL friend bool operator == ( const std::string & r_value_in, const c_big_int & r_rhs_in );
    LMX_TDECL friend bool operator != ( const std::string & r_value_in, const c_big_int & r_rhs_in );
    LMX_TDECL friend bool operator > ( const std::string & r_value_in, const c_big_int & r_rhs_in );
    LMX_TDECL friend bool operator >= ( const std::string & r_value_in, const c_big_int & r_rhs_in );
    LMX_TDECL friend bool operator < ( const std::string & r_value_in, const c_big_int & r_rhs_in );
    LMX_TDECL friend bool operator <= ( const std::string & r_value_in, const c_big_int & r_rhs_in );
    LMX_TDECL friend bool operator == ( const char * p_lhs_in, const c_big_int & r_rhs_in );
    LMX_TDECL friend bool operator != ( const char * p_lhs_in, const c_big_int & r_rhs_in );
    LMX_TDECL friend bool operator > ( const char * p_lhs_in, const c_big_int & r_rhs_in );
    LMX_TDECL friend bool operator >= ( const char * p_lhs_in, const c_big_int & r_rhs_in );
    LMX_TDECL friend bool operator < ( const char * p_lhs_in, const c_big_int & r_rhs_in );
    LMX_TDECL friend bool operator <= ( const char * p_lhs_in, const c_big_int & r_rhs_in );
    LMX_TDECL friend bool operator == ( const c_big_int & r_lhs_in, const c_big_int & r_rhs_in );
    LMX_TDECL friend bool operator != ( const c_big_int & r_lhs_in, const c_big_int & r_rhs_in );
    LMX_TDECL friend bool operator > ( const c_big_int & r_lhs_in, const c_big_int & r_rhs_in );
    LMX_TDECL friend bool operator >= ( const c_big_int & r_lhs_in, const c_big_int & r_rhs_in );
    LMX_TDECL friend bool operator < ( const c_big_int & r_lhs_in, const c_big_int & r_rhs_in );
    LMX_TDECL friend bool operator <= ( const c_big_int & r_lhs_in, const c_big_int & r_rhs_in );

    LMX_TDECL friend std::ostream & operator << ( std::ostream & r_os_in, const c_big_int & r_rhs_in );

    friend int test_c_big_int1();
    friend int test_c_big_int2();
};

inline void swap( c_big_int & r_lhs_in, c_big_int & r_rhs_in ) { r_lhs_in.swap( r_rhs_in ); }   // Added V5

class c_decimal
{
private:
    bool m_is_neg;
    c_big_int m_left;
    std::string m_right;

    enum e_decimal_separator_handling { DOT_ONLY, INCLUDE_SYSTEM_LOCALE };
    void p_init( const char *, e_decimal_separator_handling decimal_separator_handling = DOT_ONLY );   // C150320_1524
    bool p_skip_decimal_separator( const char * & rp_s_in, e_decimal_separator_handling decimal_separator_handling );
    static int p_fraction_comp( const std::string & r_lhs_in, const std::string & r_rhs_in );
    void p_cull_right_hand_zeros(); // C140606_1347

public:
    LMX_TDECL c_decimal();
    LMX_TDECL c_decimal( const c_decimal & );
    LMX_TDECL c_decimal( const std::string & );
    LMX_TDECL c_decimal( const char * );
    LMX_TDECL c_decimal( long value_in, int scale_in = 0 );
    LMX_TDECL c_decimal( double value_in ); // C130115_0930
    LMX_TDECL c_decimal( double value_in, unsigned int precision_in );
    LMX_TDECL ~c_decimal() {}
    LMX_TDECL c_decimal & operator = ( const c_decimal & );
    LMX_TDECL c_decimal & operator = ( const char * );
    LMX_TDECL c_decimal & operator = ( const std::string & r_s_in ) { return operator = ( r_s_in.c_str() ); }
    LMX_TDECL c_decimal & operator = ( double value_in );
    LMX_TDECL void swap( c_decimal & );

    LMX_TDECL static bool is_valid( const std::string & );

    LMX_TDECL long get_integral( bool * p_is_overflowed_out = LMXNULL ) const;
    LMX_TDECL unsigned long get_fractional( int precision_in, bool * p_is_overflowed_out = LMXNULL ) const;
    LMX_TDECL long get_scaled( int scale_in, bool * p_is_overflowed_out = LMXNULL ) const;
    LMX_TDECL double get_as_float() const;
    LMX_TDECL std::string get_as_string() const;

    LMX_TDECL void set( double value_in, unsigned int precision_in = DBL_DIG );
    LMX_TDECL void set_scaled( long v_in, int scale_in = 0 );
    LMX_TDECL void set_fractional( unsigned long v_in, unsigned int precision_in );
    LMX_TDECL void set_integral( long v_in );

    LMX_TDECL friend bool operator == ( const c_decimal &, const c_decimal & );
    LMX_TDECL friend bool operator != ( const c_decimal &, const c_decimal & );
    LMX_TDECL friend bool operator > ( const c_decimal &, const c_decimal & );
    LMX_TDECL friend bool operator >= ( const c_decimal &, const c_decimal & );
    LMX_TDECL friend bool operator < ( const c_decimal &, const c_decimal & );
    LMX_TDECL friend bool operator <= ( const c_decimal &, const c_decimal & );

    LMX_TDECL friend std::ostream & operator << ( std::ostream &, const c_decimal & );

    LMX_TDECL friend int test_c_decimal();
};

inline void swap( c_decimal & r_lhs_in, c_decimal & r_rhs_in ) { r_lhs_in.swap( r_rhs_in ); }   // Added V5

class c_duration
{
private:
    bool m_is_neg;
    unsigned int m_years;
    unsigned int m_months;
    unsigned int m_days;
    unsigned int m_hours;
    unsigned int m_mins;
    tlmx_precision_seconds m_secs;    // C120927_1127
    static const unsigned int m_day_mons_mins[];
    static const unsigned int m_day_mons_maxs[];

    bool p_is_indeterminate( const c_duration & r_rhs_in ) const;
    bool p_is_greater_than( const c_duration & r_rhs_in ) const;
    bool p_is_equal( const c_duration & r_rhs_in ) const;

public:
    LMX_TDECL c_duration();
    // c_duration( const c_duration & );    // Default copy constructor & assignment operator is fine
    LMX_TDECL c_duration( const std::string & );
    LMX_TDECL c_duration( const char * );
    LMX_TDECL ~c_duration() {}
    LMX_TDECL c_duration & operator = ( const char * );
    LMX_TDECL c_duration & operator = ( const std::string & r_s_in ) { return operator = ( r_s_in.c_str() ); }

    LMX_TDECL static bool is_valid( const char * );
    LMX_TDECL static bool is_valid( const std::string & r_s_in ) { return is_valid( r_s_in.c_str() ); }

    LMX_TDECL bool get_is_neg() const { return m_is_neg; }
    LMX_TDECL unsigned int get_years() const { return m_years; }
    LMX_TDECL unsigned int get_months() const { return m_months; }
    LMX_TDECL unsigned int get_days() const { return m_days; }
    LMX_TDECL unsigned int get_hours() const { return m_hours; }
    LMX_TDECL unsigned int get_mins() const { return m_mins; }
    LMX_TDECL tlmx_precision_seconds get_secs() const { return m_secs; }
    LMX_TDECL std::string get_as_string() const;

    LMX_TDECL void set_is_neg( bool is_neg_in ) { m_is_neg = is_neg_in; }
    LMX_TDECL void set_years( unsigned int years_in ) { m_years = years_in; }
    LMX_TDECL void set_months( unsigned int months_in ) { m_months = months_in; }
    LMX_TDECL void set_days( unsigned int days_in ) { m_days = days_in; }
    LMX_TDECL void set_hours( unsigned int hours_in ) { m_hours = hours_in; }
    LMX_TDECL void set_mins( unsigned int mins_in ) { m_mins = mins_in; }
    LMX_TDECL void set_secs( tlmx_precision_seconds secs_in ) { lmx_assert( secs_in >= 0.0 ); m_secs = secs_in; }

    LMX_TDECL void normalize();

    LMX_TDECL friend bool operator == ( const c_duration & r_lhs_in, const c_duration & r_rhs_in );
    LMX_TDECL friend bool operator != ( const c_duration & r_lhs_in, const c_duration & r_rhs_in );
    LMX_TDECL friend bool operator > ( const c_duration & r_lhs_in, const c_duration & r_rhs_in );
    LMX_TDECL friend bool operator >= ( const c_duration & r_lhs_in, const c_duration & r_rhs_in );
    LMX_TDECL friend bool operator < ( const c_duration & r_lhs_in, const c_duration & r_rhs_in );
    LMX_TDECL friend bool operator <= ( const c_duration & r_lhs_in, const c_duration & r_rhs_in );

    LMX_TDECL friend std::ostream & operator << ( std::ostream &, const c_duration & );

    LMX_TDECL friend int test_c_duration();
};

class c_datetime_base
{
protected:
    int m_year;      // Year can be negative
    unsigned int m_month;
    unsigned int m_day;
    unsigned int m_hours;
    unsigned int m_mins;
    tlmx_precision_seconds m_secs;    // C120927_1127
    int m_off_mins;   // Combination of offset hours and offset minutes
    bool m_is_utc;

public:
    LMX_TDECL c_datetime_base();

protected:
    bool is_equal( const c_datetime_base & r_rhs_in ) const;
    bool is_greater_than( const c_datetime_base & r_rhs_in ) const;
    void set_tz( const char * p_s_in );
    static bool is_valid_tz( const char * & rp_s_in );
    std::ostream & output_seconds( std::ostream & r_os_in ) const;  // C130425_1106
    std::ostream & output_tz( std::ostream & r_os_in ) const;
    void set_offset( int off_hours_in, unsigned int off_mins_in )
    {
        if( off_hours_in >= 0 )
            m_off_mins = off_hours_in * 60 + off_mins_in;
        else
            m_off_mins = off_hours_in * 60 - off_mins_in;
        m_is_utc = (m_off_mins == 0);   // Modified in V3.8.1
    }

    ~c_datetime_base() {}
};

class c_datetime : protected c_datetime_base
{
public:
    LMX_TDECL c_datetime();
    // c_datetime( const c_datetime & );                    // Default OK
    LMX_TDECL c_datetime( const char * );
    LMX_TDECL c_datetime( const std::string & );
    // c_datetime & operator = ( const c_datetime & );  // Default OK
    LMX_TDECL c_datetime & operator = ( const char * );
    LMX_TDECL c_datetime & operator = ( const std::string & );
    LMX_TDECL ~c_datetime() {}

    LMX_TDECL static bool is_valid( const std::string & );

    LMX_TDECL int get_year() const { return m_year; }
    LMX_TDECL unsigned int get_month() const { return m_month; }
    LMX_TDECL unsigned int get_day() const { return m_day; }
    LMX_TDECL unsigned int get_hours() const { return m_hours; }
    LMX_TDECL unsigned int get_mins() const { return m_mins; }
    LMX_TDECL tlmx_precision_seconds get_secs() const { return m_secs; }
    LMX_TDECL int get_total_offset_mins() const { return m_off_mins; }  // Hours * 60 + Mins
    LMX_TDECL bool get_is_utc() const { return m_is_utc; }
    LMX_TDECL std::string get_as_string() const;

    LMX_TDECL void set_year( int year_in ) { m_year = year_in; }
    LMX_TDECL void set_month( unsigned int month_in ) { m_month = month_in; }
    LMX_TDECL void set_day( unsigned int day_in ) { m_day = day_in; }
    LMX_TDECL void set_hours( unsigned int hours_in ) { m_hours = hours_in; }
    LMX_TDECL void set_mins( unsigned int mins_in ) { m_mins = mins_in; }
    LMX_TDECL void set_secs( tlmx_precision_seconds secs_in ) { lmx_assert( secs_in >= 0.0 ); m_secs = secs_in; }
    LMX_TDECL void set_total_offset_mins( int off_mins_in ) { m_off_mins = off_mins_in; } // Hours * 60 + Mins
    LMX_TDECL void set_offset_mins( int off_hours_in, unsigned int off_mins_in )    // Deprecated
        { c_datetime_base::set_offset( off_hours_in, off_mins_in ); }
    LMX_TDECL void set_offset( int off_hours_in, unsigned int off_mins_in )
        { c_datetime_base::set_offset( off_hours_in, off_mins_in ); }
    LMX_TDECL void set_is_utc( bool is_utc_in ) { m_is_utc = is_utc_in; }

    LMX_TDECL friend bool operator == ( const c_datetime & r_lhs_in, const c_datetime & r_rhs_in );
    LMX_TDECL friend bool operator != ( const c_datetime & r_lhs_in, const c_datetime & r_rhs_in );
    LMX_TDECL friend bool operator > ( const c_datetime & r_lhs_in, const c_datetime & r_rhs_in );
    LMX_TDECL friend bool operator >= ( const c_datetime & r_lhs_in, const c_datetime & r_rhs_in );
    LMX_TDECL friend bool operator < ( const c_datetime & r_lhs_in, const c_datetime & r_rhs_in );
    LMX_TDECL friend bool operator <= ( const c_datetime & r_lhs_in, const c_datetime & r_rhs_in );

    LMX_TDECL friend std::ostream & operator << ( std::ostream &, const c_datetime & );
};

class c_time : protected c_datetime_base
{
public:
    LMX_TDECL c_time();
    // c_time( const c_time & );                    // Default OK
    LMX_TDECL c_time( const char * );
    LMX_TDECL c_time( const std::string & );
    LMX_TDECL ~c_time() {}
    // c_time & operator = ( const c_time & );  // Default OK
    LMX_TDECL c_time & operator = ( const char * );
    LMX_TDECL c_time & operator = ( const std::string & );

    LMX_TDECL static bool is_valid( const std::string & );

    LMX_TDECL unsigned int get_hours() const { return m_hours; }
    LMX_TDECL unsigned int get_mins() const { return m_mins; }
    LMX_TDECL tlmx_precision_seconds get_secs() const { return m_secs; }
    LMX_TDECL int get_total_offset_mins() const { return m_off_mins; }  // Hours * 60 + Mins
    LMX_TDECL bool get_is_utc() const { return m_is_utc; }
    LMX_TDECL std::string get_as_string() const;

    LMX_TDECL void set_hours( unsigned int hours_in ) { m_hours = hours_in; }
    LMX_TDECL void set_mins( unsigned int mins_in ) { m_mins = mins_in; }
    LMX_TDECL void set_secs( tlmx_precision_seconds secs_in ) { lmx_assert( secs_in >= 0.0 ); m_secs = secs_in; }
    LMX_TDECL void set_total_offset_mins( int off_mins_in ) { m_off_mins = off_mins_in; }
    LMX_TDECL void set_offset_mins( int off_hours_in, unsigned int off_mins_in )    // Deprecated
        { c_datetime_base::set_offset( off_hours_in, off_mins_in ); }
    LMX_TDECL void set_offset( int off_hours_in, unsigned int off_mins_in )
        { c_datetime_base::set_offset( off_hours_in, off_mins_in ); }
    LMX_TDECL void set_is_utc( bool is_utc_in ) { m_is_utc = is_utc_in; }

    LMX_TDECL friend bool operator == ( const c_time & r_lhs_in, const c_time & r_rhs_in );
    LMX_TDECL friend bool operator != ( const c_time & r_lhs_in, const c_time & r_rhs_in );
    LMX_TDECL friend bool operator > ( const c_time & r_lhs_in, const c_time & r_rhs_in );
    LMX_TDECL friend bool operator >= ( const c_time & r_lhs_in, const c_time & r_rhs_in );
    LMX_TDECL friend bool operator < ( const c_time & r_lhs_in, const c_time & r_rhs_in );
    LMX_TDECL friend bool operator <= ( const c_time & r_lhs_in, const c_time & r_rhs_in );

    LMX_TDECL friend std::ostream & operator << ( std::ostream &, const c_time & );
};

class c_date : protected c_datetime_base
{
public:
    LMX_TDECL c_date();
    // c_date( const c_date & );                    // Default OK
    LMX_TDECL c_date( const char * );
    LMX_TDECL c_date( const std::string & );
    // c_date & operator = ( const c_date & );  // Default OK
    LMX_TDECL ~c_date() {}
    LMX_TDECL c_date & operator = ( const char * );
    LMX_TDECL c_date & operator = ( const std::string & );

    LMX_TDECL static bool is_valid( const std::string & );

    LMX_TDECL int get_year() const { return m_year; }
    LMX_TDECL unsigned int get_month() const { return m_month; }
    LMX_TDECL unsigned int get_day() const { return m_day; }
    LMX_TDECL int get_total_offset_mins() const { return m_off_mins; }  // Hours * 60 + Mins
    LMX_TDECL bool get_is_utc() const { return m_is_utc; }
    LMX_TDECL std::string get_as_string() const;

    LMX_TDECL void set_year( int year_in ) { m_year = year_in; }
    LMX_TDECL void set_month( unsigned int month_in ) { m_month = month_in; }
    LMX_TDECL void set_day( unsigned int day_in ) { m_day = day_in; }
    LMX_TDECL void set_total_offset_mins( int off_mins_in ) { m_off_mins = off_mins_in; }
    LMX_TDECL void set_offset_mins( int off_hours_in, unsigned int off_mins_in )    // Deprecated
        { c_datetime_base::set_offset( off_hours_in, off_mins_in ); }
    LMX_TDECL void set_offset( int off_hours_in, unsigned int off_mins_in )
        { c_datetime_base::set_offset( off_hours_in, off_mins_in ); }
    LMX_TDECL void set_is_utc( bool is_utc_in ) { m_is_utc = is_utc_in; }

    LMX_TDECL friend bool operator == ( const c_date & r_lhs_in, const c_date & r_rhs_in );
    LMX_TDECL friend bool operator != ( const c_date & r_lhs_in, const c_date & r_rhs_in );
    LMX_TDECL friend bool operator > ( const c_date & r_lhs_in, const c_date & r_rhs_in );
    LMX_TDECL friend bool operator >= ( const c_date & r_lhs_in, const c_date & r_rhs_in );
    LMX_TDECL friend bool operator < ( const c_date & r_lhs_in, const c_date & r_rhs_in );
    LMX_TDECL friend bool operator <= ( const c_date & r_lhs_in, const c_date & r_rhs_in );

    LMX_TDECL friend std::ostream & operator << ( std::ostream &, const c_date & );
};

class c_gyearmonth : protected c_datetime_base
{
public:
    LMX_TDECL c_gyearmonth();
    // c_gyearmonth( const c_gyearmonth & );                // Default OK
    LMX_TDECL c_gyearmonth( const char * );
    LMX_TDECL c_gyearmonth( const std::string & );
    LMX_TDECL ~c_gyearmonth() {}
    // c_gyearmonth & operator = ( const c_gyearmonth & );// Default OK
    LMX_TDECL c_gyearmonth & operator = ( const char * );
    LMX_TDECL c_gyearmonth & operator = ( const std::string & );

    LMX_TDECL static bool is_valid( const std::string & );

    LMX_TDECL int get_year() const { return m_year; }
    LMX_TDECL unsigned int get_month() const { return m_month; }
    LMX_TDECL int get_total_offset_mins() const { return m_off_mins; }          // Hours * 60 + Mins
    LMX_TDECL bool get_is_utc() const { return m_is_utc; }
    LMX_TDECL std::string get_as_string() const;

    LMX_TDECL void set_year( int year_in ) { m_year = year_in; }
    LMX_TDECL void set_month( unsigned int month_in ) { m_month = month_in; }
    LMX_TDECL void set_total_offset_mins( int off_mins_in ) { m_off_mins = off_mins_in; }
    LMX_TDECL void set_offset_mins( int off_hours_in, unsigned int off_mins_in )    // Deprecated
        { c_datetime_base::set_offset( off_hours_in, off_mins_in ); }
    LMX_TDECL void set_offset( int off_hours_in, unsigned int off_mins_in )
        { c_datetime_base::set_offset( off_hours_in, off_mins_in ); }
    LMX_TDECL void set_is_utc( bool is_utc_in ) { m_is_utc = is_utc_in; }

    LMX_TDECL friend bool operator == ( const c_gyearmonth & r_lhs_in, const c_gyearmonth & r_rhs_in );
    LMX_TDECL friend bool operator != ( const c_gyearmonth & r_lhs_in, const c_gyearmonth & r_rhs_in );
    LMX_TDECL friend bool operator > ( const c_gyearmonth & r_lhs_in, const c_gyearmonth & r_rhs_in );
    LMX_TDECL friend bool operator >= ( const c_gyearmonth & r_lhs_in, const c_gyearmonth & r_rhs_in );
    LMX_TDECL friend bool operator < ( const c_gyearmonth & r_lhs_in, const c_gyearmonth & r_rhs_in );
    LMX_TDECL friend bool operator <= ( const c_gyearmonth & r_lhs_in, const c_gyearmonth & r_rhs_in );

    LMX_TDECL friend std::ostream & operator << ( std::ostream &, const c_gyearmonth & );
};

class c_gyear : protected c_datetime_base
{
public:
    LMX_TDECL c_gyear();
    // c_gyear( const c_gyear & );              // Default OK
    LMX_TDECL c_gyear( const char * );
    LMX_TDECL c_gyear( const std::string & );
    LMX_TDECL ~c_gyear() {}
    // c_gyear & operator = ( const cmlx_gyear & ); // Default OK
    LMX_TDECL c_gyear & operator = ( const char * );
    LMX_TDECL c_gyear & operator = ( const std::string & );

    LMX_TDECL static bool is_valid( const std::string & );

    LMX_TDECL int get_year() const { return m_year; }
    LMX_TDECL int get_total_offset_mins() const { return m_off_mins; }  // Hours * 60 + Mins
    LMX_TDECL bool get_is_utc() const { return m_is_utc; }
    LMX_TDECL std::string get_as_string() const;

    LMX_TDECL void set_year( int year_in ) { m_year = year_in; }
    LMX_TDECL void set_total_offset_mins( int off_mins_in ) { m_off_mins = off_mins_in; }
    LMX_TDECL void set_offset_mins( int off_hours_in, unsigned int off_mins_in )    // Deprecated
        { c_datetime_base::set_offset( off_hours_in, off_mins_in ); }
    LMX_TDECL void set_offset( int off_hours_in, unsigned int off_mins_in )
        { c_datetime_base::set_offset( off_hours_in, off_mins_in ); }
    LMX_TDECL void set_is_utc( bool is_utc_in ) { m_is_utc = is_utc_in; }

    LMX_TDECL friend bool operator == ( const c_gyear & r_lhs_in, const c_gyear & r_rhs_in );
    LMX_TDECL friend bool operator != ( const c_gyear & r_lhs_in, const c_gyear & r_rhs_in );
    LMX_TDECL friend bool operator > ( const c_gyear & r_lhs_in, const c_gyear & r_rhs_in );
    LMX_TDECL friend bool operator >= ( const c_gyear & r_lhs_in, const c_gyear & r_rhs_in );
    LMX_TDECL friend bool operator < ( const c_gyear & r_lhs_in, const c_gyear & r_rhs_in );
    LMX_TDECL friend bool operator <= ( const c_gyear & r_lhs_in, const c_gyear & r_rhs_in );

    LMX_TDECL friend std::ostream & operator << ( std::ostream &, const c_gyear& );
};

class c_gmonthday : protected c_datetime_base
{
public:
    LMX_TDECL c_gmonthday();
    // c_gmonthday( const c_gmonthday & );              // Default OK
    LMX_TDECL c_gmonthday( const char * );
    LMX_TDECL c_gmonthday( const std::string & );
    LMX_TDECL ~c_gmonthday() {}
    // c_gmonthday & operator = ( const c_gmonthday & );    // Default OK
    LMX_TDECL c_gmonthday & operator = ( const char * );
    LMX_TDECL c_gmonthday & operator = ( const std::string & );

    LMX_TDECL static bool is_valid( const std::string & );

    LMX_TDECL unsigned int get_month() const { return m_month; }
    LMX_TDECL unsigned int get_day() const { return m_day; }
    LMX_TDECL int get_total_offset_mins() const { return m_off_mins; }          // Hours * 60 + Mins
    LMX_TDECL bool get_is_utc() const { return m_is_utc; }
    LMX_TDECL std::string get_as_string() const;

    LMX_TDECL void set_month( unsigned int month_in ) { m_month = month_in; }
    LMX_TDECL void set_day( unsigned int day_in ) { m_day = day_in; }
    LMX_TDECL void set_total_offset_mins( int off_mins_in ) { m_off_mins = off_mins_in; }
    LMX_TDECL void set_offset_mins( int off_hours_in, unsigned int off_mins_in )    // Deprecated
        { c_datetime_base::set_offset( off_hours_in, off_mins_in ); }
    LMX_TDECL void set_offset( int off_hours_in, unsigned int off_mins_in )
        { c_datetime_base::set_offset( off_hours_in, off_mins_in ); }
    LMX_TDECL void set_is_utc( bool is_utc_in ) { m_is_utc = is_utc_in; }

    LMX_TDECL friend bool operator == ( const c_gmonthday & r_lhs_in, const c_gmonthday & r_rhs_in );
    LMX_TDECL friend bool operator != ( const c_gmonthday & r_lhs_in, const c_gmonthday & r_rhs_in );
    LMX_TDECL friend bool operator > ( const c_gmonthday & r_lhs_in, const c_gmonthday & r_rhs_in );
    LMX_TDECL friend bool operator >= ( const c_gmonthday & r_lhs_in, const c_gmonthday & r_rhs_in );
    LMX_TDECL friend bool operator < ( const c_gmonthday & r_lhs_in, const c_gmonthday & r_rhs_in );
    LMX_TDECL friend bool operator <= ( const c_gmonthday & r_lhs_in, const c_gmonthday & r_rhs_in );

    LMX_TDECL friend std::ostream & operator << ( std::ostream &, const c_gmonthday & );
};

class c_gday : protected c_datetime_base
{
public:
    LMX_TDECL c_gday();
    // c_gday( const c_gday & );                    // Default OK
    LMX_TDECL c_gday( const char * );
    LMX_TDECL c_gday( const std::string & );
    LMX_TDECL ~c_gday() {}
    // c_gday & operator = ( const c_gday & );  // Default OK
    LMX_TDECL c_gday & operator = ( const char * );
    LMX_TDECL c_gday & operator = ( const std::string & );

    LMX_TDECL static bool is_valid( const std::string & );

    LMX_TDECL unsigned int get_day() const { return m_day; }
    LMX_TDECL int get_total_offset_mins() const { return m_off_mins; }  // Hours * 60 + Mins
    LMX_TDECL bool get_is_utc() const { return m_is_utc; }
    LMX_TDECL std::string get_as_string() const;

    LMX_TDECL void set_day( unsigned int day_in ) { m_day = day_in; }
    LMX_TDECL void set_total_offset_mins( int off_mins_in ) { m_off_mins = off_mins_in; }
    LMX_TDECL void set_offset_mins( int off_hours_in, unsigned int off_mins_in )    // Deprecated
        { c_datetime_base::set_offset( off_hours_in, off_mins_in ); }
    LMX_TDECL void set_offset( int off_hours_in, unsigned int off_mins_in )
        { c_datetime_base::set_offset( off_hours_in, off_mins_in ); }
    LMX_TDECL void set_is_utc( bool is_utc_in ) { m_is_utc = is_utc_in; }

    LMX_TDECL friend bool operator == ( const c_gday & r_lhs_in, const c_gday & r_rhs_in );
    LMX_TDECL friend bool operator != ( const c_gday & r_lhs_in, const c_gday & r_rhs_in );
    LMX_TDECL friend bool operator > ( const c_gday & r_lhs_in, const c_gday & r_rhs_in );
    LMX_TDECL friend bool operator >= ( const c_gday & r_lhs_in, const c_gday & r_rhs_in );
    LMX_TDECL friend bool operator < ( const c_gday & r_lhs_in, const c_gday & r_rhs_in );
    LMX_TDECL friend bool operator <= ( const c_gday & r_lhs_in, const c_gday & r_rhs_in );

    LMX_TDECL friend std::ostream & operator << ( std::ostream &, const c_gday & );
};

class c_gmonth : protected c_datetime_base
{
public:
    LMX_TDECL c_gmonth();
    // c_gmonth( const c_gmonth & );                    // Default OK
    LMX_TDECL c_gmonth( const char * );
    LMX_TDECL c_gmonth( const std::string & );
    LMX_TDECL ~c_gmonth() {}
    // c_gmonth & operator = ( const c_gmonth & );  // Default OK
    LMX_TDECL c_gmonth & operator = ( const char * );
    LMX_TDECL c_gmonth & operator = ( const std::string & );

    LMX_TDECL static bool is_valid( const std::string & );

    LMX_TDECL std::string get_as_string() const;
    LMX_TDECL unsigned int get_month() const { return m_month; }
    LMX_TDECL int get_total_offset_mins() const { return m_off_mins; }      // Hours * 60 + Mins
    LMX_TDECL bool get_is_utc() const { return m_is_utc; }

    LMX_TDECL void set_month( unsigned int month_in ) { m_month = month_in; }
    LMX_TDECL void set_total_offset_mins( int off_mins_in ) { m_off_mins = off_mins_in; }
    LMX_TDECL void set_offset_mins( int off_hours_in, unsigned int off_mins_in )    // Deprecated
        { c_datetime_base::set_offset( off_hours_in, off_mins_in ); }
    LMX_TDECL void set_offset( int off_hours_in, unsigned int off_mins_in )
        { c_datetime_base::set_offset( off_hours_in, off_mins_in ); }
    LMX_TDECL void set_is_utc( bool is_utc_in ) { m_is_utc = is_utc_in; }

    LMX_TDECL friend bool operator == ( const c_gmonth & r_lhs_in, const c_gmonth & r_rhs_in );
    LMX_TDECL friend bool operator != ( const c_gmonth & r_lhs_in, const c_gmonth & r_rhs_in );
    LMX_TDECL friend bool operator > ( const c_gmonth & r_lhs_in, const c_gmonth & r_rhs_in );
    LMX_TDECL friend bool operator >= ( const c_gmonth & r_lhs_in, const c_gmonth & r_rhs_in );
    LMX_TDECL friend bool operator < ( const c_gmonth & r_lhs_in, const c_gmonth & r_rhs_in );
    LMX_TDECL friend bool operator <= ( const c_gmonth & r_lhs_in, const c_gmonth & r_rhs_in );

    LMX_TDECL friend std::ostream & operator << ( std::ostream &, const c_gmonth & );
};

class c_binary
{
private:
    std::vector<unsigned char> m_value;

    static unsigned char p_base64_char_to_6_bits( int c_in );
    void p_base64_convert_24bits_to_bytes( tlmx_uns32 accumulated_in, size_t number_accumulated_in );
    static char p_6_bits_to_base64_char( int c_in );
    static void p_convert_24bits_to_4x6bits( std::ostream & r_os_in, tlmx_uns32 accumulated_in, size_t number_accumulated_in );

public:
    LMX_TDECL c_binary() {}
    LMX_TDECL c_binary( const c_binary & r_v_in );
    LMX_TDECL c_binary( const unsigned char * p_v_in, size_t len_in );
    LMX_TDECL ~c_binary() {}
    LMX_TDECL c_binary & operator = ( const c_binary & );
    LMX_TDECL void swap( c_binary & r_v_in );

    LMX_TDECL static bool is_valid_hex( const std::string & r_v_in );
    LMX_TDECL static bool is_valid_base64( const std::string & r_v_in );

    LMX_TDECL unsigned char * get( unsigned char * p_buffer_out, size_t buffer_size_in ) const;
    LMX_TDECL size_t size() const;
    LMX_TDECL bool empty() const;   // v5
    LMX_TDECL const unsigned char & operator [] ( size_t i_in ) const;  // C130117_1505 - Returning ref allows use with fwrite() etc.
    LMX_TDECL unsigned char & operator [] ( size_t i_in );
    #if defined( _MSC_VER ) ||  defined( __GNUC__ ) && __GNUC__ >= 3
    // pre-GCC v3 does not have vector::at (at() throws an exception if out of bounds)
    LMX_TDECL const unsigned char & at( size_t i_in ) const;    // C130117_1505 - Returning ref allows use with fwrite() etc.
    LMX_TDECL unsigned char & at( size_t i_in );
    #endif
    std::string get_as_hex_string() const;      // C141222_1154
    std::string get_as_base64_string() const;   // C141222_1154

    LMX_TDECL void set( const unsigned char * p_v_in, size_t len_in );
    LMX_TDECL void operator += ( unsigned char c_in );
    LMX_TDECL void append( unsigned char c_in );
    LMX_TDECL void from_hex( const std::string & r_src_in );
    LMX_TDECL void from_base64( const std::string & r_src_in );

    LMX_TDECL bool equal( const unsigned char c_rhs_in[], size_t rhs_len_in ) const;
    LMX_TDECL friend bool operator == ( const c_binary & r_lhs_in, const c_binary & r_rhs_in );
    LMX_TDECL friend bool operator != ( const c_binary & r_lhs_in, const c_binary & r_rhs_in );

    LMX_TDECL void out_hex( std::ostream & r_os_in ) const;
    LMX_TDECL void out_base64( std::ostream & r_os_in ) const;

    // Output raw binary data. Use os << lmx::as_base64( my_binary ) << lmx::as_hex( my_binary ) to output encoded binary data
    LMX_TDECL friend std::ostream & operator << ( std::ostream &, const c_binary & );   // C170425_1618

    LMX_TDECL friend int test_c_binary();
};

inline void swap( c_binary & r_lhs_in, c_binary & r_rhs_in ) { r_lhs_in.swap( r_rhs_in ); } // Added V5

LMX_TDECL extern const std::string k_qname_user_explicit_ns_prefix_not_set; /* Declaration in ct_qname template is more complicated */

template< typename Tstring >    // V3.2.4
class ct_qname
{
private:
    tlmx_string m_namespace;                // V3.10 - changed to tlmx_string type
    Tstring m_local_name;
    tlmx_string m_user_explicit_ns_prefix;    // V3.8 added & V3.10 changed to tlmx_string type

public:
    ct_qname() : m_user_explicit_ns_prefix( k_qname_user_explicit_ns_prefix_not_set )
            {}
    ct_qname( const ct_qname< Tstring > & r_rhs_in ) :
            m_namespace( r_rhs_in.m_namespace ), m_local_name( r_rhs_in.m_local_name ),
            m_user_explicit_ns_prefix( k_qname_user_explicit_ns_prefix_not_set )
            {}
    ct_qname( const tlmx_string & r_namespace_in, const Tstring & r_local_name_in ) :     // V3.8
            m_namespace( r_namespace_in ), m_local_name( r_local_name_in ),
            m_user_explicit_ns_prefix( k_qname_user_explicit_ns_prefix_not_set )
            {}
    ct_qname< Tstring > & operator = ( const ct_qname< Tstring > & r_rhs_in )
            { ct_qname< Tstring > t( r_rhs_in ); swap( t ); return *this; }
    void swap( ct_qname< Tstring > & r_rhs_in )
    {
        string_swap( m_local_name, r_rhs_in.m_local_name );
        string_swap( m_namespace, r_rhs_in.m_namespace );
        string_swap( m_user_explicit_ns_prefix, r_rhs_in.m_user_explicit_ns_prefix );
    }
    void set( const tlmx_string & r_namespace_in, const Tstring & r_local_name_in )
            { m_namespace = r_namespace_in; m_local_name = r_local_name_in; }
    void set_namespace( const tlmx_string & r_namespace_in )
            { m_namespace = r_namespace_in; }
    const tlmx_string &get_namespace() const
            { return m_namespace; }
    void set_explicit_ns_prefix( const tlmx_string & r_ns_prefix_in )  // V3.8 - By default the ns prefix will be automatically derived from the namespace at marshal time.  This allows this default action to be overridden.
            { m_user_explicit_ns_prefix = r_ns_prefix_in; }
    const tlmx_string &get_explicit_ns_prefix() const
            { return m_user_explicit_ns_prefix; }
    void set_local_name( const Tstring & r_local_name_in )
            { m_local_name = r_local_name_in; }
    const Tstring &get_local_name() const
            { return m_local_name; }
    bool operator == ( const ct_qname< Tstring > & r_rhs_in ) const
            { return string_eq( m_local_name, r_rhs_in.m_local_name ) && string_eq( m_namespace, r_rhs_in.m_namespace ); }
    bool operator != ( const ct_qname< Tstring > & r_rhs_in ) const
            { return ! (*this == r_rhs_in); }
    bool empty() const  // V3.8
            { return string_empty( m_local_name ) && string_empty( m_namespace ); }
    ~ct_qname() {}
};

inline void swap( ct_qname< std::string > & r_lhs_in, ct_qname< std::string > & r_rhs_in ) { r_lhs_in.swap( r_rhs_in ); }   // Added V5
#ifndef LMX_NO_WSTRING
    inline void swap( ct_qname< std::wstring > & r_lhs_in, ct_qname< std::wstring > & r_rhs_in ) { r_lhs_in.swap( r_rhs_in ); } // Added V5
#endif

//------------------------------------------------------------------------------
//                           Output converter prototypes - v3.8.2
//------------------------------------------------------------------------------

template< typename T >   // C171115_1549
std::ostream & output_convert_to_xml( std::ostream & r_os_in, const T & r_v_in )
{
    r_os_in << r_v_in;
    return r_os_in;
}

#ifndef LMX_NO_WSTRING
    LMX_TDECL std::ostream & output_convert_to_xml( std::ostream & r_os_in, const std::wstring & r_v_in );
#endif

LMX_TDECL std::ostream & output_convert_to_xml( std::ostream & r_os_in, const std::string & r_v_in );
LMX_TDECL std::ostream & output_convert_to_xml( std::ostream & r_os_in, bool v_in );
LMX_TDECL std::ostream & output_convert_to_bool_digit( std::ostream & r_os_in, bool v_in );    // V3.7
LMX_TDECL std::ostream & output_convert_to_xml( std::ostream & r_os_in, unsigned char v_in );
LMX_TDECL std::ostream & output_convert_to_xml( std::ostream & r_os_in, signed char v_in );
LMX_TDECL std::ostream & output_convert_to_xml( std::ostream & r_os_in, float v_in );
LMX_TDECL std::ostream & output_convert_to_xml( std::ostream & r_os_in, double v_in );
LMX_TDECL std::ostream & output_convert_to_hex( std::ostream & r_os_in, const c_binary & r_v_in );
LMX_TDECL std::ostream & output_convert_to_base64( std::ostream & r_os_in, const c_binary & r_v_in );

LMX_TDECL std::ostream & output_convert_to_escaped( std::ostream & r_os_in, const std::string & r_v_in );

#ifndef LMX_NO_WSTRING
    LMX_TDECL std::ostream & output_convert_to_ascii( std::ostream & r_os_in, const std::wstring & r_v_in );
#endif

//------------------------------------------------------------------------------
//                         QName helper prototypes - V3.3
//------------------------------------------------------------------------------

LMX_TDECL void make_qname( ct_qname< std::string > * p_value_out, const std::string & r_qname_in, const std::string & r_namespace_in, const std::string & r_local_name_in );
LMX_TDECL void make_qname( std::string * p_value_out, const std::string & r_qname_in, const std::string & r_namespace_in, const std::string & r_local_name_in );
LMX_TDECL void make_qname( ct_qname< std::string > * p_value_out, const std::string & r_qname_default_spec_in );
LMX_TDECL void make_qname( std::string * p_value_out, const std::string & r_qname_default_spec_in );

#ifndef LMX_NO_WSTRING
LMX_TDECL void make_qname( ct_qname< std::wstring > * p_value_out, const std::string & r_qname_in, const std::string & r_namespace_in, const std::string & r_local_name_in );
LMX_TDECL void make_qname( std::wstring * p_value_out, const std::string & r_qname_in, const std::string & r_namespace_in, const std::string & r_local_name_in );
LMX_TDECL void make_qname( ct_qname< std::wstring > * p_value_out, const std::string & r_qname_default_spec_in );
LMX_TDECL void make_qname( std::wstring * p_value_out, const std::string & r_qname_default_spec_in );
#endif

//------------------------------------------------------------------------------
//                            Conversion functions
//------------------------------------------------------------------------------

#ifndef LMX_NO_WSTRING
LMX_TDECL std::string & convert( std::string * p_out, const std::wstring & r_in );
LMX_TDECL std::wstring & convert( std::wstring * p_out, const std::string & r_in );
inline std::wstring & convert( std::wstring * p_out, const std::wstring & r_in ) // V2.8
{
    *p_out = r_in;
    return *p_out;
}
inline tlmx_unicode_string convert( const std::wstring & r_in )     // V3.8 - Less efficient, but easier to use
{
    tlmx_unicode_string unicode_string;
    return convert( &unicode_string, r_in );
}
inline std::string convert_to_narrow( const std::wstring & r_in )   // V3.8 - Less efficient, but easier to use
{
    std::string narrow_string;
    return convert( &narrow_string, r_in );
}
inline std::wstring convert_to_wide( const std::string & r_in )     // V3.8 - Less efficient, but easier to use
{
    std::wstring wide_string;
    return convert( &wide_string, r_in );
}
inline std::wstring convert_to_wide( const std::wstring & r_in )    // V3.8 - Less efficient, but easier to use
{
    return r_in;
}
#endif

inline std::string & convert( std::string * p_out, const std::string & r_in ) // V2.8
{
    *p_out = r_in;
    return *p_out;
}
inline tlmx_unicode_string convert( const std::string & r_in )      // V3.8 - Less efficient, but easier to use
{
    tlmx_unicode_string unicode_string;
    return convert( &unicode_string, r_in );
}
inline std::string convert_to_narrow( const std::string & r_in )    // V3.8 - Less efficient, but easier to use
{
    return r_in;
}

//------------------------------------------------------------------------------
//                     Pattern Facet Validation Classes
// These are the default class that does pattern facet validation.
// c_pattern_null is included for backwards compatibility with old versions of
// LMX that did not do pattern validation.  Set #define LMX_VALIDATE_PATTERNS
// to 0 to use c_pattern_null.
//------------------------------------------------------------------------------

#if defined( LMX_VALIDATE_PATTERNS ) && LMX_VALIDATE_PATTERNS == 1
class c_pattern
{
private:
    c_regex m_regex;

public:
    LMX_TDECL c_pattern( const char * p_pattern_in ) : m_regex( p_pattern_in ) {}
    LMX_TDECL bool is_match( const std::string & r_v_in ) const
        { return m_regex.is_match( r_v_in ); }
    #ifndef LMX_NO_WSTRING
    LMX_TDECL bool is_match( const std::wstring & r_v_in ) const
        { std::string narrow; return m_regex.is_match( convert( &narrow, r_v_in ) ); }
    #endif
};
#endif

class c_pattern_null
{
public:
    c_pattern_null( const char * LMX_UNUSED_PARAM(p_pattern_in) ) {}
    bool is_match( const std::string & LMX_UNUSED_PARAM(r_v_in) ) const { return true; }
    #ifndef LMX_NO_WSTRING
    bool is_match( const std::wstring & LMX_UNUSED_PARAM(r_v_in) ) const { return true; }
    #endif
};


} // End of namespace lmx

#if defined( _MSC_VER ) && _MSC_VER >= 1400
#pragma warning (pop)  // Restore VC8 'safe' warnings
#endif

#endif // LMXTYPES_H


//------------------------------------------------------------------------------
//                   Copyright (c) 2003-2017 Codalogic Ltd.
//------------------------------------------------------------------------------
