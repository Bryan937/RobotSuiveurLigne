#ifndef SONG_H
#define SONG_H

#include <stdint.h>

class Sound
{
public:
    Sound();

    void playNote(uint8_t note);
    void clearNote();
    void playDeepNote();
    void playHighNote();
    
private:
    void initialisation();
};
#endif