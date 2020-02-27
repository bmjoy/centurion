#ifndef LMXUSER_H
#define LMXUSER_H

#if defined( _MSC_VER ) && _MSC_VER > 1000
#pragma once
#pragma warning(disable:4786)
#endif

#if defined( _MSC_VER ) && _MSC_VER >= 1400
#pragma warning (push)
#pragma warning (disable:4996)  // Disable VC8 'safe' warnings
#endif

#if defined( __BORLANDC__ )
#pragma warn -8058 // Disable "code in header" warning
#pragma warn -8008 // Disable "condition always true"
#pragma warn -8066 // Disable "unreachable code" warnings
using std::_atoi64;
#endif

#ifndef LMXSTDCRT
    // VC 6 does not place library functions in the std namespace
    #if defined( _MSC_VER ) && _MSC_VER <= 1200 || defined( UNDER_CE )
    #define LMXSTDCRT
    #else
    #define LMXSTDCRT std
    #endif
#endif

#if defined( __GNUC__ ) && __GNUC__ == 2
    #define __GLIBCPP_WCHAR_SUPPORT__
#endif
#include <string>
#include <cstdio>
#include <cstring>  // v4.2
#include <cstdlib>  // v4.2
#include <clocale>
#include <vector>
#include <list>
#include <deque>
#include <iosfwd>
#include <sstream>
#include <limits>
#if defined( _MSC_VER ) && ! defined( UNDER_CE ) || defined( __BORLANDC__ ) || defined( __GNUC__ ) && __GNUC__ >= 3 || defined( __SUNPRO_CC ) || defined( __HP_aCC ) || defined( __IBMCPP__ )
    #include <locale>
#endif

#ifdef LMX_WANT_USER_DEFS   // C101015_0953
#include "lmxuser-defs.h"
#endif

