#include "simple_logger.h"

#include "SpiderLeg.h"

typedef struct {
	TextLine classname;
}LegData;

void leg_think(Entity* self);
void leg_update(Entity* self);
void leg_free(Entity* self);

Entity* leg_new(Entity* base, Vector2D offsets, int dir) {
	Entity* self;
	LegData* data;
    self = entity_new();
    if (!self)
    {
        slog("failed to spawn a spiderleg entity");
        return NULL;
    }
    if (dir == 0) {
        self->sprite = gf2d_sprite_load_all(
            "images/SpiderLeg.png",
            128,
            128,
            16,
            0);
    }
    if (dir == 1) {
        self->sprite = gf2d_sprite_load_all(
            "images/SpiderLegI.png",
            128,
            128,
            16,
            0);
    }
    self->frame = 0;
    self->position = vector2d(base->position.x + offsets.x, base->position.y + offsets.y);
    self->think = leg_think;
    self->update = leg_think;
    self->free = leg_think;


    data = gfc_allocate_array(sizeof(LegData), 1);
    if (data)
    {
        self->data = data;
    }
    return self;

}

void leg_think(Entity* self) {
    
}

void leg_update(Entity* self) {

}

void leg_free(Entity* self) {
    LegData* data;
    if ((!self) || (!self->data))return;
    data = (LegData*)self->data;
    free(data);
}