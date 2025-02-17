#include <cmath>
#include "granary.h"

#include "building/destruction.h"
#include "building/model.h"
#include "building/storage.h"
#include "building/warehouse.h"
#include "city/message.h"
#include "city/resource.h"
#include "core/calc.h"
#include "io/config/config.h"
#include "grid/routing/routing_terrain.h"
#include "scenario/property.h"
#include "sound/effect.h"
#include "core/game_environment.h"

#define MAX_GRANARIES 100
#define ONE_LOAD 100
#define UNITS_PER_LOAD 100
#define CURSE_LOADS 16
#define INFINITE 10000

static struct {
    int building_ids[MAX_GRANARIES];
    int num_items;
    int total_storage_wheat;
    int total_storage_vegetables;
    int total_storage_fruit;
    int total_storage_meat;
} non_getting_granaries;

int building_granary_get_amount(building *granary, int resource) {
    if (!resource_is_food(resource))
        return 0;

    if (granary->type != BUILDING_GRANARY)
        return 0;

    return granary->data.granary.resource_stored[resource];
}

int FULL_GRANARY = 3200;
int THREEQUARTERS_GRANARY = 2400;
int HALF_GRANARY = 1600;
int QUARTER_GRANARY = 800;

bool building_granary_is_accepting(int resource, building *b) {
    const building_storage *s = building_storage_get(b->storage_id);
    int amount = building_granary_get_amount(b, resource);
    if ((s->resource_state[resource] == STORAGE_STATE_PHARAOH_ACCEPT && s->resource_max_accept[resource] == FULL_GRANARY) ||
        (s->resource_state[resource] == STORAGE_STATE_PHARAOH_ACCEPT && s->resource_max_accept[resource] >= THREEQUARTERS_GRANARY && amount < THREEQUARTERS_GRANARY) ||
        (s->resource_state[resource] == STORAGE_STATE_PHARAOH_ACCEPT && s->resource_max_accept[resource] >= HALF_GRANARY && amount < HALF_GRANARY) ||
        (s->resource_state[resource] == STORAGE_STATE_PHARAOH_ACCEPT && s->resource_max_accept[resource] >= QUARTER_GRANARY && amount < QUARTER_GRANARY))
        return true;
    else
        return false;
}
bool building_granary_is_getting(int resource, building *b) {
    const building_storage *s = building_storage_get(b->storage_id);
    int amount = building_granary_get_amount(b, resource);
    if ((s->resource_state[resource] == STORAGE_STATE_PHARAOH_GET && s->resource_max_get[resource] == FULL_GRANARY) ||
        (s->resource_state[resource] == STORAGE_STATE_PHARAOH_GET && s->resource_max_get[resource] >= THREEQUARTERS_GRANARY && amount < THREEQUARTERS_GRANARY) ||
        (s->resource_state[resource] == STORAGE_STATE_PHARAOH_GET && s->resource_max_get[resource] >= HALF_GRANARY && amount < HALF_GRANARY) ||
        (s->resource_state[resource] == STORAGE_STATE_PHARAOH_GET && s->resource_max_get[resource] >= QUARTER_GRANARY && amount < QUARTER_GRANARY))
        return true;
    else
        return false;
}
bool building_granary_is_gettable(int resource, building *b) {
    const building_storage *s = building_storage_get(b->storage_id);
    if (s->resource_state[resource] == STORAGE_STATE_PHARAOH_GET)
        return true;
    else
        return false;
}
int building_granary_is_not_accepting(int resource, building *b) {
    return !((building_granary_is_accepting(resource, b) || building_granary_is_getting(resource, b)));
}

