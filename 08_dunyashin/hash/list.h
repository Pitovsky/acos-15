#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node{
	int value;
	struct Node* next;
	char* key;
};

struct List{
	struct Node* first;
};

struct List* New_List(){
	struct List* new_list = (struct List*)malloc(sizeof(struct List));
	new_list->first = NULL;
	return new_list;
}

void AddToList(struct List* list, int value, char* key){
	struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
	new_node->next = list->first;
	new_node->value = value;
	new_node->key = key;
	list->first = new_node;
	return;
}

int GetFromList(struct List* list, int* result, char* key){
	struct Node* current_node = list->first;
	while(current_node){
		if (strcmp(current_node->key, key) == 0){
			*result = current_node->value;
			return 0;
		}	
		current_node = current_node->next;
	}
	return 1;
}

void DeleteFromList(struct List* list, char* key){
	struct Node* current_node = list->first;
	struct Node* prev_node = NULL;
	while(current_node){
		if (strcmp(current_node->key, key) == 0){
			if(prev_node){
				prev_node->next = current_node->next;
			}
			else{
				list->first = current_node->next;
			}
			return;
		}
		prev_node = current_node;
		current_node = prev_node->next;
	}
}


void Delete_First(struct List* list){
	struct Node* tmp = list->first;
	if (list->first == NULL){
		return;
	}	
	list->first = tmp->next;
	free(tmp);
	return;
}

void Delete_List(struct List* list){
	while(list->first){
		Delete_First(list);
	}
	free(list);
	return;
}
