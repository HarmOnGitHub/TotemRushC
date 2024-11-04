#include "entitylist.h"

EntityList* EntityListInitialize(size_t initial_capacity) {
	EntityList* e_list = (EntityList*)malloc(sizeof(EntityList));
	e_list->entities = (Entity**)malloc(sizeof(Entity*) * initial_capacity);
	e_list->size = 0;
	e_list->capacity = initial_capacity;
	return e_list;
}

void EntityListAddEntity(EntityList* list, Entity* entity) {
	if (list->size >= list->capacity) {
		list->capacity *= 2;
		list->entities = (Entity**)realloc(list->entities, sizeof(Entity*) * list->capacity);
	}
	list->entities[list->size++] = entity;
}

void EntityListFree(EntityList* list) {
	for (size_t i = 0; i < list->size; i++) {
		free(list->entities[i]);
	}
	free(list->entities);
	free(list);

	DEBUG_PRINT("Freed memory used by EntityList.\n");
}
