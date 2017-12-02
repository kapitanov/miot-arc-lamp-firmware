#include "os/os.h"
#include "indicator.h"

using namespace os;

#define PIN_RED D5
#define PIN_GREEN D6
#define PIN_BLUE D7
#define PIN_RING D8

namespace indicator
{
state_t state_ring;
state_t state_core_r;
state_t state_core_g;
state_t state_core_b;

uint8_t state_iter;

void thread();
void update();
void update(int pin, state_t state);
}

void indicator::update()
{
    indicator::update(PIN_RING, indicator::state_ring);
    indicator::update(PIN_RED, indicator::state_core_r);
    indicator::update(PIN_GREEN, indicator::state_core_g);
    indicator::update(PIN_BLUE, indicator::state_core_b);
}

void indicator::update(int pin, indicator::state_t state)
{
    switch (state)
    {
    case indicator::OFF:
        analogWrite(pin, 0);
        break;

    case indicator::ON:
        analogWrite(pin, 255);
        break;

    case indicator::BLINK:
        switch (indicator::state_iter % 4)
        {
        case 0:
            analogWrite(pin, 0);
            break;
        case 1:
            analogWrite(pin, 127);
            break;
        case 2:
            analogWrite(pin, 255);
            break;
        case 3:
            analogWrite(pin, 127);
            break;
        }
        break;

    case indicator::BLINK_2:
        switch (indicator::state_iter % 4)
        {
        case 0:
            analogWrite(pin, 255);
            break;
        case 1:
            analogWrite(pin, 127);
            break;
        case 2:
            analogWrite(pin, 127);
            break;
        case 3:
            analogWrite(pin, 127);
            break;
        }
        break;
    }
}

void indicator::thread()
{
    indicator::state_iter++;

    indicator::update();

    set_delay(100);
}

const char *indicator::print(state_t s)
{
    switch (s)
    {
    case indicator::OFF:
        return "OFF";
    case indicator::ON:
        return "ON";
    case indicator::BLINK:
        return "BLINK";
    case indicator::BLINK_2:
        return "BLINK_2";
    default:
        return "?";
    }
}

indicator::state_t indicator::parse(const String &s)
{
    if (s.equalsIgnoreCase("OFF"))
    {
        return indicator::OFF;
    }

    if (s.equalsIgnoreCase("ON"))
    {
        return indicator::ON;
    }

    if (s.equalsIgnoreCase("BLINK"))
    {
        return indicator::BLINK;
    }

    if (s.equalsIgnoreCase("BLINK_2"))
    {
        return indicator::BLINK_2;
    }

    return indicator::OFF;
}

void indicator::init()
{
    pinMode(PIN_RED, OUTPUT);
    pinMode(PIN_GREEN, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);
    pinMode(PIN_RING, OUTPUT);

    create_thread(indicator::thread, "indicator");
}

void indicator::get(state_t &ring, state_t &red, state_t &green, state_t &blue)
{
    ring = indicator::state_ring;
    red = indicator::state_core_r;
    green = indicator::state_core_g;
    blue = indicator::state_core_b;
}

void indicator::set(indicator::state_t ring, indicator::state_t red, indicator::state_t green, indicator::state_t blue, bool direct)
{
    indicator::state_ring = ring;
    indicator::state_core_r = red;
    indicator::state_core_g = green;
    indicator::state_core_b = blue;

    if (direct)
    {
        indicator::update();
    }
}