#include <city/floods.h>
#include <scenario/events.h>
#include "io/manager.h"
#include <grid/vegetation.h>
#include "tick.h"

#include "building/count.h"
#include "building/dock.h"
#include "building/figure.h"
#include "building/government.h"
#include "building/granary.h"
#include "building/house_evolution.h"
#include "building/house_population.h"
#include "building/house_service.h"
#include "building/industry.h"
#include "building/maintenance.h"
#include "building/warehouse.h"
#include "city/culture.h"
#include "city/emperor.h"
#include "city/festival.h"
#include "city/finance.h"
#include "city/gods.h"
#include "city/health.h"
#include "city/labor.h"
#include "city/message.h"
#include "city/migration.h"
#include "city/population.h"
#include "city/ratings.h"
#include "city/resource.h"
#include "city/sentiment.h"
#include "city/trade.h"
#include "city/victory.h"
#include "core/random.h"
#include "editor/editor.h"
#include "empire/city.h"
#include "figure/formation.h"
#include "figuretype/crime.h"
#include "io/gamestate/boilerplate.h"
#include "game/settings.h"
#include "game/time.h"
#include "game/tutorial.h"
#include "game/undo.h"
#include "grid/desirability.h"
#include "grid/natives.h"
#include "grid/road_network.h"
#include "grid/routing/routing_terrain.h"
#include "grid/tiles.h"
#include "grid/water_supply.h"
#include "scenario/demand_change.h"
#include "scenario/distant_battle.h"
#include "scenario/earthquake.h"
#include "scenario/emperor_change.h"
#include "scenario/empire.h"
#include "scenario/gladiator_revolt.h"
#include "scenario/invasion.h"
#include "scenario/price_change.h"
#include "scenario/random_event.h"
#include "scenario/request.h"
#include "sound/music.h"
#include "widget/minimap.h"

static void advance_year(void) {
    scenario_empire_process_expansion();
    game_undo_disable();
    game_time_advance_year();
    city_population_request_yearly_update();
    city_finance_handle_year_change();
    empire_city_reset_yearly_trade_amounts();
    building_maintenance_update_fire_direction();
    city_ratings_update(1);
//    city_gods_reset_yearly_blessings();
}

static void advance_month(void) {
    city_migration_reset_newcomers();
    city_health_update();
    scenario_random_event_process();
    city_finance_handle_month_change();
    city_resource_consume_food();
    scenario_distant_battle_process();
    if (GAME_ENV == ENGINE_ENV_C3) {
        scenario_invasion_process();
        scenario_request_process_C3();
        scenario_demand_change_process();
        scenario_price_change_process();
    } else if (GAME_ENV == ENGINE_ENV_PHARAOH) { // Pharaoh uses its own event system
        random_generate_next(); // TODO: find out the source / reason for this
        scenario_event_process();
    }
    city_victory_update_months_to_govern();
    formation_update_monthly_morale_at_rest();
    city_message_decrease_delays();

    map_tiles_update_all_roads();
//    map_tiles_river_refresh_entire();
    map_routing_update_land_citizen();
//    city_message_sort_and_compact();

    if (game_time_advance_month())
        advance_year();
    else
        city_ratings_update(0);

    city_population_record_monthly();
    city_festival_update();
    if (GAME_ENV == ENGINE_ENV_C3)
        tutorial_on_month_tick();
    if (setting_monthly_autosave()) {
//        if (0)
//            SaveFileIO::write_savegame("autosave.sav");
//        if (1)
//            SaveFileIO::write_savegame("autosave_history.sav");
//        if (2)
//            SaveFileIO::write_savegame("autosave_replay.sav");
    }
}

static void advance_day(void) {
//    map_advance_floodplain_growth();

    if (game_time_advance_day())
        advance_month();

    if (game_time_day() % 8 == 0)
        city_sentiment_update();

    tutorial_on_day_tick();
}

static void advance_tick(void) {

    tutorial_starting_message();

    if (GAME_ENV == ENGINE_ENV_PHARAOH)
        floodplains_tick_update(false);

    // NB: these ticks are noop:
    // 0, 9, 11, 13, 14, 15, 26, 41, 42, 47

    switch (game_time_tick()) {
        case 1:
            city_gods_update(false);
            break;
        case 2:
            sound_music_update(false);
            break;
        case 3:
            widget_minimap_invalidate();
            break;
        case 4:
            city_emperor_update();
            break;
        case 5:
            formation_update_all(false);
            break;
        case 6:
            map_natives_check_land();
            break;
        case 7:
            map_road_network_update();
            break;
        case 8:
            building_granaries_calculate_stocks();
            break;
        case 10:
            building_update_highest_id();
            break;
        case 12:
            house_service_decay_houses_covered();
            break;
        case 16:
            city_resource_calculate_warehouse_stocks();
            break;
        case 17:
            city_resource_calculate_food_stocks_and_supply_wheat();
            break;
        case 18:
//            city_resource_calculate_workshop_stocks();
            break;
        case 19:
            building_dock_update_open_water_access();
            break;
        case 20:
            building_industry_update_production();
            vegetation_growth_update();
            break;
        case 21:
            building_maintenance_check_rome_access();
            break;
        case 22:
            house_population_update_room();
            break;
        case 23:
            house_population_update_migration();
            break;
        case 24:
            house_population_evict_overcrowded();
            break;
        case 25:
            city_labor_update();
            break;
        case 27:
            map_update_wells_range();
            map_update_canals();
            break;
        case 28:
            map_water_supply_update_houses();
            break;
        case 29:
            formation_update_all(true);
            break;
        case 30:
            widget_minimap_invalidate();
            break;
        case 31:
            building_figure_generate();
            break;
        case 32:
            city_trade_update();
            break;
        case 33:
            building_count_update();
            building_entertainment_update();
            city_culture_update_coverage();
            building_industry_update_farms();
            break;
        case 34:
            building_government_distribute_treasury();
            break;
        case 35:
            house_service_decay_culture();
            break;
        case 36:
            house_service_calculate_culture_aggregates();
            break;
        case 37:
            map_desirability_update();
            break;
        case 38:
            building_update_desirability();
            break;
        case 39:
            building_house_process_evolve_and_consume_goods();
            break;
        case 40:
            building_update_state();
            break;
        case 43:
            building_maintenance_update_burning_ruins();
            break;
        case 44:
            building_maintenance_check_fire_collapse();
            break;
        case 45:
            figure_generate_criminals();
            break;
        case 46:
            building_industry_update_wheat_production();
            break;
        case 48:
            house_service_decay_tax_collector();
            break;
        case 49:
            city_culture_calculate();
            break;
        case 50:
            break;
    }
    if (game_time_advance_tick())
        advance_day();
}

void game_tick_run(void) {
    if (editor_is_active()) {
        random_generate_next(); // update random to randomize native huts
        figure_action_handle(); // just update the flag figures
        return;
    }
    random_generate_next();
    game_undo_reduce_time_available();
    advance_tick();
    figure_action_handle();
    scenario_earthquake_process();
    scenario_gladiator_revolt_process();
    scenario_emperor_change_process();
    city_victory_check();
}

void game_tick_cheat_year(void) {
    advance_year();
}
