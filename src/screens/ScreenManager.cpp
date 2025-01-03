#include "ScreenManager.h"
#include "config.h"

ScreenManager::ScreenManager(TFT_eSPI& tft) : tft(tft), currentScreen(nullptr) {
    // Inicializar los botones
    buttons[0].pin = BUTTON1_PIN;
    buttons[0].state = HIGH;
    buttons[0].pressed = false;
    buttons[0].released = true;
    buttons[0].lastDebounceTime = 0;

    buttons[1].pin = BUTTON2_PIN;
    buttons[1].state = HIGH;
    buttons[1].pressed = false;
    buttons[1].released = true;
    buttons[1].lastDebounceTime = 0;

    buttons[2].pin = BUTTON3_PIN;
    buttons[2].state = HIGH;
    buttons[2].pressed = false;
    buttons[2].released = true;
    buttons[2].lastDebounceTime = 0;
}

void ScreenManager::init() {
    lvgl_init();
}

void ScreenManager::addScreen(const std::string& name, Screen* screen) {
    screens[name] = screen;
}

void ScreenManager::setScreen(const std::string& name) {
    if (screens.find(name) != screens.end()) {
        if (currentScreen != nullptr) {
            // Limpiar la pantalla actual si es necesario
            lv_obj_clean(lv_scr_act());
        }
        currentScreen = screens[name];
        currentScreen->load();
        Serial.print("Pantalla cargada: ");
        Serial.println(name.c_str());
    } else {
        Serial.println("Error: Pantalla no encontrada.");
    }
}

void ScreenManager::update() {
    if (currentScreen != nullptr) {
        currentScreen->update();
    }
    lv_timer_handler();
}

void ScreenManager::handleButtons() {
    static ButtonState previousState = {false, false, false}; // Estado anterior de los botones
    ButtonState currentState = {
        digitalRead(BUTTON1_PIN) == LOW,
        digitalRead(BUTTON2_PIN) == LOW,
        digitalRead(BUTTON3_PIN) == LOW
    };

    // Detectar cambios en los botones
    ButtonChange change = {
        currentState.button1Pressed != previousState.button1Pressed,
        currentState.button2Pressed != previousState.button2Pressed,
        currentState.button3Pressed != previousState.button3Pressed
    };

    // Verificar si al menos un botón ha cambiado
    if (change.button1Changed || change.button2Changed || change.button3Changed) {
        // Notificar a la pantalla actual sobre los cambios en los botones
        if (currentScreen != nullptr) {
            currentScreen->handleButtonEvent(currentState, change);
        }
    }

    // Actualizar el estado anterior
    previousState = currentState;
}

void ScreenManager::lvgl_init() {
    tft.begin();
    tft.fillScreen(TFT_BLACK);
    lv_init();

    // Crear un buffer de dibujo
    static lv_color_t buf[TFT_WIDTH * 10];  // Buffer para 10 líneas de la pantalla
    lv_draw_buf_t draw_buf;
    lv_draw_buf_init(&draw_buf, TFT_WIDTH, 10, LV_COLOR_FORMAT_NATIVE, TFT_WIDTH * sizeof(lv_color_t), buf, sizeof(buf));

    // Crear un display
    lv_display_t *disp = lv_display_create(TFT_WIDTH, TFT_HEIGHT);

    // Configurar los buffers del display
    lv_display_set_buffers(disp, buf, NULL, sizeof(buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

    // Configurar la función de flush
    lv_display_set_flush_cb(disp, my_disp_flush);

    // Configurar el display
    lv_display_set_driver_data(disp, this);
}

void ScreenManager::my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    ScreenManager* manager = (ScreenManager*)lv_display_get_driver_data(disp);
    manager->tft.startWrite();
    manager->tft.setAddrWindow(area->x1, area->y1, w, h);
    manager->tft.pushColors((uint16_t *)px_map, w * h, true);
    manager->tft.endWrite();

    lv_display_flush_ready(disp);
}