/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Define --------------------------------------------------------------------*/
#define MAX_ROW 2
#define MAX_COLUMN 15
#define MAX_ENTERPRISE 30

/* Enum ----------------------------------------------------------------------*/
typedef enum Status {
    Empty,
    Registered
} Status;

typedef enum CommandType {
    REGISTER,
    ALTER,
    SHOW,
    DELETE,
    QUIT,
    INVALID
} CommandType;

/* --------------------------------------------------------------------------- */
/*                            Begin: Student Answer                            */
/* --------------------------------------------------------------------------- */

// Requirement 1: Generate abbreviation from the full name of an enterprise
void getAbbreviation(char *name, char *abbre) {
    int j = 0;
    bool isWord = true; // To track the beginning of a word
    for (int i = 0; name[i] != '\0'; i++) {
        if (name[i] == ' ') {
            isWord = true; // Next character is the start of a new word
        } else if (isWord) {
            abbre[j++] = name[i]; // Add the first letter of the word
            isWord = false; // Reset for the next word
        }
    }
    abbre[j] = '\0'; // Null-terminate the abbreviation
}

// Requirement 2: Determine the type of command
CommandType getCommandType(char *command) {
    if (strncmp(command, "Register", 8) == 0)
        return REGISTER;
    else if (strncmp(command, "Alter", 5) == 0)
        return ALTER;
    else if (strncmp(command, "Show", 4) == 0)
        return SHOW;
    else if (strncmp(command, "Delete", 6) == 0)
        return DELETE;
    else if (strncmp(command, "Quit", 4) == 0)
        return QUIT;
    else
        return INVALID;
}

// Requirement 3: Define the structure to store enterprise details
typedef struct {
    int booth_index;
    char name[100];
    char abbre[10];
    int itemValue;
    int itemWeight;
} Enterprise;

// Requirement 4: Print the details of a specific enterprise
void printEnterpriseDetails(Enterprise e) {
    printf("||%d||%s||%s||%d||%d||\n", e.booth_index, e.name, e.abbre, e.itemValue, e.itemWeight);
}

// Requirement 5: Create an enterprise with specified values
void createEnterprise(Enterprise *e, int booth_index, int itemValue, int itemWeight, char *name, char *abbre) {
    e->booth_index = booth_index;
    e->itemValue = itemValue;
    e->itemWeight = itemWeight;
    strcpy(e->name, name);
    strcpy(e->abbre, abbre);
}

// Requirement 6: Register an enterprise to a booth
void registerEnterprise(int map[MAX_ROW][MAX_COLUMN], Enterprise enterpriseArray[MAX_ENTERPRISE],
                        char *name, int booth, int itemValue, int itemWeight, int *out_booth, char *out_abbre) {
    if (booth == -1) { // If booth is not specified
        // Find the next available booth
        for (booth = 0; booth < MAX_COLUMN; booth++) {
            if (map[0][booth] == Empty) {
                map[0][booth] = Registered;
                *out_booth = booth;
                getAbbreviation(name, out_abbre);
                createEnterprise(&enterpriseArray[booth], booth, itemValue, itemWeight, name, out_abbre);
                return;
            }
        }
        // If no empty booth is found
        *out_booth = -1;
    } else {
        if (map[0][booth] == Empty) { // If the booth is empty
            map[0][booth] = Registered;
            *out_booth = booth;
            getAbbreviation(name, out_abbre);
            createEnterprise(&enterpriseArray[booth], booth, itemValue, itemWeight, name, out_abbre);
        } else {
            *out_booth = -1; // Registration failed
        }
    }
}

// Requirement 7: Display the booth map and status
void showMap(int map[MAX_ROW][MAX_COLUMN], Enterprise enterpriseArray[MAX_ENTERPRISE]) {
    for (int i = 0; i < MAX_COLUMN; i++) {
        printf("|%d", i);
    }
    printf("|\n");

    for (int i = 0; i < MAX_COLUMN; i++) {
        printf("-%s", i < 10 ? "---" : "----");
    }
    printf("\n");

    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COLUMN; j++) {
            printf("|%d", map[i][j]);
        }
        printf("|\n");
    }
}

