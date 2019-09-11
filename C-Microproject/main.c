/*
 * Author: Robert Kilmer
 * Date: Fall 2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char itemName[50];
    int price;
    int placed;
} Item;

int main() {
    int row, column;
    Item** array2D;
    printf("Welcome to the C Vending Machine. How many rows am I?\n");
    scanf("%d", &row);
    printf("How many columns am I?\n");
    scanf("%d", &column);

    array2D = malloc(sizeof(Item*) * row);
    for (int i = 0; i < row; i++) {
        array2D[i] = malloc(sizeof(Item) * column);
    }
    printf("I am a vending machine with %d rows and %d columns!\n", row, column);

    char input[50];
    char itemName[50];
    int itemPrice, itemRow, itemColumn;
    char delimeter[3] = ",";
    while (strcmp(input, "done") != 0) {
        printf("Add items in the following format 'name,price,row,column'. Say 'done' when finished adding items\n");
        scanf("%s", input);
        if (strcmp(input, "done") != 0) {
            char *token = strtok(input, delimeter);
            strcpy(itemName, token);
            token = strtok(NULL, delimeter);
            itemPrice = atoi(token);
            token = strtok(NULL, delimeter);
            itemRow = atoi(token);
            token = strtok(NULL, delimeter);
            itemColumn = atoi(token);
            if (itemRow > 0 && itemRow <= row && itemColumn > 0 && itemColumn <= column) {
                strcpy(array2D[itemRow - 1][itemColumn - 1].itemName, itemName);
                array2D[itemRow - 1][itemColumn - 1].price = itemPrice;
                array2D[itemRow - 1][itemColumn - 1].placed = 1;
                printf("Added %s at $%d in row %d in spot %d\n", itemName, itemPrice, itemRow, itemColumn);
            } else {
                printf("Spot not in vending machine! Try again!\n");
            }
        }
    }
    while (strcmp (input, "Q") != 0) {
        printf("Choose an option:\n1 -- View an item\nQ -- Exit\n");
        scanf("%s", input, 1);
        if (strcmp(input, "1") == 0) {
            int rowChoice, columnChoice;
            printf("What row do you want?\n");
            scanf("%d", &rowChoice);
            printf("What column do you want?\n");
            scanf("%d", &columnChoice);
            if (rowChoice > 0 && rowChoice <= row && columnChoice > 0 && columnChoice <= column) {
                if (array2D[rowChoice - 1][columnChoice - 1].placed == 1) {
                    printf("Item Found!\n");
                    printf("Item = %s and Price = $%d\n", array2D[rowChoice - 1][columnChoice - 1].itemName,
                       array2D[rowChoice - 1][columnChoice - 1].price);
                } else {
                    printf("There is nothing in this spot! Try again!\n");
                }
            } else {
                printf("Spot not in vending machine! Try again!\n");
            }
        } else if (strcmp(input, "Q") != 0) {
            printf("I don't understand that input. Can we try again?\n");
        }
    }
    return 0;
}