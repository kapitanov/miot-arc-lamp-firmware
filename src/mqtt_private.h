#include "os/os.h"
#include "mqtt.h"

#include <MQTTClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define MQTT_CLIENT "arc_reactor_esp8266"
#define MQTT_REQUEST_TOPIC "/arc/request"
#define MQTT_STATE_TOPIC "/arc/state"

namespace mqtt
{
extern WiFiClient _wifi_client;
extern MQTTClient _mqtt;

extern StaticJsonBuffer<512> _json_buffer;
extern callback_t _callback;

extern os::thread_id _thread_id;

extern state_t _state;

struct config_t
{
    char host[64];
    char username[64];
    char password[64];
};

extern config_t _config;

void _cfg_read();
void _cfg_write();
uint32_t _crc(const uint8_t *buffer, size_t length);

void _on_message(String &topic, String &payload);
void _set_state(state_t state);

namespace fsm
{

void init();
void connect();
void wait_connect();
void connected();
void reconnect();
void loop();
void disconnected();
}
}