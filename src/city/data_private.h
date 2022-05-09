#ifndef CITY_DATA_PRIVATE_H
#define CITY_DATA_PRIVATE_H

#include <stdint.h>

#include "city/emperor.h"
#include "city/finance.h"
#include "city/houses.h"
#include "city/labor.h"
#include "city/resource.h"
#include "map/point.h"

typedef struct {
    uint8_t mood;
    uint8_t target_mood;
    uint8_t wrath_bolts;
    uint8_t happy_ankhs;
    bool blessing_done;
    bool curse_done;
    uint32_t months_since_festival;
    int8_t unused1;
    int8_t unused2;
    int8_t unused3;
    bool is_known;
} god_status;

extern struct city_data_t {
    struct {
        bool senate_placed;
        int32_t senate_building_id;
        map_point senate;

        bool festival_square_placed;
        int32_t festival_building_id;
        map_point festival_square;

        bool mansion_placed;
        int32_t mansion_building_id;
        map_point mansion;

        int32_t hippodrome_placed;

        bool barracks_placed;
        int32_t barracks_building_id;
        map_point barracks;

        bool distribution_center_placed;
        int32_t distribution_center_building_id;
        map_point distribution_center;

        int32_t trade_center_building_id;
        int8_t triumphal_arches_available;
        int8_t triumphal_arches_placed;
        int16_t working_wharfs;
        int32_t shipyard_boats_requested;
        int16_t working_docks;
        int16_t working_dock_ids[10];
        int32_t mission_post_operational;
        map_point main_native_meeting;
        int8_t unknown_value;

        bool temple_complex_placed;
        int32_t temple_complex_id;
    } building;
    struct {
        int16_t animals;
        int32_t attacking_natives;
        int32_t enemies;
        int32_t imperial_soldiers;
        int32_t rioters;
        int32_t soldiers;
        int32_t security_breach_duration;
    } figure;
    house_demands houses;
    struct {
        emperor_gift gifts[3];
        int32_t selected_gift_size;
        int32_t months_since_gift;
        int32_t gift_overdose_penalty;

        int32_t debt_state;
        int32_t months_in_debt;

        int32_t player_rank;
        int32_t salary_rank;
        int32_t salary_amount;
        int32_t donate_amount;
        int32_t personal_savings;
        uint8_t player_name_adversary[32];
        uint8_t player_name[32];
        uint8_t campaign_player_name[32]; /**< Temp storage for carrying over player name to next campaign mission */
        struct {
            int32_t count;
            int32_t size;
            int32_t soldiers_killed;
            int32_t warnings_given;
            int32_t days_until_invasion;
            int32_t duration_day_countdown;
            int32_t retreat_message_shown;
        } invasion;
    } emperor;
    struct {
        uint8_t total_legions;
        uint8_t total_soldiers;
        uint8_t empire_service_legions;
        int32_t legionary_legions;
        int32_t native_attack_duration;
    } military;
    struct {
        uint8_t city;
        int8_t city_foreign_months_left;
        int8_t total_count;
        int8_t won_count;
        uint8_t enemy_strength;
        uint8_t roman_strength;
        int8_t months_until_battle;
        int8_t roman_months_to_travel_forth;
        int8_t roman_months_to_travel_back;
        int8_t enemy_months_traveled;
        int8_t roman_months_traveled;
    } distant_battle;
    struct {
        int32_t treasury;
        int32_t tax_percentage;
        int32_t estimated_tax_uncollected;
        int32_t estimated_tax_income;
        int32_t estimated_wages;
        finance_overview last_year;
        finance_overview this_year;
        int32_t interest_so_far;
        int32_t salary_so_far;
        int32_t wages_so_far;
        int16_t stolen_this_year;
        int16_t stolen_last_year;
        int32_t cheated_money;
        int32_t tribute_not_paid_last_year;
        int32_t tribute_not_paid_total_years;
        int32_t wage_rate_paid_this_year;
        int32_t wage_rate_paid_last_year;
    } finance;
    struct {
        int32_t taxed_plebs;
        int32_t taxed_patricians;
        int32_t untaxed_plebs;
        int32_t untaxed_patricians;
        int32_t percentage_taxed_plebs;
        int32_t percentage_taxed_patricians;
        int32_t percentage_taxed_people;
        struct {
            int32_t collected_plebs;
            int32_t collected_patricians;
            int32_t uncollected_plebs;
            int32_t uncollected_patricians;
        } yearly;
        struct {
            int32_t collected_plebs;
            int32_t collected_patricians;
            int32_t uncollected_plebs;
            int32_t uncollected_patricians;
        } monthly;
    } taxes;
    struct {
        int32_t population;
        int32_t population_last_year;
        int32_t school_age;
        int32_t academy_age;
        int32_t working_age;
        struct {
            int32_t values[2400];
            int32_t next_index;
            int32_t count;
        } monthly;
        int16_t at_age[100];
        int32_t at_level[20];

        int32_t yearly_update_requested;
        int32_t yearly_births;
        int32_t yearly_deaths;
        int32_t lost_removal;
        int32_t lost_homeless;
        int32_t lost_troop_request;
        int32_t last_change;
        int32_t total_all_years;
        int32_t total_years;
        int32_t average_per_year;
        int32_t highest_ever;
        int32_t total_capacity;
        int32_t room_in_houses;

        int32_t people_in_tents;
        int32_t people_in_tents_shacks;
        int32_t people_in_large_insula_and_above;
        int32_t people_in_villas_palaces;
        int32_t percentage_plebs;

        int32_t last_used_house_add;
        int32_t last_used_house_remove;
        int32_t graph_order;
    } population;
    struct {
        int32_t wages;
        int32_t wages_rome;
        int32_t workers_available;
        int32_t workers_employed;
        int32_t workers_unemployed;
        int32_t workers_needed;
        int32_t unemployment_percentage;
        int32_t unemployment_percentage_for_senate;
        labor_category_data categories[10];
    } labor;
    struct {
        int32_t immigration_duration;
        int32_t emigration_duration;
        int32_t immigration_amount_per_batch;
        int32_t emigration_amount_per_batch;
        int32_t immigration_queue_size;
        int32_t emigration_queue_size;
        int32_t immigrated_today;
        int32_t emigrated_today;
        int32_t refused_immigrants_today;
        int32_t no_immigration_cause;
        int32_t percentage;
        int32_t newcomers;
        int32_t emigration_message_shown;
    } migration;
    struct {
        int32_t value;
        int32_t previous_value;
        int32_t message_delay;

        int8_t include_tents;
        int32_t unemployment;
        int32_t wages;
        int32_t low_mood_cause;

        int32_t protesters;
        int32_t criminals; // muggers+rioters
    } sentiment;
    struct {
        int32_t num_hospital_workers;
        int32_t target_value;
        int32_t value;
    } health;
    struct {
        int32_t culture;
        int32_t prosperity;
        int32_t monument; // peace
        int32_t kingdom; // favor
        struct {
            int32_t theater;
            int32_t religion;
            int32_t school;
            int32_t library;
            int32_t academy;
        } culture_points;
        int32_t prosperity_treasury_last_year;
        int32_t prosperity_max;
        int32_t monument_destroyed_buildings;
        int32_t monument_years_of_monument;
        int32_t monument_num_criminals;
        int32_t monument_num_rioters;
        int32_t monument_riot_cause;
        int32_t kingdom_salary_penalty;
        int32_t kingdom_milestone_penalty;
        int32_t kingdom_ignored_request_penalty;
        int32_t kingdom_last_year;
        int32_t kingdom_change; // 0 = dropping, 1 = stalling, 2 = rising

        int32_t selected;
        int32_t culture_explanation;
        int32_t prosperity_explanation;
        int32_t monument_explanation;
        int32_t kingdom_explanation;
    } ratings;
    struct {
        int32_t average_entertainment;
        int32_t average_religion;
        int32_t average_education;
        int32_t average_health;
        int32_t religion_coverage;
    } culture;
    struct {
        god_status gods[5];
        int32_t least_happy_god;
        int32_t angry_message_delay;
        bool venus_curse_active;
        int32_t ra_150_export_profits_months_left;
        int32_t ra_harshly_reduced_trading_months_left;
        int32_t ra_no_traders_months_left;
        int32_t ra_slightly_reduced_trading_months_left;
        int32_t ra_slightly_increased_trading_months_left;
        int32_t neptune_sank_ships;
        int32_t seth_crush_enemy_troops;
        int32_t seth_protect_player_troops;
        bool osiris_double_farm_yield;
        int32_t osiris_flood_will_destroy_active;
    } religion;
    struct {
        int32_t theater_shows;
        int32_t theater_no_shows_weighted;
        int32_t amphitheater_shows;
        int32_t amphitheater_no_shows_weighted;
        int32_t colosseum_shows;
        int32_t colosseum_no_shows_weighted;
        int32_t hippodrome_shows;
        int32_t hippodrome_no_shows_weighted;
        int32_t venue_needing_shows;
        int32_t hippodrome_has_race;
        int32_t hippodrome_message_shown;
        int32_t colosseum_message_shown;
    } entertainment;
    struct {
        struct {
            int32_t months_to_go;
            int32_t god;
            int32_t size;
        } planned;
        struct {
            int32_t god;
            int32_t size;
        } selected;
        int32_t small_cost;
        int32_t large_cost;
        int32_t grand_cost;
        int32_t grand_alcohol;
        int32_t not_enough_alcohol;

        int32_t months_since_festival;
        int32_t first_festival_effect_months;
        int32_t second_festival_effect_months;
    } festival;
    struct {
        int16_t space_in_warehouses[RESOURCES_MAX];
        int16_t stored_in_warehouses[RESOURCES_MAX];
//        int32_t space_in_workshops[6];
//        int32_t stored_in_workshops[6];
        int16_t trade_status[RESOURCES_MAX];
        int16_t trading_amount[RESOURCES_MAX];
        int32_t stockpiled[RESOURCES_MAX];
        int16_t mothballed[RESOURCES_MAX];
        int16_t unk_00[RESOURCES_MAX];

        int32_t wine_types_available;
//        int32_t food_types_available;
//        int32_t food_types_eaten;
        int8_t food_types_available_arr[RESOURCES_FOODS_MAX];
        int8_t food_types_eaten_arr[RESOURCES_FOODS_MAX];
        int8_t food_types_allowed[4];
        int32_t food_types_available_num;
        int32_t food_types_eaten_num;
        int32_t granary_food_stored[RESOURCES_FOODS_MAX];
        int32_t granary_total_stored;
        int32_t food_supply_months;
        int32_t food_needed_per_month;
        int32_t food_consumed_last_month;
        int32_t food_produced_last_month;
        int32_t food_produced_this_month;
        int8_t food_types_arr_unk_00[RESOURCES_FOODS_MAX];
        int8_t food_types_arr_unk_01[RESOURCES_FOODS_MAX];
        struct {
            int operating;
            int not_operating;
            int not_operating_with_food;
            int understaffed;
        } granaries;
        int16_t last_used_warehouse;
    } resource;
    struct {
        int8_t march_enemy;
        int8_t march_horse;
        int8_t march_wolf;
        int8_t shoot_arrow;
        int8_t hit_soldier;
        int8_t hit_spear;
        int8_t hit_club;
        int8_t hit_elephant;
        int8_t hit_axe;
        int8_t hit_wolf;
        int8_t die_citizen;
        int8_t die_soldier;
    } sound;
    struct {
        int16_t num_land_routes;
        int16_t num_sea_routes;
        int16_t land_trade_problem_duration;
        int16_t sea_trade_problem_duration;
        int32_t caravan_import_resource;
        int32_t caravan_backup_import_resource;
        int32_t docker_import_resource;
        int32_t docker_export_resource;
    } trade;
    struct {
        map_point entry_point;
        map_point exit_point;
        map_point entry_flag;
        map_point exit_flag;
        struct {
            int32_t id;
            int32_t size;
        } largest_road_networks[10];
    } map;
    struct {
        int32_t has_won;
        int32_t continue_months_left;
        int32_t continue_months_chosen;
        int32_t fired_message_shown;
        int32_t victory_message_shown;
        int32_t start_saved_game_written;
        int32_t tutorial_fire_message_shown;
        int32_t tutorial_disease_message_shown;
        int32_t tutorial_senate_built;
    } mission;
    struct {
        int8_t other_player[18904];
        int8_t unknown_00a0;
        int8_t unknown_00a1;
        int8_t unknown_00a2;
        int8_t unknown_00a3;
        int8_t unknown_00a4;
        int8_t unknown_00a6;
        int8_t unknown_00a7;
        int32_t unknown_00c0;
        int32_t unused_27d0;
        int32_t unknown_27e0[4];
        int16_t unknown_27f0;
        int16_t unknown_27f4[18];
        int16_t unknown_2828;
        int16_t unused_28ca;
        int8_t unknown_2924[272];
        int32_t unknown_2b6c;
        int32_t unknown_2c20[1400];
        int32_t houses_requiring_unknown_to_evolve[8];
        int32_t unknown_4238[4];
        int32_t unknown_4284;
        int32_t unknown_4294[2];
        int32_t unknown_4334;
        int32_t unknown_4374[2];
        int16_t unknown_439c[3];
        int8_t padding_43b2[2];
        int32_t unknown_43d8[5];
        int32_t unknown_43f0;
        int32_t unused_4454;
        int32_t unknown_446c[4];
        int32_t unused_4488;
        int32_t unused_native_force_attack;
        int32_t unused_44e0[2];
        int32_t unused_44ec;
        int32_t unused_44f8;
        int32_t unused_4524[11];
        uint8_t unknown_458e;
        int8_t unused_45a5[6];
        int8_t unknown_464c[232];
        int32_t unknown_order;
        int32_t faction_id;
        int16_t faction_bytes[2];
    } unused;
} city_data;

const city_data_t * city_data_struct();

#endif // CITY_DATA_PRIVATE_H
