#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "pos.h"
#include "util.h"

// ========================================
// helper function declaration
// ========================================

/**
 * Free the error in the lexer
 *
 * params:
 * 	self	self pointer
 */
void lexer_free_err(struct lexer_t *self);

/**
 * Set the current error status in the lexer
 *
 * params:
 * 	self	self pointer
 * 	type	type of the error
 * 	msg	message for the error
 * 	start	start of the error (included)
 *	end	end of the error (excluded)
 */
int lexer_set_err(struct lexer_t *self, int type, const char *msg, 
	struct pos_t start, struct pos_t end);


// ========================================
// lexer.h definition
// ========================================

void lexer_init(struct lexer_t *self, const char *filename, const char *src) {
	assert(self != NULL && src != NULL);

	self->filename = filename;
	self->src = src;
	self->len = strlen(src);
	self->cur = (struct pos_t){.idx=0, .ln=1, .col=1};
	self->err_type = LEXER_NO_ERR;
	self->err_msg = NULL;
}

void lexer_free(struct lexer_t *self) {
	self->src = NULL;
	lexer_free_err(self);
}

int lexer_next(struct lexer_t *self, struct token_t *dest) {
	assert(self != NULL);

	// free any previous error
	lexer_free_err(self);

	// ignore whitespaces
	while (self->cur.idx < self->len && isspace(self->src[self->cur.idx]))
		pos_next(&self->cur, self->src[self->cur.idx]);

	// check if end of the source text reached
	if (self->cur.idx >= self->len)
		return lexer_set_err(self, LEXER_EOF_ERR, "eof", self->cur,
			self->cur);
	
	struct token_t res;
	struct pos_t start = self->cur;
	char first = self->src[start.idx];
	pos_next(&self->cur, first);

	// check for single character tokens
	if (first == '+' || first == '-' || first == '*' || first == '/' ||
		first == '(' || first == ')') {
		int type = -1;
		if (first == '+') type = TT_PLUS;
		else if (first == '-') type = TT_MINUS;
		else if (first == '*') type = TT_STAR;
		else if (first == '/') type = TT_FSLASH;
		else if (first == '(') type = TT_LPAREN;
		else if (first == ')') type = TT_RPAREN;
		res = (struct token_t){.src=self->src, .type=type, .start=start, 
			.end=self->cur, .filename=self->filename};
	}
	// check for decimal numbers
	else if ('1' <= first && first <= '9') {
		while (self->cur.idx < self->len && 
			isdigit(self->src[self->cur.idx]))
			pos_next(&self->cur, self->src[self->cur.idx]);
	
		res = (struct token_t){.src=self->src, .type=TT_DECIMAL, 
			.start=start, .end=self->cur, .filename=self->filename};
	}
	// for unexpected characters
	else {
		char msg[256];
		sprintf(msg, "unexpected character '%c'", first);
		return lexer_set_err(self, LEXER_UNEXPECTED_ERR, msg, start,
			self->cur);
	}

	if (dest != NULL) *dest = res;

	return LEXER_NO_ERR;
}

void lexer_print_err(struct lexer_t *self) {
	if (self->err_type == LEXER_NO_ERR) {
		printf("no lexer error.");
	}
	else {
		print_err(self->filename, self->src, self->err_msg, 
			self->err_start, self->err_end);
	}
}

// ========================================
// helper function definitions
// ========================================

void lexer_free_err(struct lexer_t *self) {
	assert(self != NULL);
	self->err_type = 0;
	free(self->err_msg);
	self->err_msg = NULL;
}

int lexer_set_err(struct lexer_t *self, int type, const char *msg, 
	struct pos_t start, struct pos_t end) {
	self->err_type = type;
	
	int len = strlen(msg);
	self->err_msg = malloc(sizeof(char)*(len+1));
	memcpy(self->err_msg, msg, sizeof(char)*(len+1));

	self->err_start = start;
	self->err_end = end;

	return type;
}
