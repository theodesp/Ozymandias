#include "military.h"

#include "city/figures.h"
#include "city/military.h"
#include "graphics/view/view.h"
#include "figure/formation_legion.h"
#include "graphics/elements/generic_button.h"
#include "graphics/boilerplate.h"
#include "graphics/elements/lang_text.h"
#include "graphics/elements/panel.h"
#include "graphics/elements/scrollbar.h"
#include "graphics/text.h"
#include "graphics/window.h"
#include "grid/grid.h"
#include "scenario/invasion.h"
#include "window/city.h"

#define ADVISOR_HEIGHT 26

static void button_go_to_legion(int legion_id, int param2);
static void button_return_to_fort(int legion_id, int param2);
static void button_empire_service(int legion_id, int param2);
static void on_scroll(void);

static scrollbar_type scrollbar = {592, 70, 272, on_scroll};

static generic_button fort_buttons[] = {
        {384, 83,  30, 30, button_go_to_legion,   button_none, 1, 0},
        {464, 83,  30, 30, button_return_to_fort, button_none, 1, 0},
        {544, 83,  30, 30, button_empire_service, button_none, 1, 0},
        {384, 127, 30, 30, button_go_to_legion,   button_none, 2, 0},
        {464, 127, 30, 30, button_return_to_fort, button_none, 2, 0},
        {544, 127, 30, 30, button_empire_service, button_none, 2, 0},
        {384, 171, 30, 30, button_go_to_legion,   button_none, 3, 0},
        {464, 171, 30, 30, button_return_to_fort, button_none, 3, 0},
        {544, 171, 30, 30, button_empire_service, button_none, 3, 0},
        {384, 215, 30, 30, button_go_to_legion,   button_none, 4, 0},
        {464, 215, 30, 30, button_return_to_fort, button_none, 4, 0},
        {544, 215, 30, 30, button_empire_service, button_none, 4, 0},
        {384, 259, 30, 30, button_go_to_legion,   button_none, 5, 0},
        {464, 259, 30, 30, button_return_to_fort, button_none, 5, 0},
        {544, 259, 30, 30, button_empire_service, button_none, 5, 0},
        {384, 303, 30, 30, button_go_to_legion,   button_none, 6, 0},
        {464, 303, 30, 30, button_return_to_fort, button_none, 6, 0},
        {544, 303, 30, 30, button_empire_service, button_none, 6, 0},
};

static int focus_button_id;
static int num_legions;

static void init() {
    num_legions = formation_get_num_legions();
    scrollbar_init(&scrollbar, 0, num_legions - 6);
}

