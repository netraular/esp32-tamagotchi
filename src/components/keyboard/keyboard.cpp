#include "Keyboard.h"
#include <string> // Incluir la biblioteca <string>
#include <Arduino.h> // Incluir la biblioteca Arduino para usar Serial

Keyboard::Keyboard(lv_obj_t* parent) : selectedRowIndex(0), selectedLetterIndex(0), currentMode(Mode::SelectRow) {
    // Crear el contenedor del teclado
    keyboardContainer = lv_obj_create(parent);
    lv_obj_set_size(keyboardContainer, 128, 80); // Tamaño del contenedor (128px de ancho x 80px de alto)
    lv_obj_align(keyboardContainer, LV_ALIGN_BOTTOM_MID, 0, 0); // Alinear en la parte inferior de la pantalla

    // Desactivar las barras de desplazamiento
    lv_obj_set_scrollbar_mode(keyboardContainer, LV_SCROLLBAR_MODE_OFF); // Desactivar barras de desplazamiento

    // Definir las filas de letras
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

    // Crear el teclado
    createKeyboard();
    updateSelection(); // Resaltar la fila seleccionada

    Serial.println("Teclado inicializado.");
}

void Keyboard::show() {
    lv_obj_clear_flag(keyboardContainer, LV_OBJ_FLAG_HIDDEN); // Mostrar el teclado
    Serial.println("Teclado mostrado.");
}

void Keyboard::hide() {
    lv_obj_add_flag(keyboardContainer, LV_OBJ_FLAG_HIDDEN); // Ocultar el teclado
    Serial.println("Teclado ocultado.");
}

bool Keyboard::isVisible() const {
    return !lv_obj_has_flag(keyboardContainer, LV_OBJ_FLAG_HIDDEN); // Verificar si el teclado está visible
}

void Keyboard::handleButtonEvent(int button) {
    if (currentMode == Mode::SelectRow) {
        if (button == 1) {
            // Cambiar a la siguiente fila
            selectedRowIndex = (selectedRowIndex + 1) % rows.size();
            Serial.printf("Fila seleccionada: %d\n", selectedRowIndex);
        } else if (button == 2) {
            // Seleccionar la fila actual y entrar en el modo de selección de letras
            selectLetter();
            Serial.println("Modo de selección de letra activado");
        } else if (button == 3) {
            // Salir del teclado
            Serial.println("Botón 3 presionado: Saliendo del teclado");
            if (onLetterSelectedCallback) {
                onLetterSelectedCallback("exit"); // Notificar la salida del teclado
            }
            hide(); // Ocultar el teclado
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
                    onLetterSelectedCallback(std::string(1, selectedLetter)); // Notificar la letra seleccionada
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
        lv_obj_scroll_to_view(rowContainers[selectedRowIndex], LV_ANIM_ON);
    }
}

void Keyboard::setOnLetterSelectedCallback(std::function<void(std::string)> callback) {
    onLetterSelectedCallback = callback;
}

char Keyboard::getSelectedLetter() const {
    if (currentMode == Mode::SelectLetter) {
        return rows[selectedRowIndex][selectedLetterIndex]; // Devolver la letra seleccionada
    }
    return '\0'; // Devolver un valor vacío si no se ha seleccionado una letra
}

void Keyboard::createKeyboard() {
    const int buttonSize = 16; // Tamaño de cada botón (16x16px)
    const int spacing = 2; // Espacio entre botones
    const int rowHeight = 20; // Altura de cada fila
    const int rowWidth = 108; // Ancho de cada fila (ajustado para 6 botones)

    // Crear los botones para cada fila
    for (int i = 0; i < rows.size(); i++) {
        // Crear un contenedor para la fila
        lv_obj_t* rowContainer = lv_obj_create(keyboardContainer);
        lv_obj_set_size(rowContainer, rowWidth, rowHeight); // Tamaño del contenedor de la fila
        lv_obj_align(rowContainer, LV_ALIGN_TOP_LEFT, -5, i * (rowHeight + spacing)); // Alinear en la parte superior del teclado, movido 5px a la izquierda

        // Estilo del contenedor de la fila
        lv_obj_set_style_bg_color(rowContainer, lv_color_hex(0xF5F5DC), 0); // Color crema
        lv_obj_set_style_border_color(rowContainer, lv_color_hex(0x808080), 0); // Borde gris
        lv_obj_set_style_border_width(rowContainer, 1, 0); // Grosor del borde

        // Eliminar el margen interno (padding)
        lv_obj_set_style_pad_all(rowContainer, 0, 0); // Sin padding

        // Desactivar las barras de desplazamiento
        lv_obj_set_scrollbar_mode(rowContainer, LV_SCROLLBAR_MODE_OFF); // Desactivar barras de desplazamiento

        // Crear los botones dentro del contenedor de la fila
        std::vector<lv_obj_t*> buttons;
        for (int j = 0; j < rows[i].size(); j++) {
            // Crear un botón
            lv_obj_t* btn = lv_btn_create(rowContainer);
            lv_obj_set_size(btn, buttonSize, buttonSize);
            lv_obj_align(btn, LV_ALIGN_TOP_LEFT, j * (buttonSize + spacing), 0);

            // Estilo del botón: color crema y bordes grises
            lv_obj_set_style_bg_color(btn, lv_color_hex(0xF5F5DC), 0); // Color crema
            lv_obj_set_style_border_color(btn, lv_color_hex(0x808080), 0); // Borde gris
            lv_obj_set_style_border_width(btn, 1, 0); // Grosor del borde

            // Aplicar bordes solo a los lados
            lv_obj_set_style_border_side(btn, LV_BORDER_SIDE_LEFT, 0); // Borde izquierdo
            lv_obj_set_style_border_side(btn, LV_BORDER_SIDE_RIGHT, 0); // Borde derecho

            // Añadir una etiqueta con la letra (o vacía si es '\0')
            lv_obj_t* label = lv_label_create(btn);
            if (rows[i][j] != '\0') {
                lv_label_set_text(label, std::string(1, rows[i][j]).c_str()); // Convertir char a string
            } else {
                lv_label_set_text(label, ""); // Casilla vacía
            }
            lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0); // Usar fuente 12
            lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0); // Color negro
            lv_obj_center(label);

            buttons.push_back(btn);
        }

        rowContainers.push_back(rowContainer); // Guardar el contenedor de la fila
        letterButtons.push_back(buttons); // Guardar los botones de la fila
    }

    Serial.println("Teclado creado con éxito.");
}

