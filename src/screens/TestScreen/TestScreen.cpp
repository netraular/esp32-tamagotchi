#include "TestScreen.h"
#include "../ScreenManager.h"

extern ScreenManager screenManager;

void TestScreen::load() {
    // Crear el teclado
    keyboard = new Keyboard(lv_scr_act());
    keyboard->hide(); // Ocultar el teclado inicialmente 
    isKeyboardActive = false; // El teclado no está activo inicialmente

    // Configurar el callback para recibir la letra seleccionada o la salida del teclado
    keyboard->setOnLetterSelectedCallback([this](std::string value) {
        if (value == "exit") {
            Serial.println("Callback: Saliendo del teclado");
            isKeyboardActive = false;
            lv_label_set_text(outputLabel, "Nombre");
        } else {
            Serial.printf("Callback: Letra enviada: %s\n", value.c_str());
            inputText->insertChar(value[0]); // Insertar la letra en la casilla seleccionada
        }
    });

    // Crear una etiqueta para mostrar el valor seleccionado
    outputLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(outputLabel, "Nombre");
    lv_obj_set_style_text_font(outputLabel, &lv_font_montserrat_12, 0); // Usar fuente 12
    lv_obj_align(outputLabel, LV_ALIGN_TOP_MID, 0, 10); // Alinear en la parte superior

    // Crear el componente InputText
    inputText = new InputText(lv_scr_act(), 6); // 6 caracteres máximos
    inputText->show(); // Mostrar el InputText
    lv_obj_align(inputText->getContainer(), LV_ALIGN_CENTER, 0, -20); // Alinear en el centro, justo debajo del outputLabel

    Serial.println("TestScreen cargada.");
}

void TestScreen::update() {
    // No es necesario actualizar nada en esta pantalla
}

void TestScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    if (isKeyboardActive) {
        // Si el teclado está activo, pasar los eventos al teclado
        if (change.button1Changed && state.button1Pressed) {
            Serial.println("Botón 1 presionado: Pasando evento al teclado");
            keyboard->handleButtonEvent(1); // Pasar el evento al teclado
        }
        if (change.button2Changed && state.button2Pressed) {
            Serial.println("Botón 2 presionado: Pasando evento al teclado");
            keyboard->handleButtonEvent(2); // Pasar el evento al teclado
        }
        if (change.button3Changed && state.button3Pressed) {
            Serial.println("Botón 3 presionado: Pasando evento al teclado");
            keyboard->handleButtonEvent(3); // Pasar el evento al teclado
        }
    } else {
        // Si el teclado no está activo, manejar los botones
        if (change.button1Changed && state.button1Pressed) {
            int currentIndex = inputText->getSelectedBoxIndex();

            if (currentIndex != -1) {
                // Si hay una casilla seleccionada, avanzar al siguiente índice
                inputText->moveNext();
            } else {
                // Si no hay ninguna casilla seleccionada, mostrar el texto ingresado
                const char* text = inputText->getText();
                Serial.printf("Texto ingresado: %s\n", text);
                char buffer[50]; // Asegúrate de que el tamaño del buffer sea suficiente
                snprintf(buffer, sizeof(buffer), "Nombre: %s", text);
                lv_label_set_text(outputLabel, buffer);
            }
        }
        if (change.button2Changed && state.button2Pressed) {
            int currentIndex = inputText->getSelectedBoxIndex();

            // Mostrar el teclado para editar la casilla seleccionada
            if (currentIndex != -1 and currentIndex != 0) {
                Serial.println("Botón 2 presionado: Activando el teclado");
                isKeyboardActive = true;
                keyboard->show();
                lv_label_set_text(outputLabel, "Selecciona una letra");
            } else {
                Serial.println("Ninguna casilla seleccionada. Presiona el botón 1 para empezar.");
            }
        }
        if (change.button3Changed && state.button3Pressed) {
            int currentIndex = inputText->getSelectedBoxIndex();

            if (currentIndex != 0) {
                // Si la posición del selector no es 0, retroceder al índice anterior
                inputText->movePrevious();
            } else {
                // Si la posición del selector es 0, volver al menú principal
                Serial.println("Botón 3 presionado: Volviendo al menú principal");
                screenManager.setScreen("MainMenu");
            }
        }
    }
}