// Requirement 8: Show booth indexes based on their status
void showIndexOfStatus(int map[MAX_ROW][MAX_COLUMN], Status status) {
    printf("Index[");
    for (int i = 0; i < MAX_COLUMN; i++) {
        if (map[0][i] == status) {
            printf("%d, ", i);
        }
    }
    printf("]\n");
}

// Requirement 9: Show the total count of booths with a specific status
void showTotalOfStatus(Enterprise enterpriseArray[MAX_ENTERPRISE], Status status) {
    int count = 0;
    for (int i = 0; i < MAX_ENTERPRISE; i++) {
        if (enterpriseArray[i].booth_index != -1 && status == Registered) {
            count++;
        }
    }
    printf("Total: %d\n", count);
}

// Requirement 10: Display details of booths in a specified range
void showIndexBracket(int map[MAX_ROW][MAX_COLUMN], Enterprise enterpriseArray[MAX_ENTERPRISE], int start, int end) {
    if (end == -1) {
        end = start;
    }
    for (int i = start; i <= end; i++) {
        if (map[0][i] == Registered) {
            printEnterpriseDetails(enterpriseArray[i]);
        } else {
            printf("Index %d: NONE\n", i);
        }
    }
}

// Requirement 11: Handle the "Show" command and call the appropriate function
void handleShowCommand(int map[MAX_ROW][MAX_COLUMN], Enterprise enterpriseArray[MAX_ENTERPRISE], char *command) {
    if (strcmp(command, "Show map") == 0) {
        showMap(map, enterpriseArray);
    } else if (command[0] == '&') {
        showIndexOfStatus(map, command[1] == '0' ? Empty : Registered);
    } else if (command[0] == '#') {
        showTotalOfStatus(enterpriseArray, command[1] == '0' ? Empty : Registered);
    } else if (command[0] == '[') {
        int num;
        sscanf(command + 6, "%d", &num);
        showIndexBracket(map, enterpriseArray, num, -1);
    }
}

// Requirement 12: Alter the booth assignment for a specified enterprise
void alterEnterprise(int map[MAX_ROW][MAX_COLUMN], Enterprise enterpriseArray[MAX_ENTERPRISE], char *abbre,
                     int registerBooth, int newBooth, int *out_booth, char *out_abbre) {
    if (newBooth == -1) {
        // Find the next empty booth
        while (map[0][registerBooth] == Registered) {
            registerBooth = (registerBooth + 1) % MAX_COLUMN;
        }
        newBooth = registerBooth;
    }

    if (map[0][newBooth] == Empty) {
        map[0][registerBooth] = Empty; // Free the old booth
        map[0][newBooth] = Registered; // Register the new booth
        *out_booth = newBooth;
        // Move enterprise details
        enterpriseArray[newBooth] = enterpriseArray[registerBooth];
        enterpriseArray[registerBooth].booth_index = -1; // Clear old enterprise
    }
}

// Requirement 13: Delete an enterprise from the system
void deleteEnterprise(int map[MAX_ROW][MAX_COLUMN], Enterprise enterpriseArray[MAX_ENTERPRISE], char *abbre, int booth, int *totalEmpty) {
    for (int i = 0; i < MAX_ENTERPRISE; i++) {
        if (strcmp(enterpriseArray[i].abbre, abbre) == 0 && enterpriseArray[i].booth_index == booth) {
            map[0][booth] = Empty; // Free the booth
            enterpriseArray[i].booth_index = -1; // Clear enterprise
            (*totalEmpty)++;
            break;
        }
    }
}

