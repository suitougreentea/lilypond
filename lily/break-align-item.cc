/*
  break-align-item.cc -- implement Break_align_item

  source file of the GNU LilyPond music typesetter

  (c)  1997--2000 Han-Wen Nienhuys <hanwen@cs.uu.nl>
*/


#include <math.h>
#include <libc-extension.hh>

#include "side-position-interface.hh"
#include "axis-group-interface.hh"
#include "warn.hh"
#include "lily-guile.hh"
#include "break-align-item.hh"
#include "dimensions.hh"
#include "paper-def.hh"
#include "paper-column.hh"
#include "group-interface.hh"
#include "align-interface.hh"

MAKE_SCHEME_CALLBACK(Break_align_item,before_line_breaking);

SCM
Break_align_item::before_line_breaking (SCM smob)
{
  Score_element* me = unsmob_element (smob);
  do_alignment (me);
  return SCM_UNSPECIFIED;
}

Real
Break_align_item::alignment_callback (Score_element*c, Axis a)
{
  assert (a == X_AXIS);
  Score_element *par = c->parent_l (a);
  if (par && !to_boolean (par->get_elt_property ("break-alignment-done")))\
    {
      par->set_elt_property ("break-alignment-done", SCM_BOOL_T);
      Break_align_item::do_alignment (par);
    }
    
  return 0.0;
}

void
Break_align_item::add_element (Score_element*me, Score_element *toadd)
{
  toadd->add_offset_callback (alignment_callback, X_AXIS);
  Axis_group_interface::add_element (me, toadd);
}

void
Break_align_item::do_alignment (Score_element *me)
{
  Item * item = dynamic_cast<Item*> (me);
  Item *column = item->column_l ();
  if (item->break_status_dir() == LEFT)
    {
      me->set_elt_property ("self-alignment-X", gh_int2scm (RIGHT));
    }
  else
    {
      me->add_offset_callback (Align_interface::center_on_element, X_AXIS);
    }

  Real interline= me->paper_l ()->get_var ("staffspace");	
  Link_array<Score_element> elems;
  Link_array<Score_element> all_elems
    = Pointer_group_interface__extract_elements (me, (Score_element*)0,
						 "elements");
  
  for (int i=0; i < all_elems.size(); i++) 
    {
      Interval y = all_elems[i]->extent(X_AXIS);
      if (!y.empty_b())
	elems.push (dynamic_cast<Score_element*> (all_elems[i]));
    }
  
  if (!elems.size ())
    return;

  SCM symbol_list = SCM_EOL;
  Array<Real> dists;
  SCM current_origin = ly_symbol2scm ("none");
  for (int i=0; i <= elems.size (); i++)
    {
      Score_element *next_elt  = i < elems.size ()
	? elems[i]
	: 0 ;
      
      SCM next_origin;

      if (next_elt)
	{
	  next_origin = next_elt->get_elt_property ("break-align-symbol");
	  next_origin =
	    gh_symbol_p (next_origin)? 
	    next_origin : ly_symbol2scm ("none")
;
	}
      else
	next_origin = ly_symbol2scm ("begin-of-note");
      
      SCM e = scm_assoc (scm_listify (current_origin,
				      next_origin,
				      SCM_UNDEFINED),
			 scm_eval2 (ly_symbol2scm ("space-alist"), SCM_EOL));
      
      SCM extra_space;
      if (e != SCM_BOOL_F)
	{
	  extra_space = gh_cdr (e);
	}
      else
	{
	  warning (_f ("unknown spacing pair `%s', `%s'",
		       ly_symbol2string (current_origin),
		       ly_symbol2string (next_origin)));
	  extra_space = scm_listify (ly_symbol2scm ("minimum-space"), gh_double2scm (0.0), SCM_UNDEFINED);
	}

      SCM symbol = gh_car  (extra_space);
      Real spc = gh_scm2double (gh_cadr(extra_space));
      spc *= interline;

      dists.push(spc);
      symbol_list = gh_cons (symbol, symbol_list);
      current_origin = next_origin;
    }


  // skip the first sym.
  symbol_list  = gh_cdr (scm_reverse (symbol_list));
  for (int i=0; i <elems.size()-1; i++)
    {
      elems[i]->set_elt_property (gh_car  (symbol_list),
				  scm_cons (gh_double2scm (0),
					    gh_double2scm (dists[i+1])));

      symbol_list = gh_cdr (symbol_list);
    }


  // urg
  SCM first_pair = elems[0]->get_elt_property ("minimum-space");
  if (gh_pair_p (first_pair))
    first_pair = first_pair;
  else
    first_pair = gh_cons (gh_double2scm (0.0), gh_double2scm (0.0));
  
  scm_set_car_x (first_pair, gh_double2scm (-dists[0]));
  elems[0]->set_elt_property ("minimum-space", first_pair);


  /*
    Force callbacks for alignment to be called   
  */
  Align_interface::do_side_processing (me, X_AXIS);

  Real pre_space = elems[0]->relative_coordinate (column, X_AXIS);

  Real xl = elems[0]->extent (X_AXIS)[LEFT];
  if (!isinf (xl))
    pre_space += xl;
  else
    programming_error ("Infinity reached. ");

  Real xr = elems.top ()->extent (X_AXIS)[RIGHT];
  Real spring_len = elems.top ()->relative_coordinate (column, X_AXIS);
  if (!isinf (xr))
    spring_len += xr;
  else
    programming_error ("Infinity reached.");
  
  Real stretch_distance =0.;
  
  if (gh_car  (symbol_list) == ly_symbol2scm ("extra-space"))
    {
      spring_len += dists.top ();
      stretch_distance = dists.top ();
    }
  else if (gh_car  (symbol_list) == ly_symbol2scm ("minimum-space"))
    {
      spring_len = spring_len >? dists.top ();
      stretch_distance = spring_len;
    }

  /*
    Hint the spacing engine how much space to put in.

    The pairs are in the format of an interval (ie. CAR <  CDR).
  */
  column->set_elt_property ("extra-space",
			    scm_cons (gh_double2scm (pre_space),
				      gh_double2scm (spring_len)));

  column->set_elt_property ("stretch-distance",
			    gh_cons (gh_double2scm (-dists[0]),
				     gh_double2scm (stretch_distance)));


}


void
Break_align_item::set_interface (Score_element*me)
{
  Align_interface::set_interface (me); 
  Align_interface::set_axis (me,X_AXIS);

  me->add_offset_callback (Side_position::aligned_on_self, X_AXIS);
}
