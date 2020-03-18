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

#ifndef EDITOR_WINDOWS_HXX
#define EDITOR_WINDOWS_HXX

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
class c_editorWindows;
class c_editorWindow;
class conditionScript;
class simpleText;
class scriptText;
class image;
class button;
class textList;
class color;
class text;
class text_background;

#include <memory>    // ::std::auto_ptr
#include <limits>    // std::numeric_limits
#include <algorithm> // std::binary_search

#include <xsd/cxx/xml/char-utf8.hxx>

#include <xsd/cxx/tree/exceptions.hxx>
#include <xsd/cxx/tree/elements.hxx>
#include <xsd/cxx/tree/containers.hxx>
#include <xsd/cxx/tree/list.hxx>

#include <xsd/cxx/xml/dom/parsing-header.hxx>

class c_editorWindows : public ::xml_schema::type
{
public:
	// c_editorWindow
	//
	typedef ::c_editorWindow c_editorWindow_type;
	typedef ::xsd::cxx::tree::sequence< c_editorWindow_type > c_editorWindow_sequence;
	typedef c_editorWindow_sequence::iterator c_editorWindow_iterator;
	typedef c_editorWindow_sequence::const_iterator c_editorWindow_const_iterator;
	typedef ::xsd::cxx::tree::traits< c_editorWindow_type, char > c_editorWindow_traits;

	const c_editorWindow_sequence&
		c_editorWindow() const;

	c_editorWindow_sequence&
		c_editorWindow();

	void
		c_editorWindow(const c_editorWindow_sequence& s);

	// Constructors.
	//
	c_editorWindows();

	c_editorWindows(const ::xercesc::DOMElement& e,
		::xml_schema::flags f = 0,
		::xml_schema::container* c = 0);

	c_editorWindows(const c_editorWindows& x,
		::xml_schema::flags f = 0,
		::xml_schema::container* c = 0);

	virtual c_editorWindows*
		_clone(::xml_schema::flags f = 0,
			::xml_schema::container* c = 0) const;

	c_editorWindows&
		operator= (const c_editorWindows& x);

	virtual
		~c_editorWindows();

	// Implementation.
	//
protected:
	void
		parse(::xsd::cxx::xml::dom::parser< char >&,
			::xml_schema::flags);

protected:
	c_editorWindow_sequence c_editorWindow_;
};

class c_editorWindow : public ::xml_schema::type
{
public:
	// conditionScript
	//
	typedef ::conditionScript conditionScript_type;
	typedef ::xsd::cxx::tree::traits< conditionScript_type, char > conditionScript_traits;

	const conditionScript_type&
		conditionScript() const;

	conditionScript_type&
		conditionScript();

	void
		conditionScript(const conditionScript_type& x);

	void
		conditionScript(::std::auto_ptr< conditionScript_type > p);

	// openingScript
	//
	typedef ::xml_schema::string openingScript_type;
	typedef ::xsd::cxx::tree::traits< openingScript_type, char > openingScript_traits;

	const openingScript_type&
		openingScript() const;

	openingScript_type&
		openingScript();

	void
		openingScript(const openingScript_type& x);

	void
		openingScript(::std::auto_ptr< openingScript_type > p);

	// simpleText
	//
	typedef ::simpleText simpleText_type;
	typedef ::xsd::cxx::tree::sequence< simpleText_type > simpleText_sequence;
	typedef simpleText_sequence::iterator simpleText_iterator;
	typedef simpleText_sequence::const_iterator simpleText_const_iterator;
	typedef ::xsd::cxx::tree::traits< simpleText_type, char > simpleText_traits;

	const simpleText_sequence&
		simpleText() const;

	simpleText_sequence&
		simpleText();

	void
		simpleText(const simpleText_sequence& s);

	// scriptText
	//
	typedef ::scriptText scriptText_type;
	typedef ::xsd::cxx::tree::optional< scriptText_type > scriptText_optional;
	typedef ::xsd::cxx::tree::traits< scriptText_type, char > scriptText_traits;

	const scriptText_optional&
		scriptText() const;

	scriptText_optional&
		scriptText();

	void
		scriptText(const scriptText_type& x);

	void
		scriptText(const scriptText_optional& x);

