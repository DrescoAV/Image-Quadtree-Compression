#include "quadtree.h"

grid_t **alloc_grid(int height, int width)
{
	// Allocating memory for an array of pointers (matrix rows).
	grid_t **grid = malloc(sizeof(grid_t *) * height);

	// For each array element (matrix row), it allocates memory.
	for (int i = 0; i < height; i++)
	{
		grid[i] = malloc(sizeof(grid_t) * width);
	}

	// Returning pointer to the allocated matrix.
	return grid;
}

void dealloc_grid(grid_t **grid, int height)
{
	// For each pointer from the array(matrix row) memory is deallocated.
	for (int i = 0; i < height; i++)
	{
		free(*(grid + i));
	}

	// Deallocating the pointer array of matrix rows.
	free(grid);
}

node_t *create_node()
{
	// Allocating memory for a node.
	node_t *node = malloc(sizeof(node_t));

	// Initializing the new node with default values.
	node->upper_left = NULL;
	node->upper_right = NULL;
	node->bottom_right = NULL;
	node->bottom_left = NULL;
	node->type = 0;
	node->red_value = 0;
	node->green_value = 0;
	node->blue_value = 0;

	// Returning a pointer to the newly created node.
	return node;
}

node_t *create_quadtree(node_t *root, grid_t **grid, unsigned int size, unsigned int x_coord,
						unsigned int y_coord, uint64_t factor, long unsigned int *biggest_undivided_size)
{
	// Allocating memory for node if it is NULL.
	if (root == NULL)
	{
		root = create_node();
	}

	// Creating variables for calculating RGB sumes and mean value.
	uint64_t red = 0, green = 0, blue = 0, mean = 0;
	unsigned int i, j;

	// Using given formula calculating score for dividing image.
	for (i = x_coord; i < x_coord + size; i++)
	{
		for (j = y_coord; j < y_coord + size; j++)
		{
			red += grid[i][j].red_value;
			green += grid[i][j].green_value;
			blue += grid[i][j].blue_value;
		}
	}
	red /= size * size;
	blue /= size * size;
	green /= size * size;
	for (i = x_coord; i < x_coord + size; i++)
	{
		for (j = y_coord; j < y_coord + size; j++)
		{
			mean += (red - grid[i][j].red_value) * (red - grid[i][j].red_value);
			mean += (green - grid[i][j].green_value) * (green - grid[i][j].green_value);
			mean += (blue - grid[i][j].blue_value) * (blue - grid[i][j].blue_value);
		}
	}
	mean /= 3 * size * size;

	// If score is less or equal to given factor then the node is a terminal one,
	// containing calculated RGB values and it's type is 1.
	if (mean <= factor)
	{
		root->type = 1;
		root->red_value = red;
		root->green_value = green;
		root->blue_value = blue;

		// Calculating the maximum undivided area.
		if (*biggest_undivided_size < size)
		{
			*biggest_undivided_size = size;
		}
	}

	// If the node is not terminal, create child nodes using recursion.
	if (root->type != 1)
	{
		// For each node's child x and y coordonates are modified to move around the matrix,
		// and size is divided by 2.
		root->upper_left = create_quadtree(root->upper_left, grid, size / 2, x_coord, y_coord,
										   factor, biggest_undivided_size);
		root->upper_right = create_quadtree(root->upper_right, grid, size / 2,
											x_coord, y_coord + size / 2, factor, biggest_undivided_size);
		root->bottom_right = create_quadtree(root->bottom_right, grid, size / 2, x_coord + size / 2,
											 y_coord + size / 2, factor, biggest_undivided_size);
		root->bottom_left = create_quadtree(root->bottom_left, grid, size / 2, x_coord + size / 2,
											y_coord, factor, biggest_undivided_size);
	}
	return root;
}

node_t *decompress_level_traversal(FILE *image_in, node_t *root, long int end_position,
								   grid_t **grid, unsigned int size)
{
	int i = 1;

	// Continue decompression until the end of the compressed file is reached.
	while (1)
	{
		// If the current position in the file matches the end position, stop.
		if (ftell(image_in) == end_position)
			break;

		// Recursively decompress the current level and update the root node.
		root = decompress_current_level(image_in, root, i, grid, size, 0, 0);
		i++;
	}
	return root;
}

