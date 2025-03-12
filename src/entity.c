#include "simple_logger.h"
#include "entity.h"
#include "gf2d_collision.h"
#include "gf2d_draw.h"
#include "gfc_vector.h"
#include "world.h"
#include "camera.h"
#include "gf2d_graphics.h"


EntityManager _entity_manager = { 0 }; /**<initalize a LOCAL global entity manager*/


void entity_system_initialize(Uint32 max) {
	if (_entity_manager.entity_list) {
		slog("cannot have two instances of an entity manager, one is already active");
		return;
	}
	if (!max) {
		slog("cannot allocate 0 entities!");
		return;
	}
	_entity_manager.entity_list = gfc_allocate_array(sizeof(Entity), max);
	if (!_entity_manager.entity_list)
	{
		slog("failed to allocate global entity list");
		return;
	}
	_entity_manager.entity_max = max;
	slog("entity system initialized");
	atexit(entity_system_close);
}

void entity_system_close() {
	entity_clear_all(NULL);
	if (_entity_manager.entity_list) {
		free(_entity_manager.entity_list);
	}
	memset(&_entity_manager, 0, sizeof(EntityManager));
}

void entity_clear_all(Entity *ignore)
{
	int i;
	for (i = 0; i < _entity_manager.entity_max; i++) {
		if (&_entity_manager.entity_list[i] == ignore)continue;
		if (!_entity_manager.entity_list[i]._inuse)continue;//skip this iteration of the loop
		entity_free(&_entity_manager.entity_list[i]);
	}
}

Entity *entity_new() {
	int i;
	for (i = 0; i < _entity_manager.entity_max; i++) {
		if (_entity_manager.entity_list[i]._inuse)continue; //skip active entities
		memset(&_entity_manager.entity_list[i], 0, sizeof(Entity));
		_entity_manager.entity_list[i]._inuse = 1;
		return &_entity_manager.entity_list[i];
	}
	slog("no more avaliable entities");
	return NULL;
}

void entity_free(Entity *self) {
	if (!self)return; //can't do work with nothing
	gf2d_sprite_free(self->sprite);
	if (self->free)self->free(self->data);
} 

void entity_think(Entity* self) {
	if (!self)return;
	if (self->think)self->think(self);
}

void entity_system_think()
{
	int i;
	for (i = 0; i < _entity_manager.entity_max; i++) {
		if (!_entity_manager.entity_list[i]._inuse)continue; //skip inactive entities
		entity_think(&_entity_manager.entity_list[i]);
	}
}


void entity_update(Entity* self) {
	if (!self)return;
	if (self->update)self->update(self);
}

void entity_system_update()
{
	int i;
	for (i = 0; i < _entity_manager.entity_max; i++) {
		if (!_entity_manager.entity_list[i]._inuse)continue; //skip inactive entities
		entity_update(&_entity_manager.entity_list[i]);
	}
}

void entity_draw(Entity *self) {
	Vector2D camera, position;
	if (!self)return;
	camera = camera_get_offset();
	if (self->sprite) {
		gf2d_sprite_render(
			self->sprite,
			self->position,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			(Uint32)self->frame);
	}
}

void entity_system_draw() {
	int i;
	for (i = 0; i < _entity_manager.entity_max; i++) {
		if (!_entity_manager.entity_list[i]._inuse)continue; //skip inactive entities
		entity_draw(&_entity_manager.entity_list[i]);
	}
}

Collision entity_scan_hit(Entity* self, Vector2D start, Vector2D end, CollisionFilter filter)
{
	Collision c = { 0 };
	if (!self)return c;
	filter.ignore = &self->body;
	c = gf2d_collision_trace_space(world_get_space(), start, end, filter);
	gf2d_draw_shape(gfc_shape_edge(start.x, start.y, end.x, end.y), gfc_color(255, 255, 0, 255), camera_get_offset());
	return c;
}

void entity_update_position(Entity* self) {
	Vector2D screen;

	if (!self)return;
	vector2d_normalize(&self->velocity);
	// vector2d_scale(&self->velocity, &self->velocity, 10);
	screen = gf2d_graphics_get_resolution();
	vector2d_add(self->position, self->position, self->velocity);
	if (self->position.x + self->bounds.x < 0)self->position.x = 0 - self->bounds.x;
	if (self->position.y + self->bounds.y < 0)self->position.y = 0 - self->bounds.y;
	if (self->position.x + self->bounds.x > screen.x)self->position.x = screen.x - self->bounds.x;
	if (self->position.y + self->bounds.y > screen.y)self->position.y = screen.y - self->bounds.y;
	self->bounds.x = self->position.x;;
	self->bounds.y = self->position.y;
}

int entity_layer_check(Entity* self, EntityCollisionLayer layer) {

	if (!self)return 0;
	return self->collisionLayer & layer;
}

int entity_wall_check(Entity* self, Vector2D dir)
{
	Shape s;
	int i, count;
	Collision* c;
	List* collisionList;
	CollisionFilter filter = {
		1,
		WORLD_LAYER,
		0,
		0,
		&self->body
	};

	if (!self)return 0;
	s = gf2d_body_to_shape(&self->body);
	gfc_shape_move(&s, dir);
	collisionList = gf2d_collision_check_space_shape(world_get_space(), s, filter);
	if (collisionList != NULL)
	{
		count = gfc_list_get_count(collisionList);
		for (i = 0; i < count; i++)
		{
			c = (Collision*)gfc_list_get_nth(collisionList, i);
			if (!c)continue;
			if (!c->shape)continue;
			gf2d_draw_shape(*c->shape, gfc_color(255, 255, 0, 255), camera_get_offset());
		}
		gf2d_collision_list_free(collisionList);
		return 1;
	}
	return 0;
}


void entity_hitbox_draw(Entity* self) {
//	Collision c;
//	CollisionFilter filter = { 0 };
	Rect box = { self->position.x, self->position.y, self->bounds.z, self->bounds.w };
	if (!self) return 0;
	Color ye = gfc_color(255, 255, 252, 255);
	Color ne = gfc_color(0, 0, 252, 255);
//	c = gf2d_collision_trace_space(world_get_space(), self->position, self->position, filter);
	if (entity_wall_check(self, self->velocity)) {
		gf2d_draw_rect(box, ne);
	 }
	else {
		gf2d_draw_rect(box, ye);
	}
	return 0;
}