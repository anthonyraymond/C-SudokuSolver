#include "ft_sudoku.h"

void				force_cell_reliable(t_cell *cell, t_digit reliable_digit)
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
	t_digit			reliable_digit;

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




int					main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	t_grid *grid;
	grid = (t_grid*) create_grid(argv[1]);

	print_grid(grid);

	grid = recursive_solver(grid, NULL);

	print_grid(grid);

/*
printf("\n");
	t_grid		*gridcop = malloc(sizeof(t_grid));
	copy_grid(gridcop, grid);
	gridcop->cells[0][0]->reliable_digit = 6;
	print_grid(gridcop);

	printf("\n");
	print_grid(grid);

	destroy_grid(gridcop);*/
	destroy_grid(grid);
	return (0);
}
