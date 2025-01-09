#include "TestScreen.h"
#include "../ScreenManager.h"

// Declarar la variable externa de la imagen
extern const lv_img_dsc_t n05ApplePie;

extern ScreenManager screenManager;

void TestScreen::load() {
    // Crear un objeto de imagen
    img = lv_img_create(lv_scr_act());

    // Asignar la imagen desde la variable n05ApplePie
    lv_img_set_src(img, &n05ApplePie);

    // Centrar la imagen en la pantalla
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

    Serial.println("TestScreen cargada.");
}

void TestScreen::update() {
    // No es necesario actualizar nada en esta pantalla
}

void TestScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    // Si se presiona el botón 3, volver a la pantalla MainMenu
    if (change.button3Changed && state.button3Pressed) {
        screenManager.setScreen("MainMenu");
    }
}