#include <vector>

#include "main.cpp"
#include "raylib.h"
#include "map.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "amber/style_amber.h"
#include "ashes/style_ashes.h"
#include "bluish/style_bluish.h"
#include "candy/style_candy.h"
#include "cherry/style_cherry.h"
#include "cyber/style_cyber.h"
#include "dark/style_dark.h"
#include "enefete/style_enefete.h"
#include "genesis/style_genesis.h"
#include "jungle/style_jungle.h"
#include "lavanda/style_lavanda.h"
#include "rltech/style_rltech.h"
#include "sunny/style_sunny.h"
#include "terminal/style_terminal.h"

#define THEMES_COUNT 15 // including default

class Snake : public App {
private:
    int32_t window_width = 1000;
    int32_t window_height = 700;
    int32_t theme = 9;

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

    void apply_theme() {
        GuiLoadStyleDefault();
        switch (theme) {
        case 0: GuiLoadStyleAmber(); break;
        case 1: GuiLoadStyleAshes(); break;
        case 2: GuiLoadStyleBluish(); break;
        case 3: GuiLoadStyleCandy(); break;
        case 4: GuiLoadStyleCherry(); break;
        case 5: GuiLoadStyleCyber(); break;
        case 6: GuiLoadStyleDark(); break;
        case 7: GuiLoadStyleDefault(); break;
        case 8: GuiLoadStyleEnefete(); break;
        case 9: GuiLoadStyleGenesis(); break;
        case 10: GuiLoadStyleJungle(); break;
        case 11: GuiLoadStyleLavanda(); break;
        case 12: GuiLoadStyleRLTech(); break;
        case 13: GuiLoadStyleSunny(); break;
        case 14: GuiLoadStyleTerminal(); break;
        
        default:
            break;
        } GuiSetStyle(DEFAULT, TEXT_SIZE, 32);
    }
public:
    Snake() {
        SetConfigFlags(FLAG_VSYNC_HINT);
        InitWindow(
            window_width, 
            window_height, 
            "8-bit snake"
        ); 

        apply_theme();
        
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
        bool on_drop = false;
        const char* options = "amber;ashes;bluish;candy;cherry;cyber;dark;default;enefete;genesis;jungle;lavanda;rltech;sunny;terminal";
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
                
                if (playing) GuiDisable(); else GuiEnable();
                GuiCheckBox({10, 52, 32, 32}, "Body Collision", &map.body_collision);
                GuiCheckBox({10, 94, 32, 32}, "Border Collision", &map.border_collision);
                
                if (GuiSpinner({10, 146, 280, 32}, "", &apple_count, 1, 10, false)) map.set_apple(apple_count);
                GuiLabel({10, 178, 280, 32}, "Apple Count");
                if (GuiSpinner({10, 220, 280, 32}, "", &wall_count, 0, 10, false)) map.set_wall(wall_count);
                GuiLabel({10, 252, 280, 32}, "Wall Count");
                GuiSpinner({10, 294, 280, 32}, "", &map.tick_rate, 1, 20, false);
                GuiLabel({10, 326, 280, 32}, "Tick rate");
                
                GuiEnable();
                GuiLabel({10, 410, 280, 32}, "Key Bindings:");
                if (status == 0) GuiLabel({10, 452, 280, 32}, "SPACE - start");
                else if (status == 1) GuiLabel({10, 452, 280, 32}, "SPACE - pause");
                else GuiLabel({10, 452, 280, 32}, "SPACE - play again");
                if (status != 0) GuiDisable();
                GuiLabel({10, 484, 280, 32}, "R - reset");
                if (status != 1) GuiDisable(); else GuiEnable();
                GuiLabel({10, 526, 280, 32}, "W - move up");
                GuiLabel({10, 568, 280, 32}, "A - move left");
                GuiLabel({10, 610, 280, 32}, "S - move down");
                GuiLabel({10, 652, 280, 32}, "D - move right");
                
                GuiEnable();
                GuiLabel({10, 368, 280, 32}, TextFormat("Score: %d", map.get_snake_length() - 3));
                if (GuiDropdownBox({10, 10, 280, 32}, options, &theme, on_drop)) {
                    on_drop = !on_drop;
                    apply_theme();
                } map.draw({300, 0, 700, 700});
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