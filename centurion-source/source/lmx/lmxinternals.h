#ifndef LMXINTERNALS_H
#define LMXINTERNALS_H

#if defined( _MSC_VER ) && _MSC_VER > 1000
#pragma once
#pragma warning(disable:4786)
#endif

#if defined( _MSC_VER ) && _MSC_VER >= 1400
#pragma warning (push)
#pragma warning (disable:4996)  // Disable VC8 'safe' warnings
#endif

#ifndef LMXUSER_H
#error lmxinternals.h should now be included via lmxuser.h in order to setup declspec
#endif

#if defined( __BORLANDC__ )
    #pragma warn -8004
    #pragma warn -8012
#endif

#include <cctype>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

namespace lmx {

// Bring std::swap functions into lmx namespace so we can use them without a
// prefix
using std::swap;

// Can't use the following code to do swap because vc10 complains about abiguity
//template< typename T >
//void swap( T & ar_lhs, T & ar_rhs )
//{
//    std::swap( ar_lhs, ar_rhs );    // Use std::swap if LMX doesn't provide a special version
//}

// Enable for backwards compatibility with v5.0 - C100510_1423
#ifdef LMX_USE_ISSET_METHOD
#define is_value_set isset
#endif

//------------------------------------------------------------------------------
//                           C++11 Migration Support
//------------------------------------------------------------------------------

template<typename T>
struct uniq_ptr // C180315_1111
{
#if defined( LMX_USE_UNIQUE_PTR )   // C190722_1906
    typedef std::unique_ptr<T> type;
#elif defined( LMX_USE_AUTO_PTR )   // C190722_1906
    typedef std::auto_ptr<T> type;
#elif __cplusplus >= 201103L || _MSC_VER >= 1700
    typedef std::unique_ptr<T> type;
#else
    typedef std::auto_ptr<T> type;
#endif
};

#if __cplusplus >= 201103L || _MSC_VER >= 1900  // C190725_1326
    #define LMX_CC_DEFAULT( x ) x = default;
#else
    #define LMX_CC_DEFAULT( x )
#endif

//------------------------------------------------------------------------------
//                             xs:all ordering constants
//------------------------------------------------------------------------------

const size_t k_all_order_not_present = ~static_cast<size_t>( 0 );

//------------------------------------------------------------------------------
//                             Error codes
//------------------------------------------------------------------------------

enum elmx_error_type { EET_FATAL, EET_ERROR, EET_WARNING, EET_NOTE, EET_NONE };

enum elmx_error {
    ELMX_OK = 0,
    ELMX_NO_FILE = 1,
    ELMX_ALL_CARDINALITY_VIOLATION = 2,
    ELMX_ATTRIBUTE_ALREADY_READ = 3,
    ELMX_ATTRIBUTES_IN_ANONYMOUS_COMPOSITOR = 4,
    ELMX_BADLY_FORMED = 5,
    ELMX_BAD_CHAR_ENCODING = 6,
    ELMX_BAD_CHOICE = 7,
    ELMX_BAD_COMMENT = 8,
    ELMX_BAD_DTD = 9,
    ELMX_BAD_ELEMENT_END = 10,
    ELMX_BAD_END_OF_START_TAG = 11,
    ELMX_BAD_END_TAG = 12,
    ELMX_BAD_XML_DECL = 13,
    ELMX_BAD_XML_VERSION = 14,
    ELMX_CHAR_ENCODING_MISMATCH = 15,
    ELMX_ELEMENT_NOT_FOUND = 16,
    ELMX_EXTERNAL_ENTITY = 17,
    ELMX_FRACTION_DIGITS_EXCEEDED = 18,
    ELMX_LENGTH_TOO_LONG = 19,
    ELMX_LENGTH_TOO_SHORT = 20,
    ELMX_MANDATORY_ELEMENT_MISSING = 21,
    ELMX_NO_PATTERN_MATCHED = 22,
    ELMX_NOT_WELL_FORMED = 23,
    ELMX_RECURSIVE_ENTITY_DEFINITION = 24,
    ELMX_REQUIRED_ATTRIBUTES_MISSING = 25,
    ELMX_TOO_MANY_ITEMS = 26,
    ELMX_TOTAL_DIGITS_EXCEEDED = 27,
    ELMX_UNABLE_TO_READ_ATTRIBUTE_VALUE = 28,
    ELMX_UNABLE_TO_READ_ELEMENT_VALUE = 29,
    ELMX_UNEXPECTED_ALL_EVENT = 30,
    ELMX_UNEXPECTED_ELEMENT_EVENT = 31,
    ELMX_UNEXPECTED_EMPTY_ELEMENT = 32,
    ELMX_UNEXPECTED_ENTITY = 33,
    ELMX_UNEXPECTED_EOM = 34,
    ELMX_VALUE_BAD_FORMAT = 35,
    ELMX_VALUE_EXCEEDS_MAX = 36,
    ELMX_VALUE_EXCEEDS_MIN = 37,
    ELMX_VALUE_NOT_ENUMERATED = 38,
    //------------------------
    // Added in V2.1
    ELMX_UNEXPECTED_ATTRIBUTE = 39,
    ELMX_NILLED_ELEMENT_NOT_EMPTY = 40,
    //------------------------
    // Added in V2.7
    ELMX_ENTITY_NOT_FOUND = 41,
    //------------------------
    // Added in V3.0
    ELMX_UNDEFINED_ERROR = 42,   // Used as an initialization value that can be later overwritten with a valid error code
    //------------------------
    // Added in V3.2
    ELMX_VALUE_DOES_NOT_MATCH_FIXED_CONSTRAINT = 43,
    //------------------------
    // Added in V3.4
    ELMX_UNKNOWN_XSI_TYPE = 44,
    ELMX_TYPE_NOT_SET = 45,
    //------------------------
    // Added in V3.8
    ELMX_OCCURRENCE_ERROR = 46,
    //------------------------
    ELMX_LAST_ERROR = 47,

    //------------------------
    // Added in V3.8 - primarily for use by users in snippet event handlers
    ELMX_USER_DEFINED_1 = 1000,
    ELMX_USER_DEFINED_2 = 1001,
    ELMX_USER_DEFINED_3 = 1002,
    ELMX_USER_DEFINED_4 = 1003,
    ELMX_USER_DEFINED_5 = 1004,
    ELMX_USER_DEFINED_6 = 1005,
    ELMX_USER_DEFINED_7 = 1006,
    ELMX_USER_DEFINED_8 = 1007,
    ELMX_USER_DEFINED_9 = 1008
};

enum elxp_xml_event
{
    // General events
    EXE_EOM,
    EXE_UNKNOWN,
    EXE_FATAL_ERROR,

    // XML events
    EXE_BADLY_FORMED,
    EXE_START_TAG_END,
    EXE_CDATA,
    EXE_ELEMENT_END,
    EXE_TOKEN,

    EXE_XSI_TYPE,
    EXE_XSI_NIL,
    EXE_XSI_SCHEMALOCATION,
    EXE_XSI_NONAMESPACESCHEMALOCATION,

    EXE_HAS_SKIPPED_UNKNOWN_ELEMENTS,

    EXE_LAST_XML_EVENT
};

enum elmx_whitespace { EXWS_PRESERVE, EXWS_REPLACE, EXWS_COLLAPSE };

//------------------------------------------------------------------------------
//                 Utility functions for initialising constants
//------------------------------------------------------------------------------

// V3.9.2 - String and binary initialisers moved to lmxuser.h

typedef char tc_utf8_buf[6+1];

// V3.3
inline tc_qname make_qname(
                    const std::string & r_qname_in,
                    const std::string & r_namespace_in,
                    const std::string & r_local_name_in )
{
    tc_qname qname;
    make_qname( &qname, r_qname_in, r_namespace_in, r_local_name_in );
    return qname;
}

//------------------------------------------------------------------------------
//                         Error code mapping support
//------------------------------------------------------------------------------

struct s_error_description
{
    elmx_error      code;
    const char *    to_string;
    const char *    p_description;
};

LMX_PDECL const s_error_description * get_error_mapping( elmx_error error_code_in );
LMX_PDECL const char * to_string( elmx_error error_code_in );
LMX_PDECL const char * get_error_description( elmx_error error_code_in );

//------------------------------------------------------------------------------
//                           LMX exception classes
// These are designed to be non-throwing.  Interogate c_xml_reader or
// c_xml_writer within the catch block to find out more information.
//------------------------------------------------------------------------------

class c_lmx_exception : public std::exception     // C130509_1309 // C160721_1012
{
private:
    elmx_error m_error_code;

public:
    c_lmx_exception( elmx_error error_code_in ) : m_error_code( error_code_in ) {}
    virtual ~c_lmx_exception() throw() {}   // v2.11.3 // C130509_1312
    elmx_error get_error() const { return m_error_code; }
    virtual const char * what() const throw()
    {
        return get_error_description( m_error_code );
    }
};

class c_lmx_exception_with_name : public c_lmx_exception  // V3.8.1 // C160721_1012
{
private:
    const char * mp_item_name;

public:
    c_lmx_exception_with_name( elmx_error error_code_in, const char * p_item_name_in ) :
            c_lmx_exception( error_code_in ), mp_item_name( p_item_name_in )
    {}
    virtual ~c_lmx_exception_with_name() throw() {} // C130509_1312
    const char * get_item_name() const { return mp_item_name; }
};

class c_lmx_reader_exception : public c_lmx_exception // C160721_1012
{
private:
    int m_line;

public:
    c_lmx_reader_exception( elmx_error error_code_in, int line_in ) :
            c_lmx_exception( error_code_in ), m_line( line_in )
    {}
    int get_line() const { return m_line; }
};

class c_check_exception : public std::exception
{
private:
    std::string location;
    const char * p_prog_file;
    int prog_line;

public:
    c_check_exception() : p_prog_file( "(not set)" ), prog_line( -1 ) {}
    c_check_exception( const std::string & r_location_in, const char * p_prog_file_in, int prog_line_in )
        : location( r_location_in ), p_prog_file( p_prog_file_in ), prog_line( prog_line_in )
    {}
    LMX_CC_DEFAULT( c_check_exception( const c_check_exception & ) )
    LMX_CC_DEFAULT( c_check_exception & operator = ( const c_check_exception & ) )
    virtual ~c_check_exception() throw() {}

