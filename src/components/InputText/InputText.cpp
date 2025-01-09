#include "InputText.h"
#include <cstring> // Añadir esta línea para usar strlen y strncpy

InputText::InputText(lv_obj_t* parent, int maxLength) : maxLength(maxLength) {
    // Crear el área de texto
    textArea = lv_textarea_create(parent);
    lv_textarea_set_one_line(textArea, true); // Permitir solo una línea
    lv_textarea_set_max_length(textArea, maxLength); // Establecer el límite de caracteres

    // Añadir un evento para manejar la entrada de texto
    lv_obj_add_event_cb(textArea, InputText::eventHandler, LV_EVENT_VALUE_CHANGED, this);
}

lv_obj_t* InputText::getTextArea() const {
    return textArea;
}

void InputText::setMaxLength(int maxLength) {
    this->maxLength = maxLength;
    lv_textarea_set_max_length(textArea, maxLength);
}

int InputText::getMaxLength() const {
    return maxLength;
}

void InputText::setPlaceholder(const char* placeholder) {
    lv_textarea_set_placeholder_text(textArea, placeholder);
}

void InputText::setText(const char* text) {
    lv_textarea_set_text(textArea, text);
}

const char* InputText::getText() const {
    return lv_textarea_get_text(textArea);
}

void InputText::eventHandler(lv_event_t* e) {
    InputText* inputText = (InputText*)lv_event_get_user_data(e);
    lv_obj_t* textArea = inputText->getTextArea();

    // Obtener el texto actual
    const char* text = lv_textarea_get_text(textArea);

    // Si el texto excede el límite, truncarlo
    if (strlen(text) > inputText->getMaxLength()) {
        char truncatedText[inputText->getMaxLength() + 1];
        strncpy(truncatedText, text, inputText->getMaxLength());
        truncatedText[inputText->getMaxLength()] = '\0';
        lv_textarea_set_text(textArea, truncatedText);
    }
}