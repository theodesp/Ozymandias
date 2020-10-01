#include "action.h"

#include "city/entertainment.h"
#include "city/figures.h"
#include "figure/figure.h"
#include "figuretype/animal.h"
#include "figuretype/cartpusher.h"
#include "figuretype/crime.h"
#include "figuretype/docker.h"
#include "figuretype/editor.h"
#include "figuretype/enemy.h"
#include "figuretype/entertainer.h"
#include "figuretype/maintenance.h"
#include "figuretype/market.h"
#include "figuretype/migrant.h"
#include "figuretype/missile.h"
#include "figuretype/native.h"
#include "figuretype/service.h"
#include "figuretype/soldier.h"
#include "figuretype/trader.h"
#include "figuretype/wall.h"
#include "figuretype/water.h"
#include "core/game_environment.h"
#include "map/road_access.h"

//static void no_action() {
//
//}

typedef struct {
    char terrain_usage;
    short max_roam_length;
    short image_offset_max;
} figure_action_property;

static figure_action_property action_lookup[] = {
        {0,                     0,      12},  //FIGURE_NONE = 0,
        {TERRAIN_USAGE_ANY,     0,      12},  //FIGURE_IMMIGRANT = 1,
        {TERRAIN_USAGE_ANY,     0,      12},  //FIGURE_EMIGRANT = 2,
        {TERRAIN_USAGE_ANY,     0,      12},  //FIGURE_HOMELESS = 3,
        {TERRAIN_USAGE_ROADS,   0,      12},  //FIGURE_CART_PUSHER = 4,
        {TERRAIN_USAGE_ROADS,   384,    12},  //FIGURE_LABOR_SEEKER = 5,
        {TERRAIN_USAGE_ANY,     0,      12},  //FIGURE_EXPLOSION = 6,
        {TERRAIN_USAGE_ROADS,   512,    12},  //FIGURE_TAX_COLLECTOR = 7,
        {TERRAIN_USAGE_ROADS,   640,    12},  //FIGURE_ENGINEER = 8,
        {TERRAIN_USAGE_ROADS,   0,      12},  //FIGURE_WAREHOUSEMAN = 9,
        {TERRAIN_USAGE_ROADS,   640,    12},  //FIGURE_PREFECT = 10,
        {TERRAIN_USAGE_ANY,     0,      12},  //FIGURE_FORT_JAVELIN = 11,
        {TERRAIN_USAGE_ANY,     0,      12},  //FIGURE_FORT_MOUNTED = 12,
        {TERRAIN_USAGE_ANY,     0,      12},  //FIGURE_FORT_LEGIONARY = 13,
        {TERRAIN_USAGE_ANY,     0,      12},  //FIGURE_FORT_STANDARD = 14,
        {TERRAIN_USAGE_ROADS,   512,    12},  //FIGURE_ACTOR = 15,
        {TERRAIN_USAGE_ROADS,   512,    12},  //FIGURE_GLADIATOR = 16,
        {TERRAIN_USAGE_ROADS,   512,    12},  //FIGURE_LION_TAMER = 17,
        {TERRAIN_USAGE_ROADS,   512,    12},  //FIGURE_CHARIOTEER = 18,
        {TERRAIN_USAGE_ROADS,   0,      12},  //FIGURE_TRADE_CARAVAN = 19,
        {TERRAIN_USAGE_ANY,     0,      12},  //FIGURE_TRADE_SHIP = 20,
        {TERRAIN_USAGE_ROADS,   0,      12},  //FIGURE_TRADE_CARAVAN_DONKEY = 21,
        {TERRAIN_USAGE_ROADS,   0,      12},  //FIGURE_PROTESTER = 22,
        {TERRAIN_USAGE_ROADS,   0,      12},  //FIGURE_CRIMINAL = 23,
        {TERRAIN_USAGE_ROADS,   480,    12},  //FIGURE_RIOTER = 24,
        {TERRAIN_USAGE_ANY,     0,      12},  //FIGURE_FISHING_BOAT = 25,
        {TERRAIN_USAGE_ROADS,   384,    12},  //FIGURE_MARKET_TRADER = 26,
        {TERRAIN_USAGE_ROADS,   384,    12},  //FIGURE_PRIEST = 27,
        {TERRAIN_USAGE_ROADS,   192,    12},   //FIGURE_SCHOOL_CHILD = 28,
        {TERRAIN_USAGE_ROADS,   384,    12},  //FIGURE_TEACHER = 29,
        {TERRAIN_USAGE_ROADS,   384,    12},  //FIGURE_LIBRARIAN = 30,
        {TERRAIN_USAGE_ROADS,   384,    12},  //FIGURE_BARBER = 31,
        {TERRAIN_USAGE_ROADS,   384,    12},  //FIGURE_BATHHOUSE_WORKER = 32,
        {TERRAIN_USAGE_ROADS,   384,    12},  //FIGURE_DOCTOR = 33,
        {TERRAIN_USAGE_ROADS,   384,    12},   //FIGURE_SURGEON = 34,
        {TERRAIN_USAGE_ROADS,   384,    12},   //FIGURE_WORKER = 35,
        {TERRAIN_USAGE_ANY,     0,      12},   //FIGURE_MAP_FLAG = 36,
        {TERRAIN_USAGE_ANY,     0,      12}, //FIGURE_FLOTSAM = 37,
        {TERRAIN_USAGE_ROADS,   0,      12},    //FIGURE_DOCKER = 38,
        {TERRAIN_USAGE_ROADS,   800,    12},   //FIGURE_MARKET_BUYER = 39,
        {TERRAIN_USAGE_ROADS,   128,    12},   //FIGURE_PATRICIAN = 40,
        {TERRAIN_USAGE_ANY,     800,    12},   //FIGURE_INDIGENOUS_NATIVE = 41,
        {-1,                    800,    12},   //FIGURE_TOWER_SENTRY = 42,
        {TERRAIN_USAGE_ANY,     0,      12},   //FIGURE_ENEMY43_SPEAR = 43,
        {TERRAIN_USAGE_ANY,     0,      12},   //FIGURE_ENEMY44_SWORD = 44,
        {TERRAIN_USAGE_ANY,     0,      12},   //FIGURE_ENEMY45_SWORD = 45,
        {TERRAIN_USAGE_ANY,     0,      12},   //FIGURE_ENEMY46_CAMEL = 46,
        {TERRAIN_USAGE_ANY,     0,      12},   //FIGURE_ENEMY47_ELEPHANT = 47,
        {TERRAIN_USAGE_ANY,     0,      12},   //FIGURE_ENEMY48_CHARIOT = 48,
        {TERRAIN_USAGE_ANY,     0,      12},  //FIGURE_ENEMY49_FAST_SWORD = 49,
        {TERRAIN_USAGE_ANY,     0,      12},  //FIGURE_ENEMY50_SWORD = 50,
        {TERRAIN_USAGE_ANY,     0,      12},  //FIGURE_ENEMY51_SPEAR = 51,
        {TERRAIN_USAGE_ANY,     0,      12},   //FIGURE_ENEMY52_MOUNTED_ARCHER = 52,
        {TERRAIN_USAGE_ANY,     0,      12},   //FIGURE_ENEMY53_AXE = 53,
        {TERRAIN_USAGE_ANY,     0,      12},   //FIGURE_ENEMY54_GLADIATOR = 54,
        {TERRAIN_USAGE_ANY,     0,      12},   //FIGURE_ENEMY_CAESAR_JAVELIN = 55,
        {TERRAIN_USAGE_ANY,     0,      12},   //FIGURE_ENEMY_CAESAR_MOUNTED = 56,
        {TERRAIN_USAGE_ANY,     0,      12},   //FIGURE_ENEMY_CAESAR_LEGIONARY = 57,
        {TERRAIN_USAGE_ROADS,   0,      12},   //FIGURE_NATIVE_TRADER = 58,
        {TERRAIN_USAGE_ANY,     0,      12},   //FIGURE_ARROW = 59,
        {TERRAIN_USAGE_ANY,     0,      12},   //FIGURE_JAVELIN = 60,
        {TERRAIN_USAGE_ANY,     0,      12},   //FIGURE_BOLT = 61,
        {TERRAIN_USAGE_ANY,     0,      12},   //FIGURE_BALLISTA = 62,
        {TERRAIN_USAGE_ANY,     0,      12},  //FIGURE_CREATURE = 63,
        {TERRAIN_USAGE_ROADS,   192,    12},    //FIGURE_MISSIONARY = 64,
        {TERRAIN_USAGE_ANY,     0,      12},   //FIGURE_FISH_GULLS = 65,
        {TERRAIN_USAGE_ROADS,   0,      12},  //FIGURE_DELIVERY_BOY = 66,
        {TERRAIN_USAGE_ANY,     0,      12},  //FIGURE_SHIPWRECK = 67,
        {TERRAIN_USAGE_ANY,     0,      12},  //FIGURE_SHEEP = 68,
        {TERRAIN_USAGE_ANY,     0,      12},  //FIGURE_WOLF = 69,
        {TERRAIN_USAGE_ANY,     0,      12},  //FIGURE_ZEBRA = 70,
        {TERRAIN_USAGE_ANY,     0,      12},  //FIGURE_SPEAR = 71,
        {TERRAIN_USAGE_ANY,     0,      12},  //FIGURE_HIPPODROME_HORSES = 72,

        // PHARAOH

        {TERRAIN_USAGE_ANY,     0,      12},  // 73
        {TERRAIN_USAGE_ANY,     0,      12},  // 74
        {TERRAIN_USAGE_ANY,     0,      12},  // 75
        {TERRAIN_USAGE_ANY,     0,      12},  // 76
        {TERRAIN_USAGE_ROADS,   0,      12},  // 77
        {TERRAIN_USAGE_ROADS,   0,      12},  // 78
        {TERRAIN_USAGE_ANY,     0,      12},  // 79
        {TERRAIN_USAGE_ROADS,   0,      12},  // 80
        {TERRAIN_USAGE_ROADS,   0,      12},  // 81
        {TERRAIN_USAGE_ROADS,   0,      12},  // 82
        {TERRAIN_USAGE_ANY,     0,      12},  // 83
        {TERRAIN_USAGE_ANY,     0,      12},  // 84
        {TERRAIN_USAGE_ANY,     0,      12},  // 85
        {TERRAIN_USAGE_ANY,     0,      12},  // 86
        {TERRAIN_USAGE_ANY,     0,      12},  // 87
        {TERRAIN_USAGE_ROADS,   640,    12},  // 88 policeman
        {TERRAIN_USAGE_ANY,     0,      12},  // 89
        {TERRAIN_USAGE_ANY,     0,      12},  // 90
        {TERRAIN_USAGE_ANY,     0,      12},  // 91
        {TERRAIN_USAGE_ANY,     0,      12},  // 92
        {TERRAIN_USAGE_ANY,     0,      12},  // 93
        {TERRAIN_USAGE_ANY,     0,      12},  // 94
        {TERRAIN_USAGE_ANY,     0,      12},  // 95
        {TERRAIN_USAGE_ANY,     0,      12},  // 96
        {TERRAIN_USAGE_ROADS,   0,      12},  // 97
        {TERRAIN_USAGE_ROADS,   0,      12},  // 98
        {TERRAIN_USAGE_ANY,     0,      12},  // 99
        {TERRAIN_USAGE_ROADS,   0,      12},  // 100
};

