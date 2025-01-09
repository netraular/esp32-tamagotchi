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
            lv_label_set_text(outputLabel, "Pulsa el botón 1 para usar el teclado");
        } else {
            Serial.printf("Callback: Letra enviada: %s\n", value.c_str());
            lv_label_set_text_fmt(outputLabel, "Letra enviada: %s", value.c_str());
        }
    });

    // Crear una etiqueta para mostrar el valor seleccionado
    outputLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(outputLabel, "Pulsa el botón 1 para usar el teclado");
    lv_obj_set_style_text_font(outputLabel, &lv_font_montserrat_12, 0); // Usar fuente 12
    lv_obj_align(outputLabel, LV_ALIGN_TOP_MID, 0, 10); // Alinear en la parte superior

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
        // Si el teclado no está activo, manejar el botón 1 para activarlo
        if (change.button1Changed && state.button1Pressed) {
            Serial.println("Botón 1 presionado: Activando el teclado");
            isKeyboardActive = true;
            keyboard->show();
            lv_label_set_text(outputLabel, "Selecciona una letra");
        }
        // Si el teclado no está activo y se pulsa el botón 3, volver al menú principal
        if (change.button3Changed && state.button3Pressed) {
            Serial.println("Botón 3 presionado: Volviendo al menú principal");
            screenManager.setScreen("MainMenu");
        }
    }
}