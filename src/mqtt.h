#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

namespace mqtt
{
enum state_t
{
    INIT,
    RECONNECT,
    CONNECT,
    DISCONNECTED,
    RUNNING,
    CONNECTED,
    WAIT
};

typedef void (*callback_t)(const JsonObject &msg);

void init(callback_t callback);
bool is_connected();
const char *get_hostname();
const char *get_username();
const char *get_password();
state_t get_state();
void connect(const String &host, const String &username, const String &password);
void reset();
}