static int draw_background(void) {
    outer_panel_draw(0, 0, 40, ADVISOR_HEIGHT);
    ImageDraw::img_generic(image_id_from_group(GROUP_ADVISOR_ICONS) + 1, 10, 10);
    lang_text_draw(51, 0, 60, 12, FONT_LARGE_BLACK_ON_LIGHT);

    lang_text_draw(51, 1, 374, 43, FONT_SMALL_PLAIN);
    lang_text_draw(51, 2, 374, 58, FONT_SMALL_PLAIN);
    lang_text_draw(51, 3, 454, 43, FONT_SMALL_PLAIN);
    lang_text_draw(51, 4, 454, 58, FONT_SMALL_PLAIN);
    lang_text_draw(51, 5, 534, 43, FONT_SMALL_PLAIN);
    lang_text_draw(51, 6, 534, 58, FONT_SMALL_PLAIN);
    lang_text_draw(138, 36, 274, 58, FONT_SMALL_PLAIN);

    int enemy_text_id;
    if (city_figures_enemies())
        enemy_text_id = 10;
    else if (city_figures_imperial_soldiers())
        enemy_text_id = 11;
    else if (scenario_invasion_exists_upcoming())
        enemy_text_id = 9;
    else {
        enemy_text_id = 8;
    }
    int distant_battle_text_id;
    if (city_military_distant_battle_roman_army_is_traveling_back())
        distant_battle_text_id = 15;
    else if (city_military_distant_battle_roman_army_is_traveling_forth())
        distant_battle_text_id = 14;
    else if (city_military_months_until_distant_battle() > 0)
        distant_battle_text_id = 13;
    else {
        distant_battle_text_id = 12;
    }
    int bullet_x = 60;
    int text_x = 80;
    if (num_legions <= 0) {
        ImageDraw::img_generic(image_id_from_group(GROUP_BULLET), bullet_x, 359);
        lang_text_draw(51, enemy_text_id, text_x, 358, FONT_NORMAL_BLACK_ON_LIGHT);

        ImageDraw::img_generic(image_id_from_group(GROUP_BULLET), bullet_x, 379);
        lang_text_draw(51, distant_battle_text_id, text_x, 378, FONT_NORMAL_BLACK_ON_LIGHT);
    } else {
        // has forts
        ImageDraw::img_generic(image_id_from_group(GROUP_BULLET), bullet_x, 349);
        int width = lang_text_draw_amount(8, 46, city_military_total_soldiers(), text_x, 348, FONT_NORMAL_BLACK_ON_LIGHT);
        width += lang_text_draw(51, 7, text_x + width, 348, FONT_NORMAL_BLACK_ON_LIGHT);
        lang_text_draw_amount(8, 48, city_military_total_legions(), text_x + width, 348, FONT_NORMAL_BLACK_ON_LIGHT);

        ImageDraw::img_generic(image_id_from_group(GROUP_BULLET), bullet_x, 369);
        lang_text_draw(51, enemy_text_id, text_x, 368, FONT_NORMAL_BLACK_ON_LIGHT);

        ImageDraw::img_generic(image_id_from_group(GROUP_BULLET), bullet_x, 389);
        lang_text_draw(51, distant_battle_text_id, text_x, 388, FONT_NORMAL_BLACK_ON_LIGHT);
    }

    inner_panel_draw(16, 70, 36, 17);
    if (num_legions <= 0) {
        lang_text_draw_multiline(51, 16, 64, 200, 496, FONT_NORMAL_BLACK_ON_DARK);
        return ADVISOR_HEIGHT;
    }
    for (int i = 0; i < 6 && i < num_legions; i++) {
        const formation *m = formation_get(formation_for_legion(i + 1 + scrollbar.scroll_position));
        button_border_draw(22, 77 + 44 * i, 560, 40, 0);
        ImageDraw::img_generic(image_id_from_group(GROUP_FIGURE_FORT_STANDARD_ICONS) + m->legion_id, 32, 82 + 44 * i);
        lang_text_draw(138, m->legion_id, 84, 83 + 44 * i, FONT_NORMAL_WHITE_ON_DARK);
        int width = text_draw_number(m->num_figures, '@', " ", 84, 100 + 44 * i, FONT_NORMAL_BLACK_ON_DARK);
        switch (m->figure_type) {
            case FIGURE_FORT_LEGIONARY:
                lang_text_draw(138, 33, 84 + width, 100 + 44 * i, FONT_NORMAL_BLACK_ON_DARK);
                break;
            case FIGURE_FORT_MOUNTED:
                lang_text_draw(138, 34, 84 + width, 100 + 44 * i, FONT_NORMAL_BLACK_ON_DARK);
                break;
            case FIGURE_FORT_JAVELIN:
                lang_text_draw(138, 35, 84 + width, 100 + 44 * i, FONT_NORMAL_BLACK_ON_DARK);
                break;
        }
        lang_text_draw_centered(138, 37 + m->morale / 5, 224, 91 + 44 * i, 150, FONT_NORMAL_BLACK_ON_DARK);

        int image_id = image_id_from_group(GROUP_FORT_ICONS);
        button_border_draw(384, 83 + 44 * i, 30, 30, 0);
        ImageDraw::img_generic(image_id, 387, 86 + 44 * i);

        button_border_draw(464, 83 + 44 * i, 30, 30, 0);
        if (m->is_at_fort)
            ImageDraw::img_generic(image_id + 2, 467, 86 + 44 * i);
        else {
            ImageDraw::img_generic(image_id + 1, 467, 86 + 44 * i);
        }

        button_border_draw(544, 83 + 44 * i, 30, 30, 0);
        if (m->empire_service)
            ImageDraw::img_generic(image_id + 3, 547, 86 + 44 * i);
        else {
            ImageDraw::img_generic(image_id + 4, 547, 86 + 44 * i);
        }
    }

    return ADVISOR_HEIGHT;
}

static void draw_foreground(void) {
    scrollbar_draw(&scrollbar);
    num_legions = formation_get_num_legions();
    for (int i = 0; i < 6 && i < num_legions; i++) {
        button_border_draw(384, 83 + 44 * i, 30, 30, focus_button_id == 3 * i + 1);
        button_border_draw(464, 83 + 44 * i, 30, 30, focus_button_id == 3 * i + 2);
        button_border_draw(544, 83 + 44 * i, 30, 30, focus_button_id == 3 * i + 3);
    }
}

static int handle_mouse(const mouse *m) {
    if (scrollbar_handle_mouse(&scrollbar, m))
        return 1;

    return generic_buttons_handle_mouse(m, 0, 0, fort_buttons, 3 * num_legions, &focus_button_id);
}

static void button_go_to_legion(int legion_id, int param2) {
    const formation *m = formation_get(formation_for_legion(legion_id + scrollbar.scroll_position));
    camera_go_to_mappoint(map_point(MAP_OFFSET(m->x_home, m->y_home)));
    window_city_show();
}

static void button_return_to_fort(int legion_id, int param2) {
    formation *m = formation_get(formation_for_legion(legion_id + scrollbar.scroll_position));
    if (!m->in_distant_battle) {
        formation_legion_return_home(m);
        window_invalidate();
    }
}

static void button_empire_service(int legion_id, int param2) {
    int formation_id = formation_for_legion(legion_id + scrollbar.scroll_position);
    formation_toggle_empire_service(formation_id);
    formation_calculate_figures();
    window_invalidate();
}

static void on_scroll(void) {
    window_invalidate();
}

const advisor_window_type *window_advisor_military(void) {
    static const advisor_window_type window = {
            draw_background,
            draw_foreground,
            handle_mouse,
            0
    };
    init();
    return &window;
}
