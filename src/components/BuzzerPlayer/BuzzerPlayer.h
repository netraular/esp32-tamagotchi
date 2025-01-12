#ifndef BUZZERPLAYER_H
#define BUZZERPLAYER_H

#include <Arduino.h>

/**
 * @class BuzzerPlayer
 * @brief A component for playing melodies on a buzzer.
 * 
 * This class allows you to play melodies using a buzzer connected to a GPIO pin. It supports loading
 * melodies, setting the tempo, and controlling playback (play, stop, and update). The melodies are
 * defined as arrays of notes and durations, making it easy to create and play custom tunes.
 */
class BuzzerPlayer {
public:
    BuzzerPlayer(int buzzerPin);
    void loadSong(const int* melody, int length, int tempo); // Load a melody
    void play(); // Start playing the loaded melody
    void stop(); // Stop playback
    void update(); // Update the playback state (call in the main loop)
    void setTempo(int tempo); // Set the playback tempo

private:
    int buzzerPin; // GPIO pin connected to the buzzer
    const int* melody; // Pointer to the melody array (notes and durations)
    int songLength; // Length of the melody array
    int currentNote; // Index of the current note being played
    unsigned long previousMillis; // Timestamp of the last note change
    unsigned long noteDuration; // Duration of the current note
    bool isPlaying; // Indicates whether the melody is currently playing
    int tempo; // Playback tempo (beats per minute)

    void playNextNote(); // Play the next note in the melody
    int calculateNoteDuration(int divider); // Calculate the duration of a note based on the tempo
};

#endif