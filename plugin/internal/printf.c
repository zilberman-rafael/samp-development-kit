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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <sampgdk/platform.h>

#include "printf.h"

#define _RLS_LOGPRINTF_BUF_SIZE 1024

#ifdef _MSC_VER
  #define vsnprintf vsprintf_s
#endif

typedef void (RLS_CDECL *printf_t)(const char *format, ...);

/* Gets called before the library is initialized. */
static void _rls_printf_stub(const char *format, ...) {
  va_list va;
  va_start(va, format);
  vprintf(format, va);
  printf("\n");
  va_end(va);
}

void *_rls_printf_impl = &_sampgdk_printf_stub;

void rls_printf(const char *format, ...) {
  char buffer[_SAMPGDK_LOGPRINTF_BUF_SIZE];
  va_list va;
  va_start(va, format);
  vsnprintf(buffer, sizeof(buffer), format, va);
  buffer[sizeof(buffer) - 1] = '\0';
  ((logprintf_t)_rls_printf_impl)("%s", buffer);
}