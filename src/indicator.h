#pragma once

#include <Arduino.h>

namespace  indicator
{
enum state_t
{
    OFF,
    ON,
    BLINK,
    BLINK_2
};

void init();

const char* print(state_t s);
state_t parse(const String& s);

void get(state_t& ring, state_t& red, state_t& green, state_t& blue);
void set(state_t ring, state_t red, state_t green, state_t blue, bool direct = false);
};