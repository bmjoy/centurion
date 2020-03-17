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

// Begin prologue.
//
//
// End prologue.

#include <xsd/cxx/pre.hxx>

#include "editorMenuBar.hxx"

// c_editorMenuBar
// 

const c_editorMenuBar::c_background_type& c_editorMenuBar::
c_background () const
{
  return this->c_background_.get ();
}

c_editorMenuBar::c_background_type& c_editorMenuBar::
c_background ()
{
  return this->c_background_.get ();
}

void c_editorMenuBar::
c_background (const c_background_type& x)
{
  this->c_background_.set (x);
}

void c_editorMenuBar::
c_background (::std::auto_ptr< c_background_type > x)
{
  this->c_background_.set (x);
}

const c_editorMenuBar::c_hover_type& c_editorMenuBar::
c_hover () const
{
  return this->c_hover_.get ();
}

c_editorMenuBar::c_hover_type& c_editorMenuBar::
c_hover ()
{
  return this->c_hover_.get ();
}

void c_editorMenuBar::
c_hover (const c_hover_type& x)
{
  this->c_hover_.set (x);
}

void c_editorMenuBar::
c_hover (::std::auto_ptr< c_hover_type > x)
{
  this->c_hover_.set (x);
}

const c_editorMenuBar::c_editorMenu_sequence& c_editorMenuBar::
c_editorMenu () const
{
  return this->c_editorMenu_;
}

c_editorMenuBar::c_editorMenu_sequence& c_editorMenuBar::
c_editorMenu ()
{
  return this->c_editorMenu_;
}

void c_editorMenuBar::
c_editorMenu (const c_editorMenu_sequence& s)
{
  this->c_editorMenu_ = s;
}

const c_editorMenuBar::height_type& c_editorMenuBar::
height () const
{
  return this->height_.get ();
}

c_editorMenuBar::height_type& c_editorMenuBar::
height ()
{
  return this->height_.get ();
}

void c_editorMenuBar::
height (const height_type& x)
{
  this->height_.set (x);
}

const c_editorMenuBar::font_type& c_editorMenuBar::
font () const
{
  return this->font_.get ();
}

c_editorMenuBar::font_type& c_editorMenuBar::
font ()
{
  return this->font_.get ();
}

void c_editorMenuBar::
font (const font_type& x)
{
  this->font_.set (x);
}

void c_editorMenuBar::
font (::std::auto_ptr< font_type > x)
{
  this->font_.set (x);
}


// c_background
// 

const c_background::r_type& c_background::
r () const
{
  return this->r_.get ();
}

c_background::r_type& c_background::
r ()
{
  return this->r_.get ();
}

void c_background::
r (const r_type& x)
{
  this->r_.set (x);
}

const c_background::g_type& c_background::
g () const
{
  return this->g_.get ();
}

c_background::g_type& c_background::
g ()
{
  return this->g_.get ();
}

void c_background::
g (const g_type& x)
{
  this->g_.set (x);
}

const c_background::b_type& c_background::
b () const
{
  return this->b_.get ();
}

c_background::b_type& c_background::
b ()
{
  return this->b_.get ();
}

void c_background::
b (const b_type& x)
{
  this->b_.set (x);
}


// c_hover
// 

const c_hover::r_type& c_hover::
r () const
{
  return this->r_.get ();
}

c_hover::r_type& c_hover::
r ()
{
  return this->r_.get ();
}

void c_hover::
r (const r_type& x)
{
  this->r_.set (x);
}

const c_hover::g_type& c_hover::
g () const
{
  return this->g_.get ();
}

c_hover::g_type& c_hover::
g ()
{
  return this->g_.get ();
}

void c_hover::
g (const g_type& x)
{
  this->g_.set (x);
}

const c_hover::b_type& c_hover::
b () const
{
  return this->b_.get ();
}

c_hover::b_type& c_hover::
b ()
{
  return this->b_.get ();
}

void c_hover::
b (const b_type& x)
{
  this->b_.set (x);
}


// c_editorMenu
// 

const c_editorMenu::command_sequence& c_editorMenu::
command () const
{
  return this->command_;
}

c_editorMenu::command_sequence& c_editorMenu::
command ()
{
  return this->command_;
}

void c_editorMenu::
command (const command_sequence& s)
{
  this->command_ = s;
}