void Keyboard::updateSelection() {
    // Actualizar el borde de todas las filas
    for (int i = 0; i < rowContainers.size(); i++) {
        if (i == selectedRowIndex && currentMode == Mode::SelectRow) {
            // Resaltar la fila seleccionada con un borde rojo
            lv_obj_set_style_border_color(rowContainers[i], lv_color_hex(0xFF0000), 0);
        } else {
            // Restaurar el borde gris para las filas no seleccionadas
            lv_obj_set_style_border_color(rowContainers[i], lv_color_hex(0x808080), 0);
        }
    }

    // Actualizar el resaltado de las letras
    if (currentMode == Mode::SelectLetter) {
        for (int j = 0; j < letterButtons[selectedRowIndex].size(); j++) {
            lv_obj_t* label = lv_obj_get_child(letterButtons[selectedRowIndex][j], 0); // Obtener la etiqueta del botón
            if (j == selectedLetterIndex) {
                // Resaltar la letra seleccionada con color rojo
                lv_obj_set_style_text_color(label, lv_color_hex(0xFF0000), 0);
            } else {
                // Restaurar el color negro para las letras no seleccionadas
                lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0);
            }
        }
    } else if (currentMode == Mode::SelectRow) {
        // Restaurar el color de todas las letras de la fila seleccionada al volver al modo de selección de fila
        for (int j = 0; j < letterButtons[selectedRowIndex].size(); j++) {
            lv_obj_t* label = lv_obj_get_child(letterButtons[selectedRowIndex][j], 0); // Obtener la etiqueta del botón
            lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0); // Color negro
        }
    }
}

void Keyboard::selectLetter() {
    currentMode = Mode::SelectLetter; // Cambiar al modo de selección de letras
    selectedLetterIndex = 0; // Seleccionar la primera letra de la fila
}

void Keyboard::selectRow() {
    currentMode = Mode::SelectRow; // Cambiar al modo de selección de filas
    selectedLetterIndex = 0; // Reiniciar la selección de letra
}

bool Keyboard::isSelectingLetter() const {
    return currentMode == Mode::SelectLetter;
}

bool Keyboard::isSelectingRow() const {
    return currentMode == Mode::SelectRow;
}