namespace lmx {

//------------------------------------------------------------------------------
//                                Useful macros
//------------------------------------------------------------------------------

// V3.0 - We like NULL for clarity, but it's not always portable!
#ifndef LMXNULL
#define LMXNULL 0
#endif

// USER: ASSERTS - This option allows control of the handling of assertions.
// -You may use a custom assertion handler for debug code, or have different
// -handling of assertion code depending on whether the code is running in
// -debug or release mode.  For example, you may wish to throw an exception in
// -release mode. (V2.7)(v3.3 - moved position in file)(v3.8 - put in
// -conditional compilation block)

#if ! defined( lmx_assert )
    #if ! defined( LMX_NO_LMX_ASSERT )
        #define lmx_assert( x )     assert( x )
    #else
        #define lmx_assert( x )
    #endif
#endif

#if ! defined( lmx_assert_code_not_reached )
    #if ! defined( LMX_NO_LMX_ASSERT_CODE_NOT_REACHED )
        #define lmx_assert_code_not_reached()     lmx_assert( 0 )
    #else
        #define lmx_assert_code_not_reached()
    #endif
#endif

#if ! defined( lmx_assert_is_occurs_ok )    // C150325_1731
    #if ! defined( LMX_NO_LMX_ASSERT_IS_OCCURS_OK )
        #define lmx_assert_is_occurs_ok( x )     lmx_assert( x )
    #else
        #define lmx_assert_is_occurs_ok( x )
    #endif
#endif

// USER: UNUSED_PARAMS - This section allows configuring the code for marking
// -unused parameters.

#ifndef LMX_CONFIG_UNUSED   // v3.10
    #ifndef LMX_IGNORE_UNUSED
        #define LMX_NO_WARN_UNUSED( x ) ((void)x)
        #define LMX_UNUSED_PARAM( x )
    #else
        #define LMX_NO_WARN_UNUSED( x )
        #define LMX_UNUSED_PARAM( x ) x
    #endif
#endif

//------------------------------------------------------------------------------
//                     Definitions to configure operation
//------------------------------------------------------------------------------

// USER: CTRL_DEFS - Set these #defines to tailor your code builds

// V2.11.1 - Allowed #defines to be optionally set on the C++ compiler command line

#ifdef _DEBUG
    #ifndef LMX_USER_VALIDATE
        #define LMX_USER_VALIDATE
    #endif
    #ifndef LMX_USE_USER_VALIDATE_EXCEPTIONS
        #define LMX_USE_USER_VALIDATE_EXCEPTIONS 0
                // Set to 1 to throw exceptions when validating user set
                // -operations in Debug mode
    #endif
#else
    #ifndef LMX_USE_USER_VALIDATE_EXCEPTIONS
        #define LMX_USE_USER_VALIDATE_EXCEPTIONS 0
                // Set to 1 to throw exceptions when validating user set
                // -operations in Release mode (v4.2)
    #endif
#endif

#ifndef LMX_USE_EXCEPTIONS
    #define LMX_USE_EXCEPTIONS 0
                // Set to 1 to throw exceptions on marshalling and unmarshalling
                // -errors
#endif

#ifndef LMX_USE_CHECK_EXCEPTIONS
    #define LMX_USE_CHECK_EXCEPTIONS LMX_USE_EXCEPTIONS
                // Set to 1 to throw exceptions on check() errors C130509_1618
#endif

#ifndef LMX_WANT_DECIMAL_FLOAT_SET
    #define LMX_WANT_DECIMAL_FLOAT_SET 1
                // Set to 1 to include special function to convert float number to
                // -decimal number taking into account fractionDigits facet of
                // -the decimal value

#endif

// Note: WANT_DLL - Under Windows there are many ways to partition your code,
// LMX generated code, and LMX Supporting Software between your main exe and
// DLLs.  These variables allow set up your system for the more common
// arrangements.  If the arrangement you wish to use is not supported by these
// flags, you may wish to change the layout of the code in the DECL_DEFS
// section.  Note that these #defines can either be set here or in the project
// settings.
// If you are statically linking all code into a stand-alone exe, use
// LMX_WANT_DLL = 0 and LMX_WANT_GEN_IMPORT_DLL = 0.  (This is also the option
// for Linux and other OSs that do not use such constructs.)
// If you are linking DLLs, but want the code to automagically work, then do
// not define LMX_WANT_DLL and LMX_WANT_GEN_IMPORT_DLL and allow their values
// to be automatically set by the following section of code.
// Alternatively, if you want to be explicit about your DLL usage, do as
// follows:
// If you want to use the LMX Supporting Software in a DLL (or create such a
// DLL) then use LMX_WANT_DLL = 1.
// If you are producing a DLL that exports LMX generated code, also use
// LMX_WANT_DLL = 1.
// If you are producing code that accesses LMX generated code that is located
// in a separate DLL and you want explicit dllimports, then use
// LMX_WANT_GEN_IMPORT_DLL = 1.

#ifndef LMX_WANT_DLL
    #ifndef _DLL    // C130207_1913
        #define LMX_WANT_DLL 0
                        // Set to 1 if a DLL version is required.
    #else
        #define LMX_WANT_DLL 1
    #endif
#endif

#ifndef LMX_WANT_GEN_IMPORT_DLL
    #define LMX_WANT_GEN_IMPORT_DLL 0
                // Set to 1 if a DLL version is required.
#endif

// USER: XML_STRICT - Setting this #define to 1 forces a strict interpretation
// of XML.  Setting it to 0 allows a more liberal interpretation of XML.
// In particular, when LMX_XML_STRICT == 0 references to entities that have not
// been defined are treated as if they were regular text, rather than entity
// references, UTF-16 surrogates are allowed in UTF-8 input.
// N.B. This definition only takes effect if you have the supporting software
// source code (Professional Edition).

#ifndef LMX_XML_STRICT
    #define LMX_XML_STRICT 1
#endif

// USER: STRING_SIZE - Set this #define to 1 to use narrow (UTF-8) std::strings
// for the user interface to the methods.

#ifdef LMX_NO_WSTRING
    #define LMX_NARROW_UNICODE_STRINGS 1
    #define LMX_NARROW_URI_STRINGS 1
#endif

#ifndef LMX_NARROW_UNICODE_STRINGS
    #define LMX_NARROW_UNICODE_STRINGS 0
#endif

#ifndef LMX_NARROW_URI_STRINGS  // v4.0
    #define LMX_NARROW_URI_STRINGS LMX_NARROW_UNICODE_STRINGS
#endif

// USER: FLOAT_PRECISION - The default precision for float conversion should
// result in 'pretty' text representation. If LMX_FLOAT_HIGH_PRECISION is
// defined, then round tripping from float (double) to text, back to
// float (double) should be lossless, but may result in text representations
// that include trailing ...9998 or ...0001 etc.
// C170614_1145

#ifndef LMX_FLOAT_PRECISION
    #ifndef LMX_FLOAT_HIGH_PRECISION
        #define LMX_FLOAT_PRECISION 7
    #else
        #define LMX_FLOAT_PRECISION 9
    #endif
#endif

#ifndef LMX_DOUBLE_PRECISION
    #ifndef LMX_FLOAT_HIGH_PRECISION
        #define LMX_DOUBLE_PRECISION 14
    #else
        #define LMX_DOUBLE_PRECISION 17
    #endif
#endif

// USER: VALIDATE_PATTERNS - As of version 3.6 LMX validates patterns by
// default.  For compatibility with earlier versions that did not validate
// patterns set this #define to 0.

#ifndef LMX_VALIDATE_PATTERNS
    #define LMX_VALIDATE_PATTERNS 1
#endif

// USER: TWO_PHASE_TEMPLATES - Older compilers (including VS2005) do not
// support two phase template instantiation.  If you are using such a
// compiler and getting errors such as "attempting to delete incomplete type"
// then set LMX_TWO_PHASE_TEMPLATES_SUPPORTED to 0, otherwise set to 1.

#ifndef LMX_TWO_PHASE_TEMPLATES_SUPPORTED
    #if defined( __GNUC__ ) && __GNUC__ >= 3
        #define LMX_TWO_PHASE_TEMPLATES_SUPPORTED 1
    #else
        #define LMX_TWO_PHASE_TEMPLATES_SUPPORTED 0
    #endif
#endif

//------------------------------------------------------------------------------
//                       Control DLL export requirements
//------------------------------------------------------------------------------

#ifdef LMX_RT_SOURCE
    #ifndef LMX_PARSE_SOURCE
        #define LMX_PARSE_SOURCE
    #endif
    #ifndef LMX_TYPES_SOURCE
        #define LMX_TYPES_SOURCE
    #endif
    #ifndef LMX_REGEX_SOURCE
        #define LMX_REGEX_SOURCE
    #endif
#endif

// USER: DECL_DEFS - See the note WANT_DLL for additional guidance on this
// section.
// v3.0 - These flags have been re-arranged to allow generated code to reside
// in the same module as the code that is using it.

#ifndef LMX_CONFIG_DECL_DEFS
    #if LMX_WANT_DLL == 1 || defined( LMX_MAKE_DLL )
        #ifdef LMX_PARSE_SOURCE
            #define LMX_PDECL __declspec(dllexport)
        #else
            #define LMX_PDECL __declspec(dllimport)
        #endif
        #ifdef LMX_TYPES_SOURCE
            #define LMX_TDECL __declspec(dllexport)
        #else
            #define LMX_TDECL __declspec(dllimport)
        #endif
        #ifdef LMX_REGEX_SOURCE
            #define LMX_RDECL __declspec(dllexport)
        #else
            #define LMX_RDECL __declspec(dllimport)
        #endif
    #else
        #define LMX_PDECL
        #define LMX_TDECL
        #define LMX_RDECL
    #endif