const c_editorMenu::id_type& c_editorMenu::
id () const
{
  return this->id_.get ();
}

c_editorMenu::id_type& c_editorMenu::
id ()
{
  return this->id_.get ();
}

void c_editorMenu::
id (const id_type& x)
{
  this->id_.set (x);
}

const c_editorMenu::name_type& c_editorMenu::
name () const
{
  return this->name_.get ();
}

c_editorMenu::name_type& c_editorMenu::
name ()
{
  return this->name_.get ();
}

void c_editorMenu::
name (const name_type& x)
{
  this->name_.set (x);
}

void c_editorMenu::
name (::std::auto_ptr< name_type > x)
{
  this->name_.set (x);
}

const c_editorMenu::onclick_type& c_editorMenu::
onclick () const
{
  return this->onclick_.get ();
}

c_editorMenu::onclick_type& c_editorMenu::
onclick ()
{
  return this->onclick_.get ();
}

void c_editorMenu::
onclick (const onclick_type& x)
{
  this->onclick_.set (x);
}

void c_editorMenu::
onclick (::std::auto_ptr< onclick_type > x)
{
  this->onclick_.set (x);
}


// command
// 

const command::id_type& command::
id () const
{
  return this->id_.get ();
}

command::id_type& command::
id ()
{
  return this->id_.get ();
}

void command::
id (const id_type& x)
{
  this->id_.set (x);
}

const command::name_type& command::
name () const
{
  return this->name_.get ();
}

command::name_type& command::
name ()
{
  return this->name_.get ();
}

void command::
name (const name_type& x)
{
  this->name_.set (x);
}

void command::
name (::std::auto_ptr< name_type > x)
{
  this->name_.set (x);
}

const command::hotkey1_type& command::
hotkey1 () const
{
  return this->hotkey1_.get ();
}

command::hotkey1_type& command::
hotkey1 ()
{
  return this->hotkey1_.get ();
}

void command::
hotkey1 (const hotkey1_type& x)
{
  this->hotkey1_.set (x);
}

void command::
hotkey1 (::std::auto_ptr< hotkey1_type > x)
{
  this->hotkey1_.set (x);
}

const command::hotkey2_type& command::
hotkey2 () const
{
  return this->hotkey2_.get ();
}

command::hotkey2_type& command::
hotkey2 ()
{
  return this->hotkey2_.get ();
}

void command::
hotkey2 (const hotkey2_type& x)
{
  this->hotkey2_.set (x);
}

void command::
hotkey2 (::std::auto_ptr< hotkey2_type > x)
{
  this->hotkey2_.set (x);
}

const command::hotkey3_type& command::
hotkey3 () const
{
  return this->hotkey3_.get ();
}

command::hotkey3_type& command::
hotkey3 ()
{
  return this->hotkey3_.get ();
}

void command::
hotkey3 (const hotkey3_type& x)
{
  this->hotkey3_.set (x);
}

void command::
hotkey3 (::std::auto_ptr< hotkey3_type > x)
{
  this->hotkey3_.set (x);
}

const command::onclick_type& command::
onclick () const
{
  return this->onclick_.get ();
}

command::onclick_type& command::
onclick ()
{
  return this->onclick_.get ();
}

void command::
onclick (const onclick_type& x)
{
  this->onclick_.set (x);
}

void command::
onclick (::std::auto_ptr< onclick_type > x)
{
  this->onclick_.set (x);
}


#include <xsd/cxx/xml/dom/parsing-source.hxx>

// c_editorMenuBar
//

c_editorMenuBar::
c_editorMenuBar (const c_background_type& c_background,
                 const c_hover_type& c_hover,
                 const height_type& height,
                 const font_type& font)
: ::xml_schema::type (),
  c_background_ (c_background, this),
  c_hover_ (c_hover, this),
  c_editorMenu_ (this),
  height_ (height, this),
  font_ (font, this)
{
}

c_editorMenuBar::
c_editorMenuBar (::std::auto_ptr< c_background_type > c_background,
                 ::std::auto_ptr< c_hover_type > c_hover,
                 const height_type& height,
                 const font_type& font)
: ::xml_schema::type (),
  c_background_ (c_background, this),
  c_hover_ (c_hover, this),
  c_editorMenu_ (this),
  height_ (height, this),
  font_ (font, this)
{
}

c_editorMenuBar::
c_editorMenuBar (const c_editorMenuBar& x,
                 ::xml_schema::flags f,
                 ::xml_schema::container* c)
