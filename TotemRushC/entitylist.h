#pragma once

#include <stdlib.h>
#include "entity.h"
#include "util.h"

typedef struct entity_list {
	Entity** entities;
	size_t size;
	size_t capacity;
} EntityList;

EntityList* EntityListInitialize(size_t initial_capacity);
void EntityListAddEntity(EntityList* list, Entity* entity);
void EntityListFree(EntityList* list);