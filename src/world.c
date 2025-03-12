#include "simple_json.h"
#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_draw.h"


#include "world.h"
#include "camera.h"

void world_tile_layer_build(World* world)
{
	int i, j;
	Vector2D position;
	Uint32 frame;
	Uint32 index;
	if (!world)return;

	if (!world->tileset)return;

	if (world->tileLayer)
	{
		gf2d_sprite_free(world->tileLayer);
	}

	world->tileLayer = gf2d_sprite_new();

	world->tileLayer->surface = gf2d_graphics_create_surface(
		world->tileWidth * world->tileset->frame_w,
		world->tileHeight * world->tileset->frame_h);

	world->tileLayer->frame_w = world->tileWidth * world->tileset->frame_w;
	world->tileLayer->frame_h = world->tileHeight * world->tileset->frame_h;

	if (!world->tileLayer->surface)
	{
		slog("failed to create tileLayer surface");
		return;
	}

	for (j = 0; j < world->tileHeight; j++)
	{
		for (i = 0; i < world->tileWidth; i++)
		{
			index = i + (j * world->tileWidth);
			if (world->tilemap[index] == 0)continue;

			position.x = i * world->tileset->frame_w;
			position.y = j * world->tileset->frame_h;
			frame = world->tilemap[index] - 1;

			gf2d_sprite_draw_to_surface(
				world->tileset,
				position,
				NULL,
				NULL,
				frame,
				world->tileLayer->surface);
		}
	}
	world->tileLayer->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), world->tileLayer->surface);
	if (!world->tileLayer->texture)
	{
		slog("failed to convert world tile layer to texture");
		return;
	}
}

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
	gf2d_space_free(world->space);
	free(world->tilemap);
	free(world);
}

void world_draw(World* world) {
	Vector2D camera;
	int i, j;
	Uint32 frame = 1;
	int index;
	Vector2D position;
	camera = camera_get_offset();
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

void world_setup_camera(World* world)
{
	if (!world)return;
	if ((!world->tileLayer) || (!world->tileLayer->surface))
	{
		slog("no tile layer set for world");
		return;
	}
	camera_set_bounds(gfc_rect(0, 0, world->tileLayer->surface->w, world->tileLayer->surface->h));
	camera_apply_bounds();
	camera_enable_binding(1);
}

Space* world_get_space() {
	return gameWorld.space;
}

int worlds_bounds_test(World* world, Shape shape) {
	Rect boundingRect;
	if ((!world) || (!world->tileset)) return 0;
	boundingRect = gfc_shape_get_bounds(shape);
}
void world_draw_bounds(World* world) {

	Rect rect = { 0,0,64,64 };
	Shape test;
	Uint8 tileIndex;
	int i, j;
	if (!world)return;
	for (j = 0; j < world->tileHeight; j++) {
		for (i = 0; i < world->tileWidth; i++) {
			tileIndex = world->tilemap[i + j * world->tileWidth];
			if (tileIndex == 0)continue;
			rect.x = i * 64;
			rect.y = j * 64;
			gf2d_draw_rect(rect, gfc_color(255, 0, 0, 255));
		}
	}
}