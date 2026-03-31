#pragma once

enum GameState
{
    HOMESCREEN,
    SIMULATION
};

enum CellEvent
{
    DEAD,
    ALIVE_SURVIVED,
    ALIVE_BORN,
    ALIVE_DIED,
};

enum ColorWheel
{
    DEAD_C,
    SURVIVE_C,
    BORN_C,
    DIED_C,
    NEIGH_C,
    NEIGH_CENTER_C,
    NEIGH_SECONDARY_C,
    NEIGH_SECONDARY_CENTER_C,
    RULES_C,
    RULES_SECONDARY_C,
    COLOR_COUNT
};

enum ButtonTypes
{
    SIM_START_STOP,
    SIM_RESET_EMPTY,
    SIM_RESET_FULL,
    SIM_RESET_RANDOM,

    NEIGH_RESET_EMPTY,
    NEIGH_RESET_FULL,
    NEIGH_RESET_RANDOM,

    SIM_PTR_SIZE_UP,
    SIM_PTR_SIZE_DOWN,
    NEIGH_PTR_SIZE_UP,
    NEIGH_PTR_SIZE_DOWN,

    RULES_DISPLAY_STEP,
    RULES_SECTION_STEP,
    RULES_ADD_RULE,
    RULES_REMOVE_RULE,

    BUTTON_TYPE_COUNT
};

enum RuleSections
{
    RULE_NUM,
    NEIGHBORHOOD_NUM,
    MIN_PERCENT,
    MAX_PERCENT,
    OUTPUT,

    RULE_SECTION_COUNT,
};

struct CellUpdate
{
    int row;
    int col;
    CellEvent state;
};

struct CellRule
{
    float min;
    float max;
    int output;
    int neighAffiliation;
};

struct SpriteInfo
{
    float x;
    float y;
    float width;
    float height;
    float pressedX; // button "pressed" state
    float pressedY;
};
