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

#ifndef CHARACTERS_HXX
#define CHARACTERS_HXX

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
class chars;
class char_;

#include <memory>    // ::std::auto_ptr
#include <limits>    // std::numeric_limits
#include <algorithm> // std::binary_search

#include <xsd/cxx/xml/char-utf8.hxx>

#include <xsd/cxx/tree/exceptions.hxx>
#include <xsd/cxx/tree/elements.hxx>
#include <xsd/cxx/tree/containers.hxx>
#include <xsd/cxx/tree/list.hxx>

#include <xsd/cxx/xml/dom/parsing-header.hxx>

class chars: public ::xml_schema::type
{
  public:
  // char
  //
  typedef ::char_ char_type;
  typedef ::xsd::cxx::tree::sequence< char_type > char_sequence;
  typedef char_sequence::iterator char_iterator;
  typedef char_sequence::const_iterator char_const_iterator;
  typedef ::xsd::cxx::tree::traits< char_type, char > char_traits;

  const char_sequence&
  char_ () const;

  char_sequence&
  char_ ();

  void
  char_ (const char_sequence& s);

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

  // Constructors.
  //
  chars (const name_type&);

  chars (const ::xercesc::DOMElement& e,
         ::xml_schema::flags f = 0,
         ::xml_schema::container* c = 0);

  chars (const chars& x,
         ::xml_schema::flags f = 0,
         ::xml_schema::container* c = 0);

  virtual chars*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  chars&
  operator= (const chars& x);

  virtual 
  ~chars ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  char_sequence char__;
  ::xsd::cxx::tree::one< name_type > name_;
};

class char_: public ::xml_schema::type
{
  public:
  // id
  //
  typedef ::xml_schema::integer id_type;
  typedef ::xsd::cxx::tree::traits< id_type, char > id_traits;

  const id_type&
  id () const;

  id_type&
  id ();

  void
  id (const id_type& x);

  // x
  //
  typedef ::xml_schema::integer x_type;
  typedef ::xsd::cxx::tree::traits< x_type, char > x_traits;

  const x_type&
  x () const;

  x_type&
  x ();

  void
  x (const x_type& x);

  // y
  //
  typedef ::xml_schema::integer y_type;
  typedef ::xsd::cxx::tree::traits< y_type, char > y_traits;

  const y_type&
  y () const;

  y_type&
  y ();

  void
  y (const y_type& x);

  // width
  //
  typedef ::xml_schema::integer width_type;
  typedef ::xsd::cxx::tree::traits< width_type, char > width_traits;

  const width_type&
  width () const;

  width_type&
  width ();

  void
  width (const width_type& x);

  // height
  //
  typedef ::xml_schema::integer height_type;
  typedef ::xsd::cxx::tree::traits< height_type, char > height_traits;

  const height_type&
  height () const;

  height_type&
  height ();

  void
  height (const height_type& x);

  // xoffset
  //
  typedef ::xml_schema::integer xoffset_type;
  typedef ::xsd::cxx::tree::traits< xoffset_type, char > xoffset_traits;

  const xoffset_type&
  xoffset () const;

  xoffset_type&
  xoffset ();

  void
  xoffset (const xoffset_type& x);

  // yoffset
  //
  typedef ::xml_schema::integer yoffset_type;
  typedef ::xsd::cxx::tree::traits< yoffset_type, char > yoffset_traits;

  const yoffset_type&
  yoffset () const;

  yoffset_type&
  yoffset ();

  void
  yoffset (const yoffset_type& x);

  // xadvance
  //
  typedef ::xml_schema::integer xadvance_type;
  typedef ::xsd::cxx::tree::traits< xadvance_type, char > xadvance_traits;

  const xadvance_type&
  xadvance () const;

  xadvance_type&
  xadvance ();

  void
  xadvance (const xadvance_type& x);

  // lineheight
  //
  typedef ::xml_schema::integer lineheight_type;
  typedef ::xsd::cxx::tree::traits< lineheight_type, char > lineheight_traits;

  const lineheight_type&
  lineheight () const;

  lineheight_type&
  lineheight ();

  void
  lineheight (const lineheight_type& x);

  // Constructors.
  //
  char_ (const id_type&,
         const x_type&,
         const y_type&,
         const width_type&,
         const height_type&,
         const xoffset_type&,
         const yoffset_type&,
         const xadvance_type&,
         const lineheight_type&);

  char_ (const ::xercesc::DOMElement& e,
         ::xml_schema::flags f = 0,
         ::xml_schema::container* c = 0);

  char_ (const char_& x,
         ::xml_schema::flags f = 0,
         ::xml_schema::container* c = 0);

  virtual char_*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  char_&
  operator= (const char_& x);

  virtual 
  ~char_ ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< id_type > id_;
  ::xsd::cxx::tree::one< x_type > x_;
  ::xsd::cxx::tree::one< y_type > y_;
  ::xsd::cxx::tree::one< width_type > width_;
  ::xsd::cxx::tree::one< height_type > height_;
  ::xsd::cxx::tree::one< xoffset_type > xoffset_;
  ::xsd::cxx::tree::one< yoffset_type > yoffset_;
  ::xsd::cxx::tree::one< xadvance_type > xadvance_;
  ::xsd::cxx::tree::one< lineheight_type > lineheight_;
};

#include <iosfwd>

#include <xercesc/sax/InputSource.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>

// Parse a URI or a local file.
//

::std::auto_ptr< ::chars >
chars_ (const ::std::string& uri,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::chars >
chars_ (const ::std::string& uri,
        ::xml_schema::error_handler& eh,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::chars >
chars_ (const ::std::string& uri,
        ::xercesc::DOMErrorHandler& eh,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

// Parse std::istream.
//

::std::auto_ptr< ::chars >
chars_ (::std::istream& is,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::chars >
chars_ (::std::istream& is,
        ::xml_schema::error_handler& eh,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::chars >
chars_ (::std::istream& is,
        ::xercesc::DOMErrorHandler& eh,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::chars >
chars_ (::std::istream& is,
        const ::std::string& id,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::chars >
chars_ (::std::istream& is,
        const ::std::string& id,
        ::xml_schema::error_handler& eh,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::chars >
chars_ (::std::istream& is,
        const ::std::string& id,
        ::xercesc::DOMErrorHandler& eh,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

// Parse xercesc::InputSource.
//

::std::auto_ptr< ::chars >
chars_ (::xercesc::InputSource& is,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::chars >
chars_ (::xercesc::InputSource& is,
        ::xml_schema::error_handler& eh,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::chars >
chars_ (::xercesc::InputSource& is,
        ::xercesc::DOMErrorHandler& eh,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

// Parse xercesc::DOMDocument.
//

::std::auto_ptr< ::chars >
chars_ (const ::xercesc::DOMDocument& d,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::chars >
chars_ (::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
//
// End epilogue.

#endif // CHARACTERS_HXX
