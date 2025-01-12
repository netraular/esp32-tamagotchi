#include "RoomSelectionScreen.h"
#include "../ScreenManager.h"

extern const lv_img_dsc_t background1;
extern const lv_img_dsc_t background2;
extern const lv_img_dsc_t background3;
extern const lv_img_dsc_t background4;
extern const lv_img_dsc_t background5;
extern const lv_img_dsc_t background6;
extern const lv_img_dsc_t ArrowLeft;
extern const lv_img_dsc_t ArrowRight;

extern ScreenManager screenManager;

void RoomSelectionScreen::load() {
    // Inicializar las habitaciones disponibles en el orden especificado
    roomNames = {"Bedroom", "Bathroom", "Kitchen", "LivingRoom", "Garden", "Outside"};
    roomImages = {&background1, &background2, &background3, &background4, &background5, &background6};

    // Crear la imagen de fondo
    backgroundImage = lv_img_create(lv_scr_act());
    lv_img_set_src(backgroundImage, roomImages[selectedRoomIndex]);
    lv_obj_set_size(backgroundImage, 128, 160); // Ajustar al tamaño de la pantalla
    lv_obj_align(backgroundImage, LV_ALIGN_CENTER, 0, 0);

    // Crear una etiqueta para mostrar el nombre de la habitación
    roomLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(roomLabel, roomNames[selectedRoomIndex]);
    lv_obj_set_style_text_font(roomLabel, &lv_font_montserrat_16, 0);
    lv_obj_align(roomLabel, LV_ALIGN_BOTTOM_MID, 0, -10); // Alinear en la parte inferior

    // Crear las flechas izquierda y derecha
    arrowLeft = lv_img_create(lv_scr_act());
    lv_img_set_src(arrowLeft, &ArrowLeft);
    lv_obj_set_size(arrowLeft, 16, 16); // Tamaño de la flecha
    lv_obj_align(arrowLeft, LV_ALIGN_LEFT_MID, 10, 0); // Alinear a la izquierda

    arrowRight = lv_img_create(lv_scr_act());
    lv_img_set_src(arrowRight, &ArrowRight);
    lv_obj_set_size(arrowRight, 16, 16); // Tamaño de la flecha
    lv_obj_align(arrowRight, LV_ALIGN_RIGHT_MID, -10, 0); // Alinear a la derecha

    // Configurar animaciones para las flechas
    lv_anim_init(&animLeft);
    lv_anim_set_var(&animLeft, arrowLeft);
    lv_anim_set_exec_cb(&animLeft, (lv_anim_exec_xcb_t)lv_obj_set_x);
    lv_anim_set_values(&animLeft, 10, 15); // Mover 5 píxeles a la derecha
    lv_anim_set_time(&animLeft, 500); // Duración de 500 ms
    lv_anim_set_playback_time(&animLeft, 500); // Tiempo de vuelta a la posición original
    lv_anim_set_repeat_count(&animLeft, LV_ANIM_REPEAT_INFINITE); // Repetir infinitamente
    lv_anim_start(&animLeft);

    lv_anim_init(&animRight);
    lv_anim_set_var(&animRight, arrowRight);
    lv_anim_set_exec_cb(&animRight, (lv_anim_exec_xcb_t)lv_obj_set_x);
    lv_anim_set_values(&animRight, -10, -15); // Mover 5 píxeles a la izquierda
    lv_anim_set_time(&animRight, 500); // Duración de 500 ms
    lv_anim_set_playback_time(&animRight, 500); // Tiempo de vuelta a la posición original
    lv_anim_set_repeat_count(&animRight, LV_ANIM_REPEAT_INFINITE); // Repetir infinitamente
    lv_anim_start(&animRight);

    // Actualizar la visualización inicial
    updateRoomDisplay();

    Serial.println("RoomSelectionScreen cargada.");
}

void RoomSelectionScreen::update() {
    // No es necesario actualizar nada en esta pantalla
}

void RoomSelectionScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    // Navegar entre las habitaciones con el botón 1 (anterior) y el botón 3 (siguiente)
    if (change.button1Changed && state.button1Pressed) {
        if (selectedRoomIndex > 0) { // Si hay una habitación a la izquierda
            selectedRoomIndex--; // Mover a la habitación anterior
            updateRoomDisplay();
        }
    }
    if (change.button3Changed && state.button3Pressed) {
        if (selectedRoomIndex < roomNames.size() - 1) { // Si hay una habitación a la derecha
            selectedRoomIndex++; // Mover a la siguiente habitación
            updateRoomDisplay();
        }
    }

    // Seleccionar la habitación con el botón 2
    if (change.button2Changed && state.button2Pressed) {
        const char* selectedRoom = roomNames[selectedRoomIndex];
        screenManager.setScreen(selectedRoom); // Cargar la pantalla correspondiente
    }
}

void RoomSelectionScreen::updateRoomDisplay() {
    // Actualizar la imagen de fondo
    lv_img_set_src(backgroundImage, roomImages[selectedRoomIndex]);

    // Actualizar el nombre de la habitación
    lv_label_set_text(roomLabel, roomNames[selectedRoomIndex]);

    // Mostrar u ocultar las flechas según la posición actual
    if (selectedRoomIndex > 0) {
        lv_obj_clear_flag(arrowLeft, LV_OBJ_FLAG_HIDDEN); // Mostrar flecha izquierda
    } else {
        lv_obj_add_flag(arrowLeft, LV_OBJ_FLAG_HIDDEN); // Ocultar flecha izquierda
    }

    if (selectedRoomIndex < roomNames.size() - 1) {
        lv_obj_clear_flag(arrowRight, LV_OBJ_FLAG_HIDDEN); // Mostrar flecha derecha
    } else {
        lv_obj_add_flag(arrowRight, LV_OBJ_FLAG_HIDDEN); // Ocultar flecha derecha
    }
}