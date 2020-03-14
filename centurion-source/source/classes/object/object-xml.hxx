// Copyright (c) 2005-2014 Code Synthesis Tools CC
//
// This program was generated by CodeSynthesis XSD, an XML Schema to
// C++ data binding compiler.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
// In addition, as a special exception, Code Synthesis Tools CC gives
// permission to link this program with the Xerces-C++ library (or with
// modified versions of Xerces-C++ that use the same license as Xerces-C++),
// and distribute linked combinations including the two. You must obey
// the GNU General Public License version 2 in all respects for all of
// the code used other than Xerces-C++. If you modify this copy of the
// program, you may extend this exception to your version of the program,
// but you are not obligated to do so. If you do not wish to do so, delete
// this exception statement from your version.
//
// Furthermore, Code Synthesis Tools CC makes a special exception for
// the Free/Libre and Open Source Software (FLOSS) which is described
// in the accompanying FLOSSE file.
//

#ifndef OBJECT_HXX
#define OBJECT_HXX

#ifndef XSD_USE_CHAR
#define XSD_USE_CHAR
#endif

#ifndef XSD_CXX_TREE_USE_CHAR
#define XSD_CXX_TREE_USE_CHAR
#endif

// Begin prologue.
//
//
// End prologue.

#include <xsd/cxx/config.hxx>

#if (XSD_INT_VERSION != 4000000L)
#error XSD runtime version mismatch
#endif

#include <xsd/cxx/pre.hxx>

#include <xsd/cxx/xml/char-utf8.hxx>

#include <xsd/cxx/tree/exceptions.hxx>
#include <xsd/cxx/tree/elements.hxx>
#include <xsd/cxx/tree/types.hxx>

#include <xsd/cxx/xml/error-handler.hxx>

#include <xsd/cxx/xml/dom/auto-ptr.hxx>

#include <xsd/cxx/tree/parsing.hxx>
#include <xsd/cxx/tree/parsing/byte.hxx>
#include <xsd/cxx/tree/parsing/unsigned-byte.hxx>
#include <xsd/cxx/tree/parsing/short.hxx>
#include <xsd/cxx/tree/parsing/unsigned-short.hxx>
#include <xsd/cxx/tree/parsing/int.hxx>
#include <xsd/cxx/tree/parsing/unsigned-int.hxx>
#include <xsd/cxx/tree/parsing/long.hxx>
#include <xsd/cxx/tree/parsing/unsigned-long.hxx>
#include <xsd/cxx/tree/parsing/boolean.hxx>
#include <xsd/cxx/tree/parsing/float.hxx>
#include <xsd/cxx/tree/parsing/double.hxx>
#include <xsd/cxx/tree/parsing/decimal.hxx>

namespace xml_schema
{
  // anyType and anySimpleType.
  //
  typedef ::xsd::cxx::tree::type type;
  typedef ::xsd::cxx::tree::simple_type< char, type > simple_type;
  typedef ::xsd::cxx::tree::type container;

  // 8-bit
  //
  typedef signed char byte;
  typedef unsigned char unsigned_byte;

  // 16-bit
  //
  typedef short short_;
  typedef unsigned short unsigned_short;

  // 32-bit
  //
  typedef int int_;
  typedef unsigned int unsigned_int;

  // 64-bit
  //
  typedef long long long_;
  typedef unsigned long long unsigned_long;

  // Supposed to be arbitrary-length integral types.
  //
  typedef long long integer;
  typedef long long non_positive_integer;
  typedef unsigned long long non_negative_integer;
  typedef unsigned long long positive_integer;
  typedef long long negative_integer;

  // Boolean.
  //
  typedef bool boolean;

  // Floating-point types.
  //
  typedef float float_;
  typedef double double_;
  typedef double decimal;

