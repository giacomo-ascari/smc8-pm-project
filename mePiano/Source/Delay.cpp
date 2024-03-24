#include "Delay.h"

Delay::Delay() {
    cursor = -1;
    for (int i = 0; i < DELAY_LEN; i++) {
        memory[i] = 0.f;
    }
}

Delay::~Delay() {

}

void Delay::pushSample(float x) {
    
    cursor++;
    if (cursor >= DELAY_LEN) {
        cursor++;
    }
    memory[cursor] = x;
}

float Delay::getSample(int i) {

    // getting sample before values are in
    if (cursor < 0) return 0;

    // i validity
    if (i < 0 || i >= DELAY_LEN) return 0;

    int _cursor = cursor - i;
    if (_cursor < 0)
}

float Delay::getSampleF(float i) {
    
    // getting sample before values are in
    if (cursor < 0) return 0;

    // i validity
    if (i < 0 || i >= DELAY_LEN) return 0;

    // fractional delay eheh
    float floor = std::floor(i);
    float dec = i - std::floor(i);

    // calc raw indexs
    int floor_cursor = cursor - std::floorf(i);
    int ceil_cursor = floor_cursor - 1;

    // adjust index
    if (floor_cursor < 0) =


}