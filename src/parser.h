#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast.h"

// ========================================
// Parser error types
// ========================================

enum {
	PARSER_NO_ERR = 0, 	// no error
	PARSER_EOF_ERR,		// end of parsing error
	PARSER_UNEXPECTED_ERR,	// unexpected token error
};

// ========================================
// Parser type
// ========================================

/**
 * parser_t
 *
 * members:
 * 	cap		capacity of the tokens array
 * 	sz		size of the tokens array
 * 	tokens		array containing all the tokens (OWNED)
 * 	cur		current token index while parsing
 * 	err_type	type of error, if occured
 * 	err_msg		any error message with parsing (OWNED)
 * 	err_start	start position of the error
 * 	err_end		end position of the error
 */
struct parser_t {
	int cap, sz;
	struct token_t *tokens;
	int cur;

	int err_type;
	char *err_msg;
	struct pos_t err_start;
	struct pos_t err_end;
};

/**
 * Initialize the parser
 *
 * params:
 * 	self	self pointer
 */
void parser_init(struct parser_t *self);

/**
 * Append a new token to the parser
 *
 * params:
 * 	self	self pointer
 * 	token	token that needs to be appended
 */
void parser_append(struct parser_t *self, struct token_t token);

/**
 * Start the parser
 *
 * params:
 * 	self	self pointer
 * 	dest	create the ast and apply to the destination pointer
 */
void parser_parse(struct parser_t *self, struct ast_t **dest);

/**
 * Free any data related to the parser
 *
 * params:
 * 	self	self pointer
 */
void parser_free(struct parser_t *self);

/**
 * Print the current error of the parser, if any
 *
 * params:
 * 	self	self pointer
 */
void parser_print_err(struct parser_t *self);

#endif // PARSER_H
