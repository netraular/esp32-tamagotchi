#include "Keyboard.h"
#include <string>
#include <Arduino.h>

Keyboard::Keyboard(lv_obj_t* parent) : selectedRowIndex(0), selectedLetterIndex(0), currentMode(Mode::SelectRow), visibleRowStart(0), visibleRowCount(3) {
    keyboardContainer = lv_obj_create(parent);
    lv_obj_set_size(keyboardContainer, 128, 70);
    lv_obj_align(keyboardContainer, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_scrollbar_mode(keyboardContainer, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_all(keyboardContainer, 0, 0); // Sin padding general

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

    createKeyboard();
    updateSelection();

    Serial.println("Teclado inicializado.");
}

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
    Serial.println("Teclado creado con éxito.");
}

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

void Keyboard::handleButtonEvent(int button) {
    if (currentMode == Mode::SelectRow) {
        if (button == 1) {
            // Cambiar a la siguiente fila
            selectedRowIndex = (selectedRowIndex + 1) % rows.size(); // Ciclar al principio si se llega al final
            if (selectedRowIndex >= visibleRowStart + visibleRowCount) {
                visibleRowStart = selectedRowIndex - visibleRowCount + 1;
                updateVisibleRows();
            } else if (selectedRowIndex < visibleRowStart) {
                visibleRowStart = selectedRowIndex;
                updateVisibleRows();
            }
            Serial.printf("Fila seleccionada: %d\n", selectedRowIndex);
        } else if (button == 2) {
            // Seleccionar la fila actual y entrar en el modo de selección de letras
            selectLetter();
            Serial.println("Modo de selección de letra activado");
        } else if (button == 3) {
            // Salir del teclado
            Serial.println("Botón 3 presionado: Saliendo del teclado");
            if (onLetterSelectedCallback) {
                onLetterSelectedCallback("exit");
            }
            hide();
        }
    } else if (currentMode == Mode::SelectLetter) {
        if (button == 1) {
            // Cambiar a la siguiente letra
            selectedLetterIndex = (selectedLetterIndex + 1) % rows[selectedRowIndex].size();
            Serial.printf("Letra seleccionada: %c\n", rows[selectedRowIndex][selectedLetterIndex]);
        } else if (button == 2) {
            // Seleccionar la letra actual y enviarla como output
            char selectedLetter = rows[selectedRowIndex][selectedLetterIndex];
            if (selectedLetter != '\0') {
                Serial.printf("Letra enviada: %c\n", selectedLetter);
                if (onLetterSelectedCallback) {
                    onLetterSelectedCallback(std::string(1, selectedLetter));
                }
            }
            // Volver al modo de selección de fila
            selectRow();
            Serial.println("Volviendo al modo de selección de fila");
        } else if (button == 3) {
            // Volver al modo de selección de fila
            selectRow();
            Serial.println("Botón 3 presionado: Volviendo al modo de selección de fila");
        }
    }

    // Actualizar la selección visual
    updateSelection();

    // Mover el scroll para que la fila seleccionada esté visible
    if (currentMode == Mode::SelectRow) {
        lv_obj_scroll_to_view(rowContainers[selectedRowIndex - visibleRowStart], LV_ANIM_ON);
    }
}

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

void Keyboard::selectLetter() {
    currentMode = Mode::SelectLetter;
    selectedLetterIndex = 0;
}

void Keyboard::selectRow() {
    currentMode = Mode::SelectRow;
    selectedLetterIndex = 0;
}

bool Keyboard::isSelectingLetter() const {
    return currentMode == Mode::SelectLetter;
}

bool Keyboard::isSelectingRow() const {
    return currentMode == Mode::SelectRow;
}

void Keyboard::show() {
    lv_obj_clear_flag(keyboardContainer, LV_OBJ_FLAG_HIDDEN); // Mostrar el teclado
    Serial.println("Teclado mostrado.");
}

void Keyboard::hide() {
    lv_obj_add_flag(keyboardContainer, LV_OBJ_FLAG_HIDDEN); // Ocultar el teclado
    Serial.println("Teclado ocultado.");
}

void Keyboard::setOnLetterSelectedCallback(std::function<void(std::string)> callback) {
    onLetterSelectedCallback = callback;
}