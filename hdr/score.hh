/*
  score.hh -- declare Score

  source file of the LilyPond music typesetter

  (c) 1997 Han-Wen Nienhuys <hanwen@stack.nl>
*/


#ifndef SCORE_HH
#define SCORE_HH

#include "varray.hh"
#include "proto.hh"
#include "plist.hh"
#include "moment.hh"
#include "assoc.hh"
#include "string.hh"

/// the total music def of one movement
struct Score {
    /// paper_, staffs_ and commands_ form the problem definition.
    Paperdef *paper_p_;
    Mididef *midi_p_;
    IPointerList<Staff*> staffs_;
    
    /// "runtime" fields for setting up spacing    
    IPointerList<Score_column*> cols_;
    PScore *pscore_p_;

    const char *defined_ch_c_l_;
    int errorlevel_i_;
    
    /* *************************************************************** */

    /// construction
    Score(Paperdef*);
    ~Score();    
    void add(Staff*);

    /// do everything except outputting to file
    void process();
    
    /// output to file
    void output(String fn);

    /// do midi stuff
    void midi();

    ///
    void set(Mididef* midi_p);

    // standard
    void OK() const;
    void print() const;

    /// find a column.
    PCursor<Score_column *> find_col(Moment,bool);
    
    /// when does the last *musical* element finish?
    Moment last() const;

private:
    void setup_music();
    void process_music();
    // utils:
    PCursor<Score_column*> create_cols(Moment);

    Score(Score const&){}

    /**
      make the pcol_l_ fields of each Score_column point to the correct PCol,
      remove any unnecessary Score_column's
     */
    void do_cols();

    /// remove unused cols
    void clean_cols();
    
    /// add #Idealspacings# to #pscore_#
    void calc_idealspacing();
};
#endif
