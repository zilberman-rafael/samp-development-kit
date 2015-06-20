/*
*	Description:
*		Provides functions for bit manipulation and bit arrays greater than 32bits.
*		The arrays are usually bigger than required due to cell boundaries but this
*		shouldn't cause a major problem (bit tests on the 101st bit of a 100 bit
*		array won't return 0 for out of bounds, but the 129th will).
*
*		Note that y_commands has a few optimisations which bypass the code in here
*		so any modifications to bit array layouts will need to be reflected there.
*/

#include <stdlib.h>
#include <string.h>
#include "logprintf.h"

#ifndef RLS_INTERNAL_LOG
#define RLS_INTERNAL_LOG

/* disclaimer log functions */
char* rls_log_debug(char *format);
void rls_log_info(const char *format, ...);
void rls_log_warning(const char *format, ...);
void rls_log_error(const char *format, ...);

typedef enum { RLS_DEBUG, RLS_INFO, RLS_ERROR, RLS_WARNING } status; /* rls log printing situations */
static void rls_log_printf(status s, const char *format, va_list args) { /* main function are report data were */
	char *szString = "";
	char *szStatus = "";
	switch (s) {
	case RLS_DEBUG:
		szStatus = "[rls_Debug]: ";
		break;
	case RLS_ERROR:
		szStatus = "[rls_Error]: ";
		break;
	case RLS_INFO:
		szStatus = "[rls_Info]: ";
		break;
	case RLS_WARNING:
		szStatus = "[rls_Warning]: ";
		break;
	}
	szString = (char*)malloc(sizeof("[rls_Warning]: ") - 1 + strlen(szStatus) + strlen(format) + 1);
	strcpy(szString, szStatus);
	strcat(szString, format);
	rls_logprintf(szString, args);
	free(szString);
}

void rls_log_debug(const char *format, ...) {
	va_list args;
	va_start(args, format);
	rls_log_printf(RLS_DEBUG, format, args);
	va_end(args);
}

void rls_log_info(const char* format, ...) {
	va_list args;
	va_start(args, format);
	rls_log_printf(RLS_INFO, format, args);
	va_end(args);
}

void rls_log_warning(const char* format, ...) {
	va_list args;
	va_start(args, format);
	rls_log_printf(RLS_WARNING, format, args);
	va_end(args);
}

void rls_log_error(const char* format, ...) {
	va_list args;
	va_start(args, format);
	rls_log_printf(RLS_ERROR, format, args);
	va_end(args);
}

#endif /* !RLS_INTERNAL_LOG */
