#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "util.h"

// ========================================
// helper function declaration
// ========================================

/**
 * Parser free any errors
 *
 * params:
 * 	self	self pointer
 */
void parser_free_err(struct parser_t *self);

/**
 * Set parse error
 *
 * params:
 * 	self	self pointer
 */
void parser_set_err(struct parser_t *self, int type, const char *msg, 
	struct pos_t start, struct pos_t end);

/**
 * Parse the expr rule
 *
 * params:
 * 	self	self parser pointer
 *
 * returns:
 * 	ast pointer if valid parsing
 * 	NULL if error
 */
struct ast_t *parse_expr(struct parser_t *self);

/**
 * Parse using the term rule
 *
 * params:
 * 	self	self parser pointer
 *
 * returns:
 * 	ast pointer if valid parsing
 * 	NULL if error
 */
struct ast_t *parse_term(struct parser_t *self);

/**
 * Parse using the factor rule
 *
 * params:
 * 	self	self parser pointer
 *
 * returns:
 * 	ast pointer if valid parsing
 * 	NULL if error
 */
struct ast_t *parse_factor(struct parser_t *self);

/**
 * Parse using the primary rule
 *
 * params:
 * 	self	self parser pointer
 *
 * returns:
 * 	ast pointer if valid parsing
 * 	NULL if error
 */
struct ast_t *parse_primary(struct parser_t *self);

// ========================================
// parser.h definition
// ========================================

void parser_init(struct parser_t *self) {
	self->cap = 0;
	self->sz = 0;
	self->tokens = NULL;
	self->cur = 0;

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
	// check if there is any tokens
	if (self->sz == 0) {
		struct pos_t start = (struct pos_t){.idx=0, .ln=1, .col=1};
		parser_set_err(self, PARSER_EOF_ERR, "no tokens", start, start);
		return;
	}

	*dest = parse_expr(self);
}

void parser_free(struct parser_t *self) {
	free(self->tokens);
	parser_free_err(self);
	parser_init(self);
}

void parser_print_err(struct parser_t *self) {
	if (self->err_type == PARSER_NO_ERR) {
		fprintf(stderr, "no parser error.\n");
	}
	else {
		if (self->sz == 0) {
			fprintf(stderr, "ERR: No tokens in the parser");
			return;
		}
		print_err(self->tokens[0].filename, self->tokens[0].src, 
			self->err_msg, self->err_start, self->err_end);
	}
}

// ========================================
// helper function definition
// ========================================

void parser_free_err(struct parser_t *self) {
	assert(self != NULL);

	self->err_type = 0;
	free(self->err_msg);
	self->err_msg = NULL;
}

void parser_set_err(struct parser_t *self, int type, const char *msg, 
	struct pos_t start, struct pos_t end) {
	self->err_type = type;
	
	int len = strlen(msg);
	self->err_msg = malloc(sizeof(char)*(len+1));
	memcpy(self->err_msg, msg, sizeof(char)*(len+1));

	self->err_start = start;
	self->err_end = end;
}


struct ast_t *parse_expr(struct parser_t *self) {
	return parse_term(self);
}

struct ast_t *parse_term(struct parser_t *self) {
	struct ast_t *left = parse_factor(self);
	if (left == NULL) return NULL;
	while (self->cur < self->sz && (self->tokens[self->cur].type == TT_PLUS
		|| self->tokens[self->cur].type == TT_MINUS)) {
		struct token_t op = self->tokens[self->cur++];
		struct ast_t *right = parse_factor(self);
		if (right == NULL) {
			// some error occured while parsing so removing the 
			// left pointer
			ast_free(left);
			return NULL;
		}

		left = ast_binary(left, op, right);
	}
	return left;
}

struct ast_t *parse_factor(struct parser_t *self) {
	struct ast_t *left = parse_primary(self);
	if (left == NULL) return NULL;
	while (self->cur < self->sz && (self->tokens[self->cur].type == TT_STAR 
		|| self->tokens[self->cur].type == TT_FSLASH)) {
		struct token_t op = self->tokens[self->cur++];
		struct ast_t *right = parse_primary(self);
		if (right == NULL) {
			// some error occurred while parsing so removing the
			// left pointer
			ast_free(left);
			return NULL;
		}

		left = ast_binary(left, op, right);
	}
	return left;
}

struct ast_t *parse_primary(struct parser_t *self) {
	if (self->cur >= self->sz) {
		struct pos_t end = self->tokens[self->sz-1].end;
		parser_set_err(self, PARSER_EOF_ERR, "eof", end, end);
		return NULL;
	}

	// check grouping
	if (self->tokens[self->cur].type == TT_LPAREN) {
		struct token_t lparen = self->tokens[self->cur++];
		struct ast_t *expr = parse_expr(self);
		if (expr == NULL) {
			return NULL;
		}
		
		// check if right parenthesis exists
		if (self->cur >= self->sz) {
			struct pos_t end = self->tokens[self->sz-1].end;
			parser_set_err(self, PARSER_EOF_ERR, 
				"expected ')', instead eof", end, end);
			return NULL;
		}
		if (self->tokens[self->cur].type != TT_RPAREN) {
			parser_set_err(self, PARSER_UNEXPECTED_ERR, 
				"expected ')'", self->tokens[self->cur].start,
				self->tokens[self->cur].end);
			ast_free(expr);
			return NULL;
		}

		struct token_t rparen = self->tokens[self->cur++];
		return ast_group(lparen, expr, rparen);
	}

	// check literal
	if (self->tokens[self->cur].type == TT_DECIMAL ||
		self->tokens[self->cur].type == TT_BINARY ||
		self->tokens[self->cur].type == TT_OCTAL ||
		self->tokens[self->cur].type == TT_HEXADECIMAL) {
		return ast_literal(self->tokens[self->cur++]);
	}

	parser_set_err(self, PARSER_UNEXPECTED_ERR, "Unexpected token",
		self->tokens[self->cur].start, self->tokens[self->cur].end);
	return NULL;
}


