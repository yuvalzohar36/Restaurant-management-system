#include "resturant.h"
int main() {
	List manager; //define new manager List variable
	manager.head = manager.tail = NULL; //initalize the head and tail to NULL
	List table_array[MAX_TABLES]; //define table_array List type for all the tables
	int i, is_items_cancelled[MAX_TABLES] = { 0 }; //define new integer and array for the tables who already cancelled meal
	for (i = 0; i < MAX_TABLES; i++)
		table_array[i].head = table_array[i].tail = NULL; //initalize for each table the head and tail to NULL
	ManageInstructions("Instructions.txt", &manager, table_array, is_items_cancelled); //call to function the manage all the instructions
	free_memory(manager.head); //free the kitchen meals linked list memory
	for (i = 0; i < MAX_TABLES; i++)
		if (table_array[i].head != NULL)
			free_memory(table_array[i].head); //free all the memory of the tables
	return 0;
}