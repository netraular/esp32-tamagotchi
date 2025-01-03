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
        selectedOption = (selectedOption + 1) % 4; // Cambiar entre las cuatro opciones
        updateMenu();
    }

    // Manejar la selección con el botón 2
    if (change.button2Changed && state.button2Pressed) {
        switch (selectedOption) {
            case 0:
                screenManager.setScreen("FoodScreen");
                break;
            case 1:
                screenManager.setScreen("ButtonsTestScreen");
                break;
            case 2:
                screenManager.setScreen("SetupScreen");
                break;
            case 3:
                screenManager.setScreen("RestartScreen");
                break;
        }
    }
}

void MainMenu::updateMenu() {
    const char* menuText = "";  // Inicializar con un valor predeterminado

    switch (selectedOption) {
        case 0:
            menuText = "> FoodScreen\n  ButtonsTestScreen\n  SetupScreen\n  RestartScreen";
            break;
        case 1:
            menuText = "  FoodScreen\n> ButtonsTestScreen\n  SetupScreen\n  RestartScreen";
            break;
        case 2:
            menuText = "  FoodScreen\n  ButtonsTestScreen\n> SetupScreen\n  RestartScreen";
            break;
        case 3:
            menuText = "  FoodScreen\n  ButtonsTestScreen\n  SetupScreen\n> RestartScreen";
            break;
    }

    lv_label_set_text(menuLabel, menuText);
    lv_obj_align(menuLabel, LV_ALIGN_CENTER, 0, 0); // Centrar el texto en la pantalla
}