#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lvgl.h>
#include <vector>
#include <functional>
#include <string>

/**
 * @class Keyboard
 * @brief Implements a virtual keyboard for text input.
 * 
 * This class provides a virtual keyboard with multiple rows of characters, allowing users
 * to select letters, numbers, and symbols using only 3 buttons. It supports two modes: row selection
 * and character selection. The keyboard is displayed on an LVGL screen and can be shown or
 * hidden dynamically.
 */
class Keyboard {
public:
    enum class Mode {
        SelectRow, // Mode for selecting a row of characters
        SelectLetter // Mode for selecting a character within a row
    };

    Keyboard(lv_obj_t* parent); // Constructor
    void show(); // Show the keyboard
    void hide(); // Hide the keyboard
    bool isVisible() const; // Check if the keyboard is visible
    void handleButtonEvent(int button); // Handle button events (e.g., navigation and selection)
    char getSelectedLetter() const; // Get the currently selected letter
    void setOnLetterSelectedCallback(std::function<void(std::string)> callback); // Set callback for letter selection

    bool isSelectingLetter() const; // Check if the keyboard is in character selection mode
    bool isSelectingRow() const; // Check if the keyboard is in row selection mode

private:
    lv_obj_t* keyboardContainer; // LVGL container for the keyboard
    std::vector<std::vector<char>> rows; // Rows of characters in the keyboard
    std::vector<lv_obj_t*> rowContainers; // LVGL containers for each row
    std::vector<std::vector<lv_obj_t*>> letterButtons; // LVGL buttons for each character
    int selectedRowIndex; // Index of the currently selected row
    int selectedLetterIndex; // Index of the currently selected character
    Mode currentMode; // Current mode (row or character selection)
    std::function<void(std::string)> onLetterSelectedCallback; // Callback for letter selection

    int visibleRowStart; // Index of the first visible row
    int visibleRowCount; // Number of visible rows

    void createKeyboard(); // Create the keyboard UI
    void updateSelection(); // Update the visual selection (highlighting)
    void selectRow(); // Switch to row selection mode
    void selectLetter(); // Switch to character selection mode
    void updateVisibleRows(); // Update the visible rows based on the current scroll position
};

#endif