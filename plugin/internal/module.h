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

#ifndef _RLS_@INTERNAL_@@MODULE_H
#define _RLS_@INTERNAL_@@MODULE_H

#define RLS_MODULE_INIT(module) \
  int rls_##module##_init(void)

#define RLS_MODULE_CLEANUP(module) \
  void rls_##module##_cleanup(void)

int  rls_module_init(void);
void rls_module_cleanup(void);

#endif /* !_RLS_@INTERNAL_@@MODULE_H */