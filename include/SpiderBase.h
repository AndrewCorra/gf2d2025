#ifndef __SPIDERBASE_H__
#define __SPIDERBASE_H__

#include "entity.h"


/**
 * @brief spawn a Spider
 * @return NULL on error, or a pointer to the player otherwise
 */
Entity* spider_new();

void spider_attack(Entity* target, Entity* projectile);

#endif