    #if LMX_WANT_GEN_IMPORT_DLL == 1
        #define LMX_GDECL __declspec(dllimport)
    #elif LMX_WANT_DLL == 1
        #ifdef LMX_GEN_SOURCE   // C130207_1913
            #define LMX_GDECL __declspec(dllexport)
        #else
            #define LMX_GDECL
        #endif
    #elif defined( LMX_MAKE_DLL )
        #ifdef LMX_GEN_SOURCE
            #define LMX_GDECL __declspec(dllexport)
        #else
            #define LMX_GDECL __declspec(dllimport)
        #endif
    #else
        #define LMX_GDECL
    #endif
#endif

// declspec for when custom string conversion functions are defined for when a
// custom narrow string char set is used.  Appropriate functions will be called
// when LMX_CUSTOM_NARROW_STRING_CHAR_SET is defined.
#ifndef LMX_CCSDECL
#define LMX_CCSDECL LMX_TDECL
#endif

// (V2.7) Inclusion of lmxinternals.h moved to lower down in file.

//------------------------------------------------------------------------------
//                          Mapping of basic types
//------------------------------------------------------------------------------

// USER: PLATFORM_TYPES - Modify these typedefs according to your platform
// -requirements.  This tells LMX how, for example, to represent a 16-bit int on
// -your platform (currently set for MS VC++, Borland and GCC). v4.1

#ifndef LMX_CONFIG_PLATFORM_TYPES
    #if defined( _MSC_VER ) || defined( __BORLANDC__ )
        typedef __int64 tlmx_platform_int64;
        typedef unsigned __int64 tlmx_platform_uns64;
    #elif defined( __GNUC__ ) || defined( __SUNPRO_CC ) || defined( __HP_aCC ) || defined( __IBMCPP__ )
        typedef long long tlmx_platform_int64;
        typedef unsigned long long tlmx_platform_uns64;
    #else
        // If we want to represent 64-bit values using c_big_int, we need to do
        // -that after c_big_int is defined.  Use a #define to trigger this later.
        #define LMX_REPRESENT_INT64_USING_C_BIG_INT
    #endif
    typedef int tlmx_platform_int32;
    typedef unsigned int tlmx_platform_uns32;
    typedef short tlmx_platform_int16;
    typedef unsigned short tlmx_platform_uns16;
    typedef signed char tlmx_platform_int8;
    typedef unsigned char tlmx_platform_uns8;
#endif

// USER: BASIC_TYPES - Modify these typedefs according to how you would like
// -the basic numerical types to be stored by the generated code.  For example
// -you may want 16-bit values to be stored in 32-bit ints.  To do this you
// -would do something like typedef tlmx_platform_int32 tlmx_int16;.

#ifndef LMX_CONFIG_BASIC_TYPES
    #if defined( _MSC_VER ) || defined( __BORLANDC__ ) || defined( __GNUC__ ) || defined( __SUNPRO_CC ) || defined( __HP_aCC ) || defined( __IBMCPP__ )
        typedef tlmx_platform_int64 tlmx_int64;
        typedef tlmx_platform_uns64 tlmx_uns64;
    #else
        // If we want to represent 64-bit values using c_big_int, we need to do
        // -that after c_big_int is defined.  Use a #define to trigger this later.
        #define LMX_REPRESENT_INT64_USING_C_BIG_INT
    #endif
    typedef tlmx_platform_int32 tlmx_int32;
    typedef tlmx_platform_uns32 tlmx_uns32;
    typedef tlmx_platform_int16 tlmx_int16;
    typedef tlmx_platform_uns16 tlmx_uns16;
    typedef tlmx_platform_int8 tlmx_int8;
    typedef tlmx_platform_uns8 tlmx_uns8;

    typedef float tlmx_float;
    typedef double tlmx_double;
#endif

#ifndef LMX_CONFIG_STRING_TYPES
    typedef std::string tlmx_string;

    #if LMX_NARROW_UNICODE_STRINGS == 1
        typedef std::string tlmx_unicode_string;
    #else
        typedef std::wstring tlmx_unicode_string;
    #endif

    #if LMX_NARROW_URI_STRINGS == 1     // 3.8, modified 3.9
        typedef std::string tlmx_uri_string;
    #else
        typedef std::wstring tlmx_uri_string;
    #endif
#endif

// Typedef tlmx_wide_string with the same type as tlmx_unicode_string
// for compatibility with older versions.
typedef tlmx_unicode_string tlmx_wide_string;

// USER: INT_NATIVE_FOREIGN - Define whether each integer type is mapped
// -to a C++ native integer type (e.g. short), or mapped to a defined
// -object (e.g. c_big_int).  If the type is native, set the macro
// -result to 'native'.  Otherwise set the macro result to 'foreign'.

#ifndef LMX_CONFIG_INT_NATIVE_FOREIGN
    #define MKINT8( native, foreign ) native
    #define MKUNS8( native, foreign ) native
    #define MKINT16( native, foreign ) native
    #define MKUNS16( native, foreign ) native
    #define MKINT32( native, foreign ) native
    #define MKUNS32( native, foreign ) native
    #if defined( _MSC_VER ) || defined( __BORLANDC__ )
        #define MKINT64( native, foreign ) native
        #define MKUNS64( native, foreign ) native
    #elif defined( __GNUC__ )
        // See note GCC_LL_IN_MACRO below
        #define MKINT64( native, foreign ) native ## LL
        #define MKUNS64( native, foreign ) native ## ULL
    #elif defined( __SUNPRO_CC ) || defined( __HP_aCC ) || defined( __IBMCPP__ )
        #define MKINT64( native, foreign ) native ## LL
        #define MKUNS64( native, foreign ) native ## ULL
    #else
        #define MKINT64( native, foreign ) foreign
        #define MKUNS64( native, foreign ) foreign
    #endif
#endif

// USER: DECIMAL_INIT_MODE - Define whether decimal types should be
// initialised from a string value, or a float value. C150324_1329

#ifndef LMX_DECIMAL_INIT_MODE
    #define LMX_MKDECIMAL( from_string, from_float ) from_string
#endif

// USER: FLOAT_SECONDS - Previously seconds in the dateTime and time classes
// used to be recorded using float values.  However, this does not allow
// -nanosecond resolution and so has been changed to double as of v6.2.  If
// -this change causes you compilation problems, define LMX_FLOAT_SECONDS in
// -your build setup to revert to using a float type. (C120927_1127)

#ifndef LMX_FLOAT_SECONDS
    typedef double tlmx_precision_seconds;
#else
    typedef float tlmx_precision_seconds;
#endif

// Note: GCC_LL_IN_MACRO - We occasionally use constructs such as
// -#if MKINT64( 1, 0 ) == 1 ..., which may appear problematic if we
// -#define MKINT64( native, foreign ) native ## LL because the C++ preprocessor
// -need not interpret integers in the same way as the compiler for real and so
// -may not understand something like 1LL as a number.  However, the GCC
// -preprocessor does understand these integer decorators and so doing
// -native ## LL is not a problem.

// USER: FULL_UNICODE_CHAR - Minimum 20 bits to hold whole range of Unicode
// -chars (0x0-0x10ffff).  Signed so that -1 can mean end-of-data.
typedef tlmx_int32 t_unicode_char;

//------------------------------------------------------------------------------
//                  Generic string operations to allow defining
//                  different user side string classes - V3.9.2
//    Moved from lmxinternals.h to lmxuser.h to fix gcc 4.1.2 warnings - V3.10
//------------------------------------------------------------------------------

#ifndef LMX_CONFIG_STRING_HELPERS
    template< class T >
    size_t string_size( const T & r_string_in )
    {
        return r_string_in.size();
    }

