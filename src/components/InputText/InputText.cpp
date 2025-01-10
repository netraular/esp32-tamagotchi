#include "InputText.h"
#include <cstring>

InputText::InputText(lv_obj_t* parent, int maxLength) : maxLength(maxLength) {
    // Crear el contenedor de las casillas de texto
    container = lv_obj_create(parent);
    lv_obj_set_size(container, 126, 36);
    lv_obj_set_style_pad_all(container, 0, 0); // Sin padding general
    lv_obj_set_style_border_width(container, 1, 0); // Borde de 1 píxel
    lv_obj_set_style_border_color(container, lv_color_hex(0x000000), 0); // Borde negro
    lv_obj_set_style_bg_color(container, lv_color_hex(0xFFFFFF), 0); // Fondo blanco
    lv_obj_set_style_bg_opa(container, LV_OPA_COVER, 0); // Fondo opaco

    // Estilizar la scrollbar para que sea más delgada
    static lv_style_t style_scrollbar;
    lv_style_init(&style_scrollbar);
    lv_style_set_width(&style_scrollbar, 2);
    lv_style_set_bg_opa(&style_scrollbar, LV_OPA_70); // Opacidad del fondo
    lv_style_set_bg_color(&style_scrollbar, lv_color_hex(0x808080)); // Color gris
    lv_style_set_radius(&style_scrollbar, 1); // Radio de las esquinas

    // Aplicar el estilo a la scrollbar
    lv_obj_add_style(container, &style_scrollbar, LV_PART_SCROLLBAR);

    // Crear el buffer para almacenar el texto
    textBuffer = new char[maxLength + 1];
    memset(textBuffer, 0, maxLength + 1);

    // Crear las casillas de texto
    createBoxes(container);

    // Desplazar la barra de scroll a la izquierda al cargar el componente
    lv_obj_scroll_to_x(container, 0, LV_ANIM_OFF); // Sin animación
}

void InputText::createBoxes(lv_obj_t* parent) {
    const int boxWidth = 18; // Ancho fijo de cada casilla
    const int boxHeight = 20; // Altura fija de cada casilla

    // Calcular el espacio total ocupado por las casillas
    int totalWidth = maxLength * boxWidth;
    int startX = ((126 - totalWidth) / 2)+10; // Centrar las casillas horizontalmente

    for (int i = 0; i < maxLength; i++) {
        lv_obj_t* box = lv_obj_create(parent);
        lv_obj_set_size(box, boxWidth, boxHeight); // Tamaño fijo de 18x20 píxeles
        lv_obj_set_pos(box, startX + i * boxWidth, ((35 - boxHeight)  / 2) - 1); // Centrar verticalmente
        lv_obj_set_style_border_width(box, 1, 0); // Borde de 1 píxel
        lv_obj_set_style_border_color(box, lv_color_hex(0x000000), 0); // Borde negro
        lv_obj_set_style_pad_all(box, 0, 0); // Sin padding
        lv_obj_set_style_bg_opa(box, LV_OPA_TRANSP, 0); // Fondo transparente

        // Desactivar scroll horizontal y vertical
        lv_obj_set_scrollbar_mode(box, LV_SCROLLBAR_MODE_OFF);
        lv_obj_set_scroll_dir(box, LV_DIR_NONE);

        // Crear una etiqueta para el carácter
        lv_obj_t* label = lv_label_create(box);
        lv_label_set_text(label, ""); // Inicialmente vacío
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0); // Centrar el texto
        lv_obj_center(label); // Centrar la etiqueta en la casilla

        boxes.push_back(box);
    }
}

void InputText::updateBoxes() {
    for (int i = 0; i < maxLength; i++) {
        lv_obj_t* box = boxes[i];
        lv_obj_t* label = lv_obj_get_child(box, 0);
        if (i < strlen(textBuffer)) {
            lv_label_set_text_fmt(label, "%c", textBuffer[i]); // Mostrar el carácter
        } else {
            lv_label_set_text(label, ""); // Casilla vacía
        }
    }
}

void InputText::show() {
    lv_obj_clear_flag(container, LV_OBJ_FLAG_HIDDEN);
}

void InputText::hide() {
    lv_obj_add_flag(container, LV_OBJ_FLAG_HIDDEN);
}

bool InputText::isVisible() const {
    return !lv_obj_has_flag(container, LV_OBJ_FLAG_HIDDEN);
}

void InputText::setText(const char* text) {
    strncpy(textBuffer, text, maxLength);
    textBuffer[maxLength] = '\0'; // Asegurar que el texto no exceda el límite
    updateBoxes();
}

const char* InputText::getText() const {
    return textBuffer;
}

void InputText::setMaxLength(int maxLength) {
    this->maxLength = maxLength;
    delete[] textBuffer;
    textBuffer = new char[maxLength + 1];
    memset(textBuffer, 0, maxLength + 1);
    createBoxes(container);
}

int InputText::getMaxLength() const {
    return maxLength;
}

void InputText::setPlaceholder(const char* placeholder) {
    // Implementar lógica para mostrar un placeholder si es necesario
}

lv_obj_t* InputText::getContainer() const {
    return container; // Devolver el contenedor
}