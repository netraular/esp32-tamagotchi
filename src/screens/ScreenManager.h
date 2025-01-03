#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "Screen.h"
#include "config.h"
#include <TFT_eSPI.h>
#include <map>
#include <string>
#include <lvgl.h>  // Incluir LVGL

class ScreenManager {
public:
    ScreenManager(TFT_eSPI& tft);
    void init();
    void addScreen(const std::string& name, Screen* screen);
    void setScreen(const std::string& name);
    void update();
    void handleButtons();

private:
    TFT_eSPI& tft;
    std::map<std::string, Screen*> screens;
    Screen* currentScreen;

    // Estructura para almacenar el estado y el tiempo de debounce de cada botón
    struct Button {
        int pin;
        bool state;
        bool pressed; // Indica si el botón está presionado
        bool released; // Indica si el botón se ha liberado
        unsigned long lastDebounceTime;
    };

    // Crear instancias para los botones
    Button buttons[3];

    void lvgl_init();
    static void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map);
};

#endif