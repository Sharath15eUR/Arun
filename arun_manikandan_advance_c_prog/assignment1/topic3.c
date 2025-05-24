#include <stdio.h>
#include <stdbool.h>

bool searchMatrix(int matrix[][100], int n, int key) {
    int row = 0;                    // left pointer
    int col = n - 1;                // right pointer

    while (row < n && col >= 0) {
        if (matrix[row][col] == key)
        {
            printf("The key is found at :  %d %d\n\n" ,row , col );
            return true;
        }
        else if (matrix[row][col] > key)
            col--;  // Move left
        else
            row++;  // Move down
    }

    return false;  // Not found
}

// Example usage
int main() {
    int n = 4;
    int matrix[4][100] = {
        {10, 20, 30, 40},
        {15, 25, 35, 45},
        {27, 29, 37, 48},
        {32, 33, 39, 50}
    };

    int key = 29;
    if (searchMatrix(matrix, n, key))
        printf("Key %d found.\n", key);
    else
        printf("Key %d not found.\n", key);

    return 0;
}