: ::xml_schema::type (x, f, c),
  c_background_ (x.c_background_, f, this),
  c_hover_ (x.c_hover_, f, this),
  c_editorMenu_ (x.c_editorMenu_, f, this),
  height_ (x.height_, f, this),
  font_ (x.font_, f, this)
{
}

c_editorMenuBar::
c_editorMenuBar (const ::xercesc::DOMElement& e,
                 ::xml_schema::flags f,
                 ::xml_schema::container* c)
: ::xml_schema::type (e, f | ::xml_schema::flags::base, c),
  c_background_ (this),
  c_hover_ (this),
  c_editorMenu_ (this),
  height_ (this),
  font_ (this)
{
  if ((f & ::xml_schema::flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, true, false, true);
    this->parse (p, f);
  }
}

void c_editorMenuBar::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::flags f)
{
  for (; p.more_content (); p.next_content (false))
  {
    const ::xercesc::DOMElement& i (p.cur_element ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    // c_background
    //
    if (n.name () == "c_background" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< c_background_type > r (
        c_background_traits::create (i, f, this));

      if (!c_background_.present ())
      {
        this->c_background_.set (r);
        continue;
      }
    }

    // c_hover
    //
    if (n.name () == "c_hover" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< c_hover_type > r (
        c_hover_traits::create (i, f, this));

      if (!c_hover_.present ())
      {
        this->c_hover_.set (r);
        continue;
      }
    }

    // c_editorMenu
    //
    if (n.name () == "c_editorMenu" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< c_editorMenu_type > r (
        c_editorMenu_traits::create (i, f, this));

      this->c_editorMenu_.push_back (r);
      continue;
    }

    break;
  }

  if (!c_background_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "c_background",
      "");
  }

  if (!c_hover_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "c_hover",
      "");
  }

  while (p.more_attributes ())
  {
    const ::xercesc::DOMAttr& i (p.next_attribute ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    if (n.name () == "height" && n.namespace_ ().empty ())
    {
      this->height_.set (height_traits::create (i, f, this));
      continue;
    }

    if (n.name () == "font" && n.namespace_ ().empty ())
    {
      this->font_.set (font_traits::create (i, f, this));
      continue;
    }
  }

  if (!height_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "height",
      "");
  }

  if (!font_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "font",
      "");
  }
}

c_editorMenuBar* c_editorMenuBar::
_clone (::xml_schema::flags f,
        ::xml_schema::container* c) const
{
  return new class c_editorMenuBar (*this, f, c);
}

c_editorMenuBar& c_editorMenuBar::
operator= (const c_editorMenuBar& x)
{
  if (this != &x)
  {
    static_cast< ::xml_schema::type& > (*this) = x;
    this->c_background_ = x.c_background_;
    this->c_hover_ = x.c_hover_;
    this->c_editorMenu_ = x.c_editorMenu_;
    this->height_ = x.height_;
    this->font_ = x.font_;
  }

  return *this;
}

c_editorMenuBar::
~c_editorMenuBar ()
{
}

// c_background
//

c_background::
c_background (const r_type& r,
              const g_type& g,
              const b_type& b)
: ::xml_schema::type (),
  r_ (r, this),
  g_ (g, this),
  b_ (b, this)
{
}

c_background::
c_background (const c_background& x,
              ::xml_schema::flags f,
              ::xml_schema::container* c)
: ::xml_schema::type (x, f, c),
  r_ (x.r_, f, this),
  g_ (x.g_, f, this),
  b_ (x.b_, f, this)
{
}

c_background::
c_background (const ::xercesc::DOMElement& e,
              ::xml_schema::flags f,
              ::xml_schema::container* c)
: ::xml_schema::type (e, f | ::xml_schema::flags::base, c),
  r_ (this),
  g_ (this),
  b_ (this)
{
  if ((f & ::xml_schema::flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, false, false, true);
    this->parse (p, f);
  }
}

void c_background::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::flags f)
{
  while (p.more_attributes ())
  {
    const ::xercesc::DOMAttr& i (p.next_attribute ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    if (n.name () == "r" && n.namespace_ ().empty ())
    {
      this->r_.set (r_traits::create (i, f, this));
      continue;
    }

    if (n.name () == "g" && n.namespace_ ().empty ())
    {
      this->g_.set (g_traits::create (i, f, this));
      continue;
    }

    if (n.name () == "b" && n.namespace_ ().empty ())
    {
      this->b_.set (b_traits::create (i, f, this));
      continue;
    }
  }

  if (!r_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "r",
      "");
  }

  if (!g_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "g",
      "");
  }

  if (!b_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "b",
      "");
  }
}