    virtual const char * what() const throw() { return "lmx check exception"; }

    void set_location( const std::string & r_location_in ) { location = r_location_in; }
    const std::string & get_location() const { return location; }
    void set_prog_file( const char * p_prog_file_in ) { p_prog_file = p_prog_file_in; }
    const char * get_prog_file() const { return p_prog_file; }
    void set_prog_line( int prog_line_in ) { prog_line = prog_line_in; }
    int get_prog_line() const { return prog_line; }
};

LMX_PDECL extern c_check_exception global_check_exception;

class c_check_tracker
{
private:
    struct c_members {
        c_check_exception * p_exception;
        c_check_tracker * p_tracker_prev;
        const char * p_node_name;
        c_members() : p_exception( 0 ), p_tracker_prev( 0 ), p_node_name( 0 ) {}
    } m;

public:
    c_check_tracker() {}
    c_check_tracker( c_check_exception * p_exception_in ) { m.p_exception = p_exception_in; }
    c_check_tracker( c_check_tracker * p_tracker_prev_in )
        { m.p_tracker_prev = p_tracker_prev_in; }
    c_check_tracker( c_check_tracker * p_tracker_prev_in, const char * p_node_name_in )
        { m.p_tracker_prev = p_tracker_prev_in; m.p_node_name = p_node_name_in; }

    bool set_name( const char * p_node_name_in ) { m.p_node_name = p_node_name_in; return true; }   // Returns true so it can be included in if statement shortcut operators
    bool named_check( bool condition_in, const char * p_node_name_in )
    {
        if( condition_in )
            m.p_node_name = p_node_name_in;
        return condition_in;
    }

    bool report( const char * p_prog_file_in, int prog_line_in )
    {
        std::string location;
        c_check_exception * p_exception = build_location( &location );
        c_check_exception check_exception( location, p_prog_file_in, prog_line_in );
        if( p_exception )
            *p_exception = check_exception;
        global_check_exception = check_exception;
#if LMX_USE_CHECK_EXCEPTIONS == 1
        throw check_exception;
#endif
        return false;
    }

private:
    c_check_exception * build_location( std::string * p_location_out ) const
    {
        c_check_exception * p_exception = m.p_exception;
        if( m.p_tracker_prev )
            p_exception = m.p_tracker_prev->build_location( p_location_out );
        if( m.p_node_name )
            p_location_out->append( "/" ).append( m.p_node_name );
        return p_exception;
    }
};

//------------------------------------------------------------------------------
//                 Reader and writer class forward references
//------------------------------------------------------------------------------

typedef int t_xml_event;
typedef const char * t_class_identity;
struct s_event_map;
struct s_known_elements;    // V3.7.5
struct s_ns_map;    // Added in V2.9.2
class c_xml_reader;
class c_xml_writer;

//------------------------------------------------------------------------------
//                          struct s_debug_error
//  This structure provides a way to capture (un)marshalling error information
//  during code debugging.  It's prime purpose is to help debugging when the
//  convenience methods are being used.  If extended error information is
//  required in Release mode, then the Advanced forms of (un)marshalling should
//  be used.
//------------------------------------------------------------------------------

#if defined( _DEBUG ) && ! defined( LMX_DEBUG_CAPTURE_ERROR )
    #define LMX_DEBUG_CAPTURE_ERROR 1
#endif

struct s_custom_error_description   // Use in an array. Last entry should be { lmx::ELMX_OK, LMXNULL }
{
    elmx_error      code;
    const char *    p_description;
};  // C170428_1753

struct s_debug_error
{
    elmx_error error;
    std::string item_name;
    const char * p_prog_file;
    int prog_line;
    int xml_line;

    LMX_PDECL s_debug_error();
    LMX_PDECL void clear();
    LMX_PDECL void set( elmx_error error_in, const std::string & r_item_name_in,
                const char * p_prog_file_in, int prog_line_in, int xml_line_in = -1 );
    LMX_PDECL const char * get_description() const;
    LMX_PDECL const char * get_description( const s_custom_error_description * p_custom_error_description ) const;  // C170428_1753
    LMX_PDECL std::string to_string() const;
    LMX_PDECL std::string to_string( const s_custom_error_description * p_custom_error_description ) const;  // C170428_1753
    LMX_PDECL std::ostream & to_stream( std::ostream & r_os_in ) const;
    LMX_PDECL std::ostream & to_stream( std::ostream & r_os_in, const s_custom_error_description * p_custom_error_description  ) const;  // C170428_1753
};

LMX_PDECL std::ostream & operator << ( std::ostream & r_os_in, const s_debug_error & r_debug_error_in );

#if defined( LMX_DEBUG_CAPTURE_ERROR ) && LMX_DEBUG_CAPTURE_ERROR == 1
    LMX_PDECL extern s_debug_error debug_error;

    #define MK_LMX_DEBUG_ERROR_PTR( x ) lmx::s_debug_error *x = &lmx::debug_error;
    #define LMX_OUTPUT_DEBUG_ERROR( xstream ) xstream << lmx::debug_error
#else
    #define MK_LMX_DEBUG_ERROR_PTR( x )
    #define LMX_OUTPUT_DEBUG_ERROR( xstream )
#endif

//------------------------------------------------------------------------------
//                          c_custom_error_reporter
//------------------------------------------------------------------------------

class c_custom_error_reporter
{
private:
    const s_debug_error & r_debug_error;
    const s_custom_error_description * p_custom_error_descriptions;

    c_custom_error_reporter & operator = ( const c_custom_error_reporter & );

public:
    c_custom_error_reporter(
            const s_debug_error & r_debug_error_in,
            const s_custom_error_description * p_custom_error_descriptions_in )
        :
        r_debug_error( r_debug_error_in ),
        p_custom_error_descriptions( p_custom_error_descriptions_in )
    {}
    const char * get_description() const
    {
        return r_debug_error.get_description( p_custom_error_descriptions );
    }
    std::string to_string() const
    {
        return r_debug_error.to_string( p_custom_error_descriptions );
    }
    std::ostream & to_stream( std::ostream & r_os_in ) const
    {
        return r_debug_error.to_stream( r_os_in, p_custom_error_descriptions );
    }
};

inline std::ostream & operator << ( std::ostream & r_os_in, const c_custom_error_reporter & r_error_in )
{
    return r_error_in.to_stream( r_os_in );
}

//------------------------------------------------------------------------------
// Older compilers do not support two phase template instantiation.  These
// classes allow us to get around this problem using the arg::grin_ptr technique
// described at: http://www.octopull.demon.co.uk/arglib/TheGrin.html
//------------------------------------------------------------------------------

template< class T >
class ct_grin_ptr_deleter
{
private:
    typedef void (* tpfn_deleter)( T * );
    tpfn_deleter m_pfn_deleter;

public:
    ct_grin_ptr_deleter() : m_pfn_deleter( &deleter ) {}
    void release( T * ap ) { m_pfn_deleter( ap ); }

private:
    static void deleter( T * ap ) { delete ap; }
};

template< class T >
class ct_happy_ptr_deleter
{
public:
    void release( T * ap ) { delete ap; }

};

#if LMX_TWO_PHASE_TEMPLATES_SUPPORTED == 1
    template< class T >
    class ct_grin_or_happy_ptr_deleter : public ct_happy_ptr_deleter< T > {};
#else
    template< class T >
    class ct_grin_or_happy_ptr_deleter : public ct_grin_ptr_deleter< T > {};
#endif

//------------------------------------------------------------------------------
// ct_pod_container, ct_non_pod_container & ct_clonable_container:
// Low level containers
//------------------------------------------------------------------------------

template< typename T, typename TC = std::vector< T > >
class ct_pod_container : public TC
{
public:
    typedef ct_pod_container< T, TC > Tself;
    typedef TC Trawcontainer;
    typedef TC & Trangebounds;   // C181030_1203
    typedef const TC & Tconst_rangebounds;

    T & at( size_t index_in )
    {
        #if defined( __GNUC__ ) && __GNUC__ == 2
            return (*this)[ index_in ];
        #else
            return TC::at( index_in );
        #endif
    }
    const T & at( size_t index_in ) const
    {
        #if defined( __GNUC__ ) && __GNUC__ == 2
            return (*this)[ index_in ];
        #else
            return TC::at( index_in );
        #endif
    }
    void insert( size_t index_in, const T & r_new_in )
    {
        if( index_in >= TC::size() )
            TC::push_back( r_new_in );    // C121119_0851
        else
            TC::insert( TC::begin() + index_in, r_new_in );
    }
    void erase( size_t index_in )
    {
        lmx_assert( index_in < TC::size() );
        TC::erase( TC::begin() + index_in );
    }

    Trangebounds range_bounds() { return *this; }   // Call .begin() and .end() on return value (C181030_1203)
    Tconst_rangebounds const_range_bounds() const { return *this; }