    template< class T >     // Return number of unicode characters - C121028_1228
    size_t string_n_characters( const T & r_string_in )
    {
        return string_size( r_string_in );
    }

    LMX_TDECL size_t length( const std::string & r_utf8_string_in );
    inline
    size_t string_n_characters( const std::string & r_utf8_string_in )
    {
        return lmx::length( r_utf8_string_in );
    }

    #ifndef LMX_NO_WSTRING
    LMX_TDECL size_t length( const std::wstring & r_utf16_string_in );
    inline
    size_t string_n_characters( const std::wstring & r_utf16_string_in )
    {
        return lmx::length( r_utf16_string_in );
    }
    #endif

    template< class T >
    void string_swap( T & r_string1_in, T & r_string2_in )
    {
        r_string1_in.swap( r_string2_in );
    }

    template< class T >
    bool string_eq( const T & r_string1_in, const T & r_string2_in )
    {
        return r_string1_in == r_string2_in;
    }

    template< class T >     // C110118_1032
    bool string_eq( const T & r_string1_in, const char * p_string2_in )
    {
        return r_string1_in == p_string2_in;
    }

    template< class T > // v3.10
    bool string_empty( const T & r_string_in )
    {
        return r_string_in.empty();
    }
#endif

} // End of namespace lmx

//------------------------------------------------------------------------------
//            Default version of compound types supplied with LMX
//------------------------------------------------------------------------------

#include "lmxtypes.h"

namespace lmx {

// v5
template < class T >
inline bool empty( const T & r_string_in ) { return string_empty( r_string_in ); }
inline bool empty( const c_binary & r_binary_in ) { return r_binary_in.empty(); }

#if defined( LMX_REPRESENT_INT64_USING_C_BIG_INT )
    typedef c_big_int tlmx_int64;
    typedef c_big_int tlmx_uns64;
#endif

//------------------------------------------------------------------------------
//                       Mapping of compound types
//------------------------------------------------------------------------------

// USER: COMPOUND_TYPES - Change these typedefs as required to match your
// -requirements

#ifndef LMX_CONFIG_COMPOUND_TYPES
    typedef c_big_int       tc_big_int;
    typedef c_decimal       tc_decimal;
    typedef c_duration      tc_duration;
    typedef c_datetime      tc_datetime;
    typedef c_time          tc_time;
    typedef c_date          tc_date;
    typedef c_gyearmonth    tc_gyearmonth;
    typedef c_gyear         tc_gyear;
    typedef c_gmonthday     tc_gmonthday;
    typedef c_gday          tc_gday;
    typedef c_gmonth        tc_gmonth;
    typedef c_binary        tc_binary;
    typedef ct_qname< tlmx_unicode_string > tc_qname;  // V3.2.4
#endif

#ifndef LMX_CONFIG_PATTERNS
    #if defined( LMX_VALIDATE_PATTERNS ) && LMX_VALIDATE_PATTERNS == 1
        typedef c_pattern       tc_pattern;
    #else
        typedef c_pattern_null  tc_pattern;
    #endif
#endif

//------------------------------------------------------------------------------
//      Control of marshalling and unmarshalling template specialisation
//------------------------------------------------------------------------------

// USER: CONTROL_TEMPLATE_SPECIALISATION - Enable or disable marshalling and
// -unmarshalling template specialisation.  Depending on the C++ compiler used
// -this option can reduce the size of your executable.  C110504_1221

#ifndef LMX_USE_TEMPLATE_SPECIALISATIONS
    #if defined( _MSC_VER ) && _MSC_VER <= 1200 || defined( UNDER_CE )
        #define LMX_USE_TEMPLATE_SPECIALISATIONS 0
    #else
        #define LMX_USE_TEMPLATE_SPECIALISATIONS 1
    #endif
#endif

//------------------------------------------------------------------------------
//                     Include lmxinternals.h file (V2.7)
//------------------------------------------------------------------------------

} // End of namespace lmx

#include "lmxinternals.h"

