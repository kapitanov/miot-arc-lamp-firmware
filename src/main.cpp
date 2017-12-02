#include <Arduino.h>
#include "os/os.h"
#include "indicator.h"
#include "mqtt.h"
#include "cli.h"

void on_message(const JsonObject &msg)
{
    indicator::set(
        indicator::parse(msg.get<String>("ring")),
        indicator::parse(msg.get<String>("core_r")),
        indicator::parse(msg.get<String>("core_g")),
        indicator::parse(msg.get<String>("core_b")));
}

void setup()
{
    indicator::init();
    os::init(cli_handler);
    mqtt::init(on_message);
}

void loop()
{
    os::run();
}