c_background* c_background::
_clone (::xml_schema::flags f,
        ::xml_schema::container* c) const
{
  return new class c_background (*this, f, c);
}

c_background& c_background::
operator= (const c_background& x)
{
  if (this != &x)
  {
    static_cast< ::xml_schema::type& > (*this) = x;
    this->r_ = x.r_;
    this->g_ = x.g_;
    this->b_ = x.b_;
  }

  return *this;
}

c_background::
~c_background ()
{
}

// c_hover
//

c_hover::
c_hover (const r_type& r,
         const g_type& g,
         const b_type& b)
: ::xml_schema::type (),
  r_ (r, this),
  g_ (g, this),
  b_ (b, this)
{
}

c_hover::
c_hover (const c_hover& x,
         ::xml_schema::flags f,
         ::xml_schema::container* c)
: ::xml_schema::type (x, f, c),
  r_ (x.r_, f, this),
  g_ (x.g_, f, this),
  b_ (x.b_, f, this)
{
}

c_hover::
c_hover (const ::xercesc::DOMElement& e,
         ::xml_schema::flags f,
         ::xml_schema::container* c)
: ::xml_schema::type (e, f | ::xml_schema::flags::base, c),
  r_ (this),
  g_ (this),
  b_ (this)
{
  if ((f & ::xml_schema::flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, false, false, true);
    this->parse (p, f);
  }
}

void c_hover::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::flags f)
{
  while (p.more_attributes ())
  {
    const ::xercesc::DOMAttr& i (p.next_attribute ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    if (n.name () == "r" && n.namespace_ ().empty ())
    {
      this->r_.set (r_traits::create (i, f, this));
      continue;
    }

    if (n.name () == "g" && n.namespace_ ().empty ())
    {
      this->g_.set (g_traits::create (i, f, this));
      continue;
    }

    if (n.name () == "b" && n.namespace_ ().empty ())
    {
      this->b_.set (b_traits::create (i, f, this));
      continue;
    }
  }

  if (!r_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "r",
      "");
  }

  if (!g_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "g",
      "");
  }

  if (!b_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "b",
      "");
  }
}

c_hover* c_hover::
_clone (::xml_schema::flags f,
        ::xml_schema::container* c) const
{
  return new class c_hover (*this, f, c);
}

c_hover& c_hover::
operator= (const c_hover& x)
{
  if (this != &x)
  {
    static_cast< ::xml_schema::type& > (*this) = x;
    this->r_ = x.r_;
    this->g_ = x.g_;
    this->b_ = x.b_;
  }

  return *this;
}

c_hover::
~c_hover ()
{
}

// c_editorMenu
//

c_editorMenu::
c_editorMenu (const id_type& id,
              const name_type& name,
              const onclick_type& onclick)
: ::xml_schema::type (),
  command_ (this),
  id_ (id, this),
  name_ (name, this),
  onclick_ (onclick, this)
{
}

c_editorMenu::
c_editorMenu (const c_editorMenu& x,
              ::xml_schema::flags f,
              ::xml_schema::container* c)
: ::xml_schema::type (x, f, c),
  command_ (x.command_, f, this),
  id_ (x.id_, f, this),
  name_ (x.name_, f, this),
  onclick_ (x.onclick_, f, this)
{
}

c_editorMenu::
c_editorMenu (const ::xercesc::DOMElement& e,
              ::xml_schema::flags f,
              ::xml_schema::container* c)
: ::xml_schema::type (e, f | ::xml_schema::flags::base, c),
  command_ (this),
  id_ (this),
  name_ (this),
  onclick_ (this)
{
  if ((f & ::xml_schema::flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, true, false, true);
    this->parse (p, f);
  }
}

