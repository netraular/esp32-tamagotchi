#include "MainMenu.h"
#include "../ScreenManager.h"

extern ScreenManager screenManager;

void MainMenu::load() {
    // Crear una etiqueta para el menú
    menuLabel = lv_label_create(lv_scr_act());
    selectedOption = 0; // Inicialmente seleccionar la primera opción
    updateMenu();

    Serial.println("MainMenu cargada.");
}

void MainMenu::update() {
    // No es necesario actualizar nada en esta pantalla
}

void MainMenu::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    // Manejar la navegación con el botón 1
    if (change.button1Changed && state.button1Pressed) {
        selectedOption = (selectedOption + 1) % 2; // Cambiar entre las dos opciones
        updateMenu();
    }

    // Manejar la selección con el botón 2
    if (change.button2Changed && state.button2Pressed) {
        if (selectedOption == 0) {
            screenManager.setScreen("FoodScreen");
        } else if (selectedOption == 1) {
            screenManager.setScreen("ButtonsTestScreen");
        }
    }
}

void MainMenu::updateMenu() {
    // Actualizar el texto del menú según la opción seleccionada
    if (selectedOption == 0) {
        lv_label_set_text(menuLabel, "> FoodScreen\n  ButtonsTestScreen");
    } else {
        lv_label_set_text(menuLabel, "  FoodScreen\n> ButtonsTestScreen");
    }
    lv_obj_align(menuLabel, LV_ALIGN_CENTER, 0, 0); // Centrar el texto en la pantalla
}