	void
		scriptText(::std::auto_ptr< scriptText_type > p);

	// image
	//
	typedef ::image image_type;
	typedef ::xsd::cxx::tree::sequence< image_type > image_sequence;
	typedef image_sequence::iterator image_iterator;
	typedef image_sequence::const_iterator image_const_iterator;
	typedef ::xsd::cxx::tree::traits< image_type, char > image_traits;

	const image_sequence&
		image() const;

	image_sequence&
		image();

	void
		image(const image_sequence& s);

	// button
	//
	typedef ::button button_type;
	typedef ::xsd::cxx::tree::sequence< button_type > button_sequence;
	typedef button_sequence::iterator button_iterator;
	typedef button_sequence::const_iterator button_const_iterator;
	typedef ::xsd::cxx::tree::traits< button_type, char > button_traits;

	const button_sequence&
		button() const;

	button_sequence&
		button();

	void
		button(const button_sequence& s);

	// textList
	//
	typedef ::textList textList_type;
	typedef ::xsd::cxx::tree::sequence< textList_type > textList_sequence;
	typedef textList_sequence::iterator textList_iterator;
	typedef textList_sequence::const_iterator textList_const_iterator;
	typedef ::xsd::cxx::tree::traits< textList_type, char > textList_traits;

	const textList_sequence&
		textList() const;

	textList_sequence&
		textList();

	void
		textList(const textList_sequence& s);

	// id
	//
	typedef ::xml_schema::integer id_type;
	typedef ::xsd::cxx::tree::traits< id_type, char > id_traits;

	const id_type&
		id() const;

	id_type&
		id();

	void
		id(const id_type& x);

	// name
	//
	typedef ::xml_schema::string name_type;
	typedef ::xsd::cxx::tree::traits< name_type, char > name_traits;

	const name_type&
		name() const;

	name_type&
		name();

	void
		name(const name_type& x);

	void
		name(::std::auto_ptr< name_type > p);

	// iframe
	//
	typedef ::xml_schema::string iframe_type;
	typedef ::xsd::cxx::tree::traits< iframe_type, char > iframe_traits;

	const iframe_type&
		iframe() const;

	iframe_type&
		iframe();

	void
		iframe(const iframe_type& x);

	void
		iframe(::std::auto_ptr< iframe_type > p);

	// x
	//
	typedef ::xml_schema::integer x_type;
	typedef ::xsd::cxx::tree::traits< x_type, char > x_traits;

	const x_type&
		x() const;

	x_type&
		x();

	void
		x(const x_type& x);

	// y
	//
	typedef ::xml_schema::integer y_type;
	typedef ::xsd::cxx::tree::traits< y_type, char > y_traits;

	const y_type&
		y() const;

	y_type&
		y();

	void
		y(const y_type& x);

	// width
	//
	typedef ::xml_schema::integer width_type;
	typedef ::xsd::cxx::tree::traits< width_type, char > width_traits;

	const width_type&
		width() const;

	width_type&
		width();

	void
		width(const width_type& x);

	// height
	//
	typedef ::xml_schema::integer height_type;
	typedef ::xsd::cxx::tree::traits< height_type, char > height_traits;

	const height_type&
		height() const;

	height_type&
		height();

	void
		height(const height_type& x);

	// isOpened
	//
	typedef ::xml_schema::boolean isOpened_type;
	typedef ::xsd::cxx::tree::traits< isOpened_type, char > isOpened_traits;

	const isOpened_type&
		isOpened() const;

	isOpened_type&
		isOpened();

	void
		isOpened(const isOpened_type& x);

	// Constructors.
	//
	c_editorWindow(const conditionScript_type&,
		const openingScript_type&,
		const id_type&,
		const name_type&,
		const iframe_type&,
		const x_type&,
		const y_type&,
		const width_type&,
		const height_type&,
		const isOpened_type&);

	c_editorWindow(::std::auto_ptr< conditionScript_type >,
		const openingScript_type&,
		const id_type&,
		const name_type&,
		const iframe_type&,
		const x_type&,
		const y_type&,
		const width_type&,
		const height_type&,
		const isOpened_type&);

	c_editorWindow(const ::xercesc::DOMElement& e,
		::xml_schema::flags f = 0,
		::xml_schema::container* c = 0);