  // String types.
  //
  typedef ::xsd::cxx::tree::string< char, simple_type > string;
  typedef ::xsd::cxx::tree::normalized_string< char, string > normalized_string;
  typedef ::xsd::cxx::tree::token< char, normalized_string > token;
  typedef ::xsd::cxx::tree::name< char, token > name;
  typedef ::xsd::cxx::tree::nmtoken< char, token > nmtoken;
  typedef ::xsd::cxx::tree::nmtokens< char, simple_type, nmtoken > nmtokens;
  typedef ::xsd::cxx::tree::ncname< char, name > ncname;
  typedef ::xsd::cxx::tree::language< char, token > language;

  // ID/IDREF.
  //
  typedef ::xsd::cxx::tree::id< char, ncname > id;
  typedef ::xsd::cxx::tree::idref< char, ncname, type > idref;
  typedef ::xsd::cxx::tree::idrefs< char, simple_type, idref > idrefs;

  // URI.
  //
  typedef ::xsd::cxx::tree::uri< char, simple_type > uri;

  // Qualified name.
  //
  typedef ::xsd::cxx::tree::qname< char, simple_type, uri, ncname > qname;

  // Binary.
  //
  typedef ::xsd::cxx::tree::buffer< char > buffer;
  typedef ::xsd::cxx::tree::base64_binary< char, simple_type > base64_binary;
  typedef ::xsd::cxx::tree::hex_binary< char, simple_type > hex_binary;

  // Date/time.
  //
  typedef ::xsd::cxx::tree::time_zone time_zone;
  typedef ::xsd::cxx::tree::date< char, simple_type > date;
  typedef ::xsd::cxx::tree::date_time< char, simple_type > date_time;
  typedef ::xsd::cxx::tree::duration< char, simple_type > duration;
  typedef ::xsd::cxx::tree::gday< char, simple_type > gday;
  typedef ::xsd::cxx::tree::gmonth< char, simple_type > gmonth;
  typedef ::xsd::cxx::tree::gmonth_day< char, simple_type > gmonth_day;
  typedef ::xsd::cxx::tree::gyear< char, simple_type > gyear;
  typedef ::xsd::cxx::tree::gyear_month< char, simple_type > gyear_month;
  typedef ::xsd::cxx::tree::time< char, simple_type > time;

  // Entity.
  //
  typedef ::xsd::cxx::tree::entity< char, ncname > entity;
  typedef ::xsd::cxx::tree::entities< char, simple_type, entity > entities;

  typedef ::xsd::cxx::tree::content_order content_order;
  // Flags and properties.
  //
  typedef ::xsd::cxx::tree::flags flags;
  typedef ::xsd::cxx::tree::properties< char > properties;

  // Parsing/serialization diagnostics.
  //
  typedef ::xsd::cxx::tree::severity severity;
  typedef ::xsd::cxx::tree::error< char > error;
  typedef ::xsd::cxx::tree::diagnostics< char > diagnostics;

  // Exceptions.
  //
  typedef ::xsd::cxx::tree::exception< char > exception;
  typedef ::xsd::cxx::tree::bounds< char > bounds;
  typedef ::xsd::cxx::tree::duplicate_id< char > duplicate_id;
  typedef ::xsd::cxx::tree::parsing< char > parsing;
  typedef ::xsd::cxx::tree::expected_element< char > expected_element;
  typedef ::xsd::cxx::tree::unexpected_element< char > unexpected_element;
  typedef ::xsd::cxx::tree::expected_attribute< char > expected_attribute;
  typedef ::xsd::cxx::tree::unexpected_enumerator< char > unexpected_enumerator;
  typedef ::xsd::cxx::tree::expected_text_content< char > expected_text_content;
  typedef ::xsd::cxx::tree::no_prefix_mapping< char > no_prefix_mapping;

  // Error handler callback interface.
  //
  typedef ::xsd::cxx::xml::error_handler< char > error_handler;

  // DOM interaction.
  //
  namespace dom
  {
    // Automatic pointer for DOMDocument.
    //
    using ::xsd::cxx::xml::dom::auto_ptr;

#ifndef XSD_CXX_TREE_TREE_NODE_KEY__XML_SCHEMA
#define XSD_CXX_TREE_TREE_NODE_KEY__XML_SCHEMA
    // DOM user data key for back pointers to tree nodes.
    //
    const XMLCh* const tree_node_key = ::xsd::cxx::tree::user_data_keys::node;
#endif
  }
}

