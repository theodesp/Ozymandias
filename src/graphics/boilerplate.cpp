#include "boilerplate.h"

#include "graphics/screen.h"
#include "platform/renderer.h"
#include "dev/debug.h"

#ifdef __vita__
#include <vita2d.h>
#endif

#ifdef __vita__
extern vita2d_texture *tex_buffer_ui;
extern vita2d_texture * tex_buffer_city;
#endif

static void set_translation(int x, int y) {
    if (x != 0 || y != 0)
        graphics_renderer()->set_viewport(x, y, screen_width() - x, screen_height() - y);
    else
        graphics_renderer()->reset_viewport();
}

void graphics_set_to_dialog(void) {
    set_translation(screen_dialog_offset_x(), screen_dialog_offset_y());
}
void graphics_in_dialog_with_size(int width, int height) {
    set_translation((screen_width() - width) / 2, (screen_height() - height) / 2);
}
void graphics_reset_dialog(void) {
    set_translation(0, 0);
}

void graphics_set_clip_rectangle(int x, int y, int width, int height) {
    graphics_renderer()->set_clip_rectangle(x, y, width, height);
}
void graphics_reset_clip_rectangle(void) {
    graphics_renderer()->reset_clip_rectangle();
}

void graphics_clear_screen(void) {
    graphics_renderer()->clear_screen();
}

void graphics_draw_line(int x_start, int x_end, int y_start, int y_end, color_t color) {
    graphics_renderer()->draw_line(x_start, x_end, y_start, y_end, color);
}
void graphics_draw_vertical_line(int x, int y1, int y2, color_t color) {
    graphics_renderer()->draw_line(x, x, y1, y2, color);
}
void graphics_draw_horizontal_line(int x1, int x2, int y, color_t color) {
    graphics_renderer()->draw_line(x1, x2, y, y, color);
}
void graphics_draw_rect(int x, int y, int width, int height, color_t color) {
    graphics_renderer()->draw_rect(x, y, width, height, color);
}
void graphics_draw_inset_rect(int x, int y, int width, int height) {
    int x_end = x + width - 1;
    int y_end = y + height - 1;
    graphics_renderer()->draw_line(x, x_end, y, y, COLOR_INSET_DARK);
    graphics_renderer()->draw_line(x_end, x_end, y, y_end, COLOR_INSET_LIGHT);
    graphics_renderer()->draw_line(x, x_end, y_end, y_end, COLOR_INSET_LIGHT);
    graphics_renderer()->draw_line(x, x, y, y_end, COLOR_INSET_DARK);
}

void graphics_fill_rect(int x, int y, int width, int height, color_t color) {
    graphics_renderer()->fill_rect(x, y, width, height, color);
}
void graphics_shade_rect(int x, int y, int width, int height, int darkness) {
    color_t alpha = (0x11 * darkness) << COLOR_BITSHIFT_ALPHA;
    graphics_renderer()->fill_rect(x, y, width, height, alpha);
}

//////////////

int graphics_save_to_texture(int image_id, int x, int y, int width, int height) {
    return graphics_renderer()->save_texture_from_screen(image_id, x, y, width, height);
}
void graphics_draw_from_texture(int image_id, int x, int y, int width, int height) {
    graphics_renderer()->draw_saved_texture_to_screen(image_id, x, y, width, height);
}

//static const int FOOTPRINT_X_START_PER_HEIGHT[] = {
//        28, 26, 24, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2, 0,
//        0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28
//};
//
//static const int FOOTPRINT_OFFSET_PER_HEIGHT[] = {
//        0, 2, 8, 18, 32, 50, 72, 98, 128, 162, 200, 242, 288, 338, 392, 450,
//        508, 562, 612, 658, 700, 738, 772, 802, 828, 850, 868, 882, 892, 898
//};

static int get_visible_footprint_pixels_per_row(int tiles, int width, int height, int row) {
    int base_height = tiles * FOOTPRINT_HEIGHT;
    int footprint_row = row - (height - base_height);
    if (footprint_row < 0)
        return 0;
    else if (footprint_row < tiles * FOOTPRINT_HALF_HEIGHT)
        return 2 + 4 * footprint_row;
    else {
        return 2 + 4 * (base_height - 1 - footprint_row);
    }
}

