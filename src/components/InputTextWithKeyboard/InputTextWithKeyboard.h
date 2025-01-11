#ifndef INPUTTEXTWITHKEYBOARD_H
#define INPUTTEXTWITHKEYBOARD_H

#include <lvgl.h>
#include "../InputText/InputText.h" // Ruta corregida
#include "../Keyboard/Keyboard.h"   // Ruta corregida
#include "../../screens/Screen.h"   // Ruta corregida para Screen.h

class InputTextWithKeyboard {
public:
    InputTextWithKeyboard(lv_obj_t* parent, int maxLength);
    void show();
    void hide();
    bool isVisible() const;
    void setText(const char* text);
    const char* getText() const;
    void setMaxLength(int maxLength);
    int getMaxLength() const;
    void handleButtonEvent(const ButtonState& state, const ButtonChange& change);
    void setOnTextEnteredCallback(std::function<void(const char*)> callback);

private:
    InputText* inputText;
    Keyboard* keyboard;
    bool isKeyboardActive;
    std::function<void(const char*)> onTextEnteredCallback;

    void handleKeyboardInput(const std::string& value);
};

#endif