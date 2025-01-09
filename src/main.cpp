#include <Arduino.h>
#include "config.h"
#include "screens/ScreenManager.h"
#include "screens/ButtonsTestScreen/ButtonsTestScreen.h"
#include "screens/FoodScreen/FoodScreen.h"
#include "screens/MainMenu/MainMenu.h"
#include "screens/SetupScreen/SetupScreen.h"
#include "screens/RestartScreen/RestartScreen.h"
#include "PersistentDataManager/PersistentDataManager.h"
#include "screens/TestScreen/TestScreen.h"
#include "screens/ShopScreen/ShopScreen.h"
#include "screens/PetStatsScreen/PetStatsScreen.h"

// Objetos globales
TFT_eSPI tft;
ScreenManager screenManager(tft);
ButtonsTestScreen buttonsTestScreen;
FoodScreen foodScreen;
MainMenu mainMenu;
SetupScreen setupScreen;
RestartScreen restartScreen;
PersistentDataManager persistentDataManager;
TestScreen testScreen;
ShopScreen shopScreen;
PetStatsScreen petStatsScreen;

// Intervalo de tiempo para 30 FPS (en milisegundos)
const uint32_t FRAME_INTERVAL = 1000 / 30;

void setup() {
    Serial.begin(115200);
    Serial.println("Inicializando el sistema...");

    // Inicializar LittleFS y crear archivos iniciales
    if (!persistentDataManager.init()) {
        Serial.println("Error al inicializar LittleFS. Intentando formatear...");
        if (!persistentDataManager.format()) {
            Serial.println("Error al formatear LittleFS.");
            return;
        }
        if (!persistentDataManager.init()) {
            Serial.println("Error al inicializar LittleFS después de formatear.");
            return;
        }
    }

    // Crear archivos iniciales si no existen
    if (!persistentDataManager.createInitialFiles()) {
        Serial.println("Error al crear archivos iniciales.");
        return;
    }

    // Configurar pines
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(BUTTON1_PIN, INPUT_PULLUP); // Usar resistencia pull-up interna
    pinMode(BUTTON2_PIN, INPUT_PULLUP); // Usar resistencia pull-up interna
    pinMode(BUTTON3_PIN, INPUT_PULLUP); // Usar resistencia pull-up interna

    // Registrar pantallas en ScreenManager
    screenManager.addScreen("ButtonsTestScreen", &buttonsTestScreen);
    screenManager.addScreen("FoodScreen", &foodScreen);
    screenManager.addScreen("MainMenu", &mainMenu);
    screenManager.addScreen("SetupScreen", &setupScreen);
    screenManager.addScreen("RestartScreen", &restartScreen);
    screenManager.addScreen("TestScreen", &testScreen);
    screenManager.addScreen("ShopScreen", &shopScreen);
    screenManager.addScreen("PetStatsScreen", &petStatsScreen);

    // Inicializar LVGL y cargar la pantalla principal
    screenManager.init();
    screenManager.setScreen("MainMenu"); // Cargar MainMenu al inicio

    Serial.println("Setup completado.");
}

void loop() {
    static uint32_t last_frame_time = millis(); // Tiempo del último fotograma
    uint32_t current_time = millis();
    uint32_t elapsed_time = current_time - last_frame_time; // Tiempo transcurrido desde el último fotograma

    // Actualizar cada fotograma (30 FPS)
    if (elapsed_time >= FRAME_INTERVAL) {
        last_frame_time = current_time; // Reiniciar el tiempo del último fotograma

        screenManager.handleButtons();

        // Actualizar LVGL con el tiempo transcurrido
        lv_tick_inc(elapsed_time);

        // Actualizar el ScreenManager (LVGL y la pantalla activa)
        screenManager.update();
    }
}