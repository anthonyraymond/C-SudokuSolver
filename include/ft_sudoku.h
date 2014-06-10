#ifndef FT_SUDOKU_H
# define FT_SUDOKU_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>


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
typedef enum e_bool			t_bool;

enum						e_bool { true = 1, false = 0 };

struct s_cell
{
	int					possible_digits[GRID_TOP_DIGIT];
	int					possible_digits_count;
	int					reliable_digit;
	int					pos_x;
	int					pos_y;
};

struct s_grid
{
	t_cell					*cells[GRID_TOP_DIGIT][GRID_TOP_DIGIT];
	int					reliable_cells_count;
};


t_grid						*create_grid(char **input_array);
void						destroy_grid(t_grid *grid);
void						print_grid(t_grid *grid);
void						print_grid_as_line(t_grid *grid);
void						copy_grid(t_grid *dest, t_grid *origin);

t_cell						*create_cell(int reliable_digit, int pos_x, int pos_y);
void						destroy_cell(t_cell *cell);
void						copy_cell(t_cell *dest, t_cell *origin);

t_bool						check_row_and_col_integrity(t_grid *grid, int row_index, int col_index);
t_bool						check_cell_block_integrity(t_grid *grid, int cell_pos_x, int cell_pos_y);
t_bool						check_whole_grid_integrity(t_grid *grid);
void						propage_constraint(t_grid *grid, int reliable_digit, int cell_pos_x, int cell_pos_y);

#endif
