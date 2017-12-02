#include "os/os.h"
#include "mqtt_private.h"

using namespace mqtt;
using namespace os;

void mqtt::fsm::init()
{
    _cfg_read();

    _mqtt.onMessage(_on_message);
    _mqtt.begin(_config.host, _wifi_client);

    if (strlen(_config.host) <= 0)
    {
        lock locker;
        attr(RED);
        println(F("MQTT connection is not configured"));
        attr(RESET);

        _set_state(DISCONNECTED);
        return;
    }

    _set_state(CONNECT);
}

void mqtt::fsm::reconnect()
{
    if (_mqtt.connected())
    {
        _mqtt.disconnect();
    }

    _set_state(CONNECT);
}

void mqtt::fsm::connect()
{
    if (strlen(_config.host) <= 0)
    {
        _set_state(DISCONNECTED);
        return;
    }

    _set_state(WAIT);
}

void mqtt::fsm::wait_connect()
{
    if (strlen(_config.host) <= 0)
    {
        _set_state(DISCONNECTED);
        return;
    }

    printf(F("mqtt: connecting to \"%s\" as \"%s\:%s\"\r\n"), _config.host, _config.username, _config.password);    
    _mqtt.setHost(_config.host);
    if (!_mqtt.connect(MQTT_CLIENT, _config.username, _config.password))
    {
        lock lock;
        attr(RED);
        printf(F("mqtt: failed to connect to \"%s\"\r\n"), _config.host);
        attr(RESET);

        set_delay(1000);
        return;
    }

    _set_state(CONNECTED);
}

void mqtt::fsm::connected()
{
    _mqtt.subscribe(MQTT_STATE_TOPIC);

    {
        lock lock;
        printf(F("mqtt: >>> [%s] "), MQTT_REQUEST_TOPIC);
        attr(INVERSE);
        print("REQ");
        attr(RESET);
        println();
    }
    _mqtt.publish(MQTT_REQUEST_TOPIC, "REQ");

    _set_state(RUNNING);
}

void mqtt::fsm::loop()
{
    _mqtt.loop();

    if (!_mqtt.connected())
    {
        attr(RED);
        println(F("mqtt:  connection lost"));
        attr(RESET);

        _set_state(CONNECT);
    }

    set_delay(10);
}

void mqtt::fsm::disconnected() {}