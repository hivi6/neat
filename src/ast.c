#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

#define AST_MAX_DEPTH 1024

// ========================================
// helper function declaration
// ========================================

/**
 * Print the ast with the given indentations
 *
 * params:
 * 	ast	printing ast
 * 	indent	how much indentation for each line
 */
void ast_print_helper(struct ast_t *ast, int indent, char last[]);

/**
 * Print the token with the given indentations
 *
 * params:
 * 	label	labeling of the token
 * 	token 	printing token
 * 	indent	how much indentation for each line
 */
void ast_print_token(const char *label, struct token_t token, int indent, char last[]);

// ========================================
// ast.h definition
// ========================================

struct ast_t *ast_literal(struct token_t token) {
	struct ast_t *res = (struct ast_t *)malloc(sizeof(struct ast_t));
	if (res == NULL) return NULL;
	res->type = AST_LITERAL;
	res->start = res->end = token;

	res->value.literal.token = token;

	return res;
}

struct ast_t *ast_group(struct token_t lparen, struct ast_t *expr, 
	struct token_t rparen) {
	struct ast_t *res = (struct ast_t *)malloc(sizeof(struct ast_t));
	if (res == NULL) return NULL;
	res->type = AST_GROUP;
	res->start = lparen;
	res->end = rparen;

	res->value.group.lparen = lparen;
	res->value.group.expr = expr;
	res->value.group.rparen = rparen;

	return res;
}

struct ast_t *ast_binary(struct ast_t *left, struct token_t op, 
	struct ast_t *right) {
	struct ast_t *res = (struct ast_t *)malloc(sizeof(struct ast_t));
	if (res == NULL) return NULL;
	res->type = AST_BINARY;
	res->start = left->start;
	res->end = right->end;

	res->value.binary.left = left;
	res->value.binary.op = op;
	res->value.binary.right = right;

	return res;
}

void ast_free(struct ast_t *ast) {
	if (ast == NULL) return;

	switch (ast->type) {
	case AST_LITERAL:
		break;

	case AST_GROUP:
		ast_free(ast->value.group.expr);
		break;

	case AST_BINARY:
		ast_free(ast->value.binary.left);
		ast_free(ast->value.binary.right);
		break;
	}
	free(ast);
}

void ast_print(struct ast_t *ast) {
	if (ast == NULL) {
		printf("<NULL> No ast generated\n");
		return;
	}

	printf("PROGRAM: %s\n", ast->start.filename);
	char last[AST_MAX_DEPTH] = {};
	last[0] = 1;
	ast_print_helper(ast, 0, last);
}

// ========================================
// helper function definition
// ========================================

void ast_print_helper(struct ast_t *ast, int depth, char last[]) {
	for (int i = 0; i < depth; i++) {
		if (i < AST_MAX_DEPTH && last[i]) printf("    ");
		else printf("|   ");
	}

	last[depth+1] = 0;
	switch (ast->type) {
	case AST_LITERAL:
		printf("+- LITERAL\n");
		last[depth+1] = 1;
		ast_print_token("TOKEN", ast->value.literal.token, depth+1, last);
		break;

	case AST_GROUP:
		printf("+- GROUP\n");
		ast_print_token("LPAREN", ast->value.group.lparen, depth+1, last);
		ast_print_helper(ast->value.group.expr, depth+1, last);
		last[depth+1] = 1;
		ast_print_token("RPAREN", ast->value.group.rparen, depth+1, last);
		break;
	
	case AST_BINARY:
		printf("+- BINARY\n");
		ast_print_helper(ast->value.binary.left, depth+1, last);
		ast_print_token("OP", ast->value.binary.op, depth+1, last);
		last[depth+1] = 1;
		ast_print_helper(ast->value.binary.right, depth+1, last);
		break;
	}
}

void ast_print_token(const char *label, struct token_t token, int depth, char last[]) {
	for (int i = 0; i < depth; i++) {
		if (i < AST_MAX_DEPTH && last[i]) printf("    ");
		else printf("|   ");
	}

	printf("+- %s ", label);
	token_print(&token);
	printf("\n");
}