	c_editorWindow(const c_editorWindow& x,
		::xml_schema::flags f = 0,
		::xml_schema::container* c = 0);

	virtual c_editorWindow*
		_clone(::xml_schema::flags f = 0,
			::xml_schema::container* c = 0) const;

	c_editorWindow&
		operator= (const c_editorWindow& x);

	virtual
		~c_editorWindow();

	// Implementation.
	//
protected:
	void
		parse(::xsd::cxx::xml::dom::parser< char >&,
			::xml_schema::flags);

protected:
	::xsd::cxx::tree::one< conditionScript_type > conditionScript_;
	::xsd::cxx::tree::one< openingScript_type > openingScript_;
	simpleText_sequence simpleText_;
	scriptText_optional scriptText_;
	image_sequence image_;
	button_sequence button_;
	textList_sequence textList_;
	::xsd::cxx::tree::one< id_type > id_;
	::xsd::cxx::tree::one< name_type > name_;
	::xsd::cxx::tree::one< iframe_type > iframe_;
	::xsd::cxx::tree::one< x_type > x_;
	::xsd::cxx::tree::one< y_type > y_;
	::xsd::cxx::tree::one< width_type > width_;
	::xsd::cxx::tree::one< height_type > height_;
	::xsd::cxx::tree::one< isOpened_type > isOpened_;
};

class conditionScript : public ::xml_schema::string
{
public:
	// function
	//
	typedef ::xml_schema::string function_type;
	typedef ::xsd::cxx::tree::traits< function_type, char > function_traits;

	const function_type&
		function() const;

	function_type&
		function();

	void
		function(const function_type& x);

	void
		function(::std::auto_ptr< function_type > p);

	// Constructors.
	//
	conditionScript(const function_type&);

	conditionScript(const char*,
		const function_type&);

	conditionScript(const ::std::string&,
		const function_type&);

	conditionScript(const ::xml_schema::string&,
		const function_type&);

	conditionScript(const ::xercesc::DOMElement& e,
		::xml_schema::flags f = 0,
		::xml_schema::container* c = 0);

	conditionScript(const conditionScript& x,
		::xml_schema::flags f = 0,
		::xml_schema::container* c = 0);

	virtual conditionScript*
		_clone(::xml_schema::flags f = 0,
			::xml_schema::container* c = 0) const;

	conditionScript&
		operator= (const conditionScript& x);

	virtual
		~conditionScript();

	// Implementation.
	//
protected:
	void
		parse(::xsd::cxx::xml::dom::parser< char >&,
			::xml_schema::flags);

protected:
	::xsd::cxx::tree::one< function_type > function_;
};

class simpleText : public ::xml_schema::type
{
public:
	// color
	//
	typedef ::color color_type;
	typedef ::xsd::cxx::tree::traits< color_type, char > color_traits;

	const color_type&
		color() const;

	color_type&
		color();

	void
		color(const color_type& x);

	void
		color(::std::auto_ptr< color_type > p);

	// text
	//
	typedef ::xml_schema::string text_type;
	typedef ::xsd::cxx::tree::traits< text_type, char > text_traits;

	const text_type&
		text() const;

	text_type&
		text();

	void
		text(const text_type& x);

	void
		text(::std::auto_ptr< text_type > p);

	// xOffset
	//
	typedef ::xml_schema::integer xOffset_type;
	typedef ::xsd::cxx::tree::traits< xOffset_type, char > xOffset_traits;

	const xOffset_type&
		xOffset() const;

	xOffset_type&
		xOffset();

	void
		xOffset(const xOffset_type& x);

	// yOffset
	//
	typedef ::xml_schema::integer yOffset_type;
	typedef ::xsd::cxx::tree::traits< yOffset_type, char > yOffset_traits;

	const yOffset_type&
		yOffset() const;

	yOffset_type&
		yOffset();

	void
		yOffset(const yOffset_type& x);

	// font
	//
	typedef ::xml_schema::string font_type;
	typedef ::xsd::cxx::tree::traits< font_type, char > font_traits;

	const font_type&
		font() const;

	font_type&
		font();

	void
		font(const font_type& x);

	void
		font(::std::auto_ptr< font_type > p);

