#ifndef LMXPARSE_H
#define LMXPARSE_H

#if defined( _MSC_VER ) && _MSC_VER > 1000
#pragma once
#pragma warning (disable:4786)  // Disable truncated debug info warning
#endif

#if defined( _MSC_VER ) && _MSC_VER >= 1400
#pragma warning (push)
#pragma warning (disable:4996)  // Disable VC8 'safe' warnings
#pragma warning (disable:4100)  // Disable unreferenced formal parameter
#pragma warning (disable:4702)  // Disable unreachable code
#endif

#if defined( __BORLANDC__ )
#pragma warn -8027 // Disable "not expanded inline" warnings
#endif

#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include <cstring>  // v4.2
#include <cstdlib>  // v4.2
#include <iostream>
#include <fstream>  // For the marshal() template function
#include <sstream>  // For the marshal() template function

#include "lmxuser.h"

// To help MS intellisense
#if ! defined( LMX_PDECL )
#define LMX_PDECL
#endif

namespace lmx {

static std::string get_exe_folder_path(std::string path) {
	std::string::size_type pos = path.find_last_of("\\/");
	return path.substr(0, pos);
}

#if LMX_WANT_DLL == 0 && ! defined( LMX_MAKE_DLL ) && (defined( _MSC_VER ) || defined( __BORLANDC__ ))
    // USER: Try to warn user if they have not setup DLL imports correctly.
    // You may remove this section if you are happy with your project's linking.
    namespace NOTE_TO_USER_____
    {
        extern const int set_hash_define_LMX_WANT_DLL_to_1_to_use_DLL_versions_of_LMX_libraries___See_comments_near_LMX_WANT_DLL_in_lmxuser_h_file;
        const int user_note = set_hash_define_LMX_WANT_DLL_to_1_to_use_DLL_versions_of_LMX_libraries___See_comments_near_LMX_WANT_DLL_in_lmxuser_h_file;
    }
#endif

char * xstrdup( const char * );

#if defined( _MSC_VER ) && _MSC_VER < 1400 || defined( __BORLANDC__ )   // C20140127_1218 - Moved from lmxparse.cpp
    inline int xstricmp( const char * p_1_in, const char * p_2_in ) { return stricmp( p_1_in, p_2_in ); }
#elif defined( _MSC_VER ) && _MSC_VER >= 1400
    inline int xstricmp( const char * p_1_in, const char * p_2_in ) { return _stricmp( p_1_in, p_2_in ); }
#else
    int xstricmp( const char * p_1_in, const char * p_2_in );     // stricmp is non-standard
#endif

typedef std::vector< std::string > t_comment_collection;    // For LMX_WANT_COMMENT_AND_PI

//------------------------------------------------------------------------------
//                                 Error classes
//------------------------------------------------------------------------------

class c_get_as_utf8;    // Provides information on line numbers etc.
class c_getter_error_binder;

class LMX_PDECL c_error
{
protected:
    const c_get_as_utf8 *mp_getter;  // See note error_binding in lmxparse.cpp.
    int m_fatals;     // Treat fatals as pure errors also.  i.e. inc errors when fatals inced
    int m_errors;
    int m_warnings;
    int m_notes;      // Things the compiler does not support - could be switched off
    elmx_error_type m_last_error_type;
    elmx_error m_last_error_code;
    int m_last_error_line;

public:
    c_error() : mp_getter(LMXNULL), m_fatals(0), m_errors(0), m_warnings(0), m_notes(0),
                m_last_error_type( EET_NONE ), m_last_error_code( ELMX_OK ), m_last_error_line(-1)
    {}
    // To ensure proper registering and unregistering of the getter, binding must be done
    // using an instance of the c_getter_error_binder class.
    const c_get_as_utf8 * register_getter( c_getter_error_binder * p_binder_in );
    void unregister_getter( c_getter_error_binder * p_binder_in );

    virtual void report_error( elmx_error_type LMX_UNUSED_PARAM( type_in ),
                     elmx_error LMX_UNUSED_PARAM( code_in ),
                     const char * LMX_UNUSED_PARAM( p_file_in ),
                     int LMX_UNUSED_PARAM( line_in ),
                     const char * LMX_UNUSED_PARAM( p_msg_in ) ) {}

    // For no file, set ap_file to LMXNULL.  For no line, set a_line to -1.
    void error( elmx_error_type type_in,
            elmx_error code_in,
            const char * p_file_in,
            int line_in,
            const char * p_msg_in );
    // If p_getter is specified, c_error will ask for file and line information
    void error( elmx_error_type type_in,
            elmx_error code_in,
            const char * p_msg_in );

    bool is_errored() { return (m_errors + m_fatals) ? true : false; }
    elmx_error get_last_error() { return m_last_error_code; }
    elmx_error_type get_last_error_type() { return m_last_error_type; }
    bool get_error_info( elmx_error_type * p_type_out, elmx_error * p_code_out, int * p_line_out );
    bool is_fatal_errored() { return m_fatals != 0; }
    virtual ~c_error() {}
};

//------------------------------------------------------------------------------
//                            c_getter_error_binder
//               Implements binding between c_error and getter.
// By making this the only way a getter can register with the error class we
// ensure that the binding is always done and undone correctly.
// See note error_binding in lmxparse.cpp.
//------------------------------------------------------------------------------

class LMX_PDECL c_getter_error_binder
{
private:
    bool m_is_bound;
    c_error & mr_error;
    const c_get_as_utf8 * mp_new_getter;
    const c_get_as_utf8 * mp_old_getter;

private:
    c_getter_error_binder( const c_getter_error_binder & );     // No copy operations
    c_getter_error_binder & operator =( const c_getter_error_binder & );

public:
    c_getter_error_binder( c_error & r_error_in );
    void bind( const c_get_as_utf8 * p_getter_in );
    const c_get_as_utf8 * get_new_getter() const { return mp_new_getter; }
    const c_get_as_utf8 * get_old_getter() const { return mp_old_getter; }
    ~c_getter_error_binder();
};

enum elxp_attr_elem { EXNT_ATTR, EXNT_ELEM };
enum elxp_xml_known_namespaces {
    EXKN_UNKNOWN, EXKN_NONE, EXKN_XML, EXKN_XS, EXKN_XMLNS, EXKN_XSI, EXKN_CLTYPES,
    EXKN_SOAP_ENV, EXKN_SOAP_ENC, EXKN_DYNAMIC_START = 1000 };

//------------------------------------------------------------------------------
//                      Unicode conversion functions
//------------------------------------------------------------------------------

LMX_PDECL bool utf8_to_unicode_char( tc_utf8_buf c_utf8_in, t_unicode_char * p_unicode_in );
LMX_PDECL char * unicode_char_to_utf8( t_unicode_char c_in, tc_utf8_buf c_utf8_in );
LMX_PDECL bool is_valid_utf8_char_sequence( const char c_utf8_in[] );     // V3.2.4 - Added

//------------------------------------------------------------------------------
//                       Classes for low-level input
//                    Either memory mapped or File mapped
//------------------------------------------------------------------------------

class LMX_PDECL c_read
{
public:
    enum { k_eof = -1 };    // Can't be '\0' because UTF-16 and similar encodings contain 0

    c_read() {}

    virtual bool is_open() const { return true; }
    virtual int  read() = 0;
    virtual const char *get_source_id() const { return LMXNULL; }
    virtual void rewind() = 0;

    virtual ~c_read() {}
};

class c_read_memory : public c_read
{
private:
    std::string m_source_id;  // C100923_1255
    const char *mp_message;
    const char *mp_message_ptr;
    const char *mp_message_end;

    c_read_memory() {}

public:
    LMX_PDECL c_read_memory( const char * p_message_in, size_t message_length_in )
        :
        mp_message( p_message_in ),
        mp_message_ptr( p_message_in ),
        mp_message_end( p_message_in + message_length_in )
    {}
    LMX_PDECL c_read_memory( const char * p_message_in, size_t message_length_in, const std::string & r_source_id_in )    // C100923_1255
        :
        m_source_id( r_source_id_in ),
        mp_message( p_message_in ),
        mp_message_ptr( p_message_in ),
        mp_message_end( p_message_in + message_length_in )
    {}
    LMX_PDECL c_read_memory( const std::string & r_string_in )    // C100930_1617
        :
        mp_message( r_string_in.data() ),
        mp_message_ptr( r_string_in.data() ),
        mp_message_end( r_string_in.data() + r_string_in.size() )
    {}
    LMX_PDECL c_read_memory( const std::string & r_string_in, const std::string & r_source_id_in )  // C100923_1255 C100930_1617
        :
        m_source_id( r_source_id_in ),
        mp_message( r_string_in.data() ),
        mp_message_ptr( r_string_in.data() ),
        mp_message_end( r_string_in.data() + r_string_in.size() )
    {}
    LMX_PDECL ~c_read_memory()
    {}
    LMX_PDECL bool is_open() const
    {
        return true;
    }
    LMX_PDECL int read()
    {
        if( mp_message_ptr >= mp_message_end )
            return c_read::k_eof;

        return static_cast<unsigned char>(*mp_message_ptr++);
    }
    LMX_PDECL void rewind()
    {
        mp_message_ptr = mp_message;
    }
    LMX_PDECL virtual const char * get_source_id() const    // C100923_1255
    {
        return m_source_id.c_str();
    }
    LMX_PDECL void set_source_id( const std::string & r_source_id_in )    // C100923_1255
    {
        m_source_id = r_source_id_in;
    }
};

class c_read_string : public c_read_memory
{
public:
    LMX_PDECL c_read_string( const std::string & r_string_in ) : c_read_memory( r_string_in )
    {}
    LMX_PDECL c_read_string( const std::string & r_string_in, const std::string & r_source_id_in )  // C100923_1255
        : c_read_memory( r_string_in, r_source_id_in )
    {}
};

class c_read_file : public c_read
{
private:
    std::string m_file;
    // We could use ifstream.  However, the functionality provided by ifstream
    // -is way more than we need, and for the bits we want ultimately ends up calling
    // -the same functions as this.  Hence this should compile faster and run
    // -faster.
    LMXSTDCRT::FILE * mh_input;
    bool m_is_file_to_be_closed_on_delete;
    char mc_buf[512];
    size_t m_n_chars;
    size_t m_char_index;

private:
    c_read_file() {}

public:
    LMX_PDECL c_read_file( const char * p_file_in )
        :
        m_file( p_file_in ),
        m_is_file_to_be_closed_on_delete( true ),
        m_n_chars( 0 ),
        m_char_index( 0 )
    {
        mh_input = LMXSTDCRT::fopen( p_file_in, "rb" );
    }
#if LMX_WANT_WIDE_FILE_NAMES == 1
    LMX_PDECL c_read_file( const wchar_t * p_file_in );
#endif
    LMX_PDECL c_read_file( LMXSTDCRT::FILE *h_file_in )
        :
        m_file( "" ),
        m_is_file_to_be_closed_on_delete( true ),
        m_n_chars( 0 ),
        m_char_index( 0 )
    {
        mh_input = h_file_in;
    }
    LMX_PDECL void leave_file_open_on_delete( bool leave_open_in = true )
    {
        m_is_file_to_be_closed_on_delete = ! leave_open_in;
    }
    LMX_PDECL bool is_open() const
    {
        return (mh_input)?true: false;
    }
    LMX_PDECL int read()
    {
        if( m_char_index >= m_n_chars ) // v3.1
        {
            if( ! mh_input )
                return c_read::k_eof;
            m_n_chars = LMXSTDCRT::fread( mc_buf, 1, sizeof(mc_buf), mh_input );
            if( m_n_chars == 0 )
                return c_read::k_eof;
            m_char_index = 0;
        }
        return static_cast<unsigned char>(mc_buf[m_char_index++]);
    }
    LMX_PDECL void rewind()
    {
        LMXSTDCRT::fseek( mh_input, 0, SEEK_SET );
        m_char_index = m_n_chars = 0;       // V3.8.1 - fixed proper rewind
    }
    LMX_PDECL const char * get_source_id() const
    {
        return m_file.c_str();
    }
    LMX_PDECL void close()
    {
        if( mh_input )
            LMXSTDCRT::fclose( mh_input );
        mh_input = LMXNULL;
    }
    LMX_PDECL ~c_read_file()
    {
        if( m_is_file_to_be_closed_on_delete && mh_input )
            LMXSTDCRT::fclose( mh_input );
    }
};

//------------------------------------------------------------------------------
//                           c_unget_buffer
//           Allows multi character push back (min 2 required)
//------------------------------------------------------------------------------

class LMX_PDECL c_unget_buffer
{
private:
    enum { size = 3 };
    int mc_buffer[size];
    size_t m_free_index; // index of next empty slot.  == 0 if empty

public:
    c_unget_buffer() : m_free_index( 0 ) {}
    void clear() { m_free_index = 0; }
    inline bool empty() const { return m_free_index == 0; }
    inline int get()
    {
        lmx_assert( m_free_index > 0 );
        return mc_buffer[--m_free_index];
    }
    inline void unget( int c_in )
    {
        lmx_assert( m_free_index < size );
        mc_buffer[m_free_index++] = c_in;
    }
};

//------------------------------------------------------------------------------
//                           class c_entity_name_reader
//              A base utility class that allows the name of entities to be
//                read from various input sources such as c_get_for_xml,
//                   other entities and the stored attribute data.
//------------------------------------------------------------------------------

class c_entity_name_reader
{
private:
    bool m_is_markup_possible_in_entity;    // set to true if contents of entity will be parsed for markup - C101112_2052

public:
    c_entity_name_reader() : m_is_markup_possible_in_entity( false ) {}
    virtual ~c_entity_name_reader() {}  // Needs virtual destructor in base

    virtual int get() = 0;      // Returns c_read::k_eof for end of text
    virtual void unget( int c_in ) = 0;

    void is_markup_possible_in_entity( bool is_markup_possible_in_entity_in ) { m_is_markup_possible_in_entity = is_markup_possible_in_entity_in; }
    bool is_markup_possible_in_entity() const { return m_is_markup_possible_in_entity; }
};

//------------------------------------------------------------------------------
//                 c_entity_name_reader specializations
// These classes are specializations of c_entity_name_reader and allow us to
// use a common function to read the name of entities from different input
// sources such as c_get_for_xml, strings and the stored attribute data.
//------------------------------------------------------------------------------

class c_get_for_xml;
class c_attribute_data;

class c_entity_name_reader_for_c_get : public c_entity_name_reader
{
private:
    c_get_for_xml & mr_get;

private:
    c_entity_name_reader_for_c_get( const c_entity_name_reader_for_c_get & );       // No copy operations
    c_entity_name_reader_for_c_get & operator =( const c_entity_name_reader_for_c_get & );

public:
    c_entity_name_reader_for_c_get( c_get_for_xml & r_get_in );

    virtual int get();   // Returns c_read::k_eof for end of text
    virtual void unget( int c_in );
};

class c_entity_name_reader_for_attribute_data : public c_entity_name_reader
{
private:
    c_attribute_data & mr_attribute_data;

private:
    c_entity_name_reader_for_attribute_data( const c_entity_name_reader_for_attribute_data & );     // No copy operations
    c_entity_name_reader_for_attribute_data & operator =( const c_entity_name_reader_for_attribute_data & );

public:
    c_entity_name_reader_for_attribute_data( c_attribute_data & r_attribute_data_in );

    virtual int get();   // Returns c_read::k_eof for end of text
    virtual void unget( int c_in );
};

class c_entity_name_reader_for_entity : public c_entity_name_reader
{
private:
    const char * mp_entity_value_start_value;
    const char ** mpp_entity_value;   // We want to modify the value of the pointer

public:
    c_entity_name_reader_for_entity( const char ** pp_entity_value_in );

    virtual int get();   // Returns c_read::k_eof for end of text
    virtual void unget( int c_in );
};

class c_entity_name_reader_for_c_str : public c_entity_name_reader
{
private:
    const char * mp_string;
    size_t m_index;

public:
    c_entity_name_reader_for_c_str( const char * p_string_in, size_t index_in );