    Trawcontainer & raw_container() { return *this; }
    const Trawcontainer & raw_container() const { return *this; }
};

template<>
class ct_pod_container< bool > : public ct_pod_container< bool, std::deque< bool > >
{};

template< typename T, typename TC = std::vector< T * >, typename Tdeleter = ct_happy_ptr_deleter< T > >
class ct_non_pod_container : public Tdeleter
{
private:
    TC m_container;

public:
    typedef ct_non_pod_container< T, TC, Tdeleter > Tself;
    typedef typename TC::const_iterator Tconst_iterator;
    typedef typename TC::iterator Titerator;
    typedef TC Trawcontainer;

    // C181030_1203 - Support for ranged-for
    struct st_range_bounds
    {
        TC & r_container;
        struct iterator
        {
            iterator( const Titerator & iter_in ) : iter( iter_in ) {}  // C190708_1452
            Titerator iter;
            bool operator != ( const iterator & r_rhs ) const { return iter != r_rhs.iter; }
            iterator & operator ++ () { ++iter; return *this; }
            T & operator * () const { return **iter; }
        };
        st_range_bounds( TC & r_container_in ) : r_container( r_container_in ) {}
        iterator begin() { return iterator( r_container.begin() ); }
        iterator end() { return iterator( r_container.end() ); }
    };
    struct st_const_range_bounds
    {
        const TC & r_container;
        struct const_iterator
        {
            const_iterator( const Tconst_iterator & iter_in ) : iter( iter_in ) {}  // C190708_1452
            Tconst_iterator iter;
            bool operator != ( const const_iterator & r_rhs ) const { return iter != r_rhs.iter; }
            const_iterator & operator ++ () { ++iter; return *this; }
            const T & operator * () const { return **iter; }
        };
        st_const_range_bounds( const TC & r_container_in ) : r_container( r_container_in ) {}
        const_iterator begin() const { return const_iterator( r_container.begin() ); }
        const_iterator end() const { return const_iterator( r_container.end() ); }
    };

    typedef st_range_bounds Trangebounds;
    typedef st_const_range_bounds Tconst_rangebounds;

    ct_non_pod_container()
    {}
    ct_non_pod_container( const Tself & r_rhs_in )    // C120312_1122
    {
        Tself temp;
        for( Tconst_iterator i( r_rhs_in.m_container.begin() ), i_end( r_rhs_in.m_container.end() );
                i != i_end;
                ++i )
        {
            typename uniq_ptr< T >::type lap_new( new T( **i ) );
            temp.push_back( lap_new );
        }
        swap( temp );
    }
    ~ct_non_pod_container()
    {
        clear();
    }
    Tself & operator = ( const Tself & r_rhs_in )
    {
        Tself temp( r_rhs_in );
        swap( temp );
        return *this;
    }
    void swap( Tself & r_rhs_in )
    {
        m_container.swap( r_rhs_in.m_container );
    }
    Tconst_iterator begin() const
    {
        return m_container.begin();
    }
    Titerator begin()
    {
        return m_container.begin();
    }
    Tconst_iterator end() const
    {
        return m_container.end();
    }
    Titerator end()
    {
        return m_container.end();
    }
    void push_back( T * p_new_in )
    {
        m_container.push_back( p_new_in );
    }
    void push_back( typename uniq_ptr< T >::type & arap_new )
    {
        m_container.push_back( arap_new.get() );
        arap_new.release();
    }
    void push_back( const T & r_new_in )
    {
        typename uniq_ptr< T >::type lap_t( new T( r_new_in ) );
        push_back( lap_t );
    }
    void push_back_w_autop( T * p_new_in )
    {
        typename uniq_ptr< T >::type lap_t( p_new_in );
        push_back( lap_t );
    }
    void insert( size_t index_in, typename uniq_ptr< T >::type & arap_new )
    {
        if( index_in >= size() )
            push_back( arap_new );
        else
        {
            m_container.insert( m_container.begin() + index_in, arap_new.get() );
            arap_new.release();
        }
    }
    void insert( size_t index_in, const T & r_new_in )
    {
        insert_w_autop( index_in, new T( r_new_in ) );
    }
    void insert_w_autop( size_t index_in, T * p_new_in )
    {
        typename uniq_ptr< T >::type lap_t( p_new_in );
        insert( index_in, lap_t );
    }
    T & assign( size_t index_in, T * p_new_in )    // Added to support C110301_1621
    {
        if( index_in >= size() )
        {
            push_back( p_new_in );
            return back();
        }
        else
        {
            if( ! p_new_in )
            {
                T *p_old = m_container[ index_in ];
                m_container[ index_in ] = 0;
                return *p_old;     // Assigning NULL pointer causes object to surrender ownership
            }
            delete m_container[ index_in ];
            m_container[ index_in ] = p_new_in;
        }
        return *(m_container[ index_in ]);
    }
    T & at( size_t index_in )
    {
        #if defined( __GNUC__ ) && __GNUC__ == 2
            return *(m_container[ index_in ]);
        #else
            return *(m_container.at( index_in ));
        #endif
    }
    const T & at( size_t index_in ) const
    {
        #if defined( __GNUC__ ) && __GNUC__ == 2
            return *(m_container[ index_in ]);
        #else
            return *(m_container.at( index_in ));
        #endif
    }
    T & operator [] ( size_t index_in )
    {
        return *(m_container[ index_in ]);
    }
    const T & operator [] ( size_t index_in ) const
    {
        return *(m_container[ index_in ]);
    }
    T & back()
    {
        return *(m_container.back());
    }
    const T & back() const
    {
        return *(m_container.back());
    }
    void erase( size_t index_in )
    {
        lmx_assert( index_in < size() );
        Tdeleter::release( *(m_container.begin() + index_in) );
        m_container.erase( m_container.begin() + index_in );
    }
    void clear()
    {
        for( Titerator i( m_container.begin() ), i_end( m_container.end() ); i != i_end; ++i )
            Tdeleter::release( *i );
        m_container.clear();
    }
    size_t size() const { return m_container.size(); }
    void resize( size_t new_size_in )
    {
        while( m_container.size() < new_size_in )
            push_back_w_autop( new T );
    }
    bool empty() const { return m_container.empty(); }
    bool operator == ( const Tself & r_rhs_in ) const
    {
        if( size() != r_rhs_in.size() )
            return false;
        for( size_t i=0; i<size(); ++i )
            if( (*this)[i] != r_rhs_in[i] )
                return false;
        return true;
    }
    bool operator != ( const Tself & r_rhs_in ) const { return ! (*this == r_rhs_in); }

    Trangebounds range_bounds() { return Trangebounds( m_container ); }   // Call .begin() and .end() on return value (C181030_1203)
    Tconst_rangebounds const_range_bounds() const { return Tconst_rangebounds( m_container ); }

    Trawcontainer & raw_container() { return m_container; }
    const Trawcontainer & raw_container() const { return m_container; }
};

template< typename T, typename TC = std::vector< T * >, typename Tdeleter = ct_grin_or_happy_ptr_deleter< T > >
class ct_clonable_container : public ct_non_pod_container< T, TC, Tdeleter >
{
public:
    typedef ct_clonable_container< T, TC, Tdeleter > Tself;
    typedef ct_non_pod_container< T, TC, Tdeleter > Tbase;

    ct_clonable_container() {}
    ct_clonable_container( const Tself & r_rhs_in ) : ct_non_pod_container< T, TC, Tdeleter >() // Be explicit about not using ct_non_pod_container copy constructor
    {
        for( typename TC::const_iterator i( r_rhs_in.raw_container().begin() ), i_end( r_rhs_in.raw_container().end() ); i != i_end; ++i )
            Tbase::push_back_w_autop( (*i)->clone() );
    }
    void clone( const Tself & r_rhs_in )
    {
        Tself t( r_rhs_in );
        Tbase::swap( t );
    }
    Tself & operator = ( const Tself & r_rhs_in )
    {
        clone( r_rhs_in );  // clone() is exception safe
        return *this;
    }
};

//------------------------------------------------------------------------------
//                  Classes for representing special schema types
//------------------------------------------------------------------------------

struct s_namesapace_context_item    // v4.1.2
{
    std::string m_ns_prefix;
    std::string ns_uri;
    s_namesapace_context_item( const std::string & r_ns_prefix_in, const std::string & r_ns_uri_in )
        : m_ns_prefix( r_ns_prefix_in ), ns_uri( r_ns_uri_in )
        {}
};

class c_namespace_context   // v4.1.2
{
private:
    typedef std::vector< s_namesapace_context_item > t_container;
    t_container m_container;

public:
    c_namespace_context()
    {
        m_container.reserve( 6 );    // We don't expect many ns to uri mappings so keep it simple
    }
    c_namespace_context( const c_namespace_context & r_rhs_in ) : m_container( r_rhs_in.m_container )
    {}
    c_namespace_context & operator = ( const c_namespace_context & r_rhs_in )
    {
        c_namespace_context( r_rhs_in ).swap( *this );
        return *this;
    }
    void swap( c_namespace_context & r_rhs_in ) { m_container.swap( r_rhs_in.m_container ); }   // C160420_1332
    void add( const std::string &r_ns_prefix_in, const std::string &r_ns_uri_in )
    {
        for( size_t i=0; i<m_container.size(); ++i )
            if( m_container[i].m_ns_prefix == r_ns_prefix_in )
            {
                m_container[i].ns_uri = r_ns_uri_in;
                return;
            }
        m_container.push_back( s_namesapace_context_item( r_ns_prefix_in, r_ns_uri_in ) );
    }
    size_t size() const { return m_container.size(); }
    void clear() { m_container.clear(); }   // C160421_1501
    const std::string & get_ns_prefix( size_t index_in ) const { return m_container[index_in].m_ns_prefix; }
    const std::string & get_ns_uri( size_t index_in ) const { return m_container[index_in].ns_uri; }
};

class c_any_info
{
private:
    std::string m_namespace;
    std::string m_name;
    std::string m_value;
    c_namespace_context m_namespace_context;  // v4.1.2

public:
    c_any_info()
    {
    }
    LMX_CC_DEFAULT( c_any_info( const c_any_info & ) )
    // For writing during unmarshaling
    c_any_info( const std::string & r_namespace_in,
                const std::string & r_name_in,
                const std::string & r_value_in ) :
        m_namespace( r_namespace_in ),
        m_name( r_name_in ),
        m_value( r_value_in )
    {
    }
    // For user writing xs:anyAttribute
    c_any_info( const std::string & r_name_in,
                const std::string & r_value_in ) :
        m_name( r_name_in ),
        m_value( r_value_in )
    {
    }
    // For user writing xs:any
    c_any_info( const std::string & r_value_in ) :
        m_value( r_value_in )
    {
    }
    LMX_CC_DEFAULT( c_any_info & operator = ( const c_any_info & ) )
    const std::string & get_namespace() const { return m_namespace; }
    const std::string & get_name() const { return m_name; }
    const std::string & get_value() const { return m_value; }
    std::string get_local_name() const  // v4.1.2
    {
        std::string::size_type colon_pos = m_name.find( ':' );

        if( colon_pos == std::string::npos )
            return m_name;
        return m_name.substr( colon_pos+1, std::string::npos );
    }

