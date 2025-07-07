#include <stdio.h>

#include "pos.h"
#include "util.h"

int main() {
	const char *src = "This is the first line of the source.\n"
		"This is the second line in the source.\n"
		"hehehe";

	struct pos_t cur = {.idx=0, .ln=1, .col=1};
	int i = 0;
	while (i < 50 && src[i]) {
		pos_next(&cur, src[i++]);
	}

	struct pos_t start = cur;
	while (src[i]) {
		pos_next(&cur, src[i++]);
	}

	print_err("<stdin>", src, "random error msg", start, cur);

	return 0;
}
