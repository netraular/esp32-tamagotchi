#include "PersistentDataManager.h"

PersistentDataManager::PersistentDataManager() {
    // Constructor
}

bool PersistentDataManager::init() {
    if (!LittleFS.begin()) {
        Serial.println("Error al inicializar LittleFS");
        return false;
    }
    Serial.println("LittleFS inicializado correctamente");

    // Verificar si el directorio /data existe
    if (!LittleFS.exists("/data")) {
        Serial.println("Creando directorio /data...");
        if (!LittleFS.mkdir("/data")) {
            Serial.println("Error al crear el directorio /data");
            return false;
        }
    }

    return true;
}

bool PersistentDataManager::fileExists(const char* filePath) {
    return LittleFS.exists(filePath);
}

bool PersistentDataManager::createFile(const char* filePath, const char* initialContent) {
    File file = LittleFS.open(filePath, "w");
    if (!file) {
        Serial.printf("Error al crear el archivo: %s\n", filePath);
        return false;
    }
    size_t bytesWritten = file.print(initialContent);
    file.close();
    if (bytesWritten == 0) {
        Serial.printf("Error al escribir en el archivo: %s\n", filePath);
        return false;
    }
    Serial.printf("Archivo creado correctamente: %s\n", filePath);
    return true;
}

bool PersistentDataManager::createInitialFiles() {
    if (!fileExists("/data/food.json")) {
        const char* initialFoodData = R"(
        [
            {"id":1,"name":"Apple Pie","hunger":100,"health":5,"happiness":2,"quantity":5,"price":5,"image":"/assets/food/n05ApplePie.c"},
            {"id":2,"name":"Bread","hunger":20,"health":10,"happiness":5,"quantity":5,"price":15,"image":"/assets/food/n07Bread.c"}
        ])";
        if (!createFile("/data/food.json", initialFoodData)) {
            return false;
        }
    }

    if (!fileExists("/data/pet_stats.json")) {
        const char* initialPetStats = R"(
        {
            "name": "TestName",
            "health": 100,
            "hunger": 100,
            "happiness": 100,
            "coins": 0,
            "birthdate": "2023-01-01",
            "evolution": 0,
            "alive": true
        })";
        if (!createFile("/data/pet_stats.json", initialPetStats)) {
            return false;
        }
    }

    return true;
}

bool PersistentDataManager::resetFoodData() {
    const char* initialFoodData = R"(
    [
        {"id":1,"name":"Apple Pie","hunger":100,"health":5,"happiness":2,"quantity":5,"price":5,"image":"/assets/food/n05ApplePie.c"},
        {"id":2,"name":"Bread","hunger":20,"health":10,"happiness":5,"quantity":5,"price":15,"image":"/assets/food/n07Bread.c"}
    ])";
    return createFile("/data/food.json", initialFoodData);
}

bool PersistentDataManager::resetPetStats() {
    const char* initialPetStats = R"(
    {
        "name": "TestName",
        "health": 100,
        "hunger": 100,
        "happiness": 100,
        "coins": 0,
        "birthdate": "2023-01-01",
        "evolution": 0,
        "alive": true
    })";
    return createFile("/data/pet_stats.json", initialPetStats);
}

JsonDocument PersistentDataManager::loadData(const char* filePath) {
    File file = LittleFS.open(filePath, "r");
    if (!file) {
        Serial.printf("Error al abrir el archivo para lectura: %s\n", filePath);
        return JsonDocument();
    }

    size_t size = file.size();
    if (size == 0) {
        Serial.printf("El archivo está vacío: %s\n", filePath);
        file.close();
        return JsonDocument();
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
        Serial.printf("Error al parsear el JSON en el archivo: %s\n", filePath);
        return JsonDocument();
    }

    Serial.printf("Archivo cargado correctamente: %s\n", filePath);
    return doc;
}

bool PersistentDataManager::saveData(const char* filePath, const JsonDocument& data) {
    File file = LittleFS.open(filePath, "w");
    if (!file) {
        Serial.printf("Error al abrir el archivo para escritura: %s\n", filePath);
        return false;
    }

    serializeJson(data, file);
    file.close();
    return true;
}