    void set_namespace( const std::string & r_namespace_in ) { m_namespace = r_namespace_in; }
    void set_name( const std::string & r_name_in ) { m_name = r_name_in; }
    void set_value( const std::string & r_value_in ) { m_value = r_value_in; }

    const c_namespace_context & get_namespace_context() const { return m_namespace_context; }
    c_namespace_context & get_namespace_context() { return m_namespace_context; }

    bool operator == ( const c_any_info & ) const { return true; }  // v5 - Contents of c_any_info ignored during comparison
    bool operator != ( const c_any_info & ) const { return false; } // v5
};

typedef std::vector< c_any_info * > t_any_info_vector;
typedef ct_non_pod_container< c_any_info > t_any_info_container;

class c_empty
{
public:
    bool operator == ( const c_empty & ) const { return true; }     // v5
    bool operator != ( const c_empty & ) const { return false; }    // v5

public: // C110918_1018: To keep gcc 323 and 423 happy (323 doesn't like to swap empty objects, 423 doesn't like no initialisation)
    c_empty() : dummy( 0 ) {}
    char dummy;
};

//------------------------------------------------------------------------------
//                                LMXG2 Classes
//------------------------------------------------------------------------------

class c_handling_qname {};

class c_handling_ur_type {};

// To allow error free symmetry between ct_non_mixed and ct_mixed classes
static tlmx_unicode_string m_unused_non_mixed_text;

template< typename T >
class ct_non_mixed
{
private:
    T m_value;

public:
    ct_non_mixed() : m_value( T() ) {}        // C110507_1734 - Added initialisers for template members
    ct_non_mixed( const T & r_v_in ) : m_value( r_v_in ) {}
    LMX_CC_DEFAULT( ct_non_mixed( const ct_non_mixed & ) )
    LMX_CC_DEFAULT( ct_non_mixed & operator = ( const ct_non_mixed & ) )
    void swap( ct_non_mixed< T > & r_rhs_in ) { lmx::swap( m_value, r_rhs_in.m_value ); }

    bool operator == ( const ct_non_mixed< T > & r_rhs_in ) const { return m_value == r_rhs_in.m_value; }
    bool operator != ( const ct_non_mixed< T > & r_rhs_in ) const { return ! (*this == r_rhs_in); }

    static bool is_in_mixed() { return false; }

    T & ref_value() { return m_value; }
    const T & ref_value() const { return m_value; }
    tlmx_unicode_string & ref_text() { lmx_assert( 0 ); return m_unused_non_mixed_text; }
    const tlmx_unicode_string & ref_text() const { lmx_assert( 0 ); return m_unused_non_mixed_text; }

    // Allow treating object as simply an instance of T if required
    T & operator = ( const T & r_v_in ) { m_value = r_v_in; return m_value; }
    operator T & () { return m_value; }
    operator const T & () const { return m_value; }
};

template< typename T >
class ct_mixed
{
private:
    T m_value;
    tlmx_unicode_string m_text;

public:
    ct_mixed() : m_value( T() ) {}        // C110507_1734 - Added initialisers for template members
    ct_mixed( const T & r_v_in ) : m_value( r_v_in ) {}
    LMX_CC_DEFAULT( ct_mixed( const ct_mixed & ) )
    LMX_CC_DEFAULT( ct_mixed & operator = ( const ct_mixed & ) )
    void swap( ct_mixed< T > & r_rhs_in ) { lmx::swap( m_value, r_rhs_in.m_value ); lmx::swap( m_text, r_rhs_in.m_text ); }

    bool operator == ( const ct_mixed< T > & r_rhs_in ) const { return m_value == r_rhs_in.m_value && m_text == r_rhs_in.m_text; }
    bool operator != ( const ct_mixed< T > & r_rhs_in ) const { return ! (*this == r_rhs_in); }

    static bool is_in_mixed() { return true; }

    T & ref_value() { return m_value; }
    const T & ref_value() const { return m_value; }
    tlmx_unicode_string & ref_text() { return m_text; }
    const tlmx_unicode_string & ref_text() const { return m_text; }

    // Allow treating object as simply an instance of T if required
    T & operator = ( const T & r_v_in ) { m_value = r_v_in; return m_value; }
    operator T & () { return m_value; }
    operator const T & () const { return m_value; }
};

template< typename Treader, typename Tcollection, typename Tatomic_type, typename Thandling >
class c_unmarshal_bridge;

template< typename Treader, typename Tcollection, typename Tatomic_type, typename Thandling >
class c_unmarshal_list_bridge;

template< typename Twriter, typename Tcollection, typename Tatomic_type, typename Thandling >
class c_marshal_bridge;

template< typename Twriter, typename Tcollection, typename Tatomic_type, typename Thandling >
class c_marshal_list_bridge;

template< typename Tatomic_type >
class ct_typed_validation_spec;

// For simple POD (Plain Ordinary Data) types such as int, float etc., that
// we can store directly in a std::vector
template< typename T, typename Thandling = T, typename Twrapper = ct_non_mixed< T > >
class ct_simple_pod_single
{
private:
    Twrapper m_value;
    bool m_is_set;

public:
    typedef ct_simple_pod_single< T, Thandling, Twrapper > Tself;
    typedef T t_atomic_type;
    typedef Thandling t_handling;

    ct_simple_pod_single() : m_is_set( false ) {}
    LMX_CC_DEFAULT( ct_simple_pod_single( const Tself & ) )
    Tself & operator = ( const Tself & r_rhs_in )
    {
        Tself t( r_rhs_in ); swap( t );
        return *this;
    }
    ~ct_simple_pod_single() {}
    void swap( Tself & r_rhs_in ) { m_value.swap( r_rhs_in.m_value ); std::swap( m_is_set, r_rhs_in.m_is_set ); }

    // User interface methods
    const T & get() const { return m_value.ref_value(); }
    T & get() { return m_value.ref_value(); }
    elmx_error set( const T & r_v_in ) { m_value.ref_value() = r_v_in; m_is_set = true; return ELMX_OK; }
    const tlmx_unicode_string & get_text() const { return m_value.ref_text(); }
    tlmx_unicode_string & get_text() { return m_value.ref_text(); }
    void set_text( const tlmx_unicode_string & r_t_in ) { m_value.ref_text() = r_t_in; m_is_set = true; }
    bool is_value_set() const { return m_is_set; }  // C100510_1423
    void unset() { m_is_set = false; }
    void unset( const T & r_default_in ) { m_value.ref_value() = r_default_in; m_is_set = false; }
    bool is_occurs_ok( bool LMX_UNUSED_PARAM( is_required_in ) ) const { return m_is_set; }  // See note is_occurs_ok_is_required

    bool operator == ( const Tself & r_rhs_in ) const
    {
        return m_is_set == r_rhs_in.m_is_set && (! m_is_set || m_value == r_rhs_in.m_value);
    }
    bool operator != ( const Tself & r_rhs_in ) const { return ! (*this == r_rhs_in); }

public:
    // For c_xml_reader and c_xml_writer access

    bool is_in_mixed() const { return Twrapper::is_in_mixed(); }

    // Unmarshal interface methods
    bool is_already_unmarshalled() const { return is_value_set(); }
    void mk_new_space() { m_is_set = true; }
    tlmx_unicode_string & get_new_space_text_ref() { return m_value.ref_text(); }
    T & get_new_space_value_ref() { return m_value.ref_value(); }

    // Marshaling interface methods
    size_t count() const { return 1; }      // C120411_0908
    const tlmx_unicode_string & access_text( size_t LMX_UNUSED_PARAM( i ) ) const { return m_value.ref_text(); }
    const T & access_value( size_t LMX_UNUSED_PARAM( i ) ) const { return m_value.ref_value(); }

