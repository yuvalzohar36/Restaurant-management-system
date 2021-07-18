#include "resturant.h" //call the the resturant header
void ErrorMsg(char* msg) {
	printf("\n%s\n", msg); //print the error message
	exit(1); //close the file
}
FILE* open_file(char* file_name, char* file_format) {
	FILE* file = fopen(file_name, file_format); //open the file with the name and file format 
	if (file == NULL) //in case of error while opening the file
		ErrorMsg("error while opening the file");
	return file;
}

void CreateProducts(List* manager) {
	FILE* meals_file = open_file("Manot.txt", "r"); //open the Manot.txt file
	char temp_str[MAX_LEN_NAME + 1]; //define new string for the name of the meal
	float price; //define float number for the price
	unsigned int quantity;//define unsigned integer number for the product quantity
	Manot* temp = NULL; //define temp variable from Manot pointer type
	while ((fscanf(meals_file, "%s %d %f", temp_str, &quantity, &price)) != EOF) {//while there is still meals in the file
		temp = NewProduct(temp_str, quantity, manager);// call to function that create new product
		temp->price = price; //update the price of the product
		if (manager->head == NULL) //in case of empty list
			manager->head = temp; //update the new head
		else
			manager->tail->next = temp; // update the previous tail to its point the new tail
		manager->tail = temp; //update the tail
	}
	fclose(meals_file); //close the file
	printf("The kitchen was created\n");
}

void free_memory(Manot* head) {
	Manot* temp = NULL; //define new pointer to Manot type variable
	while (head != NULL) { //in case there is still meals in the linked list
		temp = head;
		head = head->next; //update the head to the next one
		free(temp->meal_name); // free the name allocated memory
		free(temp);//free all the  structure allocated memory
	}
}

void AddItems(Manot* head, char* ProductName, int Quantity) {
	int changed = 0; //define new integer variable
	if (Quantity > 0) { // in case the quantity greater than zero
		while (head != NULL && !changed) { //while there is still items in the linked list and not already changed
			if (!(strcmp(head->meal_name, ProductName))) { //in case the two Product name equals the meal name
				head->quantity += Quantity;
				changed = 1;
				printf("%d %s were added to the kitchen\n", Quantity, ProductName);
			}
			else
				head = head->next; //update the head to the next one in the linked list
		}
		if (!changed) // in case of didnt found the specific meal name
			printf("There is no %s in our meals\n", ProductName);
	}
	else //in case the quantity less or equals to 0
		printf("Quanity less than 0\n");
}

void OrderItem(int TableNumber, char* ProductName, int Quantity, List* manager, List table_array[]) {
	Manot* temp = NULL, * new_product = NULL; // temp new pointer to Manot type variables
	if (!(TableNumber <= 0 || TableNumber > MAX_TABLES)) { //in case that the Table number is legal
		temp = findProduct(manager->head, ProductName); //find the product in the linked list
		if (temp == NULL) //in case it isnt exists
			printf("We don't have %s,sorry!\n", ProductName);
		else if (temp->quantity < Quantity)  //in case there is not enough in the quantity of the product
			printf("There is not enough quantity for %s\n", ProductName);
		else {
			temp->quantity -= Quantity; //update the quantity of the kitchen
			new_product = NewProduct(ProductName, Quantity, manager); //creating new product(node)
			new_product->price = temp->price; //update the product price
			if (table_array[TableNumber - 1].head == NULL) { //in case of empty linked list
				table_array[TableNumber - 1].head = new_product; //update the head
				table_array[TableNumber - 1].tail = new_product; //update the tail
				table_array[TableNumber - 1].head->next = NULL; //update the next of the head to null
				table_array[TableNumber - 1].tail->next = NULL; //update the next of the tail to null
			}
			else {
				new_product->next = table_array[TableNumber - 1].head; // update the next of the new product to the currently head
				table_array[TableNumber - 1].head = new_product; //update the new head
			}
			printf("%d %s were added to the table number %d\n", Quantity, ProductName, TableNumber);
		}
	}
	else
		printf("There is no table number %d\n", TableNumber);
}

Manot* findProduct(Manot* head, char* ProductName) {
	while (head != NULL) { //while there is still items in the linked list
		if (!strcmp(head->meal_name, ProductName)) //in case the meal name equals to the Product name
			return head;
		head = head->next; //update the head to the next node in the linked list
	}
	return NULL;
}

