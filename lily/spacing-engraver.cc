/*   
  spacing-engraver.cc --  implement Spacing_engraver
  
  source file of the GNU LilyPond music typesetter
  
  (c) 1999 Han-Wen Nienhuys <hanwen@cs.uu.nl>
  
 */

#include "musical-request.hh"
#include "score-column.hh"
#include "spacing-engraver.hh"
#include "spacing-spanner.hh"

inline int
compare (Rhythmic_tuple const &a, Rhythmic_tuple const &b)
{
  return Rhythmic_tuple::time_compare (a,b);
}

int
Rhythmic_tuple::time_compare (Rhythmic_tuple const &h1,
			      Rhythmic_tuple const &h2)
{
  
  return (h1.end_ - h2.end_ ).sign ();
}

Spacing_engraver::Spacing_engraver()
{
  spacing_p_ = 0;
}

void
Spacing_engraver::do_creation_processing ()
{
  spacing_p_  =new Spacing_spanner;
  spacing_p_->set_bounds (LEFT, get_staff_info ().command_pcol_l ());  
  announce_element (Score_element_info (spacing_p_, 0));
}

void
Spacing_engraver::do_removal_processing ()
{
  Paper_column * p = get_staff_info ().command_pcol_l ();

  spacing_p_->set_bounds (RIGHT, p);
  typeset_element (spacing_p_);
  spacing_p_ =0;
}

void
Spacing_engraver::acknowledge_element (Score_element_info i)
{
  if (i.elem_l_->get_elt_property ("grace") != SCM_UNDEFINED)
    return;

  if (i.elem_l_->get_elt_property ("non-rhythmic") != SCM_UNDEFINED)
    return;
  
  if (Rhythmic_req * r = dynamic_cast<Rhythmic_req*>(i.req_l_))
    {
      Rhythmic_tuple t(i, now_mom () + r->length_mom ());
      now_durations_.push (t);
    }
}

void
Spacing_engraver::do_pre_move_processing ()
{
  Moment shortest_playing;
  shortest_playing.set_infinite (1);
  for (int i=0; i < playing_durations_.size (); i++)
    {
      Moment m = (playing_durations_[i].info_.req_l_)->length_mom ();
      if (m)
	shortest_playing = shortest_playing <? m;
    }

  Moment starter, inf;
  inf.set_infinite (1);
  starter=inf;
  for (int i=0; i < now_durations_.size (); i++)
    {
      Moment m = now_durations_[i].info_.req_l_->length_mom ();
      if (m)
	starter = starter <? m;

      playing_durations_.insert (now_durations_[i]);
    }
  now_durations_.clear ();
  
  shortest_playing = shortest_playing <? starter;
  
  Score_column * sc
    = dynamic_cast<Score_column*> (get_staff_info ().musical_pcol_l ());

  sc->shortest_playing_mom_ = shortest_playing;
  sc->shortest_starter_mom_ = starter;
}

void
Spacing_engraver::do_post_move_processing ()
{
  Moment now = now_mom ();
  stopped_durations_.clear ();
  while (playing_durations_.size () && playing_durations_.front ().end_ < now)
    playing_durations_.delmin ();
  while (playing_durations_.size () && playing_durations_.front ().end_ == now)
    stopped_durations_.push (playing_durations_.get ());
}

ADD_THIS_TRANSLATOR(Spacing_engraver);


