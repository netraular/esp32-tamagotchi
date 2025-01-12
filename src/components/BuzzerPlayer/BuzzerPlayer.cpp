// src/components/BuzzerPlayer/BuzzerPlayer.cpp
#include "BuzzerPlayer.h"
#include "../../assets/sounds/song1.h" // Incluir song1.h para acceder a REST

BuzzerPlayer::BuzzerPlayer(int buzzerPin) 
    : buzzerPin(buzzerPin), melody(nullptr), songLength(0), 
      currentNote(0), previousMillis(0), noteDuration(0), isPlaying(false), tempo(0) {
    pinMode(buzzerPin, OUTPUT);
}

void BuzzerPlayer::loadSong(const int* melody, int length, int tempo) {
    this->melody = melody;
    this->songLength = length;
    this->tempo = tempo;
    this->currentNote = 0;
    this->isPlaying = true;
    this->previousMillis = millis();
    this->noteDuration = calculateNoteDuration(melody[1]); // La duración está en la posición 1
}

void BuzzerPlayer::play() {
    if (melody) {
        isPlaying = true;
        currentNote = 0;
        previousMillis = millis();
        noteDuration = calculateNoteDuration(melody[1]); // La duración está en la posición 1
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
        currentNote = (currentNote + 2) % songLength; // Avanzar de 2 en 2 (nota y duración)
        previousMillis = currentMillis;
        noteDuration = calculateNoteDuration(melody[currentNote + 1]); // La duración está en la posición impar
        playNextNote();
    }
}

void BuzzerPlayer::playNextNote() {
    if (melody) {
        int notePitch = melody[currentNote]; // La nota está en la posición par
        if (notePitch != REST) { // No reproducir si es un silencio (REST)
            tone(buzzerPin, notePitch, noteDuration * 0.9); // Reproducir la nota
        }
    }
}

int BuzzerPlayer::calculateNoteDuration(int divider) {
    int wholenote = (60000 * 4) / tempo; // Duración de una nota entera en milisegundos
    if (divider > 0) {
        return wholenote / divider; // Nota regular
    } else if (divider < 0) {
        return (wholenote / abs(divider)) * 1.5; // Nota con puntillo
    }
    return 0; // Silencio (REST)
}

void BuzzerPlayer::setTempo(int tempo) {
    this->tempo = tempo; // Ajustar el tempo
}