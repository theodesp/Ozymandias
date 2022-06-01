#ifndef OZYMANDIAS_DEBUG_H
#define OZYMANDIAS_DEBUG_H

#include "input/hotkey.h"
#include "graphics/color.h"
#include "graphics/view/view.h"
#include "graphics/image.h"

extern int debug_range_1;
extern int debug_range_2;
extern int debug_range_3;
extern int debug_range_4;

void handle_debug_hotkeys(const hotkeys *h);

void draw_debug_font_text();

void draw_debug_line(uint8_t* str, int x, int y, int indent, const char *text, int value, color_t color = COLOR_WHITE);
void draw_debug_line_float(uint8_t* str, int x, int y, int indent, const char *text, double value, color_t color = COLOR_WHITE);
void draw_debug_line_double_left(uint8_t* str, int x, int y, int indent, int indent2, const char *text, int value1, int value2, color_t color = COLOR_WHITE);

void debug_draw_crosshair(int x, int y);
void debug_draw_sprite_box(int x, int y, const image_t *img, float scale, color_t color_mask);
void debug_draw_tile_box(int x, int y, int tile_size_x = 1, int tile_size_y = 1);

void draw_debug_tile(pixel_coordinate pixel, map_point point);
void draw_debug_figures(pixel_coordinate pixel, map_point point);

void draw_debug_ui(int x, int y);

#endif //OZYMANDIAS_DEBUG_H
