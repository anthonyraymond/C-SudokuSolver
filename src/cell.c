#include "ft_sudoku.h"

t_cell			*create_cell(const int reliable_digit, const int pos_x, int pos_y)
{
	t_cell		*cell;
	int			i;

	cell = malloc(sizeof(t_cell));
	if (!cell)
		return (NULL);

	if (reliable_digit != 0)
	{
		for (i = 0; i < GRID_TOP_DIGIT; ++i)
		{
			cell->possible_digits[i] = 0;
		}
		cell->possible_digits[reliable_digit - 1] = reliable_digit;
		cell->possible_digits_count = 1;
		cell->reliable_digit = reliable_digit;
	}
	else
	{
		for (i = 0; i < GRID_TOP_DIGIT; ++i)
		{
			cell->possible_digits[i] = 1;
		}
		cell->possible_digits_count = 9;
		cell->reliable_digit = 0;
	}

	cell->pos_x = pos_x;
	cell->pos_y = pos_y;

	return (cell);
}

void			destroy_cell(t_cell *cell)
{
	if (cell != NULL)
	{
		free(cell);
	}
}

void			copy_cell(t_cell *dest, const t_cell *origin)
{
	memcpy(dest, origin, sizeof(t_cell));
}
