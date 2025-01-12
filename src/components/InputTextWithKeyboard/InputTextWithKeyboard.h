#ifndef INPUTTEXTWITHKEYBOARD_H
#define INPUTTEXTWITHKEYBOARD_H

#include <lvgl.h>
#include "../InputText/InputText.h"
#include "../Keyboard/Keyboard.h"
#include "../../screens/Screen.h"

/**
 * @class InputTextWithKeyboard
 * @brief A combined component for text input with an integrated virtual keyboard.
 * 
 * This class combines the `InputText` component (a grid of character boxes) with a `Keyboard` component
 * (a virtual keyboard for character selection). It allows users to enter text using 3 buttons,
 * making it suitable for embedded systems with constrained input methods.
 */
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
    void setInputPosition(int x_offset, int y_offset);

private:
    InputText* inputText; // The text input component
    Keyboard* keyboard;   // The virtual keyboard component
    bool isKeyboardActive; // Indicates whether the keyboard is currently active
    std::function<void(const char*)> onTextEnteredCallback; // Callback for when text is entered

    void handleKeyboardInput(const std::string& value); // Handles input from the keyboard
};

#endif