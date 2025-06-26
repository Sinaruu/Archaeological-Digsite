#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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

#define MEMORY_ERROR_MSG "Failed to allocate memory.\n"

int isValidCode(const char *code);
int isDuplicate(char **artifactCodes, int count, const char *code);

int main(void) {
    int row, col, artifacts, i, j, placed, conflict, x, y;
    char *data;
    char **array;
    char **artifactCodes;
    int *artifactX;
    int *artifactY;
    int *artifactFound;

    printf("Rows (%d-%d u): ", MIN_SIZE, MAX_SIZE);
    if (scanf("%d", &row) != 1) {
        printf("Invalid input.\n");
        return 1;
    }

    printf("Columns (%d-%d u): ", MIN_SIZE, MAX_SIZE);
    if (scanf("%d", &col) != 1) {
        printf("Invalid input.\n");
        return 1;
    }

    printf("Amount of artifacts (max %d): ", row * col);
    if (scanf("%d", &artifacts) != 1) {
        printf("Invalid input.\n");
        return 1;
    }

    if (row < MIN_SIZE || row > MAX_SIZE || col < MIN_SIZE || col > MAX_SIZE) {
        printf("Wrong dimensions entered.\nAllowed dimensions: %d-%d units.\n", MIN_SIZE, MAX_SIZE);
        return 1;
    } else if (artifacts > (row * col) || artifacts < 0) {
        printf("Invalid amount of artifacts entered.\n");
        return 1;
    }

    /* Allocate map */
    data = (char *)malloc(row * col * sizeof(char));
    array = (char **)malloc(row * sizeof(char *));
    if (data == NULL || array == NULL) {
        printf(MEMORY_ERROR_MSG);
        return 1;
    }

    for (i = 0; i < row * col; i++) {
        data[i] = ' ';
    }

    for (i = 0; i < row; i++) {
        array[i] = data + i * col;
    }

    /* Allocate artifact data */
    artifactCodes = (char **)malloc(artifacts * sizeof(char *));
    artifactX = (int *)malloc(artifacts * sizeof(int));
    artifactY = (int *)malloc(artifacts * sizeof(int));
    artifactFound = (int *)malloc(artifacts * sizeof(int));

    if (artifactCodes == NULL || artifactX == NULL || artifactY == NULL || artifactFound == NULL) {
        printf(MEMORY_ERROR_MSG);
        return 1;
    }

    for (i = 0; i < artifacts; i++) {
        artifactCodes[i] = (char *)malloc(artifacts * sizeof(char));
        artifactFound[i] = 0;
    }

    /* Artifact code input and validation */
    for (i = 0; i < artifacts; i++) {
        int valid = 0;
        char temp[100];

        while (!valid) {
            printf("Enter artifact code #%d (letter + 3 digits): ", i + 1);
            if (scanf("%99s", temp) != 1) {
                printf("Invalid input.\n");
                return 1;
            }

            if (!isValidCode(temp)) {
                printf("Incorrect entry. Codes must be a letter and three digits.\n");
            } else if (isDuplicate(artifactCodes, i, temp)) {
                printf("Artifact code already in use.\n");
            } else {
                strcpy(artifactCodes[i], temp);
                valid = 1;
            }
        }
    }

    /* Random number generator for artifact placement */
    srand((unsigned int)time(NULL));

    for(i = 0; i < artifacts; i++) {
        placed = 0;
        while(!placed) {
            x = rand() % col;
            y = rand() % row;

            conflict = 0;
            for(j = 0; j < artifacts; j++) {
                if(artifactX[j] == x && artifactY[j] == y) {
                    conflict = 1;
                    break;
                }
            }

            if(!conflict) {
                artifactX[i] = x;
                artifactY[i] = y;
                placed = 1;
            }
        }
    }

    /* This is where game starts*/

    /* Free memory at the end */
    for (i = 0; i < artifacts; i++) {
        free(artifactCodes[i]);
    }
    free(artifactCodes);
    free(artifactX);
    free(artifactY);
    free(artifactFound);
    free(array);
    free(data);

    return 0;
}

/* Check if the artifact code is valid (letter + 3 digits) */
int isValidCode(const char *code) {
    return strlen(code) == 4 &&
           isalpha((unsigned char)code[0]) &&
           isdigit((unsigned char)code[1]) &&
           isdigit((unsigned char)code[2]) &&
           isdigit((unsigned char)code[3]);
}

/* Check for duplicates among previous codes */
int isDuplicate(char **artifactCodes, int count, const char *code) {
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(artifactCodes[i], code) == 0) {
            return 1;
        }
    }
    return 0;
}