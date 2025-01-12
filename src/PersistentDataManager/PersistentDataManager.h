#ifndef PERSISTENTDATAMANAGER_H
#define PERSISTENTDATAMANAGER_H

#include <ArduinoJson.h>
#include <LittleFS.h>

/**
 * @class PersistentDataManager
 * @brief Manages persistent data storage using LittleFS on the ESP32.
 * 
 * This class provides functionality to initialize, format, and manage files in LittleFS.
 * It handles the creation, loading, and saving of JSON files for game data, such as
 * settings, food inventory, and pet statistics. It follows the Singleton pattern to ensure
 * a single instance manages all persistent data operations.
 */
class PersistentDataManager {
public:
    static PersistentDataManager& getInstance(); // Singleton method
    PersistentDataManager();
    bool init(); // Initialize LittleFS
    bool format(); // Format LittleFS
    bool createInitialFiles(); // Create initial JSON files if they don't exist
    bool resetFoodData(); // Reset food.json to its initial state (used when deleting a game)
    bool resetOwnFoodData(); // Reset own_food.json to its initial state (used when deleting a game)
    bool resetPetStats(); // Reset pet_stats.json to its initial state (used when deleting a game)
    JsonDocument loadData(const char* filePath); // Load JSON data from a file into fast-access variables
    bool saveData(const char* filePath, const JsonDocument& data); // Save JSON data to a file to persist progress in flash memory
    bool saveDefaultTime(const char* time); // Save the default time to settings.json
    const char* loadDefaultTime(); // Load the default time from settings.json

private:
    bool createFile(const char* filePath, const char* initialContent); // Helper function to create a file with initial content
    bool createDirectory(const char* path); // Helper function to create a directory

    // Functions to get initial JSON data for default files
    const char* getInitialFoodData(); // Returns default data for food.json
    const char* getInitialOwnFoodData(); // Returns default data for own_food.json
    const char* getInitialPetStats(); // Returns default data for pet_stats.json
};

#endif