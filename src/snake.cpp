#include <vector>

#include "main.cpp"
#include "raylib.h"
#include "map.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "dark/style_dark.h"

class Snake : public App {
private:
    int32_t window_width = 800;
    int32_t window_height = 600;

    double current_time = 0.0;
    double previous_time = 0.0;
    double delta_time = 0.0;
    double sigma_dt = 0.0;
    uint32_t frame_count = 0;
    uint32_t frame_rate = 0;
    uint8_t status = 0;
    bool playing = false;

    Map map = Map(15, 4.0);
    int32_t apple_count = 1;
    int32_t wall_count = 0;

public:
    Snake() {
        SetConfigFlags(FLAG_VSYNC_HINT);
        InitWindow(
            window_width, 
            window_height, 
            "8-bit snake"
        ); 

        GuiLoadStyleDark();
        GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
        
        map.set_snake(map.get_size() / 2, map.get_size() / 2);
        map.set_apple(apple_count);
        map.set_wall(wall_count);
        map.body_collision = true;
        map.border_collision = true;
    }

    ~Snake() { 
        CloseWindow();
    }

    void run() override {
        int32_t text_width = 0;
        while (!WindowShouldClose()) {
            current_time = GetTime();
            delta_time = current_time - previous_time;
            sigma_dt += delta_time;

            switch (status) {
            case 0: // paused
                if (IsKeyPressed(KEY_SPACE)) {
                    playing = true;
                    status = 1;
                } else if (IsKeyPressed(KEY_R)) {
                    map.set_snake(map.get_size() / 2, map.get_size() / 2);
                    map.set_apple(apple_count);
                    map.set_wall(wall_count);
                    playing = false;
                } break;
            
            case 1: // playing
                if (IsKeyPressed(KEY_SPACE)) status = 0;

                map.update(delta_time);
                if (map.snake_alive == false) {
                    status = 2;
                } break;

            case 2: // game over
                if (IsKeyPressed(KEY_SPACE)) {
                    map.set_snake(map.get_size() / 2, map.get_size() / 2);
                    map.set_apple(apple_count);
                    map.set_wall(wall_count);
                    playing = false;
                    status = 0;
                } break;
            }

            BeginDrawing();
                ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
                map.draw({200, 0, 600, 600});
                
                if (playing) GuiDisable(); else GuiEnable();
                GuiCheckBox({10, 10, 20, 20}, "Body Collision", &map.body_collision);
                GuiCheckBox({10, 40, 20, 20}, "Border Collision", &map.border_collision);
                
                if (GuiSpinner({10, 70, 180, 20}, "", &apple_count, 1, 10, false)) map.set_apple(apple_count);
                GuiLabel({10, 90, 180, 20}, "Apple Count");
                if (GuiSpinner({10, 120, 180, 20}, "", &wall_count, 0, 10, false)) map.set_wall(wall_count);
                GuiLabel({10, 140, 180, 20}, "Wall Count");
                GuiSpinner({10, 170, 180, 20}, "", &map.tick_rate, 1, 20, false);
                GuiLabel({10, 190, 180, 20}, "Tick rate");
                
                GuiEnable();
                GuiLabel({10, 250, 180, 20}, "Key Bindings:");
                if (status == 0) GuiLabel({10, 280, 180, 20}, "SPACE - start");
                else if (status == 1) GuiLabel({10, 280, 180, 20}, "SPACE - pause");
                else GuiLabel({10, 280, 180, 20}, "SPACE - play again");
                if (status != 0) GuiDisable();
                GuiLabel({10, 310, 180, 20}, "R - reset");
                if (status != 1) GuiDisable(); else GuiEnable();
                GuiLabel({10, 340, 180, 20}, "W - move up");
                GuiLabel({10, 370, 180, 20}, "A - move left");
                GuiLabel({10, 400, 180, 20}, "S - move down");
                GuiLabel({10, 430, 180, 20}, "D - move right");
                
                GuiEnable();
                GuiLabel({10, 220, 180, 20}, TextFormat("Score: %d", map.get_snake_length() - 3));
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