    virtual int get();   // Returns c_read::k_eof for end of text
    virtual void unget( int c_in );
    size_t get_index() const { return m_index; }  // To get back index in base string so that parsing can continue after the entity has been expanded
};

//------------------------------------------------------------------------------
//                   Data Repository (Namespaces and enitites)
//------------------------------------------------------------------------------

template <class T>
struct c_rep_ops
{
private:
    T m_data;

public:
    typedef T Tconst;
    c_rep_ops( Tconst a ) : m_data( a ) {}
    ~c_rep_ops() {}
    T get() { return m_data; }
    static bool eq( Tconst a, Tconst b ) { return a == b; }
    static void release( T LMX_UNUSED_PARAM( a ) ) {}
};

template<> struct c_rep_ops< char * >
{
private:
    uniq_ptr< char >::type mp_data;

public:
    typedef const char * Tconst;
    c_rep_ops( Tconst a ) : mp_data( xstrdup( a ) ) {}
    char * get() { return mp_data.get(); }
    static bool eq( Tconst a, Tconst b ) { return LMXSTDCRT::strcmp( a, b ) == 0; }
    static void release( char * a ) { delete a; }
    ~c_rep_ops() { mp_data.release(); }  // This object only exists while adding the data to the repository
};

template<> struct c_rep_ops< const char * >
{
private:
    uniq_ptr< char >::type mp_data;   // Must be < char > (not < const char >) for VC6's benefit

public:
    typedef const char * Tconst;
    c_rep_ops( Tconst a ) : mp_data( xstrdup( a ) ) {}
    const char * get() { return mp_data.get(); }
    static bool eq( Tconst a, Tconst b ) { return LMXSTDCRT::strcmp( a, b ) == 0; }
    static void release( const char * a ) { delete const_cast<char *>( a ); }   // The cast is for VC6's benefit!
    ~c_rep_ops() { mp_data.release(); }  // This object only exists while adding the data to the repository
};

// This repository is expected to only have a few members, hence is likely to
// -be faster than a std::map and generate less code.
template <class T, class Tops = c_rep_ops<T> >
class c_repository
{
protected:
    typedef std::pair< char *, T > t_repository_item;
    typedef std::vector< t_repository_item > t_repository;
    t_repository m_repository;

public:
    typedef typename t_repository::const_iterator const_iterator;   // v4.1.1
    void set( const char * p_key_in, typename Tops::Tconst value_in )
    {
        // Scan database to see if key already exists
        typename t_repository::iterator i_db( m_repository.begin() );
        typename t_repository::iterator i_db_end( m_repository.end() );

        while( i_db != i_db_end )
        {
            if( LMXSTDCRT::strcmp( i_db->first, p_key_in ) == 0 )
            {
                Tops::release( i_db->second );
                Tops ops( value_in );
                i_db->second = ops.get();
                return;
            }
            ++i_db;
        }

        // If key was not found, then insert value at end
        uniq_ptr< char >::type key_copy( xstrdup( p_key_in ) );
        Tops ops( value_in );
        m_repository.push_back( t_repository_item( key_copy.get(), ops.get() ) );
        key_copy.release();
    }
    bool get( const char * p_key_in, T * p_value_out ) const
    {
        typename t_repository::const_iterator i_db( m_repository.begin() );
        typename t_repository::const_iterator i_db_end( m_repository.end() );
        while( i_db != i_db_end )
        {
            if( LMXSTDCRT::strcmp( i_db->first, p_key_in ) == 0 )
            {
                *p_value_out = i_db->second;
                return true;
            }
            ++i_db;
        }
        return false;
    }
    const char * reverse_lookup( typename Tops::Tconst r_value_in ) const
    {
        typename t_repository::const_iterator i_db( m_repository.begin() );
        typename t_repository::const_iterator i_db_end( m_repository.end() );
        while( i_db != i_db_end )
        {
            if( Tops::eq( i_db->second, r_value_in ) )
                return i_db->first;
            ++i_db;
        }
        return LMXNULL;
    }
    bool is_defined( const char * p_key_in ) const
    {
        typename t_repository::const_iterator i_db( m_repository.begin() );
        typename t_repository::const_iterator i_db_end( m_repository.end() );
        while( i_db != i_db_end )
        {
            if( LMXSTDCRT::strcmp( i_db->first, p_key_in ) == 0 )
                return true;
            ++i_db;
        }
        return false;
    }
    typename t_repository::const_iterator begin() const { return m_repository.begin(); }  // v4.1.1
    typename t_repository::const_iterator end() const { return m_repository.end(); }  // v4.1.1
    const char * get_key( typename t_repository::const_iterator & ai ) const { return ai->first; }  // v4.1.1
    const T & get_value( typename t_repository::const_iterator & ai ) const { return ai->second; }  // v4.1.1
    virtual void clear()
    {
        typename t_repository::iterator i_db( m_repository.begin() );
        typename t_repository::iterator i_db_end( m_repository.end() );
        while( i_db != i_db_end )
        {
            delete i_db->first;
            Tops::release( i_db->second );
            ++i_db;
        }
        m_repository.resize( 0 );     // Hold on to any memory we've already allocated
    }
    virtual ~c_repository() { clear(); }
};

//------------------------------------------------------------------------------
//                            class c_get_as_utf8
//             Class for reading 'any' input and converting to UTF-8
//------------------------------------------------------------------------------

class c_external_transcoder;
class c_external_transcoder_factory;

class c_get_as_utf8
{
public:
    enum e_encoding_mode {
                EXR_INIT, EXR_ISO8859_1, EXR_UTF8,
                EXR_UTF16_BE, EXR_UTF16_LE,
                EXR_UCS_2_BE, EXR_UCS_2_LE,
                EXR_EXTERNAL };

private:
    c_read & mr_reader;
    c_error & mr_error;
    c_getter_error_binder m_getter_error_binder;
    e_encoding_mode m_encoding_mode;
    static const c_external_transcoder_factory * mp_default_external_transcoder_factory;    // C20140124_1118
    const c_external_transcoder_factory * mp_external_transcoder_factory;               // C20140124_1118
    c_external_transcoder * mp_external_transcoder;                                     // C20140124_1118
    char m_last_get;                        // Used for end of line fix-up
    c_unget_buffer m_unget_buffer;
    tc_utf8_buf mc_utf8_expansion_buffer;   // Filled when ISO or UTF16 code is encountered
    const char * mp_utf8_expansion;         // LMXNULL if no utf8 expansion
    int m_line_num;

private:
    c_get_as_utf8( const c_get_as_utf8 & );     // No copy operations
    c_get_as_utf8 & operator =( const c_get_as_utf8 & );

    void p_init();
    int p_read() { return mr_reader.read(); }
    void p_read_single_utf8_compound_char_sequence( int first_c_in, char c_utf8_in[] );
    int p_read_utf8_compound_char_sequence( int first_c_in );
    int p_read_local_transcoded();      // C20140124_1118
    int p_read_external_transcoded();   // C20140124_1118
    int p_get();

    int p_resolve_initial_character_encoding( int initial_c_in );
    int p_map_unicode_to_utf8( t_unicode_char c_in );
    t_unicode_char p_convert_utf16_bytes( int b1, int b2 );

public:
    LMX_PDECL c_get_as_utf8( c_read & r_reader_in, c_error & r_error_in );
    LMX_PDECL virtual ~c_get_as_utf8();

    LMX_PDECL static void set_default_external_transcoder_factory(      // C20140124_1118
                                const c_external_transcoder_factory & r_default_external_transcoder_factory_in );
    LMX_PDECL static void clear_default_external_transcoder_factory();  // C20140124_1118
    LMX_PDECL void set_external_transcoder_factory(                     // C20140124_1118
                                const c_external_transcoder_factory & r_default_external_transcoder_factory_in );
    LMX_PDECL void clear_external_transcoder_factory();                 // C20140124_1118

    LMX_PDECL void rewind();

    LMX_PDECL bool set_encoding( const char * p_encoding_in );

    LMX_PDECL int get();
    LMX_PDECL void unget( int returned_in ) { m_unget_buffer.unget( returned_in ); }

    LMX_PDECL const char *get_source_id() const { return mr_reader.get_source_id(); }
    LMX_PDECL int get_line_num() const { return m_line_num; }

    // Error reporting is delegated to this class because it knows about line numbers, file names etc.
    LMX_PDECL c_error & get_error_handler() const { return mr_error; }
    LMX_PDECL void error( elmx_error_type type_in, elmx_error code_in, const char * p_format_in );
    LMX_PDECL void error( elmx_error_type type_in, elmx_error code_in, const std::string & r_s_in );
};

//------------------------------------------------------------------------------
//                           c_external_transcoder
//                Interface for external character transcoder
//------------------------------------------------------------------------------

class c_external_transcoder // C20140124_1118
{
protected:
    c_read & mr_reader;
    c_error & mr_error;

public:
    LMX_PDECL c_external_transcoder(
                    c_read & r_reader_in,
                    c_error & r_error_in )
        :
        mr_reader( r_reader_in ),
        mr_error( r_error_in )
    {}
    LMX_PDECL virtual ~c_external_transcoder() {}

    struct s_result
    {
        int c;
        const char * mp_utf8_expansion;

        s_result() : mp_utf8_expansion( LMXNULL ) {}
        s_result( int c_in ) : c( c_in ), mp_utf8_expansion( LMXNULL ) {}
        s_result( int c_in, const char * mp_utf8_expansion_in ) : c( c_in ), mp_utf8_expansion( mp_utf8_expansion_in ) {}
    };

    LMX_PDECL virtual s_result read() = 0;

private:
    c_external_transcoder & operator = ( const c_external_transcoder & );
};

//------------------------------------------------------------------------------
//                        c_external_transcoder_factory
//           Interface for selecting external character transcoders
//------------------------------------------------------------------------------

class c_external_transcoder_factory // C20140124_1118
{
public:
    // Return NULL if no suitable transcoder found
    LMX_PDECL virtual c_external_transcoder * get_transcoder(
                        c_read & r_reader_in,
                        c_error & r_error_in,
                        c_get_as_utf8::e_encoding_mode initial_encoding_mode_in,
                        const char * p_encoding_name_in ) const = 0;

    LMX_PDECL virtual ~c_external_transcoder_factory() {}
};

//------------------------------------------------------------------------------
//                            class c_get_with_entities
//                    Adds the ability to read input from entities
//                            defined in an internal DTD.
//                         (c_dtd_active_entity is a helper)
//------------------------------------------------------------------------------

struct c_dtd_active_entity
{
    const char * p_current;
    const char * p_base;
    c_unget_buffer m_unget_buffer;    // See note unget_buffer_in_active_entity

    c_dtd_active_entity( const char * p_entity_in )
        :
        p_current( p_entity_in ),
        p_base( p_entity_in )
    {}
    int get()
    {
        if( ! m_unget_buffer.empty() )
            return m_unget_buffer.get();
        lmx_assert( *p_current != '\0' );
        return *p_current++;
    }
    void unget( int c_in )
    {
        m_unget_buffer.unget( c_in );
    }
    bool empty() const
    {
        return m_unget_buffer.empty() && *p_current == '\0';
    }
};

class c_get_with_entities : public c_get_as_utf8
{
private:
    bool m_is_reading_xsd;    // White space handling of characters from entities is affected by this. // C101112_1316
    std::vector< c_dtd_active_entity > m_entity_input_stack;
    c_repository< const char * > m_general_entities;      // Of the form &entity;
    c_repository< const char * > parameter_entities;    // Of the form %entity; - Only valid in DTD but get() operation needs to be able to return contents
    std::string m_entity_tag;
    elmx_whitespace m_ws_mode;    // C101112_1316
    bool m_ws_proc_got_white;     // C101112_1316

private:
    c_get_with_entities( const c_get_with_entities & );     // No copy operations
    c_get_with_entities & operator =( const c_get_with_entities & );

    bool p_append_entity( const char * p_entity_value_in, std::string * p_value_out );     // C101112_1316

public:
    LMX_PDECL c_get_with_entities( c_read & r_reader_in, c_error & r_error_in, bool is_reading_xsd_in = true );
    LMX_PDECL virtual ~c_get_with_entities();

    LMX_PDECL int get();
    LMX_PDECL void unget( int returned_in );

    LMX_PDECL void rewind()
    {
        m_general_entities.clear();
        parameter_entities.clear();
        c_get_as_utf8::rewind();
    }

    LMX_PDECL void add_general_entity( const char * p_entity_tag_in, const char * p_value_in )
    {
        m_general_entities.set( p_entity_tag_in, p_value_in );
    }
    LMX_PDECL bool get_general_entity( const char * p_entity_tag_in, const char ** pp_entity_value_out_in ) const
    {
        return m_general_entities.get( p_entity_tag_in, pp_entity_value_out_in );
    }
    LMX_PDECL void add_parameter_entity( const char * p_entity_tag_in, const char * p_value_in )
    {
        parameter_entities.set( p_entity_tag_in, p_value_in );
    }
    LMX_PDECL bool get_parameter_entity( const char * p_entity_tag_in, const char ** pp_entity_value_out_in ) const
    {
        return parameter_entities.get( p_entity_tag_in, pp_entity_value_out_in );
    }
    LMX_PDECL bool set_input_entity_value( const char * p_input_text_in );

    LMX_PDECL size_t get_entity_stack_depth() const { return m_entity_input_stack.size(); }

    // C101112_1316
    LMX_PDECL bool select_general_entity( c_entity_name_reader & r_entity_tag_reader_in, std::string * p_value_out );
    LMX_PDECL bool insert_character_entity( c_entity_name_reader & r_entity_tag_reader_in, std::string * p_value_out );

    // C101112_1316
    LMX_PDECL void add_char_wsp( std::string * p_value_out, int c_in, bool is_from_char_entity_in=false );    // Add char with white space processing
    LMX_PDECL void set_ws_mode( elmx_whitespace ws_mode_in );
    LMX_PDECL void reset_ws_mode() { set_ws_mode( m_ws_mode ); }
};

//------------------------------------------------------------------------------
//                              class c_get_for_xml
//             Class for getting characters specific to XML and DTDs.
//               Mainly this adds white space handling and utility
//                     functions common to parsing XML and DTDs.
//------------------------------------------------------------------------------

class c_get_for_xml : public c_get_with_entities
{
private:
    t_comment_collection m_comment_collection;    // For LMX_WANT_COMMENT_AND_PI
    bool m_has_pi;                                // For LMX_WANT_COMMENT_AND_PI

private:
    c_get_for_xml( const c_get_for_xml & );     // No copy operations
    c_get_for_xml & operator =( const c_get_for_xml & );

    void p_init();

public:
    LMX_PDECL c_get_for_xml( c_read & r_reader_in, c_error & r_error_in, bool is_reading_xsd_in = true );
    LMX_PDECL virtual ~c_get_for_xml();

    LMX_PDECL int get_non_ws()  // Get next non-whitespace character
    {
        int c;
        while( (c = get()) <= ' ' && xisspace( c ) )    // Various line ends have been converted to 0x0a so all ws <= ASCII space.
            {}
        return c;
    }

    LMX_PDECL void rewind();

    LMX_PDECL bool read_specified_string_from_input( const char * p_input_string_to_be_read_in );
    LMX_PDECL bool get_simple_quoted_value( std::string * p_value_out );
    LMX_PDECL bool read_xml_decl();
    LMX_PDECL bool skip_comment();
    LMX_PDECL bool skip_PI();
    LMX_PDECL bool move_to_start_tag_end( bool * p_end_of_element_in = LMXNULL );

    LMX_PDECL t_comment_collection & get_comment_collection() { return m_comment_collection; }    // For LMX_WANT_COMMENT_AND_PI
    LMX_PDECL bool get_has_pi() { return m_has_pi; }  // For LMX_WANT_COMMENT_AND_PI
};

//------------------------------------------------------------------------------
//                              class c_dtd_util
//             Utilities common to reading internal and external DTDs.
//------------------------------------------------------------------------------

class c_dtd_util
{
private:
    c_get_for_xml & mr_get;

private:
    c_dtd_util( const c_dtd_util & );       // No copy operations
    c_dtd_util & operator =( const c_dtd_util & );

public:
    LMX_PDECL c_dtd_util( c_get_for_xml & r_get_in );

    LMX_PDECL c_get_for_xml & m_getter() { return mr_get; }
    LMX_PDECL int get() { return mr_get.get(); }
    LMX_PDECL void unget( int returned_in ) { mr_get.unget( returned_in ); }

    LMX_PDECL bool get_entity_value( std::string * p_value_out );
    LMX_PDECL bool get_quoted_entity_value( std::string * p_value_out );
    LMX_PDECL bool select_parameter_entity();
};

//------------------------------------------------------------------------------
//                              class c_internal_dtd
//                Extracts user defined entities from an internal DTD.
//                  Definitions of elements, attributes and all other
//                             such items are ignored.
//------------------------------------------------------------------------------

class c_internal_dtd
{
private:
    c_dtd_util m_dtd_util;

private:
    c_internal_dtd( const c_internal_dtd & );       // No copy operations
    c_internal_dtd & operator =( const c_internal_dtd & );

    void p_get_markupdecl_name( std::string * p_name_out ); // Different to external DTD form (does not read external references)
    bool p_read_dtd_entity();
    bool p_skip_to_end_of_markupdecl();

public:
    LMX_PDECL c_internal_dtd( c_get_for_xml & r_get_in );

    LMX_PDECL bool read_doctype_decl();
    LMX_PDECL void reset();
};

//------------------------------------------------------------------------------
//                          class c_enumerated_namespaces
//           Allows namespace names to be mapped to integer enumerations.
//             This allows namespaces to be more efficiently handled.
//------------------------------------------------------------------------------

class c_enumerated_namespaces
{
private:
    int                         m_next_enumeration;
    typedef c_repository<int>   t_namespaces;
    t_namespaces                m_namespaces;

public:
    LMX_PDECL c_enumerated_namespaces();
    LMX_PDECL ~c_enumerated_namespaces() {}