    template< typename Treader >    // C121120_1245
    c_unmarshal_bridge< Treader, Tself, t_atomic_type, t_handling > get_unmarshal_bridge( Treader & r_reader_in, const ct_typed_validation_spec< t_atomic_type > & r_item_spec_in )
    {
        return c_unmarshal_bridge< Treader, Tself, t_atomic_type, t_handling >( r_reader_in, *this, r_item_spec_in );
    }
    template< typename Twriter >
    c_marshal_bridge< Twriter, Tself, t_atomic_type, t_handling > get_marshal_bridge( Twriter & r_writer_in, const ct_typed_validation_spec< t_atomic_type > & r_item_spec_in ) const
    {
        return c_marshal_bridge< Twriter, Tself, t_atomic_type, t_handling >( r_writer_in, *this, r_item_spec_in );
    }
};

template< typename T, typename Thandling = T, typename Twrapper = ct_non_mixed< T > >
class ct_simple_pod_optional : public ct_simple_pod_single< T, Thandling, Twrapper >
{
public:
    typedef ct_simple_pod_optional< T, Thandling, Twrapper > Tself;
    typedef ct_simple_pod_single< T, Thandling, Twrapper > Tbase;  // See http://www.parashift.com/c++-faq-lite/templates.html#faq-35.19
    typedef T t_atomic_type;
    typedef Thandling t_handling;

    LMX_CC_DEFAULT( ct_simple_pod_optional() )
    LMX_CC_DEFAULT( ct_simple_pod_optional( const Tself & ) )
    LMX_CC_DEFAULT( Tself & operator = ( const Tself & ) )
    ~ct_simple_pod_optional() {}

    // Additional user interface methods
    bool is_occurs_ok( bool is_required_in ) const { return is_required_in ? Tbase::is_value_set() : true; }    // See note is_occurs_ok_is_required

    // Marshaling interface methods
    size_t count() const { return Tbase::is_value_set() ? 1 : 0; }  // C120411_0908

    template< typename Treader >    // C121120_1245
    c_unmarshal_bridge< Treader, Tself, t_atomic_type, t_handling > get_unmarshal_bridge( Treader & r_reader_in, const ct_typed_validation_spec< t_atomic_type > & r_item_spec_in )
    {
        return c_unmarshal_bridge< Treader, Tself, t_atomic_type, t_handling >( r_reader_in, *this, r_item_spec_in );
    }
    template< typename Twriter >
    c_marshal_bridge< Twriter, Tself, t_atomic_type, t_handling > get_marshal_bridge( Twriter & r_writer_in, const ct_typed_validation_spec< t_atomic_type > & r_item_spec_in ) const
    {
        return c_marshal_bridge< Twriter, Tself, t_atomic_type, t_handling >( r_writer_in, *this, r_item_spec_in );
    }
};

template< typename T, typename Thandling = T, typename Twrapper = ct_non_mixed< T >, typename TC = ct_pod_container< Twrapper > >
class ct_simple_pod_multi
{
private:
    TC m_values;

public:
    typedef ct_simple_pod_multi< T, Thandling, Twrapper, TC > Tself;
    typedef T t_atomic_type;
    typedef Thandling t_handling;
    typedef typename TC::Trawcontainer Tcontainer;
    typedef typename TC::Trangebounds Trange;   // C181030_1203
    typedef typename TC::Tconst_rangebounds Tconst_range;

    ct_simple_pod_multi() {}
    LMX_CC_DEFAULT( ct_simple_pod_multi( const Tself & ar_rhs ) )
    Tself & operator = ( const Tself & r_rhs_in )
    {
        Tself t( r_rhs_in ); swap( t );
        return *this;
    }
    ~ct_simple_pod_multi() {}
    void swap( Tself & r_rhs_in )
    {
        m_values.swap( r_rhs_in.m_values );
    }

    // User interface methods
    const T & get( size_t index_in ) const       // For read access
    {
        lmx_assert( index_in < m_values.size() );
        return m_values.at( index_in ).ref_value();
    }
    T & get( size_t index_in )                   // For read/write access
    {
        while( m_values.size() <= index_in )       // C100902_2046
            append( T() );
        return m_values[index_in].ref_value();
    }
    const T & get() const       // For read access - C110301_1550 - For when multi occurs is restricted to single or optional
    {
        return get( 0 );
    }
    T & get()                // For read/write access - C110301_1550 - For when multi occurs is restricted to single or optional
    {
        return get( 0 );
    }
    const T & back( size_t index_in ) const  // For read access
    {
        return m_values.back().ref_value();
    }
    T & back( size_t index_in )              // For read/write access
    {
        return m_values.back().ref_value();
    }
    elmx_error set( size_t index_in, const T & r_value_in )
    {
        while( m_values.size() <= index_in )       // C100902_2046
            append( r_value_in );
        m_values[index_in].ref_value() = r_value_in;
        return ELMX_OK;
    }
    elmx_error set( const T & r_value_in )       // C110301_1550 - For when multi occurs is restricted to single or optional
    {
        return set( 0, r_value_in );
    }
    void append( const T & r_value_in )
    {
        m_values.push_back( r_value_in );
    }
    void insert( size_t index_in, const T & r_value_in )
    {
        m_values.insert( index_in, r_value_in );
    }
    void erase( size_t index_in )
    {
        m_values.erase( index_in );
    }
    void clear() { m_values.clear(); }
    size_t size() const { return m_values.size(); }
    bool empty() const { return m_values.empty(); }
    bool is_value_set() const { return ! empty(); } // C110301_1550 - For when multi occurs is restricted to single or optional
    void unset() { clear(); }   // C110301_1550 - For when multi occurs is restricted to single or optional
    bool is_occurs_ok( size_t min_occurs_in ) const  // For maxOccurs unbounded
    {
        return m_values.size() >= min_occurs_in;
    }
    bool is_occurs_ok( size_t min_occurs_in, size_t max_occurs_in ) const
    {
        return min_occurs_in <= m_values.size() && m_values.size() <= max_occurs_in;
    }
    bool operator == ( const Tself & r_rhs_in ) const
    {
        return m_values == r_rhs_in.m_values;
    }
    bool operator != ( const Tself & r_rhs_in ) const { return ! (*this == r_rhs_in); }

    Trange range_bounds() { return m_values.range_bounds(); }   // C181030_1203
    Tconst_range const_range_bounds() const { return m_values.const_range_bounds(); }

    Tcontainer & raw_container() { return m_values.raw_container(); }
    const Tcontainer & raw_container() const { return m_values.raw_container(); }

public:
    // For c_xml_reader and c_xml_writer access

    bool is_in_mixed() const { return Twrapper::is_in_mixed(); }

    // Unmarshal interface methods
    bool is_already_unmarshalled() const { return is_value_set(); }
    void mk_new_space() { m_values.resize( m_values.size() + 1 ); }
    tlmx_unicode_string & get_new_space_text_ref() { return m_values.back().ref_text(); }
    T & get_new_space_value_ref() { return m_values.back().ref_value(); }

    // cit_multi can be multi or list
    // Marshaling interface methods
    size_t count() const { return m_values.size(); }
    const tlmx_unicode_string & access_text( size_t i ) const { return m_values.at(i).ref_text(); }
    const T & access_value( size_t i ) const { return m_values.at(i).ref_value(); }

    template< typename Treader >    // C121120_1245
    c_unmarshal_bridge< Treader, Tself, t_atomic_type, t_handling > get_unmarshal_bridge( Treader & r_reader_in, const ct_typed_validation_spec< t_atomic_type > & r_item_spec_in )
    {
        return c_unmarshal_bridge< Treader, Tself, t_atomic_type, t_handling >( r_reader_in, *this, r_item_spec_in );
    }
    template< typename Twriter >
    c_marshal_bridge< Twriter, Tself, t_atomic_type, t_handling > get_marshal_bridge( Twriter & r_writer_in, const ct_typed_validation_spec< t_atomic_type > & r_item_spec_in ) const
    {
        return c_marshal_bridge< Twriter, Tself, t_atomic_type, t_handling >( r_writer_in, *this, r_item_spec_in );
    }
};

template< typename T, typename Thandling = T, typename TC = ct_pod_container< T >, typename Twrapper = ct_non_mixed< TC > >
class ct_simple_pod_list_single
{
protected:
    Twrapper list;
    bool is_present;    // A list may be present but empty and we need to be able to tell the difference

public:
    typedef ct_simple_pod_list_single< T, Thandling, TC, Twrapper > Tself;
    typedef T t_atomic_type;
    typedef Thandling t_handling;
    typedef typename TC::Trawcontainer Tcontainer;
    typedef typename TC::Trangebounds Trange;   // C181030_1203
    typedef typename TC::Tconst_rangebounds Tconst_range;

    ct_simple_pod_list_single() : is_present( false ) {}
    LMX_CC_DEFAULT( ct_simple_pod_list_single( const Tself & ) )
    Tself & operator = ( const Tself & r_rhs_in )
    {
        Tself t( r_rhs_in ); swap( t );
        return *this;
    }
    ~ct_simple_pod_list_single() {}
    void swap( Tself & r_rhs_in )
    {
        list.swap( r_rhs_in.list );
        std::swap( is_present, r_rhs_in.is_present );
    }

