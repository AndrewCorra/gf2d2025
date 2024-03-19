#include "simple_json.h"
#include "simple_logger.h"

#include "gf2d_graphics.h"


#include "world.h"

World *world_test_new() {
	int i;
	int width = 65, height = 45;
	World *world;
	world = world_new(width, height);

	if (!world)return NULL;
	world->background = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
	world->tileset = gf2d_sprite_load_all(
		"images/backgrounds/Backgroundsample.png",
		16,
		16,
		1,
		1);
	for (i = 0; i < width; i++) {
		world->tilemap[i] = 1;
		world->tilemap[i + ((height - 1) * width)] = 1;
	}
	for (i = 0; i < height; i++) {
		world->tilemap[i*width] = 1;
		world->tilemap[i*width + (width - 1)] = 1;
	}
	return world;
}
World *world_new(Uint32 width, Uint32 height) {
	World *world;

	if ((!width) || (!height)) {
		slog("can't make a world with no height or width");
		return NULL;
	}
	world = gfc_allocate_array(sizeof(World), 1);

	if (!world) {
		slog("failed to allocate a new world");
		return NULL;
	}
	world->tilemap = gfc_allocate_array(sizeof(Uint8), height * width);
	world->tileWidth = width;
	world->tileHeight = height;
	return world;
}

void world_free(World* world) {
	if (!world)return;
	gf2d_sprite_free(world->background);
	gf2d_sprite_free(world->tileset);
	free(world->tilemap);
	free(world);
}

void world_draw(World* world) {
	int i, j;
	Uint32 frame = 1;
	int index;
	Vector2D position;
	if (!world)return;
	gf2d_sprite_draw_image(world->background, vector2d(0, 0));
	if (!world->tileset)return;
	for (j = 0; j < world->tileHeight; j++) {
		for (i = 0; i < world->tileWidth; i++) {
			index = i + (j * world->tileWidth);
			position.x = i * world->tileset->frame_w;
			position.y = i * world->tileset->frame_h;
			if (world->tilemap[index] == 0) continue;
			gf2d_sprite_draw(
				world->tileset,
				position,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				frame);
		}
	}
}