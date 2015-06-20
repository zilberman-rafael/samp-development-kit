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

#ifndef _RLS_@INTERNAL_@@CALLBACK_H
#define _RLS_@INTERNAL_@@CALLBACK_H

#include <rls/bool.h>
#include <rls/sdk.h>

/* Callback handler function. */
typedef bool (*rls_callback)(AMX *amx, cell *retval);

/* Register and unregister a callback in the global callback table.
 *
 * This is usually done only from generated init and cleanup functions of a_*
 * modules. Currently the only module that defines callbacks is a_samp.
 */
int rls_callback_register(const char *name, rls_callback handler);
void rls_callback_unregister(const char *name);

/* Gets the name of the callback with the specified index,
 * similar to amx_GetPublic().
 */
bool rls_callback_get(int index, char **name);

/* Executes the callback handler registered for the specified callback.
 *
 * The return value indicates whether the callback returned a "bad" value,
 * i.e. whether the gamemode is allowed to execute the associated public
 * function. This value is specified in the IDL files via the callback's
 * "badret" attribute.
 */
bool rls_callback_invoke(AMX *amx, const char *name, cell *retval);

#endif /* !_RLS_@INTERNAL_@@CALLBACK_H */