    LMX_PDECL void clear();
    LMX_PDECL int find_namespace( const std::string & r_namespace_uri_in ) const;
    LMX_PDECL const char * find_namespace_from_id( int namespace_id_in ) const;
    LMX_PDECL int add_namespace( const char * p_namespace_uri_in );
    LMX_PDECL void add_namespace( const char * p_namespace_uri_in, int value_in );
};

//------------------------------------------------------------------------------
//                              class c_xml_local
//              Stores local state associated with the XML parsing.
//          One thing it does is set up a local hierarchy for namespaces.
//------------------------------------------------------------------------------

class c_xml;
class c_xml_local
{
private:
    const c_xml_local           * mp_previous;
    c_xml_local                 * mp_active;      // Pointer to member that contains ns data
    typedef c_repository<int>   t_namespaces;    // Contains namespace prefix to namespace mapping.  See note Active_prev in lmxparse.h
    t_namespaces                m_namespaces;

public:
    LMX_PDECL explicit c_xml_local() : mp_previous( LMXNULL ), mp_active( LMXNULL ) {}
    LMX_PDECL c_xml_local( const c_xml_local & r_previous_in )
        : mp_previous( &r_previous_in ), mp_active( r_previous_in.mp_active )
    {}
    LMX_PDECL ~c_xml_local() {}

    LMX_PDECL int find_namespace_id( const std::string & r_namespace_prefix_in ) const;
    LMX_PDECL const char *find_namespace_prefix( int namespace_id_in ) const;
    LMX_PDECL const char *find_first_namespace_prefix( int namespace_id_in ) const;
    LMX_PDECL void add_namespace( const char *r_namespace_prefix_in, int namespace_id_in );
    LMX_PDECL void extract_namespace_context( c_namespace_context * p_namespace_context_in, const c_xml & r_xml_in ) const;
    LMX_PDECL void clear() { m_namespaces.clear(); }
};

//------------------------------------------------------------------------------
//                              class c_attribute_data
//              Stores parsed attribute data.  Attributes data potentially
//           has to be parsed multiple time (to extract namespace information,
//          xsi:type, and finally the actual attribute data).  This store
//                            makes this more efficient.
//
// Note: attribute_data_note - The attribute_data structure becomes a
//          concatenation of sets of information describing the attribute data
//          for an element.  The first part of the set is the \0 terminated
//          attribute name.  The second part of the set is either ' or "
//          depending on the type of quotes around the attribute value.
//          (Storing this allows us to more accurately re-constitute the
//          attribute data when xs:any data is being retrieved.)  The last value
//          is the \0 terminated attribute value.  Entities are NOT expanded
//          when stored in this structure.  (This is again so we can
//          re-constitute xs:any data.)  If the attribute name parameter is
//          empty (i.e. just has \0), this marks the end of the data store.
//          An example of the stored data is therefore:
//
//                  name1\0"value1\0name2\0'value2\0\0
//
//          For convenience, each '\0' terminated bit is called a segment.
//------------------------------------------------------------------------------

class c_attribute_data
{
private:
    std::vector<char> m_data;         // See note attribute_data_note.
    size_t m_index;

public:
    c_attribute_data() : m_index(0)
            { m_data.reserve( 150 ); }
    ~c_attribute_data() {}

    inline void clear()
            { m_data.resize(0); m_index = 0; }      // Avoid deleting memory we have already allocated as we can reuse it
    inline bool empty() const
            { return m_data.empty(); }
    inline bool is_eof() const          // v4.2 - gcc_433_brackets
            { return m_data.empty() || (m_data[m_index] == '\0' && m_index + 1 >= m_data.size()); }
    inline void rewind()
            { m_index = 0; }
    inline void put( char c_in )
            { m_data.push_back( c_in ); }
    inline void append( const char * p_begin_in, const char * p_end_in )  // ap_end points to the char after the data
            { m_data.insert( m_data.end(), p_begin_in, p_end_in ); }
    inline void append( const std::string & r_string_in )
            { m_data.insert( m_data.end(), r_string_in.begin(), r_string_in.end() ); }
    inline void mark_segment_end()
            { put( '\0' ); }
    inline int peek() const
            { if( m_data[m_index] == '\0' ) return c_read::k_eof; return static_cast<unsigned char>( m_data[m_index] ); }
    inline int get()
            { if( m_data[m_index] == '\0' ) return c_read::k_eof; return static_cast<unsigned char>( m_data[m_index++] ); }
    inline void next()
            { if( m_data[m_index] != '\0' ) ++m_index; }
    inline void unget( int c_in )
            { lmx_assert( m_index > 0 ); if( c_in != c_read::k_eof && m_index > 0 ) { --m_index; lmx_assert( m_data[m_index] == c_in ); } }
    inline void next_segment()
            { while( m_data[m_index] != '\0' ) ++m_index; if( m_index + 1 < m_data.size() ) ++m_index; }
    inline size_t size() const
            { return m_data.size(); }
    // N.B. The pointer returned by this function may become invalid if extra data is added to the store
    inline const char *get_str() const
            { return &m_data[m_index]; }     // String at current segment
    inline const char *get_str( size_t index_in ) const
            { return &m_data[index_in]; }   // String at specified segment
};

//------------------------------------------------------------------------------
//                               class c_xml
//                     This class understands XML things
//                     such as namespaces but not Schema.
//------------------------------------------------------------------------------

class c_xml
{
private:
    c_get_for_xml m_getter;   // c_get_for_xml getter MUST be declared before c_internal_dtd dtd
    std::string m_entity_tag;
    c_internal_dtd m_dtd;
    c_enumerated_namespaces m_enumerated_namespaces;
    bool m_dtd_read;
    bool m_element_read;
    mutable c_attribute_data attribute_data;    // To allow operation of get_xsi_type().
    c_attribute_data m_namespace_attribute_data;  // Stored so that we can re-constitute xs:any if required
    mutable std::string m_prefix_for_get_ns_id;

private:
    bool p_capture_attribute_value( c_attribute_data * p_attribute_data_in, size_t * p_initial_index_in = LMXNULL );
    bool p_capture_attributes( c_xml_local * p_local_control_in = LMXNULL );
    bool p_read_cdata( std::string * p_value_out );
    bool p_get_xml_text( std::string * p_text_in );    // Read text part of mixed content
    bool p_handle_lt_qmark();       // Have read "<?"
    bool p_handle_lt_pling( std::string * p_text_in, elxp_xml_event * p_returnable_event_in );        // Have read "<!"

    // For xs:any handling
    enum e_tag_type { ETM_START, ETM_BODY, ETM_END, ETM_ERROR };

    void p_add_any_attribute_data( std::string * p_value_out, c_attribute_data & r_attribute_data_in );
    bool p_add_any_attributes( std::string * p_value_out );
    e_tag_type p_get_any_tag_data( std::string * p_value_out );
    bool p_get_any_element( const std::string & r_name_in, std::string * p_value_out );
    bool p_select_entity( c_entity_name_reader & r_tag_reader_in, std::string * p_value_out );

public:
    LMX_PDECL static const std::string k_non_namespace; // Text representation of EXKN_NONE
    LMX_PDECL static const std::string k_namespace_not_found;

    LMX_PDECL c_xml( c_read & r_reader_in, c_error & r_error_in, bool is_reading_xsd_in = true );
    LMX_PDECL ~c_xml() {}

    LMX_PDECL static void set_default_external_transcoder_factory(      // C20140124_1118
                                const c_external_transcoder_factory & r_default_external_transcoder_factory_in )
    {
        c_get_as_utf8::set_default_external_transcoder_factory( r_default_external_transcoder_factory_in );
    }
    LMX_PDECL static void clear_default_external_transcoder_factory()   // C20140124_1118
    {
        c_get_as_utf8::clear_default_external_transcoder_factory( );
    }
    LMX_PDECL void set_external_transcoder_factory(                     // C20140124_1118
                                const c_external_transcoder_factory & r_external_transcoder_factory_in )
    {
        m_getter.set_external_transcoder_factory( r_external_transcoder_factory_in );
    }
    LMX_PDECL void clear_external_transcoder_factory()                  // C20140124_1118
    {
        m_getter.clear_external_transcoder_factory();
    }

    LMX_PDECL void rewind();
    LMX_PDECL const char *get_source_id() const { return m_getter.get_source_id(); }
    LMX_PDECL int get_line_num() const { return m_getter.get_line_num(); }

    LMX_PDECL void add_general_entity( const std::string & r_name_in, const std::string & r_value_in );
    LMX_PDECL void add_parameter_entity( const std::string & r_name_in, const std::string & r_value_in );

    LMX_PDECL void add_namespace( const char * p_namespace_uri_in, int value_in );
    LMX_PDECL int add_namespace( const char * p_namespace_uri_in );
    LMX_PDECL int find_enumerated_namespace( const std::string & r_namespace_in ) const;
    LMX_PDECL const char * find_namespace_from_id( int namespace_id_in ) const;

    LMX_PDECL static std::string & get_local_name( const std::string & r_name_in, std::string * p_local_name_out_in );
    LMX_PDECL static std::string & get_namespace_prefix( const std::string & r_name_in, std::string * p_namespace_prefix_out_in );
    LMX_PDECL int get_ns_id( const c_xml_local * p_xml_local_in, const std::string & r_name_in ) const;
    LMX_PDECL bool is_xsi_attribute( const c_xml_local * p_xml_local_in, const std::string & r_name_in ) const;

    LMX_PDECL elxp_xml_event get_element_name( c_xml_local * p_local_control_in, std::string * p_name_out );
                                // Reads an element name
                                // - also takes care of CDATA
    LMX_PDECL bool get_element_value( std::string * p_value_out,
                                    elmx_whitespace ws_mode_in = EXWS_PRESERVE,
                                    bool expand_entities_in = true );
    LMX_PDECL bool is_element_end_tag( const std::string & r_name_in );
    LMX_PDECL bool get_any_element( const std::string & r_name_in, std::string * p_value_out,
                                    elmx_whitespace ws_mode_in = EXWS_PRESERVE,
                                    bool expand_entities_in = false );
    LMX_PDECL bool get_nested_element_value( std::string * p_value_out, std::string * p_end_tag_name_out,
                                    elmx_whitespace ws_mode_in = EXWS_PRESERVE,
                                    bool expand_entities_in = false );

    LMX_PDECL elxp_xml_event get_attribute_name( std::string * p_name_out, bool in_xml_decl_in = false );
    LMX_PDECL bool get_attribute_value( std::string * p_value_out,
                                    elmx_whitespace ws_mode_in = EXWS_PRESERVE );
                                // Reads an attribute and does suitable expansion
                                // - Skip white space then look for " or '
                                // - Also does attribute normalisation
    LMX_PDECL bool skip_attribute_value();
    LMX_PDECL bool get_namespace_attribute_name( std::string * p_name_out );    // Returns true if name present - V3.8
    LMX_PDECL bool get_namespace_attribute_value( std::string * p_value_out );  // V3.8
    LMX_PDECL bool skip_namespace_attribute_value();                        // V3.8

    LMX_PDECL bool skip_start_tag( bool * p_is_end_of_element_in = LMXNULL )
        { return m_getter.move_to_start_tag_end( p_is_end_of_element_in ); }

    LMX_PDECL bool get_xsi_type( const c_xml_local * p_xml_local_in,
                                int * p_type_ns_id_in, std::string * p_type_local_name_in );  // See get_xsi_type_is_non_const
    LMX_PDECL bool get_named_attribute(
                                const c_xml_local * p_xml_local_in,
                                int ns_id_in,
                                const std::string & r_local_name_in,
                                std::string * p_value_out );    // V3.7

    LMX_PDECL bool skip_rest_of_element( const std::string & r_name_in );
                                // Skips remainder of start tag, element body (if any)
                                // -and element end tag.
    LMX_PDECL bool skip_rest_of_empty_element( const std::string & r_name_in );
    LMX_PDECL bool skip_element_body( const std::string & r_name_in );
    LMX_PDECL static bool is_error_event( int event_in )
    {
        // Don't include EXE_EOM as a message that contains only a global simple type get this
        if( event_in == EXE_FATAL_ERROR || event_in == EXE_BADLY_FORMED )
            return true;
        return false;
    }

    LMX_PDECL t_comment_collection & get_comment_collection()   // For LMX_WANT_COMMENT_AND_PI
    {
        return m_getter.get_comment_collection();
    }
    LMX_PDECL bool get_has_pi() { return m_getter.get_has_pi(); } // For LMX_WANT_COMMENT_AND_PI
};

//------------------------------------------------------------------------------
//                           Tokeniser
//------------------------------------------------------------------------------

class c_xml_list_tokeniser
{
private:
    std::string m_string;
    std::string::const_iterator mi_position;

public:
    LMX_PDECL c_xml_list_tokeniser( const std::string & r_string_in );
    LMX_PDECL ~c_xml_list_tokeniser() {}
    LMX_PDECL void set( const std::string & r_string_in );
    LMX_PDECL bool get( std::string * p_sub_string_in );

    LMX_PDECL static size_t count( const std::string & r_string_in );
};

//------------------------------------------------------------------------------
//                  Reader and writer classes common components
//------------------------------------------------------------------------------

typedef int t_enumed_namespace;

struct s_event_map
{
    t_enumed_namespace          known_namespace;
    const char *                p_name;
    size_t                      size;
    t_xml_event                 m_event;
};

struct s_ns_map
{
    int                         id;
    const char *                p_ns_prefix;
    const char *                p_ns;
};

struct s_user_ns_prefix_map
{
    std::string m_ns_prefix;
    std::string ns_uri;
    s_user_ns_prefix_map( const std::string & r_ns_prefix_in, const std::string & r_ns_uri_in ) :
            m_ns_prefix( r_ns_prefix_in ), ns_uri( r_ns_uri_in )
    {}
};

typedef std::vector< s_user_ns_prefix_map > t_user_namespaces;

struct s_schema_location_item
{
    std::string ns_uri;
    std::string location;
    s_schema_location_item( const std::string & r_ns_uri_in, const std::string & r_location_in ) :
            ns_uri( r_ns_uri_in ), location( r_location_in )
    {}
};

typedef std::vector< s_schema_location_item > t_schema_locations;

//------------------------------------------------------------------------------
//                      Validator specification classes
//------------------------------------------------------------------------------

class c_xml_reader;
class c_xml_writer;

typedef elmx_error (*tpf_list_lexical_validator)( c_xml_reader & r_reader_in, const std::string & r_value_in );

class c_untyped_validation_spec
{
public:
    typedef elmx_error (*tpf_lexical_validator)( c_xml_reader & r_reader_in, const std::string & r_value_in );

private:
    elmx_whitespace         m_whitespace_handling;
    std::string             m_default;
    tpf_lexical_validator   m_pf_lexical_validator;

public:
    LMX_PDECL c_untyped_validation_spec(
                    elmx_whitespace             whitespace_handling_in,
                    const std::string &         r_default_in,
                    const tpf_lexical_validator apf_lexical_validator )
        :
        m_whitespace_handling( whitespace_handling_in ),
        m_default( r_default_in ),
        m_pf_lexical_validator( apf_lexical_validator )
    {}

    LMX_PDECL elmx_whitespace get_whitespace_handling() const { return m_whitespace_handling; }
    LMX_PDECL const std::string & get_default() const { return m_default; }
    LMX_PDECL elmx_error is_lexical_valid( c_xml_reader & r_reader_in, const std::string & r_value_in ) const
        { return (*m_pf_lexical_validator)( r_reader_in, r_value_in ); }
};

template< typename T >
class ct_typed_validation_spec : public c_untyped_validation_spec
{
public:
    typedef elmx_error (*tpf_value_validator)( c_xml_reader & r_reader_in, const T & r_value_in );
    typedef elmx_error (*tpf_user_validator)( const T & r_value_in );
    typedef void (*tpf_v_to_o)( T & r_object_in, const std::string & r_value_in );
    typedef std::ostream & (*tpf_o_to_v)( std::ostream & r_os_in, const T & r_value_in );

private:
    tpf_value_validator m_pf_value_validator;
    tpf_v_to_o          m_pf_v_to_o;
    tpf_o_to_v          m_pf_o_to_v;

public:
    ct_typed_validation_spec(
                    elmx_whitespace             whitespace_handling_in,
                    const char *                p_default_in,
                    const tpf_lexical_validator apf_lexical_validator,
                    const tpf_value_validator   apf_value_validator,
                    const tpf_v_to_o            apf_v_to_o,
                    const tpf_o_to_v            apf_o_to_v )
        :
        c_untyped_validation_spec( whitespace_handling_in, p_default_in, apf_lexical_validator ),
        m_pf_value_validator( apf_value_validator ),
        m_pf_v_to_o( apf_v_to_o ),
        m_pf_o_to_v( apf_o_to_v )
    {}

