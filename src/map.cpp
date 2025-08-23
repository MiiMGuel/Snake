#include "map.hpp"

#include "raygui.h"

Map::Map(int32_t size, double tr) : size(size), tick_rate(tr) { map.resize(size * size, 0); }
Map::~Map() { }

Vector2 Map::rand_apple_pos() {
    Vector2 vec;
    do {
        vec.x = GetRandomValue(0, size - 1);
        vec.y = GetRandomValue(0, size - 1);
    } while (
        map[vec.x + (vec.y * size)] & HEAD || 
        map[vec.x + (vec.y * size)] & BODY ||
        map[vec.x + (vec.y * size)] & WALL ||
        map[vec.x + (vec.y * size)] & APPLE
    );

    return vec;
}

Vector2 Map::rand_wall_pos() {
    Vector2 vec;
    do {
        vec.x = GetRandomValue(1, size - 2);
        vec.y = GetRandomValue(1, size - 2);
    } while (
        map[vec.x + (vec.y * size)] & HEAD  || 
        map[vec.x + (vec.y * size)] & BODY  ||
        map[vec.x + (vec.y * size)] & APPLE ||
        map[vec.x + (vec.y * size)] & WALL
    );

    return vec;
}

int32_t Map::get_size() const { return size; }

int32_t Map::get_snake_length() { return snake.size(); }

void Map::clear() { map.clear(); }
    
void Map::resize(int32_t size) { 
    this->size = size; 
    if (map.size()) map.clear();
    map.resize(size * size, 0);
}

void Map::add_flag(uint8_t flag, int32_t x, int32_t y) {
    map[x + (y * size)] |= flag;
}

void Map::remove_flag(uint8_t flag, int32_t x, int32_t y) {
    map[x + (y * size)] &= ~flag;
}

void Map::clear_flag(int32_t x, int32_t y) {
    map[x + (y * size)] &= ~0b11111111;
}

void Map::set_snake(int32_t x, int32_t y) {
    if (x + 2 >= size) x = size - 3;

    snake_dir = 0;
    snake_alive = true;
    if (snake.size()) {
        for (auto c : snake) remove_flag(c.id, c.x, c.y);
        snake.clear();
    } snake.emplace_back(Cell{HEAD, x, y}); add_flag(HEAD, x, y);
    snake.emplace_back(Cell{BODY, x + 1, y}); add_flag(BODY, x + 1, y);
    snake.emplace_back(Cell{BODY, x + 2, y}); add_flag(BODY, x + 2, y);
}

void Map::set_apple(int32_t n) {
    if (apple.size()) {
        for (auto a : apple) {
            remove_flag(a.id, a.x, a.y);
        } apple.clear();
    } for (int32_t i = 1; i <= n; i++) {
        Vector2 vec = rand_apple_pos();
        apple.emplace_back(Cell{APPLE, (int32_t)vec.x, (int32_t)vec.y}); 
        add_flag(APPLE, (int32_t)vec.x, (int32_t)vec.y);
    }
}


void Map::set_wall(int32_t n) {
    if (wall.size()) {
        for (auto w : wall) {
            remove_flag(w.id, w.x, w.y);
        } wall.clear();
    }
    for (int32_t i = 1; i <= n; i++) {
        Vector2 vec = rand_wall_pos();
        wall.emplace_back(Cell{WALL, (int32_t)vec.x, (int32_t)vec.y}); 
        add_flag(WALL, (int32_t)vec.x, (int32_t)vec.y);
    }
}

