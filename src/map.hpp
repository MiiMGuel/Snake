#ifndef MAP_HPP
#define MAP_HPP

#include <vector>

#include "raylib.h"

enum CellId {
    EMPTY  = (uint8_t)0b00000000,
    HEAD   = (uint8_t)0b00000001,
    BODY   = (uint8_t)0b00000010,
    APPLE  = (uint8_t)0b00000100,
    WALL   = (uint8_t)0b00001000
};

struct Cell {
    CellId id;
    int32_t x;
    int32_t y;
};

class Map {
private:
    std::vector<uint8_t> map;
    std::vector<Cell> snake;
    std::vector<Cell> apple;
    std::vector<Cell> wall;
    int32_t size = 0;
    int32_t snake_dir = 0;
    bool next_input = true;

    Vector2 rand_apple_pos();
    Vector2 rand_wall_pos();
public:
    bool body_collision = true;
    bool border_collision = true;
    double tick_rate = 4.0;
    bool snake_alive = true;
    
    Map(int32_t size, double tr);
    ~Map();
    int32_t get_size() const;
    int32_t get_snake_length();
    void clear();    
    void resize(int32_t size);
    void add_flag(uint8_t flag, int32_t x, int32_t y);
    void remove_flag(uint8_t flag, int32_t x, int32_t y);
    void clear_flag(int32_t x, int32_t y);
    void set_snake(int32_t x, int32_t y);
    void set_apple(int32_t n);
    void set_wall(int32_t n);
    void update(double dt);
    void draw(Rectangle rect);
};

#endif // map_hpp