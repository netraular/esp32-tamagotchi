#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lvgl.h>
#include <vector>

class Keyboard {
public:
    enum class Action {
        None,
        LetterSelected,
        Exit
    };

    Keyboard(lv_obj_t* parent);
    void show();
    void hide();
    bool isVisible() const;
    void handleButtonEvent(int button);
    Action getLastAction() const;
    char getSelectedLetter() const;

private:
    lv_obj_t* keyboardContainer;
    lv_obj_t* label;
    std::vector<std::vector<char>> rows;
    int currentRowIndex;
    int currentLetterIndex;
    bool isSelectingRow;
    Action lastAction;
    char selectedLetter;

    void updateDisplay();
    void selectRow();
    void selectLetter();
};

#endif