	// Constructors.
	//
	simpleText(const color_type&,
		const text_type&,
		const xOffset_type&,
		const yOffset_type&,
		const font_type&);

	simpleText(::std::auto_ptr< color_type >,
		const text_type&,
		const xOffset_type&,
		const yOffset_type&,
		const font_type&);

	simpleText(const ::xercesc::DOMElement& e,
		::xml_schema::flags f = 0,
		::xml_schema::container* c = 0);

	simpleText(const simpleText& x,
		::xml_schema::flags f = 0,
		::xml_schema::container* c = 0);

	virtual simpleText*
		_clone(::xml_schema::flags f = 0,
			::xml_schema::container* c = 0) const;

	simpleText&
		operator= (const simpleText& x);

	virtual
		~simpleText();

	// Implementation.
	//
protected:
	void
		parse(::xsd::cxx::xml::dom::parser< char >&,
			::xml_schema::flags);

protected:
	::xsd::cxx::tree::one< color_type > color_;
	::xsd::cxx::tree::one< text_type > text_;
	::xsd::cxx::tree::one< xOffset_type > xOffset_;
	::xsd::cxx::tree::one< yOffset_type > yOffset_;
	::xsd::cxx::tree::one< font_type > font_;
};

class scriptText : public ::xml_schema::type
{
public:
	// color
	//
	typedef ::color color_type;
	typedef ::xsd::cxx::tree::traits< color_type, char > color_traits;

	const color_type&
		color() const;

	color_type&
		color();

	void
		color(const color_type& x);

	void
		color(::std::auto_ptr< color_type > p);

	// src
	//
	typedef ::xml_schema::string src_type;
	typedef ::xsd::cxx::tree::traits< src_type, char > src_traits;

	const src_type&
		src() const;

	src_type&
		src();

	void
		src(const src_type& x);

	void
		src(::std::auto_ptr< src_type > p);

	// xOffset
	//
	typedef ::xml_schema::integer xOffset_type;
	typedef ::xsd::cxx::tree::traits< xOffset_type, char > xOffset_traits;

	const xOffset_type&
		xOffset() const;

	xOffset_type&
		xOffset();

	void
		xOffset(const xOffset_type& x);

	// yOffset
	//
	typedef ::xml_schema::integer yOffset_type;
	typedef ::xsd::cxx::tree::traits< yOffset_type, char > yOffset_traits;

	const yOffset_type&
		yOffset() const;

	yOffset_type&
		yOffset();

	void
		yOffset(const yOffset_type& x);

	// font
	//
	typedef ::xml_schema::string font_type;
	typedef ::xsd::cxx::tree::traits< font_type, char > font_traits;

	const font_type&
		font() const;

	font_type&
		font();

	void
		font(const font_type& x);

	void
		font(::std::auto_ptr< font_type > p);

	// Constructors.
	//
	scriptText(const color_type&,
		const src_type&,
		const xOffset_type&,
		const yOffset_type&,
		const font_type&);

	scriptText(::std::auto_ptr< color_type >,
		const src_type&,
		const xOffset_type&,
		const yOffset_type&,
		const font_type&);

	scriptText(const ::xercesc::DOMElement& e,
		::xml_schema::flags f = 0,
		::xml_schema::container* c = 0);

	scriptText(const scriptText& x,
		::xml_schema::flags f = 0,
		::xml_schema::container* c = 0);

	virtual scriptText*
		_clone(::xml_schema::flags f = 0,
			::xml_schema::container* c = 0) const;

	scriptText&
		operator= (const scriptText& x);

	virtual
		~scriptText();

	// Implementation.
	//
protected:
	void
		parse(::xsd::cxx::xml::dom::parser< char >&,
			::xml_schema::flags);

protected:
	::xsd::cxx::tree::one< color_type > color_;
	::xsd::cxx::tree::one< src_type > src_;
	::xsd::cxx::tree::one< xOffset_type > xOffset_;
	::xsd::cxx::tree::one< yOffset_type > yOffset_;
	::xsd::cxx::tree::one< font_type > font_;
};

class image : public ::xml_schema::type
{
public:
	// image_name
	//
	typedef ::xml_schema::string image_name_type;
	typedef ::xsd::cxx::tree::traits< image_name_type, char > image_name_traits;

