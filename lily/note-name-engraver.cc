/*   
  note-name-engraver.cc --  implement Note_name_engraver
  
  source file of the GNU LilyPond music typesetter
  
  (c) 1999--2000 Han-Wen Nienhuys <hanwen@cs.uu.nl>
  
 */

#include "engraver.hh"
#include "musical-request.hh"
#include "item.hh"

class Note_name_engraver : public Engraver
{
public:
  VIRTUAL_COPY_CONS(Translator);
  Link_array<Note_req> req_l_arr_;
  Link_array<Item> texts_;
  virtual bool  do_try_music (Music*m);
  void deprecated_process_music ();
  virtual void do_pre_move_processing ();
};

bool
Note_name_engraver::do_try_music (Music *m)
{
  if (Note_req *r = dynamic_cast<Note_req* > (m))
    {
      req_l_arr_.push (r);
      return true;
    }
  return false;
}


void
Note_name_engraver::deprecated_process_music ()
{
  if (texts_.size ())
    return;
  String s ;
  for (int i=0; i < req_l_arr_.size (); i++)
    {
      if (i)
	s += " ";
      s += unsmob_pitch (req_l_arr_[i]->get_mus_property ("pitch"))->str ();
    }
  if (s.length_i())
    {
      Item * t = new Item (get_property ("NoteName"));
      t->set_elt_property ("text", ly_str02scm ( s.ch_C()));
      announce_element (t, req_l_arr_[0]);
      texts_.push (t);
    }
}

void
Note_name_engraver::do_pre_move_processing ()
{
  for (int i=0; i < texts_.size (); i++)
    {
      typeset_element (texts_[i]);
    }
  texts_.clear() ;
  req_l_arr_.clear ();
}

ADD_THIS_TRANSLATOR(Note_name_engraver);
