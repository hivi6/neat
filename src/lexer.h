#ifndef LEXER_H
#define LEXER_H

#include "pos.h"
#include "token.h"

// ========================================
// Lexer error types
// ========================================

enum {
	LEXER_NO_ERR = 0,	// No error
	LEXER_EOF_ERR,		// end of file error
	LEXER_UNEXPECTED_ERR,	// unexpected character error
};

// ========================================
// Lexer
// ========================================

/**
 * Lexer
 *
 * Keep track of the tokenization
 *
 * members:
 * 	filename	filename of the source text (NOT OWNED)
 * 	src		source text where tokenization is done (NOT OWNED)
 * 	len		length of the source text
 * 	cur		current position in the source text
 * 	err_type	error type, if any (defaults to 0)
 * 	err_msg		error message, if any (OWNED)
 * 	err_start	start position of the error in the source text (inc)
 * 	err_end		end position of the error in the source text (excluded)
 */
struct lexer_t {
	const char *filename;
	const char *src;
	int len;
	struct pos_t cur;

	int err_type;
	char *err_msg;
	struct pos_t err_start;
	struct pos_t err_end;
};

/**
 * Initialize the lexer with the source text
 *
 * params:
 * 	self		self pointer
 * 	filename	filename of the source text
 * 	src		source text
 */
void lexer_init(struct lexer_t *self, const char *filename, const char *src);

/**
 * free the lexer data and any dangling references to other data
 *
 * params:
 * 	self	self pointer
 */
void lexer_free(struct lexer_t *self);

/**
 * get the next token
 *
 * params:
 * 	self	self pointer
 * 	dest	destination where the token is set
 *
 * returns:
 * 	non-zero int means lexer error
 */
int lexer_next(struct lexer_t *self, struct token_t *dest);

/**
 * print the lexer error
 *
 * params:
 * 	self	self pointer
 */
void lexer_print_err(struct lexer_t *self);

#endif // LEXER_H