	const image_name_type&
		image_name() const;

	image_name_type&
		image_name();

	void
		image_name(const image_name_type& x);

	void
		image_name(::std::auto_ptr< image_name_type > p);

	// xOffset
	//
	typedef ::xml_schema::integer xOffset_type;
	typedef ::xsd::cxx::tree::traits< xOffset_type, char > xOffset_traits;

	const xOffset_type&
		xOffset() const;

	xOffset_type&
		xOffset();

	void
		xOffset(const xOffset_type& x);

	// yOffset
	//
	typedef ::xml_schema::integer yOffset_type;
	typedef ::xsd::cxx::tree::traits< yOffset_type, char > yOffset_traits;

	const yOffset_type&
		yOffset() const;

	yOffset_type&
		yOffset();

	void
		yOffset(const yOffset_type& x);

	// onclick
	//
	typedef ::xml_schema::string onclick_type;
	typedef ::xsd::cxx::tree::traits< onclick_type, char > onclick_traits;

	const onclick_type&
		onclick() const;

	onclick_type&
		onclick();

	void
		onclick(const onclick_type& x);

	void
		onclick(::std::auto_ptr< onclick_type > p);

	// Constructors.
	//
	image(const image_name_type&,
		const xOffset_type&,
		const yOffset_type&,
		const onclick_type&);

	image(const ::xercesc::DOMElement& e,
		::xml_schema::flags f = 0,
		::xml_schema::container* c = 0);

	image(const image& x,
		::xml_schema::flags f = 0,
		::xml_schema::container* c = 0);

	virtual image*
		_clone(::xml_schema::flags f = 0,
			::xml_schema::container* c = 0) const;

	image&
		operator= (const image& x);

	virtual
		~image();

	// Implementation.
	//
protected:
	void
		parse(::xsd::cxx::xml::dom::parser< char >&,
			::xml_schema::flags);

protected:
	::xsd::cxx::tree::one< image_name_type > image_name_;
	::xsd::cxx::tree::one< xOffset_type > xOffset_;
	::xsd::cxx::tree::one< yOffset_type > yOffset_;
	::xsd::cxx::tree::one< onclick_type > onclick_;
};

class button : public ::xml_schema::type
{
public:
	// name
	//
	typedef ::xml_schema::string name_type;
	typedef ::xsd::cxx::tree::traits< name_type, char > name_traits;

	const name_type&
		name() const;

	name_type&
		name();

	void
		name(const name_type& x);

	void
		name(::std::auto_ptr< name_type > p);

	// image_name
	//
	typedef ::xml_schema::string image_name_type;
	typedef ::xsd::cxx::tree::traits< image_name_type, char > image_name_traits;

	const image_name_type&
		image_name() const;

	image_name_type&
		image_name();

	void
		image_name(const image_name_type& x);

	void
		image_name(::std::auto_ptr< image_name_type > p);

	// xOffset
	//
	typedef ::xml_schema::integer xOffset_type;
	typedef ::xsd::cxx::tree::traits< xOffset_type, char > xOffset_traits;

	const xOffset_type&
		xOffset() const;

	xOffset_type&
		xOffset();

	void
		xOffset(const xOffset_type& x);

	// yOffset
	//
	typedef ::xml_schema::integer yOffset_type;
	typedef ::xsd::cxx::tree::traits< yOffset_type, char > yOffset_traits;

	const yOffset_type&
		yOffset() const;

	yOffset_type&
		yOffset();

	void
		yOffset(const yOffset_type& x);

	// onclick
	//
	typedef ::xml_schema::string onclick_type;
	typedef ::xsd::cxx::tree::traits< onclick_type, char > onclick_traits;

	const onclick_type&
		onclick() const;

	onclick_type&
		onclick();

	void
		onclick(const onclick_type& x);

	void
		onclick(::std::auto_ptr< onclick_type > p);

	// Constructors.
	//
	button(const name_type&,
		const image_name_type&,
		const xOffset_type&,
		const yOffset_type&,
		const onclick_type&);

	button(const ::xercesc::DOMElement& e,
		::xml_schema::flags f = 0,
		::xml_schema::container* c = 0);

