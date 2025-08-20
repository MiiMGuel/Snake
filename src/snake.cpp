#include <vector>

#include "main.cpp"
#include "raylib.h"
#include "input_box.hpp"

enum CellId {
    EMPTY = (uint8_t)0b00000000,
    HEAD  = (uint8_t)0b00000001,
    BODY  = (uint8_t)0b00000010,
    APPLE = (uint8_t)0b00000100
};

class Map {
private:
    std::vector<uint8_t> map;
    uint32_t size = 0;

public:
    Map(uint32_t size) : size(size) { map.resize(size, 0); }
    ~Map() { }

    void clear(void) { map.clear(); }
    void resize(uint32_t size) { map.resize(size); }
};

class Snake : public App {
private:
    int32_t window_width = 800;
    int32_t window_height = 600;

    // core - timing
    double current_time = 0.0;
    double previous_time = 0.0;
    double delta_time = 0.0;
    double sigma_dt = 0.0;
    uint32_t frame_count = 0;
    uint32_t frame_rate = 0;

    InputBox input_text = InputBox(TEXT, 12, {10, 110, 200, 40});
    InputBox input_number = InputBox(NUMBER, 12, {10, 160, 200, 40});

public:
    Snake() {
        SetConfigFlags(FLAG_VSYNC_HINT);
        InitWindow(
            window_width, 
            window_height, 
            "8-bit snake"
        );
    }

    ~Snake() { 
        CloseWindow();
    }

    void run() override { 
        char text[255];
        while (!WindowShouldClose()) {
            current_time = GetTime();
            delta_time = current_time - previous_time;
            sigma_dt += delta_time;

            input_text.update();
            input_number.update();

            BeginDrawing();
                ClearBackground(WHITE);
                input_text.draw();
                input_number.draw();
                DrawText(TextFormat("frame rate: %d/s", frame_rate), 10, 10, 40, BLACK);
                DrawText(TextFormat("delta time: %0.3fs", delta_time), 10, 60, 40, BLACK);
            EndDrawing();

            if (sigma_dt >= 1.0) {
                sigma_dt = 0.0;
                frame_rate = frame_count;
                frame_count = 0;
            }

            frame_count++;
            previous_time = current_time;
        }
    }
};

App* main_app(int argc, char* argv[]) {
    return new Snake();
}