/*
  lily-guile.cc -- implement assorted guile functions

  source file of the GNU LilyPond music typesetter

  (c) 1998--1999 Jan Nieuwenhuizen <janneke@gnu.org>

  Han-Wen Nienhuys <hanwen@cs.uu.nl>
*/


#include <stdio.h>
#include <stdlib.h>

#include "libc-extension.hh"
#include "lily-guile.hh"
#include "main.hh"
#include "simple-file-storage.hh"
#include "file-path.hh"
#include "debug.hh"

/*
  scm_m_quote doesn't use any env, but needs one for a good signature in GUILE.

  Why there is no gh_quote () in GUILE  beats me.
*/

SCM
ly_quote_scm (SCM s)
{
  return scm_cons2 (scm_i_quote, s, SCM_EOL);
}

/*
  See: libguile/symbols.c

  SCM
  scm_string_to_symbol(s)
  
*/
SCM
ly_symbol (String name)
{
  return gh_car (scm_intern (name.ch_C(), name.length_i()));
}

String
symbol_to_string (SCM s)
{
  return String((Byte*)SCM_CHARS (s), (int) SCM_LENGTH(s));
}

SCM
ly_set_scm (String name, SCM val)
{
  return scm_sysintern (name.ch_C(), val);
  
}

/**
   Read a file, and shove it down GUILE.  GUILE also has file read
   functions, but you can't fiddle with the path of those.
 */
void
read_lily_scm_file (String fn)
{
  String s = global_path.find (fn);
  if (s == "")
    {
      String e = _f ("Can not find file `%s\'", fn);
      e += " ";
      e += _f ("(Load path is `%s\'", global_path.str ());
      error (e);
    }
  else
    *mlog << '[' << s;


  Simple_file_storage f(s);
  
  gh_eval_str ((char *) f.ch_C());
  *mlog << ']' << flush;  
}


SCM
ly_gulp_file (SCM name)
{
  String fn (ly_scm2string (name));
 String s = global_path.find (fn);
  if (s == "")
    {
      String e = _f ("Can not find file `%s\'", fn);
      e += " ";
      e += _f ("(Load path is `%s\'", global_path.str ());
      error (e);
    }
  else
    *mlog << '[' << s;


  Simple_file_storage f(s);
  return gh_str02scm (f.ch_C());
}

void
ly_display_scm (SCM s)
{
  gh_display (s);
  gh_newline ();
}

String
ly_scm2string (SCM s)
{
  int len; 
  char * p = gh_scm2newstr (s , &len);
  
  String r (p);
  //  delete p;
  free (p);
  return r;
}

SCM
index_cell (SCM s, Direction d)
{
  assert (d);
  return (d == LEFT) ? SCM_CAR (s) : SCM_CDR (s);
}

  
SCM
array_to_list (SCM *a , int l)
{
  SCM list = SCM_EOL;
  for (int i= l; i--;  )
    {
      list =  gh_cons (a[i], list);
    }
  return list;
}

SCM
ly_warning (SCM str)
{
  assert (gh_string_p (str));
  warning ("lily-guile: " + ly_scm2string (str));
  return SCM_BOOL_T;
}

void
init_functions ()
{
  scm_make_gsubr ("ly-warn", 1, 0, 0, ly_warning);
  scm_make_gsubr ("ly-gulp-file", 1,0, 0, ly_gulp_file);
}

extern void init_symbols ();

void
init_lily_guile ()
{
  init_symbols();
  init_functions ();
}
