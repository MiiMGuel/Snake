#include "input_box.hpp"

InputBox::InputBox(InputType type, int32_t max_char, Rectangle rect)
: type(type), max_char(max_char), rect(rect) { }

InputBox::~InputBox() { }

void InputBox::update() {
    if (CheckCollisionPointRec(GetMousePosition(), rect)) focus = true;
    else focus = false;

    if (focus) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);

        int32_t key = GetCharPressed();

        while (key > 0) {
            if (type == TEXT && key >= 32 && key <= 125 && text.size() < max_char) {
                text = text + (char)key;
            } else if (type == NUMBER && key >= '0' && key <= '9' && text.size() < max_char) {
                text = text + (char)key;
            }
            
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (text.size() > 0) text.erase(text.size() - 1, 1);
        }
    } else SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

void InputBox::draw() {
    DrawRectangleRec(rect, LIGHTGRAY);
    if (focus) DrawRectangleLines((int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height, RED);
    else DrawRectangleLines((int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height, DARKGRAY);

    DrawText(text.c_str(), (int)rect.x + 5, (int)rect.y + 8, (int)rect.height - 10, MAROON);
}


std::string InputBox::str() {
    if (type == TEXT) return text;
    else return std::string();
}

const char* InputBox::c_str() {
    if (type == TEXT) return text.c_str();
    else return std::string().c_str();
}

int32_t InputBox::num() {
    if (type == TEXT) return std::stoi(text);
    else return 0;
}