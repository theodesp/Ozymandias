#ifndef MAP_TERRAIN_H
#define MAP_TERRAIN_H

#include "core/buffer.h"

enum {
    TERRAIN_NONE = 0x0,

    TERRAIN_TREE = 0x1,
    TERRAIN_ROCK = 0x2,
    TERRAIN_WATER = 0x4,
    TERRAIN_BUILDING = 0x8,
    TERRAIN_SHRUB = 0x10,
    TERRAIN_GARDEN = 0x20,
    TERRAIN_ROAD = 0x40,
    TERRAIN_GROUNDWATER = 0x80,
    TERRAIN_AQUEDUCT = 0x100,
    TERRAIN_ELEVATION = 0x200,
    TERRAIN_ACCESS_RAMP = 0x400,
    TERRAIN_MEADOW = 0x800,
    TERRAIN_RUBBLE = 0x1000,
    TERRAIN_FOUNTAIN_RANGE = 0x2000,
    TERRAIN_WALL = 0x4000,
    TERRAIN_GATEHOUSE = 0x8000,

    // pharaoh
    TERRAIN_FLOODPLAIN = 0x10000,
        TERRAIN_UNK_00 = 0x20000,
    TERRAIN_REEDS = 0x40000,
        TERRAIN_UNK_01 = 0x80000,
    TERRAIN_ORE = 0x100000,
        TERRAIN_UNK_02 = 0x200000,
        TERRAIN_UNK_03 = 0x400000,
        TERRAIN_UNK_04 = 0x800000,
    TERRAIN_IRRIGATION_RANGE = 0x1000000,
    TERRAIN_DUNE = 0x2000000,
    TERRAIN_DEEPWATER = 0x4000000,
    TERRAIN_SUBMERGED_ROAD = 0x8000000,
        TERRAIN_UNK_07 = 0x10000000,
        TERRAIN_UNK_08 = 0x20000000,
        TERRAIN_UNK_09 = 0x40000000,
        TERRAIN_UNK_10 = 0x80000000,

    // combined
    TERRAIN_ALL = 0xffffffff,
    TERRAIN_WALL_OR_GATEHOUSE = TERRAIN_WALL | TERRAIN_GATEHOUSE,
    TERRAIN_NOT_CLEAR = TERRAIN_TREE
            + TERRAIN_ROCK
            + TERRAIN_WATER
            + TERRAIN_BUILDING
            + TERRAIN_SHRUB
            + TERRAIN_GARDEN
            + TERRAIN_ROAD
              // ... //
            + TERRAIN_AQUEDUCT
            + TERRAIN_ELEVATION
            + TERRAIN_ACCESS_RAMP
              // ... //
            + TERRAIN_RUBBLE
              // ... //
            + TERRAIN_WALL
            + TERRAIN_GATEHOUSE
            + TERRAIN_FLOODPLAIN
              // ... //
            + TERRAIN_REEDS
              // ... //
            + TERRAIN_ORE
              // ... //
            + TERRAIN_DUNE
            + TERRAIN_DEEPWATER
            + TERRAIN_SUBMERGED_ROAD,

    TERRAIN_CLEARABLE = TERRAIN_NOT_CLEAR
            - TERRAIN_ROCK
            - TERRAIN_WATER
            - TERRAIN_ELEVATION
            - TERRAIN_FLOODPLAIN
            - TERRAIN_REEDS
            - TERRAIN_ORE
            - TERRAIN_DUNE
            - TERRAIN_DEEPWATER
            - TERRAIN_SUBMERGED_ROAD,
//    TERRAIN_NOT_REMOVABLE = TERRAIN_ROCK | TERRAIN_FLOODPLAIN | TERRAIN_DUNE,

    TERRAIN_IMPASSABLE = TERRAIN_NOT_CLEAR
            - TERRAIN_ROAD
            - TERRAIN_GATEHOUSE
            - TERRAIN_DEEPWATER
            - TERRAIN_SUBMERGED_ROAD,

    TERRAIN_IMPASSABLE_ENEMY = TERRAIN_IMPASSABLE
            + TERRAIN_GATEHOUSE
            - TERRAIN_BUILDING
            - TERRAIN_FLOODPLAIN,

    TERRAIN_IMPASSABLE_WOLF = TERRAIN_IMPASSABLE
            + TERRAIN_GARDEN
            + TERRAIN_GATEHOUSE
            - TERRAIN_FLOODPLAIN,

