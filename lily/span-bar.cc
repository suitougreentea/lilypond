/*
  span-bar.cc -- implement Span_bar

  source file of the GNU LilyPond music typesetter

  (c)  1997--2000 Han-Wen Nienhuys <hanwen@cs.uu.nl>
*/
#include "dimension-cache.hh"
#include "span-bar.hh"
#include "lookup.hh"
#include "dimensions.hh"
#include "paper-def.hh"
#include "molecule.hh"
#include "warn.hh"
#include "axis-group-interface.hh"
#include "group-interface.hh"

void
Span_bar::add_bar (Score_element*b)
{
  Pointer_group_interface gi (this);
  gi.add_element (b);

  add_dependency (b);
}


Interval
Span_bar::width_callback (Score_element const *se, Axis )
{
  Span_bar*  s= dynamic_cast<Span_bar*> ((Score_element*)se);
  String gl = ly_scm2string (s->get_elt_property ("glyph"));

  /*
    urg.
   */
  Molecule m = s->compound_barline (gl, 40 PT);
  
  return m.extent (X_AXIS);
}

void
Span_bar::before_line_breaking ()
{
  Bar::before_line_breaking ();
  
  evaluate_empty ();
}

void
Span_bar::after_line_breaking ()
{
  Bar::after_line_breaking ();
  Interval i (get_spanned_interval ());
  translate_axis (i.center (), Y_AXIS);
}

void
Span_bar::evaluate_empty ()
{ 
  if (!gh_pair_p (get_elt_pointer ("elements")))
    {
      suicide ();
    }
  
  SCM gl = get_elt_property ("glyph");
  if (!gh_string_p (gl))
    {
      suicide ();
      return ; 
    }
  else {
    String type_str = ly_scm2string (gl);
    String orig = type_str;
    if (type_str == "|:") 
      {
	type_str= ".|";
      }
    else if (type_str== ":|")
      {
	type_str= "|.";
      }
    else if (type_str== ":|:")
      {
	type_str= ".|.";
      }
    if (orig != type_str)
      set_elt_property ("glyph", ly_str02scm (type_str.ch_C()));
  }
}

Interval
Span_bar::get_spanned_interval () const
{
  return Axis_group_interface::group_extent_callback (this, Y_AXIS);  
}


Real
Span_bar::get_bar_size () const
{
  Interval iv (get_spanned_interval ());
  if (iv.empty_b ())
    {
      programming_error("Huh? My children deflated (FIXME)");
      iv = Interval (0,0);
    }
  return iv.length ();
}

Span_bar::Span_bar (SCM s)
  : Bar (s)
{
  Pointer_group_interface(this).set_interface ();
  set_extent_callback (width_callback, X_AXIS);
  
  // dim_cache_[Y_AXIS]->set_extent_callback (Axis_group_interface::group_extent_callback);

  set_extent_callback (0, Y_AXIS);
}

