#ifndef PERSISTENTDATAMANAGER_H
#define PERSISTENTDATAMANAGER_H

#include <ArduinoJson.h>
#include <LittleFS.h>

class PersistentDataManager {
public:
    PersistentDataManager();
    bool init(); // Inicializar LittleFS
    bool format(); // Formatear LittleFS
    bool createInitialFiles(); // Crear archivos iniciales si no existen
    bool resetFoodData(); // Borrar y reescribir el archivo food.json
    bool resetOwnFoodData(); // Borrar y reescribir el archivo own_food.json
    bool resetPetStats(); // Borrar y reescribir el archivo pet_stats.json
    JsonDocument loadData(const char* filePath); // Cargar datos desde un archivo JSON
    bool saveData(const char* filePath, const JsonDocument& data); // Guardar datos en un archivo JSON
    bool saveDefaultTime(const char* time); // Guardar la hora por defecto en settings.json
    const char* loadDefaultTime(); // Cargar la hora por defecto desde settings.json

private:
    bool createFile(const char* filePath, const char* initialContent); // Crear un archivo con contenido inicial
    bool createDirectory(const char* path); // Crear un directorio

    // Funciones privadas para obtener el JSON inicial
    const char* getInitialFoodData(); // Obtener el JSON inicial de food.json
    const char* getInitialOwnFoodData(); // Obtener el JSON inicial de own_food.json
    const char* getInitialPetStats(); // Obtener el JSON inicial de pet_stats.json
};

#endif