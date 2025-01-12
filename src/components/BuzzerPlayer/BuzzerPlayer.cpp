// src/components/BuzzerPlayer/BuzzerPlayer.cpp
#include "BuzzerPlayer.h"

BuzzerPlayer::BuzzerPlayer(int buzzerPin) 
    : buzzerPin(buzzerPin), melody(nullptr), noteDurations(nullptr), songLength(0), 
      currentNote(0), previousMillis(0), noteDuration(0), isPlaying(false) {
    pinMode(buzzerPin, OUTPUT);
}

void BuzzerPlayer::loadSong(const int* melody, const int* noteDurations, int length) {
    this->melody = melody;
    this->noteDurations = noteDurations;
    this->songLength = length;
    this->currentNote = 0;
    this->isPlaying = true;
    this->previousMillis = millis();
    this->noteDuration = 1000 / noteDurations[0];
}

void BuzzerPlayer::play() {
    if (melody && noteDurations) {
        isPlaying = true;
        currentNote = 0;
        previousMillis = millis();
        noteDuration = 1000 / noteDurations[0];
        playNextNote();
    }
}

void BuzzerPlayer::stop() {
    isPlaying = false;
    noTone(buzzerPin);
}

void BuzzerPlayer::update() {
    if (!isPlaying) return;

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= noteDuration) {
        currentNote = (currentNote + 1) % songLength; // Loop the song
        previousMillis = currentMillis;
        noteDuration = 1000 / noteDurations[currentNote];
        playNextNote();
    }
}

void BuzzerPlayer::playNextNote() {
    if (melody && noteDurations) {
        tone(buzzerPin, melody[currentNote], noteDuration);
    }
}