namespace lmx {

//------------------------------------------------------------------------------
//                    Utility functions for initialising constants
//------------------------------------------------------------------------------

// USER: INITIALISERS - This section allows you to modify the way string and
// -binary types are initialised.
// V3.9.2 - Functions moved from lmxinternals.h

#ifndef LMX_CONFIG_INIT_STRING
    inline tlmx_unicode_string inittowstring( const char * p_string_in )
    {
        tlmx_unicode_string string;
        return vtostring( string, p_string_in );
    }

    inline tlmx_uri_string inittouristring( const char * p_string_in )    // V3.8.1
    {
        tlmx_uri_string string;
        return vtostring( string, p_string_in );
    }

    inline tlmx_string inittostring( const char * p_string_in )    // C110118_1032
    {
        tlmx_string string;
        return vtostring( string, p_string_in );
    }
#endif

#ifndef LMX_CONFIG_INIT_BINARY
    inline tc_binary from_hex( const std::string &r_in )
    {
        tc_binary binary;
        binary.from_hex( r_in );
        return binary;
    }

    inline tc_binary from_base64( const std::string &r_in )
    {
        tc_binary binary;
        binary.from_base64( r_in );
        return binary;
    }
#endif

//------------------------------------------------------------------------------
//                         Micro Format Validators
//------------------------------------------------------------------------------

// USER: MICRO_FORMAT_VALIDATORS - The default assumption if you have defined a
// -class to represent a micro format is that the class has a static is_valid()
// -member.  If this is not the case then you can define a template class
// -specialization of s_is_valid< T > which contains appropriate validator
// -code in the is_valid() member. (C100902_1050)

#ifndef LMX_CONFIG_MICRO_FORMAT_VALIDATORS
    template< class T >
    struct s_is_valid
    {
        static bool is_valid( const std::string & r_v_in )
            { return T::is_valid( r_v_in ); }
    };
#endif

//------------------------------------------------------------------------------
//                             Wide file names
//------------------------------------------------------------------------------

// USER: WIDE_FILE_NAMES - Some operating systems allow wide file names, and
// -others don't.  Configure this section if you want to allow wide file names
// -on non-Windows systems.

#ifndef LMX_WANT_WIDE_FILE_NAMES
    #ifdef _MSC_VER
        #define LMX_WANT_WIDE_FILE_NAMES 1
    #else
        #define LMX_WANT_WIDE_FILE_NAMES 0
    #endif
#endif

//------------------------------------------------------------------------------
//                         Input Converters Helpers
//------------------------------------------------------------------------------

// USER: ATOFLOAT_MODE - This section allows you to modify the method of
// -converting strings to floats.

#ifndef LMX_CONFIG_ATOFLOAT_MODE
    #define LMX_ATOFLOAT_MODE_DEFAULT   0
    #define LMX_ATOFLOAT_MODE_USE_FACET 1
    #define LMX_ATOFLOAT_MODE_IOSTREAM  2
    #define LMX_ATOFLOAT_MODE_SIMPLE    3

    #ifndef LMX_ATOFLOAT_MODE       // C110824_1629 - Modified heuristics for setting LMX_ATOFLOAT_MODE
        #if defined( _MSC_VER ) && ! defined( UNDER_CE )
            #define LMX_ATOFLOAT_MODE   LMX_ATOFLOAT_MODE_DEFAULT

        #elif defined( __GNUC__ ) && __GNUC__ >= 3
            #define LMX_ATOFLOAT_MODE   LMX_ATOFLOAT_MODE_DEFAULT

        #elif defined( __SUNPRO_CC )
            #define LMX_ATOFLOAT_MODE   LMX_ATOFLOAT_MODE_DEFAULT

        #elif defined( __HP_aCC )
            #define LMX_ATOFLOAT_MODE   LMX_ATOFLOAT_MODE_DEFAULT

        #elif defined( __IBMCPP__ )
            #define LMX_ATOFLOAT_MODE   LMX_ATOFLOAT_MODE_DEFAULT

        #elif defined( VXWORKS )
            #define LMX_ATOFLOAT_MODE   LMX_ATOFLOAT_MODE_SIMPLE

