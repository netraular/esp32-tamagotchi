#ifndef SCREEN_H
#define SCREEN_H

#include <lvgl.h>

/**
 * @struct ButtonState
 * @brief Represents the current state of the buttons.
 * 
 * This struct stores whether each button is currently pressed.
 */
struct ButtonState {
    bool button1Pressed; // State of button 1 (true if pressed)
    bool button2Pressed; // State of button 2 (true if pressed)
    bool button3Pressed; // State of button 3 (true if pressed)
    bool button4Pressed; // State of button 4 (true if pressed)
};

/**
 * @struct ButtonChange
 * @brief Represents changes in the state of the buttons.
 * 
 * This struct indicates whether the state of each button has changed since the last update.
 */
struct ButtonChange {
    bool button1Changed; // True if button 1's state has changed
    bool button2Changed; // True if button 2's state has changed
    bool button3Changed; // True if button 3's state has changed
    bool button4Changed; // True if button 4's state has changed
};

/**
 * @struct ButtonChangeBuffer
 * @brief Combines the current state and changes of the buttons.
 * 
 * This struct is used to pass both the current state and changes of the buttons to screens.
 */
struct ButtonChangeBuffer {
    ButtonState state;  // Current state of the buttons
    ButtonChange change; // Changes in the button states
};

/**
 * @class Screen
 * @brief Base class for all screens in the application.
 * 
 * This class defines the interface for screens, including methods for loading, updating,
 * and handling button events. All screens in the application must inherit from this class
 * and implement its virtual methods.
 */
class Screen {
public:
    virtual ~Screen() {}
    virtual void load() = 0; // Load the screen (initialize UI elements)
    virtual void update() = 0; // Update the screen (refresh UI elements)
    virtual void handleButtonEvent(const ButtonState& state, const ButtonChange& change) = 0; // Handle button events
    virtual void unload() {} // Método para liberar recursos
};

#endif