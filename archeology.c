#include <stdio.h>
#include <stdlib.h>

#define GREATER_ARTIFACT "G"
#define LESSER_ARTIFACT "A"

#define MIN_SIZE 2
#define MAX_SIZE 40

int main() {
    int row, col, i;

    /* Getting field dimensions */
    printf("Rows (2-40 u): ");
    scanf("%d", &row);

    printf("Columns (2-40 u): ");
    scanf("%d", &col);

    /* Checking if the values entered are correct */
    if(row < MIN_SIZE || col < MIN_SIZE || row > MAX_SIZE || col > MAX_SIZE) {
        printf("Wrong dimensions entered.\nAllowed dimensions: 2-40 units for both rows and columns.\n");
        return 1;
    }

    printf("Generating an area of %d rows and %d columns...\n", row, col);

    /* Attempting to allocate memory for the field */
    char *data = malloc(row * col * sizeof(char));

    /* Stop program if allocation fails */
    if(data == NULL) {
        printf("Failed to allocate memory\n");
        return 1;
    }
    
    /* Freeing memory */
    free(data);

    return 0;
}