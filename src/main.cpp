#include <Arduino.h>
#include "config.h"
#include <esp_sntp.h>
#include "screens/ScreenManager.h"
#include "screens/ButtonsTestScreen/ButtonsTestScreen.h"
#include "screens/FoodScreen/FoodScreen.h"
#include "screens/MainMenu/MainMenu.h"
#include "screens/SetupScreen/SetupScreen.h"
#include "screens/RestartScreen/RestartScreen.h"
#include "PersistentDataManager/PersistentDataManager.h"
#include "screens/TestScreen/TestScreen.h"
#include "screens/ShopScreen/ShopScreen.h"
#include "screens/PetStatsScreen/PetStatsScreen.h"
#include "screens/PetScreen/PetScreen.h"
#include "screens/SettingsScreen/SettingsScreen.h"
#include "screens/SettingsScreen/NicknameScreen/NicknameScreen.h"
#include "screens/SettingsScreen/DateTimeScreen/DateTimeScreen.h"
#include "screens/SettingsScreen/SleepTimeScreen/SleepTimeScreen.h"
#include "screens/SettingsScreen/FavoriteAchievementScreen/FavoriteAchievementScreen.h"
#include "screens/SettingsScreen/FavoriteItemScreen/FavoriteItemScreen.h"
#include "screens/SettingsScreen/FavoriteFoodScreen/FavoriteFoodScreen.h"
#include "screens/SettingsScreen/WifiScreen/WifiScreen.h"
#include "screens/SettingsScreen/LanguageScreen/LanguageScreen.h"
#include "screens/LoadScreen/LoadScreen.h"
#include "ClockManager/ClockManager.h"
#include "screens/TestScreen2/TestScreen2.h"
#include "screens/RoomSelectionScreen/RoomSelectionScreen.h"
#include "screens/RoomSelectionScreen/BedroomScreen/BedroomScreen.h"
#include "screens/RoomSelectionScreen/BathroomScreen/BathroomScreen.h"
#include "screens/RoomSelectionScreen/KitchenScreen/KitchenScreen.h"
#include "screens/RoomSelectionScreen/LivingRoomScreen/LivingRoomScreen.h"
#include "screens/RoomSelectionScreen/GardenScreen/GardenScreen.h"
#include "screens/RoomSelectionScreen/OutsideScreen/OutsideScreen.h"
#include <LittleFS.h>
#include <lvgl.h>

// Global objects
TFT_eSPI tft;
ScreenManager screenManager(tft);
ButtonsTestScreen buttonsTestScreen;
FoodScreen foodScreen;
MainMenu mainMenu;
SetupScreen setupScreen;
RestartScreen restartScreen;
PersistentDataManager persistentDataManager;
TestScreen testScreen;
ShopScreen shopScreen;
PetStatsScreen petStatsScreen;
PetScreen petScreen;
SettingsScreen settingsScreen;
NicknameScreen nicknameScreen;
DateTimeScreen dateTimeScreen;
SleepTimeScreen sleepTimeScreen;
FavoriteAchievementScreen favoriteAchievementScreen;
FavoriteItemScreen favoriteItemScreen;
FavoriteFoodScreen favoriteFoodScreen;
WifiScreen wifiScreen;
LanguageScreen languageScreen;
LoadScreen loadScreen;
TestScreen2 testScreen2;
RoomSelectionScreen roomSelectionScreen;
BedroomScreen bedroomScreen;
BathroomScreen bathroomScreen;
KitchenScreen kitchenScreen;
LivingRoomScreen livingRoomScreen;
GardenScreen gardenScreen;
OutsideScreen outsideScreen;

// Frame interval for 30 FPS (in milliseconds)
const uint32_t FRAME_INTERVAL = 1000 / 30;

// LVGL file system callbacks
static void* lvgl_fs_open(lv_fs_drv_t* drv, const char* path, lv_fs_mode_t mode) {
    const char* mode_str = "";
    if (mode == LV_FS_MODE_RD) mode_str = "r";
    else if (mode == LV_FS_MODE_WR) mode_str = "w";
    else if (mode == (LV_FS_MODE_WR | LV_FS_MODE_RD)) mode_str = "w+";

    fs::File* file = new fs::File(LittleFS.open(path, mode_str));
    return (*file) ? (void*)file : nullptr;
}

static lv_fs_res_t lvgl_fs_close(lv_fs_drv_t* drv, void* file_p) {
    fs::File* file = (fs::File*)file_p;
    if (file) {
        file->close();
        delete file;
    }
    return LV_FS_RES_OK;
}

static lv_fs_res_t lvgl_fs_read(lv_fs_drv_t* drv, void* file_p, void* buf, uint32_t btr, uint32_t* br) {
    fs::File* file = (fs::File*)file_p;
    *br = file->readBytes((char*)buf, btr);
    return LV_FS_RES_OK;
}

static lv_fs_res_t lvgl_fs_seek(lv_fs_drv_t* drv, void* file_p, uint32_t pos, lv_fs_whence_t whence) {
    fs::File* file = (fs::File*)file_p;
    switch(whence) {
        case LV_FS_SEEK_SET: file->seek(pos); break;
        case LV_FS_SEEK_CUR: file->seek(file->position() + pos); break;
        case LV_FS_SEEK_END: file->seek(file->size() + pos); break;
        default: return LV_FS_RES_INV_PARAM;
    }
    return LV_FS_RES_OK;
}

static lv_fs_res_t lvgl_fs_tell(lv_fs_drv_t* drv, void* file_p, uint32_t* pos_p) {
    fs::File* file = (fs::File*)file_p;
    *pos_p = file->position();
    return LV_FS_RES_OK;
}

