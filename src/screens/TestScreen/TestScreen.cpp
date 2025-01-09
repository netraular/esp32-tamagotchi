#include "TestScreen.h"
#include "../ScreenManager.h"

extern ScreenManager screenManager;

void TestScreen::load() {
    // Crear el teclado
    keyboard = new Keyboard(lv_scr_act());
    keyboard->show();

    // Crear una etiqueta para mostrar el valor seleccionado
    outputLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(outputLabel, "Selecciona una letra");
    lv_obj_align(outputLabel, LV_ALIGN_TOP_MID, 0, 10);

    Serial.println("TestScreen cargada.");
}

void TestScreen::update() {
    // Actualizar la etiqueta con la última acción del teclado
    if (keyboard->getLastAction() == Keyboard::Action::LetterSelected) {
        lv_label_set_text_fmt(outputLabel, "Letra seleccionada: %c", keyboard->getSelectedLetter());
    } else if (keyboard->getLastAction() == Keyboard::Action::Exit) {
        lv_label_set_text(outputLabel, "Saliendo del teclado...");
    }
}

void TestScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    // Manejar los botones
    if (change.button1Changed && state.button1Pressed) {
        keyboard->handleButtonEvent(1);
    }
    if (change.button2Changed && state.button2Pressed) {
        keyboard->handleButtonEvent(2);
    }
    if (change.button3Changed && state.button3Pressed) {
        keyboard->handleButtonEvent(3);
    }
}