    elmx_error is_value_valid( c_xml_reader & r_reader_in, const T & r_value_in ) const
        { return (*m_pf_value_validator)( r_reader_in, r_value_in ); }
    void v_to_o( T & r_object_in, const std::string & r_value_in ) const
        { (*m_pf_v_to_o)( r_object_in, r_value_in ); }
    std::ostream & o_to_v( std::ostream & r_os_in, const T & r_value_in ) const
        { return (*m_pf_o_to_v)( r_os_in, r_value_in ); }
};

LMX_PDECL extern const ct_typed_validation_spec< c_empty > validation_spec_empty;

//------------------------------------------------------------------------------
// When the in and out converter specs in a ct_typed_validation_spec are not used,
// these are used instead.
//------------------------------------------------------------------------------

template <typename T>
inline void v_to_o_not_used( T & r_out, const std::string & r_in )
{
    lmx_assert( 0 );    // QName and others need special handling
    LMX_NO_WARN_UNUSED( r_out );
    LMX_NO_WARN_UNUSED( r_in );
}

template <typename T>
inline std::ostream & o_to_v_not_used( std::ostream & r_os_in, const T & r_v_in )
{
    lmx_assert( 0 );    // QName and others need special handling
    LMX_NO_WARN_UNUSED( r_os_in );
    LMX_NO_WARN_UNUSED( r_v_in );
    return r_os_in;
}

//------------------------------------------------------------------------------
//                               c_handling
// This class helps get method template overloading functionality in a way that
// works with older compilers.  Hopefully the presence of this class will be
// optimised out by the compiler. (C101015_1032)
//------------------------------------------------------------------------------

template< typename T, typename Thandling >
class c_handling
{};

//------------------------------------------------------------------------------
//          Bridge classes to interface between the (un)marshalling
//                  classes and the collection classes
//------------------------------------------------------------------------------

class c_untyped_unmarshal_bridge
{
public:
    virtual bool is_in_mixed() const = 0;

    virtual bool is_already_unmarshalled() const = 0;
    virtual void mk_new_space() = 0;
    virtual tlmx_unicode_string & get_new_space_text_ref() = 0;

    virtual elmx_error get_element_value() = 0;
    virtual elmx_error convert_store_and_check_value( const std::string & r_value_in ) = 0;

    virtual ~c_untyped_unmarshal_bridge() {}    // To keep GCC happy
};

template< typename Treader, typename Tatomic_type, typename Thandling >
class c_atomic_unmarshal_bridge : public c_untyped_unmarshal_bridge
{
public:
    c_atomic_unmarshal_bridge( Treader & r_reader_in, const ct_typed_validation_spec< Tatomic_type > & r_item_spec_in )
        : mr_reader( r_reader_in ), mr_item_spec( r_item_spec_in )
    {}

    virtual Tatomic_type & get_new_space_value_ref() = 0;

    virtual elmx_error get_element_value()
    {
        return mr_reader.get_element_value( mr_item_spec, c_handling< Tatomic_type, Thandling >() );
    }
    virtual elmx_error convert_store_and_check_value( const std::string & r_value_in )
    {
        mk_new_space();
        Tatomic_type & r_stored_value = get_new_space_value_ref();
        mr_reader.v_to_o( r_stored_value, r_value_in, mr_item_spec, c_handling< Tatomic_type, Thandling >() );
        return mr_item_spec.is_value_valid( mr_reader, r_stored_value );
    }

private:
    Treader & mr_reader;
    const ct_typed_validation_spec< Tatomic_type > & mr_item_spec;

    c_atomic_unmarshal_bridge & operator = ( const c_atomic_unmarshal_bridge & );
};

template< typename Treader, typename Tcollection, typename Tatomic_type, typename Thandling >
class c_unmarshal_bridge : public c_atomic_unmarshal_bridge< Treader, Tatomic_type, Thandling >
{
public:
    c_unmarshal_bridge( Treader & r_reader_in, Tcollection & r_collection_in, const ct_typed_validation_spec< Tatomic_type > & r_item_spec_in )
        :
        c_atomic_unmarshal_bridge< Treader, Tatomic_type, Thandling >( r_reader_in, r_item_spec_in ),
        mr_collection( r_collection_in )
    {}

    virtual bool is_in_mixed() const { return mr_collection.is_in_mixed(); }

    virtual bool is_already_unmarshalled() const { return mr_collection.is_already_unmarshalled(); }
    virtual void mk_new_space() { mr_collection.mk_new_space(); }
    virtual tlmx_unicode_string & get_new_space_text_ref() { return mr_collection.get_new_space_text_ref(); }
    virtual Tatomic_type & get_new_space_value_ref() { return mr_collection.get_new_space_value_ref(); }

    c_untyped_unmarshal_bridge & base() { return *this; }   // Older compilers like gcc4.3 need a little help!

private:
    Tcollection & mr_collection;

    c_unmarshal_bridge & operator = ( const c_unmarshal_bridge & );
};

class c_untyped_unmarshal_list_bridge
{
public:
    virtual bool is_in_mixed() const = 0;

    virtual bool is_already_unmarshalled() const = 0;
    virtual void start_new_list() = 0;
    virtual tlmx_unicode_string & get_new_space_text_ref() = 0;
    virtual void mk_new_space() = 0;
    virtual size_t new_list_size() const = 0;

    virtual elmx_error get_element_value() = 0;
    virtual elmx_error convert_store_and_check_value( const std::string & r_value_in ) = 0;

    virtual ~c_untyped_unmarshal_list_bridge() {}   // To keep GCC happy
};

template< typename Treader, typename Tatomic_type, typename Thandling >
class c_atomic_unmarshal_list_bridge : public c_untyped_unmarshal_list_bridge
{
public:
    c_atomic_unmarshal_list_bridge( Treader & r_reader_in, const ct_typed_validation_spec< Tatomic_type > & r_item_spec_in )
        : mr_reader( r_reader_in ), mr_item_spec( r_item_spec_in )
    {}

    virtual Tatomic_type & get_new_space_value_ref() = 0;

    virtual elmx_error get_element_value()
    {
        return mr_reader.get_element_value( mr_item_spec, c_handling< Tatomic_type, Thandling >() );
    }
    virtual elmx_error convert_store_and_check_value( const std::string & r_value_in )
    {
        mk_new_space();
        Tatomic_type & r_stored_value = get_new_space_value_ref();
        mr_reader.v_to_o( r_stored_value, r_value_in, mr_item_spec, c_handling< Tatomic_type, Thandling >() );
        return mr_item_spec.is_value_valid( mr_reader, r_stored_value );
    }

private:
    Treader & mr_reader;
    const ct_typed_validation_spec< Tatomic_type > & mr_item_spec;

    c_atomic_unmarshal_list_bridge & operator = ( const c_atomic_unmarshal_list_bridge & );
};

template< typename Treader, typename Tcollection, typename Tatomic_type, typename Thandling >
class c_unmarshal_list_bridge : public c_atomic_unmarshal_list_bridge< Treader, Tatomic_type, Thandling >
{
public:
    c_unmarshal_list_bridge( Treader & r_reader_in, Tcollection & r_collection_in, const ct_typed_validation_spec< Tatomic_type > & r_item_spec_in )
        :
        c_atomic_unmarshal_list_bridge< Treader, Tatomic_type, Thandling >( r_reader_in, r_item_spec_in ),
        mr_collection( r_collection_in )
    {}

    virtual bool is_in_mixed() const { return mr_collection.is_in_mixed(); }

    virtual bool is_already_unmarshalled() const { return mr_collection.is_already_unmarshalled(); }
    virtual void start_new_list() { mr_collection.start_new_list(); }
    virtual tlmx_unicode_string & get_new_space_text_ref() { return mr_collection.get_new_space_text_ref(); }
    virtual void mk_new_space() { mr_collection.mk_new_space(); }
    virtual size_t new_list_size() const { return mr_collection.new_list_size(); }
    virtual Tatomic_type & get_new_space_value_ref() { return mr_collection.get_new_space_value_ref(); }

    c_untyped_unmarshal_list_bridge & base() { return *this; }  // Older compilers like gcc4.3 need a little help!

private:
    Tcollection & mr_collection;

    c_unmarshal_list_bridge & operator = ( const c_unmarshal_list_bridge & );
};

class c_untyped_marshal_bridge
{
public:
    virtual bool is_in_mixed() const = 0;

    virtual size_t count() const = 0;               // Number of elements
    virtual const tlmx_unicode_string & access_text( size_t i_in ) const = 0;

    virtual bool is_value_empty( size_t i_in ) const = 0;
    virtual void output_value( size_t i_in ) = 0;

    virtual ~c_untyped_marshal_bridge() {}  // To keep GCC happy
};

template< typename Twriter, typename Tatomic_type, typename Thandling >
class c_atomic_marshal_bridge : public c_untyped_marshal_bridge
{
public:
    c_atomic_marshal_bridge( Twriter & r_writer_in, const ct_typed_validation_spec< Tatomic_type > & r_item_spec_in )
        : mr_writer( r_writer_in ), mr_item_spec( r_item_spec_in )
    {}

    virtual const Tatomic_type & access_value( size_t i_in ) const = 0;

    virtual bool is_value_empty( size_t i_in ) const
    {
        return mr_writer.is_value_empty( access_value( i_in ), c_handling< Tatomic_type, Thandling >() );
    }
    virtual void output_value( size_t i_in )
    {
        mr_writer.output_value( access_value( i_in ), mr_item_spec, c_handling< Tatomic_type, Thandling >() );
    }
    const ct_typed_validation_spec< Tatomic_type > & get_item_spec() const { return mr_item_spec; }

private:
    Twriter & mr_writer;
    const ct_typed_validation_spec< Tatomic_type > & mr_item_spec;

    c_atomic_marshal_bridge & operator = ( const c_atomic_marshal_bridge & );
};

template< typename Twriter, typename Tcollection, typename Tatomic_type, typename Thandling >
class c_marshal_bridge : public c_atomic_marshal_bridge< Twriter, Tatomic_type, Thandling >
{
public:
    c_marshal_bridge( Twriter & r_writer_in, const Tcollection & r_collection_in, const ct_typed_validation_spec< Tatomic_type > & r_item_spec_in )
        :
        c_atomic_marshal_bridge< Twriter, Tatomic_type, Thandling >( r_writer_in, r_item_spec_in ),
        mr_collection( r_collection_in ),
        m_count( r_collection_in.count() )    // See Note GCC_O3_Optimization_Override
    {}

    virtual bool is_in_mixed() const { return mr_collection.is_in_mixed(); }

    virtual size_t count() const { return m_count; }               // Number of elements    // See Note GCC_O3_Optimization_Override
    virtual const tlmx_unicode_string & access_text( size_t i_in ) const { return mr_collection.access_text( i_in ); }
    virtual const Tatomic_type & access_value( size_t i_in ) const { return mr_collection.access_value( i_in ); }

    c_untyped_marshal_bridge & base() { return *this; } // Older compilers like gcc4.3 need a little help!

    virtual ~c_marshal_bridge() {}

private:
    const Tcollection & mr_collection;
    size_t m_count; // See Note GCC_O3_Optimization_Override

    //c_marshal_bridge & operator = ( const c_marshal_bridge & );
};

class c_untyped_marshal_list_bridge
{
public:
    virtual bool is_in_mixed() const = 0;

    virtual size_t list_count() const = 0;          // Number of lists
    virtual const tlmx_unicode_string & access_text( size_t i_in ) const = 0;
    virtual size_t count( size_t i_in ) const = 0;     // Number of items in list i

    virtual void output_value( size_t i_in, size_t j_in ) = 0;

    virtual ~c_untyped_marshal_list_bridge() {} // To keep GCC happy
};

template< typename Twriter, typename Tatomic_type, typename Thandling >
class c_atomic_marshal_list_bridge : public c_untyped_marshal_list_bridge
{
public:
    c_atomic_marshal_list_bridge( Twriter & r_writer_in, const ct_typed_validation_spec< Tatomic_type > & r_item_spec_in )
        : mr_writer( r_writer_in ), mr_item_spec( r_item_spec_in )
    {}

    virtual const Tatomic_type & access_value( size_t i_in, size_t j_in ) const = 0;

    virtual void output_value( size_t i_in, size_t j_in )
    {
        mr_writer.output_value( access_value( i_in, j_in ), mr_item_spec, c_handling< Tatomic_type, Thandling >() );
    }
    const ct_typed_validation_spec< Tatomic_type > & get_item_spec() const { return mr_item_spec; }

private:
    Twriter & mr_writer;
    const ct_typed_validation_spec< Tatomic_type > & mr_item_spec;

    c_atomic_marshal_list_bridge & operator = ( const c_atomic_marshal_list_bridge & );
};

template< typename Twriter, typename Tcollection, typename Tatomic_type, typename Thandling >
class c_marshal_list_bridge : public c_atomic_marshal_list_bridge< Twriter, Tatomic_type, Thandling >
{
public:
    c_marshal_list_bridge( Twriter & r_writer_in, const Tcollection & r_collection_in, const ct_typed_validation_spec< Tatomic_type > & r_item_spec_in )
        :
        c_atomic_marshal_list_bridge< Twriter, Tatomic_type, Thandling >( r_writer_in, r_item_spec_in ),
        mr_collection( r_collection_in ),
        m_list_count( r_collection_in.list_count() )  // See Note GCC_O3_Optimization_Override
    {}

    virtual bool is_in_mixed() const { return mr_collection.is_in_mixed(); }

    virtual size_t list_count() const { return m_list_count; }  // See Note GCC_O3_Optimization_Override
    virtual const tlmx_unicode_string & access_text( size_t i_in ) const { return mr_collection.access_text( i_in ); }
    virtual size_t count( size_t i_in ) const { return mr_collection.count( i_in ); }     // Number of items in list i
    virtual const Tatomic_type & access_value( size_t i_in, size_t j_in ) const { return mr_collection.access_value( i_in, j_in ); }

    c_untyped_marshal_list_bridge & base() { return *this; }    // Older compilers like gcc4.3 need a little help!

    virtual ~c_marshal_list_bridge() {}

private:
    const Tcollection & mr_collection;
    size_t m_list_count;    // See Note GCC_O3_Optimization_Override

    c_marshal_list_bridge & operator = ( const c_marshal_list_bridge & );
};

//------------------------------------------------------------------------------
//                               Reader class(es)
//------------------------------------------------------------------------------

class c_xml_reader_local : public c_xml_local
{
private:
    c_xml_reader * mp_reader;
    c_xml_reader_local * mp_prev;

public:
    LMX_PDECL c_xml_reader_local( c_xml_reader * p_reader_in );
    LMX_PDECL explicit c_xml_reader_local();
    LMX_PDECL void set_reader( c_xml_reader * p_reader_in );
    LMX_PDECL ~c_xml_reader_local();
};

class c_cracked_name
{
private:
    std::string m_full_name;
    bool m_has_ns_prefix;
    int m_ns_id;
    std::string m_local_name;
    std::string m_ns_prefix;

public:
    LMX_PDECL c_cracked_name() : m_has_ns_prefix( false ), m_ns_id( 0 ) {}
    LMX_PDECL c_cracked_name( c_xml_local * p_reader_local_in, const std::string & r_name_in ) { set( p_reader_local_in, r_name_in ); }
    LMX_PDECL void set( c_xml_local * p_reader_local_in, const std::string & r_name_in );
    LMX_PDECL void set_for_attribute( c_xml_local * p_reader_local_in, const std::string & r_name_in );   // v4.1
    LMX_PDECL const std::string &get_full_name() const { return m_full_name; }
    LMX_PDECL bool get_has_ns_prefix() const { return m_has_ns_prefix; }
    LMX_PDECL int get_ns_id() const { return m_ns_id; }
    LMX_PDECL int get_element_ns_id() const;    // V3.7.5 - returns ns_id for an element
    LMX_PDECL const std::string &get_ns_prefix() const { return m_ns_prefix; }
    LMX_PDECL const std::string &get_local_name() const { return m_local_name; }
};

struct s_known_elements
{
    t_enumed_namespace          known_namespace;
    const char *                p_local_name;
    size_t                      local_name_size;
};

class c_known_elements_selector
{
private:
    c_xml_reader & mr_xml_reader;
    const s_known_elements * mp_old_known_elements;

private:
    c_known_elements_selector( const c_known_elements_selector & );     // No copy operations
    c_known_elements_selector & operator =( const c_known_elements_selector & );

public:
    LMX_PDECL c_known_elements_selector( c_xml_reader & r_xml_reader_in, const s_known_elements * p_known_elements_in );
    LMX_PDECL ~c_known_elements_selector();
};

//------------------------------------------------------------------------------
//                   class c_string_stack and c_string_stack_node
//        These two classes work together to produce a stack of strings.
//       The intention is that as we move in and out of elements their names
//           can be stored efficiently on the stack by using memory that
//                     might have been previously allocated.
//------------------------------------------------------------------------------

class c_string_stack
{
private:
    std::vector< char > m_string_stack;
    std::vector< size_t > m_indexes;

    // All the functionality is in c_string_stack_node
    friend class c_string_stack_node;

public:
    LMX_PDECL size_t size() const { return m_indexes.size(); }
    LMX_PDECL const char * operator [] ( size_t index_in ) const { return & m_string_stack[m_indexes[index_in]]; }
    LMX_PDECL const char * back() const { return & m_string_stack[m_indexes.back()]; }
    LMX_PDECL std::string to_string( const std::string & separator = "/" ) const
    {
        std::string result;
        for( size_t i=0; i<m_indexes.size(); ++i )
        {
            result += separator;
            result += (*this)[i];
        }
        return result;
    }

