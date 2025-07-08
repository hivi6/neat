#include <assert.h>
#include <stdio.h>

#include "token.h"

void token_print(struct token_t *self) {
	static char *TOKEN_NAMES[] = {
		"TT_PLUS",
		"TT_MINUS",
		"TT_STAR",
		"TT_FSLASH",
		"TT_LPAREN",
		"TT_RPAREN",
		"TT_DECIMAL",
		"TT_OCTAL",
		"TT_BINARY",
		"TT_HEXADECIMAL",
	};

	printf("{ type: %s, filename: %s, lexical: '", TOKEN_NAMES[self->type], 
		self->filename);
	for (int i = self->start.idx; i < self->end.idx; i++) {
		printf("%c", self->src[i]);
	}
	printf("' }");
}
