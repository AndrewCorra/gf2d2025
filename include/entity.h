#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_types.h"
#include "gf2d_sprite.h"
#include "gf2d_body.h"
#include "gfc_shape.h"

typedef struct Entity_S
{
	Uint8 _inuse;
	Sprite *sprite;
	float frame; //current frame of animation
	Vector2D position;
	Vector2D velocity;
	Body body;
	Shape shape;
	void (*think)(struct Entity_S *self);
	void (*update)(struct Entity_S *self);
	void (*free)(struct Entity_S *self);
	void* data;
}Entity;

/**
* @brief initializes the system
* @param max the maximum number of entities that can exist at any one time
*/
void entity_system_initialize(Uint32 max);
/**
* @brief clears all active entities
* @param ignore do not clean up this entity
*/
void entity_clear_all(Entity *ignore);

Entity *entity_new();

void entity_free(Entity *self);

void entity_system_think();

void entity_system_update();

void entity_system_draw();

int entity_wall_check(Entity* self, Vector2D dir);

#endif