//static void draw_modded_footprint(int image_id, int x, int y, color_t color) {
//    const image *img = image_get(image_id);
//    const color_t *data = image_data(image_id);
//    if (!data)
//        return;
//    int tiles = (img->width + 2) / (FOOTPRINT_WIDTH + 2);
//    int y_top_offset = img->height - FOOTPRINT_HEIGHT * tiles;
//    y -= y_top_offset + FOOTPRINT_HALF_HEIGHT * tiles - FOOTPRINT_HALF_HEIGHT;
//    const clip_info *clip = graphics_get_clip_info(x, y + y_top_offset, img->width,
//                                                   img->height - y_top_offset);
//    if (!clip->is_visible)
//        return;
//    data += img->width * (clip->clipped_pixels_top + y_top_offset);
//    for (int _y = clip->clipped_pixels_top + y_top_offset; _y < img->height - clip->clipped_pixels_bottom; _y++) {
//        int visible_pixels_per_row = get_visible_footprint_pixels_per_row(tiles, img->width, img->height, _y);
//        int x_start = (img->width - visible_pixels_per_row) / 2;
//        int x_max = img->width - x_start;
//        if (x_start < clip->clipped_pixels_left)
//            x_start = clip->clipped_pixels_left;
//
//        if (x_max > img->width - clip->clipped_pixels_right)
//            x_max = img->width - clip->clipped_pixels_right;
//
//        if (x_start >= x_max) {
//            data += img->width;
//            continue;
//        }
//        color_t *dst = graphics_get_pixel(x + x_start, y + _y);
//        data += x_start;
//        if (color && color != COLOR_MASK_NONE) {
//            for (int _x = x_start; _x < x_max; _x++, dst++) {
//                color_t alpha = *data & COLOR_CHANNEL_ALPHA;
//                if (alpha == ALPHA_OPAQUE)
//                    *dst = *data & color;
//
//                data++;
//            }
//        } else {
//            for (int _x = x_start; _x < x_max; _x++, dst++) {
//                color_t alpha = *data & COLOR_CHANNEL_ALPHA;
//                if (alpha == ALPHA_OPAQUE)
//                    *dst = *data;
//
//                data++;
//            }
//        }
//        data += img->width - x_max;
//    }
//}
//static void draw_modded_top(int image_id, int x, int y, color_t color) {
//    const image *img = image_get(image_id);
//    const color_t *data = image_data(image_id);
//    if (!data)
//        return;
//    int tiles = (img->width + 2) / (FOOTPRINT_WIDTH + 2);
//    int y_top_offset = img->height - FOOTPRINT_HEIGHT * tiles;
//    y_top_offset += FOOTPRINT_HALF_HEIGHT * tiles - FOOTPRINT_HALF_HEIGHT;
//    y -= y_top_offset;
//    int height = img->height - FOOTPRINT_HALF_HEIGHT * tiles;
//    const clip_info *clip = graphics_get_clip_info(x, y, img->width, height);
//    if (!clip->is_visible)
//        return;
//    data += img->width * clip->clipped_pixels_top;
//    for (int _y = clip->clipped_pixels_top; _y < height - clip->clipped_pixels_bottom; _y++) {
//        int visible_pixels_per_row = get_visible_footprint_pixels_per_row(tiles, img->width, img->height, _y);
//        int half_width = img->width / 2;
//        int half_visible_pixels = visible_pixels_per_row / 2;
//        int x_start = clip->clipped_pixels_left;
//        if (x_start < half_width) {
//            color_t *dst = graphics_get_pixel(x + x_start, y + _y);
//            int x_max = half_width - half_visible_pixels;
//            if (x_start > x_max)
//                x_start = x_max;
//
//            data += x_start;
//            int half_image_only = 0;
//            if (img->width - clip->clipped_pixels_right < x_max) {
//                x_max = img->width - clip->clipped_pixels_right;
//                half_image_only = 1;
//            }
//            if (color && color != COLOR_MASK_NONE) {
//                for (int _x = x_start; _x < x_max; _x++, dst++) {
//                    color_t alpha = *data & COLOR_CHANNEL_ALPHA;
//                    if (alpha == ALPHA_OPAQUE)
//                        *dst = *data & color;
//                    else if (alpha != ALPHA_TRANSPARENT)
//                        *dst = COLOR_BLEND_ALPHA_TO_OPAQUE(*data, *dst, alpha >> COLOR_BITSHIFT_ALPHA) & color;
//
//                    data++;
//                }
//            } else {
//                for (int _x = x_start; _x < x_max; _x++, dst++) {
//                    color_t alpha = *data & COLOR_CHANNEL_ALPHA;
//                    if (alpha == ALPHA_OPAQUE)
//                        *dst = *data;
//                    else if (alpha != ALPHA_TRANSPARENT)
//                        *dst = COLOR_BLEND_ALPHA_TO_OPAQUE(*data, *dst, alpha >> COLOR_BITSHIFT_ALPHA);
//
//                    data++;
//                }
//            }
//            if (half_image_only) {
//                data += clip->clipped_pixels_right;
//                continue;
//            }
//            data += half_width + half_visible_pixels - x_max;
//            x_start = half_width + half_visible_pixels;
//        } else {
//            x_start = half_width + half_visible_pixels;
//            if (x_start < clip->clipped_pixels_left)
//                x_start = clip->clipped_pixels_left;
//
//            data += x_start;
//        }
//        int x_max = img->width - clip->clipped_pixels_right;
//        color_t *dst = graphics_get_pixel(x + x_start, y + _y);
//        if (color && color != COLOR_MASK_NONE) {
//            for (int _x = x_start; _x < x_max; _x++, dst++) {
//                color_t alpha = *data & COLOR_CHANNEL_ALPHA;
//                if (alpha == ALPHA_OPAQUE)
//                    *dst = *data & color;
//                else if (alpha != ALPHA_TRANSPARENT)
//                    *dst = COLOR_BLEND_ALPHA_TO_OPAQUE(*data, *dst, alpha >> COLOR_BITSHIFT_ALPHA) & color;
//
//                data++;
//            }
//        } else {
//            for (int _x = x_start; _x < x_max; _x++, dst++) {
//                color_t alpha = *data & COLOR_CHANNEL_ALPHA;
//                if (alpha == ALPHA_OPAQUE)
//                    *dst = *data;
//                else if (alpha != ALPHA_TRANSPARENT)
//                    *dst = COLOR_BLEND_ALPHA_TO_OPAQUE(*data, *dst, alpha >> COLOR_BITSHIFT_ALPHA);
//
//                data++;
//            }
//        }
//        if (x_start > x_max)
//            data -= x_start - x_max;
//
//        data += clip->clipped_pixels_right;
//    }
//}
//static void draw_modded_image(const image *img, const color_t *data, int x, int y, color_t color) {
//    const clip_info *clip = graphics_get_clip_info(x, y, img->width, img->height);
//    if (!clip->is_visible)
//        return;
//    data += img->width * clip->clipped_pixels_top;
//    for (int _y = clip->clipped_pixels_top; _y < img->height - clip->clipped_pixels_bottom; _y++) {
//        data += clip->clipped_pixels_left;
//        color_t *dst = graphics_get_pixel(x + clip->clipped_pixels_left, y + _y);
//        int x_max = img->width - clip->clipped_pixels_right;
//        if (color && color != COLOR_MASK_NONE) {
//            for (int _x = clip->clipped_pixels_left; _x < x_max; _x++, dst++) {
//                color_t alpha = *data & COLOR_CHANNEL_ALPHA;
//                if (alpha == ALPHA_OPAQUE)
//                    *dst = *data & color;
//                else if (alpha != ALPHA_TRANSPARENT)
//                    *dst = COLOR_BLEND_ALPHA_TO_OPAQUE(*data, *dst, alpha >> COLOR_BITSHIFT_ALPHA) & color;
//                data++;
//            }
//        } else {
//            for (int _x = clip->clipped_pixels_left; _x < x_max; _x++, dst++) {
//                color_t alpha = *data & COLOR_CHANNEL_ALPHA;
//                if (alpha == ALPHA_OPAQUE)
//                    *dst = *data;
//                else if (alpha != ALPHA_TRANSPARENT)
//                    *dst = COLOR_BLEND_ALPHA_TO_OPAQUE(*data, *dst, alpha >> COLOR_BITSHIFT_ALPHA);
//                data++;
//            }
//        }
//        data += clip->clipped_pixels_right;
//    }
//}
//
//static void draw_uncompressed(const image *img, const color_t *data, int x, int y, color_t color, draw_type type) {
////    if (image_is_external(image_id)) {
////        image_load_external_data(image_id);
////    } else if ((img->atlas.id >> IMAGE_ATLAS_BIT_OFFSET) == ATLAS_UNPACKED_EXTRA_ASSET) {
////        assets_load_unpacked_asset(image_id);
////    }
//    graphics_renderer()->draw_image(img, x, y, color, 1.0f);
//}
//static void draw_compressed(const image *img, const color_t *data, int x, int y, int height) {
////    bool mirr = (img->offset_mirror != 0);
////    const clip_info *clip = graphics_get_clip_info(x_offset, y_offset, img->width, height, mirr);
////    if (!clip->is_visible)
////        return;
////    int unclipped = clip->clip_x == CLIP_NONE;
////
////    for (int y = 0; y < height - clip->clipped_pixels_bottom; y++) {
////        int x = 0;
////        while (x < img->width) {
////            color_t b = *data;
////            data++;
////            if (b == 255) {
////                // transparent pixels to skip
////                x += *data;
////                data++;
////            } else if (y < clip->clipped_pixels_top) {
////                data += b;
////                x += b;
////            } else {
////                // number of concrete pixels
////                const color_t *pixels = data;
////                data += b;
////                color_t *dst;
////                if (mirr)
////                    dst = graphics_get_pixel(x_offset + img->width - x - b, y_offset + y);
////                else
////                    dst = graphics_get_pixel(x_offset + x, y_offset + y);
////                if (unclipped) {
////                    x += b;
////                    if (mirr)
////                        for (int px = 0; px < b; px++) {
////                            int pcorr = b - px - 1;
////                            memcpy(dst + px, pixels + pcorr, sizeof(color_t));
////                        }
////                    else
////                        memcpy(dst, pixels, b * sizeof(color_t));
////                } else {
////                    if (mirr)
////                        int a = 3565;
////
////                    while (b) {
////                        if (x >= clip->clipped_pixels_left && x < img->width - clip->clipped_pixels_right)
////                            *dst = *pixels;
////
////                        dst++;
////                        x++;
////                        pixels++;
////                        b--;
////                    }
////                }
////            }
////        }
////    }
////    graphics_renderer()->draw_isometric_top(img, x, y, color, 1.0f);
//}
//static void draw_compressed_set(const image *img, const color_t *data, int x_offset, int y_offset, int height, color_t color) {
////    const clip_info *clip = graphics_get_clip_info(x_offset, y_offset, img->width, height);
////    if (!clip->is_visible)
////        return;
////    int unclipped = clip->clip_x == CLIP_NONE;
////
////    for (int y = 0; y < height - clip->clipped_pixels_bottom; y++) {
////        int x = 0;
////        while (x < img->width) {
////            color_t b = *data;
////            data++;
////            if (b == 255) {
////                // transparent pixels to skip
////                x += *data;
////                data++;
////            } else if (y < clip->clipped_pixels_top) {
////                data += b;
////                x += b;
////            } else {
////                data += b;
////                color_t *dst = graphics_get_pixel(x_offset + x, y_offset + y);
////                if (unclipped) {
////                    x += b;
////                    while (b) {
////                        *dst = color;
////                        dst++;
////                        b--;
////                    }
////                } else {
////                    while (b) {
////                        if (x >= clip->clipped_pixels_left && x < img->width - clip->clipped_pixels_right)
////                            *dst = color;
////
////                        dst++;
////                        x++;
////                        b--;
////                    }
////                }
////            }
////        }
////    }
//}
//static void draw_compressed_and(const image *img, const color_t *data, int x_offset, int y_offset, int height, color_t color) {
//    const clip_info *clip = graphics_get_clip_info(x_offset, y_offset, img->width, height);
//    if (!clip->is_visible)
//        return;
//    int unclipped = clip->clip_x == CLIP_NONE;
//
//    for (int y = 0; y < height - clip->clipped_pixels_bottom; y++) {
//        int x = 0;
//        while (x < img->width) {
//            color_t b = *data;
//            data++;
//            if (b == 255) {
//                // transparent pixels to skip
//                x += *data;
//                data++;
//            } else if (y < clip->clipped_pixels_top) {
//                data += b;
//                x += b;
//            } else {
//                // number of concrete pixels
//                const color_t *pixels = data;
//                data += b;
//                color_t *dst = graphics_get_pixel(x_offset + x, y_offset + y);
//                if (unclipped) {
//                    x += b;
//                    while (b) {
//                        *dst = *pixels & color;
//                        dst++;
//                        pixels++;
//                        b--;
//                    }
//                } else {
//                    while (b) {
//                        if (x >= clip->clipped_pixels_left && x < img->width - clip->clipped_pixels_right)
//                            *dst = *pixels & color;
//
//                        dst++;
//                        x++;
//                        pixels++;
//                        b--;
//                    }
//                }
//            }
//        }
//    }
//}
//static void draw_compressed_blend(const image *img, const color_t *data, int x_offset, int y_offset, int height, color_t color) {
//    const clip_info *clip = graphics_get_clip_info(x_offset, y_offset, img->width, height);
//    if (!clip->is_visible)
//        return;
//    int unclipped = clip->clip_x == CLIP_NONE;
//
//    for (int y = 0; y < height - clip->clipped_pixels_bottom; y++) {
//        int x = 0;
//        while (x < img->width) {
//            color_t b = *data;
//            data++;
//            if (b == 255) {
//                // transparent pixels to skip
//                x += *data;
//                data++;
//            } else if (y < clip->clipped_pixels_top) {
//                data += b;
//                x += b;
//            } else {
//                data += b;
//                color_t *dst = graphics_get_pixel(x_offset + x, y_offset + y);
//                if (unclipped) {
//                    x += b;
//                    while (b) {
//                        *dst &= color;
//                        dst++;
//                        b--;
//                    }
//                } else {
//                    while (b) {
//                        if (x >= clip->clipped_pixels_left && x < img->width - clip->clipped_pixels_right)
//                            *dst &= color;
//
//                        dst++;
//                        x++;
//                        b--;
//                    }
//                }
//            }
//        }
//    }
//}
//static void draw_compressed_blend_alpha(const image *img, const color_t *data, int x_offset, int y_offset, int height, color_t color) {
//    const clip_info *clip = graphics_get_clip_info(x_offset, y_offset, img->width, height);
//    if (!clip->is_visible)
//        return;
//    color_t alpha = COLOR_COMPONENT(color, COLOR_BITSHIFT_ALPHA);
//    if (!alpha)
//        return;
//    if (alpha == 255) {
//        draw_compressed_set(img, data, x_offset, y_offset, height, color);
//        return;
//    }
//    color_t alpha_dst = 256 - alpha;
//    color_t src_rb = (color & 0xff00ff) * alpha;
//    color_t src_g = (color & 0x00ff00) * alpha;
//    int unclipped = clip->clip_x == CLIP_NONE;
//
//    for (int y = 0; y < height - clip->clipped_pixels_bottom; y++) {
//        int x = 0;
//        color_t *dst = graphics_get_pixel(x_offset, y_offset + y);
//        while (x < img->width) {
//            color_t b = *data;
//            data++;
//            if (b == 255) {
//                // transparent pixels to skip
//                x += *data;
//                dst += *data;
//                data++;
//            } else if (y < clip->clipped_pixels_top) {
//                data += b;
//                x += b;
//                dst += b;
//            } else {
//                data += b;
//                if (unclipped) {
//                    x += b;
//                    while (b) {
//                        color_t d = *dst;
//                        *dst = (((src_rb + (d & 0xff00ff) * alpha_dst) & 0xff00ff00) |
//                                ((src_g + (d & 0x00ff00) * alpha_dst) & 0x00ff0000)) >> 8;
//                        b--;
//                        dst++;
//                    }
//                } else {
//                    while (b) {
//                        if (x >= clip->clipped_pixels_left && x < img->width - clip->clipped_pixels_right) {
//                            color_t d = *dst;
//                            *dst = (((src_rb + (d & 0xff00ff) * alpha_dst) & 0xff00ff00) |
//                                    ((src_g + (d & 0x00ff00) * alpha_dst) & 0x00ff0000)) >> 8;
//                        }
//                        dst++;
//                        x++;
//                        b--;
//                    }
//                }
//            }
//        }
//    }
//}
//static void draw_footprint_simple(const color_t *src, int x, int y) {
//    memcpy(graphics_get_pixel(x + 28, y + 0), &src[0], 2 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 26, y + 1), &src[2], 6 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 24, y + 2), &src[8], 10 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 22, y + 3), &src[18], 14 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 20, y + 4), &src[32], 18 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 18, y + 5), &src[50], 22 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 16, y + 6), &src[72], 26 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 14, y + 7), &src[98], 30 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 12, y + 8), &src[128], 34 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 10, y + 9), &src[162], 38 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 8, y + 10), &src[200], 42 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 6, y + 11), &src[242], 46 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 4, y + 12), &src[288], 50 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 2, y + 13), &src[338], 54 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 0, y + 14), &src[392], 58 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 0, y + 15), &src[450], 58 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 2, y + 16), &src[508], 54 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 4, y + 17), &src[562], 50 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 6, y + 18), &src[612], 46 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 8, y + 19), &src[658], 42 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 10, y + 20), &src[700], 38 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 12, y + 21), &src[738], 34 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 14, y + 22), &src[772], 30 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 16, y + 23), &src[802], 26 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 18, y + 24), &src[828], 22 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 20, y + 25), &src[850], 18 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 22, y + 26), &src[868], 14 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 24, y + 27), &src[882], 10 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 26, y + 28), &src[892], 6 * sizeof(color_t));
//    memcpy(graphics_get_pixel(x + 28, y + 29), &src[898], 2 * sizeof(color_t));
//}
//static void draw_footprint_tile(const color_t *data, int x_offset, int y_offset, color_t color_mask) {
//    if (!color_mask)
//        color_mask = COLOR_MASK_NONE;
//
//    const clip_info *clip = graphics_get_clip_info(x_offset, y_offset, FOOTPRINT_WIDTH, FOOTPRINT_HEIGHT);
//    if (data == nullptr || !clip->is_visible)
//        return;
//    // If the current tile neither clipped nor color masked, just draw it normally
//    if (clip->clip_y == CLIP_NONE && clip->clip_x == CLIP_NONE && color_mask == COLOR_MASK_NONE) {
//        draw_footprint_simple(data, x_offset, y_offset);
//        return;
//    }
//    int clip_left = clip->clip_x == CLIP_LEFT || clip->clip_x == CLIP_BOTH;
//    int clip_right = clip->clip_x == CLIP_RIGHT || clip->clip_x == CLIP_BOTH;
//    const color_t *src = &data[FOOTPRINT_OFFSET_PER_HEIGHT[clip->clipped_pixels_top]];
//    for (int y = clip->clipped_pixels_top; y < clip->clipped_pixels_top + clip->visible_pixels_y; y++) {
//        int x_start = FOOTPRINT_X_START_PER_HEIGHT[y];
//        int x_max = 58 - x_start * 2;
//        int x_pixel_advance = 0;
//        if (clip_left) {
//            if (clip->clipped_pixels_left + clip->visible_pixels_x < x_start) {
//                src += x_max;
//                continue;
//            }
//            if (clip->clipped_pixels_left > x_start) {
//                int pixels_to_reduce = clip->clipped_pixels_left - x_start;
//                if (pixels_to_reduce >= x_max) {
//                    src += x_max;
//                    continue;
//                }
//                src += pixels_to_reduce;
//                x_max -= pixels_to_reduce;
//                x_start = clip->clipped_pixels_left;
//            }
//        }
//        if (clip_right) {
//            int clip_x = 58 - clip->clipped_pixels_right;
//            if (clip_x < x_start) {
//                src += x_max;
//                continue;
//            }
//            if (x_start + x_max > clip_x) {
//                int temp_x_max = clip_x - x_start;
//                x_pixel_advance = x_max - temp_x_max;
//                x_max = temp_x_max;
//            }
//        }
//        color_t *buffer = graphics_get_pixel(x_offset + x_start, y_offset + y);
//        if (color_mask == COLOR_MASK_NONE) {
//            memcpy(buffer, src, x_max * sizeof(color_t));
//            src += x_max + x_pixel_advance;
//        } else {
//            for (int x = 0; x < x_max; x++, buffer++, src++) {
//                *buffer = *src & color_mask;
//            }
//            src += x_pixel_advance;
//        }
//    }
//}

