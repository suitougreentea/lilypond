/*   
  separating-group-spanner.hh -- declare Separating_group_spanner
  
  source file of the GNU LilyPond music typesetter
  
  (c) 1998--2000 Han-Wen Nienhuys <hanwen@cs.uu.nl>
  
 */

#ifndef SEPARATING_GROUP_SPANNER_HH
#define SEPARATING_GROUP_SPANNER_HH

#include "spanner.hh"

class Separating_group_spanner
{
public:
  static void add_spacing_unit (Score_element*me, Item*);
  static void set_interface (Score_element*);
  static SCM set_spacing_rods (SCM);
};

#endif /* SEPARATING_GROUP_SPANNER_HH */

