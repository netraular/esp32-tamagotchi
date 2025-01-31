#include "KitchenScreen.h"
#include "../../ScreenManager.h"

// Declarar las imágenes de la animación
LV_IMG_DECLARE(bubblePop1);
LV_IMG_DECLARE(bubblePop2);
LV_IMG_DECLARE(bubblePop3);
LV_IMG_DECLARE(bubblePop4);
LV_IMG_DECLARE(bubblePop5);
LV_IMG_DECLARE(bubblePop6);
LV_IMG_DECLARE(bubblePop7);
LV_IMG_DECLARE(bubblePop8);
LV_IMG_DECLARE(bubblePop9);
LV_IMG_DECLARE(bubblePop10);
LV_IMG_DECLARE(bubblePop11);
LV_IMG_DECLARE(bubblePop12);

extern const lv_img_dsc_t background1; // Imagen de fondo correspondiente
extern ScreenManager screenManager;

// Callback que se ejecuta al finalizar la animación
static void animimg_finished_cb(lv_event_t* e) {
    lv_obj_t* animimg = (lv_obj_t*)lv_event_get_target(e); // Cast explícito a lv_obj_t*
    lv_obj_del(animimg); // Eliminar el objeto de la animación
}

void KitchenScreen::load() {
    // Cargar la imagen de fondo
    backgroundImage = lv_img_create(lv_scr_act());
    lv_img_set_src(backgroundImage, &background1);
    lv_obj_set_size(backgroundImage, 240,280);
    lv_obj_align(backgroundImage, LV_ALIGN_CENTER, 0, 0);

    // Crear la etiqueta de la habitación
    roomLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(roomLabel, "Kitchen");
    lv_obj_set_style_text_font(roomLabel, &lv_font_montserrat_16, 0);
    lv_obj_align(roomLabel, LV_ALIGN_CENTER, 0, 0);

    // Crear el array de imágenes para la animación
    static const lv_img_dsc_t* anim_imgs[12] = {
        &bubblePop1,
        &bubblePop2,
        &bubblePop3,
        &bubblePop4,
        &bubblePop5,
        &bubblePop6,
        &bubblePop7,
        &bubblePop8,
        &bubblePop9,
        &bubblePop10,
        &bubblePop11,
        &bubblePop12
    };

    // Crear el objeto de animación de imágenes
    animimg = lv_animimg_create(lv_scr_act());
    lv_animimg_set_src(animimg, (const void**) anim_imgs, 12); // Cast a const void**
    lv_animimg_set_duration(animimg, 1000); // Duración de la animación en milisegundos
    lv_animimg_set_repeat_count(animimg, 1); // Repetir solo una vez
    lv_obj_set_size(animimg, 32, 32); // Tamaño de la animación (32x32 px)
    lv_obj_set_pos(animimg, 50, 50); // Posición de la animación (coordenadas x, y)

    // Registrar el callback para cuando la animación termine
    lv_obj_add_event_cb(animimg, animimg_finished_cb, LV_EVENT_READY, nullptr);

    lv_animimg_start(animimg); // Iniciar la animación

    Serial.println("KitchenScreen cargada.");
}

void KitchenScreen::update() {
    // No es necesario actualizar nada aquí, LVGL maneja la animación automáticamente
}

void KitchenScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    if (change.button3Changed && state.button3Pressed) {
        screenManager.setScreen("RoomSelectionScreen");
    }
}