static const color_t *tile_data(const color_t *data, int index) {
    return &data[900 * index];
}
static void draw_footprint_size_any(int image_id, int x, int y, int size, color_t color_mask, float scale) {
//    const color_t *data = image_data(image_id);
    const image_t *img = image_get(image_id);

    // The offsets alternate very annoyingly.
    // The y offsets grow by 15 each "batch"
    // while for each "batch" there are multiple
    // x offsets, 60p apart from each other,
    // symmetric around the x axis.
    // What an absolute mess!
    int index = 0;
    for (int k = 0; k < (size * 2) - 1; k++) {

        int k_limit = k;
        if (k >= size - 1)
            k_limit = 2 * size - 2 - k;

        for (int j = -30 * k_limit; j <= 30 * k_limit; j += 60) {

            int x_offset = j;
            int y_offset = k * 15;

//            draw_footprint_tile(tile_data(data, index++), x + x_offset, y + y_offset, color_mask);
            graphics_renderer()->draw_image(img, x, y, color_mask, scale, false);
        }
    }
}
static color_t base_color_for_font(font_t font) {
    if (font == FONT_SMALL_PLAIN || font == FONT_SMALL_OUTLINED || font == FONT_SMALL_SHADED)
        return COLOR_FONT_PLAIN;
    return COLOR_MASK_NONE;
}
static void draw_multibyte_letter(font_t font, const image_t *img, int x, int y, color_t color_mask, float scale) {
//    switch (font) {
//        case FONT_NORMAL_WHITE_ON_DARK:
////            graphics_renderer()->draw_image(img, x + 1, y + 1, 0xff311c10, scale, false);
//            graphics_renderer()->draw_image(img, x, y, COLOR_WHITE, scale, false);
//            break;
//        case FONT_NORMAL_YELLOW:
////            graphics_renderer()->draw_image(img, x + 1, y + 1, 0xffe7cfad, scale, false);
//            graphics_renderer()->draw_image(img, x, y, 0xff731408, scale, false);
//            break;
//        case FONT_NORMAL_BLACK_ON_DARK:
////            graphics_renderer()->draw_image(img, x + 1, y + 1, 0xffe7cfad, scale, false);
//            graphics_renderer()->draw_image(img, x, y, 0xff311c10, scale, false);
//            break;
//        case FONT_SMALL_PLAIN:
//            if (!color_mask)
//                color_mask = base_color_for_font(font);
//            graphics_renderer()->draw_image(img, x, y, ALPHA_OPAQUE | color_mask, scale, false);
//            break;
//        case FONT_NORMAL_BLACK_ON_LIGHT:
//        case FONT_LARGE_BLACK_ON_LIGHT:
////            graphics_renderer()->draw_image(img, x + 1, y + 1, 0xffcead9c, scale, false);
//            graphics_renderer()->draw_image(img, x, y, COLOR_BLACK, scale, false);
//            break;
//        case FONT_SMALL_SHADED:
//            if (!color_mask)
//                color_mask = base_color_for_font(font);
////            graphics_renderer()->draw_image(img, x + 1, y + 1, ALPHA_OPAQUE | COLOR_TOOLTIP_SHADOW, scale, false);
//            graphics_renderer()->draw_image(img, x, y, ALPHA_OPAQUE | color_mask, scale, false);
//        default:
//            graphics_renderer()->draw_image(img, x, y, ALPHA_OPAQUE | color_mask, scale, false);
//            break;
//    }
}

