/*   
  part-combine-music-iterator.cc -- implement  Part_combine_music_iterator

  source file of the GNU LilyPond music typesetter
  
  (c) 2000 Jan Nieuwenhuizen <janneke@gnu.org>
 */

#include "part-combine-music.hh"
#include "part-combine-music-iterator.hh"
#include "translator-group.hh"
#include "musical-request.hh"
#include "warn.hh"

Part_combine_music_iterator::Part_combine_music_iterator ()
{
  combined_b_ = false;

  first_iter_p_ = 0;
  second_iter_p_ = 0;
}

Part_combine_music_iterator::~Part_combine_music_iterator ()
{
  delete second_iter_p_;
  delete first_iter_p_;
}

Moment
Part_combine_music_iterator::next_moment () const
{
  Moment first_next = first_iter_p_->next_moment ();
  Moment second_next = second_iter_p_->next_moment ();
  return first_next <? second_next;
}

bool
Part_combine_music_iterator::ok () const
{
  //hmm
  return first_iter_p_->ok ();
}

void
Part_combine_music_iterator::do_print () const
{
  first_iter_p_->print ();
  second_iter_p_->print ();
}

void
Part_combine_music_iterator::construct_children ()
{
  Part_combine_music const * m = dynamic_cast<Part_combine_music const*> (music_l_);
  
  first_iter_p_ = get_iterator_p (m->first_l ());
  second_iter_p_ = get_iterator_p (m->second_l ());
}

void
Part_combine_music_iterator::change_to (Music_iterator *it, String to_type,
					String to_id)
{
  Translator_group * current = it->report_to_l ();
  Translator_group * last = 0;

  /*
    Cut & Paste from from Auto_change_iterator from Change_iterator (ugh).

    TODO: abstract this function 
   */
  
  /* find the type  of translator that we're changing.
     
     If \translator Staff = bass, then look for Staff = *
   */
  while  (current && current->type_str_ != to_type)
    {
      last = current;
      current = current->daddy_trans_l_;
    }

  if (current && current->id_str_ == to_id)
    {
      String msg;
      msg += _ ("Can't switch translators, I'm there already");
    }
  
  if (current) 
    if (last)
      {
	Translator_group * dest = 
	  it->report_to_l ()->find_create_translator_l (to_type, to_id);
	current->remove_translator_p (last);
	dest->add_translator (last);
      }
    else
      {
	/*
	  We could change the current translator's id, but that would make 
	  errors hard to catch
	  
	   last->translator_id_str_  = change_l ()->change_to_id_str_;
	*/
	error (_f ("I'm one myself: `%s'", to_type.ch_C ()));
      }
  else
    error (_f ("none of these in my family: `%s'", to_id.ch_C ()));
}

Pitch_interrogate_req* first_spanish_inquisition; // nobody expects it
Pitch_interrogate_req* second_spanish_inquisition; // won't strike twice

