#ifndef CLOCKMANAGER_H
#define CLOCKMANAGER_H

#include <time.h>
#include <WiFi.h>

class ClockManager {
public:
    static ClockManager& getInstance(); // Método Singleton

    void begin(); // Inicializar el reloj
    void update(); // Actualizar la hora manualmente
    const char* getTimeString(); // Obtener la hora como una cadena de texto (HH:MM:SS)
    struct tm getTime(); // Obtener la hora como una estructura tm

private:
    ClockManager(); // Constructor privado (Singleton)
    void setupTime(); // Configurar el servidor NTP
    void attemptNtpSync(); // Intentar sincronizar la hora por NTP

    struct tm timeinfo; // Estructura para almacenar la hora actual
    bool timeInitialized; // Indica si la hora se ha inicializado correctamente
    bool ntpAttempted; // Indica si ya se intentó obtener la hora por NTP
    unsigned long ntpStartTime; // Tiempo en que se inició el intento de NTP
};

#endif