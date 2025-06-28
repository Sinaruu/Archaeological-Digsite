/*
    Student name: Egor Kivilev
    Student#: 041-160-949
    Course: CST8234-301
    Assignment 1 - Archaelogical Digsite
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

/* Game settings */
#define MIN_SIZE 2
#define MAX_SIZE 40

#define NEAREST_ARTIFACT_FAR '#'
#define FOUND_ARTIFACT '!'
#define UNFOUND_ARTIFACT_DEBUG '?'

#define POINTS_PER_GREATER 2
#define POINTS_PER_LESSER 1

/* Error codes/messages */

#define MEMORY_ERROR_MSG "Failed to allocate memory.\n"
#define SIGNATURE "Code by Egor Kivilev\n"
#define EXIT_SUCCESS 0
#define EXIT_FAILED 1

int isValidCode(const char *code);
int isDuplicate(char **artifactCodes, int count, const char *code);
void freeArtifacts(char **codes, int *x, int *y, int *found, int count);
int getArtifactValue(const char *code);

int main(void) {
    int row, col, artifacts, i, r, c;
    char *map;
    char **artifactCodes;
    int *artifactX, *artifactY, *artifactFound;
    int found, worth;

    int score = 0;
    int debugMode = 0;
    int choice, x, y;

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

    /* Praying that memory is allocated */
    if (!artifactCodes || !artifactX || !artifactY || !artifactFound) {
        printf(MEMORY_ERROR_MSG);
        freeArtifacts(artifactCodes, artifactX, artifactY, artifactFound, artifacts);
        free(map);
        return EXIT_FAILED;
    }

    for (i = 0; i < artifacts; i++) {
        artifactCodes[i] = malloc(sizeof(char) * 4);
        if (!artifactCodes[i]) {
            printf(MEMORY_ERROR_MSG);
            freeArtifacts(artifactCodes, artifactX, artifactY, artifactFound, artifacts);
            free(map);
            return EXIT_FAILED;
        }
        artifactFound[i] = 0;
    }

    for (i = 0; i < artifacts; i++) {
        int valid = 0;
        char temp[4];
        while (!valid) {
            printf("Enter artifact code #%d (letter + 3 digits): ", i + 1);
            if (scanf("%4s", temp) != 1) {
                return printf("Invalid input.\n"), 1;
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

    /* Randomly placing the artifacts */

    srand((unsigned int)time(NULL));
    for (i = 0; i < artifacts; i++) {
        int placed = 0, j, x, y, conflict;
        while (!placed) {
            x = rand() % col;
            y = rand() % row;
            conflict = 0;
            for (j = 0; j < artifacts; j++) {
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

    while(1) {
        int allFound = 1;

        /* Display the map */

        printf("Map\n   ");
        for(i = 0; i < col; i++) {
            printf("%d", i % 10);
        }
        printf("\n   ");
        for(i = 0; i < col; i++) {
            printf("-");
        }
        printf("\n");

        for(r = 0; r < row; r++) {
            printf("%02d|", r);
            for(c = 0; c < col; c++) {
                char symbol = map[r * col + c];

                if(debugMode && symbol == ' ') {
                    for(i = 0; i < artifacts; i++) {
                        if(!artifactFound[i] && artifactX[i] == c && artifactY[i] == r) {
                            symbol = UNFOUND_ARTIFACT_DEBUG;
                        }
                    }
                }

                putchar(symbol);
            }

            printf("\n");
        }

        /* Give options to a player */

        printf("1: Dig a spot\n2: Exit\n3: Toggle debug mode\n-> ");
        if (scanf("%d", &choice) != 1) { /* If a character is entered stop the game. (or else infinite loop) */
            printf("Non-number entered. Stopping program.\n");
            break;
        }

        /* Dig a spot */
        if(choice == 1) {
            printf("x = ");
            if(scanf("%d", &x) != 1 || x < 0 || x >= col) {
                printf("Invalid x coordinate.\n");
                continue;
            }
            printf("y = ");
            if(scanf("%d", &y) != 1 || y < 0 || y >= row) {
                printf("Invalid y coordinate.\n");
                continue;
            }

            /* If the user tries to dig the spot they already did*/

            if (map[y * col + x] != ' ') {
                printf("You already dug here silly!\n");
                continue;
            }

            /* Checks if there is a artifact already here */
            found = 0;
            for(i = 0; i < artifacts; i++) {
                if(!artifactFound[i] && artifactX[i] == x && artifactY[i] == y) {
                    artifactFound[i] = 1;
                    map[y * col + x] = FOUND_ARTIFACT;
                    worth = getArtifactValue(artifactCodes[i]);
                    score += worth;
                    printf("You found artifact %s! (+%d point%s)\n", artifactCodes[i], worth, worth == 1 ? "" : "s");
                    found = 1;
                    break;
                }
            }

            /* Checks the distance to the nearest artifact if the spot is empty */
            if(!found) {
                int minDistance = MAX_SIZE * MAX_SIZE;
                for(i = 0; i < artifacts; i++) {
                    if(!artifactFound[i]) {
                        int dx = x - artifactX[i];
                        int dy = y - artifactY[i];
                        int dist = (int) sqrt(dx * dx + dy * dy); /* Calculating distance */
                        if(dist < minDistance) {
                            minDistance = dist;
                        }
                    }
                }

                /* Determining if the artifact is close enough for the number (dont ask why using like js) */
                map[y * col + x] = (minDistance <= 9) ? ('0' + minDistance) : NEAREST_ARTIFACT_FAR;
            }
        }

        /* Exit program */
        else if(choice == 2) {
            break;
        }

        /* sv_cheats 1 */
        else if(choice == 3) {
            debugMode = !debugMode;
            printf("Debug mode %s\n", debugMode ? "ON" : "OFF");
        }

        /* Anything else */
        else {
            printf("Incorrect choice");
            continue;
        }

        /* Checking if all artifacts are found. */

        for(i = 0; i < artifacts; i++) {
            if(!artifactFound[i]) {
                allFound = 0;
                break;
            }
        }

        /* Game ends when all artifacs are found. */
        if(allFound) {
            printf("\nAll artifacts are found! Final score: %d\n", score);
            break;
        }
    }

    /* Free memory */

    freeArtifacts(artifactCodes, artifactX, artifactY, artifactFound, artifacts);
    free(map);

    printf(SIGNATURE);
    return EXIT_SUCCESS;
}

/* Checks if code is valid */
int isValidCode(const char *code) {
    return strlen(code) == 4 &&
           isalpha((unsigned char)code[0]) &&
           isdigit((unsigned char)code[1]) &&
           isdigit((unsigned char)code[2]) &&
           isdigit((unsigned char)code[3]);
}

/* Checks if the code entered already exists */
int isDuplicate(char **artifactCodes, int count, const char *code) {
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(artifactCodes[i], code) == 0) return 1;
    }
    return 0;
}

/* Frees memory of artifacts */
void freeArtifacts(char **codes, int *x, int *y, int *found, int count) {
    int i;
    if (codes) {
        for (i = 0; i < count; i++) {
            free(codes[i]);
        }
        free(codes);
    }
    if (x) {
        free(x);
    }
    if (y) {
        free(y);
    }
    if (found) {
        free(found);
    }
}

/* Returns value of the artifact */
int getArtifactValue(const char *code) {
    return isupper((unsigned char)code[0]) ? POINTS_PER_GREATER : POINTS_PER_LESSER;
}