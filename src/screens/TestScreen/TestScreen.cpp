#include "TestScreen.h"
#include "../ScreenManager.h"

extern ScreenManager screenManager;

void TestScreen::load() {
    // Crear el teclado
    keyboard = new Keyboard(lv_scr_act());
    keyboard->show();

    // Configurar el callback para recibir la letra seleccionada
    keyboard->setOnLetterSelectedCallback([this](char letter) {
        lv_label_set_text_fmt(outputLabel, "Letra enviada: %c", letter);
    });

    // Crear una etiqueta para mostrar el valor seleccionado
    outputLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(outputLabel, "Selecciona una letra");
    lv_obj_set_style_text_font(outputLabel, &lv_font_montserrat_12, 0); // Usar fuente 12
    lv_obj_align(outputLabel, LV_ALIGN_TOP_MID, 0, 10); // Alinear en la parte superior

    Serial.println("TestScreen cargada.");
}

void TestScreen::update() {
    // No es necesario actualizar nada en esta pantalla
}

void TestScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    // Manejar los botones
    if (change.button1Changed && state.button1Pressed) {
        keyboard->handleButtonEvent(1); // Pasar el evento al teclado
    }
    if (change.button2Changed && state.button2Pressed) {
        keyboard->handleButtonEvent(2); // Pasar el evento al teclado
    }
    if (change.button3Changed && state.button3Pressed) {
        keyboard->handleButtonEvent(3); // Pasar el evento al teclado
    }
}