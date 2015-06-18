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

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

static void *_rls_array_get_elem_ptr(struct rls_array *a,
                                         int index) {
  return (unsigned char *)a->data + (index * a->data_size);
}

static int _rls_array_normalize_index(struct rls_array *a,
                                          int index) {
  /*
   * if(index < 0)
   *  return a->count + index; (1*a->count + index)
   * else
   *  return index; (0*a->count + index)
   */
  return (index < 0)*a->count + index;
}

int rls_array_init(struct rls_array *a,
                      int capacity,
                      int data_size) {
  assert(a != NULL);
  assert(capacity > 0);
  assert(data_size > 0);

  if ((a->data = malloc(data_size * capacity)) == NULL) {
    return -ENOMEM;
  }

  a->capacity = capacity;
  a->data_size = data_size;
  a->count = 0;

  return 0;
}

void rls_array_dispose(struct rls_array *a) {
  assert(a != NULL);

  free(a->data);
  memset(a, 0, sizeof(*a));
}

int rls_array_resize(struct rls_array *a, int new_capacity) {
  void *new_data;

  assert(a != NULL);
  assert(new_capacity >= 0);
  assert(a->data_size > 0);

  if (new_capacity == a->capacity) {
    return 0;
  }

  if (new_capacity > 0) {
    new_data = realloc(a->data, a->data_size * new_capacity);

    if (new_data == NULL) {
      return -errno;
    }
  } else {
    free(a->data);
    new_data = NULL;
  }

  a->data = new_data;
  a->capacity = new_capacity;

  if (a->count > a->capacity) {
    a->count = a->capacity;
  }

  return 0;
}

int rls_array_grow(struct rls_array *a) {
  assert(a != NULL);

  if (a->capacity == 0) {
    return rls_array_resize(a, 1);
  }

  return rls_array_resize(a, (int)(a->capacity * 2));
}

int rls_array_shrink(struct rls_array *a) {
  assert(a != NULL);

  return rls_array_resize(a, a->count);
}

int rls_array_pad(struct rls_array *a) {
  int space;

  assert(a != NULL);

  if ((space = a->capacity - a->count) <= 0) {
    return -EBADRQC;
  }

  return (a->count = a->capacity);
}

void *rls_array_get(struct rls_array *a, int index) {
  assert(a != NULL);

  index = _rls_array_normalize_index(a, index);
  assert(index < a->count);

  return _rls_array_get_elem_ptr(a, index);
}

void rls_array_set(struct rls_array *a, int index, void *elem) {
  assert(a != NULL);

  index = _rls_array_normalize_index(a, index);
  assert(index < a->count);

  memcpy(_rls_array_get_elem_ptr(a, index), elem, a->data_size);
}

int rls_array_insert(struct rls_array *a,
                         int index,
                         int count,
                         void *elems) {
  int need_count;
  int move_count;

  assert(a != NULL);
  assert(elems != NULL);

  if (count <= 0) {
    return -EINVAL;
  }

  index = _rls_array_normalize_index(a, index);
  assert(index <= a->count);

  need_count = a->count + count - a->capacity;
  move_count = a->count - index;

  if (need_count > 0) {
    int error;

    if ((error = rls_array_resize(a, a->capacity + need_count)) < 0) {
      return error;
    }
  }

  if (move_count > 0) {
    memmove(_rls_array_get_elem_ptr(a, index + count),
            _rls_array_get_elem_ptr(a, index),
            move_count * a->data_size);
  }

  a->count += count;
  memcpy(_rls_array_get_elem_ptr(a, index), elems, count * a->data_size);

  return index;
}

int rls_array_append(struct rls_array *a, void *elem) {
  assert(a != NULL);
  assert(elem != NULL);

  if (a->count == a->capacity) {
    int error;

    if ((error = rls_array_grow(a)) < 0) {
      return error;
    }
  }

  a->count++;
  rls_array_set(a, a->count - 1, elem);

  return a->count - 1;
}

int rls_array_remove(struct rls_array *a, int index, int count) {
  int move_count;

  assert(a != NULL);

  index = _rls_array_normalize_index(a, index);
  assert(index < a->count);

  if (count <= 0 || count > a->count - index) {
    return -EINVAL;
  }

  move_count = a->count - index - count;

  if (move_count > 0) {
    memmove(_rls_array_get_elem_ptr(a, index),
            _rls_array_get_elem_ptr(a, index + count),
            move_count * a->data_size);
   }

  a->count -= count;

  if (a->count <= a->capacity / 4) {
    return rls_array_resize(a, a->capacity / 2);
  }

  return index;
}

int rls_array_clear(struct rls_array *a) {
  return rls_array_remove(a, 0, a->count);
}

int rls_array_get_index(struct rls_array *a, void *elem) {
  assert(elem != NULL);
  return ((unsigned char *)elem - (unsigned char *)a->data) / a->data_size;
}

int rls_array_find(struct rls_array *a,
                       const void *key,
                       rls_array_cmp cmp) {
  int index;
  void *cur;

  assert(a != NULL);
  assert(cmp != NULL);

  for (index = 0; index < a->count; index++) {
    cur = rls_array_get(a, index);
    if (cmp(key, cur) == 0) {
      return index;
    }
  }

  return -EINVAL;
}

int rls_array_find_remove(struct rls_array *a,
                              const void *key,
                              rls_array_cmp cmp) {
  int index;
  void *cur;

  assert(a != NULL);
  assert(cmp != NULL);

  for (index = 0; index < a->count; index++) {
    cur = rls_array_get(a, index);
    if (cmp(key, cur) == 0) {
      rls_array_remove(a, index, 1);
      return index;
    }
  }

  return -EINVAL;
}