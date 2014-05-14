#include "defs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


/*
 * this prototype only use a big switch/case 
 * to handle the events (function output_json() and output_orig() )
 * 
 * of course we can choose a more extensible design 
 * for example the table below ( json_template ) give us a hint
 */

enum out_type { USE_FMT, USE_FUN };
struct {
	output_event_t event;
	enum out_type type;
	const char *fmt;
	int (*fun)(output_event_t event, const char *str, long extra);
} json_template[EVENT_COUNT] = 
	{
		{EVENT_NONE,            USE_FMT, "", NULL },

		{EVENT_CALL_BEGIN,      USE_FMT, "\"Type\" : \"syscall\",\n"
									   "\"Name\" : \"%s\",\n", NULL },
		{EVENT_CALL_ARG,        USE_FMT, "\"Args%ld\" : \"%s\",\n", NULL },
		{EVENT_CALL_END,        USE_FMT, "", NULL },
		{EVENT_CALL_RET_ERRNO,  USE_FMT, "\"Errno\" : \"%s\",\n", NULL },
		{EVENT_CALL_RET_CODE,   USE_FMT, "\n\"Return\" : \"%s\",\n", NULL },
		{EVENT_CALL_RET_DESC,   USE_FMT, "\"Describe\" : \"%s\",\n", NULL },

		{EVENT_ALL_BEGIN,       USE_FMT, "", NULL },
		{EVENT_ALL_END,         USE_FMT, "", NULL },

		{EVENT_ENTRY_BEGIN,     USE_FMT, "{\n", NULL },
		{EVENT_ENTRY_END,       USE_FMT, "}\n", NULL },

		{EVENT_TIME_SPENT,      USE_FMT, "\"TimeSpent\" : \"%s\",\n", NULL },
		{EVENT_TIME_RELATIVE,   USE_FMT, "\"RelativeTime\" : \"%s\",\n", NULL },

		{EVENT_INSTRUCTION_POINTER,  USE_FMT, "\"Pointer\" : \"%s\",\n", NULL },
	};


int output_json(output_event_t event, const char *str, long extra)
{
	switch ( event ) {
		case EVENT_NONE:
			break;

		case EVENT_CALL_BEGIN:
			tprintf("\"Type\" : \"syscall\",\n");
			tprintf("\"Name\" : \"%s\",\n", str);
			tprintf("\"Args\" : [ ");
			break;

		case EVENT_CALL_ARG:
			tprintf("\"%s\", ",  str);
			break;

		case EVENT_CALL_END:
			tprints( " ],\n" );
			break;

		case EVENT_CALL_RET_ERRNO:
			tprintf("\"Errno\" : \"%s\",\n", str);
			break;

		case EVENT_CALL_RET_CODE:
			tprintf("\"Return\" : \"%s\",\n", str);
			break;

		case EVENT_CALL_RET_DESC:
			tprintf("\"Describe\" : \"%s\",\n", str);
			break;

		case EVENT_ALL_BEGIN:
		case EVENT_ALL_END:
			break;

	   case EVENT_ENTRY_BEGIN:
			tprints("{\n");
			break;
		case EVENT_ENTRY_END:
			tprints("}\n");
			break;

		case EVENT_TIME_SPENT:
			tprintf("\"TimeSpent\" : \"%s\",\n", str);
			break;

		case EVENT_TIME_RELATIVE:
			tprintf("\"TimeRelative\" : \"%s\",\n", str);
			break;

		case EVENT_INSTRUCTION_POINTER:
			tprintf("\"Pointer\" : \"%s\",\n", str);
			break;

		default:
			break;
	}
	return 0;
}

int output_orig(output_event_t event, const char *str, long extra)
{
	switch ( event ) {
		case EVENT_NONE:
		  break;

		case EVENT_CALL_ARG:
		  tprintf(extra == 1 ? "%s" : ", %s", str);
		  break;

		case EVENT_CALL_END:
		  tprints(") ");
		  tabto();
		  break;

		case EVENT_CALL_RET_CODE:
		  tprintf("= %s ", str);
		  break;

		case EVENT_CALL_RET_ERRNO:
		  tprintf("(errno %s) ", str);
		  break;

		case EVENT_CALL_RET_DESC:
		  tprintf("%s ", str);
		  break;

		case EVENT_ALL_BEGIN:
		case EVENT_ALL_END:
		  break;

		case EVENT_ENTRY_END:
		  tprints("\n");
		  break;

		case EVENT_CALL_BEGIN:
		  tprintf("%s(", str);
		  break;

		case EVENT_TIME_SPENT:
		  tprintf(" <%s>", str);
		  break;

		case EVENT_TIME_RELATIVE:
		  tprintf("%s ", str);
		  break;

		case EVENT_INSTRUCTION_POINTER:
		  tprintf("[%s] ", str);
		  break;

		default:
		  break;
	}
	return 0;
}

/*
 * high-level function prepare the meta-data 
 * and then call this entry function to do the print-staff
 * NOTE: the interface need to be more extensible
 * and currently we only have a very simple interface
 * (a string representing the meta-data)
 * 
 */
int output_trace(output_event_t event, const char *str, long extra)
{
	switch( jformat ) {
		case FORMAT_JSON:
			return output_json(event, str, extra);
			break;

		case FORMAT_ORIG:
			/* fall through to default output format */
		default:
			return output_orig(event, str, extra);
			break;
	}
	return 0;
}