void Map::update(double dt) {
    static double time = 0.0;

    if (snake_alive && next_input) {
        if (IsKeyPressed(KEY_W) && snake_dir != 3) { snake_dir = 1; next_input = false;}
        if (IsKeyPressed(KEY_A) && snake_dir != 2) { snake_dir = 0; next_input = false;}
        if (IsKeyPressed(KEY_S) && snake_dir != 1) { snake_dir = 3; next_input = false;}
        if (IsKeyPressed(KEY_D) && snake_dir != 0) { snake_dir = 2; next_input = false;}

        if (IsKeyPressed(KEY_UP) && snake_dir != 3) { snake_dir = 1; next_input = false;}
        if (IsKeyPressed(KEY_LEFT) && snake_dir != 2) { snake_dir = 0; next_input = false;}
        if (IsKeyPressed(KEY_DOWN) && snake_dir != 1) { snake_dir = 3; next_input = false;}
        if (IsKeyPressed(KEY_RIGHT) && snake_dir != 0) { snake_dir = 2; next_input = false;}
    }

    time += dt;
    if (snake_alive) if (time >= 1.0 / (double)tick_rate) {
        time = 0.0;
        next_input = true;

        for (auto c : snake) remove_flag(c.id, c.x, c.y);

        for (int32_t i = snake.size() - 1; i > 0; i--) {
            if (snake[i].id == BODY) {
                snake[i].x = snake[i - 1].x;
                snake[i].y = snake[i - 1].y;
            } for (auto c : snake) if (c.id == BODY) add_flag(c.id, c.x, c.y);
        }

        switch (snake_dir) {
        case 0: snake[0].x -= 1; break;
        case 1: snake[0].y -= 1; break;
        case 2: snake[0].x += 1; break;
        case 3: snake[0].y += 1; break;
        default: break;
        }

        if (
            border_collision && (
                snake[0].x < 0    || 
                snake[0].x >= size || 
                snake[0].y < 0    || 
                snake[0].y >= size
            )
        ) { snake_alive = false; snake[0].id = EMPTY; }
        else {
            if (snake[0].x < 0) snake[0].x = size - 1;
            else if (snake[0].x >= size ) snake[0].x = 0;
            if (snake[0].y < 0) snake[0].y = size - 1;
            else if (snake[0].y >= size ) snake[0].y = 0;
        }

        if (body_collision && (map[snake[0].x + (snake[0].y * size)] & BODY))
            snake_alive = false;            
        if (map[snake[0].x + (snake[0].y * size)] & WALL)
            snake_alive = false;

        if (map[snake[0].x + (snake[0].y * size)] & APPLE) {
            snake.emplace_back(Cell{BODY, snake[snake.size() - 1].x, snake[snake.size() - 1].y});
            for (auto& a : apple) if (a.x == snake[0].x && a.y == snake[0].y) {
                remove_flag(a.id, a.x, a.y);

                Vector2 vec = rand_apple_pos();
                a.x = vec.x;
                a.y = vec.y;
                add_flag(APPLE, (int32_t)vec.x, (int32_t)vec.y);
            }
        }

        add_flag(snake[0].id, snake[0].x, snake[0].y);
    } else {}
    else time = 0.0;
}

void Map::draw(Rectangle rect) {
    int8_t empty_color = 1;
    for (int32_t y = 0; y < size; y++) { 
        for (int32_t x = 0; x < size; x++) {
            Rectangle box = {
                rect.x + (x * (rect.width / size)),
                rect.y + (y * (rect.height / size)), 
                (rect.width / size), 
                (rect.height / size)
            }; 
            
            Color color;
            if (map[x + (y * size)] & WALL) color = GetColor(GuiGetStyle(BUTTON, BORDER_COLOR_PRESSED));
            else if (map[x + (y * size)] & HEAD) color = GetColor(GuiGetStyle(BUTTON, BORDER_COLOR_FOCUSED));
            else if (map[x + (y * size)] & BODY) color = GetColor(GuiGetStyle(BUTTON, BASE_COLOR_FOCUSED));
            else if (map[x + (y * size)] & APPLE) color = GetColor(GuiGetStyle(BUTTON, BASE_COLOR_PRESSED));
            else if (empty_color == 1) color = GetColor(GuiGetStyle(BUTTON, BASE_COLOR_DISABLED));
            else color = GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_DISABLED));

            DrawRectangleRec(box, color); 
            empty_color *= -1;
        } if ((size % 2) == 0) empty_color *= -1;
    }
}