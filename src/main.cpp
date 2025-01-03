#include <Arduino.h>
#include <LittleFS.h>
#include "config.h"
#include "screens/ScreenManager.h"
#include "screens/ButtonsTestScreen/ButtonsTestScreen.h"
#include "screens/FoodScreen/FoodScreen.h"
#include "screens/MainMenu/MainMenu.h"
#include "screens/SetupScreen/SetupScreen.h"
#include "screens/RestartScreen/RestartScreen.h"
#include "PersistentDataManager/PersistentDataManager.h" // Incluir el archivo de la clase

// Objetos globales
TFT_eSPI tft;
ScreenManager screenManager(tft);
ButtonsTestScreen buttonsTestScreen;
FoodScreen foodScreen;
MainMenu mainMenu;
SetupScreen setupScreen;
RestartScreen restartScreen;

// Definir la instancia de PersistentDataManager
PersistentDataManager persistentDataManager;

// Intervalo de tiempo para 30 FPS (en milisegundos)
const uint32_t FRAME_INTERVAL = 1000 / 30;

// Función para formatear LittleFS
void formatLittleFS() {
    Serial.println("Formateando LittleFS...");

    if (LittleFS.format()) {
        Serial.println("LittleFS formateado correctamente.");
    } else {
        Serial.println("Error al formatear LittleFS.");
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("Inicializando el sistema...");

    // Inicializar LittleFS
    if (!LittleFS.begin()) {
        Serial.println("Error al inicializar LittleFS. Intentando formatear...");
        formatLittleFS();

        // Intentar inicializar nuevamente después de formatear
        if (!LittleFS.begin()) {
            Serial.println("Error al inicializar LittleFS después de formatear.");
            return;
        }
    }
    Serial.println("LittleFS inicializado correctamente");

    // Verificar si el directorio /data existe
    if (!LittleFS.exists("/data")) {
        Serial.println("Creando directorio /data...");
        if (!LittleFS.mkdir("/data")) {
            Serial.println("Error al crear el directorio /data");
            return;
        }
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