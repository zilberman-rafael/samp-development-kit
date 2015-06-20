/**--------------------------------------------------------------------------**\
Author:
  Zeex, Zilberman Rafael from SA-MP.co.il
  
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
	
Special thanks to:
		Amit_B - ...
    Zexx - ...
	
Version:
	0.1
  
Changelog:
	17/06/2014:
		First version.
\**--------------------------------------------------------------------------**/

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "callback.h"
#include "module.h"
#include "log.h"

struct _rls_callback_info {
  char *name;
  void *handler;
};

static struct rls_array _rls_callbacks;

RLS_MODULE_INIT(callback) {
  int error;

  error = rls_array_init(&_rls_callbacks,
                            1,
                            sizeof(struct _rls_callback_info));
  if (error < 0) {
    return error;
  }

  return 0;
}

RLS_MODULE_CLEANUP(callback) {
  int i;
  struct _rls_callback_info *callback;

  for (i = 0; i < _rls_callbacks.count; i++) {
    callback = rls_array_get(&_rls_callbacks, i);
    free(callback->name);
  }

  rls_array_dispose(&_rls_callbacks);
}

static int _rls_callback_compare_name(const void *key,
                                          const void *elem) {
  assert(key != NULL);
  assert(elem != NULL);
  return strcmp((const char *)key,
                ((const struct _rls_callback_info *)elem)->name);
}

static struct _rls_callback_info *_rls_callback_find(const char *name) {
  assert(name != NULL);

  if (_rls_callbacks.count == 0) {
    return NULL;
  }

  return bsearch(name,
                 _rls_callbacks.data,
                 _rls_callbacks.count - 1,
                 _rls_callbacks.elem_size,
                 _rls_callback_compare_name);
}

int rls_callback_register(const char *name,
                              rls_callback handler) {
  int error;
  int i;
  struct _rls_callback_info callback;
  struct _rls_callback_info *ptr;

  assert(name != NULL);

  if ((ptr = _rls_callback_find(name)) != NULL) {
    return rls_array_get_index(&_rls_callbacks, ptr);
  }

  if ((callback.name = malloc(strlen(name) + 1)) == NULL) {
    return -ENOMEM;
  }

  callback.handler = handler;
  strcpy(callback.name, name);

  /* Keep callbacks ordered by name.
   * This allows us to use binary search in rls_callback_find().
   */
  for (i = 0; i < _rls_callbacks.count - 1; i++) {
    ptr = rls_array_get(&_rls_callbacks, i);
    if (strcmp(name, ptr->name) <= 0) {
      break;
    }
  }

  if ((error = rls_array_insert(&_rls_callbacks, i, 1, &callback)) < 0) {
    free(callback.name);
    return error;
  }

  return error; /* index */
}

void rls_callback_unregister(const char *name) {
  struct _rls_callback_info *callback;

  if ((callback = _rls_callback_find(name)) != NULL) {
    callback->handler = NULL;
  }
}

bool rls_callback_get(int index, char **name) {
  struct _rls_callback_info *callback;

  assert(name != NULL);

  if (index < 0 || index >= _rls_callbacks.count) {
    return false;
  }

  callback = rls_array_get(&_rls_callbacks, index);
  *name = callback->name;

  return true;
}

bool rls_callback_invoke(AMX *amx, const char *name, cell *retval)
{
  struct _rls_callback_info *callback;

  assert(amx != NULL);
  
  if ((callback = _rls_callback_find(name)) != NULL && 
        callback->handler != NULL && 
        !((rls_callback)callback->handler)(amx, retval)
      ) {
    return false;
  }

  return true;
}