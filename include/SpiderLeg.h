#ifndef __SPIDERLEG_H__
#define __SPIDERLEG_H__

#include "entity.h"
#include "SpiderBase.h"


/**
 * @brief spawn a Spider Leg
 * @return NULL on error, or a pointer to the player otherwise
 */
Entity* leg_new(Entity* base, Vector2D offsets, int dir);

#endif