/*   
  g-stem-staff-side.cc --  implement Stem_staff_side
  
  source file of the GNU LilyPond music typesetter
  
  (c) 1999 Han-Wen Nienhuys <hanwen@cs.uu.nl>
  
 */

#include "dimension-cache.hh"
#include "stem-staff-side.hh"
#include "stem.hh"
#include "staff-symbol.hh"
#include "paper-def.hh"

Stem_staff_side_item::Stem_staff_side_item ()
{
  stem_l_ =0;
  relative_dir_ = CENTER;
}

void
Stem_staff_side_item::do_substitute_element_pointer (Score_element*o,
						       Score_element*n)
{
  if (o == stem_l_)
    {
      stem_l_ = dynamic_cast<Stem*> (n);
    }
  Staff_side_item::do_substitute_element_pointer (o,n);
}

void
Stem_staff_side_item::set_stem (Stem*s)
{
  stem_l_ =s;
  add_dependency (s);
}


Direction
Stem_staff_side_item::get_default_direction () const
{
  return (Direction)(relative_dir_ * stem_l_->dir_);
}

void
Stem_staff_side_item::do_pre_processing ()
{
  SCM p = remove_elt_property ("padding");
  Real pad  = paper_l ()->get_var ("articulation_script_padding_default");
  if (get_elt_property ("no-staff-support") != SCM_UNDEFINED)
    pad =0.0; 
  
  Real il  = (stem_l_) ? stem_l_->staff_line_leading_f (): paper_l ()->get_var ("interline");
  
  if (p != SCM_UNDEFINED)
    {
      pad =  gh_scm2double (p);
    }
  pad *=  il ;
  set_elt_property ("padding",
		    gh_double2scm(pad));

  Staff_side_item::do_pre_processing ();
}

void
Stem_staff_side_item::do_post_processing ()
{
  Staff_side_item::do_post_processing ();

 /*
   Ugh: try to get staccato dots right. 

   TODO:
    
    
    should use general no_staff_line_collision elt property, and use
    the same elt prop also for positioning slurs, ties and dots.
  */
  if (get_elt_property ("no-staff-support") != SCM_UNDEFINED)
    {
      Graphical_element * c =common_refpoint (staff_symbol_l (), Y_AXIS);
      Real staff_coord = staff_symbol_l ()->relative_coordinate (c, Y_AXIS);
      Real self_coord = relative_coordinate (c, Y_AXIS);
      Real now_coord = self_coord - staff_coord;
      
      Real desired_coord = ceil (dir_ * 2.0 *  now_coord / staff_line_leading_f ());
      if (! (int (desired_coord) % 2))
	{
	  desired_coord ++;
	}

      translate_axis (desired_coord * dir_ *  staff_line_leading_f () / 2.0  - now_coord, Y_AXIS);
    }
}

