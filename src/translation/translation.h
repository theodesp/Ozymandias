#ifndef TRANSLATION_TRANSLATION_H
#define TRANSLATION_TRANSLATION_H

#include "core/locale.h"

#include <stdint.h>

enum {
    TR_NO_PATCH_TITLE,
    TR_NO_PATCH_MESSAGE,
    TR_MISSING_FONTS_TITLE,
    TR_MISSING_FONTS_MESSAGE,
    TR_NO_EDITOR_TITLE,
    TR_NO_EDITOR_MESSAGE,
    TR_INVALID_LANGUAGE_TITLE,
    TR_INVALID_LANGUAGE_MESSAGE,
    TR_BUTTON_OK,
    TR_BUTTON_CANCEL,
    TR_BUTTON_RESET_DEFAULTS,
    TR_BUTTON_CONFIGURE_HOTKEYS,
    TR_BUTTON_NEXT,
    TR_BUTTON_PREV,
    TR_CONFIG_TITLE,
    TR_CONFIG_LANGUAGE_LABEL,
    TR_CONFIG_LANGUAGE_DEFAULT,
    TR_CONFIG_PAGE_LABEL,
    TR_CONFIG_HEADER_UI_CHANGES,
    TR_CONFIG_HEADER_GAMEPLAY_CHANGES,
    TR_CONFIG_SHOW_INTRO_VIDEO,
    TR_CONFIG_SIDEBAR_INFO,
    TR_CONFIG_SMOOTH_SCROLLING,
    TR_CONFIG_VISUAL_FEEDBACK_ON_DELETE,
    TR_CONFIG_ALLOW_CYCLING_TEMPLES,
    TR_CONFIG_SHOW_WATER_STRUCTURE_RANGE,
    TR_CONFIG_SHOW_CONSTRUCTION_SIZE,
    TR_CONFIG_HIGHLIGHT_LEGIONS,
    TR_CONFIG_ROTATE_MANUALLY,
    TR_CONFIG_FIX_IMMIGRATION_BUG,
    TR_CONFIG_FIX_100_YEAR_GHOSTS,
    TR_CONFIG_FIX_EDITOR_EVENTS,
    TR_CONFIG_DRAW_WALKER_WAYPOINTS,
    TR_CONFIG_ZOOM_STEPPED,
    TR_CONFIG_COMPLETE_RATING_COLUMNS,
    TR_CONFIG_GRANDFESTIVAL,
    TR_CONFIG_JEALOUS_GODS,
    TR_CONFIG_GLOBAL_LABOUR,
    TR_CONFIG_SCHOOL_WALKERS,
    TR_CONFIG_RETIRE_AT_60,
    TR_CONFIG_FIXED_WORKERS,
    TR_CONFIG_EXTRA_FORTS,
    TR_CONFIG_WOLVES_BLOCK,
    TR_CONFIG_DYNAMIC_GRANARIES,
    TR_CONFIG_MORE_STOCKPILE,
    TR_CONFIG_NO_BUYER_DISTRIBUTION,
    TR_CONFIG_IMMEDIATELY_DELETE_BUILDINGS,
    TR_CONFIG_GETTING_GRANARIES_GO_OFFROAD,
    TR_CONFIG_GRANARIES_GET_DOUBLE,
    TR_CONFIG_TOWER_SENTRIES_GO_OFFROAD,
    TR_CONFIG_FARMS_DELIVER_CLOSE,
    TR_CONFIG_DELIVER_ONLY_TO_ACCEPTING_GRANARIES,
    TR_CONFIG_ALL_HOUSES_MERGE,
    TR_CONFIG_WINE_COUNTS_IF_OPEN_TRADE_ROUTE,
    TR_CONFIG_RANDOM_COLLAPSES_TAKE_MONEY,
    TR_CONFIG_MULTIPLE_BARRACKS,
    TR_CONFIG_NOT_ACCEPTING_WAREHOUSES,
    TR_CONFIG_HOUSES_DONT_EXPAND_INTO_GARDENS,
    TR_HOTKEY_TITLE,
    TR_HOTKEY_LABEL,
    TR_HOTKEY_ALTERNATIVE_LABEL,
    TR_HOTKEY_HEADER_ARROWS,
    TR_HOTKEY_HEADER_GLOBAL,
    TR_HOTKEY_HEADER_CITY,
    TR_HOTKEY_HEADER_ADVISORS,
    TR_HOTKEY_HEADER_OVERLAYS,
    TR_HOTKEY_HEADER_BOOKMARKS,
    TR_HOTKEY_HEADER_EDITOR,
    TR_HOTKEY_HEADER_BUILD,
    TR_HOTKEY_ARROW_UP,
    TR_HOTKEY_ARROW_DOWN,
    TR_HOTKEY_ARROW_LEFT,
    TR_HOTKEY_ARROW_RIGHT,
    TR_HOTKEY_TOGGLE_FULLSCREEN,
    TR_HOTKEY_CENTER_WINDOW,
    TR_HOTKEY_RESIZE_TO_640,
    TR_HOTKEY_RESIZE_TO_800,
    TR_HOTKEY_RESIZE_TO_1024,
    TR_HOTKEY_SAVE_SCREENSHOT,
    TR_HOTKEY_SAVE_CITY_SCREENSHOT,
    TR_HOTKEY_LOAD_FILE,
    TR_HOTKEY_SAVE_FILE,
    TR_HOTKEY_INCREASE_GAME_SPEED,
    TR_HOTKEY_DECREASE_GAME_SPEED,
    TR_HOTKEY_TOGGLE_PAUSE,
    TR_HOTKEY_CYCLE_LEGION,
    TR_HOTKEY_ROTATE_MAP_LEFT,
    TR_HOTKEY_ROTATE_MAP_RIGHT,
    TR_HOTKEY_SHOW_ADVISOR_LABOR,
    TR_HOTKEY_SHOW_ADVISOR_MILITARY,
    TR_HOTKEY_SHOW_ADVISOR_IMPERIAL,
    TR_HOTKEY_SHOW_ADVISOR_RATINGS,
    TR_HOTKEY_SHOW_ADVISOR_TRADE,
    TR_HOTKEY_SHOW_ADVISOR_POPULATION,
    TR_HOTKEY_SHOW_ADVISOR_HEALTH,
    TR_HOTKEY_SHOW_ADVISOR_EDUCATION,
    TR_HOTKEY_SHOW_ADVISOR_ENTERTAINMENT,
    TR_HOTKEY_SHOW_ADVISOR_RELIGION,
    TR_HOTKEY_SHOW_ADVISOR_FINANCIAL,
    TR_HOTKEY_SHOW_ADVISOR_CHIEF,
    TR_HOTKEY_SHOW_ADVISOR_HOUSING,
    TR_HOTKEY_TOGGLE_OVERLAY,
    TR_HOTKEY_SHOW_OVERLAY_WATER,
    TR_HOTKEY_SHOW_OVERLAY_FIRE,
    TR_HOTKEY_SHOW_OVERLAY_DAMAGE,
    TR_HOTKEY_SHOW_OVERLAY_CRIME,
    TR_HOTKEY_SHOW_OVERLAY_PROBLEMS,
    TR_HOTKEY_ROTATE_BUILDING,
    TR_HOTKEY_GO_TO_BOOKMARK_1,
    TR_HOTKEY_GO_TO_BOOKMARK_2,
    TR_HOTKEY_GO_TO_BOOKMARK_3,
    TR_HOTKEY_GO_TO_BOOKMARK_4,
    TR_HOTKEY_SET_BOOKMARK_1,
    TR_HOTKEY_SET_BOOKMARK_2,
    TR_HOTKEY_SET_BOOKMARK_3,
    TR_HOTKEY_SET_BOOKMARK_4,
    TR_HOTKEY_EDITOR_TOGGLE_BATTLE_INFO,
    TR_HOTKEY_EDIT_TITLE,
    TR_BUILDING_ROADBLOCK,
    TR_BUILDING_ROADBLOCK_DESC,
    TR_HEADER_HOUSING,
    TR_ADVISOR_HOUSING_ROOM,
    TR_ADVISOR_HOUSING_NO_ROOM,
    TR_ADVISOR_RESIDENCES_DEMANDING_POTTERY,
    TR_ADVISOR_RESIDENCES_DEMANDING_FURNITURE,
    TR_ADVISOR_RESIDENCES_DEMANDING_OIL,
    TR_ADVISOR_RESIDENCES_DEMANDING_WINE,
    TR_ADVISOR_TOTAL_NUM_HOUSES,
    TR_ADVISOR_AVAILABLE_HOUSING_CAPACITY,
    TR_ADVISOR_TOTAL_HOUSING_CAPACITY,
    TR_ADVISOR_ADVISOR_HEADER_HOUSING,
    TR_ADVISOR_BUTTON_GRAPHS,
    TR_ADVISOR_HOUSING_PROSPERITY_RATING,
    TR_ADVISOR_PERCENTAGE_IN_VILLAS_PALACES,
    TR_ADVISOR_PERCENTAGE_IN_TENTS_SHACKS,
    TR_ADVISOR_AVERAGE_TAX,
    TR_ADVISOR_AVERAGE_AGE,
    TR_ADVISOR_PERCENT_IN_WORKFORCE,
    TR_ADVISOR_BIRTHS_LAST_YEAR,
    TR_ADVISOR_DEATHS_LAST_YEAR,
    TR_ADVISOR_TOTAL_POPULATION,
    TRANSLATION_MAX_KEY
};

typedef struct {
    int key;
    const char *string;
} translation_string;

void translation_load(int language);

const uint8_t *translation_for(int key);

void translation_english(const translation_string **strings, int *num_strings);

void translation_french(const translation_string **strings, int *num_strings);

void translation_german(const translation_string **strings, int *num_strings);

void translation_italian(const translation_string **strings, int *num_strings);

void translation_korean(const translation_string **strings, int *num_strings);

void translation_polish(const translation_string **strings, int *num_strings);

void translation_portuguese(const translation_string **strings, int *num_strings);

void translation_russian(const translation_string **strings, int *num_strings);

void translation_spanish(const translation_string **strings, int *num_strings);

void translation_simplified_chinese(const translation_string **strings, int *num_strings);

void translation_traditional_chinese(const translation_string **strings, int *num_strings);

#endif // TRANSLATION_TRANSLATION_H