    LMX_PDECL size_t get_stack_size() const { return m_string_stack.size(); } // For testing
};

class c_string_stack_node
{
private:
    c_string_stack & mr_string_stack;
    size_t m_my_string_base_index;

private:
    c_string_stack_node( const c_string_stack_node & );     // No copy operations
    c_string_stack_node & operator =( const c_string_stack_node & );

public:
    LMX_PDECL c_string_stack_node( c_string_stack & r_string_stack_in, const std::string & r_string_in )
        :
        mr_string_stack( r_string_stack_in ),
        m_my_string_base_index( r_string_stack_in.m_string_stack.size() )
    {
        mr_string_stack.m_indexes.push_back( m_my_string_base_index );
        r_string_stack_in.m_string_stack.reserve( mr_string_stack.m_string_stack.size() + r_string_in.size() + 1 );
        mr_string_stack.m_string_stack.insert( mr_string_stack.m_string_stack.end(), r_string_in.begin(), r_string_in.end() );
        mr_string_stack.m_string_stack.push_back( '\0' );
    }
    LMX_PDECL const char * get_str() const { return &mr_string_stack.m_string_stack[m_my_string_base_index]; }
    LMX_PDECL bool operator == ( const std::string & r_string_in ) const
    {
        return r_string_in.size() <= mr_string_stack.m_string_stack.size() - m_my_string_base_index - 1 &&
                mr_string_stack.m_string_stack[m_my_string_base_index + r_string_in.size()] == '\0' &&
                r_string_in == &mr_string_stack.m_string_stack[m_my_string_base_index];
    }
    LMX_PDECL bool operator != ( const std::string & r_string_in ) const { return ! (*this == r_string_in); }
    LMX_PDECL ~c_string_stack_node()
    {
        if( mr_string_stack.m_indexes.size() > 0 )  // C170512_1914 - Prevent problem if exception thrown while doing push_back() in constructor (spotted by g++ 7.1.0)
            mr_string_stack.m_indexes.resize( mr_string_stack.m_indexes.size() - 1 );
        mr_string_stack.m_string_stack.resize( m_my_string_base_index );
    }
};

//------------------------------------------------------------------------------
//                               class c_xml_reader
//                    The main reader class when unmarshaling
//------------------------------------------------------------------------------

class c_xml_reader : public c_xml
{
private:
    c_xml_reader_local * mp_reader_local;
    c_xml_reader_local m_bottom_reader_local;
    c_error m_error;
    s_debug_error m_error_info;   // C100806_1136
    c_string_stack m_element_name_stack;
    c_cracked_name m_cracked_name;
    t_xml_event m_event;
    t_user_namespaces m_user_namespaces;
    const s_known_elements * mp_known_elements;
    bool m_want_auto_versioning;
    struct s_code_location
    {
        const char * mp_file;
        int m_line;
        s_code_location() : mp_file( "<unspecified>" ), m_line( -1 ) {}
    } m_code_location;

public: // These members allow reuse of the same string during unmarshalling.
        // This increases efficiency by removing the need for repeated allocation.
    std::string name;
    std::string value;
    std::string attr_name;  // Deprecated in v5

public:
    // User accessible functions
    LMX_PDECL c_xml_reader( c_read & r_low_reader_in ) :
                    c_xml( r_low_reader_in, m_error ),
                    mp_reader_local( LMXNULL ),
                    m_bottom_reader_local(),
                    mp_known_elements( LMXNULL ),
                    m_want_auto_versioning( false )
    {
        m_bottom_reader_local.set_reader( this );
        mp_reader_local = &m_bottom_reader_local;
    }
    LMX_PDECL virtual ~c_xml_reader() {}
    LMX_PDECL void set_auto_versioning( bool want_auto_versioning_in ) { m_want_auto_versioning = want_auto_versioning_in; }
    LMX_PDECL bool is_auto_versioning() const { return m_want_auto_versioning; }
    LMX_PDECL c_string_stack & get_element_name_stack() { return m_element_name_stack; }
    LMX_PDECL void add_namespace_mapping( const std::string & r_ns_prefix_in, const std::string & r_namespace_in );
    LMX_PDECL void add_default_namespace_mapping( const std::string & r_namespace_in )
    {
        add_namespace_mapping( "", r_namespace_in );
    }
    LMX_PDECL void add_namespace_mappings( const c_namespace_context & r_namespace_context_in );    // C190405_1134
    LMX_PDECL void add_namespace_mappings( const c_any_info & r_any_info_in );    // C190405_1134
    LMX_PDECL static std::string & get_local_name( const std::string & r_name_in, std::string * p_local_name_out_in )
        { return c_xml::get_local_name( r_name_in, p_local_name_out_in ); }
    LMX_PDECL static std::string & get_namespace_prefix( const std::string & r_name_in, std::string * p_namespace_prefix_out_in )
        { return c_xml::get_namespace_prefix( r_name_in, p_namespace_prefix_out_in ); }
    LMX_PDECL std::string & get_namespace( const std::string & r_name_in, std::string * p_namespace_out_in ) const;
    LMX_PDECL virtual elmx_error user_handle_error( elmx_error error_in ) { return error_in; }
    LMX_PDECL virtual elmx_error user_handle_error( elmx_error error_in, const std::string & r_item_name_in,
                                            const char * p_prog_file_in, int prog_line_in )
    {
        LMX_NO_WARN_UNUSED( r_item_name_in );
        LMX_NO_WARN_UNUSED( p_prog_file_in );
        LMX_NO_WARN_UNUSED( prog_line_in );

    #if defined( LMX_DEBUG_CAPTURE_ERROR ) && LMX_DEBUG_CAPTURE_ERROR == 1
        debug_error.set( error_in, r_item_name_in, p_prog_file_in, prog_line_in, get_line_no() );
    #endif
    #if LMX_USE_EXCEPTIONS == 1
        if( error_in != ELMX_OK )    // Avoid any chance of an unreachable code warning
            throw c_lmx_reader_exception( error_in, get_line_no() );
    #endif
        return user_handle_error( error_in );    // For backwards compatibility
    }
    LMX_PDECL elmx_error capture_error( elmx_error error_in, const std::string & r_item_name_in, const char * p_prog_file_in, int prog_line_in );    // V4.2
    LMX_PDECL elmx_error handle_error( elmx_error error_in, const std::string & r_item_name_in, const char * p_prog_file_in, int prog_line_in )  // V4.2
    {
        error_in = capture_error( error_in, r_item_name_in, p_prog_file_in, prog_line_in  );   // Set before calling user_handle_error so user can call get_error_message
        return user_handle_error( error_in, r_item_name_in, p_prog_file_in, prog_line_in  );
    }
    LMX_PDECL elmx_error handle_error( elmx_error error_in, int prog_line_in )  // V5
    {
        return handle_error( error_in, name, m_code_location.mp_file, prog_line_in );
    }
    LMX_PDECL elmx_error handle_error( elmx_error error_in )  // V5
    {
        return handle_error( error_in, name, m_code_location.mp_file, m_code_location.m_line );
    }
    LMX_PDECL elmx_error handle_attribute_error( elmx_error error_in )  // V5
    {
        return handle_error( error_in, std::string( "@" ) + name, m_code_location.mp_file, m_code_location.m_line );
    }
    LMX_PDECL virtual std::string & get_error_message( std::string * p_message_in ) const;
    LMX_PDECL std::string get_error_message() const;
    LMX_PDECL const s_debug_error & get_debug_error() const { return m_error_info; }
    LMX_PDECL int get_line_no() const { return c_xml::get_line_num(); }
    LMX_PDECL const std::string & get_error_item_name() const { return m_error_info.item_name; }

    // V7.1 - C150417_1659
    LMX_PDECL virtual elmx_error user_found_unknown_attribute(
                                            const std::string & r_attribute_name_in,
                                            const c_string_stack & r_element_name_stack_in,
                                            const char * p_prog_file_in,
                                            int prog_line_in )
    {
        LMX_NO_WARN_UNUSED( r_attribute_name_in );
        LMX_NO_WARN_UNUSED( r_element_name_stack_in );
        LMX_NO_WARN_UNUSED( p_prog_file_in );
        LMX_NO_WARN_UNUSED( prog_line_in );

        return ELMX_OK;
    }
    LMX_PDECL elmx_error found_unknown_attribute(
                                            const std::string & r_attribute_name_in,
                                            const char * p_prog_file_in,
                                            int prog_line_in )
    {
        elmx_error error = user_found_unknown_attribute(
                                                r_attribute_name_in,
                                                m_element_name_stack,
                                                p_prog_file_in,
                                                prog_line_in );
        if( error != ELMX_OK )
            capture_error( error, r_attribute_name_in, p_prog_file_in, prog_line_in  );
        return error;
    }
    LMX_PDECL virtual elmx_error user_found_unknown_element(
                                            const std::string & r_local_element_name_in,
                                            const c_string_stack & r_element_name_stack_in,
                                            const char * p_prog_file_in,
                                            int prog_line_in )
    {
        LMX_NO_WARN_UNUSED( r_local_element_name_in );
        LMX_NO_WARN_UNUSED( r_element_name_stack_in );
        LMX_NO_WARN_UNUSED( p_prog_file_in );
        LMX_NO_WARN_UNUSED( prog_line_in );

        return ELMX_OK;
    }
    LMX_PDECL elmx_error found_unknown_element(
                                            const std::string & r_element_name_in,
                                            const char * p_prog_file_in,
                                            int prog_line_in )
    {
        elmx_error error = user_found_unknown_element(
                                                r_element_name_in,
                                                m_element_name_stack,
                                                p_prog_file_in,
                                                prog_line_in );
        if( error != ELMX_OK )
            capture_error( error, r_element_name_in, p_prog_file_in, prog_line_in  );
        return error;
    }

    LMX_PDECL void set_ns_map( const s_ns_map * p_ns_map_in );

    // Parser functions
    LMX_PDECL elmx_error get_error() { return m_error.get_last_error(); }
    LMX_PDECL bool get_error_info( elmx_error_type * p_type_in, elmx_error * p_code_in, int * p_line_in )
        { return m_error.get_error_info( p_type_in, p_code_in, p_line_in ); }
    LMX_PDECL c_xml_reader_local * get_reader_local() const { return mp_reader_local; }
    LMX_PDECL void set_reader_local( c_xml_reader_local * p_reader_local_in ) { mp_reader_local = p_reader_local_in; }
    LMX_PDECL const std::string & get_full_name() const { return m_cracked_name.get_full_name(); }
    LMX_PDECL bool get_has_ns_prefix() const { return m_cracked_name.get_has_ns_prefix(); }
    LMX_PDECL int get_ns_id() const { return m_cracked_name.get_ns_id(); }
    LMX_PDECL int get_element_ns_id() const { return m_cracked_name.get_element_ns_id(); }
    LMX_PDECL const std::string & get_local_name() const { return m_cracked_name.get_local_name(); }
    LMX_PDECL void set_known_elements( const s_known_elements * p_known_elements_in ) { mp_known_elements = p_known_elements_in; }
    LMX_PDECL const s_known_elements * get_known_elements() const { return mp_known_elements; }
    LMX_PDECL void tokenise( const s_event_map c_event_map_in[], elxp_attr_elem type_in );
    LMX_PDECL bool is_current_event_tokenisable() const { return m_event == EXE_TOKEN || m_event == EXE_UNKNOWN || m_event >= EXE_LAST_XML_EVENT; }
    LMX_PDECL bool is_xsi_attribute() const { return c_xml::is_xsi_attribute( mp_reader_local, name ); } // v5
    LMX_PDECL bool is_xsi_attribute( const std::string & r_name_in ) const { return c_xml::is_xsi_attribute( mp_reader_local, r_name_in ); }
    LMX_PDECL bool get_xsi_type( int * p_type_ns_id_in, std::string * p_type_local_name_in )
        { return c_xml::get_xsi_type( mp_reader_local, p_type_ns_id_in, p_type_local_name_in ); }
    LMX_PDECL bool get_named_attribute( int ns_id_in, const std::string & r_local_name_in, std::string * p_value_out )  // V3.7
        { return c_xml::get_named_attribute( mp_reader_local, ns_id_in, r_local_name_in, p_value_out ); }
    LMX_PDECL bool get_attribute_event( const s_event_map c_event_map_in[], elmx_error * p_error_out, std::string * p_name_out );
    LMX_PDECL void get_element_event( elmx_error * p_error_out, std::string * p_name_out ); // v4.3
    LMX_PDECL void get_element_event( elmx_error * p_error_out ) { get_element_event( p_error_out, &name ); }  // v5
    LMX_PDECL void get_element_event( const s_event_map c_event_map_in[], elmx_error * p_error_out, std::string * p_name_out );
    LMX_PDECL void get_element_event( const s_event_map c_event_map_in[], elmx_error * p_error_out )
        { get_element_event( c_event_map_in, p_error_out, &name ); } // v5
    LMX_PDECL void get_known_element_event( elmx_error * p_error_out, std::string * p_name_out );  // v5.8 - C110707_1155
    LMX_PDECL void get_known_element_event( const s_event_map c_event_map_in[], elmx_error * p_error_out, std::string * p_name_out ); // V3.7.5
    LMX_PDECL void get_known_element_event( const s_event_map c_event_map_in[], elmx_error * p_error_out )
        { get_known_element_event( c_event_map_in, p_error_out, &name ); } // v5.8 - C110707_1155
    LMX_PDECL t_xml_event get_current_event() const { return m_event; }
    LMX_PDECL void restore_event( t_xml_event event_in ) { m_event = event_in; }
    LMX_PDECL bool skip_start_tag( bool * p_is_end_of_element_in = LMXNULL );  // V5
    LMX_PDECL bool get_element_value( std::string * p_value_out,
                                    elmx_whitespace ws_mode_in = EXWS_PRESERVE );    // V5
    LMX_PDECL bool get_nested_element_value( std::string * p_value_out, std::string * p_end_tag_name_out,
                                    elmx_whitespace ws_mode_in = EXWS_PRESERVE,
                                    bool expand_entities_in = false );
    LMX_PDECL bool get_simple_type_value( const std::string & r_name_in, elmx_whitespace ws_mode_in, std::string * p_value_out, elmx_error * p_error_out, bool * p_is_empty_element_out );
    LMX_PDECL bool get_element_ur_value( const std::string & r_name_in, std::string * p_value_out, elmx_error * p_error_out, bool * p_is_empty_element_out );
    LMX_PDECL bool is_element_end_tag() { return m_event == EXE_ELEMENT_END || c_xml::is_element_end_tag( name ); }

    LMX_PDECL bool is_xml_end() { elmx_error l_error; get_element_event( &l_error ); return l_error == ELMX_UNEXPECTED_EOM; }

    // V3.2.4
    LMX_PDECL bool is_valid_qname( const std::string & r_value_in ) const;
    template< typename Tstring >
    inline void set_qname( ct_qname< Tstring > & r_qname_in, const std::string & r_value_in ) const
    {
        c_cracked_name cracked_name;
        cracked_name.set( mp_reader_local, r_value_in );
        Tstring tstring;
        convert( &tstring, cracked_name.get_local_name() );
        r_qname_in.set( find_namespace_from_id( cracked_name.get_ns_id() ), tstring );
    }
    inline void set_qname( std::string & r_qname_in, const std::string & r_value_in ) const
            { r_qname_in = r_value_in; }
    #ifndef LMX_NO_WSTRING
    inline void set_qname( std::wstring & r_qname_in, const std::string & r_value_in ) const
            { convert( &r_qname_in, r_value_in ); }
    #endif

    // v5.5
    LMX_PDECL elmx_error ignore_element( const s_event_map c_event_map_in[] );    // C110317_1715

    // New for v5
    template< typename Tatomic_type, typename Thandling >  // C101015_1032
    elmx_error get_element_value(
                        const ct_typed_validation_spec< Tatomic_type > & r_item_spec_in,
                        const c_handling< Tatomic_type, Thandling > & )
    {
        if( ! get_element_value( &value, r_item_spec_in.get_whitespace_handling() ) )
            return handle_error( ELMX_UNABLE_TO_READ_ELEMENT_VALUE );
        return ELMX_OK;
    }
    elmx_error get_element_value(
                        const ct_typed_validation_spec< tlmx_string > &,
                        const c_handling< tlmx_string, c_handling_ur_type > & )
    {
        if( ! get_nested_element_value( &value, &name ) )
            return handle_error( ELMX_UNABLE_TO_READ_ELEMENT_VALUE );
        return ELMX_OK;
    }

    // v7.3 - C170630_1501
    void extract_namespace_context( c_namespace_context * p_namespace_context ) const
    {
        get_reader_local()->extract_namespace_context( p_namespace_context, *this );
    }

