#include "ClockManager.h"
#include "../PersistentDataManager/PersistentDataManager.h" // Incluir el archivo de cabecera

ClockManager& ClockManager::getInstance() {
    static ClockManager instance; // Instancia única (Singleton)
    return instance;
}

ClockManager::ClockManager() {
    timeInitialized = false;
    ntpAttempted = false;
    ntpStartTime = 0;
}

void ClockManager::begin() {
    // Cargar la hora por defecto desde settings.json (si es necesario)
    const char* defaultTime = PersistentDataManager::getInstance().loadDefaultTime();
    if (defaultTime) {
        // Convertir la cadena de texto a una estructura tm
        strptime(defaultTime, "%H:%M:%S", &timeinfo);
        timeInitialized = true;
        Serial.println("Hora cargada desde settings.json.");
    } else {
        // Usar la hora por defecto si no se puede cargar
        timeinfo.tm_hour = 12;
        timeinfo.tm_min = 0;
        timeinfo.tm_sec = 0;
        timeInitialized = true;
        Serial.println("Hora por defecto configurada (12:00:00).");
    }

    // Configurar el servidor NTP
    setupTime();
}

void ClockManager::update() {
    // Actualizar la hora manualmente cada segundo
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate >= 1000) {
        lastUpdate = millis();

        if (timeInitialized) {
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
}

void ClockManager::setupTime() {
    // Configurar el servidor NTP
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1); // Configurar la zona horaria
    tzset();
}

void ClockManager::attemptNtpSync() {
    // Intentar obtener la hora por NTP en segundo plano
    struct tm ntpTimeinfo;
    if (getLocalTime(&ntpTimeinfo, 2000)) { // Esperar 2 segundos para obtener la hora
        Serial.println("Hora obtenida por NTP.");

        // Actualizar la hora actual
        timeinfo = ntpTimeinfo;
        timeInitialized = true;
    } else {
        Serial.println("No se pudo obtener la hora por NTP.");
    }
}

const char* ClockManager::getTimeString() {
    static char timeString[9]; // Buffer para almacenar la cadena de texto
    strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeinfo);
    return timeString;
}

struct tm ClockManager::getTime() {
    return timeinfo;
}