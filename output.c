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

int redirect_output_flag = 0;
FILE *mock_file = NULL;
static char *mock_buf  = NULL;
static size_t mock_size = 0;

void output_enable_redirect()
{
	if (jformat == FORMAT_ORIG)
		return;
	
	redirect_output_flag = 1;
	if (mock_file == NULL) {
			mock_file = open_memstream(&mock_buf, &mock_size);
			if (mock_file == NULL) {
				tprints("json output fatal error!\n");
				return;
			}
	}
	
	rewind(mock_file);
	fflush(mock_file);
	return;
}
void output_disable_redirect()
{
	if (jformat == FORMAT_ORIG)
		return;
	
	redirect_output_flag = 0;
	rewind(mock_file);
	fflush(mock_file);
	// we never fclose(mock_file), so we do net need free(mock_buf)
	// free(mock_buf);
	return;
}
void output_begin_meta()
{
	if (jformat == FORMAT_ORIG)
		return;
	
	// clean all current buffer
	// prepare for the next output
	rewind(mock_file);
	fflush(mock_file);
	redirect_output_flag = 1;
	return;
}
/*
 * high-level function prepare the meta-data 
 * and then call this entry function to do the print-staff
 * NOTE: the interface need to be more extensible
 * and currently we only have a very simple interface
 * (a string representing the meta-data)
 * 
 */
int output_event(output_event_t event, long extra)
{
	if (jformat == FORMAT_ORIG)
		return 0;
	
	fflush(mock_file);
	redirect_output_flag = 0;
	
	output_json(event, mock_buf, extra);
	rewind(mock_file);
	
	return 0;
}