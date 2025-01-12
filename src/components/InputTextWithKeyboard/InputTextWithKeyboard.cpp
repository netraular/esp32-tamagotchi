#include "InputTextWithKeyboard.h"

/**
 * @brief Constructor for the InputTextWithKeyboard class.
 * 
 * Initializes the text input and keyboard components and sets up the callback for keyboard input.
 * 
 * @param parent The parent LVGL object (usually a screen or container).
 * @param maxLength The maximum number of characters allowed in the input.
 */
InputTextWithKeyboard::InputTextWithKeyboard(lv_obj_t* parent, int maxLength) 
    : isKeyboardActive(false) {
    inputText = new InputText(parent, maxLength);
    keyboard = new Keyboard(parent);
    keyboard->hide(); // Hide the keyboard initially

    // Set up the callback for keyboard input
    keyboard->setOnLetterSelectedCallback([this](std::string value) {
        handleKeyboardInput(value);
    });
}

/**
 * @brief Shows the input text and keyboard components.
 */
void InputTextWithKeyboard::show() {
    inputText->show();
    if (isKeyboardActive) {
        keyboard->show();
    }
}

/**
 * @brief Hides the input text and keyboard components.
 */
void InputTextWithKeyboard::hide() {
    inputText->hide();
    keyboard->hide();
}

/**
 * @brief Checks if the input text or keyboard components are visible.
 * 
 * @return true if either component is visible, false otherwise.
 */
bool InputTextWithKeyboard::isVisible() const {
    return inputText->isVisible() || keyboard->isVisible();
}

/**
 * @brief Sets the text in the input component.
 * 
 * @param text The text to set.
 */
void InputTextWithKeyboard::setText(const char* text) {
    inputText->setText(text);
}

/**
 * @brief Gets the text entered in the input component.
 * 
 * @return The entered text as a null-terminated string.
 */
const char* InputTextWithKeyboard::getText() const {
    return inputText->getText();
}

/**
 * @brief Sets the maximum length of the input text.
 * 
 * @param maxLength The maximum number of characters allowed.
 */
void InputTextWithKeyboard::setMaxLength(int maxLength) {
    inputText->setMaxLength(maxLength);
}

/**
 * @brief Gets the maximum length of the input text.
 * 
 * @return The maximum number of characters allowed.
 */
int InputTextWithKeyboard::getMaxLength() const {
    return inputText->getMaxLength();
}

/**
 * @brief Handles button events for navigation and input.
 * 
 * This function processes button presses to navigate between text boxes, activate the keyboard,
 * or confirm the entered text. Button actions are passed to either the `Keyboard` or `InputText`
 * component based on the current mode (keyboard active or inactive).
 * 
 * @param state The current state of the buttons.
 * @param change Indicates which buttons have changed state.
 */
void InputTextWithKeyboard::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    if (isKeyboardActive) {
        // If the keyboard is active, pass the events to the keyboard
        if (change.button1Changed && state.button1Pressed) {
            keyboard->handleButtonEvent(1);
        }
        if (change.button2Changed && state.button2Pressed) {
            keyboard->handleButtonEvent(2);
        }
        if (change.button3Changed && state.button3Pressed) {
            keyboard->handleButtonEvent(3);
        }
    } else {
        int currentIndex = inputText->getSelectedBoxIndex();
        // If the keyboard is not active, handle the buttons for text input
        if (change.button1Changed && state.button1Pressed) {
            if (currentIndex != -1) {
                // Move to the next box or confirm the text if no more boxes are available
                int nextPosition = inputText->moveNext();
                if (nextPosition == -1) {
                    // No more boxes available, trigger the callback with the entered text
                    if (onTextEnteredCallback) {
                        onTextEnteredCallback(inputText->getText());
                    }
                }
            } else {
                // Exit with "exitRight" if no boxes are selected
                if (onTextEnteredCallback) {
                    onTextEnteredCallback("exitRight");
                }
            }
        }
        if (change.button2Changed && state.button2Pressed) {
            // Activate the keyboard to edit the selected box
            if (currentIndex != -1 && currentIndex != 0) {
                isKeyboardActive = true;
                keyboard->show();
            }
        }
        if (change.button3Changed && state.button3Pressed) {
            if (currentIndex != 0) {
                // Move to the previous box
                inputText->movePrevious();
            } else {
                // Exit with "exitLeft" if no boxes are selected
                if (onTextEnteredCallback) {
                    onTextEnteredCallback("exitLeft");
                }
            }
        }
    }
}

/**
 * @brief Sets the callback for when text is entered.
 * 
 * This function sets a callback that is triggered when the user confirms the entered text.
 * 
 * @param callback The callback function to handle the entered text.
 */
void InputTextWithKeyboard::setOnTextEnteredCallback(std::function<void(const char*)> callback) {
    onTextEnteredCallback = callback;
}

/**
 * @brief Handles input from the keyboard.
 * 
 * This function processes input from the keyboard, either inserting a character into the selected
 * text box or exiting the keyboard.
 * 
 * @param value The input value from the keyboard (a character or "exit").
 */
void InputTextWithKeyboard::handleKeyboardInput(const std::string& value) {
    if (value == "exit") {
        // Exit the keyboard
        isKeyboardActive = false;
        keyboard->hide();
    } else {
        // Insert the character into the selected box
        int nextPosition = inputText->insertChar(value[0]);
        if (nextPosition == -1) {
            // No more boxes available, exit the keyboard and trigger the callback
            isKeyboardActive = false;
            keyboard->hide();
            if (onTextEnteredCallback) {
                onTextEnteredCallback(inputText->getText());
            }
        }
    }
}

/**
 * @brief Sets the position of the input text component.
 * 
 * This function adjusts the position of the input text component relative to the top-middle of the screen.
 * 
 * @param x_offset The horizontal offset from the top-middle position.
 * @param y_offset The vertical offset from the top-middle position.
 */
void InputTextWithKeyboard::setInputPosition(int x_offset, int y_offset) {
    lv_obj_t* inputContainer = inputText->getContainer();
    lv_obj_align(inputContainer, LV_ALIGN_TOP_MID, x_offset, y_offset);
}