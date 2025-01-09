#include "LanguageScreen.h"
#include "../../ScreenManager.h"

extern ScreenManager screenManager;

void LanguageScreen::load() {
    // Crear una etiqueta para el menú de idiomas
    menuLabel = lv_label_create(lv_scr_act());
    updateMenu();

    Serial.println("LanguageScreen cargada.");
}

void LanguageScreen::update() {
    // No es necesario actualizar nada en esta pantalla
}

void LanguageScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    // Navegar entre los idiomas con el botón 1
    if (change.button1Changed && state.button1Pressed) {
        selectedLanguage = (selectedLanguage + 1) % languages.size();
        updateMenu();
    }

    // Seleccionar un idioma con el botón 2
    if (change.button2Changed && state.button2Pressed) {
        // Aquí puedes implementar la lógica para cambiar el idioma
        Serial.printf("Idioma seleccionado: %s\n", languages[selectedLanguage]);
    }

    // Volver a SettingsScreen con el botón 3
    if (change.button3Changed && state.button3Pressed) {
        screenManager.setScreen("SettingsScreen");
    }
}

void LanguageScreen::updateMenu() {
    // Construir el texto del menú
    String menuText = "";
    for (int i = 0; i < languages.size(); i++) {
        if (i == selectedLanguage) {
            menuText += "> " + String(languages[i]) + "\n";
        } else {
            menuText += "  " + String(languages[i]) + "\n";
        }
    }

    // Actualizar la etiqueta
    lv_label_set_text(menuLabel, menuText.c_str());
    lv_obj_set_style_text_font(menuLabel, &lv_font_montserrat_16, 0);
    lv_obj_align(menuLabel, LV_ALIGN_CENTER, 0, 0);
}