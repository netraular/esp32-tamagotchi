#include "Keyboard.h"
#include <string>
#include <Arduino.h>

/**
 * @brief Constructor for the Keyboard class.
 * 
 * Initializes the keyboard with a parent LVGL object and sets up the UI elements.
 * 
 * @param parent Parent LVGL object (usually the screen or a container).
 */
Keyboard::Keyboard(lv_obj_t* parent) : selectedRowIndex(0), selectedLetterIndex(0), currentMode(Mode::SelectRow), visibleRowStart(0), visibleRowCount(3) {
    keyboardContainer = lv_obj_create(parent);
    lv_obj_set_size(keyboardContainer, 128, 70);
    lv_obj_align(keyboardContainer, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_scrollbar_mode(keyboardContainer, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_all(keyboardContainer, 0, 0); // No padding

    // Define the rows of characters
    rows = {
        {'A', 'B', 'C', 'D', 'E', 'F'},
        {'G', 'H', 'I', 'J', 'K', 'L'},
        {'M', 'N', 'O', 'P', 'Q', 'R'},
        {'S', 'T', 'U', 'V', 'W', 'X'},
        {'Y', 'Z', '_', '_', '_', '_'},
        {'a', 'b', 'c', 'd', 'e', 'f'},
        {'g', 'h', 'i', 'j', 'k', 'l'},
        {'m', 'n', 'o', 'p', 'q', 'r'},
        {'s', 't', 'u', 'v', 'w', 'x'},
        {'y', 'z', '_', '_', '_', '_'},
        {'0', '1', '2', '3', '4', '5'},
        {'6', '7', '8', '9', '_', '_'},
        {'!', '?', '@', '$', '%', '*'},
        {'&', '^', '<', '>', '+', '-'},
        {'(', ')', '[', ']', '{', '}'},
        {'/', '#', ',', '.', ';', ':'}
    };

    // Create the keyboard UI
    createKeyboard();
    updateSelection();

    Serial.println("Keyboard initialized.");
}

/**
 * @brief Creates the keyboard UI.
 * 
 * This function sets up the rows and buttons for the keyboard, including their layout and styling.
 */
void Keyboard::createKeyboard() {
    const int buttonSize = 16;
    const int spacing = 2;
    const int rowHeight = 20;
    const int rowWidth = 108;

    for (int i = 0; i < visibleRowCount; i++) {
        lv_obj_t* rowContainer = lv_obj_create(keyboardContainer);
        lv_obj_set_size(rowContainer, rowWidth, rowHeight);
        lv_obj_align(rowContainer, LV_ALIGN_TOP_LEFT, 8, i * rowHeight + 5);

        lv_obj_set_style_bg_color(rowContainer, lv_color_hex(0xF5F5DC), 0);
        lv_obj_set_style_border_color(rowContainer, lv_color_hex(0x808080), 0);
        lv_obj_set_style_border_width(rowContainer, 1, 0);
        lv_obj_set_style_pad_all(rowContainer, 0, 0);
        lv_obj_set_scrollbar_mode(rowContainer, LV_SCROLLBAR_MODE_OFF);

        std::vector<lv_obj_t*> buttons;
        for (int j = 0; j < 6; j++) {
            lv_obj_t* btn = lv_btn_create(rowContainer);
            lv_obj_set_size(btn, buttonSize, buttonSize);
            lv_obj_align(btn, LV_ALIGN_TOP_LEFT, j * (buttonSize + spacing), 0);

            lv_obj_set_style_bg_color(btn, lv_color_hex(0xF5F5DC), 0);
            lv_obj_set_style_border_color(btn, lv_color_hex(0x808080), 0);
            lv_obj_set_style_border_width(btn, 1, 0);
            lv_obj_set_style_border_side(btn, LV_BORDER_SIDE_LEFT, 0);
            lv_obj_set_style_border_side(btn, LV_BORDER_SIDE_RIGHT, 0);

            lv_obj_t* label = lv_label_create(btn);
            lv_label_set_text(label, "");
            lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
            lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0);
            lv_obj_center(label);

            buttons.push_back(btn);
        }

        rowContainers.push_back(rowContainer);
        letterButtons.push_back(buttons);
    }

    updateVisibleRows();
    Serial.println("Keyboard created successfully.");
}

/**
 * @brief Updates the visible rows of the keyboard.
 * 
 * This function ensures that only the currently visible rows are displayed, based on the
 * scroll position.
 */
void Keyboard::updateVisibleRows() {
    for (int i = 0; i < visibleRowCount; i++) {
        int rowIndex = visibleRowStart + i;
        if (rowIndex >= rows.size()) {
            break;
        }

        for (int j = 0; j < 6; j++) {
            lv_obj_t* label = lv_obj_get_child(letterButtons[i][j], 0);
            if (rows[rowIndex][j] != '\0') {
                lv_label_set_text(label, std::string(1, rows[rowIndex][j]).c_str());
            } else {
                lv_label_set_text(label, "");
            }
        }
    }
}

/**
 * @brief Handles button events for keyboard navigation and selection.
 * 
 * This function processes button presses to navigate between rows and characters, select
 * a character, or exit the keyboard.
 * 
 * @param button The button that was pressed (1, 2, or 3).
 */
void Keyboard::handleButtonEvent(int button) {
    if (currentMode == Mode::SelectRow) {
        if (button == 1) {
            // Move to the next row
            selectedRowIndex = (selectedRowIndex + 1) % rows.size();
            if (selectedRowIndex >= visibleRowStart + visibleRowCount) {
                visibleRowStart = selectedRowIndex - visibleRowCount + 1;
                updateVisibleRows();
            } else if (selectedRowIndex < visibleRowStart) {
                visibleRowStart = selectedRowIndex;
                updateVisibleRows();
            }
            Serial.printf("Row selected: %d\n", selectedRowIndex);
        } else if (button == 2) {
            // Enter character selection mode
            selectLetter();
            Serial.println("Character selection mode activated");
        } else if (button == 3) {
            // Exit the keyboard
            Serial.println("Button 3 pressed: Exiting keyboard");
            if (onLetterSelectedCallback) {
                onLetterSelectedCallback("exit");
            }
            hide();
        }
    } else if (currentMode == Mode::SelectLetter) {
        if (button == 1) {
            // Move to the next character
            selectedLetterIndex = (selectedLetterIndex + 1) % rows[selectedRowIndex].size();
            Serial.printf("Character selected: %c\n", rows[selectedRowIndex][selectedLetterIndex]);
        } else if (button == 2) {
            // Select the current character
            char selectedLetter = rows[selectedRowIndex][selectedLetterIndex];
            if (selectedLetter != '\0') {
                Serial.printf("Character sent: %c\n", selectedLetter);
                if (onLetterSelectedCallback) {
                    onLetterSelectedCallback(std::string(1, selectedLetter));
                }
            }
            // Return to row selection mode
            selectRow();
            Serial.println("Returned to row selection mode");
        } else if (button == 3) {
            // Return to row selection mode
            selectRow();
            Serial.println("Button 3 pressed: Returned to row selection mode");
        }
    }

    // Update the visual selection
    updateSelection();

    // Scroll to ensure the selected row is visible
    if (currentMode == Mode::SelectRow) {
        lv_obj_scroll_to_view(rowContainers[selectedRowIndex - visibleRowStart], LV_ANIM_ON);
    }
}

/**
 * @brief Updates the visual selection (highlighting) of the keyboard.
 * 
 * This function highlights the currently selected row or character.
 */
void Keyboard::updateSelection() {
    for (int i = 0; i < visibleRowCount; i++) {
        int rowIndex = visibleRowStart + i;
        if (rowIndex >= rows.size()) {
            break;
        }

        if (rowIndex == selectedRowIndex && currentMode == Mode::SelectRow) {
            lv_obj_set_style_border_color(rowContainers[i], lv_color_hex(0xFF0000), 0);
        } else {
            lv_obj_set_style_border_color(rowContainers[i], lv_color_hex(0x808080), 0);
        }
    }

    if (currentMode == Mode::SelectLetter) {
        for (int j = 0; j < letterButtons[selectedRowIndex - visibleRowStart].size(); j++) {
            lv_obj_t* label = lv_obj_get_child(letterButtons[selectedRowIndex - visibleRowStart][j], 0);
            if (j == selectedLetterIndex) {
                lv_obj_set_style_text_color(label, lv_color_hex(0xFF0000), 0);
            } else {
                lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0);
            }
        }
    } else if (currentMode == Mode::SelectRow) {
        for (int j = 0; j < letterButtons[selectedRowIndex - visibleRowStart].size(); j++) {
            lv_obj_t* label = lv_obj_get_child(letterButtons[selectedRowIndex - visibleRowStart][j], 0);
            lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0);
        }
    }
}

