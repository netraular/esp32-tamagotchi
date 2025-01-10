#include "LoadScreen.h"
#include "../ScreenManager.h"
#include "../../PersistentDataManager/PersistentDataManager.h"
#include <time.h>
#include <esp_sntp.h>
#include <WiFi.h> // Para verificar la conexión WiFi

extern ScreenManager screenManager;
extern PersistentDataManager persistentDataManager;

// Variable para almacenar la hora actual
static struct tm timeinfo;
static bool timeInitialized = false; // Indica si la hora se ha inicializado correctamente
static bool ntpAttempted = false;    // Indica si ya se intentó obtener la hora por NTP
static unsigned long ntpStartTime = 0; // Tiempo en que se inició el intento de NTP

void LoadScreen::load() {
    // Crear una etiqueta para mostrar el mensaje de carga
    label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Cargando...\nPulsa el botón 2 para PetScreen\nPulsa el botón 3 para MainMenu\nPulsa el botón 1 para guardar la hora actual");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -20); // Centrar el texto en la pantalla

    // Crear una etiqueta para mostrar la hora
    clockLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(clockLabel, "00:00:00");
    lv_obj_set_style_text_font(clockLabel, &lv_font_montserrat_24, 0);
    lv_obj_align(clockLabel, LV_ALIGN_CENTER, 0, 20); // Centrar el reloj en la pantalla

    // Cargar la hora desde settings.json (o usar la hora por defecto)
    loadDefaultTime();

    // Configurar el servidor NTP
    setupTime();

    Serial.println("LoadScreen cargada.");
}

void LoadScreen::update() {
    // Actualizar el reloj cada segundo
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate >= 1000) {
        lastUpdate = millis();

        // Si la hora está inicializada, actualizarla
        if (timeInitialized) {
            updateClock();
        }
    }

    // Intentar obtener la hora por NTP en segundo plano (solo una vez)
    if (!ntpAttempted && timeInitialized) {
        if (ntpStartTime == 0) {
            ntpStartTime = millis(); // Iniciar el temporizador para NTP
        }

        // Verificar si hay conexión WiFi antes de intentar NTP
        if (WiFi.status() == WL_CONNECTED) {
            if (millis() - ntpStartTime >= 1000) { // Esperar 1 segundo antes de intentar NTP
                ntpAttempted = true;
                attemptNtpSync();
            }
        } else {
            // Si no hay conexión WiFi, omitir el intento de NTP
            ntpAttempted = true;
            Serial.println("No hay conexión WiFi. Omitiendo sincronización NTP.");
        }
    }
}

void LoadScreen::handleButtonEvent(const ButtonState& state, const ButtonChange& change) {
    // Si se presiona el botón 1, guardar la hora actual en settings.json
    if (change.button1Changed && state.button1Pressed) {
        saveCurrentTime();
    }

    // Si se presiona el botón 2, ir a PetScreen
    if (change.button2Changed && state.button2Pressed) {
        screenManager.setScreen("PetScreen");
    }

    // Si se presiona el botón 3, ir a MainMenu
    if (change.button3Changed && state.button3Pressed) {
        screenManager.setScreen("MainMenu");
    }
}

void LoadScreen::loadDefaultTime() {
    // Cargar la hora por defecto desde settings.json
    const char* defaultTime = persistentDataManager.loadDefaultTime();
    if (defaultTime) {
        // Convertir la cadena de texto a una estructura tm
        strptime(defaultTime, "%H:%M:%S", &timeinfo);
        timeInitialized = true;

        // Mostrar la hora cargada
        char timeString[9];
        strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeinfo);
        lv_label_set_text(clockLabel, timeString);
    } else {
        // Usar la hora por defecto si no se puede cargar
        lv_label_set_text(clockLabel, "12:00:00");
    }
}

void LoadScreen::setupTime() {
    // Configurar el servidor NTP
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1); // Configurar la zona horaria
    tzset();
}

void LoadScreen::attemptNtpSync() {
    // Intentar obtener la hora por NTP en segundo plano
    struct tm ntpTimeinfo;
    if (getLocalTime(&ntpTimeinfo, 2000)) { // Esperar 2 segundos para obtener la hora
        Serial.println("Hora obtenida por NTP.");

        // Actualizar la hora actual
        timeinfo = ntpTimeinfo;
        timeInitialized = true;

        // Actualizar la pantalla con la nueva hora
        char timeString[9];
        strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeinfo);
        lv_label_set_text(clockLabel, timeString);
    } else {
        Serial.println("No se pudo obtener la hora por NTP.");
    }
}

void LoadScreen::updateClock() {
    // Incrementar la hora manualmente
    timeinfo.tm_sec++;
    if (timeinfo.tm_sec >= 60) {
        timeinfo.tm_sec = 0;
        timeinfo.tm_min++;
        if (timeinfo.tm_min >= 60) {
            timeinfo.tm_min = 0;
            timeinfo.tm_hour++;
            if (timeinfo.tm_hour >= 24) {
                timeinfo.tm_hour = 0;
            }
        }
    }

    // Formatear la hora como una cadena de texto
    char timeString[9];
    strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeinfo);

    // Actualizar la etiqueta del reloj
    lv_label_set_text(clockLabel, timeString);
}

void LoadScreen::saveCurrentTime() {
    if (!timeInitialized) {
        Serial.println("No se puede guardar la hora: la hora no está inicializada.");
        return;
    }

    // Formatear la hora como una cadena de texto
    char timeString[9];
    strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeinfo);

    // Guardar la hora en settings.json
    if (persistentDataManager.saveDefaultTime(timeString)) {
        Serial.println("Hora guardada correctamente en settings.json.");
    } else {
        Serial.println("Error al guardar la hora en settings.json.");
    }
}