	button(const button& x,
		::xml_schema::flags f = 0,
		::xml_schema::container* c = 0);

	virtual button*
		_clone(::xml_schema::flags f = 0,
			::xml_schema::container* c = 0) const;

	button&
		operator= (const button& x);

	virtual
		~button();

	// Implementation.
	//
protected:
	void
		parse(::xsd::cxx::xml::dom::parser< char >&,
			::xml_schema::flags);

protected:
	::xsd::cxx::tree::one< name_type > name_;
	::xsd::cxx::tree::one< image_name_type > image_name_;
	::xsd::cxx::tree::one< xOffset_type > xOffset_;
	::xsd::cxx::tree::one< yOffset_type > yOffset_;
	::xsd::cxx::tree::one< onclick_type > onclick_;
};

class textList : public ::xml_schema::type
{
public:
	// text
	//
	typedef ::text text_type;
	typedef ::xsd::cxx::tree::traits< text_type, char > text_traits;

	const text_type&
		text() const;

	text_type&
		text();

	void
		text(const text_type& x);

	void
		text(::std::auto_ptr< text_type > p);

	// text_background
	//
	typedef ::text_background text_background_type;
	typedef ::xsd::cxx::tree::traits< text_background_type, char > text_background_traits;

	const text_background_type&
		text_background() const;

	text_background_type&
		text_background();

	void
		text_background(const text_background_type& x);

	void
		text_background(::std::auto_ptr< text_background_type > p);

	// textListId
	//
	typedef ::xml_schema::integer textListId_type;
	typedef ::xsd::cxx::tree::traits< textListId_type, char > textListId_traits;

	const textListId_type&
		textListId() const;

	textListId_type&
		textListId();

	void
		textListId(const textListId_type& x);

	// xOffset
	//
	typedef ::xml_schema::integer xOffset_type;
	typedef ::xsd::cxx::tree::traits< xOffset_type, char > xOffset_traits;

	const xOffset_type&
		xOffset() const;

	xOffset_type&
		xOffset();

	void
		xOffset(const xOffset_type& x);

	// yOffset
	//
	typedef ::xml_schema::integer yOffset_type;
	typedef ::xsd::cxx::tree::traits< yOffset_type, char > yOffset_traits;

	const yOffset_type&
		yOffset() const;

	yOffset_type&
		yOffset();

	void
		yOffset(const yOffset_type& x);

	// Constructors.
	//
	textList(const text_type&,
		const text_background_type&,
		const textListId_type&,
		const xOffset_type&,
		const yOffset_type&);

	textList(::std::auto_ptr< text_type >,
		::std::auto_ptr< text_background_type >,
		const textListId_type&,
		const xOffset_type&,
		const yOffset_type&);

	textList(const ::xercesc::DOMElement& e,
		::xml_schema::flags f = 0,
		::xml_schema::container* c = 0);

	textList(const textList& x,
		::xml_schema::flags f = 0,
		::xml_schema::container* c = 0);

	virtual textList*
		_clone(::xml_schema::flags f = 0,
			::xml_schema::container* c = 0) const;

	textList&
		operator= (const textList& x);

	virtual
		~textList();

	// Implementation.
	//
protected:
	void
		parse(::xsd::cxx::xml::dom::parser< char >&,
			::xml_schema::flags);

protected:
	::xsd::cxx::tree::one< text_type > text_;
	::xsd::cxx::tree::one< text_background_type > text_background_;
	::xsd::cxx::tree::one< textListId_type > textListId_;
	::xsd::cxx::tree::one< xOffset_type > xOffset_;
	::xsd::cxx::tree::one< yOffset_type > yOffset_;
};

class color : public ::xml_schema::type
{
public:
	// r
	//
	typedef ::xml_schema::integer r_type;
	typedef ::xsd::cxx::tree::traits< r_type, char > r_traits;

	const r_type&
		r() const;

	r_type&
		r();

	void
		r(const r_type& x);

	// g
	//
	typedef ::xml_schema::integer g_type;
	typedef ::xsd::cxx::tree::traits< g_type, char > g_traits;

	const g_type&
		g() const;

	g_type&
		g();

	void
		g(const g_type& x);

