#pragma once

#include <Arduino.h>

bool cli_handler(const String &command, const String &args);

bool cmd_help(const String &args);
bool cmd_wifi(const String &args);
bool cmd_mqtt(const String &args);
bool cmd_state(const String &args);