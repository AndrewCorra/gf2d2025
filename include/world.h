#ifndef __WORLD_H__
#define __WORLD_H__

#include "gfc_types.h"
#include "gf2d_sprite.h"
#include "gf2d_space.h"

typedef struct
{
	Sprite *background;
	Sprite *tileset;
	Sprite* tileLayer;
	Uint8 *tilemap;
	Uint32 tileHeight;
	Uint32 tileWidth;
	Space* space;
}World;

static World gameWorld = { 0 };
/**
* @brief allocate a new empty world
* @return NULL on error, or a blank world
*/

World *world_new();

World* world_test_new();


/**
* @brief free a previously allocated world
* @param world the world to free
*/
void world_free(World *world);

void world_draw(World* world);

void world_setup_camera(World* world);

Space* world_get_space();
#endif