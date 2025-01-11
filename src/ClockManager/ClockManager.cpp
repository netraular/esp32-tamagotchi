#include "ClockManager.h"
#include "../PersistentDataManager/PersistentDataManager.h"
#include <sys/time.h>
#include <WiFi.h>

ClockManager& ClockManager::getInstance() {
    static ClockManager instance; // Instancia única (Singleton)
    return instance;
}

ClockManager::ClockManager() {
    timeInitialized = false;
    ntpAttempted = false;
    ntpStartTime = 0;
    clockStarted = false;
}

void ClockManager::begin() {
    if (clockStarted) {
        return; // Si el reloj ya se ha iniciado, no hacer nada
    }

    // Cargar la hora por defecto desde settings.json (si es necesario)
    const char* defaultTime = PersistentDataManager::getInstance().loadDefaultTime();
    if (defaultTime) {
        // Convertir la cadena de texto a una estructura tm
        struct tm timeinfo;
        memset(&timeinfo, 0, sizeof(timeinfo)); // Inicializar la estructura tm
        if (strptime(defaultTime, "%H:%M:%S", &timeinfo) != nullptr) {
            // Configurar la fecha actual (1 de enero de 2020 como fecha base)
            timeinfo.tm_year = 120; // Año 2020 (tm_year es años desde 1900)
            timeinfo.tm_mon = 0;    // Enero (0-11)
            timeinfo.tm_mday = 1;   // Día 1

            // Convertir a time_t (segundos desde el 1 de enero de 1970)
            time_t defaultTime_t = mktime(&timeinfo);

            // Configurar la hora del sistema
            struct timeval tv = { defaultTime_t, 0 };
            settimeofday(&tv, nullptr);

            timeInitialized = true;
            Serial.printf("Hora cargada desde settings.json: %s\n", defaultTime);
        } else {
            Serial.println("Error al parsear la hora desde settings.json.");
        }
    } else {
        // Usar la hora por defecto si no se puede cargar
        struct tm timeinfo = {0, 0, 12, 1, 0, 120}; // 12:00:00 1 Jan 2020
        time_t defaultTime_t = mktime(&timeinfo);
        struct timeval tv = { defaultTime_t, 0 };
        settimeofday(&tv, nullptr);
        timeInitialized = true;
        Serial.println("Hora por defecto configurada (12:00:00).");
    }

    // Configurar el servidor NTP
    setupTime();
    clockStarted = true; // Marcar que el reloj se ha iniciado
}

void ClockManager::update() {
    // No es necesario actualizar manualmente la hora si usamos gettimeofday()
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
        timeInitialized = true;
    } else {
        Serial.println("No se pudo obtener la hora por NTP.");
    }
}

const char* ClockManager::getTimeString() {
    static char timeString[9]; // Buffer para almacenar la cadena de texto
    struct tm timeinfo;
    time_t now;
    time(&now);
    localtime_r(&now, &timeinfo);
    strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeinfo);
    return timeString;
}

struct tm ClockManager::getTime() {
    struct tm timeinfo;
    time_t now;
    time(&now);
    localtime_r(&now, &timeinfo);
    return timeinfo;
}