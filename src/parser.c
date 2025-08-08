#include <stdlib.h>

#include "parser.h"

// ========================================
// parser.h definition
// ========================================

void parser_init(struct parser_t *self) {
	self->cap = 0;
	self->sz = 0;
	self->tokens = NULL;

	self->err_type = PARSER_NO_ERR;
	self->err_msg = NULL;
}

void parser_append(struct parser_t *self, struct token_t token) {
	// increase the size of the tokens array if capacity is less
	if (self->cap <= self->sz+1) {
		self->cap = (self->cap+1)*2;
		self->tokens = realloc(self->tokens, 
			self->cap*sizeof(struct token_t));
	}

	self->tokens[self->sz++] = token;
}

void parser_parse(struct parser_t *self, struct ast_t **dest) {
	// TODO: Complete the parse functionality
}

void parser_free(struct parser_t *self) {
	free(self->tokens);
	free(self->err_msg);

	parser_init(self);
}