        #else
            #define LMX_ATOFLOAT_MODE   LMX_ATOFLOAT_MODE_SIMPLE
        #endif
    #endif
#endif

#if defined( __GNUC__ ) && __GNUC__ < 4
    LMX_PDECL double float_zero();
#endif

inline double atofloat( const std::string & r_value_in )
{
#if defined( __GNUC__ ) && __GNUC__ < 4
    if( r_value_in == "NaN" )     // See portability note: IEEE754
        return 0.0 / float_zero();
    else if( r_value_in == "INF" )
        return 1.0 / float_zero();
    else if( r_value_in == "-INF" )
        return -1.0 / float_zero();
#else
    if( r_value_in == "NaN" )
        return std::numeric_limits<double>::quiet_NaN();    // C181024_1014
    else if( r_value_in == "INF" )
        return std::numeric_limits<double>::infinity();
    else if( r_value_in == "-INF" )
        return -std::numeric_limits<double>::infinity();
#endif
    else
    {
        // Make sure conversion is done in the "C" locale rather than any other
        // -locale that might have been set up.  (Ensures floats interpreted as
        // -1.2345 as per the standard, rather than potentially as 1,2345.)
#if LMX_ATOFLOAT_MODE == LMX_ATOFLOAT_MODE_DEFAULT
        // Although apparently cruder, we've found this method to be both
        // -faster and more portable that the other methods. v3.7.6
        const char *p_v = r_value_in.c_str();
        char c_with_comma_buffer[30];
        if ( *(LMXSTDCRT::localeconv()->decimal_point) == ',' )
        {
            const char *p_comma = LMXSTDCRT::strchr( p_v, '.' );
            if ( p_comma )
            {
                LMXSTDCRT::strcpy( c_with_comma_buffer, p_v );
                c_with_comma_buffer[p_comma-p_v] = ',';
                p_v = c_with_comma_buffer;
            }
        }
        return LMXSTDCRT::atof( p_v );
#elif LMX_ATOFLOAT_MODE == LMX_ATOFLOAT_MODE_USE_FACET
        // A more standards 'correct' way; more efficient than the third method
        typedef std::string::value_type charT;
        typedef std::string::const_iterator iterT;
        std::basic_ios<char> format(0);
        format.imbue( std::locale::classic() );
        std::ios_base::iostate error;
        double result;
        std::use_facet< std::num_get< charT, iterT > >(
                std::locale::classic() ).get (
                        r_value_in.begin(),
                        r_value_in.end(),
                        format,
                        error,
                        result );
        return result;
#elif LMX_ATOFLOAT_MODE == LMX_ATOFLOAT_MODE_IOSTREAM
        // For systems that don't support use_facet<>
        std::istringstream in_string( r_value_in );
        in_string.imbue( std::locale::classic() );
        double result;
        in_string >> result;
        return result;
#else
        // For systems that don't support locales
        return LMXSTDCRT::atof( r_value_in.c_str() );
#endif
    }
}

//------------------------------------------------------------------------------
//        Templates and template specialisations for input converters
//------------------------------------------------------------------------------

// C130227_1351 - Added default fallback for handling 64-bit integer conversions
// This function assumes the string has already been checked to ensure it's in
// the right format.
template< typename T >
T integer_from_string( T & r_number_out, const std::string & r_string_in )
{
    r_number_out = 0;
    const char * p_string = r_string_in.c_str();
    while( xisspace( *p_string ) )
        ++p_string;
    if( *p_string == '-' )
    {
        ++p_string;
        while( xisdigit( *p_string ) )
        {
            r_number_out *= 10;
            r_number_out -= (*p_string++ - '0');
        }
    }
    else
    {
        while( xisdigit( *p_string ) )
        {
            r_number_out *= 10;
            r_number_out += (*p_string++ - '0');
        }
    }
    return r_number_out;
}

// USER: TEMPLATE_INPUT_CONVERTERS - Add new template specialisations if required

// If you want to use a custom narrow string character set, then the input
// converter function must conform to the following prototype (see
// LMX_CUSTOM_NARROW_STRING_CHAR_SET):
LMX_CCSDECL void custom_vtostring( std::string & r_dest_out, const std::string & r_src_in );

template <typename T>
inline void v_to_o( T & r_out, const std::string & r_in )
{
    r_out = r_in;
}

#ifndef LMX_CONFIG_TEMPLATE_INPUT_CONVERTERS_INTS
    // Format of the conditional macros is: MKINT64( native, foreign )
    #if MKINT64( 1, 0 ) == 1
        template <>
        inline void v_to_o( tlmx_platform_int64 & r_out, const std::string & r_in )
        {
        #if defined( _MSC_VER ) || defined( __BORLANDC__ )
            r_out = _atoi64( r_in.c_str() );
        #elif defined( __GNUC__ ) || defined( __HP_aCC ) || defined( __IBMCPP__ ) || defined( __SUNPRO_CC ) || defined( sun ) || defined( __sun )
            r_out = 0;  // C130227_0956 - Removed nonstandard %Ld conversion format
            LMXSTDCRT::sscanf( r_in.c_str(), "%lld", &r_out );
        #else
            integer_from_string( r_out, r_in );     // C130227_1351 - Added default fallback for handling 64-bit integer conversions
        #endif
        }
    #endif

    #if MKINT32( 1, 0 ) == 1
        template <>
        inline void v_to_o( tlmx_platform_int32 & r_out, const std::string & r_in )
        {
            r_out = static_cast<tlmx_platform_int32>( LMXSTDCRT::atoi( r_in.c_str() ) );
        }
    #endif

    #if MKINT16( 1, 0 ) == 1
        template <>
        inline void v_to_o( tlmx_platform_int16 & r_out, const std::string & r_in )
        {
            r_out = static_cast<tlmx_platform_int16>( LMXSTDCRT::atoi( r_in.c_str() ) );
        }
    #endif

    #if MKINT8( 1, 0 ) == 1
        template <>
        inline void v_to_o( tlmx_platform_int8 & r_out, const std::string & r_in )
        {
            r_out = static_cast<tlmx_platform_int8>( LMXSTDCRT::atoi( r_in.c_str() ) );
        }
    #endif

    #if MKUNS64( 1, 0 ) == 1
        template <>
        inline void v_to_o( tlmx_platform_uns64 & r_out, const std::string & r_in )
        {
            r_out = 0;
        #if defined( _MSC_VER ) || defined( __BORLANDC__ )
            LMXSTDCRT::sscanf( r_in.c_str(), "%I64u", &r_out );
        #elif defined( __GNUC__ ) || defined( __HP_aCC ) || defined( __IBMCPP__ ) || defined( __SUNPRO_CC ) || defined( sun ) || defined( __sun )
            LMXSTDCRT::sscanf( r_in.c_str(), "%llu", &r_out );  // C130227_0956 - Removed nonstandard %Lu conversion format
        #else
            integer_from_string( r_out, r_in );     // C130227_1351 - Added default fallback for handling 64-bit integer conversions
        #endif
        }
    #endif

    #if MKUNS32( 1, 0 ) == 1
        template <>
        inline void v_to_o( tlmx_platform_uns32 & r_out, const std::string & r_in )
        {
            r_out =  static_cast<tlmx_platform_uns32>( LMXSTDCRT::strtoul( r_in.c_str(), LMXNULL, 10 ) );
        }
    #endif

    #if MKUNS16( 1, 0 ) == 1
        template <>
        inline void v_to_o( tlmx_platform_uns16 & r_out, const std::string & r_in )
        {
            r_out = static_cast<tlmx_platform_uns16>( LMXSTDCRT::atoi( r_in.c_str() ) );
        }
    #endif

