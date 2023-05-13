/*
Libraries for fields, doubly-linked lists and red-black trees.
Copyright (C) 2001 James S. Plank

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

---------------------------------------------------------------------------
Please see http://www.cs.utk.edu/~plank/plank/classes/cs360/360/notes/Libfdr/
for instruction on how to use this library.

Jim Plank
plank@cs.utk.edu
http://www.cs.utk.edu/~plank

Associate Professor
Department of Computer Science
University of Tennessee
203 Claxton Complex
1122 Volunteer Blvd.
Knoxville, TN 37996-3450

     865-974-4397
Fax: 865-974-4404
*/

#ifndef _DLLIST_H_
#define _DLLIST_H_

#include "../jval/jval.h"

typedef struct dllist
{
  struct dllist *flink;
  struct dllist *blink;
  Jval val;
} *Dllist;

/**
 * @brief Allocates and returns a new doubly linked list.
 *
 * @return Dllist
 */
extern Dllist new_dllist();

/**
 * @brief Destroys the list, calling free() on all allocated memory in the list.
 * @note The list does not have to be empty.
 */
extern void free_dllist(Dllist);

/**
 * @brief Inserts at the end of the list Inserts after a given node
 */
extern void dll_append(Dllist, Jval);

/**
 * @brief Inserts at the beginning of the list
 */
extern void dll_prepend(Dllist, Jval);

/**
 * @brief Inserts before a given node
 */
extern void dll_insert_before(Dllist, Jval);

/**
 * @brief Inserts after a given node
 */
extern void dll_insert_after(Dllist, Jval);

/**
 * @brief Deletes the given node from the list.
 * @note Don't call this on the sentinel.
 */
extern void dll_delete_node(Dllist);

/**
 * @brief Returns whether l is empty.
 */
extern int dll_empty(Dllist);

extern Jval dll_val(Dllist);

//--------------------------------------------------------------
// Traversing Macros
//--------------------------------------------------------------

#define dll_traverse(ptr, list) \
  for (ptr = list->flink; ptr != list; ptr = ptr->flink)
#define dll_rtraverse(ptr, list) \
  for (ptr = list->blink; ptr != list; ptr = ptr->blink)

//--------------------------------------------------------------
// Optional Macros: These just make your code more readable.
//--------------------------------------------------------------

#define dll_first(d) ((d)->flink)
#define dll_next(d) ((d)->flink)
#define dll_last(d) ((d)->blink)
#define dll_prev(d) ((d)->blink)

/**
 * @brief Returns a pointer to the nil (sentinel) node for the list.
 * @note You don't need to call dll_nil() to access the sentinel node.
 *       You can just use d, although it makes your code more readable if you use dll_nil().
 */
#define dll_nil(d) (d)

#endif
