#ifndef FT_SUDOKU_H
# define FT_SUDOKU_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <inttypes.h>
# include <stdbool.h>

#define t_digit				uint8_t

/* Game States */
# define STATE_SOLVED		0
# define STATE_SOLVING		1
# define STATE_UNSOLVABLE	2

# define GRID_TOP_DIGIT		9
# define CELLS_BY_ROW		9
# define BLOCK_BY_ROW		3
# define GRID_DIGIT_COUNT	(GRID_TOP_DIGIT * GRID_TOP_DIGIT)

typedef struct s_grid		t_grid;
typedef struct s_cell		t_cell;

struct s_cell
{
	t_digit					possible_digits[GRID_TOP_DIGIT];
	uint8_t					possible_digits_count;
	t_digit					reliable_digit;
	uint8_t					pos_x;
	uint8_t					pos_y;
};

struct s_grid
{
	t_cell					*cells[GRID_TOP_DIGIT][GRID_TOP_DIGIT];
	uint8_t					reliable_cells_count;
};


t_grid						*create_grid(char *input_string);
void						destroy_grid(t_grid *grid);
void						print_grid(t_grid *grid);
void						copy_grid(t_grid *dest, t_grid *origin);

t_cell						*create_cell(t_digit reliable_digit, uint8_t pos_x, uint8_t pos_y);
void						destroy_cell(t_cell *cell);
void						copy_cell(t_cell *dest, t_cell *origin);

bool						check_row_and_col_integrity(t_grid *grid, int row_index, int col_index);
bool						check_cell_block_integrity(t_grid *grid, int cell_pos_x, int cell_pos_y);
bool						check_whole_grid_integrity(t_grid *grid);
void						propage_constraint(t_grid *grid, t_digit reliable_digit, int cell_pos_x, int cell_pos_y);

#endif
