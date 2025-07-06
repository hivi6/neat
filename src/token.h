#ifndef TOKEN_H
#define TOKEN_H

#include "pos.h"

// ========================================
// Token types
// ========================================

enum {
	TT_PLUS,	// +
	TT_MINUS,	// -
	TT_STAR,	// *
	TT_FSLASH,	// /
	TT_LPAREN,	// (
	TT_RPAREN,	// )
	TT_DECIMAL	// [1-9][0-9]*
};

// ========================================
// Token
// ========================================

/**
 * token_t
 *
 * Keep track of the token and the lexical generated
 *
 * members:
 * 	type		type of the token
 * 	filename	filename of the source text
 * 	src		source text where lexical is present (NOT OWNED)
 * 	start		start position of the lexical in the source (included)
 * 	end		end position of the lexical in the source (excluded)
 */
struct token_t {
	int type;

	const char *filename;
	const char *src;
	struct pos_t start;
	struct pos_t end;
};

/**
 * Print the token
 *
 * members:
 * 	type	type of the token
 */
void token_print(struct token_t *self);

#endif // TOKEN_H
