#include "BedroomScreen.h"
#include "../../ScreenManager.h"

extern const lv_img_dsc_t background1; // Imagen de fondo correspondiente
extern const lv_img_dsc_t DefaultAnimationSpriteSheet; // Declarar la imagen de animación
extern ScreenManager screenManager;

void BedroomScreen::load() {
    // Cargar la imagen de fondo
    backgroundImage = lv_img_create(lv_scr_act());
    lv_img_set_src(backgroundImage, &background1);
    lv_obj_set_size(backgroundImage, 128, 160);
    lv_obj_align(backgroundImage, LV_ALIGN_CENTER, 0, 0);

    // Crear la etiqueta de la habitación
    roomLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(roomLabel, "Bedroom");
    lv_obj_set_style_text_font(roomLabel, &lv_font_montserrat_16, 0);
    lv_obj_align(roomLabel, LV_ALIGN_CENTER, 0, 0);

    // Cargar y mostrar la imagen de animación
    lv_obj_t * img;

    img = lv_img_create(lv_scr_act());
    /* Assuming a File system is attached to letter 'A'
     * E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h */
    // Serial.println("ola");
    // lv_image_set_src(img, "L:/animations/spritesheet_snorlax.png");
    // lv_obj_align(img, LV_ALIGN_RIGHT_MID, -20, 0);
    
    // animationImage = lv_img_create(lv_scr_act());
    // lv_img_set_src(animationImage, "A:/animations/spritesheet_snorlax.png");
    // lv_obj_align(animationImage, LV_ALIGN_CENTER, 0, 30);

    Serial.println("BedroomScreen cargada.");
}

void BedroomScreen::update() {
    // No es necesario actualizar nada en esta pantalla
}

void BedroomScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    if (change.button3Changed && state.button3Pressed) {
        screenManager.setScreen("RoomSelectionScreen");
    }
}