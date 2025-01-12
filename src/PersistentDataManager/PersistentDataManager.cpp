#include "PersistentDataManager.h"

/**
 * @brief Returns the singleton instance of PersistentDataManager.
 * 
 * This ensures only one instance of PersistentDataManager exists, providing centralized
 * control over persistent data operations.
 * 
 * @return PersistentDataManager& Singleton instance.
 */
PersistentDataManager& PersistentDataManager::getInstance() {
    static PersistentDataManager instance; // Singleton instance
    return instance;
}

/**
 * @brief Private constructor for PersistentDataManager.
 * 
 * Prevents external instantiation of the class, enforcing the Singleton pattern.
 */
PersistentDataManager::PersistentDataManager() {
    // Private constructor
}

/**
 * @brief Initializes LittleFS and prepares it for use.
 * 
 * This function mounts the LittleFS filesystem and creates the necessary directories.
 * 
 * @return true if initialization is successful, false otherwise.
 */
bool PersistentDataManager::init() {
    if (!LittleFS.begin(true, "/littlefs", 10, "littlefs")) {
        Serial.println("Failed to initialize LittleFS");
        return false;
    }
    Serial.println("LittleFS initialized successfully");

    // Check if the /data directory exists
    if (!createDirectory("/data")) {
        Serial.println("Failed to create /data directory");
        return false;
    }

    return true;
}

/**
 * @brief Formats the LittleFS filesystem.
 * 
 * This function erases all data in LittleFS and prepares it for reuse.
 * 
 * @return true if formatting is successful, false otherwise.
 */
bool PersistentDataManager::format() {
    Serial.println("Formatting LittleFS...");

    if (LittleFS.format()) {
        Serial.println("LittleFS formatted successfully.");
        return true;
    } else {
        Serial.println("Failed to format LittleFS.");
        return false;
    }
}

/**
 * @brief Creates initial JSON files if they don't already exist.
 * 
 * This function ensures that the required JSON files (settings.json, food.json,
 * own_food.json, pet_stats.json) are present in the /data directory.
 * 
 * @return true if all files are created successfully, false otherwise.
 */
bool PersistentDataManager::createInitialFiles() {
    // Create settings.json if it doesn't exist
    if (!LittleFS.exists("/data/settings.json")) {
        const char* initialSettings = R"({"default_time": "12:00:00"})";
        if (!createFile("/data/settings.json", initialSettings)) {
            Serial.println("Failed to create settings.json");
            return false;
        }
    }

    // Create food.json if it doesn't exist
    if (!LittleFS.exists("/data/food.json")) {
        if (!resetFoodData()) {
            return false;
        }
    }

    // Create own_food.json if it doesn't exist
    if (!LittleFS.exists("/data/own_food.json")) {
        if (!resetOwnFoodData()) {
            return false;
        }
    }

    // Create pet_stats.json if it doesn't exist
    if (!LittleFS.exists("/data/pet_stats.json")) {
        if (!resetPetStats()) {
            return false;
        }
    }

    return true;
}

/**
 * @brief Resets food.json to its initial state.
 * 
 * This function overwrites food.json with the default food data. It is used when
 * deleting a game to restore the initial state of food items.
 * 
 * @return true if the operation is successful, false otherwise.
 */
bool PersistentDataManager::resetFoodData() {
    const char* initialFoodData = getInitialFoodData();
    return createFile("/data/food.json", initialFoodData);
}

/**
 * @brief Resets own_food.json to its initial state.
 * 
 * This function overwrites own_food.json with the default owned food data. It is used
 * when deleting a game to restore the initial state of owned food items.
 * 
 * @return true if the operation is successful, false otherwise.
 */
bool PersistentDataManager::resetOwnFoodData() {
    const char* initialOwnFoodData = getInitialOwnFoodData();
    return createFile("/data/own_food.json", initialOwnFoodData);
}

/**
 * @brief Resets pet_stats.json to its initial state.
 * 
 * This function overwrites pet_stats.json with the default pet statistics. It is used
 * when deleting a game to restore the initial state of pet data.
 * 
 * @return true if the operation is successful, false otherwise.
 */
bool PersistentDataManager::resetPetStats() {
    const char* initialPetStats = getInitialPetStats();
    return createFile("/data/pet_stats.json", initialPetStats);
}

/**
 * @brief Loads JSON data from a file.
 * 
 * This function reads a JSON file from LittleFS and parses it into a JsonDocument.
 * It is used to load data from files into fast-access variables for runtime use.
 * 
 * @param filePath Path to the JSON file.
 * @return JsonDocument containing the parsed data. Returns an empty document on failure.
 */
JsonDocument PersistentDataManager::loadData(const char* filePath) {
    File file = LittleFS.open(filePath, "r");
    if (!file) {
        Serial.printf("Failed to open file for reading: %s\n", filePath);
        return JsonDocument();
    }

    size_t size = file.size();
    if (size == 0) {
        Serial.printf("File is empty: %s\n", filePath);
        file.close();
        return JsonDocument();
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
        Serial.printf("Failed to parse JSON in file: %s\n", filePath);
        return JsonDocument();
    }

    Serial.printf("File loaded successfully: %s\n", filePath);
    return doc;
}

