#include <stdlib.h>

#include "parser.h"

// ========================================
// helper function declaration
// ========================================

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
	*dest = parse_expr(self);
}

void parser_free(struct parser_t *self) {
	free(self->tokens);
	free(self->err_msg);

	parser_init(self);
}

// ========================================
// helper function definition
// ========================================

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
		if (right == NULL) break;

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
		if (right == NULL) break;

		left = ast_binary(left, op, right);
	}
	return left;
}

struct ast_t *parse_primary(struct parser_t *self) {
	if (self->cur >= self->sz) {
		// TODO: Handle error
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
		if (self->cur >= self->sz || 
			self->tokens[self->cur].type != TT_RPAREN) {
			// TODO: Handle error better
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

	// TODO: handle error
	return NULL;
}
