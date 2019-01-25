/*
  This file is part of LilyPond, the GNU music typesetter.

  Copyright (C) 2006--2019 Jan Nieuwenhuizen <janneke@gnu.org>

  LilyPond is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  LilyPond is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with LilyPond.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STD_VECTOR_HH
#define STD_VECTOR_HH

#if 0

/*
  leads to dubious crashes - libstdc++  bug?
*/
#ifdef DEBUG
#define _GLIBCXX_DEBUG 1
#endif
#endif

#include "config.hh"   /* needed at least for HAVE_STL_DATA_METHOD */
#include <algorithm>   /* find, reverse, sort */
#include <functional>  /* unary_function */
#include <cassert>
#include <string>

using namespace std;

template<typename T>
int default_compare (T const &a, T const &b)
{
  if (a < b)
    return -1;
  else if (b < a)
    return 1;
  else
    return 0;
}

template<typename T>
int default_compare (T *const &a, T *const &b)
{
  if (a < b)
    return -1;
  else if (a > b)
    return 1;
  else
    return 0;
}

#include "compare.hh"

#ifndef VSIZE
#define VSIZE
typedef size_t vsize;
#define VPOS ((vsize) -1)
#endif

#include <vector>

template<typename T>
T const &
boundary (vector<T> const &v, int dir, vsize i)
{
  assert (dir);
  return v[dir == -1 ? i : v.size () - 1 - i];
}

template<typename T>
T &
boundary (vector<T> &v, int dir, vsize i)
{
  assert (dir);
  return v[dir == -1 ? i : v.size () - 1 - i];
}

template<typename T>
T const &
back (vector<T> const &v, vsize i)
{
  return v[v.size () - i - 1];
}

template<typename T>
T &
back (vector<T> &v, vsize i)
{
  return v[v.size () - i - 1];
}

template<typename T>
void
concat (vector<T> &v, vector<T> const &w)
{
  v.insert (v.end (), w.begin (), w.end ());
}

template<typename T, typename Compare>
vsize
lower_bound (vector<T> const &v,
             T const &key,
             Compare less,
             vsize b = 0, vsize e = VPOS)
{
  if (e == VPOS)
    e = v.size ();
  typename vector<T>::const_iterator i = lower_bound (v.begin () + b,
                                                      v.begin () + e,
                                                      key,
                                                      less);

  return i - v.begin ();
}

template<typename T, typename Compare>
vsize
upper_bound (vector<T> const &v,
             T const &key,
             Compare less,
             vsize b = 0, vsize e = VPOS)
{
  if (e == VPOS)
    e = v.size ();

  typename vector<T>::const_iterator i = upper_bound (v.begin () + b,
                                                      v.begin () + e,
                                                      key,
                                                      less);

  return i - v.begin ();
}

template<typename T, typename Compare>
vsize
binary_search (vector<T> const &v,
               T const &key,
               Compare less,
               vsize b = 0, vsize e = VPOS)
{
  vsize lb = lower_bound (v, key, less, b, e);

  if (lb == v.size () || less (key, v[lb]))
    return VPOS;
  return lb;
}

template<typename T, typename Compare>
void
vector_sort (vector<T> &v,
             Compare less,
             vsize b = 0, vsize e = VPOS)
{
  if (e == VPOS)
    e = v.size ();

  sort (v.begin () + b, v.begin () + e, less);
}

template<typename T, typename Compare>
void
vector_stable_sort (vector<T> &v,
             Compare less,
             vsize b = 0, vsize e = VPOS)
{
  if (e == VPOS)
    e = v.size ();

  stable_sort (v.begin () + b, v.begin () + e, less);
}

template<typename T>
void
reverse (vector<T> &v)
{
  // CHECKME: for a simple vector, like vector<int>, this should
  // expand to memrev.
  reverse (v.begin (), v.end ());
}

template<typename T>
void
uniq (vector<T> &v)
{
  v.erase (unique (v.begin (), v.end ()), v.end ());
}

template<typename T>
typename vector<T>::const_iterator
find (vector<T> const &v, T const &key)
{
  return find (v.begin (), v.end (), key);
}

template<typename T> struct del : public unary_function<T, void>
{
  void operator () (T x)
  {
    delete x;
    x = 0;
  }
};

template<typename T>
void
junk_pointers (vector<T> &v)
{
  // Hmm.
  for_each (v.begin (), v.end (), del<T> ());
  v.clear ();
}

vector<string> string_split (string str, char c);
string string_join (vector<string> const &strs, const string &infix);

#define iterof(i,s) typeof((s).begin()) i((s).begin())

#endif /* STD_VECTOR_HH */