    template< typename Tatomic_type, typename Thandling >  // C101015_1032
    void v_to_o(
            Tatomic_type & r_v_in,
            const std::string & r_value_in,
            const ct_typed_validation_spec< Tatomic_type > & r_item_spec_in,
            const c_handling< Tatomic_type, Thandling > & )
    {
        r_item_spec_in.v_to_o( r_v_in, r_value_in );
    }
    void v_to_o(
            tc_qname & r_v_in,
            const std::string & r_value_in,
            const ct_typed_validation_spec< tc_qname > &,
            const c_handling< tc_qname, c_handling_qname > & )
    {
        set_qname( r_v_in, r_value_in );
    }
    void v_to_o(
            tlmx_string & r_v_in,
            const std::string & r_value_in,
            const ct_typed_validation_spec< tlmx_string > &,
            const c_handling< tlmx_string, c_handling_ur_type  > & )
    {
        r_v_in = r_value_in;    // No escaping
    }
    void v_to_o(
            c_empty &,
            const std::string &,
            const ct_typed_validation_spec< c_empty > &,
            const c_handling< c_empty, c_empty > & )
    {
        // No transfer operation
    }

    LMX_PDECL void set_code_file( const char * p_file_in ) { m_code_location.mp_file = p_file_in; }
    LMX_PDECL void set_code_line( int line_in ) { m_code_location.m_line = line_in; }

    // "Implementations"
    LMX_PDECL elmx_error unmarshal_attribute_value_impl(
                            c_untyped_unmarshal_bridge & r_bridge_in,
                            const c_untyped_validation_spec & r_item_spec_in );
    LMX_PDECL elmx_error unmarshal_attribute_list_value_impl(
                            c_untyped_unmarshal_list_bridge & r_bridge_in,
                            const c_untyped_validation_spec & r_item_spec_in,
                            tpf_list_lexical_validator pf_list_lexical_validator );
    LMX_PDECL elmx_error unmarshal_child_element_impl(
                            c_untyped_unmarshal_bridge & r_bridge_in,
                            const c_untyped_validation_spec & r_item_spec_in,
                            const s_event_map * p_event_map_in = LMXNULL );
    LMX_PDECL elmx_error unmarshal_element_value_impl(
                            c_untyped_unmarshal_bridge & r_bridge_in,
                            const c_untyped_validation_spec & r_item_spec_in );
    LMX_PDECL elmx_error unmarshal_child_element_list_impl(
                            c_untyped_unmarshal_list_bridge & r_bridge_in,
                            const c_untyped_validation_spec & r_item_spec_in,
                            tpf_list_lexical_validator pf_list_lexical_validator,
                            const s_event_map * p_event_map_in = LMXNULL );
    LMX_PDECL elmx_error unmarshal_element_list_value_impl(
                            c_untyped_unmarshal_list_bridge & r_bridge_in,
                            const c_untyped_validation_spec & r_item_spec_in,
                            tpf_list_lexical_validator pf_list_lexical_validator );

    template< typename Tcollection, typename Tvalidator >
    elmx_error unmarshal_attribute_value(
                            Tcollection & r_v_in,
                            const Tvalidator & r_item_spec_in )
    {
        return unmarshal_attribute_value_impl(
                                r_v_in.get_unmarshal_bridge( *this, r_item_spec_in ).base(),    // C121120_1245
                                r_item_spec_in );
    }
    template< typename Tcollection, typename Tvalidator >
    elmx_error unmarshal_attribute_list_value(
                            Tcollection & r_v_in,
                            const Tvalidator & r_item_spec_in,
                            tpf_list_lexical_validator pf_list_lexical_validator )
    {
        return unmarshal_attribute_list_value_impl(
                                r_v_in.get_unmarshal_list_bridge( *this, r_item_spec_in ).base(),   // C121120_1245
                                r_item_spec_in,
                                pf_list_lexical_validator );
    }
    template< typename Tcollection, typename Tvalidator >
    elmx_error unmarshal_child_element(
                            Tcollection & r_v_in,
                            const Tvalidator & r_item_spec_in,
                            const s_event_map * p_event_map_in = LMXNULL )    // C101015_1032
    {
        return unmarshal_child_element_impl(
                                r_v_in.get_unmarshal_bridge( *this, r_item_spec_in ).base(),    // C121120_1245
                                r_item_spec_in,
                                p_event_map_in );
    }
    template< typename Tcollection, typename Tvalidator >
    elmx_error unmarshal_element_value(
                            Tcollection & r_v_in,
                            const Tvalidator & r_item_spec_in )    // C101015_1032
    {
        return unmarshal_element_value_impl(
                                r_v_in.get_unmarshal_bridge( *this, r_item_spec_in ).base(),    // C121120_1245
                                r_item_spec_in );
    }
    template< typename Tcollection, typename Tvalidator >
    elmx_error unmarshal_child_element_list(
                            Tcollection & r_v_in,
                            const Tvalidator & r_item_spec_in,
                            tpf_list_lexical_validator pf_list_lexical_validator,
                            const s_event_map * p_event_map_in = LMXNULL )    // C101015_1032
    {
        return unmarshal_child_element_list_impl(
                                r_v_in.get_unmarshal_list_bridge( *this, r_item_spec_in ).base(),   // C121120_1245
                                r_item_spec_in,
                                pf_list_lexical_validator,
                                p_event_map_in );
    }
    template< typename Tcollection, typename Tvalidator >
    elmx_error unmarshal_element_list_value(
                            Tcollection & r_v_in,
                            const Tvalidator & r_item_spec_in,
                            tpf_list_lexical_validator pf_list_lexical_validator )    // C101015_1032
    {
        return unmarshal_element_list_value_impl(
                                r_v_in.get_unmarshal_list_bridge( *this, r_item_spec_in ).base(),   // C121120_1245
                                r_item_spec_in,
                                pf_list_lexical_validator );
    }

    template< typename Tcollection, typename Tvalidator >
    elmx_error unmarshal_mixed_element_value(
                            Tcollection & r_v_in,
                            const Tvalidator & LMX_UNUSED_PARAM( r_item_spec_in ) )
    {
        elmx_error error;
        if( ! get_nested_element_value( &value, &name ) && (error = handle_error( ELMX_UNABLE_TO_READ_ELEMENT_VALUE )) != ELMX_OK )
            return error;
        r_v_in.mk_new_space();
        r_v_in.get_new_space_value_ref() = value;
        return ELMX_OK;
    }
    template< typename Tcollection >
    elmx_error unmarshal_any_child_element(
                            Tcollection & r_v_in,
                            const s_event_map * p_event_map_in = LMXNULL )
    {
        elmx_error error = ELMX_OK;
        if( ! get_any_element( /*in*/name, &value ) && (error = handle_error( ELMX_UNABLE_TO_READ_ELEMENT_VALUE )) != ELMX_OK )
            return error;
        std::string namespace_uri;
        get_namespace( name, &namespace_uri );
        r_v_in.mk_new_space();
        typename Tcollection::t_atomic_type * p_v = &r_v_in.get_new_space_value_ref();
        p_v->set_namespace( namespace_uri );
        p_v->set_name( name );
        p_v->set_value( value );
        get_reader_local()->extract_namespace_context( &p_v->get_namespace_context(), *this );
        if( p_event_map_in )
            get_element_event( p_event_map_in, &error, &name );
        return error;
    }

    template< typename Tcollection >
    elmx_error list_size_check( Tcollection & r_v_in, size_t min_len_in, size_t max_len_in )
    {
        elmx_error error;
        if( r_v_in.new_list_size() < min_len_in && (error = handle_error( ELMX_LENGTH_TOO_SHORT )) != ELMX_OK )
            return error;
        if( r_v_in.new_list_size() > max_len_in && (error = handle_error( ELMX_LENGTH_TOO_LONG )) != ELMX_OK )
            return error;
        return ELMX_OK;
    }
    template< typename Tcollection >
    elmx_error list_size_check( Tcollection & r_v_in, size_t min_len_in )
    {
        elmx_error error;
        if( r_v_in.new_list_size() < min_len_in && (error = handle_error( ELMX_LENGTH_TOO_SHORT )) != ELMX_OK )
            return error;
        return ELMX_OK;
    }

    // Deprecated - For backwards compatibility - these methods are no longer preferred
    bool is_tokenisable( t_xml_event event_in ) const { return event_in == EXE_TOKEN || event_in == EXE_UNKNOWN || event_in >= EXE_LAST_XML_EVENT; }
    t_xml_event tokenise( const s_event_map c_event_map_in[], const std::string & r_name_in, elxp_attr_elem type_in );
    t_xml_event get_attribute_event( const s_event_map c_event_map_in[], std::string * p_name_out );
    t_xml_event get_element_event( const s_event_map c_event_map_in[], std::string * p_name_out );
    bool get_end_tag( const s_event_map c_event_map_in[], const std::string & r_name_in,
                        t_xml_event * p_event_in, elmx_error * p_error_out );
};

class c_xml_reader_file : public c_xml_reader
{
private:
    c_read_file m_low_reader;

public:
    LMX_PDECL c_xml_reader_file( const char * p_file_in ) :
                    c_xml_reader( m_low_reader ),
                    m_low_reader( p_file_in )
    {
    }
    LMX_PDECL c_xml_reader_file( const std::string & r_file_in ) :
                    c_xml_reader( m_low_reader ),
                    m_low_reader( r_file_in.c_str() )
    {
    }
#if LMX_WANT_WIDE_FILE_NAMES == 1
    LMX_PDECL c_xml_reader_file( const wchar_t * p_file_in ) :
                    c_xml_reader( m_low_reader ),
                    m_low_reader( p_file_in )
    {
    }
    LMX_PDECL c_xml_reader_file( const std::wstring & r_file_in ) :
                    c_xml_reader( m_low_reader ),
                    m_low_reader( r_file_in.c_str() )
    {
    }
#endif
    LMX_PDECL c_xml_reader_file( LMXSTDCRT::FILE *h_fin_in ) :
                    c_xml_reader( m_low_reader ),
                    m_low_reader( h_fin_in )
    {
    }
    LMX_PDECL ~c_xml_reader_file() {}
    LMX_PDECL bool is_open() const
    {
        return m_low_reader.is_open();
    }
};

class c_xml_reader_memory : public c_xml_reader
{
private:
    c_read_memory m_low_reader;

public:
    LMX_PDECL c_xml_reader_memory( const char * p_message_in, size_t message_length_in ) :
                    c_xml_reader( m_low_reader ),
                    m_low_reader( p_message_in, message_length_in )
    {
    }
    LMX_PDECL c_xml_reader_memory( const std::string & r_message_in ) :   // v4.1.2
                    c_xml_reader( m_low_reader ),
                    m_low_reader( r_message_in.c_str(), r_message_in.size() )
    {
    }
    LMX_PDECL ~c_xml_reader_memory() {}
    LMX_PDECL bool is_open() const
    {
        return m_low_reader.is_open();
    }
};

class c_xml_reader_string : public c_xml_reader_memory  // C190403_1342
{
public:
    c_xml_reader_string( const std::string & r_message_in ) : c_xml_reader_memory( r_message_in ) {}
};

class c_xml_reader_any_info : public c_xml_reader_string  // C190403_1342
{
public:
    c_xml_reader_any_info( const c_any_info & r_any_info_in )
        : c_xml_reader_string( r_any_info_in.get_value() )
    {
        add_namespace_mappings( r_any_info_in );
    }
};

//------------------------------------------------------------------------------
//                             c_unmarshal_helper class
//------------------------------------------------------------------------------

class c_unmarshal_helper
{
private:
    c_xml_reader & mr_reader;
    c_string_stack_node m_element_name_stack_node;
    const char * mp_cpp_file_name;
    int m_line_num;
    t_any_info_container * mp_anyattributes;
    bool m_can_body_be_empty;

private:
    c_unmarshal_helper( const c_unmarshal_helper & );           // Non-copiable/non-assignable
    c_unmarshal_helper & operator = ( const c_unmarshal_helper & );

public:
    LMX_PDECL c_unmarshal_helper( c_xml_reader & r_reader_in );
    LMX_PDECL virtual ~c_unmarshal_helper() {}

    LMX_PDECL void set_anyattributes_store( t_any_info_container * p_anyattributes_in ) { mp_anyattributes = p_anyattributes_in; }
    LMX_PDECL void set_body_can_be_empty() { m_can_body_be_empty = true; }

    LMX_PDECL elmx_error unmarshal_empty_content( const char c_cpp_file_name_in[], int line_num_in );
    LMX_PDECL elmx_error unmarshal_simple_content( const char c_cpp_file_name_in[], int line_num_in );
    LMX_PDECL elmx_error unmarshal_complex_content( const char c_cpp_file_name_in[], int line_num_in );

private:
    LMX_PDECL elmx_error unmarshal_all_attributes();
    LMX_PDECL elmx_error unmarshal_empty_content_case();
    LMX_PDECL elmx_error unmarshal_simple_content_case();
    LMX_PDECL elmx_error unmarshal_complex_content_case();
    LMX_PDECL elmx_error verify_element_end();

    LMX_PDECL virtual bool unmarshal_an_attribute(
                                    elmx_error * LMX_UNUSED_PARAM( p_error_out ) ) { return false; }
    LMX_PDECL virtual elmx_error unmarshal_attributes_check() { return ELMX_OK; }
    LMX_PDECL virtual bool unmarshal_simple_body( elmx_error * p_error_out );
    LMX_PDECL virtual bool unmarshal_complex_body( elmx_error * p_error_out );
    LMX_PDECL virtual bool is_nil() const { return false; }
    LMX_PDECL virtual bool is_not_choice_or_choice_is_not_set() const { return true; }
};

//------------------------------------------------------------------------------
//                               Writer class
//------------------------------------------------------------------------------

class c_xml_writer;

class c_xml_writer_local
{
private:
    c_xml_writer * mp_writer;
    c_xml_writer_local * mp_parent;
    bool m_has_attributes;
    bool m_has_body;
    bool m_has_children;

    c_xml_writer_local( const c_xml_writer_local & );
    c_xml_writer_local & operator = ( const c_xml_writer_local & );

public:
    LMX_PDECL explicit c_xml_writer_local();
    LMX_PDECL c_xml_writer_local( c_xml_writer & r_writer_in );
    LMX_PDECL void set_writer( c_xml_writer * p_writer_in );
    LMX_PDECL ~c_xml_writer_local();

    LMX_PDECL bool get_has_attributes() const { return m_has_attributes; }
    LMX_PDECL void set_has_attributes() { m_has_attributes = true; }
    LMX_PDECL bool get_has_body() const { return m_has_body; }
    LMX_PDECL void set_has_body() { m_has_body = true; }
    LMX_PDECL bool get_has_children() const { return m_has_children; }
    LMX_PDECL void set_parent_has_body() { if( mp_parent ) { mp_parent->m_has_body = mp_parent->m_has_children = true; } }
    LMX_PDECL bool get_parent_has_body() { if( mp_parent ) return mp_parent->m_has_body; return true; }
};

// C101014_1423
#if LMX_WRITER_LOCK_FORMAT == 0
    #define LMX_WRITER_DEFAULT_TAB_VAR m_default_tab
    #define LMX_WRITER_DEFAULT_NL_VAR m_default_nl
    #define LMX_WRITER_DEFAULT_ATTRIBUTE_TAB_VAR m_default_attribute_tab  /* C110426_1836 */
    #define LMX_WRITER_DEFAULT_ATTRIBUTE_NL_VAR m_default_attribute_nl
#else
    #define LMX_WRITER_DEFAULT_TAB_VAR LMX_WRITER_DEFAULT_TAB
    #define LMX_WRITER_DEFAULT_NL_VAR LMX_WRITER_DEFAULT_NL
    #define LMX_WRITER_DEFAULT_ATTRIBUTE_TAB_VAR LMX_WRITER_ATTRIBUTE_DEFAULT_TAB   /* C110426_1836 */
    #define LMX_WRITER_DEFAULT_ATTRIBUTE_NL_VAR LMX_WRITER_ATTRIBUTE_DEFAULT_NL
#endif

class c_xml_writer
{
public:
    typedef int t_writer_options;
    LMX_PDECL static const t_writer_options include_xml_decl;
    LMX_PDECL static const t_writer_options include_standalone;

protected:
    static std::string m_default_tab; // C101014_1423
    static std::string m_default_nl;
    static std::string m_default_attribute_tab; // C110426_1836
    static std::string m_default_attribute_nl;
    static t_writer_options m_convenience_options;  // C150616_1455

