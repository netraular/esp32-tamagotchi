#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "Screen.h"
#include "config.h"
#include <TFT_eSPI.h>
#include <map>
#include <string>
#include <lvgl.h>  // Include LVGL

/**
 * @class ScreenManager
 * @brief Manages the application's screens and handles navigation between them.
 * 
 * This class is responsible for initializing LVGL, registering screens, and managing
 * the active screen. It also handles button events and updates the display at a
 * consistent frame rate dictated by main.cpp.
 */
class ScreenManager {
public:
    ScreenManager(TFT_eSPI& tft);
    void init(); // Initialize LVGL and the display
    void addScreen(const std::string& name, Screen* screen); // Register a screen
    void setScreen(const std::string& name); // Set the active screen
    void update(); // Update the active screen and LVGL
    void handleButtons(); // Handle button state changes

private:
    TFT_eSPI& tft; // Reference to the TFT display
    std::map<std::string, Screen*> screens; // Map of registered screens
    Screen* currentScreen; // Pointer to the currently active screen

    // Structure to store the state and debounce time of each button
    struct Button {
        int pin; // Pin number of the button
        bool state; // Current state of the button
        bool pressed; // True if the button is pressed
        bool released; // True if the button has been released
        unsigned long lastDebounceTime; // Last time the button state was updated
    };

    // Array to store the state of all buttons
    Button buttons[3];

    void lvgl_init(); // Initialize LVGL
    static void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map); // LVGL display flush callback
};

#endif