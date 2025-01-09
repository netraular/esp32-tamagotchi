#include "Keyboard.h"
#include <lvgl.h>

Keyboard::Keyboard(lv_obj_t* parent) {
    // Crear el contenedor del teclado
    keyboardContainer = lv_obj_create(parent);
    lv_obj_set_size(keyboardContainer, 160, 128);
    lv_obj_align(keyboardContainer, LV_ALIGN_CENTER, 0, 0);

    // Crear una etiqueta para mostrar el estado
    label = lv_label_create(keyboardContainer);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 10);

    // Definir las filas de letras
    rows = {
        {'A', 'B', 'C', 'D', 'E', 'F'},
        {'G', 'H', 'I', 'J', 'K', 'L'},
        {'M', 'N', 'O', 'P', 'Q', 'R'},
        {'S', 'T', 'U', 'V', 'W', 'X'},
        {'Y', 'Z', ' ', '<', '\n'}
    };

    currentRowIndex = 0;
    currentLetterIndex = 0;
    isSelectingRow = true;
    lastAction = Action::None;
    selectedLetter = '\0';

    updateDisplay();
}

void Keyboard::show() {
    lv_obj_clear_flag(keyboardContainer, LV_OBJ_FLAG_HIDDEN);
}

void Keyboard::hide() {
    lv_obj_add_flag(keyboardContainer, LV_OBJ_FLAG_HIDDEN);
}

bool Keyboard::isVisible() const {
    return !lv_obj_has_flag(keyboardContainer, LV_OBJ_FLAG_HIDDEN);
}

void Keyboard::handleButtonEvent(int button) {
    if (button == 1) {
        if (isSelectingRow) {
            currentRowIndex = (currentRowIndex + 1) % rows.size();
        } else {
            currentLetterIndex = (currentLetterIndex + 1) % rows[currentRowIndex].size();
        }
    } else if (button == 2) {
        if (isSelectingRow) {
            selectRow();
        } else {
            selectLetter();
        }
    } else if (button == 3) {
        if (isSelectingRow) {
            lastAction = Action::Exit;
        } else {
            isSelectingRow = true;
        }
    }

    updateDisplay();
}

Keyboard::Action Keyboard::getLastAction() const {
    return lastAction;
}

char Keyboard::getSelectedLetter() const {
    return selectedLetter;
}

void Keyboard::updateDisplay() {
    lv_label_set_text(label, "");

    for (int i = 0; i < rows.size(); i++) {
        for (int j = 0; j < rows[i].size(); j++) {
            if (i == currentRowIndex && j == currentLetterIndex && !isSelectingRow) {
                lv_label_set_text_fmt(label, "%s|%c|", lv_label_get_text(label), rows[i][j]);
            } else if (i == currentRowIndex && isSelectingRow) {
                lv_label_set_text_fmt(label, "%s[%c]", lv_label_get_text(label), rows[i][j]);
            } else {
                lv_label_set_text_fmt(label, "%s %c ", lv_label_get_text(label), rows[i][j]);
            }
        }
        lv_label_set_text_fmt(label, "%s\n", lv_label_get_text(label));
    }
}

void Keyboard::selectRow() {
    isSelectingRow = false;
    currentLetterIndex = 0;
}

void Keyboard::selectLetter() {
    selectedLetter = rows[currentRowIndex][currentLetterIndex];
    lastAction = Action::LetterSelected;
}