    std::ostream & m_os;
    c_xml_writer_local * mp_xml_writer_local;
    c_xml_writer_local m_bottom_xml_writer_local;
    int m_indent;
    std::string m_tab;    // C101014_1423
    std::string m_nl;
    std::string m_attribute_tab;  // C110426_1836
    std::string m_attribute_nl;
    bool m_is_ns_attr_written;
    c_error m_error;
    s_debug_error m_error_info;   // C100806_1136
    const s_ns_map * mp_ns_map_to_use;
    t_user_namespaces m_user_namespaces;
    t_schema_locations m_schema_locations;
    t_class_identity m_expected_class_identity;

private:
    c_xml_writer( const c_xml_writer & );       // No copy operations
    c_xml_writer & operator = ( const c_xml_writer & );

public:
    // User accessible functions
    LMX_PDECL static void set_default_tab( const std::string & r_default_tab_in ); // C101014_1423
    LMX_PDECL static const std::string & get_default_tab();
    LMX_PDECL static void set_default_nl( const std::string & r_default_nl_in );
    LMX_PDECL static const std::string & get_default_nl();
    LMX_PDECL static void set_default_attribute_tab( const std::string & r_default_attribute_tab_in ); // C110426_1836
    LMX_PDECL static const std::string & get_default_attribute_tab();
    LMX_PDECL static void set_default_attribute_nl( const std::string & r_default_attribute_nl_in );
    LMX_PDECL static const std::string & get_default_attribute_nl();
    LMX_PDECL static void set_convenience_options( t_writer_options options_in );   // C150616_1455
    LMX_PDECL static t_writer_options get_convenience_options();    // C150616_1455

    LMX_PDECL c_xml_writer(
                    std::ostream & r_os_in,
                    const char * p_tab_in = LMXNULL, const char * p_nl_in = LMXNULL,
                    const char * p_attribute_tab_in = LMXNULL, const char * p_attribute_nl_in = LMXNULL ); // C110426_1836
    LMX_PDECL c_xml_writer(
                    std::ostream & r_os_in,
                    t_writer_options options_in,
                    const char * p_tab_in = LMXNULL, const char * p_nl_in = LMXNULL,
                    const char * p_attribute_tab_in = LMXNULL, const char * p_attribute_nl_in = LMXNULL ); // C110426_1836
    LMX_PDECL virtual ~c_xml_writer()
    {
    }
    LMX_PDECL void add_namespace( const std::string & r_ns_prefix_in, const std::string & r_namespace_in );
    LMX_PDECL void add_default_namespace( const std::string & r_namespace_in )
    {
        add_namespace( "", r_namespace_in );
    }
    LMX_PDECL void add_schema_location( const std::string & r_ns_uri_in, const std::string & r_location_in ); // v3.1
    LMX_PDECL void add_no_namespace_schema_location( const std::string & r_location_in )   // v3.1
    {
        add_schema_location( "", r_location_in );
    }
    LMX_PDECL virtual elmx_error user_handle_error( elmx_error error_in ) { return error_in; }
    LMX_PDECL virtual elmx_error user_handle_error( elmx_error error_in, const std::string & r_item_name_in,
                                            const char * p_prog_file_in, int prog_line_in )
    {
        LMX_NO_WARN_UNUSED( r_item_name_in );
        LMX_NO_WARN_UNUSED( p_prog_file_in );
        LMX_NO_WARN_UNUSED( prog_line_in );

    #if defined( LMX_DEBUG_CAPTURE_ERROR ) && LMX_DEBUG_CAPTURE_ERROR == 1
        debug_error.set( error_in, r_item_name_in, p_prog_file_in, prog_line_in );
    #endif
    #if LMX_USE_EXCEPTIONS == 1
        if( error_in != ELMX_OK )    // Avoid any chance of an unreachable code warning
            throw c_lmx_exception( error_in );
    #endif
        return user_handle_error( error_in );    // For backwards compatibility
    }
    LMX_PDECL elmx_error capture_error( elmx_error error_in, const std::string & r_item_name_in, const char * p_prog_file_in, int prog_line_in );    // V4.2
    LMX_PDECL elmx_error handle_error( elmx_error error_in, const std::string & r_item_name_in, const char * p_prog_file_in, int prog_line_in )      // V4.2
    {
        error_in = capture_error( error_in, r_item_name_in, p_prog_file_in, prog_line_in  );   // Set before calling user_handle_error so user can call get_error_message
        return user_handle_error( error_in, r_item_name_in, p_prog_file_in, prog_line_in  );
    }
    LMX_PDECL virtual std::string & get_error_message( std::string * p_message_in ) const;
    LMX_PDECL std::string get_error_message() const;
    LMX_PDECL const s_debug_error & get_debug_error() const { return m_error_info; }
    LMX_PDECL const std::string & get_error_item_name() const { return m_error_info.item_name; }

    // Parser functions
    LMX_PDECL bool get_error_info( elmx_error_type * p_type_in, elmx_error * p_code_in, int * p_line_in )
    {
        return m_error.get_error_info( p_type_in, p_code_in, p_line_in );
    }

    LMX_PDECL std::ostream & operator()(bool do_indent_in=true)
    {
        if( do_indent_in )
            for( int i=0; i<m_indent; ++i )
                m_os << m_tab;
        return m_os;
    }
    LMX_PDECL void indent()
    {
        ++m_indent;
    }
    LMX_PDECL void undent()
    {
        if( m_indent > 0 )
            --m_indent;
    }
    LMX_PDECL void conditionally_select_ns_map( const s_ns_map * );
    LMX_PDECL void conditionally_write_ns_attrs( bool );
    LMX_PDECL void reenable_ns_map_output() { mp_ns_map_to_use = LMXNULL; m_is_ns_attr_written = false; }    // V3.8
    LMX_PDECL void disable_ns_map_output() { m_is_ns_attr_written = true; }    // C101029_0844

    // v5 formatter methods
    template< typename Tatomic_type, typename Thandling >  // C101015_1032
    bool is_value_empty( const Tatomic_type &, const c_handling< Tatomic_type, Thandling > & ) const
    {
        return false;
    }
    template< typename Tatomic_type, typename Thandling >  // C101015_1032
    void output_value( const Tatomic_type & r_value_in, const ct_typed_validation_spec< Tatomic_type > & r_item_spec_in, const c_handling< Tatomic_type, Thandling > & )
    {
        r_item_spec_in.o_to_v( m_os, r_value_in );
    }

    bool is_value_empty( const std::string & r_v_in, const c_handling< std::string, std::string > & ) const
    {
        return lmx::empty( r_v_in );
    }
    #ifndef LMX_NO_WSTRING  // C101110_1934
    bool is_value_empty( const std::wstring & r_v_in, const c_handling< std::wstring, std::wstring > & )
    {
        return lmx::empty( r_v_in );
    }
    #endif
    bool is_value_empty( const tc_binary & r_v_in, const c_handling< tc_binary, tc_binary > & ) const
    {
        return lmx::empty( r_v_in );
    }

    void output_value(
            const tc_qname & r_value_in, const ct_typed_validation_spec< tc_qname > &, const c_handling< tc_qname, c_handling_qname > & )
    {
        m_os << as_xml( make_qname_string( r_value_in ) );
    }

    bool is_value_empty( const c_empty &, const c_handling< c_empty, c_empty > & ) const
    {
        return true;
    }
    void output_value(
            const c_empty &, const ct_typed_validation_spec< c_empty > &, const c_handling< c_empty, c_empty > & )
    {
        // No output
    }

    bool is_value_empty( const tlmx_string & r_v_in, const c_handling< tlmx_string, c_handling_ur_type > & ) const
    {
        return lmx::empty( r_v_in );
    }
    void output_value(
            const tlmx_string & r_value_in, const ct_typed_validation_spec< tlmx_string > &, const c_handling< tlmx_string, c_handling_ur_type > & )
    {
        m_os << r_value_in;     // No escaping
    }

    LMX_PDECL c_xml_writer_local * get_xml_writer_local() { return mp_xml_writer_local; }
    LMX_PDECL void set_xml_writer_local( c_xml_writer_local * p_xml_writer_local_in ) { mp_xml_writer_local = p_xml_writer_local_in; }
    LMX_PDECL void start_element( const char c_name_in[] )
    {
        if( ! mp_xml_writer_local->get_parent_has_body() )
        {
            m_os << ">" << m_nl;
            mp_xml_writer_local->set_parent_has_body();
        }
        (*this)() << "<" << c_name_in;
        indent();
    }
    virtual void position_attribute()
    {
        if( ! mp_xml_writer_local->get_has_attributes()  || (m_attribute_nl.empty() && m_attribute_tab.empty()) )
        {
            m_os << " ";
            mp_xml_writer_local->set_has_attributes();
        }
        else if( ! m_attribute_nl.empty() )
        {
            m_os << m_attribute_nl;
            for( int i=0; i<m_indent + 1; ++i )
                m_os << m_attribute_tab;
        }
        else
            m_os << m_attribute_tab;
    }
    LMX_PDECL void prepare_for_simple_body_data()
    {
        if( ! mp_xml_writer_local->get_has_body() )
        {
            m_os << ">";
            mp_xml_writer_local->set_has_body();
        }
    }
    template< typename T >
    void element_body( const T & r_v_in )
    {
        prepare_for_simple_body_data();
        m_os << as_xml( r_v_in );
    }
    void element_body_any( const std::string & r_v_in )
    {
        prepare_for_simple_body_data();
        m_os << r_v_in;     // No escaping
    }
    LMX_PDECL void end_element( const char c_name_in[] )
    {
        undent();
        if( mp_xml_writer_local->get_has_body() )
        {
            if( mp_xml_writer_local->get_has_children() )
                (*this)();
            m_os << "</" << c_name_in << ">";
        }
        else
            m_os << "/>";
        m_os << m_nl;
    }

    LMX_PDECL void marshal_any_attribute( const t_any_info_container & r_v_in )
    {
        for( size_t i=0; i<r_v_in.size(); ++i )
        {
            position_attribute();
            m_os << r_v_in[i].get_name() << "=\"" << as_xml( r_v_in[i].get_value() ) << "\""; // C100702_1250
        }
    }

    template< typename T >      // C100920_1730
    void marshal_attribute( const char c_name_in[], const T & r_v_in )
    {
        position_attribute();
        m_os << c_name_in << "=\"" << as_xml( r_v_in ) << "\"";
    }
    template< typename T >      // C100920_1730
    void marshal_element( const char c_name_in[], const T & r_value_in )
    {
        c_xml_writer_local xml_writer_local( *this );
        start_element( c_name_in );
        if( ! is_value_empty( r_value_in, c_handling< T, T >() ) )
        {
            prepare_for_simple_body_data();
            m_os << as_xml( r_value_in );
        }
        end_element( c_name_in );
    }

    // "Implementations"
    LMX_PDECL void marshal_attribute_impl(
                        const char c_name_in[],
                        c_untyped_marshal_bridge & r_bridge_in );
    LMX_PDECL void marshal_attribute_or_default_impl(
                        const char c_name_in[],
                        c_untyped_marshal_bridge & r_bridge_in,
                        const c_untyped_validation_spec & r_item_spec_in );
    LMX_PDECL void marshal_attribute_list_impl(
                        const char c_name_in[],
                        c_untyped_marshal_list_bridge & r_bridge_in );
    LMX_PDECL void marshal_attribute_list_or_default_impl(
                        const char c_name_in[],
                        c_untyped_marshal_list_bridge & r_bridge_in,
                        const c_untyped_validation_spec & r_item_spec_in );
    LMX_PDECL void marshal_element_impl(       // For single simple global element case
                        const char c_name_in[],
                        c_untyped_marshal_bridge & r_bridge_in,
                        const lmx::s_ns_map * p_ns_map_writer_in = LMXNULL );
    LMX_PDECL void marshal_element_or_default_impl(
                        const char c_name_in[],
                        c_untyped_marshal_bridge & r_bridge_in,
                        const c_untyped_validation_spec & r_item_spec_in );
    LMX_PDECL void marshal_element_body_impl(
                        const char c_name_in[],
                        c_untyped_marshal_bridge & r_bridge_in );
    LMX_PDECL void marshal_element_list_impl(      // For single simple global element case
                        const char c_name_in[],
                        c_untyped_marshal_list_bridge & r_bridge_in,
                        const lmx::s_ns_map * p_ns_map_writer_in = LMXNULL );
    LMX_PDECL void marshal_element_list_or_default_impl(
                        const char c_name_in[],
                        c_untyped_marshal_list_bridge & r_bridge_in,
                        const c_untyped_validation_spec & r_item_spec_in );
    LMX_PDECL void marshal_element_body_list_impl(
                        const char c_name_in[],
                        c_untyped_marshal_list_bridge & r_bridge_in );

    template< typename Tcollection, typename Tvalidator >
    void marshal_attribute(
                        const char c_name_in[],
                        const Tcollection & r_v_in,
                        const Tvalidator & r_item_spec_in )
    {
        marshal_attribute_impl( c_name_in, r_v_in.get_marshal_bridge( *this, r_item_spec_in ).base() );   // C121120_1245
    }
    template< typename Tcollection, typename Tvalidator >
    void marshal_attribute_or_default(
                        const char c_name_in[],
                        const Tcollection & r_v_in,
                        const Tvalidator & r_item_spec_in )
    {
        marshal_attribute_or_default_impl( c_name_in, r_v_in.get_marshal_bridge( *this, r_item_spec_in ).base(), r_item_spec_in );  // C121120_1245
    }
    template< typename Tcollection, typename Tvalidator >
    void marshal_attribute_list(
                        const char c_name_in[],
                        const Tcollection & r_v_in,
                        const Tvalidator & r_item_spec_in )
    {
        marshal_attribute_list_impl( c_name_in, r_v_in.get_marshal_list_bridge( *this, r_item_spec_in ).base() ); // C121120_1245
    }
    template< typename Tcollection, typename Tvalidator >
    void marshal_attribute_list_or_default(
                        const char c_name_in[],
                        const Tcollection & r_v_in,
                        const Tvalidator & r_item_spec_in )
    {
        marshal_attribute_list_or_default_impl( c_name_in, r_v_in.get_marshal_list_bridge( *this, r_item_spec_in ).base(), r_item_spec_in );    // C121120_1245
    }
    template< typename Tcollection, typename Tvalidator >
    void marshal_element(       // For single simple global element case
                        const char c_name_in[],
                        const Tcollection & r_v_in,
                        const Tvalidator & r_item_spec_in,
                        const lmx::s_ns_map * p_ns_map_writer_in = LMXNULL )
    {
        marshal_element_impl( c_name_in, r_v_in.get_marshal_bridge( *this, r_item_spec_in ).base(), p_ns_map_writer_in );   // C121120_1245
    }
    template< typename Tcollection, typename Tvalidator >
    void marshal_element_or_default(
                        const char c_name_in[],
                        const Tcollection & r_v_in,
                        const Tvalidator & r_item_spec_in )
    {
        marshal_element_or_default_impl( c_name_in, r_v_in.get_marshal_bridge( *this, r_item_spec_in ).base(), r_item_spec_in );    // C121120_1245
    }
    template< typename Tcollection, typename Tvalidator >
    void marshal_element_body(
                        const char c_name_in[],
                        const Tcollection & r_v_in,
                        const Tvalidator & r_item_spec_in )
    {
        marshal_element_body_impl( c_name_in, r_v_in.get_marshal_bridge( *this, r_item_spec_in ).base() );    // C121120_1245
    }
    template< typename Tcollection, typename Tvalidator >
    void marshal_element_list(      // For single simple global element case
                        const char c_name_in[],
                        const Tcollection & r_v_in,
                        const Tvalidator & r_item_spec_in,
                        const lmx::s_ns_map * p_ns_map_writer_in = LMXNULL )
    {
        marshal_element_list_impl( c_name_in, r_v_in.get_marshal_list_bridge( *this, r_item_spec_in ).base(), p_ns_map_writer_in ); // C121120_1245
    }
    template< typename Tcollection, typename Tvalidator >
    void marshal_element_list_or_default(
                        const char c_name_in[],
                        const Tcollection & r_v_in,
                        const Tvalidator & r_item_spec_in )
    {
        marshal_element_list_or_default_impl( c_name_in, r_v_in.get_marshal_list_bridge( *this, r_item_spec_in ).base(), r_item_spec_in );  // C121120_1245
    }
    template< typename Tcollection, typename Tvalidator >
    void marshal_element_body_list(
                        const char c_name_in[],
                        const Tcollection & r_v_in,
                        const Tvalidator & r_item_spec_in )
    {
        marshal_element_body_list_impl( c_name_in, r_v_in.get_marshal_list_bridge( *this, r_item_spec_in ).base() );  // C121120_1245
    }

    template< typename Tcollection >
    void marshal_mixed_element_body(
                        const char c_name_in[],
                        const Tcollection & r_v_in )
    {
        LMX_NO_WARN_UNUSED( c_name_in );
        prepare_for_simple_body_data();
        m_os << r_v_in.access_value( 0 );   // No escaping
    }
    template< typename Tcollection >
    void marshal_any_element( const Tcollection & r_v_in )
    {
        for( size_t i=0; i<r_v_in.count(); ++i )
        {
            prepare_for_simple_body_data();
            (*this)() << r_v_in.access_value( i ).get_value() << m_nl;
        }
    }

    LMX_PDECL t_class_identity get_expected_class_identity() const
    {
        return m_expected_class_identity;
    }
    LMX_PDECL void set_expected_class_identity( t_class_identity expected_class_identity_in )
    {
        m_expected_class_identity = expected_class_identity_in;
    }
    LMX_PDECL void clear_expected_class_identity()
    {
        m_expected_class_identity = LMXNULL;
    }
    LMX_PDECL virtual void write_any_info( const c_any_info & r_any_info_in ) // v4.1.2
    {
        (*this)() << r_any_info_in.get_value() << m_nl;
    }