// Forward declarations.
//
class c_object;
class properties;
class methods;
class sounds;
class property;
class method;
class sound;

#include <memory>    // ::std::auto_ptr
#include <limits>    // std::numeric_limits
#include <algorithm> // std::binary_search

#include <xsd/cxx/xml/char-utf8.hxx>

#include <xsd/cxx/tree/exceptions.hxx>
#include <xsd/cxx/tree/elements.hxx>
#include <xsd/cxx/tree/containers.hxx>
#include <xsd/cxx/tree/list.hxx>

#include <xsd/cxx/xml/dom/parsing-header.hxx>

class c_object: public ::xml_schema::type
{
  public:
  // properties
  //
  typedef ::properties properties_type;
  typedef ::xsd::cxx::tree::traits< properties_type, char > properties_traits;

  const properties_type&
  properties () const;

  properties_type&
  properties ();

  void
  properties (const properties_type& x);

  void
  properties (::std::auto_ptr< properties_type > p);

  // methods
  //
  typedef ::methods methods_type;
  typedef ::xsd::cxx::tree::traits< methods_type, char > methods_traits;

  const methods_type&
  methods () const;

  methods_type&
  methods ();

  void
  methods (const methods_type& x);

  void
  methods (::std::auto_ptr< methods_type > p);

  // sounds
  //
  typedef ::sounds sounds_type;
  typedef ::xsd::cxx::tree::traits< sounds_type, char > sounds_traits;

  const sounds_type&
  sounds () const;

  sounds_type&
  sounds ();

  void
  sounds (const sounds_type& x);

  void
  sounds (::std::auto_ptr< sounds_type > p);

  // parent
  //
  typedef ::xml_schema::string parent_type;
  typedef ::xsd::cxx::tree::traits< parent_type, char > parent_traits;

  const parent_type&
  parent () const;

  parent_type&
  parent ();

  void
  parent (const parent_type& x);

  void
  parent (::std::auto_ptr< parent_type > p);

  // type
  //
  typedef ::xml_schema::string type_type;
  typedef ::xsd::cxx::tree::traits< type_type, char > type_traits;

  const type_type&
  type () const;

  type_type&
  type ();

  void
  type (const type_type& x);

  void
  type (::std::auto_ptr< type_type > p);

  // class_name
  //
  typedef ::xml_schema::string class_name_type;
  typedef ::xsd::cxx::tree::traits< class_name_type, char > class_name_traits;

  const class_name_type&
  class_name () const;

  class_name_type&
  class_name ();

  void
  class_name (const class_name_type& x);

  void
  class_name (::std::auto_ptr< class_name_type > p);

  // Constructors.
  //
  c_object (const properties_type&,
            const methods_type&,
            const sounds_type&,
            const parent_type&,
            const type_type&,
            const class_name_type&);

  c_object (::std::auto_ptr< properties_type >,
            ::std::auto_ptr< methods_type >,
            ::std::auto_ptr< sounds_type >,
            const parent_type&,
            const type_type&,
            const class_name_type&);

