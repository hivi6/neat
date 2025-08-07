#ifndef AST_H
#define AST_H

#include "token.h"

// ========================================
// Ast types
// ========================================

enum {
	AST_LITERAL,
	AST_GROUP,
	AST_BINARY,
};

// ========================================
// Ast 
// ========================================

/**
 * ast_t
 *
 * Abstract syntax tree
 * All ast pointers are owned
 *
 * members:
 * 	type	type of ast
 * 	start 	left-most token of the ast (used for positional information)
 * 	end 	right-most token of the ast (used for positional information)
 * 	value	value of ast
 */
struct ast_t {
	int type;
	struct token_t start;
	struct token_t end;
	union {
		// literal value
		struct { 
			struct token_t token; 
		} literal;

		// group value
		struct {
			struct token_t lparen;
			struct ast_t *expr;
			struct token_t rparen;
		} group;

		// binary value 
		struct {
			struct ast_t *left;
			struct token_t op;
			struct ast_t *right;
		} binary;
	} value;
};

/**
 * Allocate and create a literal ast pointer
 *
 * params:
 *	token	token used to create a literal ast
 *
 * returns:
 * 	pointer to the memory allocation
 * 	returns NULL if any error
 */
struct ast_t *ast_literal(struct token_t token);

/**
 * Allocate and create a group ast pointer
 *
 * params:
 * 	lparen	token to left parenthesis
 * 	expr	pointer to the expression inside the parenthesis
 * 	rparen	token to right parenthesis
 *
 * returns:
 * 	pointer to the memory allocation
 * 	returns NULL if any error
 */
struct ast_t *ast_group(struct token_t lparen, struct ast_t *expr, 
	struct token_t rparen);

/**
 * Allocate and create a binary ast pointer
 *
 * params:
 * 	left	pointer to the left ast expression
 * 	op	token to the operation
 * 	right	pointer to the right ast expression
 * 
 * returns:
 * 	pointer to the memory allocation
 * 	returns NULL if any error
 */
struct ast_t *ast_binary(struct ast_t *left, struct token_t op, 
	struct ast_t *right);

/**
 * Free the allocated memory for ast, free recursively
 *
 * params:
 * 	ast	pointer to the ast that needs freeing
 */
void ast_free(struct ast_t *ast);

/**
 * Print the ast tree
 *
 * params:
 * 	ast	pointer to the ast that needs printing
 */
void ast_print(struct ast_t *ast);

#endif // AST_H