Manot* NewProduct(char* ProductName, int quantity, List* manager) {
	Manot* temp = (Manot*)malloc(sizeof(Manot)); //memory allocation for new product
	if (temp == NULL) { //in case of error while memory allocaton
		free_memory(manager->head); //free all the already allocated memory
		ErrorMsg("error while memory allocation");
	}
	temp->next = NULL;
	temp->meal_name = (char*)malloc(strlen(ProductName) + 1); //memory allocation for the name of the product
	if (temp->meal_name == NULL) { //in case of error while memory allocation
		free_memory(manager->head); //free all the already allocated memory
		ErrorMsg("error while memory allocation");
	}
	strcpy(temp->meal_name, ProductName); //copy the name from ProductName to the meal_name
	temp->quantity = quantity; //initalize the product quantity
	return temp;
}

void RemoveItem(int TableNumber, List table_array[], int is_items_cancelled[]) {
	Manot* temp; //define new pointer to manot type
	if (table_array[TableNumber - 1].head == NULL) //in case of empry list
		printf("The table number %d is not ordered yet\n", TableNumber);
	else if (!is_items_cancelled[TableNumber - 1]) { //in case that the table isnt cancelled meal yet
		printf("%d %s was returned to the kitchen from table number %d\n", table_array[TableNumber - 1].head->quantity, table_array[TableNumber - 1].head->meal_name, TableNumber);
		is_items_cancelled[TableNumber - 1] = 1;
		temp = table_array[TableNumber - 1].head;
		table_array[TableNumber - 1].head = table_array[TableNumber - 1].head->next;
		free(temp->meal_name);
		free(temp);
	}
	else if (is_items_cancelled[TableNumber - 1]) // in case that the table already cancelled meal
		printf("Table number %d already return to the kitchen, sorry\n", TableNumber);
}

void RemoveTable(int TableNumber, List table_array[]) {
	Manot* temp = table_array[TableNumber - 1].head; //initalize variable to the head of the table linked list
	float sum = 0; //initalize float sum variable to 0
	if (table_array[TableNumber - 1].head != NULL) { //in case that the table ordered something
		while ((table_array[TableNumber - 1].head->next) != NULL) { //loop until the last meal in the linked list 
			printf("%d %s, ", table_array[TableNumber - 1].head->quantity, table_array[TableNumber - 1].head->meal_name);
			sum += (table_array[TableNumber - 1].head->price) * (table_array[TableNumber - 1].head->quantity); //calculate the new sum
			table_array[TableNumber - 1].head = table_array[TableNumber - 1].head->next; // update the head to the next meal(node)
		}
	}
	if (table_array[TableNumber - 1].head != NULL) { //in case that the currently head isn't NULL
		printf("%d %s. ", table_array[TableNumber - 1].head->quantity, table_array[TableNumber - 1].head->meal_name);
		sum += (table_array[TableNumber - 1].head->price) * (table_array[TableNumber - 1].head->quantity);//calculate the sum
	}
	free_memory(temp); //free the table linked list memory
	table_array[TableNumber - 1].head = NULL; // reinitalize the head to NULL (for the final free memory)
	if (sum)//in case that the sum isn't 0
		printf("%g nis+%g nis for tips, please!\n", sum, sum * 0.15);
	else
		printf("The table number %d is not ordered yet\n", TableNumber);
}

void ManageInstructions(char* file_name, List* manager, List table_array[], int is_items_cancelled[]) {
	FILE* instructions_file = open_file(file_name, "r"); //open the instructions file
	char operation, food_name[MAX_LEN_NAME + 1]; //define new char and string
	unsigned int quantity; //define unsigned integer for the quantity
	int table_number; //define integer variable for the table_number
	if (fscanf(instructions_file, "%c", &operation) != EOF) { //in case that the file isn't empty
		CreateProducts(manager); //creating the kitchen
		while (fscanf(instructions_file, "%c", &operation) != EOF) { //while there is still instructions in the file
			switch (operation) { // switch operator for all the cases
			case '2': // in case of add items
				fscanf(instructions_file, "%s %d", food_name, &quantity); //get the file input  
				AddItems(manager->head, food_name, quantity); //call to AddItems function
				break;
			case '3': // in case of order an item
				fscanf(instructions_file, "%d %s %d", &table_number, food_name, &quantity); //get the file input
				OrderItem(table_number, food_name, quantity, manager, table_array); // call to OrderItem function
				break;
			case '4': // in case of remove an item
				fscanf(instructions_file, "%d", &table_number); //get the file input
				RemoveItem(table_number, table_array, is_items_cancelled); //call to RemoveItem function
				break;
			case '5':
				fscanf(instructions_file, "%d", &table_number); //get the file input
				RemoveTable(table_number, table_array); // call to RemoveTable function
				break;
			}
		}
	}
}