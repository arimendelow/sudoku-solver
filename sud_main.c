/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sud_main_wcomments.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amendelo <amendelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 10:22:16 by amendelo          #+#    #+#             */
/*   Updated: 2018/11/13 15:00:23 by amendelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> //for write
#include <stdlib.h> //for malloc & free

// TO DO:
//	- expand error checking. current iteration does not check that output is
//		a valid map, so once could input a completely filled but invalid
//		puzzle and, rather than printing "error," the invalid solution
//		would be printed.
//	- organize this into different files and use a header file and a Makefile.
//	- add a message explaining how to use the function when input is invalid.
//		right now, just returns "error."

int		ft_strlen(char *str)
{
	int len;
	
	len = 0;
	while(str[len])
		len++;
	return (len);
}

// compare two matrices. return 1 if they're the same, and 0 otherwise.
int		mats_same(char **mat1, char **mat2)
{
	int row;
	int col;
	
	row = -1;
	while (++row < 9)
	{
		col = -1;
		while (++col < 9)
			if (mat1[row][col] != mat2[row][col])
				return (0);
	}
	return (1);
}

// create duplicates of input into a 9x9 char array. meant to work spefically
//		with an input of *argv[].
char	**mat_dup(char *av[])
{
	char	**matrix;
	int		row;
	int		col;

	matrix = (char**)malloc(9 * sizeof(char*));
	row = -1;
	while (++row < 9)
	{
		matrix[row] = (char*)malloc(10 * sizeof (char));
	}
	// input row/col goes from 1-9 & 0-8; output goes from 0-8 & 0-8
	row = -1;
	while (++row < 9)
	{
		col = -1;
		while (++col < 9)
			matrix[row][col] = av[row + 1][col];
	}
	return (matrix);
}

// frees the memory used by our arrays
void	mat_clear(char *mat[])
{
	int		row;

	row = -1;
	while (++row < 9)
	{
		free(mat[row]);
	}
	free(mat);
}

// for printing out the board with spaces between each character
void	print_board(char *av[])
{
	int row;
	int col;
	
	// row goes from 0 to 8
	row = -1;
	while (++row < 9)
	{
		//col goes from 0 to 8
		col = -1;
		while (++col < 9)
		{
			write(1, &av[row][col], 1);
			if (col < 8)
				write(1, " ", 1);
			else
				write(1, "\n", 1);
		}
	}
}

// check valid input puzzle
int		is_valid_sud(int ac, char *av[])
{
	int i;
	int j;
	int val_lines;
	// If have anything other than 10 input params, we know that we don't have 9 rows, so
	//		return error.
	if (ac != 10)
		return (0);
	// Next, we need to check that the rows are the correct length and have valid characters.
	else
	{
		i = 0;
		val_lines = 0;
		// traversing rows...
		while (++i <= 9 && ft_strlen(av[i]) == 9)
		{
			j = -1;
			// traversing columns (characters)...
			while (++j < 9)
			{
				// if the character is not valid, return error.
				if (!((av[i][j] >= '1' && av[i][j] <= '9') || av[i][j] == '.'))
					return (0);
			}
			// increase count of valid lines
			val_lines++;
		}
		// if we've got 9 valid lines, return true
		if (val_lines == 9)
			return (1);
		// otherwise, return false
		else
			return (0);
	}
}

// helper for is_move_ok
int		in_row(char *av[], char num, int row)
{
	int col;

	// columns go from 0 to 8
	col = -1;
	while (++col < 9)
	{
		if (av[row][col] == num)
			return (1);
	}
	return (0);
}

// helper for is_move_ok
int		in_col(char *av[], char num, int col)
{
	int row;
	
	// rows go from 0 to 8
	row = -1;
	while (++row < 9)
	{
		if (av[row][col] == num)
			return (1);
	}
	return (0);
}

// helper for is_move_ok
int		in_box(char *av[], char num, int box_top_r, int box_top_c)
{
	int row;
	int col;

	// this is row/col for each box - so indices go from 0 to 2
	row = -1;
	while (++row < 3)
	{
		col = -1;
		while (++col < 3)
			if (av[row + box_top_r][col + box_top_c] == num) // adding 1 to row because row starts at 1
				return (1);
	}
	return (0);
}

// check that current move is valid
int		is_move_ok(char *av[], char num, int row, int col)
{
	// need to check move against current row, column, and box to make sure that 
	//		the number has not already been placed
	// in in_box, adjust input row by 1 so that we give it the correct index.
	//		without this correction, when it's on the bottom row of a given box, 
	//		it'll think it's on the next row down. this is because we're using
	//		*argv[] directly, and row 0 on that is the name of the binary rather than
	//		the first row in the puzzle.
	return (!in_row(av, num, row) &&
			!in_col(av, num, col) &&
			!in_box(av, num, row - row % 3, col - col % 3));
}

// search grid to find next unassigned space. if found, set row/col to those indices,
//		and return (1). if grid is completely assigned, returns (0).
int		find_unassigned(char *av[], int *row, int *col)
{
	// row goes 0 to 8
	*row = -1;
	while (++(*row) < 9)
	{
		// col goes 0 to 8
		*col = -1;
		while (++(*col) < 9)
			if (av[*row][*col] == '.') // character . represents unassigned space
				return (1);
	}
	return (0);
}

// solve, choosing numbers starting from 1 and ending at 9.
int		solve_sud19(char *av[])
{
	int		row;
	int		col;
	char	num;

	// if no unassigned location, board is solved! yay!
	if (!find_unassigned(av, &row, &col))
		return (1);
	
	// once we've got an unassigned location, consider characters 1 to 9 for placement
	num = '0';
	while (++num <= '9')
	{
		// once we find a number that can fit here...
		if (is_move_ok(av, num, row, col))
		{
			// ...make the assignment...
			av[row][col] = num;
			// ...then check that we can solve the rest of the grid with this assignment.
			if (solve_sud19(av))
				return (1);
			av[row][col] = '.';
		}
	}
	// if we've made it here, there are no valid assignments based on the last number entered,
	//		so we need to go back and change the previous number placed.
	// ie, this is what triggers backtracking.
	return (0);
}

// solve, choosing numbers starting from 9 and ending at 1.
// if the solution to this is *not* the same as the one given above, the sudoku is not valid.
int		solve_sud91(char *av[])
{
	int		row;
	int		col;
	char	num;

	// if no unassigned location, board is solved! yay!
	if (!find_unassigned(av, &row, &col))
		return (1);
	
	// once we've got an unassigned location, consider characters 9 to 1 for placement
	num = '9' + 1;
	while (--num >= '1')
	{
		// once we find a number that can fit here...
		if (is_move_ok(av, num, row, col))
		{
			// ...make the assignment...
			av[row][col] = num;
			// ...then check that we can solve the rest of the grid with this assignment.
			if (solve_sud91(av))
				return (1);
			av[row][col] = '.';
		}
	}
	// if we've made it here, there are no valid assignments based on the last number entered,
	//		so we need to go back and change the previous number placed.
	// ie, this is what triggers backtracking.
	return (0);
}

int		main(int argc, char *argv[])
{
	char **mat1;
	char **mat2;
	
	if (is_valid_sud(argc, argv))
	{
		// make two duplicates of the puzzle, we're going to solve two
		//		different ways and then confirm that solutions are identical
		//		to confirm that there is, in fact, only one solution to this puzzle,
		mat1 = mat_dup(argv);
		mat2 = mat_dup(argv);
		if (solve_sud19(mat1) &&
			solve_sud91(mat2) &&
			mats_same(mat1, mat2))
			print_board(mat1);
		else
			{
			write(1, "Error\n", 6);
			}
		// // free the memory used by our arrays so that we do not leak memory
		mat_clear(mat1);
		mat_clear(mat2);
	}
	else
		write(1, "Error\n", 6);
	return (0);
}
