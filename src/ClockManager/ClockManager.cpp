#include "ClockManager.h"

/**
 * @brief Returns the singleton instance of ClockManager.
 * 
 * This ensures only one instance of ClockManager exists, providing centralized
 * control over clock-related operations.
 * 
 * @return ClockManager& Singleton instance.
 */
ClockManager& ClockManager::getInstance() {
    static ClockManager instance; // Singleton instance
    return instance;
}

/**
 * @brief Private constructor for ClockManager.
 * 
 * Prevents external instantiation of the class, enforcing the Singleton pattern.
 */
ClockManager::ClockManager() {
    timeInitialized = false;
    ntpAttempted = false;
    ntpStartTime = 0;
    clockStarted = false;
}

/**
 * @brief Initializes the system clock.
 * 
 * This function sets up the system clock using the default time from settings.json
 * or a fallback time if settings.json is unavailable. It ensures that the clock starts
 * quickly without waiting for NTP synchronization, providing a seamless experience
 * for the player. NTP synchronization is attempted later in the background.
 */
void ClockManager::begin() {
    if (clockStarted) {
        return; // If the clock is already started, do nothing
    }

    // Load the default time from settings.json
    const char* defaultTime = PersistentDataManager::getInstance().loadDefaultTime();
    if (defaultTime) {
        // Convert the default time string to a tm structure
        struct tm timeinfo;
        memset(&timeinfo, 0, sizeof(timeinfo)); // Initialize the tm structure
        if (strptime(defaultTime, "%H:%M:%S", &timeinfo) != nullptr) {
            // Set the date to January 1, 2020 (as a base date)
            timeinfo.tm_year = 120; // Year 2020 (tm_year is years since 1900)
            timeinfo.tm_mon = 0;    // January (0-11)
            timeinfo.tm_mday = 1;   // Day 1

            // Convert to time_t (seconds since January 1, 1970)
            time_t defaultTime_t = mktime(&timeinfo);

            // Set the system time
            struct timeval tv = { defaultTime_t, 0 };
            settimeofday(&tv, nullptr);

            timeInitialized = true;
            Serial.printf("Time loaded from settings.json: %s\n", defaultTime);
        } else {
            Serial.println("Failed to parse time from settings.json.");
        }
    } else {
        // Use a fallback time if settings.json is unavailable
        struct tm timeinfo = {0, 0, 12, 1, 0, 120}; // 12:00:00 1 Jan 2020
        time_t defaultTime_t = mktime(&timeinfo);
        struct timeval tv = { defaultTime_t, 0 };
        settimeofday(&tv, nullptr);
        timeInitialized = true;
        Serial.println("Default time set (12:00:00).");
    }

    // Configure NTP servers for future synchronization
    setupTime();
    clockStarted = true; // Mark the clock as started
}

/**
 * @brief Updates the system clock.
 * 
 * This function attempts to synchronize the clock with NTP servers if a WiFi connection
 * is available. NTP synchronization is performed only once after the device restarts,
 * as the RTC (Real-Time Clock) is used to maintain accurate time. Optionally, NTP
 * synchronization could be performed periodically (e.g., once a day) or during specific
 * tasks. Before attempting NTP synchronization, the function checks for a WiFi connection
 * to avoid blocking the program if no network is available.
 */
void ClockManager::update() {
    // Attempt NTP synchronization in the background (only once)
    if (!ntpAttempted && timeInitialized) {
        if (ntpStartTime == 0) {
            ntpStartTime = millis(); // Start the NTP synchronization timer
        }

        // Check for WiFi connection before attempting NTP
        if (WiFi.status() == WL_CONNECTED) {
            if (millis() - ntpStartTime >= 1000) { // Wait 1 second before attempting NTP
                ntpAttempted = true;
                attemptNtpSync();
            }
        } else {
            // Skip NTP if there is no WiFi connection
            ntpAttempted = true;
            Serial.println("No WiFi connection. Skipping NTP synchronization.");
        }
    }
}

/**
 * @brief Configures NTP servers and timezone.
 * 
 * This function sets up the NTP servers and configures the system timezone.
 */
void ClockManager::setupTime() {
    // Configure NTP servers
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1); // Set timezone (example: Central European Time)
    tzset();
}

/**
 * @brief Attempts to synchronize the clock with NTP servers.
 * 
 * This function tries to obtain the current time from NTP servers and updates the system clock.
 */
void ClockManager::attemptNtpSync() {
    struct tm ntpTimeinfo;
    if (getLocalTime(&ntpTimeinfo, 2000)) { // Wait up to 2 seconds for NTP response
        Serial.println("Time synchronized with NTP.");
        timeInitialized = true;
    } else {
        Serial.println("Failed to synchronize time with NTP.");
    }
}

/**
 * @brief Gets the current time as a formatted string.
 * 
 * This function returns the current time in "HH:MM:SS" format.
 * 
 * @return const char* Formatted time string.
 */
const char* ClockManager::getTimeString() {
    static char timeString[9]; // Buffer to store the time string
    struct tm timeinfo;
    time_t now;
    time(&now);
    localtime_r(&now, &timeinfo);
    strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeinfo);
    return timeString;
}

/**
 * @brief Gets the current time as a tm structure.
 * 
 * This function returns the current time in a structured format (tm).
 * 
 * @return struct tm Current time as a tm structure.
 */
struct tm ClockManager::getTime() {
    struct tm timeinfo;
    time_t now;
    time(&now);
    localtime_r(&now, &timeinfo);
    return timeinfo;
}