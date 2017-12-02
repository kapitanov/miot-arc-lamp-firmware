#include "os/os.h"
#include "mqtt_private.h"
#include <EEPROM.h>
#include <CRC32.h>

using namespace mqtt;
using namespace os;

WiFiClient mqtt::_wifi_client;
MQTTClient mqtt::_mqtt;

StaticJsonBuffer<512> mqtt::_json_buffer;
callback_t mqtt::_callback;
thread_id mqtt::_thread_id;

config_t mqtt::_config;
state_t mqtt::_state;

void mqtt::_on_message(String &topic, String &payload)
{
    {
        lock lock;
        printf(F("mqtt: <<< [%s] "), topic.c_str());
        attr(INVERSE);
        print(payload.c_str());
        attr(RESET);
        println();
    }

    _json_buffer.clear();
    JsonObject &json = _json_buffer.parseObject(payload);
    _callback(json);
}

void mqtt::init(callback_t callback)
{
    _callback = callback;
    _cfg_read();

    _thread_id = create_thread(fsm::init, "mqtt");
    _state = INIT;
}

bool mqtt::is_connected()
{
    return _mqtt.connected();
}

state_t mqtt::get_state()
{
    return _state;
}

const char *mqtt::get_hostname()
{
    return _config.host;
}

const char *mqtt::get_username()
{
    return _config.username;
}

const char *mqtt::get_password()
{
    return _config.password;
}

void mqtt::connect(const String &host, const String &username, const String &password)
{
    strcpy(_config.host, host.c_str());
    strcpy(_config.username, username.c_str());
    strcpy(_config.password, password.c_str());

    _cfg_write();

    _set_state(RECONNECT);
}

void mqtt::reset()
{
    strcpy(_config.host, "");
    strcpy(_config.username, "");
    strcpy(_config.password, "");

    _cfg_write();

    _set_state(RECONNECT);
}

void mqtt::_cfg_read()
{
    lock l;
    println(F("mqtt: reading config"));

    memset(&_config, 0, sizeof(config_t));
    EEPROM.begin(sizeof(config_t) + sizeof(uint32_t));

    for (size_t i = 0; i < sizeof(config_t); i++)
    {
        *((uint8_t *)&_config + i) = EEPROM.read(i);
    }

    uint32_t crc = 0;
    for (size_t i = 0; i < sizeof(uint32_t); i++)
    {
        *((uint8_t *)&crc + i) = EEPROM.read(sizeof(config_t) + i);
    }

    EEPROM.end();

    uint32_t actual_crc = _crc((uint8_t *)&_config, sizeof(config_t));
    if (crc != actual_crc)
    {
        attr(RED);
        printf(F("mqtt: config is damaged (CRC 0x%08X)\r\n"), crc);
        attr(RESET);

        reset();
        return;
    }

    printf(F("mqtt: config is ready (CRC 0x%08X)\r\n"), crc);
}

void mqtt::_cfg_write()
{
    lock l;
    println(F("mqtt: writing config"));

    EEPROM.begin(sizeof(config_t) + sizeof(uint32_t));

    const uint8_t *buffer = (uint8_t *)&_config;
    for (size_t i = 0; i < sizeof(config_t); i++)
    {
        EEPROM.write(i, buffer[i]);
    }

    uint32_t crc = _crc(buffer, sizeof(config_t));
    for (size_t i = 0; i < sizeof(uint32_t); i++)
    {
        EEPROM.write(sizeof(config_t) + i, *((uint8_t *)&crc + i));
    }

    EEPROM.commit();
    EEPROM.end();

    printf(F("mqtt: config is saved (CRC 0x%08X)\r\n"), crc);
}

uint32_t mqtt::_crc(const uint8_t *buffer, size_t length)
{
    CRC32 crc;
    for (size_t i = 0; i < length; i++)
    {
        crc.update(buffer[i]);
    }

    return crc.finalize();
}

void mqtt::_set_state(state_t state)
{
    switch (state)
    {
    case INIT:
        _state = INIT;
        set_func(_thread_id, fsm::init, "init");
        break;
    case CONNECT:
        _state = CONNECT;
        set_func(_thread_id, fsm::connect, "connect");
        break;
    case RECONNECT:
        _state = RECONNECT;
        set_func(_thread_id, fsm::reconnect, "reconnect");
        break;
    case DISCONNECTED:
        _state = DISCONNECTED;
        set_func(_thread_id, fsm::disconnected, "disconnected");
        break;
    case RUNNING:
        _state = RUNNING;
        set_func(_thread_id, fsm::loop, "loop");
        break;
    case CONNECTED:
        _state = CONNECTED;
        set_func(_thread_id, fsm::connected, "connected");
        break;
    case WAIT:
        _state = WAIT;
        set_func(_thread_id, fsm::wait_connect, "wait_connect");
        break;
    }
}