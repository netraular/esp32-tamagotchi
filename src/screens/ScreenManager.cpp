#include "ScreenManager.h"
#include "config.h" // Incluir config.h
#include <lvgl.h>

ScreenManager::ScreenManager(TFT_eSPI& tft) : tft(tft), currentScreen(nullptr) {
    // Inicializar los botones
    buttons[0].pin = BUTTON1_PIN;
    buttons[0].state = HIGH;
    buttons[0].pressed = false; // Inicialmente, el botón no está presionado
    buttons[0].released = true; // Inicialmente, el botón está liberado
    buttons[0].lastDebounceTime = 0;

    buttons[1].pin = BUTTON2_PIN;
    buttons[1].state = HIGH;
    buttons[1].pressed = false; // Inicialmente, el botón no está presionado
    buttons[1].released = true; // Inicialmente, el botón está liberado
    buttons[1].lastDebounceTime = 0;

    buttons[2].pin = BUTTON3_PIN;
    buttons[2].state = HIGH;
    buttons[2].pressed = false; // Inicialmente, el botón no está presionado
    buttons[2].released = true; // Inicialmente, el botón está liberado
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

        // Imprimir el estado y los cambios de los botones
        Serial.print("State: B1=");
        Serial.print(currentState.button1Pressed ? "ON " : "OFF ");
        Serial.print("B2=");
        Serial.print(currentState.button2Pressed ? "ON " : "OFF ");
        Serial.print("B3=");
        Serial.print(currentState.button3Pressed ? "ON " : "OFF ");
        Serial.print(" | Change: B1=");
        Serial.print(change.button1Changed ? "YES " : "NO ");
        Serial.print("B2=");
        Serial.print(change.button2Changed ? "YES " : "NO ");
        Serial.print("B3=");
        Serial.println(change.button3Changed ? "YES" : "NO");

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

    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf[TFT_WIDTH * 10];
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, TFT_WIDTH * 10);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = TFT_WIDTH;
    disp_drv.ver_res = TFT_HEIGHT;
    disp_drv.flush_cb = [](lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
        ((ScreenManager*)disp->user_data)->my_disp_flush(disp, area, color_p);
    };
    disp_drv.user_data = this;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
}

void ScreenManager::my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}