  c_object (const ::xercesc::DOMElement& e,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  c_object (const c_object& x,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  virtual c_object*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  c_object&
  operator= (const c_object& x);

  virtual 
  ~c_object ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< properties_type > properties_;
  ::xsd::cxx::tree::one< methods_type > methods_;
  ::xsd::cxx::tree::one< sounds_type > sounds_;
  ::xsd::cxx::tree::one< parent_type > parent_;
  ::xsd::cxx::tree::one< type_type > type_;
  ::xsd::cxx::tree::one< class_name_type > class_name_;
};

class properties: public ::xml_schema::type
{
  public:
  // property
  //
  typedef ::property property_type;
  typedef ::xsd::cxx::tree::sequence< property_type > property_sequence;
  typedef property_sequence::iterator property_iterator;
  typedef property_sequence::const_iterator property_const_iterator;
  typedef ::xsd::cxx::tree::traits< property_type, char > property_traits;

  const property_sequence&
  property () const;

  property_sequence&
  property ();

  void
  property (const property_sequence& s);

  // Constructors.
  //
  properties ();

  properties (const ::xercesc::DOMElement& e,
              ::xml_schema::flags f = 0,
              ::xml_schema::container* c = 0);

  properties (const properties& x,
              ::xml_schema::flags f = 0,
              ::xml_schema::container* c = 0);

  virtual properties*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  properties&
  operator= (const properties& x);

  virtual 
  ~properties ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  property_sequence property_;
};

class methods: public ::xml_schema::type
{
  public:
  // method
  //
  typedef ::method method_type;
  typedef ::xsd::cxx::tree::sequence< method_type > method_sequence;
  typedef method_sequence::iterator method_iterator;
  typedef method_sequence::const_iterator method_const_iterator;
  typedef ::xsd::cxx::tree::traits< method_type, char > method_traits;

  const method_sequence&
  method () const;

  method_sequence&
  method ();

  void
  method (const method_sequence& s);

  // Constructors.
  //
  methods ();

  methods (const ::xercesc::DOMElement& e,
           ::xml_schema::flags f = 0,
           ::xml_schema::container* c = 0);

  methods (const methods& x,
           ::xml_schema::flags f = 0,
           ::xml_schema::container* c = 0);

  virtual methods*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  methods&
  operator= (const methods& x);

  virtual 
  ~methods ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  method_sequence method_;
};

class sounds: public ::xml_schema::type
{
  public:
  // sound
  //
  typedef ::sound sound_type;
  typedef ::xsd::cxx::tree::sequence< sound_type > sound_sequence;
  typedef sound_sequence::iterator sound_iterator;
  typedef sound_sequence::const_iterator sound_const_iterator;
  typedef ::xsd::cxx::tree::traits< sound_type, char > sound_traits;

  const sound_sequence&
  sound () const;

  sound_sequence&
  sound ();

  void
  sound (const sound_sequence& s);

  // Constructors.
  //
  sounds ();

  sounds (const ::xercesc::DOMElement& e,
          ::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0);

  sounds (const sounds& x,
          ::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0);

  virtual sounds*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  sounds&
  operator= (const sounds& x);

  virtual 
  ~sounds ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  sound_sequence sound_;
};

class property: public ::xml_schema::type
{
  public:
  // name
  //
  typedef ::xml_schema::string name_type;
  typedef ::xsd::cxx::tree::traits< name_type, char > name_traits;

  const name_type&
  name () const;

  name_type&
  name ();

  void
  name (const name_type& x);

  void
  name (::std::auto_ptr< name_type > p);

  // value
  //
  typedef ::xml_schema::string value_type;
  typedef ::xsd::cxx::tree::traits< value_type, char > value_traits;

  const value_type&
  value () const;

  value_type&
  value ();

  void
  value (const value_type& x);

  void
  value (::std::auto_ptr< value_type > p);

  // Constructors.
  //
  property (const name_type&,
            const value_type&);

  property (const ::xercesc::DOMElement& e,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  property (const property& x,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  virtual property*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  property&
  operator= (const property& x);

  virtual 
  ~property ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< name_type > name_;
  ::xsd::cxx::tree::one< value_type > value_;
};

class method: public ::xml_schema::type
{
  public:
  // name
  //
  typedef ::xml_schema::string name_type;
  typedef ::xsd::cxx::tree::traits< name_type, char > name_traits;

  const name_type&
  name () const;

  name_type&
  name ();

  void
  name (const name_type& x);

  void
  name (::std::auto_ptr< name_type > p);

  // script
  //
  typedef ::xml_schema::string script_type;
  typedef ::xsd::cxx::tree::traits< script_type, char > script_traits;

  const script_type&
  script () const;

  script_type&
  script ();

  void
  script (const script_type& x);

  void
  script (::std::auto_ptr< script_type > p);

  // Constructors.
  //
  method (const name_type&,
          const script_type&);

  method (const ::xercesc::DOMElement& e,
          ::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0);

  method (const method& x,
          ::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0);

  virtual method*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  method&
  operator= (const method& x);

  virtual 
  ~method ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< name_type > name_;
  ::xsd::cxx::tree::one< script_type > script_;
};

class sound: public ::xml_schema::type
{
  public:
  // name
  //
  typedef ::xml_schema::string name_type;
  typedef ::xsd::cxx::tree::traits< name_type, char > name_traits;

  const name_type&
  name () const;

  name_type&
  name ();

  void
  name (const name_type& x);

  void
  name (::std::auto_ptr< name_type > p);

  // path
  //
  typedef ::xml_schema::string path_type;
  typedef ::xsd::cxx::tree::traits< path_type, char > path_traits;

  const path_type&
  path () const;

  path_type&
  path ();

  void
  path (const path_type& x);

  void
  path (::std::auto_ptr< path_type > p);

  // Constructors.
  //
  sound (const name_type&,
         const path_type&);

  sound (const ::xercesc::DOMElement& e,
         ::xml_schema::flags f = 0,
         ::xml_schema::container* c = 0);

  sound (const sound& x,
         ::xml_schema::flags f = 0,
         ::xml_schema::container* c = 0);

  virtual sound*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  sound&
  operator= (const sound& x);

  virtual 
  ~sound ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< name_type > name_;
  ::xsd::cxx::tree::one< path_type > path_;
};

#include <iosfwd>

#include <xercesc/sax/InputSource.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>

// Parse a URI or a local file.
//

::std::auto_ptr< ::c_object >
c_object_ (const ::std::string& uri,
           ::xml_schema::flags f = 0,
           const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::c_object >
c_object_ (const ::std::string& uri,
           ::xml_schema::error_handler& eh,
           ::xml_schema::flags f = 0,
           const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::c_object >
c_object_ (const ::std::string& uri,
           ::xercesc::DOMErrorHandler& eh,
           ::xml_schema::flags f = 0,
           const ::xml_schema::properties& p = ::xml_schema::properties ());

// Parse std::istream.
//

::std::auto_ptr< ::c_object >
c_object_ (::std::istream& is,
           ::xml_schema::flags f = 0,
           const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::c_object >
c_object_ (::std::istream& is,
           ::xml_schema::error_handler& eh,
           ::xml_schema::flags f = 0,
           const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::c_object >
c_object_ (::std::istream& is,
           ::xercesc::DOMErrorHandler& eh,
           ::xml_schema::flags f = 0,
           const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::c_object >
c_object_ (::std::istream& is,
           const ::std::string& id,
           ::xml_schema::flags f = 0,
           const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::c_object >
c_object_ (::std::istream& is,
           const ::std::string& id,
           ::xml_schema::error_handler& eh,
           ::xml_schema::flags f = 0,
           const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::c_object >
c_object_ (::std::istream& is,
           const ::std::string& id,
           ::xercesc::DOMErrorHandler& eh,
           ::xml_schema::flags f = 0,
           const ::xml_schema::properties& p = ::xml_schema::properties ());

// Parse xercesc::InputSource.
//

::std::auto_ptr< ::c_object >
c_object_ (::xercesc::InputSource& is,
           ::xml_schema::flags f = 0,
           const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::c_object >
c_object_ (::xercesc::InputSource& is,
           ::xml_schema::error_handler& eh,
           ::xml_schema::flags f = 0,
           const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::c_object >
c_object_ (::xercesc::InputSource& is,
           ::xercesc::DOMErrorHandler& eh,
           ::xml_schema::flags f = 0,
           const ::xml_schema::properties& p = ::xml_schema::properties ());

// Parse xercesc::DOMDocument.
//

::std::auto_ptr< ::c_object >
c_object_ (const ::xercesc::DOMDocument& d,
           ::xml_schema::flags f = 0,
           const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::c_object >
c_object_ (::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d,
           ::xml_schema::flags f = 0,
           const ::xml_schema::properties& p = ::xml_schema::properties ());

#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
//
// End epilogue.

#endif // OBJECT_HXX