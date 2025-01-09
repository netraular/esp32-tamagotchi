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
    // Navegar entre las opciones con el botón 1
    if (change.button1Changed && state.button1Pressed) {
        selectedOption = (selectedOption + 1) % 7; // Cambiar entre las siete opciones
        updateMenu();
    }

    // Seleccionar una opción con el botón 2
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
            case 4:
                screenManager.setScreen("ShopScreen");
                break;
            case 5:
                screenManager.setScreen("PetStatsScreen");
                break;
            case 6:
                screenManager.setScreen("SettingsScreen"); // Nueva opción para acceder a SettingsScreen
                break;
        }
    }

    // Si se presiona el botón 3, ir a PetScreen
    if (change.button3Changed && state.button3Pressed) {
        screenManager.setScreen("PetScreen");
    }
}

void MainMenu::updateMenu() {
    // Construir el texto del menú
    const char* menuText = "";  // Inicializar con un valor predeterminado

    switch (selectedOption) {
        case 0:
            menuText = "> FoodScreen\n  ButtonsTestScreen\n  SetupScreen\n  RestartScreen\n  ShopScreen\n  PetStatsScreen\n  SettingsScreen";
            break;
        case 1:
            menuText = "  FoodScreen\n> ButtonsTestScreen\n  SetupScreen\n  RestartScreen\n  ShopScreen\n  PetStatsScreen\n  SettingsScreen";
            break;
        case 2:
            menuText = "  FoodScreen\n  ButtonsTestScreen\n> SetupScreen\n  RestartScreen\n  ShopScreen\n  PetStatsScreen\n  SettingsScreen";
            break;
        case 3:
            menuText = "  FoodScreen\n  ButtonsTestScreen\n  SetupScreen\n> RestartScreen\n  ShopScreen\n  PetStatsScreen\n  SettingsScreen";
            break;
        case 4:
            menuText = "  FoodScreen\n  ButtonsTestScreen\n  SetupScreen\n  RestartScreen\n> ShopScreen\n  PetStatsScreen\n  SettingsScreen";
            break;
        case 5:
            menuText = "  FoodScreen\n  ButtonsTestScreen\n  SetupScreen\n  RestartScreen\n  ShopScreen\n> PetStatsScreen\n  SettingsScreen";
            break;
        case 6:
            menuText = "  FoodScreen\n  ButtonsTestScreen\n  SetupScreen\n  RestartScreen\n  ShopScreen\n  PetStatsScreen\n> SettingsScreen";
            break;
    }

    // Actualizar la etiqueta del menú
    lv_label_set_text(menuLabel, menuText);
    lv_obj_set_style_text_font(menuLabel, &lv_font_montserrat_12, 0);
    lv_obj_align(menuLabel, LV_ALIGN_CENTER, 0, 0); // Centrar el texto en la pantalla
}