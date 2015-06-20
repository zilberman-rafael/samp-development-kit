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

#include "../internal/callback.h"
#include "../internal/module.h"
#include "../internal/log.h"

#include "command.h"

struct _rls_command_info {
  char *name;
  int handler_index;
  struct rls_array players;
};

static struct rls_array _rls_commands;

static bool OnPlayerCommandText(AMX *amx, cell *retval) {
  int playerid;
  const char * cmdtext;
  rls_param_get_cell(amx, 0, (void *)&playerid);
  rls_param_get_string(amx, 1, (void *)&cmdtext);
  
  //
  
  free((void *)cmdtext);
  
  *retval = (cell)true;
  return false;
}

RLS_MODULE_INIT(command) {
  int error;
  
  if ((error = rls_callback_register("OnPlayerCommandText", OnPlayerCommandText)) < 0) {
    return error;
  }
  
  error = rls_array_init(&_rls_commands,
                            1,
                            sizeof(struct _rls_command_info));
  if (error < 0) {
    return error;
  }
  
  return 0;
}

RLS_MODULE_CLEANUP(command) {
  /* nothing to do here */
}

static int _rls_command_compare_name(const void *key,
                                          const void *elem) {
  assert(key != NULL);
  assert(elem != NULL);
  return strcmp((const char *)key,
                ((const struct _rls_command_info *)elem)->name);
}

static struct _rls_command_info *_rls_command_find(const char *name) {
  assert(name != NULL);

  if (_rls_command.count == 0) {
    return NULL;
  }

  return bsearch(name,
                 _rls_command.data,
                 _rls_command.count - 1,
                 _rls_command.elem_size,
                 _rls_command_compare_name);
}

int rls_command_register(const char *name, int handler_index) {
  int error;
  int i;
  struct _rls_command_info command;
  struct _rls_command_info *ptr;

  assert(name != NULL);

  if ((ptr = _rls_command_find(name)) != NULL) {
    return rls_array_get_index(&_rls_commands, ptr);
  }

  if ((command.name = malloc(strlen(name) + 1)) == NULL) {
    return -ENOMEM;
  }

  command.handler_index = handler_index;
  strcpy(command.name, name);

  /* Keep commands ordered by name.
   * This allows us to use binary search in rls_callback_find().
   */
  for (i = 0; i < _rls_commands.count - 1; i++) {
    ptr = rls_array_get(&_rls_commands, i);
    if (strcmp(name, ptr->name) <= 0) {
      break;
    }
  }

  if ((error = rls_array_insert(&_rls_commands, i, 1, &callback)) < 0) {
    free(command.name);
    return error;
  }

  return error; /* index */
}

void rls_command_register_amx(AMX *amx) {
  
}

void rls_command_unregister(const char *name) {
  rls_array_find_remove(&_rls_commands, name, 
        _rls_command_compare_name);
}

bool rls_command_get(int index, char **name) {
  struct _rls_command_info *command;

  assert(name != NULL);

  if (index < 0 || index >= _rls_commands.count) {
    return false;
  }

  command = rls_array_get(&_rls_commands, index);
  *name = command->name;

  return true;
}

bool rls_command_invoke(AMX *amx, const char *name, int playerid, const char *params)
{
  cell retval;
  struct _rls_command_info *command;

  assert(amx != NULL);
  
  if ((command = _rls_command_find(name)) == NULL || 
        command->handler_index < 0) {
    return false;
  }
  
  amx_PushString(amx, NULL, NULL, params, NULL, NULL);
  amx_Push(amx, playerid);
  
  amx_Exec(amx, &retval, command->handler_index);

  return retval;
}