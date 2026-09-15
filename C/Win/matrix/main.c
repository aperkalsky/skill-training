#include <stdio.h>
#include <stdlib.h>

/**
 * Accepts a dynamic 2D matrix and returns a completely new, transposed matrix.
 * (rows become columns, columns become rows).
 */
int** transpose_matrix(const int* const* matrix, size_t rows, size_t cols)
{
    // Allocate memory for the row pointers of the new matrix.
    int** new_matrix = (int**)malloc(cols * sizeof(int*));

    if (new_matrix == NULL)
    {
        return NULL;
    }

    // Allocate each row (which has a size equal to the original 'rows')
    for (size_t i = 0; i < cols; i++)
    {
        void* p_col = malloc(rows * sizeof(int));

        if (p_col == NULL)
        {
            for (size_t j = 0; j < i; j++)
            {
                free(new_matrix[j]);
            }

            free(new_matrix);
            return NULL;
        }
        else
        {
            new_matrix[i] = (int*)p_col;
        }
    }

    // Do transpose
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            new_matrix[j][i] = matrix[i][j];
        }
    }

    return new_matrix;
}

/**
 * Safely frees a dynamically allocated 2D matrix.
 */
void free_matrix(int** matrix, size_t row_count)
{
    if (matrix == NULL) return;

    for (size_t i = 0; i < row_count; i++)
    {
        free(matrix[i]);
    }

    free(matrix);
}

// ==========================================
// TEST HARNESS
// ==========================================
int main()
{
    printf("--- Test: Matrix Transpose via Safe Double Pointers ---\n");

    size_t rows = 3;
    size_t cols = 4;

    // Allocate and construct a sample 3x4 matrix
    int** original = (int**)malloc(rows * sizeof(int*));
    for (size_t i = 0; i < rows; i++)
    {
        original[i] = (int*)malloc(cols * sizeof(int));
    }

    // Fill with sample sequential data:
    // [ 1,  2,  3,  4 ]
    // [ 5,  6,  7,  8 ]
    // [ 9, 10, 11, 12 ]
    int counter = 1;
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            original[i][j] = counter++;
        }
    }

    printf("Original Matrix (3x4):\n");
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            printf("%3d ", original[i][j]);
        }
        printf("\n");
    }

    // Call transpose. We pass 'original' directly. 
    // The compiler implicitly handles casting int** to const int *const * safely.
    int** transposed = transpose_matrix((const int* const*)original, rows, cols);

    if (transposed != NULL) {
        printf("\nTransposed Matrix (4x3):\n");
        for (size_t i = 0; i < cols; i++)
        {
            for (size_t j = 0; j < rows; j++)
            {
                printf("%3d ", transposed[i][j]);
            }
            printf("\n");
        }

        // Clean up transposed matrix
        free_matrix(transposed, cols);
    }
    else
    {
        printf("\nTransposition Failed due to allocation error!\n");
    }

    // Clean up original matrix
    for (size_t i = 0; i < rows; i++)
    {
        free(original[i]);
    }
    free(original);

    return 0;
}
