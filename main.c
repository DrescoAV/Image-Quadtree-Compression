#include "quadtree.h"

int main(int argc, char *argv[])
{
    char input_buffer;
    int requested_operation = -1;
    uint64_t factor = 0;

    // Handling command-line options.
    if (argc == 5)
    {
        sscanf(argv[1], "%c%c%d", &input_buffer, &input_buffer, &requested_operation);
        // Get factor when option c1 or c2 is specified.
        factor = atoi(argv[2]);
    }
    else if (argc == 4)
    {
        requested_operation = 3;
        // Default operation (option -d) when not specified.
    }

    char file_type[2];
    unsigned int width = 0, height = 0, max_value = 0;
    uint64_t image_dimension = 0, end_position = 0, biggest_undivided_area = 0;
    ;
    FILE *file_in, *file_out;
    node_t *root = NULL;
    int no_of_levels, no_of_leaves = 0;
    grid_t **grid;
    unsigned int i;

    if (requested_operation == 1 || requested_operation == 2)
    {
        // Handling options -c1 and -c2 (statistics or image compression).

        // Opening input .ppm file for reading.
        file_in = fopen(argv[argc - 2], "rb");
        fscanf(file_in, "%s", file_type);
        fscanf(file_in, "%u%c%u%c%u%c", &width, &input_buffer, &height, &input_buffer,
               &max_value, &input_buffer);
        image_dimension = width * height;

        // Allocating memory for pixel matrix.
        grid = alloc_grid(height, width);

        // Reading data for pixel matrix.
        for (i = 0; i < height; i++)
        {
            fread(grid[i], sizeof(unsigned char), width * 3, file_in);
        }

        // Closing .ppm file.
        fclose(file_in);

        // Processing image data and quadtree creation.
        root = create_quadtree(root, grid, height, 0, 0, factor, &biggest_undivided_area);
        no_of_levels = tree_height(root);

        if (requested_operation == 1)
        {
            // Writing statistics to output file for option -c1.

            // Counting leaves and opening output file to write into.
            leaves_counter(root, &no_of_leaves);
            file_out = fopen(argv[argc - 1], "wt");
            fprintf(file_out, "%d\n%d\n%lu\n", no_of_levels, no_of_leaves, biggest_undivided_area);
        }
        else
        {
            // Writing compressed data to output file for option -c2.

            file_out = fopen(argv[argc - 1], "wb");
            fwrite(&height, sizeof(unsigned int), 1, file_out);
            compress_level_traversal(file_out, root, no_of_levels);
        }
    }
    else
    {
        // Handling option -d (decompression).

        // Opening the compressed input file for reading.
        file_in = fopen(argv[argc - 2], "rb");

        // Calculating the number of bytes in the compressed file, used for in-level tree order
        // by going to end of stream, using ftell() and going back to the start.
        fseek(file_in, 0, 2);
        end_position = ftell(file_in);
        fseek(file_in, 0, 0);

        // Reading image dimensions from the compressed file.
        fread(&image_dimension, sizeof(unsigned int), 1, file_in);
        height = width = image_dimension;

        // Alocating pixel matrix for creating PPM file.
        grid = alloc_grid(height, width);

        // Writing PPM type, height and max rgb value in output PPM file.
        file_out = fopen(argv[argc - 1], "wt");
        fprintf(file_out, "%s\n%u %u\n%d\n", PPM_TYPE, width, height, MAX_RGB);
        fclose(file_out);

        // Opening the output PPM file for writing pixel matrix as binary data.
        file_out = fopen(argv[argc - 1], "r+b");
        fseek(file_out, 0, SEEK_END);

        // Performing decompression and pixel matrix writing.
        root = decompress_level_traversal(file_in, root, end_position, grid, height);
        for (i = 0; i < height; i++)
        {
            fwrite(&grid[i][0], sizeof(grid_t), width, file_out);
        }
        fclose(file_in);
    }
    // Deallocate resources.
    fclose(file_out);
    dealloc_grid(grid, height);
    dealloc_tree(root);
    return 0;
}