/**
 * @brief Initializes the system, including LittleFS, input pins, screens, clock, and LVGL.
 * 
 * This function sets up the following:
 * - Initializes LittleFS and creates initial files if they don't exist.
 * - Configures input pins for buttons and buzzer.
 * - Registers all screens with the ScreenManager.
 * - Initializes the system clock and synchronizes it with NTP if possible.
 * - Initializes LVGL and sets the initial screen to "LoadScreen".
 */
void setup() {
    Serial.begin(115200);
    Serial.println("Initializing system...");
    
    // Initialize LittleFS and create initial files
    if (!persistentDataManager.init()) {
        Serial.println("Failed to initialize LittleFS. Attempting to format...");
        if (!persistentDataManager.format()) {
            Serial.println("Failed to format LittleFS.");
            return;
        }
        if (!persistentDataManager.init()) {
            Serial.println("Failed to initialize LittleFS after formatting.");
            return;
        }
    }

    // Registrar el driver de archivos LVGL con LittleFS
    Serial.println("Registrar el driver de archivos LVGL con LittleFS.");
    lv_init(); // Asegurar que LVGL está inicializado
    static lv_fs_drv_t fs_drv;
    lv_fs_drv_init(&fs_drv);
    fs_drv.letter = 'L';
    fs_drv.cache_size = 1024;
    fs_drv.open_cb = lvgl_fs_open;
    fs_drv.close_cb = lvgl_fs_close;
    fs_drv.read_cb = lvgl_fs_read;
    fs_drv.seek_cb = lvgl_fs_seek;
    fs_drv.tell_cb = lvgl_fs_tell;
    lv_fs_drv_register(&fs_drv);
    Serial.println("EXITO al registrar el driver de archivos LVGL con LittleFS.");

    // Create initial files if they don't exist
    if (!persistentDataManager.createInitialFiles()) {
        Serial.println("Failed to create initial files.");
        return;
    }

    // Configure input pins
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(BUTTON1_PIN, INPUT_PULLUP); // Use internal pull-up resistor
    pinMode(BUTTON2_PIN, INPUT_PULLUP); // Use internal pull-up resistor
    pinMode(BUTTON3_PIN, INPUT_PULLUP); // Use internal pull-up resistor
    pinMode(BUTTON4_PIN, INPUT_PULLUP); // Use internal pull-up resistor

    // Register screens with ScreenManager
    screenManager.addScreen("LoadScreen", &loadScreen); // Register LoadScreen
    screenManager.addScreen("ButtonsTestScreen", &buttonsTestScreen);
    screenManager.addScreen("FoodScreen", &foodScreen);
    screenManager.addScreen("MainMenu", &mainMenu);
    screenManager.addScreen("SetupScreen", &setupScreen);
    screenManager.addScreen("RestartScreen", &restartScreen);
    screenManager.addScreen("TestScreen", &testScreen);
    screenManager.addScreen("ShopScreen", &shopScreen);
    screenManager.addScreen("PetStatsScreen", &petStatsScreen);
    screenManager.addScreen("PetScreen", &petScreen);
    screenManager.addScreen("SettingsScreen", &settingsScreen);
    screenManager.addScreen("NicknameScreen", &nicknameScreen);
    screenManager.addScreen("DateTimeScreen", &dateTimeScreen);
    screenManager.addScreen("SleepTimeScreen", &sleepTimeScreen);
    screenManager.addScreen("FavoriteAchievementScreen", &favoriteAchievementScreen);
    screenManager.addScreen("FavoriteItemScreen", &favoriteItemScreen);
    screenManager.addScreen("FavoriteFoodScreen", &favoriteFoodScreen);
    screenManager.addScreen("WifiScreen", &wifiScreen);
    screenManager.addScreen("LanguageScreen", &languageScreen);
    screenManager.addScreen("TestScreen2", &testScreen2);
    screenManager.addScreen("RoomSelectionScreen", &roomSelectionScreen);
    screenManager.addScreen("Bedroom", &bedroomScreen);
    screenManager.addScreen("Bathroom", &bathroomScreen);
    screenManager.addScreen("Kitchen", &kitchenScreen);
    screenManager.addScreen("LivingRoom", &livingRoomScreen);
    screenManager.addScreen("Garden", &gardenScreen);
    screenManager.addScreen("Outside", &outsideScreen);

    // Initialize the system clock
    ClockManager::getInstance().begin();

    // Initialize LVGL and load the initial screen
    screenManager.init();
    screenManager.setScreen("LoadScreen"); // Set initial screen to LoadScreen

    // Update the clock
    ClockManager::getInstance().update();

    Serial.println("Setup completed.");
}

/**
 * @brief Main loop that controls the flow of actions at 30 FPS using millis().
 * 
 * This function:
 * - Ensures the loop runs at a consistent frame rate (30 FPS).
 * - Handles button state changes and updates the screen accordingly.
 * - Updates LVGL's internal timer using `lv_tick_inc`.
 * - Calls the `update` function of the active screen to refresh its content.
 */
void loop() {
    static uint32_t last_frame_time = millis(); // Time of the last frame
    uint32_t current_time = millis();
    uint32_t elapsed_time = current_time - last_frame_time; // Time elapsed since the last frame

    // Update every frame (30 FPS)
    if (elapsed_time >= FRAME_INTERVAL) {
        last_frame_time = current_time; // Reset the last frame time

        // Handle button state changes
        screenManager.handleButtons();

        // Update LVGL's internal timer with the elapsed time
        lv_tick_inc(elapsed_time);

        // Update the ScreenManager (LVGL and the active screen)
        screenManager.update();
    }
}