void ImageDraw::img_generic(int image_id, int x, int y, color_t color_mask, float scale) {
    const image_t *img = image_get(image_id);
    graphics_renderer()->draw_image(img, x, y, color_mask, scale, false);
}
void ImageDraw::img_sprite(int image_id, int x, int y, color_t color_mask, float scale) {
    const image_t *img = image_get(image_id);
//    debug_draw_sprite_box(x, y, img, zoom_get_scale());
    bool mirrored = (img->offset_mirror != 0);
    if (mirrored) {
        img = img->mirrored_img;
        x -= (img->width - img->animation.sprite_x_offset);
    } else
        x -= img->animation.sprite_x_offset;
    y -= img->animation.sprite_y_offset;
    graphics_renderer()->draw_image(img, x, y, color_mask, scale, mirrored);
}
void ImageDraw::img_ornament(int image_id, int base_id, int x, int y, color_t color_mask, float scale) {
    const image_t *img = image_get(image_id);
    const image_t *base = image_get(base_id);
    int tile_size = (base->width + 2) / (FOOTPRINT_WIDTH + 2);
    int ydiff = FOOTPRINT_HALF_HEIGHT * (tile_size + 1);
    x += base->animation.sprite_x_offset;
    y += base->animation.sprite_y_offset - base->height + ydiff;
    graphics_renderer()->draw_image(img, x, y, color_mask, scale, false);
}
void ImageDraw::img_from_below(int image_id, int x, int y, color_t color_mask, float scale) {
    const image_t *img = image_get(image_id);
    graphics_renderer()->draw_image(img, x, y - img->height, color_mask, scale, false);
}
void ImageDraw::img_enemy(int image_id, int x, int y, color_t color_mask, float scale) {
    if (image_id <= 0 || image_id >= 801)
        return;
    ImageDraw::img_sprite(image_id, x, y, color_mask, scale);

//    const image *img = image_get_enemy(image_id);
//    const color_t *data = image_data_enemy(image_id);
//    if (data)
//        draw_compressed(img, data, x, y, img->height);
//    graphics_renderer()->draw_image(img, x, y, 0, scale);
//    ImageDraw::img_generic(image_id, x, y, 0, scale);
}
void ImageDraw::img_blended(int image_id, int x, int y, color_t color_mask, float scale) {
    const image_t *img = image_get(image_id);
//    const color_t *data = image_data(image_id);
//    if (!data)
//        return;
//
//    if (img->type == IMAGE_TYPE_ISOMETRIC)
//        return;
//
//    if (img->is_fully_compressed)
//        draw_compressed_blend(img, data, x, y, img->height, color);
//    else {
//        draw_uncompressed(img, data, x, y, color, DRAW_TYPE_BLEND);
//    }
//    graphics_renderer()->draw_image(img, x, y, color, scale);
    ImageDraw::img_generic(image_id, x, y, color_mask, scale);
}
void ImageDraw::img_alpha_blended(int image_id, int x, int y, color_t color_mask, float scale) {
    const image_t *img = image_get(image_id);
//    const color_t *data = image_data(image_id);
//    if (!data)
//        return;
//
//    if (img->type == IMAGE_TYPE_ISOMETRIC)
//        return;
//
//    if (img->is_fully_compressed)
//        draw_compressed_blend_alpha(img, data, x, y, img->height, color);
//    else
//        draw_uncompressed(img, data, x, y, color, DRAW_TYPE_BLEND_ALPHA);
//    graphics_renderer()->draw_image(img, x, y, color, scale);
    ImageDraw::img_generic(image_id, x, y, color_mask, scale);
}
void ImageDraw::img_letter(font_t font, int letter_id, int x, int y, color_t color_mask, float scale) {
    const image_t *img = image_letter(letter_id);
    if (letter_id >= IMAGE_FONT_MULTIBYTE_OFFSET) {
//        draw_multibyte_letter(font, img, x, y, color_mask, scale);
        return;
    }
    if (!color_mask)
        color_mask = base_color_for_font(font);
    if (font == FONT_SMALL_SHADED)
        graphics_renderer()->draw_image(img, x + 1, y + 1, COLOR_BLACK, scale, false);
    graphics_renderer()->draw_image(img, x, y, color_mask, scale, false);
}
void ImageDraw::img_background(int image_id, float scale) {
//    graphics_set_to_dialog();
//    ImageDraw::img_generic(image_id, 0, 0, COLOR_MASK_NONE, scale);
//    graphics_reset_dialog();
    const image_t *img = image_get(image_id);
    if (scale == -1) {
//        graphics_renderer()->draw_image(img, 0, 0, COLOR_MASK_NONE, scale, false); // todo?
    } else {
        graphics_renderer()->draw_image(img,
                                        (screen_width() - img->width) / 2,
                                        (screen_height() - img->height) / 2,
                                        COLOR_MASK_NONE, scale, false);
    }
}
void ImageDraw::isometric(int image_id, int x, int y, color_t color_mask, float scale) {
    const image_t *img = image_get(image_id);
    ImageDraw::img_generic(image_id, x, y, color_mask, scale);
}
void ImageDraw::isometric_from_drawtile(int image_id, int x, int y, color_t color_mask) {
    const image_t *img = image_get(image_id);
//    if ((img->atlas.id >> IMAGE_ATLAS_BIT_OFFSET) == ATLAS_UNPACKED_EXTRA_ASSET) {
//        assets_load_unpacked_asset(image_id);
//    }
    int tile_size = (img->width + 2) / (FOOTPRINT_WIDTH + 2);
    y += FOOTPRINT_HALF_HEIGHT * (tile_size + 1) - img->height;
    graphics_renderer()->draw_image(img, x, y, color_mask, 1.0f, false);
}