#include <stdio.h>
#include <stdlib.h>

#define MIN_SIZE 2
#define MAX_SIZE 40

#define GREATER_ARTIFACT "G"
#define LESSER_ARTIFACT "A"
#define NEAREST_ARTIFACT_FAR "#"
#define FOUND_ARTIFACT "!"
#define UNFOUND_ARTIFACT_DEBUG "?"

#define POINTS_PER_GREATER 2
#define POINTS_PER_LESSER 1

#define DEBUG_MODE 1

int main() {
    int row, col, artifacts, i;
    char* data;
    char **array;

    /* Getting field dimensions */
    printf("Rows (%d-%d u): ", MIN_SIZE, MAX_SIZE);
    scanf("%d", &row);

    printf("Columns (%d-%d u): ", MIN_SIZE, MAX_SIZE);
    scanf("%d", &col);

    printf("Amount of artifacts (max %d): ", row * col);
    scanf("%d", &artifacts);

    /* Check bounds */
    if (row < MIN_SIZE || row > MAX_SIZE || col < MIN_SIZE || col > MAX_SIZE) {
        printf("Wrong dimensions entered.\nAllowed dimensions: %d-%d units for both rows and columns.\n", MIN_SIZE, MAX_SIZE);
        return 1;
    } else if(artifacts > (row * col) || artifacts < 0) {
        printf("Invalid amount of artifacts entered.\n");
        return 1;
    }

    printf("Generating an area of %d rows and %d columns...\n", row, col);

    /* Allocate flat block */
    data = malloc(row * col * sizeof(char));
    if (data == NULL) {
        printf("Failed to allocate memory\n");
        return 1;
    }

    /* Allocate row pointers */
    array = malloc(row * sizeof(char *));
    if (array == NULL) {
        printf("Failed to allocate memory\n");
        free(data);
        return 1;
    }

    /* Map rows to block */
    for (i = 0; i < row; i++) {
        array[i] = data + i * col;
    }

    /* Initialization */
    for (i = 0; i < row * col; i++) {
        data[i] = ' '; /* blank space for undug cells */
    }



    /* Free memory at the end */
    free(array);
    free(data);

    return 0;
}
