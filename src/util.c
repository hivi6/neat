#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

// check if the color is enabled
int color_enabled = 0;

#define COLOR(code) 	(color_enabled ? code : "")
#define RED		COLOR("\x1b[1;31m")
#define GREEN		COLOR("\x1b[1;32m")
#define RESET		COLOR("\x1b[0m")

// ========================================
// util.h definition
// ========================================

void print_err(const char *filename, const char *src, const char *err_msg, 
	struct pos_t err_start, struct pos_t err_end) {
	// check if the color is enabled
	color_enabled = isatty(fileno(stderr));

	int src_len = strlen(src);
	fprintf(stderr, "%s:%d:%d: %serror%s: %s\n", filename, err_start.ln, 
		err_start.col, RED, RESET, err_msg);

	// calculate the start of the line
	int line_start_idx = err_start.idx;
	while (line_start_idx > 0 && src[line_start_idx-1] != '\n')
		line_start_idx--;

	// print the error source text
	int i = line_start_idx;
	int cnt = err_start.ln;
	fprintf(stderr, " %d\t|\t", cnt);
	while (cnt <= err_end.ln && i < src_len) {
		if (src[i] == '\n') {
			cnt++;
			if (cnt <= err_end.ln)
				fprintf(stderr, "\n %d\t>\t", cnt);
		}
		else {
			const char *color = (err_start.idx <= i && 
				i <= err_end.idx ? GREEN : RESET);
			fprintf(stderr, "%s%c%s", color, src[i], RESET);
		}
		i++;
	}
	fprintf(stderr, "\n");
}