/**
 * @brief Saves JSON data to a file.
 * 
 * This function writes a JsonDocument to a JSON file in LittleFS. It is used to persist
 * game progress by saving data to flash memory.
 * 
 * @param filePath Path to the JSON file.
 * @param data JsonDocument containing the data to save.
 * @return true if the operation is successful, false otherwise.
 */
bool PersistentDataManager::saveData(const char* filePath, const JsonDocument& data) {
    File file = LittleFS.open(filePath, "w");
    if (!file) {
        Serial.printf("Failed to open file for writing: %s\n", filePath);
        return false;
    }

    serializeJson(data, file);
    file.close();
    return true;
}

/**
 * @brief Saves the default time to settings.json.
 * 
 * This function updates the default time in settings.json.
 * 
 * @param time The default time to save (in "HH:MM:SS" format).
 * @return true if the operation is successful, false otherwise.
 */
bool PersistentDataManager::saveDefaultTime(const char* time) {
    // Load settings.json
    File file = LittleFS.open("/data/settings.json", "r");
    if (!file) {
        Serial.println("Failed to open settings.json for reading");
        return false;
    }

    // Parse the JSON
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
        Serial.println("Failed to parse settings.json");
        return false;
    }

    // Update the default time
    doc["default_time"] = time;

    // Save the updated JSON
    file = LittleFS.open("/data/settings.json", "w");
    if (!file) {
        Serial.println("Failed to open settings.json for writing");
        return false;
    }

    serializeJson(doc, file);
    file.close();
    return true;
}

/**
 * @brief Loads the default time from settings.json.
 * 
 * This function retrieves the default time stored in settings.json.
 * 
 * @return const char* The default time (in "HH:MM:SS" format). Returns nullptr on failure.
 */
const char* PersistentDataManager::loadDefaultTime() {
    // Load settings.json
    File file = LittleFS.open("/data/settings.json", "r");
    if (!file) {
        Serial.println("Failed to open settings.json for reading");
        return nullptr;
    }

    // Parse the JSON
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
        Serial.println("Failed to parse settings.json");
        return nullptr;
    }

    // Return the default time
    return doc["default_time"];
}

/**
 * @brief Creates a file with initial content.
 * 
 * This helper function creates a new file in LittleFS and writes the provided content to it.
 * It is used internally by other functions to create initial files.
 * 
 * @param filePath Path to the file.
 * @param initialContent Content to write to the file.
 * @return true if the operation is successful, false otherwise.
 */
bool PersistentDataManager::createFile(const char* filePath, const char* initialContent) {
    File file = LittleFS.open(filePath, "w");
    if (!file) {
        Serial.printf("Failed to create file: %s\n", filePath);
        return false;
    }
    size_t bytesWritten = file.print(initialContent);
    file.close();
    if (bytesWritten == 0) {
        Serial.printf("Failed to write to file: %s\n", filePath);
        return false;
    }
    Serial.printf("File created successfully: %s\n", filePath);
    return true;
}

/**
 * @brief Creates a directory in LittleFS.
 * 
 * This helper function creates a directory if it doesn't already exist. It is used
 * internally by other functions to ensure the necessary directory structure is in place.
 * 
 * @param path Path to the directory.
 * @return true if the operation is successful, false otherwise.
 */
bool PersistentDataManager::createDirectory(const char* path) {
    if (LittleFS.exists(path)) {
        return true;
    }
    if (LittleFS.mkdir(path)) {
        Serial.printf("Directory created successfully: %s\n", path);
        return true;
    } else {
        Serial.printf("Failed to create directory: %s\n", path);
        return false;
    }
}

/**
 * @brief Returns the initial data for food.json.
 * 
 * This function provides the default JSON data for food items, which is used to generate
 * the initial food.json file.
 * 
 * @return const char* Initial JSON data for food.json.
 */
const char* PersistentDataManager::getInitialFoodData() {
    static const char* initialFoodData = R"(
    [
        {"id":1,"name":"Apple Pie","hunger":100,"health":5,"happiness":2,"price":5,"image":"/assets/food/n05ApplePie.c"},
        {"id":2,"name":"Bread","hunger":20,"health":10,"happiness":5,"price":15,"image":"/assets/food/n07Bread.c"}
    ])";
    return initialFoodData;
}

/**
 * @brief Returns the initial data for own_food.json.
 * 
 * This function provides the default JSON data for owned food items, which is used to
 * generate the initial own_food.json file.
 * 
 * @return const char* Initial JSON data for own_food.json.
 */
const char* PersistentDataManager::getInitialOwnFoodData() {
    static const char* initialOwnFoodData = R"(
    [
        {"id":1,"quantity":5},
        {"id":2,"quantity":5}
    ])";
    return initialOwnFoodData;
}

/**
 * @brief Returns the initial data for pet_stats.json.
 * 
 * This function provides the default JSON data for pet statistics, which is used to
 * generate the initial pet_stats.json file.
 * 
 * @return const char* Initial JSON data for pet_stats.json.
 */
const char* PersistentDataManager::getInitialPetStats() {
    static const char* initialPetStats = R"(
    {
        "name": "TestName",
        "health": 100,
        "hunger": 100,
        "happiness": 100,
        "coins": 900,
        "birthdate": "2023-01-01",
        "evolution": 0,
        "alive": true
    })";
    return initialPetStats;
}