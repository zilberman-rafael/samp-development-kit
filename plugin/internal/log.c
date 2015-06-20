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

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "module.h"
#include "logprintf.h"

enum _rls_log_status {
  _RLS_LOG_DEBUG,
  _RLS_LOG_INFO,
  _RLS_LOG_WARNING,
  _RLS_LOG_ERROR
};

char _rls_log_status_enabled = 0xC; //0b1100

void rls_log_status_enable(int status) {
  assert(status >= _RLS_LOG_DEBUG &&
         status <= _RLS_LOG_ERROR);
		 
  _rls_log_status_enabled |= (1 << status);
}

void rls_log_status_disable(int status) {
  assert(status >= _RLS_LOG_DEBUG &&
         status <= _RLS_LOG_ERROR);
		 
  _rls_log_status_enabled &= ~(1 << status);
}

static void _rls_log_printf(int status, const char *format, va_list args) { 
	const char *status_format;
	char *message_format;

  assert(status >= _RLS_LOG_DEBUG &&
         status <= _RLS_LOG_ERROR);
	
	if(!(_rls_log_status_enabled & (1 << status)))
	{
		return;
	}
	
	switch (status) {
		case _RLS_LOG_DEBUG:
			status_format = "debug";
			break;
		case _RLS_LOG_INFO:
			status_format = "info";
			break;
		case _RLS_LOG_WARNING:
			status_format = "warning";
			break;
		case _RLS_LOG_ERROR:
			status_format = "error";
			break;
		default:
			status_format = "";
	}
	
	message_format = malloc(
			sizeof("[rls **]: ") - 1 + 
			strlen(status_format) + 
			strlen(format) + 
			1
		);
		
	if (message_format == NULL) {
		return /*-ENOMEM*/;
	}
		
	strcpy(message_format, "[rls *");
	strcat(message_format, status_format);
	strcat(message_format, "*]: ");
	strcat(message_format, format);
	
	rls_printf(message_format, args);
	
	free(message_format);
}

void rls_log_debug(const char *format, ...) {
	va_list args;
	va_start(args, format);
	_rls_log_printf(_RLS_LOG_DEBUG, format, args);
	va_end(args);
}

void rls_log_info(const char* format, ...) {
	va_list args;
	va_start(args, format);
	_rls_log_printf(_RLS_LOG_INFO, format, args);
	va_end(args);
}

void rls_log_warning(const char* format, ...) {
	va_list args;
	va_start(args, format);
	_rls_log_printf(_RLS_LOG_WARNING, format, args);
	va_end(args);
}

void rls_log_error(const char* format, ...) {
	va_list args;
	va_start(args, format);
	_rls_log_printf(_RLS_LOG__ERROR, format, args);
	va_end(args);
}

void rls_log_error_code(int error_code, const char* format, ...) {
	char *message_format;
	va_list args;
	
	assert(error_code > 0);
	
	va_start(args, format);
	
	message_format = malloc(
			strlen(format) +
			sizeof(" ()") - 1 +
			1
		);
		
	strcpy(message_format, format);
	strcat(message_format, " (");
	strcat(message_format, strerror(error_code));
	strcat(message_format, ")");
	
	_rls_log_printf(_RLS_LOG__ERROR, message_format, args);
	
	va_end(args);
}

RLS_MODULE_INIT(log) {
	#if DEBUG
		rls_log_status_enable(_RLS_LOG_DEBUG);
	#else
		rls_log_status_disable(_RLS_LOG_DEBUG);
	#endif
}

RLS_MODULE_CLEANUP(log) {
  /* nothing to do here */
}