    // V3.2.4
    LMX_PDECL const char *get_namespace_prefix( const std::string & r_namespace_in ) const;
    // The make_qname_string functions create an un-escaped std::string.  This is returned
    // -in an instance of c_as_xml_string.  This causes the output converters
    // -to perform any necessary XML entity escaping.
    template< typename Tstring >
    c_as_xml_string make_qname_string( const ct_qname< Tstring > & r_qname_in ) const
    {
        const char * p_ns_prefix;
        if( r_qname_in.get_explicit_ns_prefix() != k_qname_user_explicit_ns_prefix_not_set )
            p_ns_prefix = r_qname_in.get_explicit_ns_prefix().c_str();   // V3.8
        else
        {
            p_ns_prefix = get_namespace_prefix( r_qname_in.get_namespace() );
            lmx_assert( p_ns_prefix );             // Assume user won't specify an unsupported namespace
            if( ! p_ns_prefix ) p_ns_prefix = ""; // Make it safe for Release mode (V3.7.3)
        }
        std::string qname( p_ns_prefix );
        if( ! qname.empty() )
            qname += ":";
        std::string narrow_string;
        convert( &narrow_string, r_qname_in.get_local_name() );
        qname += narrow_string;
        return c_as_xml_string( qname );
    }
    c_as_xml_string make_qname_string( const std::string & r_qname_in ) const
        { return c_as_xml_string( r_qname_in ); }
    #ifndef LMX_NO_WSTRING
    c_as_xml_string make_qname_string( const std::wstring & r_qname_in ) const
        { return c_as_xml_string( r_qname_in ); }
    #endif
};

//------------------------------------------------------------------------------
//                         Error code interpretation
//------------------------------------------------------------------------------

LMX_PDECL const char * get_error_description( elmx_error error_code_in );

//------------------------------------------------------------------------------
//                             Must Understand
//------------------------------------------------------------------------------

enum elmx_must_understand { EMU_OK, EMU_ERROR, EMU_MUST_UNDERSTAND };

typedef elmx_must_understand (*t_must_understand_test_function)( const std::string & r_value_in );

LMX_PDECL elmx_must_understand bool_must_understand( const std::string & r_value_in );

LMX_PDECL elmx_must_understand has_must_understand(
                                        c_xml_reader & r_reader_in,
                                        const std::string & r_namespace_in,
                                        const std::string & r_name_in,
                                        t_must_understand_test_function function_in = bool_must_understand );

//------------------------------------------------------------------------------
//                             get_instance_namespace
//------------------------------------------------------------------------------

LMX_PDECL bool get_instance_namespace( c_xml_reader & r_reader_in, std::string * p_namespace_out_in );

//------------------------------------------------------------------------------
//                Marshalling and Unmarshalling helper templates
//------------------------------------------------------------------------------

template< class T >
elmx_error marshal( const T & r_item_in, const char c_file_name_in[], s_debug_error * p_debug_error_out = LMXNULL ) // C110512_0912
{
    std::ofstream fout( c_file_name_in );
    if( ! fout.is_open() )
        return ELMX_NO_FILE;
    c_xml_writer writer( fout, c_xml_writer::get_convenience_options() );   // C150616_1455 - Using get_convenience_options()
    elmx_error error = r_item_in.marshal( writer );
#ifndef LMX_NO_REMOVE_FILE_ON_FAILED_MARSHAL
    if( error != ELMX_OK )
        LMXSTDCRT::remove( c_file_name_in );
#endif
    if( p_debug_error_out )        // C110512_0912
        *p_debug_error_out = writer.get_debug_error();
    return error;
}

#if defined( _MSC_VER ) && _MSC_VER >= 1400    // v3.10
template< class T >
elmx_error marshal( const T & r_item_in, const wchar_t c_file_name_in[], s_debug_error * p_debug_error_out = LMXNULL ) // C110512_0912
{
    std::ofstream fout( c_file_name_in );
    if( ! fout.is_open() )
        return ELMX_NO_FILE;
    c_xml_writer writer( fout, c_xml_writer::get_convenience_options() );   // C150616_1455 - Using get_convenience_options()
    elmx_error error = r_item_in.marshal( writer );
#ifndef LMX_NO_REMOVE_FILE_ON_FAILED_MARSHAL
    if( error != ELMX_OK )
        _wremove( c_file_name_in );
#endif
    if( p_debug_error_out )        // C110512_0912
        *p_debug_error_out = writer.get_debug_error();
    return error;
}
#endif

template< class T >
elmx_error marshal( const T & r_item_in, std::string * p_string_out, s_debug_error * p_debug_error_out = LMXNULL ) // C110512_0912
{
    lmx_assert( p_string_out );     // C160921_1705
    std::ostringstream sout;
    c_xml_writer writer( sout, c_xml_writer::get_convenience_options() );   // C150616_1455 - Using get_convenience_options()
    elmx_error error = r_item_in.marshal( writer );
    if( error == ELMX_OK )
        *p_string_out = sout.str();
    if( p_debug_error_out )        // C110512_0912
        *p_debug_error_out = writer.get_debug_error();
    return error;
}

template< class T >
elmx_error marshal( const T & r_item_in, std::ostream & r_sos_in, s_debug_error * p_debug_error_out = LMXNULL ) // C110512_0912
{
    c_xml_writer writer( r_sos_in, c_xml_writer::get_convenience_options() );   // C150616_1455 - Using get_convenience_options()
    elmx_error error = r_item_in.marshal( writer );
    if( p_debug_error_out )        // C110512_0912
        *p_debug_error_out = writer.get_debug_error();
    return error;
}

template< class T >
elmx_error unmarshal( T * p_item_in, const char c_file_name_in[], s_debug_error * p_debug_error_out = LMXNULL ) // C110512_0912
{
    c_xml_reader_file reader( c_file_name_in );
    if( ! reader.is_open() )
        return ELMX_NO_FILE;
    elmx_error error = p_item_in->unmarshal( reader );
    if( p_debug_error_out )        // C110512_0912
        *p_debug_error_out = reader.get_debug_error();
    return error;
}

#if LMX_WANT_WIDE_FILE_NAMES == 1     // v3.10
template< class T >
elmx_error unmarshal( T * p_item_in, const wchar_t c_file_name_in[], s_debug_error * p_debug_error_out = LMXNULL ) // C110512_0912
{
    c_xml_reader_file reader( c_file_name_in );
    if( ! reader.is_open() )
        return ELMX_NO_FILE;
    elmx_error error = p_item_in->unmarshal( reader );
    if( p_debug_error_out )        // C110512_0912
        *p_debug_error_out = reader.get_debug_error();
    return error;
}
#endif

template< class T >
elmx_error unmarshal( T * p_item_in, const char * p_memory_in, size_t memory_size_in, s_debug_error * p_debug_error_out = LMXNULL ) // C110512_0912
{
    c_xml_reader_memory reader( p_memory_in, memory_size_in );
    elmx_error error = p_item_in->unmarshal( reader );
    if( p_debug_error_out )        // C110512_0912
        *p_debug_error_out = reader.get_debug_error();
    return error;
}

template< class T >
elmx_error unmarshal( T * p_item_in, const std::string & r_string_in, s_debug_error * p_debug_error_out = LMXNULL ) // C110512_0912
{
    return unmarshal( p_item_in, r_string_in.data(), r_string_in.size(), p_debug_error_out );
}

template< class T > // Added v4.1.2
elmx_error unmarshal( T * p_item_in, const c_any_info & r_any_info_in, s_debug_error * p_debug_error_out = LMXNULL ) // C110512_0912
{
    c_xml_reader_any_info reader( r_any_info_in );  // C190403_1342
    elmx_error error = p_item_in->unmarshal( reader );
    if( p_debug_error_out )        // C110512_0912
        *p_debug_error_out = reader.get_debug_error();
    return error;
}

template< class T >
elmx_error unmarshal_partial(
        c_xml_reader & r_reader_in,
        T * p_item_in,
        const s_ns_map c_ns_map_in[],
        const std::string & r_namespace_in,
        const std::string & r_local_name_in,
        s_debug_error * p_debug_error_out = LMXNULL ) // C110512_0912
{
    r_reader_in.set_ns_map( c_ns_map_in );
    std::string name;
    elmx_error error = lmx::ELMX_OK;
    c_xml_reader_local reader_local( &r_reader_in );
    r_reader_in.get_element_event( LMXNULL, &error, &name );
    if( error != lmx::ELMX_OK && (error = r_reader_in.handle_error( error, __LINE__ )) != lmx::ELMX_OK )
        return error;
    std::string element_namespace;
    std::string element_local_name;
    r_reader_in.get_namespace( name, &element_namespace );
    r_reader_in.get_local_name( name, &element_local_name );
    if( (element_namespace != r_namespace_in || element_local_name != r_local_name_in) && (error = r_reader_in.handle_error( lmx::ELMX_ELEMENT_NOT_FOUND, __LINE__ )) != lmx::ELMX_OK )
        return error;
    return p_item_in->unmarshal( r_reader_in, name );
}

template< class T >
elmx_error unmarshal_partial(
        T * p_item_in,
        const char c_file_name_in[],
        const s_ns_map c_ns_map_in[],
        const std::string & r_namespace_in,
        const std::string & r_local_name_in,
        s_debug_error * p_debug_error_out = LMXNULL ) // C110512_0912
{
    c_xml_reader_file reader( c_file_name_in );
    if( ! reader.is_open() )
        return ELMX_NO_FILE;
    elmx_error error = unmarshal_partial( reader, p_item_in, c_ns_map_in, r_namespace_in, r_local_name_in );
    if( p_debug_error_out )        // C110512_0912
        *p_debug_error_out = reader.get_debug_error();
    return error;
}

#if LMX_WANT_WIDE_FILE_NAMES == 1
template< class T >
elmx_error unmarshal_partial(
        T * p_item_in,
        const wchar_t c_file_name_in[],
        const s_ns_map c_ns_map_in[],
        const std::string & r_namespace_in,
        const std::string & r_local_name_in,
        s_debug_error * p_debug_error_out = LMXNULL ) // C110512_0912
{
    c_xml_reader_file reader( c_file_name_in );
    if( ! reader.is_open() )
        return ELMX_NO_FILE;
    elmx_error error = unmarshal_partial( reader, p_item_in, c_ns_map_in, r_namespace_in, r_local_name_in );
    if( p_debug_error_out )        // C110512_0912
        *p_debug_error_out = reader.get_debug_error();
    return error;
}
#endif

template< class T >
elmx_error unmarshal_partial(
        T * p_item_in,
        const char * p_memory_in,
        size_t memory_size_in,
        const s_ns_map c_ns_map_in[],
        const std::string & r_namespace_in,
        const std::string & r_local_name_in,
        s_debug_error * p_debug_error_out = LMXNULL ) // C110512_0912
{
    c_xml_reader_memory reader( p_memory_in, memory_size_in );
    elmx_error error = unmarshal_partial( reader, p_item_in, c_ns_map_in, r_namespace_in, r_local_name_in );
    if( p_debug_error_out )        // C110512_0912
        *p_debug_error_out = reader.get_debug_error();
    return error;
}

template< class T >
elmx_error unmarshal_partial(
        T * p_item_in,
        const std::string & r_string_in,
        const s_ns_map c_ns_map_in[],
        const std::string & r_namespace_in,
        const std::string & r_local_name_in,
        s_debug_error * p_debug_error_out = LMXNULL ) // C110512_0912
{
    return unmarshal_partial( p_item_in, r_string_in.data(), r_string_in.size(), c_ns_map_in, r_namespace_in, r_local_name_in, p_debug_error_out );
}

template< class T >
bool /*is_found*/ p_unmarshal_find(
        c_xml_reader & r_reader_in,
        elmx_error * p_error_out,
        T * p_item_in,
        const std::string & r_sought_namespace_in,
        const std::string & r_sought_local_name_in,
        const std::string & r_element_name_in )
{
    std::string element_namespace;
    std::string element_local_name;
    r_reader_in.get_namespace( r_element_name_in, &element_namespace );
    r_reader_in.get_local_name( r_element_name_in, &element_local_name );
    if( element_local_name == r_sought_local_name_in && element_namespace == r_sought_namespace_in )
        {
        *p_error_out = p_item_in->unmarshal( r_reader_in, r_element_name_in );
        return true;
        }

    bool is_end_of_element;

    if( ! r_reader_in.skip_start_tag( &is_end_of_element ) )
    {
        *p_error_out = ELMX_BAD_END_OF_START_TAG;
        return false;
    }

    if( is_end_of_element )
        return false;

    std::string name;

    for(;;)
    {
        c_xml_reader_local reader_local( &r_reader_in );
        r_reader_in.get_element_event( LMXNULL, p_error_out, &name );

        if( *p_error_out != ELMX_OK )
            return false;
        else if( r_reader_in.get_current_event() == lmx::EXE_TOKEN )
        {
            if( p_unmarshal_find(
                    r_reader_in,
                    p_error_out,
                    p_item_in,
                    r_sought_namespace_in,
                    r_sought_local_name_in,
                    name ) )
                return true;
            else if( *p_error_out != ELMX_OK )
                return false;
        }
        else if( r_reader_in.get_current_event() == EXE_ELEMENT_END )
        {
            if( name != r_element_name_in )
                *p_error_out = ELMX_BAD_ELEMENT_END;
            return false;
        }
        else if( r_reader_in.get_current_event() == EXE_CDATA )
            {}  // No action
        else
        {
            *p_error_out = ELMX_BADLY_FORMED;
            return false;
        }
    }

    lmx_assert_code_not_reached();  // Shouldn't get to here
    return false;
}

template< class T >
bool /*is_found*/ unmarshal_find(
        c_xml_reader & r_reader_in,
        elmx_error * p_error_out,
        T * p_item_in,
        const s_ns_map c_ns_map_in[],
        const std::string & r_sought_namespace_in,
        const std::string & r_sought_local_name_in )
{
    r_reader_in.set_ns_map( c_ns_map_in );
    c_xml_reader_local reader_local( &r_reader_in );
    std::string name;
    r_reader_in.get_element_event( LMXNULL, p_error_out, &name );
    if( r_reader_in.get_current_event() != EXE_TOKEN )
        return false;
    *p_error_out = ELMX_OK;
    return p_unmarshal_find(
                r_reader_in,
                p_error_out,
                p_item_in,
                r_sought_namespace_in,
                r_sought_local_name_in,
                name );
}

template< class T >
bool /*is_found*/ unmarshal_find(
        elmx_error * p_error_out,
        T * p_item_in,
        const char c_file_name_in[],
        const s_ns_map c_ns_map_in[],
        const std::string & r_namespace_in,
        const std::string & r_local_name_in )
{
    c_xml_reader_file reader( c_file_name_in );
    if( ! reader.is_open() )
        return ELMX_NO_FILE;
    return unmarshal_find( reader, p_error_out, p_item_in, c_ns_map_in, r_namespace_in, r_local_name_in );
}

#if LMX_WANT_WIDE_FILE_NAMES == 1
template< class T >
bool /*is_found*/ unmarshal_find(
        elmx_error * p_error_out,
        T * p_item_in,
        const wchar_t c_file_name_in[],
        const s_ns_map c_ns_map_in[],
        const std::string & r_namespace_in,
        const std::string & r_local_name_in )
{
    c_xml_reader_file reader( c_file_name_in );
    if( ! reader.is_open() )
        return ELMX_NO_FILE;
    return unmarshal_find( reader, p_error_out, p_item_in, c_ns_map_in, r_namespace_in, r_local_name_in );
}
#endif

template< class T >
bool /*is_found*/ unmarshal_find(
        elmx_error * p_error_out,
        T * p_item_in,
        const char * p_memory_in,
        size_t memory_size_in,
        const s_ns_map c_ns_map_in[],
        const std::string & r_namespace_in,
        const std::string & r_local_name_in )
{
    c_xml_reader_memory reader( p_memory_in, memory_size_in );
    return unmarshal_find( reader, p_error_out, p_item_in, c_ns_map_in, r_namespace_in, r_local_name_in );
}

template< class T >
bool /*is_found*/ unmarshal_find(
        elmx_error * p_error_out,
        T * p_item_in,
        const std::string & r_string_in,
        const s_ns_map c_ns_map_in[],
        const std::string & r_namespace_in,
        const std::string & r_local_name_in )
{
    return unmarshal_find( p_error_out, p_item_in, r_string_in.data(), r_string_in.size(), c_ns_map_in, r_namespace_in, r_local_name_in );
}

//------------------------------------------------------------------------------
//                                 Miscellaneous
//------------------------------------------------------------------------------

LMX_PDECL void bswap( void * p_lhs_in, void * p_rhs_in, size_t size );

} // End of namepsace lmx

#if defined( _MSC_VER ) && _MSC_VER >= 1400
#pragma warning (pop)  // Restore VC8 'safe' warnings
#endif

#endif  // LMXPARSE_H

//------------------------------------------------------------------------------
//                   Copyright (c) 2003-2017 Codalogic Ltd.
//------------------------------------------------------------------------------
