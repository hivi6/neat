#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

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
void ast_print_helper(struct ast_t *ast, int indent);

/**
 * Print the token with the given indentations
 *
 * params:
 * 	label	labeling of the token
 * 	token 	printing token
 * 	indent	how much indentation for each line
 */
void ast_print_token(const char *label, struct token_t token, int indent);

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
	printf("PROGRAM\n");
	ast_print_helper(ast, 0);
}

// ========================================
// helper function definition
// ========================================

void ast_print_helper(struct ast_t *ast, int indent) {
	switch (ast->type) {
	case AST_LITERAL:
		printf("%*s+- LITERAL\n", indent, "");
		ast_print_token("TOKEN", ast->value.literal.token, indent+4);
		break;

	case AST_GROUP:
		printf("%*s+- GROUP\n", indent, "");
		ast_print_token("LPAREN", ast->value.group.lparen, indent+4);
		ast_print_helper(ast->value.group.expr, indent+4);
		ast_print_token("RPAREN", ast->value.group.rparen, indent+4);
		break;
	
	case AST_BINARY:
		printf("%*s+- BINARY\n", indent, "");
		ast_print_helper(ast->value.binary.left, indent+4);
		ast_print_token("OP", ast->value.binary.op, indent+4);
		ast_print_helper(ast->value.binary.right, indent+4);
		break;
	}
}

void ast_print_token(const char *label, struct token_t token, int indent) {
	printf("%*s+- %s ", indent, "", label);
	token_print(&token);
	printf("\n");
}