    // Pharaoh moisture combinators
    MOISTURE_GRASS = 0x7,
    MOISTURE_TRANSITION = 0x80,
//    MOISTURE_TALLGRASS = 0x40,
//    MOISTURE_SHORE_TIP = 0x24,
    MOISTURE_SHORE_TALLGRASS = 0x64
};

extern int all_river_tiles[];
extern int all_floodplain_tiles[];
extern int all_river_tiles_x[];
extern int all_river_tiles_y[];
extern int river_total_tiles;
extern int floodplain_total_tiles;

typedef struct floodplain_order {
    bool initialized = false;
    uint32_t *offsets;
    uint32_t amount;
} floodplain_order;

extern floodplain_order floodplain_offsets[30];

bool map_terrain_is(int grid_offset, int terrain);

int map_terrain_get(int grid_offset);

void map_terrain_set(int grid_offset, int terrain);

void map_terrain_add(int grid_offset, int terrain);

void map_terrain_remove(int grid_offset, int terrain);

void map_terrain_add_in_area(int x_min, int y_min, int x_max, int y_max, int terrain);

void map_terrain_add_with_radius(int x, int y, int size, int radius, int terrain);

void map_terrain_remove_with_radius(int x, int y, int size, int radius, int terrain);

void map_terrain_remove_all(int terrain);

int map_terrain_count_directly_adjacent_with_type(int grid_offset, int terrain);

int map_terrain_count_diagonally_adjacent_with_type(int grid_offset, int terrain);

bool map_terrain_has_adjecent_with_type(int grid_offset, int terrain);
bool map_terrain_has_adjacent_y_with_type(int grid_offset, int terrain);
bool map_terrain_has_adjacent_x_with_type(int grid_offset, int terrain);

bool map_terrain_exists_tile_in_area_with_type(int x, int y, int size, int terrain);

bool map_terrain_exists_tile_in_radius_with_type(int x, int y, int size, int radius, int terrain);
bool map_terrain_exists_tile_in_radius_with_exact(int x, int y, int size, int radius, int terrain);

bool map_terrain_exists_clear_tile_in_radius(int x, int y, int size, int radius, int except_grid_offset,
                                             int *x_tile, int *y_tile);

bool map_terrain_all_tiles_in_area_are(int x, int y, int size, int terrain);

bool map_terrain_all_tiles_in_radius_are(int x, int y, int size, int radius, int terrain);

bool map_terrain_has_only_rocks_trees_in_ring(int x, int y, int distance);

bool map_terrain_has_only_meadow_in_ring(int x, int y, int distance);

bool map_terrain_is_adjacent_to_wall(int x, int y, int size);

bool map_terrain_is_adjacent_to_water(int x, int y, int size);

bool map_terrain_is_adjacent_to_open_water(int x, int y, int size);

bool map_terrain_get_adjacent_road_or_clear_land(int x, int y, int size, int *x_tile, int *y_tile);

void map_terrain_add_roadblock_road(int x, int y, int orientation);
void map_terrain_add_gatehouse_roads(int x, int y, int orientation);
void map_terrain_add_triumphal_arch_roads(int x, int y, int orientation);

int map_floodplain_rebuild_shoreorder();
uint8_t map_get_floodplain_shoreorder(int grid_offset);
uint8_t map_get_floodplain_growth(int grid_offset);
uint8_t map_get_fertility(int grid_offset);
uint8_t map_get_fertility_for_farm(int grid_offset);
void map_set_floodplain_growth(int grid_offset, int growth);
void map_soil_depletion(int grid_offset, int malus);

void map_soil_fertility_load_state(buffer *buf);
void map_soil_depletion_load_state(buffer *buf);

void map_terrain_backup(void);
void map_terrain_restore(void);
void map_terrain_clear(void);
void map_terrain_init_outside_map(void);

void map_terrain_save_state(buffer *buf);
void map_terrain_load_state(buffer *buf);

uint8_t  map_moisture_get(int grid_offset);
void map_moisture_load_state(buffer *buf);
uint8_t map_grasslevel_get(int grid_offset);
bool map_is_4x4_tallgrass(int x, int y, int grid_offset);

//void map_GRID03_32BIT_load_split_state(buffer *buf);
//int8_t map_GRID03_32BIT_get_byte(int grid_offset, int a);

void map_temp_grid_load(buffer *buf, int g);
int64_t map_temp_grid_get(int grid_offset, int g);

#endif // MAP_TERRAIN_H
