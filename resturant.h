#ifndef _restaurant_h //if not define already
#define _restaurant_h //define the header
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for using strlen and strcmp
#define MAX_LEN_NAME 50 //define constant value for the name of the meal
#define MAX_TABLES 50 //define constant value for the number of tables
typedef struct Manot { //define structure for the meals
	char* meal_name;
	float price;
	unsigned int quantity;
	struct Manot* next; //define pointer to the next meal in the linked list
}Manot;
typedef struct List { //define List manager
	Manot* head;
	Manot* tail;
}List;
//functions declarations
void ErrorMsg(char* msg);
void CreateProducts(List* manager);
void free_memory(Manot* head);
FILE* open_file(char* file_name, char* file_format);
void AddItems(Manot* head, char* ProductName, int Quantity);
void OrderItem(int TableNumber, char* ProductName, int Quantity, List* manager, List table_array[]);
Manot* findProduct(Manot* head, char* ProductName);
Manot* NewProduct(char* ProductName, int quantity, List* manager);
void RemoveItem(int TableNumber, List table_array[], int is_items_canccelled[]);
void RemoveTable(int TableNumber, List table_array[]);
void ManageInstructions(char* file_name, List* manager, List table_array[], int is_items_cancelled[]);
#endif //end the if
