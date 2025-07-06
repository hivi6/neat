#include <stdio.h>
#include <string.h>

#include "util.h"

// ========================================
// util.h definition
// ========================================

void print_err(const char *filename, const char *src, const char *err_msg, 
	struct pos_t err_start, struct pos_t err_end) {
	int src_len = strlen(src);
	printf("%s:%d:%d: error: %s\n", filename, err_start.ln, err_start.col, 
		err_msg);

	// calculate the start of the line
	int line_start_idx = err_start.idx;
	while (line_start_idx > 0 && src[line_start_idx-1] != '\n')
		line_start_idx--;

	// print the error source text
	int i = line_start_idx;
	int cnt = err_start.ln;
	printf("%d | ", cnt);
	while (cnt <= err_end.ln && i < src_len) {
		if (src[i] == '\n') {
			cnt++;
			printf("\n%d | ", cnt);
		}
		else {
			printf("%c", src[i]);
		}
		i++;
	}
	printf("\n");
}