void
Part_combine_music_iterator::do_process_and_next (Moment m)
{
  Moment first_next = first_iter_p_->next_moment ();
  Moment second_next = second_iter_p_->next_moment ();

  Part_combine_music const * p = dynamic_cast<Part_combine_music const* > (music_l_);

  /*
    different rhythm for combined voices: separate 
    same rhythm for separated voices: combine

    Arg.  Voices should be separated for small intervals, eg < 3.
    This should be \property settable, and, we need the outcome
    of the spanish_inquisition's...

    Can't we first do a process_and_next go into a fake/tmp tree,
    use + junk the result, and then do the real process_and_next...?
    
  */


  if (first_next <= m)
    first_iter_p_->process_and_next (m);

  if (second_next <= m)
    second_iter_p_->process_and_next (m);

  Music_iterator::do_process_and_next (m);

  /*
    TODO:

    * setting of stem directions by a2-engraver don't work !!

    * check setting/resetting of properties

    * use some unexpectedly handy abrvs.
    
    (customise) * separate for small ( <3 ?) intervals too
      
    Later (because currently, we only handle thread switching, really):

    Maybe different modes exist?

    * Wind instruments (Flute I/II)
    * Hymnals:  


      Rules for Hymnals/SATB (John Henckel <henckel@iname.com>):

      1. if S and A differ by less than a third, the stems should be up/down.
      2. else if S and A have different values, the stems should be up/down.
      3. else if A sings "la" or higher, both S and A stems should be down.
      4. else both stems should be up

    * This may get really tricky: combining voices/staffs: string instruments

   */
  
  if (!first_spanish_inquisition)
    first_spanish_inquisition = new Pitch_interrogate_req;
  Music_iterator* fit = first_iter_p_->try_music (first_spanish_inquisition);

  if (!second_spanish_inquisition)
    second_spanish_inquisition = new Pitch_interrogate_req;
  Music_iterator* sit = second_iter_p_->try_music (second_spanish_inquisition);

  SCM interval = SCM_BOOL_F;
  if (first_spanish_inquisition->pitch_arr_.size ()
      && second_spanish_inquisition->pitch_arr_.size ())
    {
      first_spanish_inquisition->pitch_arr_.sort (Musical_pitch::compare);
      second_spanish_inquisition->pitch_arr_.sort (Musical_pitch::compare);
      interval = gh_int2scm (first_spanish_inquisition->pitch_arr_.top ().semitone_pitch ());
    }


  Translator_group * fir = first_iter_p_->report_to_l ();
  Translator_group * sir = second_iter_p_->report_to_l ();

  bool solo = (first_spanish_inquisition->pitch_arr_.empty ()
	       != second_spanish_inquisition->pitch_arr_.empty ());

  bool unirhythm_b = (first_next == second_next) && !solo;

  bool too_close_b = false;
  if (gh_number_p (interval) && gh_scm2int (interval) < 3)
    too_close_b = true;

  String to_id =  combined_b_ ? "one" : "two";
  if ((!unirhythm_b && combined_b_)
      || (unirhythm_b && !combined_b_)
      || (too_close_b && combined_b_)
      || (solo && combined_b_))
    {
      combined_b_ = !combined_b_;
      to_id =  combined_b_ ? "one" : "two";
      change_to (second_iter_p_, p->what_str_, to_id);
    }

  if (!combined_b_)
    sir = second_iter_p_->report_to_l ();

  Translator_group * fd = fir->find_create_translator_l (p->what_str_, "one");
  Translator_group * sd = sir->find_create_translator_l (p->what_str_, "two");

  fd->set_property ("unirhythm", unirhythm_b ? SCM_BOOL_T : SCM_BOOL_F);
  sd->set_property ("unirhythm", unirhythm_b ? SCM_BOOL_T : SCM_BOOL_F);
  fir->set_property ("unirhythm", unirhythm_b ? SCM_BOOL_T : SCM_BOOL_F);
  sir->set_property ("unirhythm", unirhythm_b ? SCM_BOOL_T : SCM_BOOL_F);

  if (unirhythm_b
      && !compare (&first_spanish_inquisition->pitch_arr_,
		   &second_spanish_inquisition->pitch_arr_))
    {
      fd->set_property ("unison", SCM_BOOL_T);
      sd->set_property ("unison", SCM_BOOL_T);
      fir->set_property ("unison", SCM_BOOL_T);
      sir->set_property ("unison", SCM_BOOL_T);
    }
  else
    {
      fd->set_property ("unison", SCM_BOOL_F);
      sd->set_property ("unison", SCM_BOOL_F);
      fir->set_property ("unison", SCM_BOOL_F);
      sir->set_property ("unison", SCM_BOOL_F);
    }

  fd->set_property ("interval", interval);
  sd->set_property ("interval", interval);
  fir->set_property ("interval", interval);
  sir->set_property ("interval", interval);
      
  if (first_spanish_inquisition->pitch_arr_.size ()
      && !second_spanish_inquisition->pitch_arr_.size ())
    {
      fd->set_property ("solo", SCM_BOOL_T);
      fir->set_property ("solo", SCM_BOOL_T);
    }
  else
    {
      fd->set_property ("solo", SCM_BOOL_F);
      fir->set_property ("solo", SCM_BOOL_F);
    }


  if (!first_spanish_inquisition->pitch_arr_.size ()
      && second_spanish_inquisition->pitch_arr_.size ())
    {
      sd->set_property ("solo", SCM_BOOL_T);
      sir->set_property ("solo", SCM_BOOL_T);
    }
  else
    {
      sd->set_property ("solo", SCM_BOOL_F);
      sir->set_property ("solo", SCM_BOOL_F);
    }


  first_spanish_inquisition->pitch_arr_.clear ();
  second_spanish_inquisition->pitch_arr_.clear ();
}

Music_iterator*
Part_combine_music_iterator::try_music_in_children (Music *m) const
{
  Music_iterator * i =  first_iter_p_->try_music (m);
  if (i)
    return i;
  else
    return second_iter_p_->try_music (m);
}

