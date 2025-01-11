#include "TestScreen2.h"
#include "../ScreenManager.h"

extern ScreenManager screenManager;

void TestScreen2::load() {
    // Crear el componente InputTextWithKeyboard
    inputTextWithKeyboard = new InputTextWithKeyboard(lv_scr_act(), 6);
    inputTextWithKeyboard->show();

    // Mover el InputText 25 píxeles más abajo
    inputTextWithKeyboard->setInputPosition(0, 25); // Ajustar la posición vertical

    // Configurar el callback para recibir el texto ingresado
    inputTextWithKeyboard->setOnTextEnteredCallback([this](const char* text) {
        if (strcmp(text, "exitLeft") == 0) {
            // Si el texto es "exitLeft", ir a MainMenu
            screenManager.setScreen("MainMenu");
        } else if (strcmp(text, "exitRight") == 0) {
            // Si el texto es "exitRight", ir a PetScreen
            screenManager.setScreen("PetScreen");
        } else {
            // Mostrar el texto ingresado en la etiqueta
            char buffer[50]; // Asegúrate de que el tamaño del buffer sea suficiente
            snprintf(buffer, sizeof(buffer), "Nombre: %s", text);
            lv_label_set_text(outputLabel, buffer);
        }
    });

    // Crear una etiqueta para mostrar el valor seleccionado
    outputLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(outputLabel, "Nombre");
    lv_obj_set_style_text_font(outputLabel, &lv_font_montserrat_12, 0); // Usar fuente 12
    lv_obj_align(outputLabel, LV_ALIGN_TOP_MID, 0, 10); // Alinear en la parte superior

    Serial.println("TestScreen2 cargada.");
}

void TestScreen2::update() {
    // No es necesario actualizar nada en esta pantalla
}

void TestScreen2::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    // Pasar los eventos de los botones al componente InputTextWithKeyboard
    inputTextWithKeyboard->handleButtonEvent(state, change);
}