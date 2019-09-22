#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct of the machine states and commands
typedef struct {
    int currentState;
    char readVal;
    char writeVal;
    char moveDirection;
    int newState;
} StateMachine;

// Struct of the tape
typedef struct tape {
    char content;
    struct tape *left;
    struct tape *right;
};

int main( ) {
    // Initialize variables
    char initialContent[250] = "A", tempInitialContent[250], fileLine;
    int numberOfStates, startState, endState, currentState;
    FILE *file;

    // Get the file from the user
    char fileName[1000];
    printf("What is the location of the file?\n");
    scanf("%s", &fileName);
    file = fopen(&fileName, "r");

    // Check to make sure it exists
    if (file == NULL) {
        perror("No file found.\n");
        exit(1);
    }

    printf("Got a file!\n");

    // Read the file and get the initial state, the number of states, the start state, and the end state.
    fscanf(file, "%s %d %d %d", &tempInitialContent, &numberOfStates, &startState, &endState);
    strcat(initialContent, tempInitialContent);
    printf("The initial state is %s with %d total states starting at %d and ending at %d.\n", &initialContent, numberOfStates, startState, endState);

    // Create the array of rules
    printf("Creating array...\n");
    StateMachine** stateArray = malloc(sizeof(StateMachine) * numberOfStates);
    for (int j = 0; j < numberOfStates; j++) {
        stateArray[j] = malloc(sizeof(StateMachine) * 128);
    }
    printf("Array created...\n");

    // Parse the file and fill in the rules
    while ((fileLine = fgetc(file)) != EOF) {
        int currentState, newState;
        char readVal[3], writeVal[3], moveDirection[3];
        fscanf(file, "(%d,%c)->(%c,%c,%d)", &currentState, &readVal, &writeVal, &moveDirection, &newState);
        printf("%d %c %c %c %d\n", currentState, readVal[0], writeVal[0], moveDirection[0], newState);

        printf("Ascii value of %c = %d\n", readVal[0], readVal[0]);
        int ascii = readVal[0];
        stateArray[currentState][ascii].currentState = currentState;
        stateArray[currentState][ascii].readVal = readVal[0];
        stateArray[currentState][ascii].writeVal = writeVal[0];
        stateArray[currentState][ascii].moveDirection = moveDirection[0];
        stateArray[currentState][ascii].newState = newState;

        printf("%d", stateArray[currentState][ascii].currentState);
        printf("%c", stateArray[currentState][ascii].readVal);
        printf("%c", stateArray[currentState][ascii].writeVal);
        printf("%c", stateArray[currentState][ascii].moveDirection);
        printf("%d", stateArray[currentState][ascii].newState);
        printf("\n");

    }

    struct tape *newTape;
    struct tape *tempTape;

    // Create initial tape struct
    struct tape *head = malloc(sizeof(struct tape));
    head->content = initialContent[0];
    head->right = NULL;
    head->left = NULL;
    tempTape = head;
    int i = 1;

    // Create the rest of the tape tape
    while (initialContent[i] != NULL) {
        printf("TAPE TRAIN BABY %c\n", initialContent[i]);
        newTape = malloc(sizeof(struct tape));
        newTape->content = initialContent[i];
        newTape->right = NULL;
        newTape->left = tempTape;
        tempTape->right = newTape;
        tempTape = tempTape->right;
        i++;
    }

    currentState = startState;
    int tapePosition = 0;

    // Walk and write to the tape
    while (currentState != endState) {
        struct tape *walkingTape = head;
        struct tape *printTape = head;

        // Get to the current position of the tape
        for (int i = 0; i < tapePosition; i++) {
            walkingTape = walkingTape->right;
        }

        printf("--------------------\n");

        // Initialize variables
        int ascii = walkingTape->content;
        char writeVal = stateArray[currentState][ascii].writeVal;
        char moveDir = stateArray[currentState][ascii].moveDirection;

        // Set new value of the walking tape and set the new state
        walkingTape->content = writeVal;
        currentState = stateArray[currentState][ascii].newState;

        // Print out the tape as it currently is
        while (printTape != NULL) {
            printf("%c", printTape->content);
            printTape = printTape->right;
        }
        printf("\n");

        printf("Write: %c, Move %c, New State %d\n", writeVal, moveDir, currentState);
        // Create a new tape struct if needed, else change the tape position to be correct
        switch(moveDir) {
            case 'L':
                if (walkingTape-> left == NULL) {
                    printf("Add new tape left\n");
                    struct tape *newRightTape = malloc(sizeof(struct tape));
                    newRightTape->content = 'B';
                    newRightTape->left = NULL;
                    newRightTape->right = walkingTape;
                    walkingTape->left = newRightTape;
                }
                tapePosition--;
                break;
            case 'R':
                if (walkingTape->right == NULL) {
                    struct tape *newRightTape = malloc(sizeof(struct tape));
                    newRightTape->content = 'B';
                    newRightTape->right = NULL;
                    newRightTape->left = walkingTape;
                    walkingTape->right = newRightTape;
                }
                tapePosition++;
                break;
        }
    }


    printf("--------------------\n");
    printf ("Final Tape: ");
    // Print out the tape
    while (head != NULL) {
        printf("%c", head->content);
        head = head->right;
    }
    return 0;
}
