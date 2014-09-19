#include "ft_sudoku.h"

void				force_cell_reliable(t_cell *cell, const int reliable_digit)
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




void				put_reliable_and_propage_constraint(t_grid *grid, t_cell *cell)
{
	int				i;
	int				reliable_digit;

	reliable_digit = 0;

	for(i = 0; i < GRID_TOP_DIGIT; ++i)
	{
		if (cell->possible_digits[i])
		{
			reliable_digit = (i + 1);
			cell->reliable_digit = reliable_digit;
			++grid->reliable_cells_count;
			break;
		}
	}

	propage_constraint(grid, cell);
}




t_cell				*find_best_cell_and_place_reliables(t_grid *grid)
{
	t_cell			*best_cell;
	t_cell			*buffer_cell;
	int				i;
	int				j;

	best_cell = NULL;

	for (i = 0; i < GRID_TOP_DIGIT; ++i)
	{
		for (j = 0; j < GRID_TOP_DIGIT; ++j)
		{
			buffer_cell = grid->cells[i][j];
			if (buffer_cell->reliable_digit == 0)
			{
				if (buffer_cell->possible_digits_count != 1)
				{
					if (!best_cell || (buffer_cell->possible_digits_count < best_cell->possible_digits_count ))
					{
						best_cell = buffer_cell;
					}
				}
				else
				{
					put_reliable_and_propage_constraint(grid, buffer_cell);
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

	return (best_cell);
}




t_grid				*recursive_solver(t_grid *grid, t_cell *forced_cell)
{
	t_grid			*buffer_grid;
	t_grid			*grid_copy;
	t_cell			*best_cell;
	t_cell			*next_forced_cell;
	int				i;

	buffer_grid = NULL;
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
			propage_constraint(grid, forced_cell);
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


char				**input_as_array(const char *input)
{
	unsigned int	i;
	int				current_array_index;
	int				current_line_index;
	char			**grid_array;

	current_array_index = 0;
	current_line_index = 0;

	grid_array = malloc(sizeof(char *) * 9);
	grid_array[0] = malloc(sizeof(char) * 10);

	const unsigned int len = strlen(input);
	for (i = 0; i < len; ++i)
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

	propage_whole_grid_constraint(grid);

	if (!check_whole_grid_integrity(grid))
	{
		printf("Unsolvable grid\n");
		destroy_grid(grid);
		return (1);
	}

	grid = recursive_solver(grid, NULL);
	print_grid_as_line(grid);

	destroy_grid(grid);

	return (0);
}