    const T & get( size_t index_in ) const       // For read access
    {
        lmx_assert( index_in < list.ref_value().size() );
        return list.ref_value().at(index_in);
    }
    T & get( size_t index_in )                   // For read/write access
    {
        is_present = true;
        while( list.ref_value().size() <= index_in )     // C100902_2046
            append( T() );
        return list.ref_value()[index_in];
    }
    const T & back( size_t index_in ) const  // For read access
    {
        return list.ref_value().back();
    }
    T & back( size_t index_in )              // For read/write access
    {
        return list.ref_value().back();
    }
    elmx_error set( size_t index_in, const T & r_value_in )
    {
        is_present = true;
        while( list.ref_value().size() <= index_in )     // C100902_2046
            append( r_value_in );
        list.ref_value()[index_in] = r_value_in;
        return ELMX_OK;
    }
    void append( const T & r_value_in )
    {
        list.ref_value().push_back( r_value_in );
        is_present = true;
    }
    void insert( size_t index_in, const T & r_value_in )
    {
        list.ref_value().insert( index_in, r_value_in );
        is_present = true;
    }
    void erase( size_t index_in )
    {
        list.ref_value().erase( index_in );
    }
    void clear() { list.ref_value().clear(); is_present = true; }  // C160504_1254
    size_t size() const { return list.ref_value().size(); }
    bool empty() const { return list.ref_value().empty(); }

    bool is_value_set() const { return is_present; }    // C100510_1423
    void unset() { list.ref_value().clear(); is_present = false; }
    bool is_occurs_ok( bool LMX_UNUSED_PARAM( is_required_in ) ) const { return is_present; }    // See note is_occurs_ok_is_required
    bool is_occurs_ok( size_t min_occurs_in, size_t max_occurs_in ) const
    {
        return min_occurs_in <= size() && size() <= max_occurs_in;
    }
    bool operator == ( const Tself & r_rhs_in ) const
    {
        return is_present == r_rhs_in.is_present && ( ! is_present || list == r_rhs_in.list);
    }
    bool operator != ( const Tself & r_rhs_in ) const { return ! (*this == r_rhs_in); }

    Trange range_bounds() { return list.ref_value().range_bounds(); }   // C181030_1203
    Tconst_range const_range_bounds() const { return list.ref_value().const_range_bounds(); }

    Tcontainer & raw_container() { return list.ref_value().raw_container(); }
    const Tcontainer & raw_container() const { return list.ref_value().raw_container(); }

public:
    // For c_xml_reader and c_xml_writer access

    bool is_in_mixed() const { return Twrapper::is_in_mixed(); }

    // Unmarshal interface methods
    bool is_already_unmarshalled() const { return is_value_set(); }
    void start_new_list() { is_present = true; }
    tlmx_unicode_string & get_new_space_text_ref() { return list.ref_text(); }  // Max 1 text item per list
    void mk_new_space() { list.ref_value().resize( list.ref_value().size() + 1 ); }
    T & get_new_space_value_ref() { return list.ref_value().back(); }
    size_t new_list_size() const { return list.ref_value().size(); }

    // Marshaling interface methods
    size_t list_count() const { return 1; }
    const tlmx_unicode_string & access_text( size_t LMX_UNUSED_PARAM( i ) ) const { return list.ref_text(); }
    size_t count( size_t LMX_UNUSED_PARAM( i ) ) const { return list.ref_value().size(); }
    const T & access_value( size_t LMX_UNUSED_PARAM( i ), size_t j ) const { return list.ref_value()[j]; }

    template< typename Treader >    // C121120_1245
    c_unmarshal_list_bridge< Treader, Tself, t_atomic_type, t_handling > get_unmarshal_list_bridge( Treader & r_reader_in, const ct_typed_validation_spec< t_atomic_type > & r_item_spec_in )
    {
        return c_unmarshal_list_bridge< Treader, Tself, t_atomic_type, t_handling >( r_reader_in, *this, r_item_spec_in );
    }
    template< typename Twriter >
    c_marshal_list_bridge< Twriter, Tself, t_atomic_type, t_handling > get_marshal_list_bridge( Twriter & r_writer_in, const ct_typed_validation_spec< t_atomic_type > & r_item_spec_in ) const
    {
        return c_marshal_list_bridge< Twriter, Tself, t_atomic_type, t_handling >( r_writer_in, *this, r_item_spec_in );
    }
};

template< typename T, typename Thandling = T, typename TC = ct_pod_container< T >, typename Twrapper = ct_non_mixed< TC > >
class ct_simple_pod_list_optional : public ct_simple_pod_list_single< T, Thandling, TC, Twrapper >
{
public:
    typedef ct_simple_pod_list_optional< T, Thandling, TC, Twrapper > Tself;
    typedef ct_simple_pod_list_single< T, Thandling, TC, Twrapper > Tbase;
    typedef T t_atomic_type;
    typedef Thandling t_handling;

    LMX_CC_DEFAULT( ct_simple_pod_list_optional() )
    LMX_CC_DEFAULT( ct_simple_pod_list_optional( const Tself & ) )
    LMX_CC_DEFAULT( Tself & operator = ( const Tself & ar_rhs ) )

    bool is_occurs_ok( bool is_required_in ) const { return is_required_in ? Tbase::is_present : true; }  // See note is_occurs_ok_is_required

public:
    // For c_xml_reader and c_xml_writer access

    // Unmarshal interface methods
    // using ct_simple_pod_list_single::start_new_list()
    // using ct_simple_pod_list_single::mk_new_space()
    // using ct_simple_pod_list_single::get_new_space_value_ref()

    // Marshaling interface methods
    size_t list_count() const { return Tbase::is_present ? 1 : 0; }
    // using ct_simple_pod_list_single::access_text( size_t i )
    // using ct_simple_pod_list_single::count( size_t i )
    // using ct_simple_pod_list_single::access_value( size_t i, size_t j )

    template< typename Treader >    // C121120_1245
    c_unmarshal_list_bridge< Treader, Tself, t_atomic_type, t_handling > get_unmarshal_list_bridge( Treader & r_reader_in, const ct_typed_validation_spec< t_atomic_type > & r_item_spec_in )
    {
        return c_unmarshal_list_bridge< Treader, Tself, t_atomic_type, t_handling >( r_reader_in, *this, r_item_spec_in );
    }
    template< typename Twriter >
    c_marshal_list_bridge< Twriter, Tself, t_atomic_type, t_handling > get_marshal_list_bridge( Twriter & r_writer_in, const ct_typed_validation_spec< t_atomic_type > & r_item_spec_in ) const
    {
        return c_marshal_list_bridge< Twriter, Tself, t_atomic_type, t_handling >( r_writer_in, *this, r_item_spec_in );
    }
};

template< typename T, typename Thandling = T, typename Twrapper = ct_non_mixed< ct_simple_list< T > >, typename TC = ct_non_pod_container< Twrapper > >
class ct_simple_pod_list_multi
{
private:
    TC m_lists;

public:
    typedef ct_simple_pod_list_multi< T, Thandling, Twrapper, TC > Tself;
    typedef T t_atomic_type;
    typedef Thandling t_handling;
    typedef typename TC::Trawcontainer Tcontainer;
    typedef typename TC::Trangebounds Trange;   // C181030_1203
    typedef typename TC::Tconst_rangebounds Tconst_range;

    LMX_CC_DEFAULT( ct_simple_pod_list_multi() )
    LMX_CC_DEFAULT( ct_simple_pod_list_multi( const Tself & ) )
    Tself & operator = ( const Tself & r_rhs_in )
    {
        Tself t( r_rhs_in ); swap( t );
        return *this;
    }
    ~ct_simple_pod_list_multi() {}

    void swap( Tself & r_rhs_in )
    {
        m_lists.swap( r_rhs_in.m_lists );
    }

    // User interface methods
    const ct_simple_list< T > & get( size_t index_in ) const // For read access
    {
        lmx_assert( index_in < m_lists.size() );
        return m_lists.at(index_in);
    }
    void append()                                       // For write access
    {
        m_lists.push_back_w_autop( new Twrapper );
    }
    ct_simple_list< T > & back()                    // For write access
     {
        return m_lists.back();
    }
    void insert( size_t index_in )                       // For write access
    {
        m_lists.insert_w_autop( index_in, new Twrapper );
    }
    ct_simple_list< T > & get( size_t index_in ) // For read/write access
    {
        while( m_lists.size() <= index_in )
            m_lists.push_back_w_autop( new Twrapper );
        return m_lists[index_in];
    }
    void erase( size_t index_in )
    {
        m_lists.erase( index_in );
    }
    void clear()
    {
        m_lists.clear();
    }
    ct_simple_list< T > & assign( size_t index_in, const ct_simple_list< T > & r_value_in ) // Deep copy
    {
        return get( index_in ) = r_value_in;
    }
    size_t size() const
    {
        return m_lists.size();
    }
    bool is_occurs_ok( size_t min_occurs_in ) const  // For unbounded upper limit
    {
        return min_occurs_in <= m_lists.size();
    }
    bool is_occurs_ok( size_t min_occurs_in, size_t max_occurs_in ) const
    {
        return min_occurs_in <= m_lists.size() && m_lists.size() <= max_occurs_in;
    }
    bool operator == ( const Tself & r_rhs_in ) const
    {
        return m_lists == r_rhs_in.m_lists;
    }
    bool operator != ( const Tself & r_rhs_in ) const { return ! (*this == r_rhs_in); }

    Trange range_bounds() { return m_lists.range_bounds(); }   // C181030_1203
    Tconst_range const_range_bounds() const { return m_lists.const_range_bounds(); }

    Tcontainer & raw_container() { return m_lists.raw_container(); }
    const Tcontainer & raw_container() const { return m_lists.raw_container(); }

public:
    // For c_xml_reader and c_xml_writer access

    bool is_in_mixed() const { return Twrapper::is_in_mixed(); }

