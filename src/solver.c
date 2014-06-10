#include "ft_sudoku.h"

void				force_cell_reliable(t_cell *cell, int reliable_digit)
{
	int				i;

	cell->reliable_digit = reliable_digit;
	for (i = 0; i < GRID_TOP_DIGIT; ++i)
	{
		cell->possible_digits[i] = false;
	}
	cell->possible_digits_count = 1;
	cell->possible_digits[reliable_digit - 1] = true;
}




void				put_reliable_and_propage_constraint(t_grid *grid, int cell_pos_x, int cell_pos_y)
{
	int				i;
	int			reliable_digit;

		reliable_digit = 0;

	for(i = 0; i < GRID_TOP_DIGIT; ++i)
	{
		if (grid->cells[cell_pos_x][cell_pos_y]->possible_digits[i])
		{
			reliable_digit = (i + 1);
			grid->cells[cell_pos_x][cell_pos_y]->reliable_digit = reliable_digit;
			++grid->reliable_cells_count;
			break;
		}
	}

	propage_constraint(grid, reliable_digit, cell_pos_x, cell_pos_y);
}




t_cell				*find_best_cell_and_place_reliables(t_grid *grid)
{
	int				i;
	int				j;
	t_cell			*best_cell;

	best_cell = NULL;

	for (i = 0; i < GRID_TOP_DIGIT; ++i)
	{
		for (j = 0; j < GRID_TOP_DIGIT; ++j)
		{
			if (grid->cells[i][j]->reliable_digit == 0)
			{
				if (grid->cells[i][j]->possible_digits_count != 1)
				{
					if (!best_cell || (grid->cells[i][j]->possible_digits_count < best_cell->possible_digits_count ))
					{
						best_cell = grid->cells[i][j];
					}
				}
				else
				{
					put_reliable_and_propage_constraint(grid, i, j);
					if (!check_row_and_col_integrity(grid, i, j)
						|| !check_cell_block_integrity(grid, i, j)
						)
					{
						return (NULL);
					}
				}
			}
		}
	}

	return best_cell;
}




t_grid				*recursive_solver(t_grid *grid, t_cell *forced_cell)
{
	t_grid			*buffer_grid;
	t_grid			*grid_copy;
	t_cell			*best_cell;
	t_cell			*next_forced_cell;
	int				i;

//	print_grid(grid);
	if (grid->reliable_cells_count == GRID_DIGIT_COUNT && check_whole_grid_integrity(grid))
	{
		return (grid);
	}
	else
	{
		if (forced_cell != NULL)
		{
			destroy_cell(grid->cells[forced_cell->pos_x][forced_cell->pos_y]);

			grid->cells[forced_cell->pos_x][forced_cell->pos_y] = forced_cell;
			++grid->reliable_cells_count;
			propage_constraint(grid, forced_cell->reliable_digit, forced_cell->pos_x, forced_cell->pos_y);
		}

		best_cell = find_best_cell_and_place_reliables(grid);
		if (best_cell == NULL)
		{
			if (grid->reliable_cells_count == GRID_DIGIT_COUNT && check_whole_grid_integrity(grid))
			{
				return (grid);
			}

			destroy_grid(grid);
			return (NULL);
		}

		for (i = 0; i < GRID_TOP_DIGIT; ++i)
		{
			if (best_cell->possible_digits[i] != false)
			{
				next_forced_cell = malloc(sizeof(t_cell));
				grid_copy = malloc(sizeof(t_grid));

				copy_cell(next_forced_cell, best_cell);
				force_cell_reliable(next_forced_cell, i + 1);
				copy_grid(grid_copy, grid);

				buffer_grid = recursive_solver(grid_copy, next_forced_cell);


				if (buffer_grid != NULL)
				{
					destroy_grid(grid);
					grid = buffer_grid;

					if (grid->reliable_cells_count == GRID_DIGIT_COUNT && check_whole_grid_integrity(grid))
					{
						return (grid);
					}
				}
			}
		}
	destroy_grid(buffer_grid);
	destroy_grid(grid);
	return (NULL);

	}
}


char				**input_as_array(char *input)
{
	char			**grid_array;
	unsigned int	i;
	int				current_array_index;
	int				current_line_index;

	current_array_index = 0;
	current_line_index = 0;

	grid_array = malloc(sizeof(char *) * 9);
	grid_array[0] = malloc(sizeof(char) * 10);

	for (i = 0; i < strlen(input); ++i)
	{

		if (input[i] != '-')
		{
			grid_array[current_array_index][current_line_index] =  input[i];

			++current_line_index;
		}
		else
		{
			grid_array[current_array_index][current_line_index] = '\0';
			++current_array_index;
			grid_array[current_array_index] = malloc(sizeof(char) * 10);
			current_line_index = 0;
		}
	}

	return (grid_array);
}

int					main(int argc, char *argv[])
{
	t_grid 			*grid;
	char			**grid_as_array;

	if (argc != 2)
	{
		printf("Usage : %s < sudoku as string splited by '-' >\n", argv[0]);
		printf("Ex : %s 000000000-000000000-000000000-000000000-000000000-000000000-0000000000-000000000-000000000\n", argv[0]);

		return (1);
	}
	if (strlen(argv[1]) != 89)
	{
		printf("Input does not match required length\n");

		return (1);
	}


	grid_as_array = input_as_array(argv[1]);

	grid = (t_grid*) create_grid(grid_as_array);
	if (grid != NULL)
	{
		grid = recursive_solver(grid, NULL);
		print_grid_as_line(grid);

		destroy_grid(grid);
	}
	else
	{
		printf("Unsolvable grid\n");
		return (1);
	}

	return (0);
}
