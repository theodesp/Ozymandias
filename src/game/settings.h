#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

#include <stdint.h>

enum {
    TOOLTIPS_NONE = 0,
    TOOLTIPS_SOME = 1,
    TOOLTIPS_FULL = 2
};

enum {
    DIFFICULTY_VERY_EASY = 0,
    DIFFICULTY_EASY = 1,
    DIFFICULTY_NORMAL = 2,
    DIFFICULTY_HARD = 3,
    DIFFICULTY_VERY_HARD = 4
};

enum {
    SOUND_MUSIC = 1,
    SOUND_SPEECH = 2,
    SOUND_EFFECTS = 3,
    SOUND_CITY = 4,
};

enum {
    CITIES_OLD_NAMES = 0,
    CITIES_NEW_NAMES = 1
};

typedef struct {
    bool enabled;
    int volume;
} set_sound;

void settings_load(void);
void settings_save(void);

int setting_fullscreen(void);
void setting_window(int *width, int *height);
void setting_set_display(int fullscreen, int width, int height);

const set_sound *setting_sound(int type);

int setting_sound_is_enabled(int type);
void setting_toggle_sound_enabled(int type);
void setting_increase_sound_volume(int type);
void setting_decrease_sound_volume(int type);
void setting_reset_sound(int type, int enabled, int volume);

int setting_game_speed(void);
void setting_increase_game_speed(void);
void setting_decrease_game_speed(void);

int setting_scroll_speed(void);
void setting_increase_scroll_speed(void);
void setting_decrease_scroll_speed(void);
void setting_reset_speeds(int game_speed, int scroll_speed);

int setting_tooltips(void);
void setting_cycle_tooltips(void);

int setting_warnings(void);
void setting_toggle_warnings(void);

int setting_monthly_autosave(void);
void setting_toggle_monthly_autosave(void);

int setting_city_names_style(void);
void setting_toggle_city_names_style(void);

int setting_pyramid_speedup(void);
void setting_toggle_pyramid_speedup(void);

int setting_popup_messages(void);
void setting_toggle_popup_messages(int flag);

bool setting_gods_enabled(void);
void setting_toggle_gods_enabled(void);

int setting_difficulty(void);
void setting_increase_difficulty(void);
void setting_decrease_difficulty(void);

int setting_victory_video(void);

int setting_last_advisor(void);
void setting_set_last_advisor(int advisor);

const uint8_t *setting_player_name(void);
const char *setting_player_name_utf8(void);
void setting_set_player_name(const uint8_t *player_name);

int setting_personal_savings_for_mission(int mission_id);
void setting_set_personal_savings_for_mission(int mission_id, int savings);
void setting_clear_personal_savings(void);

#endif // GAME_SETTINGS_H
