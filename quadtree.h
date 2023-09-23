#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#define PPM_TYPE "P6"
#define MAX_RGB 255

// Node of the quadtree.
typedef struct node_t
{
    unsigned char type;
    unsigned char red_value;
    unsigned char green_value;
    unsigned char blue_value;
    struct node_t *upper_left, *upper_right, *bottom_right, *bottom_left;
} node_t;

// RGB colors stored in the grid's elements.
typedef struct grid_t
{
    unsigned char red_value;
    unsigned char green_value;
    unsigned char blue_value;
} grid_t;

/*
 * alloc_grid() - Allocate memory for a grid of color values.
 * @arg1: Height of the grid.
 * @arg2: Width of the grid.
 * Returns a pointer to the allocated grid.
 */
grid_t **alloc_grid(int height, int width);

/*
 * create_node() - Create an empty node for the quadtree.
 * Returns a pointer to the newly created node.
 */
node_t *create_node();

/*
 * create_quadtree() - Create a quadtree based on color values.
 * @arg1: Root node of the current subtree.
 * @arg2: Grid of color values.
 * @arg3: Size of the current grid.
 * @arg4: X-coordinate of the current grid in the larger image.
 * @arg5: Y-coordinate of the current grid in the larger image.
 * @arg6: Factor for division.
 * @arg7: Pointer to store the size of the biggest undivided grid.
 * Returns root node of the constructed quadtree.
 */
node_t *create_quadtree(node_t *root, grid_t **grid, unsigned int size, unsigned int x_coord,
                        unsigned int y_coord, uint64_t factor, long unsigned int *biggest_undivided_size);

/*
 * compress_level_traversal() - Compress a level of the quadtree and write to the output file.
 * @arg1: Output file stream.
 * @arg2: Root node of the current subtree.
 * @arg3: Height of the current level.
 */
void compress_level_traversal(FILE *image_out, node_t *root, int height);

/*
 * compress_current_level() - Compress the nodes at the current level and write to the output file.
 * @arg1: Output file stream.
 * @arg2: Root node of the current subtree.
 * @arg3: Current level of the quadtree.
 */
void compress_current_level(FILE *image_out, node_t *root, int level);

/*
 * tree_height() - Calculate the height of the quadtree.
 * @arg1: Root node of the quadtree.
 * Returns the height of the quadtree.
 */
int tree_height(node_t *root);

/*
 * decompress_level_traversal() - Decompress a level of the quadtree and reconstruct the grid.
 * @arg1: Input file stream.
 * @arg2: Root node of the current subtree.
 * @arg3: Ending position of the decompression.
 * @arg4: Grid of color values.
 * @arg5: Size of the current grid.
 * Returns root node of the reconstructed quadtree.
 */
node_t *decompress_level_traversal(FILE *image_in, node_t *root, long int end_position,
                                   grid_t **grid, unsigned int size);

/*
 * decompress_current_level() - Decompress the nodes at the current level and reconstruct the grid.
 * @arg1: Input file stream.
 * @arg2: Root node of the current subtree.
 * @arg3: Current level of the quadtree.
 * @arg4: Grid of color values.
 * @arg5: Size of the current grid.
 * @arg6: X-coordinate of the current grid in the larger image.
 * @arg7: Y-coordinate of the current grid in the larger image.
 * Returns root node of the reconstructed quadtree.
 */
node_t *decompress_current_level(FILE *image_in, node_t *root, int level, grid_t **grid,
                                 unsigned int size, unsigned int x_coord, unsigned int y_coord);

/*
 * leaves_counter() - Count the number of leaf nodes in the quadtree.
 * @arg1: Root node of the quadtree.
 * @arg2: Pointer to store the leaf count.
 */
void leaves_counter(node_t *root, int *leaf_no);

/*
 * dealloc_tree() - Deallocate memory for the quadtree.
 * @arg1: Root node of the quadtree.
 */
void dealloc_tree(node_t *root);

/*
 * dealloc_grid() - Deallocate memory for the grid.
 * @arg1: Grid of color values.
 * @arg2: Height of the grid.
 */
void dealloc_grid(grid_t **grid, int height);