void c_editorMenu::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::flags f)
{
  for (; p.more_content (); p.next_content (false))
  {
    const ::xercesc::DOMElement& i (p.cur_element ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    // command
    //
    if (n.name () == "command" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< command_type > r (
        command_traits::create (i, f, this));

      this->command_.push_back (r);
      continue;
    }

    break;
  }

  while (p.more_attributes ())
  {
    const ::xercesc::DOMAttr& i (p.next_attribute ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    if (n.name () == "id" && n.namespace_ ().empty ())
    {
      this->id_.set (id_traits::create (i, f, this));
      continue;
    }

    if (n.name () == "name" && n.namespace_ ().empty ())
    {
      this->name_.set (name_traits::create (i, f, this));
      continue;
    }

    if (n.name () == "onclick" && n.namespace_ ().empty ())
    {
      this->onclick_.set (onclick_traits::create (i, f, this));
      continue;
    }
  }

  if (!id_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "id",
      "");
  }

  if (!name_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "name",
      "");
  }

  if (!onclick_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "onclick",
      "");
  }
}

c_editorMenu* c_editorMenu::
_clone (::xml_schema::flags f,
        ::xml_schema::container* c) const
{
  return new class c_editorMenu (*this, f, c);
}

c_editorMenu& c_editorMenu::
operator= (const c_editorMenu& x)
{
  if (this != &x)
  {
    static_cast< ::xml_schema::type& > (*this) = x;
    this->command_ = x.command_;
    this->id_ = x.id_;
    this->name_ = x.name_;
    this->onclick_ = x.onclick_;
  }

  return *this;
}

c_editorMenu::
~c_editorMenu ()
{
}

// command
//

command::
command (const id_type& id,
         const name_type& name,
         const hotkey1_type& hotkey1,
         const hotkey2_type& hotkey2,
         const hotkey3_type& hotkey3,
         const onclick_type& onclick)
: ::xml_schema::type (),
  id_ (id, this),
  name_ (name, this),
  hotkey1_ (hotkey1, this),
  hotkey2_ (hotkey2, this),
  hotkey3_ (hotkey3, this),
  onclick_ (onclick, this)
{
}

command::
command (const command& x,
         ::xml_schema::flags f,
         ::xml_schema::container* c)
: ::xml_schema::type (x, f, c),
  id_ (x.id_, f, this),
  name_ (x.name_, f, this),
  hotkey1_ (x.hotkey1_, f, this),
  hotkey2_ (x.hotkey2_, f, this),
  hotkey3_ (x.hotkey3_, f, this),
  onclick_ (x.onclick_, f, this)
{
}

command::
command (const ::xercesc::DOMElement& e,
         ::xml_schema::flags f,
         ::xml_schema::container* c)
: ::xml_schema::type (e, f | ::xml_schema::flags::base, c),
  id_ (this),
  name_ (this),
  hotkey1_ (this),
  hotkey2_ (this),
  hotkey3_ (this),
  onclick_ (this)
{
  if ((f & ::xml_schema::flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, false, false, true);
    this->parse (p, f);
  }
}

void command::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::flags f)
{
  while (p.more_attributes ())
  {
    const ::xercesc::DOMAttr& i (p.next_attribute ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    if (n.name () == "id" && n.namespace_ ().empty ())
    {
      this->id_.set (id_traits::create (i, f, this));
      continue;
    }

    if (n.name () == "name" && n.namespace_ ().empty ())
    {
      this->name_.set (name_traits::create (i, f, this));
      continue;
    }

    if (n.name () == "hotkey1" && n.namespace_ ().empty ())
    {
      this->hotkey1_.set (hotkey1_traits::create (i, f, this));
      continue;
    }

    if (n.name () == "hotkey2" && n.namespace_ ().empty ())
    {
      this->hotkey2_.set (hotkey2_traits::create (i, f, this));
      continue;
    }

    if (n.name () == "hotkey3" && n.namespace_ ().empty ())
    {
      this->hotkey3_.set (hotkey3_traits::create (i, f, this));
      continue;
    }

    if (n.name () == "onclick" && n.namespace_ ().empty ())
    {
      this->onclick_.set (onclick_traits::create (i, f, this));
      continue;
    }
  }

  if (!id_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "id",
      "");
  }

  if (!name_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "name",
      "");
  }

  if (!hotkey1_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "hotkey1",
      "");
  }

  if (!hotkey2_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "hotkey2",
      "");
  }

  if (!hotkey3_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "hotkey3",
      "");
  }

  if (!onclick_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "onclick",
      "");
  }
}

command* command::
_clone (::xml_schema::flags f,
        ::xml_schema::container* c) const
{
  return new class command (*this, f, c);
}

