#include "InputText.h"
#include <cstring>

/**
 * @brief Constructor for the InputText class.
 * 
 * Initializes the text input component with a parent LVGL object and a maximum word length.
 * 
 * @param parent The parent LVGL object (usually a screen or container).
 * @param maxLength The maximum number of characters allowed in the input.
 */
InputText::InputText(lv_obj_t* parent, int maxLength) 
    : maxLength(maxLength), selectedBoxIndex(0) {
    // Create the container for the text boxes
    container = lv_obj_create(parent);
    lv_obj_set_size(container, 126, 36);
    lv_obj_set_style_pad_all(container, 0, 0);
    lv_obj_set_style_border_width(container, 1, 0);
    lv_obj_set_style_border_color(container, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_color(container, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_opa(container, LV_OPA_COVER, 0);

    // Enable horizontal scrolling
    lv_obj_set_scroll_dir(container, LV_DIR_HOR);
    lv_obj_set_scroll_snap_x(container, LV_SCROLL_SNAP_CENTER);

    // Create the buffer to store the text
    textBuffer = new char[maxLength + 1];
    memset(textBuffer, 0, maxLength + 1);

    // Create the text boxes
    createBoxes(container);

    // Initially, no box is highlighted in red
    clearSelection();
}

/**
 * @brief Creates the text boxes inside the container.
 * 
 * This function initializes the LVGL objects for each text box and arranges them in a grid.
 * 
 * @param parent The parent LVGL object (the container).
 */
void InputText::createBoxes(lv_obj_t* parent) {
    const int boxWidth = 18; // Fixed width of each box
    const int boxHeight = 20; // Fixed height of each box

    for (int i = 0; i < maxLength; i++) {
        lv_obj_t* box = lv_obj_create(parent);
        lv_obj_set_size(box, boxWidth, boxHeight);
        lv_obj_set_pos(box, 5 + i * boxWidth, ((35 - boxHeight)  / 2) - 1);
        lv_obj_set_style_border_width(box, 1, 0);
        lv_obj_set_style_border_color(box, lv_color_hex(0x000000), 0);
        lv_obj_set_style_pad_all(box, 0, 0);
        lv_obj_set_style_bg_opa(box, LV_OPA_TRANSP, 0);

        // Disable scrolling for the box
        lv_obj_set_scrollbar_mode(box, LV_SCROLLBAR_MODE_OFF);
        lv_obj_set_scroll_dir(box, LV_DIR_NONE);

        // Create a label for the character
        lv_obj_t* label = lv_label_create(box);
        lv_label_set_text(label, "");
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_center(label);

        boxes.push_back(box);
    }
}

/**
 * @brief Updates the text displayed in the boxes.
 * 
 * This function refreshes the content of each box based on the current text buffer to display the box character.
 */
void InputText::updateBoxes() {
    for (int i = 0; i < maxLength; i++) {
        lv_obj_t* box = boxes[i];
        lv_obj_t* label = lv_obj_get_child(box, 0);
        if (textBuffer[i] != '\0') {
            lv_label_set_text_fmt(label, "%c", textBuffer[i]);
        } else {
            lv_label_set_text(label, "");
        }
    }
}

/**
 * @brief Sets the currently selected box.
 * 
 * This function highlights the selected box by changing its border color to red.
 * 
 * @param index The index of the box to select (1-based).
 */
void InputText::setSelectedBox(int index) {
    // Restore the border color of the previously selected box
    if (selectedBoxIndex > 0 && selectedBoxIndex <= maxLength) {
        lv_obj_set_style_border_color(boxes[selectedBoxIndex - 1], lv_color_hex(0x000000), 0);
    }

    if (index == 0 || index == -1 || (index > 0 && index <= maxLength)) {
        selectedBoxIndex = index;
    } else if (index > maxLength) {
        selectedBoxIndex = -1;
    } else {
        selectedBoxIndex = 0;
    }

    // Change the border color of the selected box to red
    if (selectedBoxIndex > 0 && selectedBoxIndex <= maxLength) {
        lv_obj_set_style_border_color(boxes[selectedBoxIndex - 1], lv_color_hex(0xFF0000), 0);
        lv_obj_scroll_to_view(boxes[selectedBoxIndex - 1], LV_ANIM_ON);
    }
}

/**
 * @brief Inserts a character into the selected box.
 * 
 * This function adds a character to the text buffer at the position of the selected box.
 * 
 * @param c The character to insert.
 * @return The index of the next selected box.
 */
int InputText::insertChar(char c) {
    if (selectedBoxIndex > 0 && selectedBoxIndex <= maxLength) {
        textBuffer[selectedBoxIndex - 1] = c;
        updateBoxes();
        return moveNext();
    }
    return selectedBoxIndex;
}

/**
 * @brief Gets the index of the currently selected box.
 * 
 * @return The index of the selected box (1-based).
 */
int InputText::getSelectedBoxIndex() const {
    return selectedBoxIndex;
}

/**
 * @brief Moves the selection to the next box.
 * 
 * This function advances the selection to the next box with a character or exits if no more boxes are available.
 * 
 * @return The index of the next selected box or -1 if no more boxes were available.
 */
int InputText::moveNext() {
    if (selectedBoxIndex == 0){
        setSelectedBox(1);
    } else if (selectedBoxIndex > 0 && selectedBoxIndex <= maxLength) {
        if (textBuffer[selectedBoxIndex - 1] == '\0') {
            setSelectedBox(-1);
        } else {
            setSelectedBox(selectedBoxIndex + 1);
        }
    } else if (selectedBoxIndex == -1) {
        // No more boxes to select
    }

    if (selectedBoxIndex > 0 && selectedBoxIndex <= maxLength) {
        lv_obj_scroll_to_view(boxes[selectedBoxIndex - 1], LV_ANIM_ON);
    }

    updateSelection();
    return selectedBoxIndex;
}

/**
 * @brief Moves the selection to the previous box.
 * 
 * This function moves the selection to the previous box or exits if no more boxes are available.
 * 
 * @return The index of the previous selected box or 0 if no more boxes were available.
 */
int InputText::movePrevious() {
    if (selectedBoxIndex == 0) {
        // No more boxes to select
    } else if (selectedBoxIndex > 0 && selectedBoxIndex <= maxLength) {
        setSelectedBox(selectedBoxIndex - 1);
    } else if (selectedBoxIndex == -1) {
        // Move to the last filled box
        int lastFilledIndex = -1;
        for (int i = maxLength - 1; i >= 0; i--) {
            if (textBuffer[i] != '\0') {
                lastFilledIndex = i + 1;
                break;
            }
        }
        if (lastFilledIndex == -1) {
            selectedBoxIndex = 1;
        } else {
            selectedBoxIndex = lastFilledIndex;
        }
    }

    if (selectedBoxIndex > 0 && selectedBoxIndex <= maxLength) {
        lv_obj_scroll_to_view(boxes[selectedBoxIndex - 1], LV_ANIM_ON);
    }

    updateSelection();
    return selectedBoxIndex;
}

/**
 * @brief Clears the selection (deselects all boxes).
 */
void InputText::clearSelection() {
    if ((selectedBoxIndex > 0) and (selectedBoxIndex <= maxLength)) {
        lv_obj_set_style_border_color(boxes[selectedBoxIndex - 1], lv_color_hex(0x000000), 0);
    }
    selectedBoxIndex = 0;
}

/**
 * @brief Updates the visual selection (highlights the selected box).
 */
void InputText::updateSelection() {
    if ((selectedBoxIndex > 0) and (selectedBoxIndex <= maxLength)) {
        lv_obj_set_style_border_color(boxes[selectedBoxIndex - 1], lv_color_hex(0xFF0000), 0);
    }
}

/**
 * @brief Shows the input text component.
 */
void InputText::show() {
    lv_obj_clear_flag(container, LV_OBJ_FLAG_HIDDEN);
}

/**
 * @brief Hides the input text component.
 */
void InputText::hide() {
    lv_obj_add_flag(container, LV_OBJ_FLAG_HIDDEN);
}

/**
 * @brief Checks if the input text component is visible.
 * 
 * @return true if visible, false otherwise.
 */
bool InputText::isVisible() const {
    return !lv_obj_has_flag(container, LV_OBJ_FLAG_HIDDEN);
}

/**
 * @brief Sets a word in the input component.
 * 
 * @param text The text to set.
 */
void InputText::setText(const char* text) {
    strncpy(textBuffer, text, maxLength);
    textBuffer[maxLength] = '\0';
    updateBoxes();
}

/**
 * @brief Gets the text entered in the input component.
 * 
 * @return The entered text as a null-terminated string.
 */
const char* InputText::getText() const {
    static char result[50];
    int resultIndex = 0;
    bool foundNonEmpty = false;

    for (int i = 0; i < maxLength; i++) {
        if (textBuffer[i] != '\0') {
            foundNonEmpty = true;
        }
        if (foundNonEmpty) {
            result[resultIndex++] = textBuffer[i];
        }
    }

    while (resultIndex > 0 && result[resultIndex - 1] == '\0') {
        resultIndex--;
    }

    for (int i = 0; i < resultIndex; i++) {
        if (result[i] == '\0') {
            result[i] = ' ';
        }
    }

    result[resultIndex] = '\0';
    return result;
}

/**
 * @brief Sets the maximum length of the input text.
 * 
 * @param maxLength The maximum number of characters allowed.
 */
void InputText::setMaxLength(int maxLength) {
    this->maxLength = maxLength;
    delete[] textBuffer;
    textBuffer = new char[maxLength + 1];
    memset(textBuffer, 0, maxLength + 1);
    createBoxes(container);
}

/**
 * @brief Gets the maximum length of the input text.
 * 
 * @return The maximum number of characters allowed.
 */
int InputText::getMaxLength() const {
    return maxLength;
}

/**
 * @brief Gets the LVGL container of the input text component.
 * 
 * @return The LVGL container object.
 */
lv_obj_t* InputText::getContainer() const {
    return container;
}