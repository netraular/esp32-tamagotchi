#ifndef PERSISTENTDATAMANAGER_H
#define PERSISTENTDATAMANAGER_H

#include <ArduinoJson.h>
#include <LittleFS.h>

class PersistentDataManager {
public:
    PersistentDataManager();
    bool init(); // Inicializar LittleFS
    bool createInitialFiles(); // Crear archivos iniciales si no existen
    bool resetFoodData(); // Borrar y reescribir el archivo food.json
    bool resetPetStats(); // Borrar y reescribir el archivo pet_stats.json
    JsonDocument loadData(const char* filePath); // Cargar datos desde un archivo JSON
    bool saveData(const char* filePath, const JsonDocument& data); // Guardar datos en un archivo JSON

private:
    bool fileExists(const char* filePath); // Verificar si un archivo existe
    bool createFile(const char* filePath, const char* initialContent); // Crear un archivo con contenido inicial
};

#endif