#include "core/image.h"

void figure::common_action(int max_roam_frames, int group) {
//    figure_image_increase_offset(max_roam_frames);

    figure_image_update(image_id_from_group(group));
}

void figure::action_perform() {
    if (state) {
        if (targeted_by_figure_id) {
            figure *attacker = figure_get(targeted_by_figure_id);
            if (attacker->state != FIGURE_STATE_ALIVE)
                targeted_by_figure_id = 0;
            if (attacker->target_figure_id != id)
                targeted_by_figure_id = 0;
        }


        //////////////

        // reset cart image & max roaming length
        cart_image_id = 0;
        max_roam_length = 0;

        // set roaming length and terrain usage
        auto action_props = action_lookup[type];
        if (action_props.terrain_usage != -1)
            terrain_usage = action_props.terrain_usage;
        max_roam_length = action_props.max_roam_length;

        // check for building being alive (at the start of the action)
        if (type == FIGURE_IMMIGRANT) {
            building *b = building_get(immigrant_building_id);
            if (b->state != BUILDING_STATE_IN_USE || b->immigrant_figure_id != id || !b->house_size)
                return figure_delete(); // state = FIGURE_STATE_DEAD;
        }
        building *b = building_get(building_id);
        switch (type) {
            case FIGURE_ENGINEER:
            case FIGURE_PREFECT:
            case FIGURE_WORKER:
            case FIGURE_NATIVE_TRADER:
            case FIGURE_TAX_COLLECTOR:
            case FIGURE_TOWER_SENTRY:
            case FIGURE_MISSIONARY:
            case FIGURE_ACTOR:
            case FIGURE_GLADIATOR:
            case FIGURE_LION_TAMER:
            case FIGURE_CHARIOTEER:
                if (b->state != BUILDING_STATE_IN_USE || b->figure_id != id)
                    state = FIGURE_STATE_DEAD;
                break;
            case FIGURE_LABOR_SEEKER:
            case FIGURE_MARKET_BUYER:
                if (b->state != BUILDING_STATE_IN_USE || b->figure_id2 != id)
                    state = FIGURE_STATE_DEAD;
                break;
        }

        //////////////// roamer_action()
        // common action states handling
        switch (action_state) {
            case FIGURE_ACTION_150_ATTACK:
                figure_combat_handle_attack(); break;
            case FIGURE_ACTION_149_CORPSE:
                figure_combat_handle_corpse(); break;
            case FIGURE_ACTION_LABORSEEKER_ROAM:
            case FIGURE_ACTION_COMMON_ROAM:
            case FIGURE_ACTION_125_ROAMING:
                is_ghost = 0;
                roam_length++;
                if (roam_length >= max_roam_length) {
                    int x, y;
                    building *b = building_get(building_id);
                    if (map_closest_road_within_radius(b->x, b->y, b->size, 2, &x, &y)) {
                        action_state = FIGURE_ACTION_126_ROAMER_RETURNING;
                        destination_x = x;
                        destination_y = y;
                        route_remove();
                        roam_length = 0;
                    } else
                        state = FIGURE_STATE_DEAD;
                }
                roam_ticks(1);
                break;
            case FIGURE_ACTION_LABORSEEKER_RETURN:
            case FIGURE_ACTION_COMMON_RETURN:
            case FIGURE_ACTION_126_ROAMER_RETURNING:
                move_ticks(1);
                if (direction == DIR_FIGURE_AT_DESTINATION ||
                    direction == DIR_FIGURE_REROUTE || direction == DIR_FIGURE_LOST) {
                    state = FIGURE_STATE_DEAD;
                }
                break;
        }

        ////////////

        switch (type) {
            case 1: immigrant_action(); break;
            case 2: emigrant_action(); break;
            case 3: homeless_action(); break;
            case 4: cartpusher_action(); break;
            case 5: common_action(12, GROUP_FIGURE_LABOR_SEEKER); break;
            case 6: explosion_cloud_action(); break;
            case 7: tax_collector_action(); break;
            case 8: engineer_action(); break;
            case 9: warehouseman_action(); break;
            case 10: prefect_action(); break; //10
            case 11: //soldier_action(); break;
            case 12: //soldier_action(); break;
            case 13: soldier_action(); break;
            case 14: military_standard_action(); break;
            case 15: //entertainer_action(); break;
            case 16: //entertainer_action(); break;
            case 17: //entertainer_action(); break;
            case 18: entertainer_action(); break;
            case 19: trade_caravan_action(); break;
            case 20: trade_ship_action(); break; //20
            case 21: trade_caravan_donkey_action(); break;
            case 22: protestor_action(); break;
            case 23: criminal_action(); break;
            case 24: rioter_action(); break;
            case 25: fishing_boat_action(); break;
            case 26: market_trader_action(); break;
            case 27: common_action(12, GROUP_FIGURE_PRIEST); break;
            case 28: school_child_action(); break;
            case 29: common_action(12, GROUP_FIGURE_TEACHER_LIBRARIAN); break;
            case 30: common_action(12, GROUP_FIGURE_TEACHER_LIBRARIAN); break; //30
            case 31: common_action(12, GROUP_FIGURE_BARBER); break;
            case 32: common_action(12, GROUP_FIGURE_BATHHOUSE_WORKER); break;
            case 33: //doctor_action(); break;
            case 34: common_action(12, GROUP_FIGURE_DOCTOR_SURGEON); break;
            case 35: worker_action(); break;
            case 36: editor_flag_action(); break;
            case 37: flotsam_action(); break;
            case 38: docker_action(); break;
            case 39: market_buyer_action(); break;
            case 40: patrician_action(); break; //40
            case 41: indigenous_native_action(); break;
            case 42: tower_sentry_action(); break;
            case 43: enemy43_spear_action(); break;
            case 44: enemy44_sword_action(); break;
            case 45: enemy45_sword_action(); break;
            case 46: enemy_camel_action(); break;
            case 47: enemy_elephant_action(); break;
            case 48: enemy_chariot_action(); break;
            case 49: enemy49_fast_sword_action(); break;
            case 50: enemy50_sword_action(); break; //50
            case 51: enemy51_spear_action(); break;
            case 52: enemy52_mounted_archer_action(); break;
            case 53: enemy53_axe_action(); break;
            case 54: enemy_gladiator_action(); break;
//                no_action(); break;
//                no_action(); break;
            case 57: enemy_caesar_legionary_action(); break;
            case 58: native_trader_action(); break;
            case 59: arrow_action(); break;
            case 60: javelin_action(); break; //60
            case 61: bolt_action(); break;
            case 62: ballista_action(); break;
//                no_action(); break;
            case 64: missionary_action(); break;
            case 65: seagulls_action(); break;
            case 66: delivery_boy_action(); break;
            case 67: shipwreck_action(); break;
            case 68: sheep_action(); break;
            case 69: wolf_action(); break;
            case 70: zebra_action(); break; //70
            case 71: spear_action(); break;
            case 72: hippodrome_horse_action(); break;
            // PHARAOH vvvv
            case 88:
                policeman_action(); break;
            default:
                break;
        }

        // if DEAD, delete figure
        if (state == FIGURE_STATE_DEAD)
            return figure_delete();

        // advance sprite offset
        figure_image_increase_offset(action_props.image_offset_max);
    }
}

void figure_action_handle(void) {
    city_figures_reset();
    city_entertainment_set_hippodrome_has_race(0);
    for (int i = 1; i < MAX_FIGURES[GAME_ENV]; i++)
        figure_get(i)->action_perform();
}
