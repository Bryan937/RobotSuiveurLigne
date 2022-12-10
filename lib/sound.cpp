#define F_CPU 8000000UL

#include <util/delay.h>
#include <avr/io.h>
#include "sound.h"

// Const utilitaire
const uint16_t DELAY_MUSIC = 1000;
// Const pour la fréquence des notes
const uint8_t HIGH_NOTE = 165;
const uint8_t DEEP_NOTE = 255;

Sound::Sound()
{
    initialisation();
}

void Sound::initialisation()
{ 
    DDRD |= (1 << PIND7) | (1 << PIND6);
    TCNT1 = 0; 
    TCCR2A |= (1 << WGM10) | (1 << WGM11); 
    TCCR2B |= (1 << WGM12) | (1 << CS11);
}

void Sound::playNote(uint8_t note) 
{
    OCR2A = note;
    OCR2B = (note >> 1 );
    TCCR2A |= (1 << COM2B1);
}

void Sound::clearNote()
{
    OCR2A = 0;
    OCR2B = 0;
    TCCR2A &= ~(1 << COM2B0);
}

void Sound::playDeepNote()
{
    playNote(DEEP_NOTE);
    _delay_ms(DELAY_MUSIC);
    clearNote();
}

void Sound::playHighNote()
{
    playNote(HIGH_NOTE);
    _delay_ms(DELAY_MUSIC);
    clearNote();
}