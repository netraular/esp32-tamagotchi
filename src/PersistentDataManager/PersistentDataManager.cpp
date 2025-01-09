#include "PersistentDataManager.h"

PersistentDataManager::PersistentDataManager() {
    // Constructor
}

bool PersistentDataManager::init() {
    if (!LittleFS.begin(true, "/littlefs", 10, "littlefs")) {
        Serial.println("Error al inicializar LittleFS");
        return false;
    }
    Serial.println("LittleFS inicializado correctamente");

    // Verificar si el directorio /data existe
    if (!createDirectory("/data")) {
        Serial.println("Error al crear el directorio /data");
        return false;
    }

    return true;
}

bool PersistentDataManager::format() {
    Serial.println("Formateando LittleFS...");

    if (LittleFS.format()) {
        Serial.println("LittleFS formateado correctamente.");
        return true;
    } else {
        Serial.println("Error al formatear LittleFS.");
        return false;
    }
}

bool PersistentDataManager::createInitialFiles() {
    // Crear food.json si no existe
    if (!LittleFS.exists("/data/food.json")) {
        if (!resetFoodData()) {
            return false;
        }
    }

    // Crear own_food.json si no existe
    if (!LittleFS.exists("/data/own_food.json")) {
        if (!resetOwnFoodData()) {
            return false;
        }
    }

    // Crear pet_stats.json si no existe
    if (!LittleFS.exists("/data/pet_stats.json")) {
        if (!resetPetStats()) {
            return false;
        }
    }

    return true;
}

bool PersistentDataManager::resetFoodData() {
    const char* initialFoodData = getInitialFoodData();
    return createFile("/data/food.json", initialFoodData);
}

bool PersistentDataManager::resetOwnFoodData() {
    const char* initialOwnFoodData = getInitialOwnFoodData();
    return createFile("/data/own_food.json", initialOwnFoodData);
}

bool PersistentDataManager::resetPetStats() {
    const char* initialPetStats = getInitialPetStats();
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

bool PersistentDataManager::createDirectory(const char* path) {
    if (LittleFS.exists(path)) {
        return true;
    }
    if (LittleFS.mkdir(path)) {
        Serial.printf("Directorio creado correctamente: %s\n", path);
        return true;
    } else {
        Serial.printf("Error al crear el directorio: %s\n", path);
        return false;
    }
}

// Función privada para obtener el JSON inicial de food.json
const char* PersistentDataManager::getInitialFoodData() {
    static const char* initialFoodData = R"(
    [
        {"id":1,"name":"Apple Pie","hunger":100,"health":5,"happiness":2,"price":5,"image":"/assets/food/n05ApplePie.c"},
        {"id":2,"name":"Bread","hunger":20,"health":10,"happiness":5,"price":15,"image":"/assets/food/n07Bread.c"}
    ])";
    return initialFoodData;
}

// Función privada para obtener el JSON inicial de own_food.json
const char* PersistentDataManager::getInitialOwnFoodData() {
    static const char* initialOwnFoodData = R"(
    [
        {"id":1,"quantity":5},
        {"id":2,"quantity":5}
    ])";
    return initialOwnFoodData;
}

// Función privada para obtener el JSON inicial de pet_stats.json
const char* PersistentDataManager::getInitialPetStats() {
    static const char* initialPetStats = R"(
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
    return initialPetStats;
}