    // Unmarshal interface methods
    bool is_already_unmarshalled() const { return ! m_lists.empty(); }
    void start_new_list() { m_lists.push_back_w_autop( new Twrapper ); }
    tlmx_unicode_string & get_new_space_text_ref() { return m_lists.back().ref_text(); }
    void mk_new_space() { m_lists.back().ref_value().grow(); }
    T & get_new_space_value_ref() { return m_lists.back().ref_value().back(); }
    size_t new_list_size() const { return m_lists.back().ref_value().size(); }

    // Marshal interface methods
    size_t list_count() const { return m_lists.size(); }  // Number of lists
    const tlmx_unicode_string & access_text( size_t i ) const { return m_lists[i].ref_text(); }
    size_t count( size_t i ) const { return m_lists[i].ref_value().size(); }      // Number of items in list i
    const T & access_value( size_t i, size_t j ) const { return m_lists[i].ref_value().get( j ); }

    template< typename Treader >    // C121120_1245
    c_unmarshal_list_bridge< Treader, Tself, t_atomic_type, t_handling > get_unmarshal_list_bridge( Treader & r_reader_in, const ct_typed_validation_spec< t_atomic_type > & r_item_spec_in )
    {
        return c_unmarshal_list_bridge< Treader, Tself, t_atomic_type, t_handling >( r_reader_in, *this, r_item_spec_in );
    }
    template< typename Twriter >
    c_marshal_list_bridge< Twriter, Tself, t_atomic_type, t_handling > get_marshal_list_bridge( Twriter & r_writer_in, const ct_typed_validation_spec< t_atomic_type > & r_item_spec_in ) const
    {
        return c_marshal_list_bridge< Twriter, Tself, t_atomic_type, t_handling >( r_writer_in, *this, r_item_spec_in );
    }
};

// It's useful to have some additional methods for the empty type
template< class T = c_empty, class Thandler = T, typename Twrapper = ct_non_mixed< T > >
class ct_empty_single : public ct_simple_pod_single< T, Thandler, Twrapper >
{
public:
    ct_empty_single() : ct_simple_pod_single< T, Thandler, Twrapper >() { set(); }  // C100916_1636
    elmx_error set() { return ct_simple_pod_single< T, Thandler, Twrapper >::set( T() ); }
};

template< class T = c_empty, class Thandler = T, typename Twrapper = ct_non_mixed< T > >
class ct_empty_optional : public ct_simple_pod_optional< T, Thandler, Twrapper >
{
public:
    LMX_CC_DEFAULT( ct_empty_optional() )
    elmx_error set() { return ct_simple_pod_optional< T, Thandler, Twrapper >::set( T() ); }
};

template< class T = c_empty, class Thandler = T, typename Twrapper = ct_non_mixed< T >, typename TC = ct_pod_container< Twrapper > >
class ct_empty_multi : public ct_simple_pod_multi< T, Thandler, Twrapper, TC >
{
public:
    typedef ct_simple_pod_multi< T, Thandler, Twrapper, TC > Tbase;

    LMX_CC_DEFAULT( ct_empty_multi() )
    elmx_error set( size_t index_in ) { return Tbase::set( index_in, T() ); }
    void append() { Tbase::append( T() ); }
    void insert( size_t index_in ) { Tbase::insert( index_in, T() ); }
};

// For non-POD simple types such as c_big_int etc., that we prefer to store a pointer to in a std::vector.
// The single and optional variants are handled in the same way as the POD types.
template< typename T, typename Thandling = T, typename Twrapper = ct_non_mixed< T > >
class ct_simple_non_pod_single : public ct_simple_pod_single< T, Thandling, Twrapper >
{};

template< typename T, typename Thandling = T, typename Twrapper = ct_non_mixed< T > >
class ct_simple_non_pod_optional : public ct_simple_pod_optional< T, Thandling, Twrapper >
{};

template< typename T, typename Thandling = T, typename Twrapper = ct_non_mixed< T >, typename TC = ct_non_pod_container< Twrapper > >
class ct_simple_non_pod_multi : public ct_simple_pod_multi< T, Thandling, Twrapper, TC >
{};

template< typename T, typename Thandling = T, typename TC = ct_non_pod_container< T >, typename Twrapper = ct_non_mixed< TC > >
class ct_simple_non_pod_list_single : public ct_simple_pod_list_single< T, Thandling, TC, Twrapper >
{};

template< typename T, typename Thandling = T, typename TC = ct_non_pod_container< T >, typename Twrapper = ct_non_mixed< TC > >
class ct_simple_non_pod_list_optional : public ct_simple_pod_list_optional< T, Thandling, TC, Twrapper >
{};

// For backwards compatibility reasons, we don't differentiate between POD and non-POD types
// -for multi-list.
template< typename T, typename Thandling = T, typename Twrapper = ct_non_mixed< ct_simple_list< T > >, typename TC = ct_non_pod_container< Twrapper > >
class ct_simple_non_pod_list_multi : public ct_simple_pod_list_multi< T, Thandling, Twrapper, TC >
{};

// For mixed variants (both POD and non-POD mixed types treated as non-POD)
template< typename T, typename Thandling = T, typename Twrapper = ct_mixed< T > >
class ct_mixed_simple_pod_single : public ct_simple_non_pod_single< T, Thandling, Twrapper >
{};

template< typename T, typename Thandling = T, typename Twrapper = ct_mixed< T > >
class ct_mixed_simple_pod_optional : public ct_simple_non_pod_optional< T, Thandling, Twrapper >
{};

template< typename T, typename Thandling = T, typename Twrapper = ct_mixed< T >, typename TC = ct_non_pod_container< Twrapper > >
class ct_mixed_simple_pod_multi : public ct_simple_non_pod_multi< T, Thandling, TC, Twrapper >
{};

template< typename T, typename Thandling = T, typename TC = ct_pod_container< T >, typename Twrapper = ct_mixed< TC > >
class ct_mixed_simple_pod_list_single : public ct_simple_pod_list_single< T, Thandling, TC, Twrapper >
{};

template< typename T, typename Thandling = T, typename TC = ct_pod_container< T >, typename Twrapper = ct_mixed< TC > >
class ct_mixed_simple_pod_list_optional : public ct_simple_pod_list_optional< T, Thandling, TC, Twrapper >
{};

template< typename T, typename Thandling = T, typename Twrapper = ct_mixed< ct_simple_list< T > >, typename TC = ct_non_pod_container< Twrapper > >
class ct_mixed_simple_pod_list_multi : public ct_simple_pod_list_multi< T, Thandling, Twrapper, TC >
{};

template< class T = c_empty, typename Thandling = T, typename Twrapper = ct_mixed< T > >
class ct_mixed_empty_single : public ct_empty_single< T, Thandling, Twrapper >
{};

template< class T = c_empty, typename Thandling = T, typename Twrapper = ct_mixed< T > >
class ct_mixed_empty_optional : public ct_empty_optional< T, Thandling, Twrapper >
{};

template< class T = c_empty, typename Thandling = T, typename Twrapper = ct_mixed< T >, typename TC = ct_non_pod_container< Twrapper > >
class ct_mixed_empty_multi : public ct_empty_multi< T, Thandling, Twrapper, TC >
{};

template< typename T, typename Thandling = T, typename Twrapper = ct_mixed< T > >
class ct_mixed_simple_non_pod_single : public ct_simple_non_pod_single< T, Thandling, Twrapper >
{};

template< typename T, typename Thandling = T, typename Twrapper = ct_mixed< T > >
class ct_mixed_simple_non_pod_optional : public ct_simple_non_pod_optional< T, Thandling, Twrapper >
{};

template< typename T, typename Thandling = T, typename Twrapper = ct_mixed< T >, typename TC = ct_non_pod_container< Twrapper > >
class ct_mixed_simple_non_pod_multi : public ct_simple_non_pod_multi< T, Thandling, Twrapper, TC >
{};

template< typename T, typename Thandling = T, typename TC = ct_non_pod_container< T >, typename Twrapper = ct_mixed< TC > >
class ct_mixed_simple_non_pod_list_single : public ct_simple_non_pod_list_single< T, Thandling, TC, Twrapper >
{};

template< typename T, typename Thandling = T, typename TC = ct_non_pod_container< T >, typename Twrapper = ct_mixed< TC > >
class ct_mixed_simple_non_pod_list_optional : public ct_simple_non_pod_list_optional< T, Thandling, TC, Twrapper >
{};

template< typename T, typename Thandling = T, typename Twrapper = ct_mixed< ct_simple_list< T > >, typename TC = ct_non_pod_container< Twrapper > >
class ct_mixed_simple_non_pod_list_multi : public ct_simple_non_pod_list_multi< T, Thandling, Twrapper, TC >
{};

// For complex types
template< typename T >  // C100916_1604
class ct_complex_optional : public ct_grin_or_happy_ptr_deleter< T >  // C110110_2117
{
protected:
    T * p;

public:
    typedef ct_complex_optional< T > Tself;
    typedef ct_grin_or_happy_ptr_deleter< T > Tdeleter;

    ct_complex_optional() : p( 0 ) {}
    ct_complex_optional( const Tself & r_rhs_in ) : p( r_rhs_in.p ? r_rhs_in.p->clone() : 0 ) {}
    Tself & operator = ( const Tself & r_rhs_in )
    {
        Tself t( r_rhs_in ); swap( t );
        return *this;
    }
    ~ct_complex_optional() { Tdeleter::release( p ); }
    void swap( Tself & r_rhs_in ) { std::swap( p, r_rhs_in.p ); }
    void clone( Tself & r_rhs_in ) { assign( p->clone() ); }

