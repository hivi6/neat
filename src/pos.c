#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "pos.h"

// ========================================
// pos.h definition
// ========================================

void pos_next(struct pos_t *self, char ch) {
	assert(self != NULL);

	// don't move
	if (ch == 0) return;

	// check for newline
	if (ch == '\n') {
		self->ln++;
		self->col = 0;
	}
	self->idx++;
	self->col++;
}

void pos_print(struct pos_t *self) {
	assert(self != NULL);

	printf("{ idx: %d, line: %d, column: %d }", self->idx, self->ln, 
		self->col);
}
