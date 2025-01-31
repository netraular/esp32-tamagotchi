#include "ScreenManager.h"
#include "config.h"

/**
 * @brief Constructor for ScreenManager.
 * 
 * Initializes the ScreenManager with a reference to the TFT display and sets up the buttons.
 * 
 * @param tft Reference to the TFT_eSPI display object.
 */
ScreenManager::ScreenManager(TFT_eSPI& tft) : tft(tft), currentScreen(nullptr) {
    // Initialize the buttons
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

    buttons[3].pin = BUTTON4_PIN;
    buttons[3].state = HIGH;
    buttons[3].pressed = false;
    buttons[3].released = true;
    buttons[3].lastDebounceTime = 0;
}

/**
 * @brief Initializes LVGL and the display.
 * 
 * This function sets up LVGL, configures the display buffers, and prepares the screen for rendering.
 */
void ScreenManager::init() {
    lvgl_init();
}

/**
 * @brief Registers a screen with the ScreenManager.
 * 
 * This function adds a screen to the map of registered screens, allowing it to be activated later.
 * 
 * @param name Name of the screen (used as a key in the map).
 * @param screen Pointer to the Screen object.
 */
void ScreenManager::addScreen(const std::string& name, Screen* screen) {
    screens[name] = screen;
}

/**
 * @brief Sets the active screen.
 * 
 * This function changes the currently active screen and calls its `load` method to initialize it.
 * 
 * @param name Name of the screen to activate.
 */
void ScreenManager::setScreen(const std::string& name) {
    if (screens.find(name) != screens.end()) {
        if (currentScreen != nullptr) {
            // Clear the current screen if necessary
            currentScreen->unload();  // Liberar recursos de la pantalla actual
            lv_obj_clean(lv_scr_act());
        }
        currentScreen = screens[name];
        currentScreen->load();
        Serial.print("Screen loaded: ");
        Serial.println(name.c_str());
    } else {
        Serial.println("Error: Screen not found.");
    }
}

/**
 * @brief Updates the active screen and LVGL.
 * 
 * This function calls the `update` method of the active screen and handles LVGL's internal tasks.
 */
void ScreenManager::update() {
    if (currentScreen != nullptr) {
        currentScreen->update();
    }
    lv_timer_handler();
}

/**
 * @brief Handles button state changes.
 * 
 * This function reads the current state of the buttons, detects changes, and passes them
 * to the active screen for handling.
 */
void ScreenManager::handleButtons() {
    static ButtonState previousState = {false, false, false}; // Previous state of the buttons
    ButtonState currentState = {
        digitalRead(BUTTON1_PIN) == LOW,
        digitalRead(BUTTON2_PIN) == LOW,
        digitalRead(BUTTON3_PIN) == LOW,
        digitalRead(BUTTON4_PIN) == LOW
    };

    // Detect changes in the button states
    // Siempre notificar al screen actual, incluso si no hay cambios
    if (currentScreen != nullptr) {
        ButtonChange change = {
            currentState.button1Pressed != previousState.button1Pressed,
            currentState.button2Pressed != previousState.button2Pressed,
            currentState.button3Pressed != previousState.button3Pressed,
            currentState.button4Pressed != previousState.button4Pressed
        };
        
        // Notificar en cada frame si algún botón está pulsado
        currentScreen->handleButtonEvent(currentState, change);
    }

    // Update the previous state
    previousState = currentState;
}

/**
 * @brief Initializes LVGL and configures the display.
 * 
 * This function sets up LVGL, which is a lightweight graphics library for embedded systems.
 * It performs the following tasks:
 * 1. Initializes the TFT display using the TFT_eSPI library.
 * 2. Initializes LVGL and sets up the necessary buffers for rendering.
 * 3. Configures the display driver with the appropriate settings.
 * 4. Registers a flush callback function (`my_disp_flush`) to handle rendering.
 * 
 * The display buffer is configured to store 10 lines of the screen, which allows for partial
 * rendering and reduces memory usage. LVGL uses this buffer to render small portions of the
 * screen at a time, improving performance on resource-constrained devices like the ESP32.
 */
void ScreenManager::lvgl_init() {
    tft.begin(); // Initialize the TFT display
    tft.fillScreen(TFT_BLACK); // Clear the screen with a black background
    lv_init(); // Initialize LVGL

    // Create a drawing buffer for LVGL
    static lv_color_t buf[TFT_WIDTH * 10];  // Buffer for 10 lines of the display
    lv_draw_buf_t draw_buf;
    lv_draw_buf_init(&draw_buf, TFT_WIDTH, 10, LV_COLOR_FORMAT_NATIVE, TFT_WIDTH * sizeof(lv_color_t), buf, sizeof(buf));

    // Create an LVGL display object
    lv_display_t *disp = lv_display_create(TFT_WIDTH, TFT_HEIGHT);

    // Configure the display buffers
    lv_display_set_buffers(disp, buf, NULL, sizeof(buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

    // Register the flush callback function
    lv_display_set_flush_cb(disp, my_disp_flush);

    // Store a reference to the ScreenManager instance in the display driver data
    lv_display_set_driver_data(disp, this);
}

/**
 * @brief LVGL display flush callback function.
 * 
 * This function is called by LVGL whenever it needs to render a portion of the screen.
 * It performs the following tasks:
 * 1. Receives the area of the screen to be updated and the pixel data from LVGL.
 * 2. Uses the TFT_eSPI library to write the pixel data to the TFT display.
 * 3. Notifies LVGL that the flush operation is complete.
 * 
 * The function is critical for integrating LVGL with the TFT display. It ensures that
 * the graphics rendered by LVGL are correctly displayed on the screen. The `area` parameter
 * defines the region of the screen to update, and `px_map` contains the pixel data for that region.
 * 
 * @param disp LVGL display object.
 * @param area Area of the screen to update (defined by coordinates x1, y1, x2, y2).
 * @param px_map Pointer to the pixel data for the specified area.
 */
void ScreenManager::my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
    // Calculate the width and height of the area to update
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    // Get the ScreenManager instance from the display driver data
    ScreenManager* manager = (ScreenManager*)lv_display_get_driver_data(disp);

    // Start writing to the TFT display
    manager->tft.startWrite();

    // Set the address window to the specified area
    manager->tft.setAddrWindow(area->x1, area->y1, w, h);

    // Push the pixel data to the TFT display
    manager->tft.pushColors((uint16_t *)px_map, w * h, true);

    // End the write operation
    manager->tft.endWrite();

    // Notify LVGL that the flush operation is complete
    lv_display_flush_ready(disp);
}

Screen* ScreenManager::getScreen(const std::string& name) {
    auto it = screens.find(name);
    if (it != screens.end()) {
        return it->second; // Devuelve el puntero a la pantalla si se encuentra
    }
    return nullptr; // Devuelve nullptr si la pantalla no existe
}