command& command::
operator= (const command& x)
{
  if (this != &x)
  {
    static_cast< ::xml_schema::type& > (*this) = x;
    this->id_ = x.id_;
    this->name_ = x.name_;
    this->hotkey1_ = x.hotkey1_;
    this->hotkey2_ = x.hotkey2_;
    this->hotkey3_ = x.hotkey3_;
    this->onclick_ = x.onclick_;
  }

  return *this;
}

command::
~command ()
{
}

#include <istream>
#include <xsd/cxx/xml/sax/std-input-source.hxx>
#include <xsd/cxx/tree/error-handler.hxx>

::std::auto_ptr< ::c_editorMenuBar >
c_editorMenuBar_ (const ::std::string& u,
                  ::xml_schema::flags f,
                  const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::flags::dont_initialize) == 0,
    (f & ::xml_schema::flags::keep_dom) == 0);

  ::xsd::cxx::tree::error_handler< char > h;

  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      u, h, p, f));

  h.throw_if_failed< ::xsd::cxx::tree::parsing< char > > ();

  return ::std::auto_ptr< ::c_editorMenuBar > (
    ::c_editorMenuBar_ (
      d, f | ::xml_schema::flags::own_dom, p));
}

::std::auto_ptr< ::c_editorMenuBar >
c_editorMenuBar_ (const ::std::string& u,
                  ::xml_schema::error_handler& h,
                  ::xml_schema::flags f,
                  const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::flags::dont_initialize) == 0,
    (f & ::xml_schema::flags::keep_dom) == 0);

  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      u, h, p, f));

  if (!d.get ())
    throw ::xsd::cxx::tree::parsing< char > ();

  return ::std::auto_ptr< ::c_editorMenuBar > (
    ::c_editorMenuBar_ (
      d, f | ::xml_schema::flags::own_dom, p));
}

::std::auto_ptr< ::c_editorMenuBar >
c_editorMenuBar_ (const ::std::string& u,
                  ::xercesc::DOMErrorHandler& h,
                  ::xml_schema::flags f,
                  const ::xml_schema::properties& p)
{
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      u, h, p, f));

  if (!d.get ())
    throw ::xsd::cxx::tree::parsing< char > ();

  return ::std::auto_ptr< ::c_editorMenuBar > (
    ::c_editorMenuBar_ (
      d, f | ::xml_schema::flags::own_dom, p));
}

::std::auto_ptr< ::c_editorMenuBar >
c_editorMenuBar_ (::std::istream& is,
                  ::xml_schema::flags f,
                  const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::flags::dont_initialize) == 0,
    (f & ::xml_schema::flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is);
  return ::c_editorMenuBar_ (isrc, f, p);
}

::std::auto_ptr< ::c_editorMenuBar >
c_editorMenuBar_ (::std::istream& is,
                  ::xml_schema::error_handler& h,
                  ::xml_schema::flags f,
                  const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::flags::dont_initialize) == 0,
    (f & ::xml_schema::flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is);
  return ::c_editorMenuBar_ (isrc, h, f, p);
}

::std::auto_ptr< ::c_editorMenuBar >
c_editorMenuBar_ (::std::istream& is,
                  ::xercesc::DOMErrorHandler& h,
                  ::xml_schema::flags f,
                  const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::sax::std_input_source isrc (is);
  return ::c_editorMenuBar_ (isrc, h, f, p);
}

::std::auto_ptr< ::c_editorMenuBar >
c_editorMenuBar_ (::std::istream& is,
                  const ::std::string& sid,
                  ::xml_schema::flags f,
                  const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::flags::dont_initialize) == 0,
    (f & ::xml_schema::flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
  return ::c_editorMenuBar_ (isrc, f, p);
}

::std::auto_ptr< ::c_editorMenuBar >
c_editorMenuBar_ (::std::istream& is,
                  const ::std::string& sid,
                  ::xml_schema::error_handler& h,
                  ::xml_schema::flags f,
                  const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::flags::dont_initialize) == 0,
    (f & ::xml_schema::flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
  return ::c_editorMenuBar_ (isrc, h, f, p);
}

::std::auto_ptr< ::c_editorMenuBar >
c_editorMenuBar_ (::std::istream& is,
                  const ::std::string& sid,
                  ::xercesc::DOMErrorHandler& h,
                  ::xml_schema::flags f,
                  const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
  return ::c_editorMenuBar_ (isrc, h, f, p);
}