    // User interface methods
    const T & get() const { lmx_assert( p ); return *p; }   // For read access
    T & get()                                       // For read/write access
    {
        if( ! p )
            p = new T;
        return *p;
    }
    T & assign( const T & r_v_in ) { return (get() = r_v_in); } // Deep copy
    T & assign( T * p_v_in )                                  // Polymorphic assign
    {
        if( ! p_v_in )
        {
            T *p_old = p;
            p = 0;
            return *p_old;     // Assigning NULL pointer causes object to surrender ownership
        }
        Tdeleter::release( p );
        p = p_v_in;
        return *p_v_in;
    }
    bool is_value_set() const { return p != 0; }    // C100510_1423
    void unset() { Tdeleter::release( p ); p = 0; } // C121019_1950
    bool is_occurs_ok( bool is_required_in ) const { return is_required_in ? (p != 0) : true; }    // See note is_occurs_ok_is_required

    bool operator == ( const Tself & r_rhs_in ) const
    {
        return (p == 0) == (r_rhs_in.p == 0) &&
                ( p == 0 || *(p) == *(r_rhs_in.p) );
    }
    bool operator != ( const Tself & r_rhs_in ) const { return ! (*this == r_rhs_in); }
    bool iseq( const Tself & r_rhs_in ) const
    {
        return (p == 0) == (r_rhs_in.p == 0) && ( p == 0 || p->iseq( r_rhs_in.p ) );
    }
};

template< class T >
class ct_complex_single : public ct_complex_optional< T >   // C100916_1604
{
public:
    typedef ct_complex_single< T > Tself;
    typedef ct_complex_optional< T > Tbase;
    ct_complex_single() { Tbase::p = new T; }   // C100916_1604
    LMX_CC_DEFAULT( ct_complex_single( const Tself & ar_rhs ) )
    LMX_CC_DEFAULT( Tself & operator = ( const Tself & ar_rhs ) )
    ~ct_complex_single() {}

    // Additional user interface methods
    bool is_occurs_ok( bool LMX_UNUSED_PARAM( is_required_in ) ) const { return Tbase::p != 0; }    // See note is_occurs_ok_is_required
};

template< class T >
class ct_complex_multi
{
private:
    ct_clonable_container< T > m_values;

public:
    typedef ct_complex_multi< T > Tself;
    typedef typename ct_clonable_container< T >::Trawcontainer Tcontainer;
    typedef typename ct_clonable_container< T >::Trangebounds Trange;   // C181030_1203
    typedef typename ct_clonable_container< T >::Tconst_rangebounds Tconst_range;

    LMX_CC_DEFAULT( ct_complex_multi() )
    LMX_CC_DEFAULT( ct_complex_multi( const Tself & ar_rhs ) )
    LMX_CC_DEFAULT( Tself & operator = ( const Tself & ar_rhs ) )
    ~ct_complex_multi() {}
    void swap( Tself & r_rhs_in ) { m_values.swap( r_rhs_in.m_values ); }
    void clone( Tself & r_rhs_in ) { m_values.clone( r_rhs_in.m_values ); }

    const T & get( size_t index_in ) const       // For read access
    {
        lmx_assert( index_in < m_values.size() );
        return m_values.at(index_in);
    }
    T & get( size_t index_in )                   // For read/write access
    {
        while( m_values.size() <= index_in )
            m_values.push_back_w_autop( new T );
        return m_values[index_in];
    }
    const T & get() const       // For read access - C110301_1550 - For when multi occurs is restricted to single or optional
    {
        return get( 0 );
    }
    T & get()                // For read/write access - C110301_1550 - For when multi occurs is restricted to single or optional
    {
        return get( 0 );
    }
    void append()                               // For write access
    {
        m_values.push_back_w_autop( new T );
    }
    void append( T * p_v_in )                 // For write access
    {
        m_values.push_back_w_autop( p_v_in );
    }
    void append( typename uniq_ptr< T >::type & aapr_v )  // For write access - C100827_0943
    {
        m_values.push_back( aapr_v );
    }
    T & back()                              // For write access
    {
        return m_values.back();
    }
    void insert( size_t index_in )               // For write access
    {
        m_values.insert_w_autop( index_in, new T );
    }
    void insert( size_t index_in, T * p_v_in )               // For write access
    {
        m_values.insert_w_autop( index_in, p_v_in );
    }
    void insert( size_t index_in, typename uniq_ptr< T >::type & aapr_v )  // For write access - C100827_0943
    {
        m_values.insert( index_in, aapr_v );
    }
    T & assign( size_t index_in, const T & a ) // Deep copy
    {
        while( m_values.size() <= index_in )
            m_values.push_back_w_autop( new T );
        return (m_values[index_in] = a);
    }
    T & assign( size_t index_in, T * p_v_in )      // C110301_1621 - Polymorphic assign
    {
        return m_values.assign( index_in, p_v_in );
    }
    T & assign( const T & a ) // Deep copy - C110301_1550 - For when multi occurs is restricted to single or optional
    {
        return assign( 0, a );
    }
    T & assign( T * p_v_in )      // C110301_1621 - Polymorphic assign - C110301_1550 - For when multi occurs is restricted to single or optional
    {
        return assign( 0, p_v_in );
    }
    void erase( size_t index_in ) { m_values.erase( index_in ); }
    void clear() { m_values.clear(); }
    size_t size() const { return m_values.size(); }
    bool empty() const { return m_values.empty(); }
    bool is_value_set() const { return ! empty(); } // C110301_1550 - For when multi occurs is restricted to single or optional
    void unset() { clear(); }   // C110301_1550 - For when multi occurs is restricted to single or optional

    bool is_occurs_ok( size_t min_occurs_in ) const  // For maxOccurs unbounded
    {
        return m_values.size() >= min_occurs_in;
    }
    bool is_occurs_ok( size_t min_occurs_in, size_t max_occurs_in ) const
    {
        return min_occurs_in <= m_values.size() && m_values.size() <= max_occurs_in;
    }
    bool operator == ( const Tself & r_rhs_in ) const
    {
        return m_values == r_rhs_in.m_values;
    }
    bool operator != ( const Tself & r_rhs_in ) const { return ! (*this == r_rhs_in); }
    bool iseq( const Tself & r_rhs_in ) const
    {
        if( m_values.size() != r_rhs_in.size() )
            return false;
        for( size_t i=0; i<m_values.size(); ++i )
            if( ! (m_values[i].iseq( &r_rhs_in.m_values[i] ) ) )
                return false;
        return true;
    }

    Trange range_bounds() { return m_values.range_bounds(); }   // C181030_1203
    Tconst_range const_range_bounds() const { return m_values.const_range_bounds(); }

    Tcontainer & raw_container() { return m_values.raw_container(); }
    const Tcontainer & raw_container() const { return m_values.raw_container(); }
};

template< typename T >
class ct_typed_validation_spec;

//------------------------------------------------------------------------------
//                   Signed char safe versions of isxxxx functions
//------------------------------------------------------------------------------

inline bool xisalpha( int x ) { return x > 0 && LMXSTDCRT::isalpha( x ); }
inline bool xisspace( int x ) { return x > 0 && LMXSTDCRT::isspace( x ); }
inline bool xisalnum( int x ) { return x > 0 && LMXSTDCRT::isalnum( x ); }
inline bool xisdigit( int x ) { return x > 0 && LMXSTDCRT::isdigit( x ); }
inline bool xisxdigit( int x ) { return x > 0 && LMXSTDCRT::isxdigit( x ); }
inline bool xisprint( int x ) { return x > 0 && LMXSTDCRT::isprint( x ); }

//------------------------------------------------------------------------------
//                         Compiler warnings workarounds
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                         Compiler STL support fixes
//------------------------------------------------------------------------------

inline int stringncompare( const std::string &r_lhs_in, const char *p_rhs_in,
                        size_t first_in, size_t num_in )
{
#if defined( __GNUC__ ) && __GNUC__ == 2
    size_t len = LMXSTDCRT::strlen( p_rhs_in );
    if( num_in > len ) num_in = len;
    return strncmp( r_lhs_in.c_str() + first_in, p_rhs_in, num_in );
#else
    return r_lhs_in.compare( first_in, num_in, p_rhs_in );
#endif
}

inline int stringncompare( const std::string &r_lhs_in, const char *p_rhs_in, size_t rhs_len_in,
                        size_t first_in, size_t num_in )
{
#if defined( __GNUC__ ) && __GNUC__ == 2
    size_t len = LMXSTDCRT::strlen( p_rhs_in );
    if( num_in > len ) num_in = len;
    return strncmp( r_lhs_in.c_str() + first_in, p_rhs_in, num_in );
#else
    return r_lhs_in.compare( first_in, num_in, p_rhs_in, rhs_len_in );
#endif
}

inline int stringcompare( const std::string &r_lhs_in, const char *p_rhs_in, size_t rhs_len_in )
{
#if defined( __GNUC__ ) && __GNUC__ == 2
    return strcmp( r_lhs_in.c_str(), p_rhs_in );
#else
    return r_lhs_in.compare( 0, rhs_len_in, p_rhs_in, rhs_len_in );
#endif
}

} // End of namespace lmx

#if defined( _MSC_VER ) && _MSC_VER >= 1400
#pragma warning (pop)  // Restore VC8 'safe' warnings
#endif

#endif // LMXINTERNALS_H

//------------------------------------------------------------------------------
//                  Copyright (c) 2003-2017 Codalogic Ltd.
//------------------------------------------------------------------------------