int building_granary_add_resource(building *granary, int resource, int is_produced, int amount) {
    if (granary->id <= 0)
        return -1;

    if (!resource_is_food(resource))
        return -1;

    if (granary->type != BUILDING_GRANARY)
        return -1;

    if (granary->data.granary.resource_stored[RESOURCE_NONE] <= 0)
        return -1; // no space

    if (building_granary_is_not_accepting(resource, granary))
        return -1;

    if (is_produced)
        city_resource_add_produced_to_granary(ONE_LOAD);

    int deliverable_amount = fmin(granary->data.granary.resource_stored[RESOURCE_NONE], amount);
    granary->data.granary.resource_stored[resource] += deliverable_amount;
    granary->data.granary.resource_stored[RESOURCE_NONE] -= deliverable_amount;
    return amount - deliverable_amount;
}
int building_granary_remove_resource(building *granary, int resource, int amount) {
    if (amount <= 0)
        return 0;

    int removed;
    if (granary->data.granary.resource_stored[resource] >= amount)
        removed = amount;
    else {
        removed = granary->data.granary.resource_stored[resource];
    }
    city_resource_remove_from_granary(resource, removed);
    granary->data.granary.resource_stored[resource] -= removed;
    granary->data.granary.resource_stored[RESOURCE_NONE] += removed;
    return amount - removed;
}
int building_granary_remove_for_getting_deliveryman(building *src, building *dst, int *resource) {
    const building_storage *s_src = building_storage_get(src->storage_id);
    const building_storage *s_dst = building_storage_get(dst->storage_id);

    int max_amount = 0;
    int max_resource = 0;
    if (building_granary_is_getting(RESOURCE_GRAIN, dst) && !building_granary_is_gettable(RESOURCE_GRAIN, src)) {
        if (src->data.granary.resource_stored[RESOURCE_GRAIN] > max_amount) {
            max_amount = src->data.granary.resource_stored[RESOURCE_GRAIN];
            max_resource = RESOURCE_GRAIN;
        }
    }
    if (building_granary_is_getting(RESOURCE_MEAT, dst) &&
        !building_granary_is_gettable(RESOURCE_MEAT, src)) {
        if (src->data.granary.resource_stored[RESOURCE_MEAT] > max_amount) {
            max_amount = src->data.granary.resource_stored[RESOURCE_MEAT];
            max_resource = RESOURCE_MEAT;
        }
    }
    if (building_granary_is_getting(RESOURCE_LETTUCE, dst) && !building_granary_is_gettable(RESOURCE_LETTUCE, src)) {
        if (src->data.granary.resource_stored[RESOURCE_LETTUCE] > max_amount) {
            max_amount = src->data.granary.resource_stored[RESOURCE_LETTUCE];
            max_resource = RESOURCE_LETTUCE;
        }
    }
    if (building_granary_is_getting(RESOURCE_FIGS, dst) && !building_granary_is_gettable(RESOURCE_FIGS, src)) {
        if (src->data.granary.resource_stored[RESOURCE_FIGS] > max_amount) {
            max_amount = src->data.granary.resource_stored[RESOURCE_FIGS];
            max_resource = RESOURCE_FIGS;
        }
    }

    if (config_get(CONFIG_GP_CH_GRANARIES_GET_DOUBLE)) {
        if (max_amount > 1600)
            max_amount = 1600;

    } else {
        if (max_amount > 800)
            max_amount = 800;

    }
    if (max_amount > dst->data.granary.resource_stored[RESOURCE_NONE])
        max_amount = dst->data.granary.resource_stored[RESOURCE_NONE];

    building_granary_remove_resource(src, max_resource, max_amount);
    *resource = max_resource;
//    if (GAME_ENV = ENGINE_ENV_PHARAOH)
//        return max_amount;
    return max_amount / UNITS_PER_LOAD;
}

