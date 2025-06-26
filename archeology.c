#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MIN_SIZE 2
#define MAX_SIZE 40

#define GREATER_ARTIFACT 'G'
#define LESSER_ARTIFACT 'A'
#define NEAREST_ARTIFACT_FAR '#'
#define FOUND_ARTIFACT '!'
#define UNFOUND_ARTIFACT_DEBUG '?'

#define POINTS_PER_GREATER 2
#define POINTS_PER_LESSER 1

#define DEBUG_MODE 1

#define MEMORY_ERROR_MSG "Failed to allocate memory.\n"

int isValidCode(const char *code);
int isDuplicate(char **artifactCodes, int count, const char *code);
void freeArtifacts(char **codes, int *x, int *y, int *found, int count);
int getArtifactValue(const char *code);

int main(void) {
    int row, col, artifacts, i;
    char *map;
    char **artifactCodes;
    int *artifactX, *artifactY, *artifactFound;

    printf("Rows (%d-%d u): ", MIN_SIZE, MAX_SIZE);
    if (scanf("%d", &row) != 1) return printf("Invalid input.\n"), 1;

    printf("Columns (%d-%d u): ", MIN_SIZE, MAX_SIZE);
    if (scanf("%d", &col) != 1) return printf("Invalid input.\n"), 1;

    printf("Amount of artifacts (max %d): ", row * col);
    if (scanf("%d", &artifacts) != 1) return printf("Invalid input.\n"), 1;

    if (row < MIN_SIZE || row > MAX_SIZE || col < MIN_SIZE || col > MAX_SIZE) {
        printf("Wrong dimensions entered.\nAllowed dimensions: %d-%d units.\n", MIN_SIZE, MAX_SIZE);
        return 1;
    } else if (artifacts > row * col || artifacts < 0) {
        printf("Invalid amount of artifacts entered.\n");
        return 1;
    }

    map = malloc(row * col);
    if (!map) return printf(MEMORY_ERROR_MSG), 1;
    for (i = 0; i < row * col; i++) map[i] = ' ';

    artifactCodes = malloc(artifacts * sizeof(char *));
    artifactX = malloc(artifacts * sizeof(int));
    artifactY = malloc(artifacts * sizeof(int));
    artifactFound = malloc(artifacts * sizeof(int));

    if (!artifactCodes || !artifactX || !artifactY || !artifactFound) {
        printf(MEMORY_ERROR_MSG);
        freeArtifacts(artifactCodes, artifactX, artifactY, artifactFound, artifacts);
        free(map);
        return 1;
    }

    for (i = 0; i < artifacts; i++) {
        artifactCodes[i] = malloc(5);
        if (!artifactCodes[i]) {
            printf(MEMORY_ERROR_MSG);
            freeArtifacts(artifactCodes, artifactX, artifactY, artifactFound, artifacts);
            free(map);
            return 1;
        }
        artifactFound[i] = 0;
    }

    for (i = 0; i < artifacts; i++) {
        int valid = 0;
        char temp[5];
        while (!valid) {
            printf("Enter artifact code #%d (letter + 3 digits): ", i + 1);
            if (scanf("%4s", temp) != 1) return printf("Invalid input.\n"), 1;
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

    srand((unsigned int)time(NULL));
    for (i = 0; i < artifacts; i++) {
        int placed = 0, j, x, y, conflict;
        while (!placed) {
            x = rand() % col;
            y = rand() % row;
            conflict = 0;
            for (j = 0; j < i; j++) {
                if (artifactX[j] == x && artifactY[j] == y) {
                    conflict = 1;
                    break;
                }
            }
            if (!conflict) {
                artifactX[i] = x;
                artifactY[i] = y;
                placed = 1;
            }
        }
    }

    /* Gameplay loop to be implemented */

    freeArtifacts(artifactCodes, artifactX, artifactY, artifactFound, artifacts);
    free(map);
    return 0;
}

int isValidCode(const char *code) {
    return strlen(code) == 4 &&
           isalpha((unsigned char)code[0]) &&
           isdigit((unsigned char)code[1]) &&
           isdigit((unsigned char)code[2]) &&
           isdigit((unsigned char)code[3]);
}

int isDuplicate(char **artifactCodes, int count, const char *code) {
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(artifactCodes[i], code) == 0) return 1;
    }
    return 0;
}

void freeArtifacts(char **codes, int *x, int *y, int *found, int count) {
    int i;
    if (codes) {
        for (i = 0; i < count; i++) free(codes[i]);
        free(codes);
    }
    if (x) free(x);
    if (y) free(y);
    if (found) free(found);
}

int getArtifactValue(const char *code) {
    return isupper((unsigned char)code[0]) ? 2 : 1;
}