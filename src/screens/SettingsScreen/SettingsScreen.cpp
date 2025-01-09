#include "SettingsScreen.h"
#include "../ScreenManager.h"

extern ScreenManager screenManager;

void SettingsScreen::load() {
    // Crear una etiqueta para el menú
    menuLabel = lv_label_create(lv_scr_act());
    updateMenu();

    Serial.println("SettingsScreen cargada.");
}

void SettingsScreen::update() {
    // No es necesario actualizar nada en esta pantalla
}

void SettingsScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    // Navegar entre las opciones con el botón 1
    if (change.button1Changed && state.button1Pressed) {
        selectedOption = (selectedOption + 1) % options.size();
        updateMenu();
    }

    // Seleccionar una opción con el botón 2
    if (change.button2Changed && state.button2Pressed) {
        switch (selectedOption) {
            case 0:
                screenManager.setScreen("NicknameScreen");
                break;
            case 1:
                screenManager.setScreen("DateTimeScreen");
                break;
            case 2:
                screenManager.setScreen("SleepTimeScreen");
                break;
            case 3:
                screenManager.setScreen("FavoriteAchievementScreen");
                break;
            case 4:
                screenManager.setScreen("FavoriteItemScreen");
                break;
            case 5:
                screenManager.setScreen("FavoriteFoodScreen");
                break;
            case 6:
                screenManager.setScreen("WifiScreen");
                break;
            case 7:
                screenManager.setScreen("LanguageScreen"); // Nueva opción para seleccionar el idioma
                break;
        }
    }

    // Volver a PetScreen con el botón 3
    if (change.button3Changed && state.button3Pressed) {
        screenManager.setScreen("PetScreen");
    }
}

void SettingsScreen::updateMenu() {
    // Construir el texto del menú
    String menuText = "";
    for (int i = 0; i < options.size(); i++) {
        if (i == selectedOption) {
            menuText += "> " + String(options[i]) + "\n";
        } else {
            menuText += "  " + String(options[i]) + "\n";
        }
    }

    // Actualizar la etiqueta
    lv_label_set_text(menuLabel, menuText.c_str());
    lv_obj_set_style_text_font(menuLabel, &lv_font_montserrat_16, 0);
    lv_obj_align(menuLabel, LV_ALIGN_CENTER, 0, 0);
}