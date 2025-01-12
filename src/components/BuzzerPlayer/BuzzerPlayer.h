// src/components/BuzzerPlayer/BuzzerPlayer.h
#ifndef BUZZERPLAYER_H
#define BUZZERPLAYER_H

#include <Arduino.h>
#include <vector>

class BuzzerPlayer {
public:
    BuzzerPlayer(int buzzerPin);
    void loadSong(const int* melody, const int* noteDurations, int length);
    void play();
    void stop();
    void update();

private:
    int buzzerPin;
    const int* melody;
    const int* noteDurations;
    int songLength;
    int currentNote;
    unsigned long previousMillis;
    unsigned long noteDuration;
    bool isPlaying;

    void playNextNote();
};

#endif