	// b
	//
	typedef ::xml_schema::integer b_type;
	typedef ::xsd::cxx::tree::traits< b_type, char > b_traits;

	const b_type&
		b() const;

	b_type&
		b();

	void
		b(const b_type& x);

	// Constructors.
	//
	color(const r_type&,
		const g_type&,
		const b_type&);

	color(const ::xercesc::DOMElement& e,
		::xml_schema::flags f = 0,
		::xml_schema::container* c = 0);

	color(const color& x,
		::xml_schema::flags f = 0,
		::xml_schema::container* c = 0);

	virtual color*
		_clone(::xml_schema::flags f = 0,
			::xml_schema::container* c = 0) const;

	color&
		operator= (const color& x);

	virtual
		~color();

	// Implementation.
	//
protected:
	void
		parse(::xsd::cxx::xml::dom::parser< char >&,
			::xml_schema::flags);

protected:
	::xsd::cxx::tree::one< r_type > r_;
	::xsd::cxx::tree::one< g_type > g_;
	::xsd::cxx::tree::one< b_type > b_;
};

class text : public ::xml_schema::type
{
public:
	// font
	//
	typedef ::xml_schema::string font_type;
	typedef ::xsd::cxx::tree::traits< font_type, char > font_traits;

	const font_type&
		font() const;

	font_type&
		font();

	void
		font(const font_type& x);

	void
		font(::std::auto_ptr< font_type > p);

	// r
	//
	typedef ::xml_schema::integer r_type;
	typedef ::xsd::cxx::tree::traits< r_type, char > r_traits;

	const r_type&
		r() const;

	r_type&
		r();

	void
		r(const r_type& x);

	// g
	//
	typedef ::xml_schema::integer g_type;
	typedef ::xsd::cxx::tree::traits< g_type, char > g_traits;

	const g_type&
		g() const;

	g_type&
		g();

	void
		g(const g_type& x);

	// b
	//
	typedef ::xml_schema::integer b_type;
	typedef ::xsd::cxx::tree::traits< b_type, char > b_traits;

	const b_type&
		b() const;

	b_type&
		b();

	void
		b(const b_type& x);

	// Constructors.
	//
	text(const font_type&,
		const r_type&,
		const g_type&,
		const b_type&);

	text(const ::xercesc::DOMElement& e,
		::xml_schema::flags f = 0,
		::xml_schema::container* c = 0);

	text(const text& x,
		::xml_schema::flags f = 0,
		::xml_schema::container* c = 0);

	virtual text*
		_clone(::xml_schema::flags f = 0,
			::xml_schema::container* c = 0) const;

	text&
		operator= (const text& x);

	virtual
		~text();

	// Implementation.
	//
protected:
	void
		parse(::xsd::cxx::xml::dom::parser< char >&,
			::xml_schema::flags);

protected:
	::xsd::cxx::tree::one< font_type > font_;
	::xsd::cxx::tree::one< r_type > r_;
	::xsd::cxx::tree::one< g_type > g_;
	::xsd::cxx::tree::one< b_type > b_;
};

class text_background : public ::xml_schema::type
{
public:
	// r
	//
	typedef ::xml_schema::integer r_type;
	typedef ::xsd::cxx::tree::traits< r_type, char > r_traits;

	const r_type&
		r() const;

	r_type&
		r();

	void
		r(const r_type& x);

	// g
	//
	typedef ::xml_schema::integer g_type;
	typedef ::xsd::cxx::tree::traits< g_type, char > g_traits;

	const g_type&
		g() const;

	g_type&
		g();

	void
		g(const g_type& x);

	// b
	//
	typedef ::xml_schema::integer b_type;
	typedef ::xsd::cxx::tree::traits< b_type, char > b_traits;

	const b_type&
		b() const;

	b_type&
		b();

	void
		b(const b_type& x);

	// Constructors.
	//
	text_background(const r_type&,
		const g_type&,
		const b_type&);

	text_background(const ::xercesc::DOMElement& e,
		::xml_schema::flags f = 0,
		::xml_schema::container* c = 0);

	text_background(const text_background& x,
		::xml_schema::flags f = 0,
		::xml_schema::container* c = 0);

	virtual text_background*
		_clone(::xml_schema::flags f = 0,
			::xml_schema::container* c = 0) const;

