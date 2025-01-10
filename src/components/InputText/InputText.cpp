#include "InputText.h"
#include <cstring>

InputText::InputText(lv_obj_t* parent, int maxLength) 
    : maxLength(maxLength), selectedBoxIndex(0) {
    // Crear el contenedor de las casillas de texto
    container = lv_obj_create(parent);
    lv_obj_set_size(container, 126, 36); // Ajusta el tamaño según sea necesario
    lv_obj_set_style_pad_all(container, 0, 0); // Sin padding general
    lv_obj_set_style_border_width(container, 1, 0); // Borde de 1 píxel
    lv_obj_set_style_border_color(container, lv_color_hex(0x000000), 0); // Borde negro
    lv_obj_set_style_bg_color(container, lv_color_hex(0xFFFFFF), 0); // Fondo blanco
    lv_obj_set_style_bg_opa(container, LV_OPA_COVER, 0); // Fondo opaco

    // Habilitar el scroll horizontal
    lv_obj_set_scroll_dir(container, LV_DIR_HOR);
    lv_obj_set_scroll_snap_x(container, LV_SCROLL_SNAP_CENTER); // Centrar la casilla seleccionada

    // Crear el buffer para almacenar el texto
    textBuffer = new char[maxLength + 1];
    memset(textBuffer, 0, maxLength + 1);

    // Crear las casillas de texto
    createBoxes(container);

    // Inicialmente, ninguna casilla está resaltada en rojo
    clearSelection();
}

void InputText::createBoxes(lv_obj_t* parent) {
    const int boxWidth = 18; // Ancho fijo de cada casilla
    const int boxHeight = 20; // Altura fija de cada casilla

    for (int i = 0; i < maxLength; i++) {
        lv_obj_t* box = lv_obj_create(parent);
        lv_obj_set_size(box, boxWidth, boxHeight); // Tamaño fijo de 18x20 píxeles
        lv_obj_set_pos(box, 5 + i * boxWidth, ((35 - boxHeight)  / 2) - 1); // Centrar verticalmente
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
        if (textBuffer[i] != '\0') {
            lv_label_set_text_fmt(label, "%c", textBuffer[i]); // Mostrar el carácter
        } else {
            lv_label_set_text(label, ""); // Casilla vacía
        }
    }
}

void InputText::setSelectedBox(int index) {
    // Restaurar el borde de la casilla previamente seleccionada
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

    // Cambiar el borde de la casilla seleccionada a rojo
    if (selectedBoxIndex > 0 && selectedBoxIndex <= maxLength) {
        lv_obj_set_style_border_color(boxes[selectedBoxIndex - 1], lv_color_hex(0xFF0000), 0);
        // Desplazar el contenedor para que la casilla seleccionada esté visible
        lv_obj_scroll_to_view(boxes[selectedBoxIndex - 1], LV_ANIM_ON);
    }
}

void InputText::insertChar(char c) {
    if (selectedBoxIndex > 0 && selectedBoxIndex <= maxLength) {
        // Insertar el carácter en la casilla seleccionada
        textBuffer[selectedBoxIndex - 1] = c;
        updateBoxes();

        // Mover el selector a la siguiente casilla
        moveNext();
    }
}

int InputText::getSelectedBoxIndex() const {
    return selectedBoxIndex; // Devolver el índice de la casilla seleccionada
}

int InputText::moveNext() {
    if (selectedBoxIndex < maxLength) {
        setSelectedBox(selectedBoxIndex + 1);
        // Desplazar el contenedor para que la casilla seleccionada esté visible
        lv_obj_scroll_to_view(boxes[selectedBoxIndex - 1], LV_ANIM_ON);
    } else {
        setSelectedBox(-1); // Si se llega al final, deseleccionar
    }
    updateSelection();
    return selectedBoxIndex;
}

int InputText::movePrevious() {
    if (selectedBoxIndex > 1) {
        setSelectedBox(selectedBoxIndex - 1);
        // Desplazar el contenedor para que la casilla seleccionada esté visible
        lv_obj_scroll_to_view(boxes[selectedBoxIndex - 1], LV_ANIM_ON);
    } else {
        setSelectedBox(0); // Si se llega al inicio, deseleccionar
    }
    updateSelection();
    return selectedBoxIndex;
}

void InputText::clearSelection() {
    if ((selectedBoxIndex > 0) and (selectedBoxIndex <= maxLength)) {
            lv_obj_set_style_border_color(boxes[selectedBoxIndex-1], lv_color_hex(0x000000), 0);
    }
    selectedBoxIndex=0;
}

void InputText::updateSelection() {
    if ((selectedBoxIndex > 0) and (selectedBoxIndex <= maxLength)) {
        lv_obj_set_style_border_color(boxes[selectedBoxIndex-1], lv_color_hex(0xFF0000), 0);
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
    static char result[50]; // Asegúrate de que el tamaño del buffer sea suficiente
    int resultIndex = 0;
    bool foundNonEmpty = false;

    // Ignorar elementos vacíos al inicio
    for (int i = 0; i < maxLength; i++) {
        if (textBuffer[i] != '\0') {
            foundNonEmpty = true;
        }
        if (foundNonEmpty) {
            result[resultIndex++] = textBuffer[i];
        }
    }

    // Eliminar elementos vacíos al final
    while (resultIndex > 0 && result[resultIndex - 1] == '\0') {
        resultIndex--;
    }

    // Reemplazar elementos vacíos intermedios por espacios en blanco
    for (int i = 0; i < resultIndex; i++) {
        if (result[i] == '\0') {
            result[i] = ' ';
        }
    }

    result[resultIndex] = '\0'; // Asegurar que el resultado esté terminado con un carácter nulo
    return result;
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

lv_obj_t* InputText::getContainer() const {
    return container;
}