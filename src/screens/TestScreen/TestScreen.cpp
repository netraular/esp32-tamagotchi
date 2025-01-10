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
            lv_label_set_text(outputLabel, "Press 1 to start");
        } else {
            Serial.printf("Callback: Letra enviada: %s\n", value.c_str());
            inputText->insertChar(value[0]); // Insertar la letra en la casilla seleccionada
        }
    });

    // Crear una etiqueta para mostrar el valor seleccionado
    outputLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(outputLabel, "Press 1 to start");
    lv_obj_set_style_text_font(outputLabel, &lv_font_montserrat_12, 0); // Usar fuente 12
    lv_obj_align(outputLabel, LV_ALIGN_TOP_MID, 0, 10); // Alinear en la parte superior

    // Crear el componente InputText
    inputText = new InputText(lv_scr_act(), 6); // 13 caracteres máximos
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
            // Avanzar al siguiente índice
            std::string result = inputText->moveNext();
            if (result == "End") {
                Serial.println("Se alcanzó el final del InputText.");
                Serial.print("Nombre: ");
                Serial.println(inputText->getText());
            } else {
                Serial.printf("Casilla seleccionada: %s\n", result.c_str());
            }
        }
        if (change.button2Changed && state.button2Pressed) {
            // Mostrar el teclado para editar la casilla seleccionada
            if (inputText->isAnyBoxSelected()) {
                Serial.println("Botón 2 presionado: Activando el teclado");
                isKeyboardActive = true;
                keyboard->show();
                lv_label_set_text(outputLabel, "Selecciona una letra");
            } else {
                Serial.println("Ninguna casilla seleccionada. Presiona el botón 1 para empezar.");
            }
        }
        if (change.button3Changed && state.button3Pressed) {
            // Retroceder al índice anterior
            std::string result = inputText->movePrevious();
            if (result == "Start") {
                if (!inputText->isAnyBoxSelected()) {
                    // Si no hay ninguna casilla seleccionada, volver al menú principal
                    Serial.println("Botón 3 presionado: Volviendo al menú principal");
                    screenManager.setScreen("MainMenu");
                } else {
                    Serial.println("Se alcanzó el inicio del InputText.");
                }
            } else {
                Serial.printf("Casilla seleccionada: %s\n", result.c_str());
            }
        }
    }
}