/**
 * @brief Switches to row selection mode.
 * 
 * This function changes the keyboard mode to row selection and resets the character selection.
 */
void Keyboard::selectRow() {
    currentMode = Mode::SelectRow;
    selectedLetterIndex = 0;
}

/**
 * @brief Switches to character selection mode.
 * 
 * This function changes the keyboard mode to character selection and resets the character index.
 */
void Keyboard::selectLetter() {
    currentMode = Mode::SelectLetter;
    selectedLetterIndex = 0;
}

/**
 * @brief Checks if the keyboard is in character selection mode.
 * 
 * @return true if in character selection mode, false otherwise.
 */
bool Keyboard::isSelectingLetter() const {
    return currentMode == Mode::SelectLetter;
}

/**
 * @brief Checks if the keyboard is in row selection mode.
 * 
 * @return true if in row selection mode, false otherwise.
 */
bool Keyboard::isSelectingRow() const {
    return currentMode == Mode::SelectRow;
}

/**
 * @brief Shows the keyboard.
 * 
 * This function makes the keyboard visible on the screen.
 */
void Keyboard::show() {
    lv_obj_clear_flag(keyboardContainer, LV_OBJ_FLAG_HIDDEN);
    Serial.println("Keyboard shown.");
}

/**
 * @brief Hides the keyboard.
 * 
 * This function makes the keyboard invisible on the screen.
 */
void Keyboard::hide() {
    lv_obj_add_flag(keyboardContainer, LV_OBJ_FLAG_HIDDEN);
    Serial.println("Keyboard hidden.");
}

/**
 * @brief Sets the callback for letter selection.
 * 
 * This function sets a callback that is triggered when a letter is selected.
 * 
 * @param callback Callback function to handle letter selection.
 */
void Keyboard::setOnLetterSelectedCallback(std::function<void(std::string)> callback) {
    onLetterSelectedCallback = callback;
}