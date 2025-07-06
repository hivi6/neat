#include <assert.h>
#include <stdio.h>

#include "token.h"

void token_print(struct token_t *self) {
	printf("{ type: %d, filename: %s, lexical: '", self->type, 
		self->filename);
	for (int i = self->start.idx; i < self->end.idx; i++) {
		printf("%c", self->src[i]);
	}
	printf("' }");
}
