/**--------------------------------------------------------------------------**\
Author:
  Zeex, Zilberman Rafael from SA-MP.co.il
Description:
	-
Legal:
	This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
	
	Contributors:
		Agresiv, Zeex
	
	Very special thanks to:
		Amit_B - ...
    Zexx - ...
	
Version:
	0.1
Changelog:
	17/06/2014:
		First version.
<remarks>
  Internal usage only.
</remarks>
\**--------------------------------------------------------------------------**/

#ifndef _RLS_@INTERNAL_@@ARRAY_H
#define _RLS_@INTERNAL_@@ARRAY_H

struct rls_array {
  void *data;
  int   data_size;
  int   count;
  int   capacity;
};

/* Element compaison function. */
typedef int (*rls_array_cmp)(const void *x, const void *y);

int rls_array_init(struct rls_array *a, int capacity, int data_size);
void rls_array_dispose(struct rls_array *a);

int rls_array_resize(struct rls_array *a, int new_capacity);
int rls_array_grow(struct rls_array *a);
int rls_array_shrink(struct rls_array *a);
int rls_array_pad(struct rls_array *a);

void *rls_array_get(struct rls_array *a, int index);
void rls_array_set(struct rls_array *a,int index, void *elem);

int rls_array_insert(struct rls_array *a, int index,
    int count, void *elems);
int rls_array_append(struct rls_array *a, void *elem);

int rls_array_remove(struct rls_array *a, int index, int count);
int rls_array_clear(struct rls_array *a);

/* Returns the index of the element pointed to by elem. */
int rls_array_get_index(struct rls_array *a, void *elem);

int rls_array_find(struct rls_array *a, const void *key,
    rls_array_cmp cmp);
int rls_array_find_remove(struct rls_array *a, const void *key,
    rls_array_cmp cmp);

#endif /* !_RLS_@INTERNAL_@@ARRAY_H */