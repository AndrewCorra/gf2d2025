#ifndef __ENERGYBALL_H__
#define __ENERGYBALL_H__

#include "entity.h"


/**
 * @brief spawn a ball
 * @return NULL on error, or a pointer to the player otherwise
 */
Entity* ball_new(Vector2D start, Vector2D target);

#endif