	text_background&
		operator= (const text_background& x);

	virtual
		~text_background();

	// Implementation.
	//
protected:
	void
		parse(::xsd::cxx::xml::dom::parser< char >&,
			::xml_schema::flags);

protected:
	::xsd::cxx::tree::one< r_type > r_;
	::xsd::cxx::tree::one< g_type > g_;
	::xsd::cxx::tree::one< b_type > b_;
};

#include <iosfwd>

#include <xercesc/sax/InputSource.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>

// Parse a URI or a local file.
//

::std::auto_ptr< ::c_editorWindows >
c_editorWindows_(const ::std::string& uri,
	::xml_schema::flags f = 0,
	const ::xml_schema::properties& p = ::xml_schema::properties());

::std::auto_ptr< ::c_editorWindows >
c_editorWindows_(const ::std::string& uri,
	::xml_schema::error_handler& eh,
	::xml_schema::flags f = 0,
	const ::xml_schema::properties& p = ::xml_schema::properties());

::std::auto_ptr< ::c_editorWindows >
c_editorWindows_(const ::std::string& uri,
	::xercesc::DOMErrorHandler& eh,
	::xml_schema::flags f = 0,
	const ::xml_schema::properties& p = ::xml_schema::properties());

// Parse std::istream.
//

::std::auto_ptr< ::c_editorWindows >
c_editorWindows_(::std::istream& is,
	::xml_schema::flags f = 0,
	const ::xml_schema::properties& p = ::xml_schema::properties());

::std::auto_ptr< ::c_editorWindows >
c_editorWindows_(::std::istream& is,
	::xml_schema::error_handler& eh,
	::xml_schema::flags f = 0,
	const ::xml_schema::properties& p = ::xml_schema::properties());

::std::auto_ptr< ::c_editorWindows >
c_editorWindows_(::std::istream& is,
	::xercesc::DOMErrorHandler& eh,
	::xml_schema::flags f = 0,
	const ::xml_schema::properties& p = ::xml_schema::properties());

::std::auto_ptr< ::c_editorWindows >
c_editorWindows_(::std::istream& is,
	const ::std::string& id,
	::xml_schema::flags f = 0,
	const ::xml_schema::properties& p = ::xml_schema::properties());

::std::auto_ptr< ::c_editorWindows >
c_editorWindows_(::std::istream& is,
	const ::std::string& id,
	::xml_schema::error_handler& eh,
	::xml_schema::flags f = 0,
	const ::xml_schema::properties& p = ::xml_schema::properties());

::std::auto_ptr< ::c_editorWindows >
c_editorWindows_(::std::istream& is,
	const ::std::string& id,
	::xercesc::DOMErrorHandler& eh,
	::xml_schema::flags f = 0,
	const ::xml_schema::properties& p = ::xml_schema::properties());

// Parse xercesc::InputSource.
//

::std::auto_ptr< ::c_editorWindows >
c_editorWindows_(::xercesc::InputSource& is,
	::xml_schema::flags f = 0,
	const ::xml_schema::properties& p = ::xml_schema::properties());

::std::auto_ptr< ::c_editorWindows >
c_editorWindows_(::xercesc::InputSource& is,
	::xml_schema::error_handler& eh,
	::xml_schema::flags f = 0,
	const ::xml_schema::properties& p = ::xml_schema::properties());

::std::auto_ptr< ::c_editorWindows >
c_editorWindows_(::xercesc::InputSource& is,
	::xercesc::DOMErrorHandler& eh,
	::xml_schema::flags f = 0,
	const ::xml_schema::properties& p = ::xml_schema::properties());

// Parse xercesc::DOMDocument.
//

::std::auto_ptr< ::c_editorWindows >
c_editorWindows_(const ::xercesc::DOMDocument& d,
	::xml_schema::flags f = 0,
	const ::xml_schema::properties& p = ::xml_schema::properties());

::std::auto_ptr< ::c_editorWindows >
c_editorWindows_(::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d,
	::xml_schema::flags f = 0,
	const ::xml_schema::properties& p = ::xml_schema::properties());

#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
//
// End epilogue.

#endif // EDITOR_WINDOWS_HXX
