#ifndef UTIL_H
#define UTIL_H

#include "pos.h"

/**
 * Print the error based
 *
 * params:
 * 	filename	filename of the source text
 * 	source		source text where error occurred
 * 	err_msg		error message
 * 	err_start	start of the error in the source text (included)
 * 	err_end		end of the error in the source text (excluded)
 */
void print_err(const char *filename, const char *src, const char *err_msg, 
	struct pos_t err_start, struct pos_t err_end);

#endif
