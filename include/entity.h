#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_types.h"
#include "gfc_shape.h"
#include "gf2d_sprite.h"
#include "gf2d_body.h"

typedef enum {
	ETT_none,
	ETT_player,
	ETT_monster,
	ETT_pickup,
	ETT_MAX
}EntityTeam;

typedef enum {
	ECL_none = 1,
	ECL_player = 2,
	ECL_monster = 3,
	ECL_pickup = 4,
	ECL_playerAttack = 5,
	ECL_monsterAttack = 6,
	ECL_ALL = 7
}EntityCollisionLayer;

typedef struct Entity_S
{
	Uint8 _inuse;
	Sprite *sprite;
	float frame; //current frame of animation
	Vector2D position;
	Vector2D velocity;
	Body body;
	Shape shape;
	Vector4D bounds;
	EntityCollisionLayer collisionLayer;
	EntityTeam team;
	void (*think)(struct Entity_S *self);
	void (*update)(struct Entity_S *self);
	void (*free)(struct Entity_S *self);
	void* data; /*ad hoc data per instance*/
}Entity;

typedef struct {
	Entity* entity_list;
	Uint32 entity_max;
}EntityManager;

void entity_system_close();


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

void entity_update_position(Entity* self);

int entity_layer_check(Entity* self, EntityCollisionLayer layer);

int entity_wall_check(Entity* self, Vector2D dir);

void entity_hitbox_draw(Entity* self);

#endif
