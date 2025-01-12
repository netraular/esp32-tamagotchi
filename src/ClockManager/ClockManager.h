#ifndef CLOCKMANAGER_H
#define CLOCKMANAGER_H

#include <time.h>
#include <WiFi.h>

/**
 * @class ClockManager
 * @brief Manages the system clock and provides time-related functionality.
 * 
 * This class handles the initialization, synchronization, and retrieval of the system clock.
 * It supports synchronization with NTP servers and provides methods to get the current time
 * in both string and structured formats. It follows the Singleton pattern to ensure a single
 * instance manages all clock-related operations.
 */
class ClockManager {
public:
    static ClockManager& getInstance(); // Singleton method

    void begin(); // Initialize the system clock
    void update(); // Update the clock (attempt NTP synchronization)
    const char* getTimeString(); // Get the current time as a string (HH:MM:SS)
    struct tm getTime(); // Get the current time as a tm structure

private:
    ClockManager(); // Private constructor (Singleton)
    void setupTime(); // Configure NTP servers and timezone
    void attemptNtpSync(); // Attempt to synchronize the clock with NTP

    struct tm timeinfo; // Structure to store the current time
    bool timeInitialized; // Indicates if the clock has been initialized
    bool ntpAttempted; // Indicates if NTP synchronization has been attempted
    unsigned long ntpStartTime; // Time when NTP synchronization was attempted
    bool clockStarted; // Indicates if the clock has been started
};

#endif