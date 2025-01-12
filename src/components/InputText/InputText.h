#ifndef INPUTTEXT_H
#define INPUTTEXT_H

#include <lvgl.h>
#include <vector>
#include <string>

/**
 * @class InputText
 * @brief A component for text input using a grid of boxes.
 * 
 * This class provides a text input interface where each character is entered into a separate box.
 * It supports navigation between boxes, character insertion, and text retrieval. The component is
 * designed to work with LVGL and is suitable for embedded systems with 3 buttons as input methods.
 */
class InputText {
public:
    InputText(lv_obj_t* parent, int maxLength);
    void show();
    void hide();
    bool isVisible() const;
    void setText(const char* text);
    const char* getText() const;
    void setMaxLength(int maxLength);
    int getMaxLength() const;
    lv_obj_t* getContainer() const;
    void setSelectedBox(int index);
    int insertChar(char c);
    int getSelectedBoxIndex() const;
    int moveNext();
    int movePrevious();

private:
    lv_obj_t* container; // LVGL container for the text boxes
    std::vector<lv_obj_t*> boxes; // Vector of LVGL objects representing the text boxes
    int maxLength; // Maximum number of characters allowed
    char* textBuffer; // Buffer to store the entered text
    int selectedBoxIndex; // Index of the currently selected box

    void createBoxes(lv_obj_t* parent);
    void updateBoxes();
    void updateSelection();
    void clearSelection();
};

#endif