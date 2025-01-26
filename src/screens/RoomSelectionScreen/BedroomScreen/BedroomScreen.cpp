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

    if (!LittleFS.exists("/animations/spritesheet_snorlax.png")) {
        Serial.println("¡Archivo PNG no encontrado!");
        return;
    }else{
        Serial.println("¡Archivo PNG Encontrado!");
    }
    // Cargar y mostrar la imagen de animación
    animationImage = lv_img_create(lv_scr_act());
    lv_img_set_src(animationImage, "L:/animations/spritesheet_snorlax.png");
    lv_obj_align(animationImage, LV_ALIGN_CENTER, 0, 30);

    // lv_fs_file_t f;
    // lv_fs_res_t res = lv_fs_open(&f, "L:/test.txt", LV_FS_MODE_RD);
    // if (res == LV_FS_RES_OK) {
    //     char buffer[256];
    //     uint32_t bytes_read;
    //     res = lv_fs_read(&f, buffer, sizeof(buffer) - 1, &bytes_read);
    //     if (res == LV_FS_RES_OK) {
    //         buffer[bytes_read] = '\0'; // Asegurar terminación nula
    //         Serial.print("Contenido de test.txt: ");
    //         Serial.println(buffer);
    //     } else {
    //         Serial.println("Error al leer el archivo");
    //     }
    //     lv_fs_close(&f);
    // } else {
    //     Serial.println("Error al abrir test.txt");
    // }

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