node_t *decompress_current_level(FILE *image_in, node_t *root, int level, grid_t **grid,
								 unsigned int size, unsigned int x_coord, unsigned int y_coord)
{
	unsigned char aux_type;
	unsigned int i, j;

	// If it's the wanted level and there's data, process it.
	if (level == 1 && fread(&aux_type, sizeof(unsigned char), 1, image_in) != 0)
	{
		// If the root node is NULL, create a new node.
		if (root == NULL)
		{
			root = create_node();
		}

		// Set the type of the root node to the auxiliary type.
		root->type = aux_type;

		// If the type is 1(it's a leaf), read RGB values from the input file.
		if (root->type == 1)
		{
			fread(&root->red_value, sizeof(unsigned char), 1, image_in);
			fread(&root->green_value, sizeof(unsigned char), 1, image_in);
			fread(&root->blue_value, sizeof(unsigned char), 1, image_in);

			// Apply these RGB values to the corresponding pixels in the grid.
			for (i = x_coord; i < x_coord + size; i++)
			{
				for (j = y_coord; j < y_coord + size; j++)
				{
					grid[i][j].red_value = root->red_value;
					grid[i][j].green_value = root->green_value;
					grid[i][j].blue_value = root->blue_value;
				}
			}
		}
	}

	// If the level is greater than 1 (recursive case).
	if (level > 1)
	{
		// Recursively decompress child nodes for each quadrant.

		// Upper Left.
		if (root->type != 1)
			root->upper_left = decompress_current_level(image_in, root->upper_left,
														level - 1, grid, size / 2, x_coord,
														y_coord);

		// Upper Right.
		if (root->type != 1)
			root->upper_right = decompress_current_level(image_in, root->upper_right,
														 level - 1, grid, size / 2, x_coord,
														 y_coord + size / 2);

		// Bottom Right.
		if (root->type != 1)
			root->bottom_right = decompress_current_level(image_in, root->bottom_right, level - 1,
														  grid, size / 2, x_coord + size / 2,
														  y_coord + size / 2);

		// Bottom Left
		if (root->type != 1)
			root->bottom_left = decompress_current_level(image_in, root->bottom_left, level - 1,
														 grid, size / 2, x_coord + size / 2,
														 y_coord);
	}
	return root;
}

void compress_level_traversal(FILE *image_out, node_t *root, int height)
{
	int i;

	// Traverse each level of the quadtree and compress nodes.
	for (i = 1; i <= height; i++)
	{
		compress_current_level(image_out, root, i);
	}
}

void compress_current_level(FILE *image_out, node_t *root, int level)
{
	if (root == NULL)
	{
		return; // Base case: Nothing to compress.
	}

	// If it's the first level and the node type is non-zero(not a leaf), write type and data.
	if (level == 1)
	{

		fwrite(&root->type, sizeof(unsigned char), 1, image_out);
		if (root->type == 1)
		{
			fwrite(&root->red_value, sizeof(unsigned char), 1, image_out);
			fwrite(&root->green_value, sizeof(unsigned char), 1, image_out);
			fwrite(&root->blue_value, sizeof(unsigned char), 1, image_out);
		}
	}

	// For deeper levels, recursively compress child nodes.
	else if (level > 1)
	{
		compress_current_level(image_out, root->upper_left, level - 1);
		compress_current_level(image_out, root->upper_right, level - 1);
		compress_current_level(image_out, root->bottom_right, level - 1);
		compress_current_level(image_out, root->bottom_left, level - 1);
	}
}

int tree_height(node_t *root)
{
	// Base case: If the node is null, height is 0.
	if (root == NULL)
	{
		return 0;
	}

	// Recursively calculate height of child nodes.
	int upper_left = tree_height(root->upper_left);
	int upper_right = tree_height(root->upper_right);
	int bottom_right = tree_height(root->bottom_right);
	int bottom_left = tree_height(root->bottom_left);

	// Determine the maximum height among child nodes and add 1 for the current node.
	if (upper_left >= bottom_right && upper_left >= upper_right && upper_left >= bottom_left)
	{
		return upper_left + 1;
	}
	else if (upper_right >= bottom_right && upper_right >= upper_left && upper_right >= bottom_left)
	{
		return upper_right + 1;
	}
	else if (bottom_right >= bottom_left && bottom_right >= upper_left && bottom_right >= upper_right)
	{
		return bottom_right + 1;
	}
	else
		return bottom_left + 1;
}

void leaves_counter(node_t *root, int *leaf_no)
{
	// Base case: If the node is null, return.
	if (root == NULL)
	{
		return;
	}

	// If node's type is 1 it's a leaf.
	if (root->type == 1)
	{
		(*leaf_no)++;
	}

	// Continue quadtree traversal with recursion.
	leaves_counter(root->upper_left, leaf_no);
	leaves_counter(root->upper_right, leaf_no);
	leaves_counter(root->bottom_right, leaf_no);
	leaves_counter(root->bottom_left, leaf_no);
}

void dealloc_tree(node_t *root) // nuj daca merge ramane de implementat la final
{
	// Base case: If the current node is NULL, return.
	if (root == NULL)
	{
		return;
	}

	// Recursively deallocate memory for child nodes.
	dealloc_tree(root->upper_left);
	dealloc_tree(root->upper_right);
	dealloc_tree(root->bottom_right);
	dealloc_tree(root->bottom_left);

	// Deallocate memory for the given root.
	free(root);
}