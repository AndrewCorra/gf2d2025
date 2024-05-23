#include "simple_logger.h"

#include "SpiderBase.h"
#include "SpiderLeg.h"


typedef struct {
	TextLine classname;


}SpiderData;

void spider_think(Entity* self);
void spider_update(Entity* self);
void spider_free(Entity* self);

Entity* spider_new() {
	Entity* self;
	SpiderData* data;
    Entity* spiderLeg1;
    Entity* spiderLeg2;
    Entity* spiderLeg3;
    Entity* spiderLeg4;
    Entity* spiderLeg5;
    Entity* spiderLeg6;
    self = entity_new();
    Vector2D bright, bleft, midright, midleft, tright, tleft = self->position;
    bright.x = 440;
    bright.y = 380;
    bleft.x = 9;
    bleft.y = 380;
    midright.x = 480;
    midright.y = 280;
    midleft.x = -31;
    midleft.y = 280;
    tright.x = 440;
    tright.y = 200;
    tleft.x = 9;
    tleft.y = 200;


    if (!self)
    {
        slog("failed to spawn a spider entity");
        return NULL;
    }
    self->frame = 0;
    self->position = vector2d(500, -50);
    data = gfc_allocate_array(sizeof(SpiderData), 1);

    spiderLeg1 = leg_new(self, bright, 0);
    spiderLeg2 = leg_new(self, bleft, 1);
    spiderLeg3 = leg_new(self, midright, 0);
    spiderLeg4 = leg_new(self, midleft, 1);
    spiderLeg5 = leg_new(self, tright, 0);
    spiderLeg6 = leg_new(self, tleft, 1); 
    self->sprite = gf2d_sprite_load_all(
        "images/SpiderBase.png",
        567,
        567,
        16,
        1);
    if (data)
    {
        self->data = data;
    }
   
    return self;

}	

void spider_think(Entity* self) {

}
void spider_update(Entity* self) {

}

void spider_attack(Entity* target, Entity* projectile) {

}
void spider_free(Entity* self) {
    SpiderData* data;
    if ((!self) || (!self->data))return;
    data = (SpiderData*)self->data;
    free(data);
}