    #if MKUNS8( 1, 0 ) == 1
        template <>
        inline void v_to_o( tlmx_platform_uns8 & r_out, const std::string & r_in )
        {
            r_out = static_cast<tlmx_platform_uns8>( LMXSTDCRT::atoi( r_in.c_str() ) );
        }
    #endif
#endif

#ifndef LMX_CONFIG_TEMPLATE_INPUT_CONVERTERS_FLOATS
    template <>
    inline void v_to_o( float & r_out, const std::string & r_in )
    {
        r_out = static_cast<float>( atofloat( r_in ) );
    }

    template <>
    inline void v_to_o( double & r_out, const std::string & r_in )
    {
        r_out = atofloat( r_in );
    }

    template <>
    inline void v_to_o( bool & r_out, const std::string & r_in )
    {
        //true, false, 1, 0
        if( r_in == "true" || r_in == "1" )
            r_out = true;
        else
            r_out = false;
    }
#endif

#ifndef LMX_CONFIG_TEMPLATE_INPUT_CONVERTERS_STRINGS
    template <>
    inline void v_to_o( std::string & r_out, const std::string & r_in )
    {
        #ifdef LMX_CUSTOM_NARROW_STRING_CHAR_SET
            custom_vtostring( r_out, r_in );
        #else
            vtostring( r_out, r_in );
        #endif
    }

    #ifndef LMX_NO_WSTRING
    template <>
    inline void v_to_o( std::wstring & r_out, const std::string & r_in )
    {
        vtostring( r_out, r_in );
    }
    #endif
#endif

#ifndef LMX_CONFIG_TEMPLATE_INPUT_CONVERTERS_BINARY
    inline void from_hex( c_binary & r_out, const std::string & r_in )
    {
        r_out.from_hex( r_in );
    }

    inline void from_base64( c_binary & r_out, const std::string & r_in )
    {
        r_out.from_base64( r_in );
    }
#endif

//------------------------------------------------------------------------------
//                         Basic output converters
//                   See also TEMPLATE_OUTPUT_CONVERTERS
//------------------------------------------------------------------------------

// USER: BASIC_OUTPUT_CONVERTERS - Adjust these to your platform requirements
// -(Currently set for MSVC++, Borland & GCC)

#ifndef LMX_CONFIG_BASIC_OUTPUT_CONVERTERS
    #if defined( _MSC_VER ) && _MSC_VER <= 1200
            // MSVC 6 ostream library does not natively support __int64

    inline std::ostream & operator << ( std::ostream & r_os_in, unsigned __int64 uns64_in )
    {
        char c_buf[22];
        LMXSTDCRT::sprintf( c_buf, "%I64u", uns64_in );
        r_os_in << c_buf;
        return r_os_in;
    }

    inline std::ostream & operator << ( std::ostream & r_os_in, __int64 int64_in )
    {
        // MSVC 6 ostream library does not natively support __int64
        char c_buf[22];
        LMXSTDCRT::sprintf( c_buf, "%I64d", int64_in );
        r_os_in << c_buf;
        return r_os_in;
    }
    #endif
#endif

//------------------------------------------------------------------------------
//                           Common User Base Class
//------------------------------------------------------------------------------

// USER: USER_BASE - This class can be used as the base for all generated
// -class.  Specify '-userbase' on the command line, or select
// -'Use common user base class' in the Windows interface.

#ifndef LMX_CONFIG_USER_BASE
    class c_user_base
    {
        // You may add code here
    public:
        virtual ~c_user_base() {}   // C110908_1023
    };

    typedef c_user_base UserBase;   // C110613_1525 - For CamelCase builds
#endif

//------------------------------------------------------------------------------
//                           Basic error handling function
//------------------------------------------------------------------------------

// USER: ERROR_HANDLING - Redefine this function to modify error handling

#ifndef LMX_CONFIG_ERROR_HANDLING
    inline elmx_error lmx_error( elmx_error error_in )
    {
    #if LMX_USE_USER_VALIDATE_EXCEPTIONS == 1
        if( error_in != ELMX_OK )    // Avoid any chance of an unreachable code warning
            throw c_lmx_exception( error_in );
    #endif
        return error_in;
    }

    inline elmx_error lmx_error( elmx_error error_in,
                                    const char * p_name_in,
                                    const char * prog_file_in,
                                    int prog_line_in )  // V3.8.1
    {
        LMX_NO_WARN_UNUSED( p_name_in );
        LMX_NO_WARN_UNUSED( prog_file_in );
        LMX_NO_WARN_UNUSED( prog_line_in );

    #if LMX_USE_USER_VALIDATE_EXCEPTIONS == 1
        if( error_in != ELMX_OK )    // Avoid any chance of an unreachable code warning
            throw c_lmx_exception_with_name( error_in, p_name_in );
    #endif
        return lmx_error( error_in );
    }
#endif

//------------------------------------------------------------------------------
//                  IO manipulator classes for output converters
//------------------------------------------------------------------------------

// IOMANIP does not seem to be supported on all platforms, so use alternative strategy
// Define on one line to avoid misc platform eol problems. Use macro to save typing!
#define ALIASED_PTR_CLASS( x ) template< class T > class c_##x { public: c_##x( const T & a ) : p(&a) {} const T* p; };
#define ALIASED_PTR_FUNC( x ) template< class T > inline const c_##x< T > x( const T &a ) { return c_##x< T >( a ); }
#define ALIASED_PTR( x ) ALIASED_PTR_CLASS( x ) ALIASED_PTR_FUNC( x )

ALIASED_PTR( as_xml )
ALIASED_PTR( as_bool_digit )    // V3.7
ALIASED_PTR( as_hex )
ALIASED_PTR( as_base64 )
ALIASED_PTR( as_escaped )
ALIASED_PTR( as_ascii )     // Primarily for user convenience

// V3.2.4 - To help with QName output
class c_as_xml_string
{
private:
    std::string m_s;
public:
    c_as_xml_string( const std::string & a ) : m_s(a) {}
    #ifndef LMX_NO_WSTRING
        c_as_xml_string( const std::wstring & a ) { convert( &m_s, a ); }
    #endif
    const std::string &get_s() const { return m_s; }
};

//------------------------------------------------------------------------------
//        Templates and template specialisations for output converters
//------------------------------------------------------------------------------

// USER: TEMPLATE_OUTPUT_CONVERTERS - Add new template specialisations if
// required

// v3.8.2 - Modified to use output_convert_to_???() functions for additional
// customizability.

// If you want to use a custom narrow string character set, then the output
// converter function must conform to the following prototype (see
// LMX_CUSTOM_NARROW_STRING_CHAR_SET):
LMX_CCSDECL std::ostream & custom_output_convert_to_xml( std::ostream &r_os_in, const std::string & r_v_in );

#ifndef LMX_CONFIG_TEMPLATE_OUTPUT_CONVERTERS
    template <typename T>
    inline std::ostream & operator << ( std::ostream & r_os_in, const c_as_xml<T> & r_v_in )
    {
        return output_convert_to_xml( r_os_in, *(r_v_in.p) );   // C171115_1549
    }