int building_granary_determine_worker_task(building *granary) {
    int pct_workers = calc_percentage(granary->num_workers, model_get_building(granary->type)->laborers);
    if (pct_workers < 50)
        return GRANARY_TASK_NONE;

    const building_storage *s = building_storage_get(granary->storage_id);
    if (s->empty_all) {
        // bring food to another granary
        for (int i = RESOURCE_MIN_FOOD; i < RESOURCES_FOODS_MAX; i++) {
            if (granary->data.granary.resource_stored[i])
                return i;

        }
        return GRANARY_TASK_NONE;
    }
    if (granary->data.granary.resource_stored[RESOURCE_NONE] <= 0)
        return GRANARY_TASK_NONE; // granary full, nothing to get

    if (building_granary_is_getting(RESOURCE_GRAIN, granary) && non_getting_granaries.total_storage_wheat > ONE_LOAD)
        return GRANARY_TASK_GETTING;

    if (building_granary_is_getting(RESOURCE_MEAT, granary) &&
        non_getting_granaries.total_storage_vegetables > ONE_LOAD)
        return GRANARY_TASK_GETTING;

    if (building_granary_is_getting(RESOURCE_LETTUCE, granary) && non_getting_granaries.total_storage_fruit > ONE_LOAD)
        return GRANARY_TASK_GETTING;

    if (building_granary_is_getting(RESOURCE_FIGS, granary) && non_getting_granaries.total_storage_meat > ONE_LOAD)
        return GRANARY_TASK_GETTING;

    return GRANARY_TASK_NONE;
}
void building_granaries_calculate_stocks(void) {
    non_getting_granaries.num_items = 0;
    for (int i = 0; i < MAX_GRANARIES; i++) {
        non_getting_granaries.building_ids[i] = 0;
    }
    non_getting_granaries.total_storage_wheat = 0;
    non_getting_granaries.total_storage_vegetables = 0;
    non_getting_granaries.total_storage_fruit = 0;
    non_getting_granaries.total_storage_meat = 0;

    for (int i = 1; i < MAX_BUILDINGS; i++) {
        building *b = building_get(i);
        if (b->state != BUILDING_STATE_VALID || b->type != BUILDING_GRANARY)
            continue;

        if (!b->has_road_access || b->distance_from_entry <= 0)
            continue;

        const building_storage *s = building_storage_get(b->storage_id);
        int total_non_getting = 0;
        if (!building_granary_is_gettable(RESOURCE_GRAIN, b)) {
            total_non_getting += b->data.granary.resource_stored[RESOURCE_GRAIN];
            non_getting_granaries.total_storage_wheat += b->data.granary.resource_stored[RESOURCE_GRAIN];
        }
        if (!building_granary_is_gettable(RESOURCE_MEAT, b)) {
            total_non_getting += b->data.granary.resource_stored[RESOURCE_MEAT];
            non_getting_granaries.total_storage_vegetables += b->data.granary.resource_stored[RESOURCE_MEAT];
        }
        if (!building_granary_is_gettable(RESOURCE_LETTUCE, b)) {
            total_non_getting += b->data.granary.resource_stored[RESOURCE_LETTUCE];
            non_getting_granaries.total_storage_fruit += b->data.granary.resource_stored[RESOURCE_LETTUCE];
        }
        if (!building_granary_is_gettable(RESOURCE_FIGS, b)) {
            total_non_getting += b->data.granary.resource_stored[RESOURCE_FIGS];
            non_getting_granaries.total_storage_meat += b->data.granary.resource_stored[RESOURCE_FIGS];
        }
        if (total_non_getting > MAX_GRANARIES) {
            non_getting_granaries.building_ids[non_getting_granaries.num_items] = i;
            if (non_getting_granaries.num_items < MAX_GRANARIES - 2)
                non_getting_granaries.num_items++;

        }
    }
}
int building_granary_for_storing(int x, int y, int resource, int distance_from_entry, int road_network_id, int force_on_stockpile, int *understaffed, map_point *dst) {
    if (scenario_property_rome_supplies_wheat())
        return 0;

    if (!resource_is_food(resource))
        return 0;

    if (city_resource_is_stockpiled(resource) && !force_on_stockpile)
        return 0;

    int min_dist = INFINITE;
    int min_building_id = 0;
    for (int i = 1; i < MAX_BUILDINGS; i++) {
        building *b = building_get(i);
        if (b->state != BUILDING_STATE_VALID || b->type != BUILDING_GRANARY)
            continue;

        if (!b->has_road_access || b->distance_from_entry <= 0 || b->road_network_id != road_network_id)
            continue;

        if (!config_get(CONFIG_GP_CH_UNDERSTAFFED_ACCEPT_GOODS)) {
            int pct_workers = calc_percentage(b->num_workers, model_get_building(b->type)->laborers);
            if (pct_workers < 75) {
                if (understaffed)
                    *understaffed += 1;
                continue;
            }
        }
        const building_storage *s = building_storage_get(b->storage_id);
        if (building_granary_is_not_accepting(resource, b) || s->empty_all)
            continue;

        if (config_get(CONFIG_GP_CH_DELIVER_ONLY_TO_ACCEPTING_GRANARIES)) {
            if (building_granary_is_getting(resource, b))
                continue;
        }

        if (b->data.granary.resource_stored[RESOURCE_NONE] >= ONE_LOAD) {
            // there is room
            int dist = calc_distance_with_penalty(b->tile.x() + 1, b->tile.y() + 1, x, y, distance_from_entry,
                                                  b->distance_from_entry);
            if (dist < min_dist) {
                min_dist = dist;
                min_building_id = i;
            }
        }
    }
    // deliver to center of granary
    building *min = building_get(min_building_id);
    map_point_store_result(min->tile.x() + 1, min->tile.y() + 1, dst);
    return min_building_id;
}
int building_getting_granary_for_storing(int x, int y, int resource, int distance_from_entry, int road_network_id, map_point *dst) {
    if (scenario_property_rome_supplies_wheat())
        return 0;

    if (!resource_is_food(resource))
        return 0;

    if (city_resource_is_stockpiled(resource))
        return 0;

    int min_dist = INFINITE;
    int min_building_id = 0;
    for (int i = 1; i < MAX_BUILDINGS; i++) {
        building *b = building_get(i);
        if (b->state != BUILDING_STATE_VALID || b->type != BUILDING_GRANARY)
            continue;

        if (!b->has_road_access || b->distance_from_entry <= 0 || b->road_network_id != road_network_id)
            continue;

        int pct_workers = calc_percentage(b->num_workers, model_get_building(b->type)->laborers);
        if (pct_workers < 100)
            continue;

        const building_storage *s = building_storage_get(b->storage_id);
        if (building_granary_is_getting(resource, b) || s->empty_all)
            continue;

        if (b->data.granary.resource_stored[RESOURCE_NONE] > ONE_LOAD) {
            // there is room
            int dist = calc_distance_with_penalty(b->tile.x() + 1, b->tile.y() + 1, x, y, distance_from_entry,
                                                  b->distance_from_entry);
            if (dist < min_dist) {
                min_dist = dist;
                min_building_id = i;
            }
        }
    }
    building *min = building_get(min_building_id);
    map_point_store_result(min->tile.x() + 1, min->tile.y() + 1, dst);
    return min_building_id;
}
int building_granary_for_getting(building *src, map_point *dst) {
    const building_storage *s_src = building_storage_get(src->storage_id);
    if (s_src->empty_all)
        return 0;

    if (scenario_property_rome_supplies_wheat())
        return 0;

    int is_getting = 0;
    if (building_granary_is_getting(RESOURCE_GRAIN, src) ||
        building_granary_is_getting(RESOURCE_MEAT, src) ||
        building_granary_is_getting(RESOURCE_LETTUCE, src) ||
        building_granary_is_getting(RESOURCE_FIGS, src)) {
        is_getting = 1;
    }
    if (is_getting <= 0)
        return 0;


    int min_dist = INFINITE;
    int min_building_id = 0;
    for (int i = 0; i < non_getting_granaries.num_items; i++) {
        building *b = building_get(non_getting_granaries.building_ids[i]);
        if (!config_get(CONFIG_GP_CH_GETTING_GRANARIES_GO_OFFROAD)) {
            if (b->road_network_id != src->road_network_id)
                continue;

        }
        const building_storage *s = building_storage_get(b->storage_id);
        int amount_gettable = 0;
        if ((building_granary_is_getting(RESOURCE_GRAIN, src)) &&
            !building_granary_is_gettable(RESOURCE_GRAIN, b)) {
            amount_gettable += b->data.granary.resource_stored[RESOURCE_GRAIN];
        }
        if ((building_granary_is_getting(RESOURCE_MEAT, src)) &&
            !building_granary_is_gettable(RESOURCE_MEAT, b)) {
            amount_gettable += b->data.granary.resource_stored[RESOURCE_MEAT];
        }
        if ((building_granary_is_getting(RESOURCE_LETTUCE, src)) &&
            !building_granary_is_gettable(RESOURCE_LETTUCE, b)) {
            amount_gettable += b->data.granary.resource_stored[RESOURCE_LETTUCE];
        }
        if ((building_granary_is_getting(RESOURCE_FIGS, src)) &&
            !building_granary_is_gettable(RESOURCE_FIGS, b)) {
            amount_gettable += b->data.granary.resource_stored[RESOURCE_FIGS];
        }
        if (amount_gettable > 0) {
            int dist = calc_distance_with_penalty(
                    b->tile.x() + 1, b->tile.y() + 1,
                    src->tile.x() + 1, src->tile.y() + 1,
                    src->distance_from_entry, b->distance_from_entry);
            if (amount_gettable <= 400)
                dist *= 2; // penalty for less food

            if (dist < min_dist) {
                min_dist = dist;
                min_building_id = b->id;
            }
        }
    }
    building *min = building_get(min_building_id);
    map_point_store_result(min->tile.x() + 1, min->tile.y() + 1, dst);
    return min_building_id;
}

