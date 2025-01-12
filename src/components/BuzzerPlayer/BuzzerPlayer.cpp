#include "BuzzerPlayer.h"
#include "../../assets/sounds/song1.h" // Include song1.h to access REST

/**
 * @brief Constructor for the BuzzerPlayer class.
 * 
 * Initializes the buzzer player with the specified GPIO pin and sets up the buzzer for output.
 * 
 * @param buzzerPin The GPIO pin connected to the buzzer.
 */
BuzzerPlayer::BuzzerPlayer(int buzzerPin) 
    : buzzerPin(buzzerPin), melody(nullptr), songLength(0), 
      currentNote(0), previousMillis(0), noteDuration(0), isPlaying(false), tempo(0) {
    pinMode(buzzerPin, OUTPUT);
}

/**
 * @brief Loads a melody into the buzzer player.
 * 
 * This function sets the melody, its length, and the playback tempo. The melody is defined as an array
 * of integers, where each pair of values represents a note and its duration.
 * 
 * @param melody Pointer to the melody array (notes and durations).
 * @param length Length of the melody array.
 * @param tempo Playback tempo in beats per minute (BPM).
 */
void BuzzerPlayer::loadSong(const int* melody, int length, int tempo) {
    this->melody = melody;
    this->songLength = length;
    this->tempo = tempo;
    this->currentNote = 0;
    this->isPlaying = true;
    this->previousMillis = millis();
    this->noteDuration = calculateNoteDuration(melody[1]); // Duration is in the second element
}

/**
 * @brief Starts playing the loaded melody.
 * 
 * This function begins playback of the melody from the first note.
 */
void BuzzerPlayer::play() {
    if (melody) {
        isPlaying = true;
        currentNote = 0;
        previousMillis = millis();
        noteDuration = calculateNoteDuration(melody[1]); // Duration is in the second element
        playNextNote();
    }
}

/**
 * @brief Stops playback of the melody.
 * 
 * This function stops the buzzer and halts playback.
 */
void BuzzerPlayer::stop() {
    isPlaying = false;
    noTone(buzzerPin);
}

/**
 * @brief Updates the playback state.
 * 
 * This function should be called in the main loop to ensure proper playback timing. It handles
 * transitioning between notes based on their durations.
 */
void BuzzerPlayer::update() {
    if (!isPlaying) return;

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= noteDuration) {
        // Move to the next note (each note and duration is a pair in the melody array)
        currentNote = (currentNote + 2) % songLength;
        previousMillis = currentMillis;
        noteDuration = calculateNoteDuration(melody[currentNote + 1]); // Duration is in the odd position
        playNextNote();
    }
}

/**
 * @brief Plays the next note in the melody.
 * 
 * This function generates a tone for the current note using the buzzer. If the note is a rest (REST),
 * no tone is played.
 */
void BuzzerPlayer::playNextNote() {
    if (melody) {
        int notePitch = melody[currentNote]; // Note pitch is in the even position
        if (notePitch != REST) { // Do not play if it's a rest
            tone(buzzerPin, notePitch, noteDuration * 0.9); // Play the note
        }
    }
}

/**
 * @brief Calculates the duration of a note based on the tempo.
 * 
 * This function converts the note's duration (as a divider) into milliseconds based on the current tempo.
 * 
 * @param divider The duration of the note (e.g., 4 for a quarter note, -8 for a dotted eighth note).
 * @return The duration of the note in milliseconds.
 */
int BuzzerPlayer::calculateNoteDuration(int divider) {
    int wholenote = (60000 * 4) / tempo; // Duration of a whole note in milliseconds
    if (divider > 0) {
        return wholenote / divider; // Regular note
    } else if (divider < 0) {
        return (wholenote / abs(divider)) * 1.5; // Dotted note
    }
    return 0; // Rest (REST)
}

/**
 * @brief Sets the playback tempo.
 * 
 * This function updates the tempo of the melody, affecting the speed of playback.
 * 
 * @param tempo The new tempo in beats per minute (BPM).
 */
void BuzzerPlayer::setTempo(int tempo) {
    this->tempo = tempo;
}