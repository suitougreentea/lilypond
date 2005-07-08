/*
  arpeggio-engraver.cc -- implement Arpeggio_engraver

  source file of the GNU LilyPond music typesetter

  (c) 2000--2005 Jan Nieuwenhuizen <janneke@gnu.org>
*/

#include "engraver.hh"
#include "group-interface.hh"
#include "arpeggio.hh"
#include "stem.hh"
#include "rhythmic-head.hh"
#include "side-position-interface.hh"
#include "note-column.hh"

class Arpeggio_engraver : public Engraver
{
public:
  TRANSLATOR_DECLARATIONS (Arpeggio_engraver);

protected:
  virtual void acknowledge_grob (Grob_info);
  virtual void process_music ();
  virtual void stop_translation_timestep ();
  virtual bool try_music (Music *);
private:
  Item *arpeggio_;
  Music *arpeggio_event_;
};

Arpeggio_engraver::Arpeggio_engraver ()
{
  arpeggio_ = 0;
  arpeggio_event_ = 0;
}

bool
Arpeggio_engraver::try_music (Music *m)
{
  if (!arpeggio_event_)
    {
      arpeggio_event_ = m;
    }
  return true;
}

void
Arpeggio_engraver::acknowledge_grob (Grob_info info)
{
  if (arpeggio_)
    {
      if (Stem::has_interface (info.grob ()))
	{
	  if (!arpeggio_->get_parent (Y_AXIS))
	    arpeggio_->set_parent (info.grob (), Y_AXIS);

	  Pointer_group_interface::add_grob (arpeggio_,
					     ly_symbol2scm ("stems"),
					     info.grob ());
	}

      /*
	We can't catch local key items (accidentals) from Voice context,
	see Local_key_engraver
      */
      else if (Rhythmic_head::has_interface (info.grob ()))
	{
	  Side_position_interface::add_support (arpeggio_, info.grob ());
	}
      else if (Note_column::has_interface (info.grob ()))
	{
	  info.grob ()->set_property ("arpeggio", arpeggio_->self_scm ());
	}
    }
}

void
Arpeggio_engraver::process_music ()
{
  if (arpeggio_event_)
    {
      arpeggio_ = make_item ("Arpeggio", arpeggio_event_->self_scm ());
    }
}

void
Arpeggio_engraver::stop_translation_timestep ()
{
  arpeggio_ = 0;
  arpeggio_event_ = 0;
}

ADD_TRANSLATOR (Arpeggio_engraver,
		/* descr */ "Generate an Arpeggio symbol",
		/* creats*/ "Arpeggio",
		/* accepts */ "arpeggio-event",
		/* acks  */ "stem-interface rhythmic-head-interface note-column-interface",
		/* reads */ "",
		/* write */ "");