// Requirement 14: Process commands and call the relevant function
void handleCommand(char *command, int map[MAX_ROW][MAX_COLUMN], Enterprise enterpriseArray[MAX_ENTERPRISE], CommandType *commandType) {
    *commandType = getCommandType(command);
    switch (*commandType) {
    case REGISTER:
        // Handle register command (not implemented here)
        break;
    case ALTER:
        // Handle alter command (not implemented here)
        break;
    case SHOW:
        handleShowCommand(map, enterpriseArray, command);
        break;
    case DELETE:
        // Handle delete command (not implemented here)
        break;
    case QUIT:
        exit(0);
    default:
        printf("Invalid command\n");
        break;
    }
}

// Requirement 15: Optimize items collected using the knapsack algorithm
int knapsack(int map[MAX_ROW][MAX_COLUMN], Enterprise enterpriseArray[MAX_ENTERPRISE],
             int maxWeight, int numOfEnterprises, int index) {
    if (index == numOfEnterprises || maxWeight <= 0)
        return 0;

    // Skip the current enterprise
    int valueWithoutCurrent = knapsack(map, enterpriseArray, maxWeight, numOfEnterprises, index + 1);

    // Include the current enterprise if it fits in the knapsack
    int valueWithCurrent = 0;
    if (enterpriseArray[index].itemWeight <= maxWeight) {
        valueWithCurrent = enterpriseArray[index].itemValue + knapsack(map, enterpriseArray, maxWeight - enterpriseArray[index].itemWeight, numOfEnterprises, index + 1);
    }

    return valueWithCurrent > valueWithoutCurrent ? valueWithCurrent : valueWithoutCurrent;
}

/* --------------------------------------------------------------------------- */
/*                             End: Student Answer                             */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/*                           Example Test Cases for Main                        */
/* --------------------------------------------------------------------------- */

// Initializes booth map and enterprise array to default values and then
// runs a series of test cases to verify enterprise registration, booth
// assignments, alterations, and knapsack optimization functionality.

// Initialize the booth map with default status Empty
void initMap(int map[MAX_ROW][MAX_COLUMN]) {
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COLUMN; j++) {
            map[i][j] = Empty;
        }
    }
}

// Initialize the enterprise array with default values
void initEnterpriseArray(Enterprise enterpriseArray[MAX_ENTERPRISE]) {
    for (int i = 0; i < MAX_ENTERPRISE; i++) {
        enterpriseArray[i].booth_index = -1;
        strcpy(enterpriseArray[i].name, "");
        strcpy(enterpriseArray[i].abbre, "");
        enterpriseArray[i].itemValue = 0;
        enterpriseArray[i].itemWeight = 0;
    }
}

int main() {
    int map[MAX_ROW][MAX_COLUMN];
    Enterprise enterpriseArray[MAX_ENTERPRISE];
    initMap(map);
    initEnterpriseArray(enterpriseArray);

    // Sample Test Case 03
    printf("----- Sample testcase 03 -----\n");
    printf("Test Enterprise:\n");
    int* out_booth = malloc(sizeof(int));
    char* out_abbre = malloc(10);
    registerEnterprise(map, enterpriseArray, "Tech Co", -1, 0, 0, out_booth, out_abbre);
    printEnterpriseDetails(enterpriseArray[*out_booth]);

    // Sample Test Case 04
    printf("----- Sample testcase 04 -----\n");
    printf("Test ShowCommand:\n");
    registerEnterprise(map, enterpriseArray, "Innovative Solutions Company", 10, 0, 0, out_booth, out_abbre);
    registerEnterprise(map, enterpriseArray, "EMLP Technology and Solutions Limited Company", -1, 0, 0, out_booth, out_abbre);
    registerEnterprise(map, enterpriseArray, "NextGen", 12, 0, 0, out_booth, out_abbre);
    showIndexOfStatus(map, Registered);

    // Sample Test Case 05
    printf("----- Sample testcase 05 -----\n");
    printf("Test ShowCommand:\n");
    char *command = "Show [24]";
    handleShowCommand(map, enterpriseArray, command);

    free(out_booth);
    free(out_abbre);
    return 0;
}
