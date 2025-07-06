#ifndef POS_H
#define POS_H

/**
 * Position type
 *
 * Keep track of position in a given source text
 *
 * members:
 * 	idx	current index in the source text
 * 	ln	current line number in the source text
 * 	col	current column number in the source text
 */
struct pos_t {
	int idx;
	int ln;
	int col;
};

/**
 * What should be the next position based on the character?
 * If 0 then don't move
 * If new line then increment the line number and reset the column number
 *
 * params:
 * 	self	self pointer
 * 	ch	react based on the character
 */
void pos_next(struct pos_t *self, char ch);

/**
 * Print the current position
 *
 * params:
 * 	self	self pointer
 */
void pos_print(struct pos_t *self);

#endif // POS_H