void building_granary_bless(void) {
    int min_stored = INFINITE;
    building *min_building = 0;
    for (int i = 1; i < MAX_BUILDINGS; i++) {
        building *b = building_get(i);
        if (b->state != BUILDING_STATE_VALID || b->type != BUILDING_GRANARY)
            continue;

        int total_stored = 0;
        for (int r = RESOURCE_MIN_FOOD; r < RESOURCES_FOODS_MAX; r++) {
            total_stored += building_granary_get_amount(b, r);
        }
        if (total_stored < min_stored) {
            min_stored = total_stored;
            min_building = b;
        }
    }
    if (min_building) {
        for (int n = 0; n < 6; n++) {
            building_granary_add_resource(min_building, RESOURCE_GRAIN, 0, 100);
        }
        for (int n = 0; n < 6; n++) {
            building_granary_add_resource(min_building, RESOURCE_MEAT, 0, 100);
        }
        for (int n = 0; n < 6; n++) {
            building_granary_add_resource(min_building, RESOURCE_LETTUCE, 0, 100);
        }
        for (int n = 0; n < 6; n++) {
            building_granary_add_resource(min_building, RESOURCE_FIGS, 0, 100);
        }
    }
}
void building_granary_warehouse_curse(int big) {
    int max_stored = 0;
    building *max_building = 0;
    for (int i = 1; i < MAX_BUILDINGS; i++) {
        building *b = building_get(i);
        if (b->state != BUILDING_STATE_VALID)
            continue;

        int total_stored = 0;
        if (b->type == BUILDING_WAREHOUSE) {
            for (int r = RESOURCE_MIN; r < RESOURCES_MAX; r++) {
                total_stored += building_warehouse_get_amount(b, r);
            }
        } else if (b->type == BUILDING_GRANARY) {
            for (int r = RESOURCE_MIN_FOOD; r < RESOURCES_FOODS_MAX; r++) {
                total_stored += building_granary_get_amount(b, r);
            }
            total_stored /= UNITS_PER_LOAD;
        } else {
            continue;
        }
        if (total_stored > max_stored) {
            max_stored = total_stored;
            max_building = b;
        }
    }
    if (!max_building)
        return;
    if (big) {
        city_message_disable_sound_for_next_message();
        city_message_post(false, MESSAGE_FIRE, max_building->type, max_building->tile.grid_offset());
        building_destroy_by_fire(max_building);
        map_routing_update_land();
    } else {
        if (max_building->type == BUILDING_WAREHOUSE)
            building_warehouse_remove_resource_curse(max_building, CURSE_LOADS);
        else if (max_building->type == BUILDING_GRANARY) {
            int amount = building_granary_remove_resource(max_building, RESOURCE_GRAIN, CURSE_LOADS * UNITS_PER_LOAD);
            amount = building_granary_remove_resource(max_building, RESOURCE_MEAT, amount);
            amount = building_granary_remove_resource(max_building, RESOURCE_LETTUCE, amount);
            building_granary_remove_resource(max_building, RESOURCE_FIGS, amount);
        }
    }
}