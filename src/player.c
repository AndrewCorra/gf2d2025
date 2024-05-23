#include "simple_logger.h"

#include "camera.h"
#include "player.h"
#include "gf2d_body.h"
#include "gf2d_collision.h"

typedef struct
{
    TextLine classname;
}PlayerData;


void player_think(Entity* self);
void player_update(Entity* self);
void player_free(Entity* self);

Entity* player_new()
{
    Entity* self;
    PlayerData* data;

    self = entity_new();
    if (!self)
    {
        slog("failed to spawn a player entity");
        return NULL;
    }
    self->sprite = gf2d_sprite_load_all(
        "images/protago.png",
        64,
        64,
        4,
        0);
    self->frame = 0;
    self->position = vector2d(0, 0);

    self->think = player_think;
    self->update = player_update;
    self->free = player_free;

    gf2d_body_set(
        &self->body,
        "player",
        1,
        WORLD_LAYER,
        0,
        1,
        self->position,
        vector2d(0, 0),
        10,
        1,
        0,
        &self->shape,
        self,
        NULL);

    data = gfc_allocate_array(sizeof(PlayerData), 1);
    if (data)
    {
        self->data = data;
    }
    return self;
}

void player_think(Entity* self)
{
    const Uint8* keys;
    Vector2D screen;
    Vector2D dir = { 0 };
    Sint32 mx = 0, my = 0;
    if (!self)return;
    screen = camera_get_position();
    SDL_GetMouseState(&mx, &my);
    mx += screen.x;
    my += screen.y;
    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
    if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
        //if (!entity_wall_check(self, vector2d(-1.5, 0))) {
            dir.x = 1;
       // }
    }
    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
        //if (!entity_wall_check(self, vector2d(1.5, 0))) {
            dir.x = -1;
        //}
    }
    if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) {
       // if (!entity_wall_check(self, vector2d(0, -1.5))) {
            dir.y = -1;
       // }
    }
    if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) {
       // if (!entity_wall_check(self, vector2d(0, 1.5))) {
            dir.y = 1;
       // }
    }
        // if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_Q])slog(self->position.x, self->position.y);


    

    /*
    if (self->position.x < mx)dir.x = 1;
    if (self->position.y < my)dir.y = 1;
    if (self->position.x > mx)dir.x = -1;
    if (self->position.y > my)dir.y = -1;
    */
    vector2d_normalize(&dir);
    vector2d_scale(self->velocity, dir, 3);
}

void player_update(Entity* self)
{
    if (!self)return;
    self->frame += 0.1;
    if (self->frame >= 16)self->frame = 0;
    vector2d_add(self->position, self->position, self->velocity);

    camera_center_on(self->position);
}

void player_free(Entity* self)
{
    PlayerData* data;
    if ((!self) || (!self->data))return;
    data = (PlayerData*)self->data;
    free(data);
}

/*eol@eof*/