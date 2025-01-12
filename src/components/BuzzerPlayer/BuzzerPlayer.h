// src/components/BuzzerPlayer/BuzzerPlayer.h
#ifndef BUZZERPLAYER_H
#define BUZZERPLAYER_H

#include <Arduino.h>

class BuzzerPlayer {
public:
    BuzzerPlayer(int buzzerPin);
    void loadSong(const int* melody, int length, int tempo); // Cambiamos la firma del método
    void play();
    void stop();
    void update();
    void setTempo(int tempo); // Método para ajustar el tempo

private:
    int buzzerPin;
    const int* melody;
    int songLength;
    int currentNote;
    unsigned long previousMillis;
    unsigned long noteDuration;
    bool isPlaying;
    int tempo; // Variable para almacenar el tempo

    void playNextNote();
    int calculateNoteDuration(int divider); // Método para calcular la duración de una nota
};

#endif