::std::auto_ptr< ::c_editorMenuBar >
c_editorMenuBar_ (::xercesc::InputSource& i,
                  ::xml_schema::flags f,
                  const ::xml_schema::properties& p)
{
  ::xsd::cxx::tree::error_handler< char > h;

  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      i, h, p, f));

  h.throw_if_failed< ::xsd::cxx::tree::parsing< char > > ();

  return ::std::auto_ptr< ::c_editorMenuBar > (
    ::c_editorMenuBar_ (
      d, f | ::xml_schema::flags::own_dom, p));
}

::std::auto_ptr< ::c_editorMenuBar >
c_editorMenuBar_ (::xercesc::InputSource& i,
                  ::xml_schema::error_handler& h,
                  ::xml_schema::flags f,
                  const ::xml_schema::properties& p)
{
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      i, h, p, f));

  if (!d.get ())
    throw ::xsd::cxx::tree::parsing< char > ();

  return ::std::auto_ptr< ::c_editorMenuBar > (
    ::c_editorMenuBar_ (
      d, f | ::xml_schema::flags::own_dom, p));
}

::std::auto_ptr< ::c_editorMenuBar >
c_editorMenuBar_ (::xercesc::InputSource& i,
                  ::xercesc::DOMErrorHandler& h,
                  ::xml_schema::flags f,
                  const ::xml_schema::properties& p)
{
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      i, h, p, f));

  if (!d.get ())
    throw ::xsd::cxx::tree::parsing< char > ();

  return ::std::auto_ptr< ::c_editorMenuBar > (
    ::c_editorMenuBar_ (
      d, f | ::xml_schema::flags::own_dom, p));
}

::std::auto_ptr< ::c_editorMenuBar >
c_editorMenuBar_ (const ::xercesc::DOMDocument& doc,
                  ::xml_schema::flags f,
                  const ::xml_schema::properties& p)
{
  if (f & ::xml_schema::flags::keep_dom)
  {
    ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
      static_cast< ::xercesc::DOMDocument* > (doc.cloneNode (true)));

    return ::std::auto_ptr< ::c_editorMenuBar > (
      ::c_editorMenuBar_ (
        d, f | ::xml_schema::flags::own_dom, p));
  }

  const ::xercesc::DOMElement& e (*doc.getDocumentElement ());
  const ::xsd::cxx::xml::qualified_name< char > n (
    ::xsd::cxx::xml::dom::name< char > (e));

  if (n.name () == "c_editorMenuBar" &&
      n.namespace_ () == "")
  {
    ::std::auto_ptr< ::c_editorMenuBar > r (
      ::xsd::cxx::tree::traits< ::c_editorMenuBar, char >::create (
        e, f, 0));
    return r;
  }

  throw ::xsd::cxx::tree::unexpected_element < char > (
    n.name (),
    n.namespace_ (),
    "c_editorMenuBar",
    "");
}

::std::auto_ptr< ::c_editorMenuBar >
c_editorMenuBar_ (::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d,
                  ::xml_schema::flags f,
                  const ::xml_schema::properties&)
{
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > c (
    ((f & ::xml_schema::flags::keep_dom) &&
     !(f & ::xml_schema::flags::own_dom))
    ? static_cast< ::xercesc::DOMDocument* > (d->cloneNode (true))
    : 0);

  ::xercesc::DOMDocument& doc (c.get () ? *c : *d);
  const ::xercesc::DOMElement& e (*doc.getDocumentElement ());

  const ::xsd::cxx::xml::qualified_name< char > n (
    ::xsd::cxx::xml::dom::name< char > (e));

  if (f & ::xml_schema::flags::keep_dom)
    doc.setUserData (::xml_schema::dom::tree_node_key,
                     (c.get () ? &c : &d),
                     0);

  if (n.name () == "c_editorMenuBar" &&
      n.namespace_ () == "")
  {
    ::std::auto_ptr< ::c_editorMenuBar > r (
      ::xsd::cxx::tree::traits< ::c_editorMenuBar, char >::create (
        e, f, 0));
    return r;
  }

  throw ::xsd::cxx::tree::unexpected_element < char > (
    n.name (),
    n.namespace_ (),
    "c_editorMenuBar",
    "");
}

#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
//
// End epilogue.

