#include "InputTextWithKeyboard.h"

InputTextWithKeyboard::InputTextWithKeyboard(lv_obj_t* parent, int maxLength) 
    : isKeyboardActive(false) {
    inputText = new InputText(parent, maxLength);
    keyboard = new Keyboard(parent);
    keyboard->hide(); // Ocultar el teclado inicialmente

    // Configurar el callback para recibir la letra seleccionada o la salida del teclado
    keyboard->setOnLetterSelectedCallback([this](std::string value) {
        handleKeyboardInput(value);
    });
}

void InputTextWithKeyboard::show() {
    inputText->show();
    if (isKeyboardActive) {
        keyboard->show();
    }
}

void InputTextWithKeyboard::hide() {
    inputText->hide();
    keyboard->hide();
}

bool InputTextWithKeyboard::isVisible() const {
    return inputText->isVisible() || keyboard->isVisible();
}

void InputTextWithKeyboard::setText(const char* text) {
    inputText->setText(text);
}

const char* InputTextWithKeyboard::getText() const {
    return inputText->getText();
}

void InputTextWithKeyboard::setMaxLength(int maxLength) {
    inputText->setMaxLength(maxLength);
}

int InputTextWithKeyboard::getMaxLength() const {
    return inputText->getMaxLength();
}

void InputTextWithKeyboard::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    if (isKeyboardActive) {
        // Si el teclado está activo, pasar los eventos al teclado
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
        // Si el teclado no está activo, manejar los botones
        if (change.button1Changed && state.button1Pressed) {
            if (currentIndex != -1) {
                // Si hay una casilla seleccionada, avanzar al siguiente índice
                int nextPosition = inputText->moveNext();
                if (nextPosition == -1) {
                    // No hay más casillas disponibles, mostrar el nombre ingresado
                    if (onTextEnteredCallback) {
                        onTextEnteredCallback(inputText->getText());
                    }
                }
            } else {
                // Si currentIndex == -1, salir con exitRight
                if (onTextEnteredCallback) {
                    onTextEnteredCallback("exitRight");
                }
            }
        }
        if (change.button2Changed && state.button2Pressed) {
            // Mostrar el teclado para editar la casilla seleccionada
            if (currentIndex != -1 && currentIndex != 0) {
                isKeyboardActive = true;
                keyboard->show();
            }
        }
        if (change.button3Changed && state.button3Pressed) {
            if (currentIndex != 0) {
                // Si la posición del selector no es 0, retroceder al índice anterior
                inputText->movePrevious();
            } else {
                // Si la posición del selector es 0, salir con exitLeft
                if (onTextEnteredCallback) {
                    onTextEnteredCallback("exitLeft");
                }
            }
        }
    }
}

void InputTextWithKeyboard::setOnTextEnteredCallback(std::function<void(const char*)> callback) {
    onTextEnteredCallback = callback;
}

void InputTextWithKeyboard::handleKeyboardInput(const std::string& value) {
    if (value == "exit") {
        isKeyboardActive = false;
        keyboard->hide();
    } else {
        int nextPosition = inputText->insertChar(value[0]); // Insertar la letra en la casilla seleccionada
        if (nextPosition == -1) {
            isKeyboardActive = false;
            keyboard->hide();
            if (onTextEnteredCallback) {
                onTextEnteredCallback(inputText->getText());
            }
        }
    }
}

void InputTextWithKeyboard::setInputPosition(int x_offset, int y_offset) {
    lv_obj_t* inputContainer = inputText->getContainer();
    lv_obj_align(inputContainer, LV_ALIGN_TOP_MID, x_offset, y_offset);
}