    #ifdef LMX_CUSTOM_NARROW_STRING_CHAR_SET
        template <>
        inline std::ostream & operator << ( std::ostream & r_os_in, const c_as_xml<std::string> &r_v_in )
        {
            return custom_output_convert_to_xml( r_os_in, *(r_v_in.p) );   // C171115_1549
        }
    #endif

    // V3.2.4 - To help with QName output
    inline std::ostream & operator << ( std::ostream & r_os_in, const c_as_xml_string & r_v_in )
            { r_os_in << as_xml( r_v_in.get_s() ); return r_os_in; }
#endif

#ifndef LMX_CONFIG_MISC_OUTPUT_CONVERTERS
    template <typename T>   // C171115_1549 - Made into templates
    std::ostream & operator << ( std::ostream & r_os_in, const c_as_hex<T> & r_v_in )
            { return output_convert_to_hex( r_os_in, *(r_v_in.p) ); }
    template <typename T>
    std::ostream & operator << ( std::ostream & r_os_in, const c_as_base64<T> & r_v_in )
            { return output_convert_to_base64( r_os_in, *(r_v_in.p) ); }
    template <typename T>
    std::ostream & operator << ( std::ostream & r_os_in, const c_as_bool_digit<T> & r_v_in )  // V3.7
            { return output_convert_to_bool_digit( r_os_in, *(r_v_in.p) ); }
    template <typename T>
    std::ostream & operator << ( std::ostream & r_os_in, const c_as_escaped<T> & r_v_in )
            { return output_convert_to_escaped( r_os_in, *(r_v_in.p) ); }
    template <typename T>
    std::ostream & operator << ( std::ostream & r_os_in, const c_as_ascii<T> & r_v_in )
        { return output_convert_to_ascii( r_os_in, *(r_v_in.p) ); }
#endif

// Added in v4.3
template <typename T>
inline std::ostream & o_to_v( std::ostream & r_os_in, const T & r_v_in )
        { r_os_in << c_as_xml< T >( r_v_in ); return r_os_in; }
template <typename T>
inline std::ostream & o_to_v_hex( std::ostream & r_os_in, const T & r_v_in )
        { r_os_in << c_as_hex< T >( r_v_in ); return r_os_in; }
template <typename T>
inline std::ostream & o_to_v_base64( std::ostream & r_os_in, const T & r_v_in )
        { r_os_in << c_as_base64< T >( r_v_in ); return r_os_in; }
template <typename T>
inline std::ostream & o_to_v_bool_digit( std::ostream & r_os_in, const T & r_v_in )
        { r_os_in << c_as_bool_digit< T >( r_v_in ); return r_os_in; }

//------------------------------------------------------------------------------
//                          Output XML formatting
//------------------------------------------------------------------------------

// USER: XML_WRITER_FORMAT - Adjust these to format the generated default XML
// -output as required.  If LMX_WRITER_LOCK_FORMAT is set to 1, runtime code
// -will not be able to modifiy the default format using the static methods
// -c_xml_writer::set_default_tab(...) and c_xml_writer::set_default_nl(...).
// -(C101014_1423)

#ifndef LMX_WRITER_DEFAULT_TAB
    #define LMX_WRITER_DEFAULT_TAB "\t"
#endif

#ifndef LMX_WRITER_DEFAULT_NL
    #define LMX_WRITER_DEFAULT_NL "\n"
#endif

#ifndef LMX_WRITER_DEFAULT_ATTRIBUTE_TAB    // C110426_1836
    #define LMX_WRITER_DEFAULT_ATTRIBUTE_TAB LMX_WRITER_DEFAULT_TAB
#endif

#ifndef LMX_WRITER_DEFAULT_ATTRIBUTE_NL     // C110426_1836
    #define LMX_WRITER_DEFAULT_ATTRIBUTE_NL LMX_WRITER_DEFAULT_NL
#endif

#ifndef LMX_WRITER_DEFAULT_CONVENIENCE_OPTIONS
    #define LMX_WRITER_DEFAULT_CONVENIENCE_OPTIONS c_xml_writer::include_xml_decl
#endif

#ifndef LMX_WRITER_LOCK_FORMAT
    #define LMX_WRITER_LOCK_FORMAT 0
#endif

} // End of namespace lmx

#ifdef LMX_WANT_USER_DEFS_END   // C101015_0953
#include "lmxuser-defs-end.h"
#endif

#if defined( _MSC_VER ) && _MSC_VER >= 1400
#pragma warning (pop)  // Restore VC8 'safe' warnings
#endif

#endif // LMXUSER_H

//------------------------------------------------------------------------------
//                   Copyright (c) 2003-2017 Codalogic Ltd.
//------------------------------------------------------------------------------
