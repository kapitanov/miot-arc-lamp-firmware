#include "os/os.h"
#include "cli.h"
#include "mqtt.h"

using namespace os;

void cmd_mqtt_status()
{
    attr(BLACK, BG_CYAN);
    print(F("MQTT status"));
    attr(RESET);
    println();

    printf(F("Hostname:    \"%s\"\r\n"), mqtt::get_hostname());
    printf(F("Username:    \"%s\"\r\n"), mqtt::get_username());
    printf(F("Password:    \"%s\"\r\n"), mqtt::get_password());
    printf(F("IsConnected: %s\r\n"), mqtt::is_connected() ? "YES" : "NO");

    printf(F("IsConnected: "));
    switch (mqtt::get_state())
    {
    case mqtt::INIT:
        println(F("INIT"));
        break;
    case mqtt::RECONNECT:
        println(F("RECONNECT"));
        break;
    case mqtt::CONNECT:
        println(F("CONNECT"));
        break;
    case mqtt::DISCONNECTED:
        println(F("DISCONNECTED"));
        break;
    case mqtt::RUNNING:
        println(F("RUNNING"));
        break;
    case mqtt::CONNECTED:
        println(F("CONNECTED"));
        break;
    case mqtt::WAIT:
        println(F("WAIT"));
        break;

    default:
        println(F("?"));
        break;
    }
}

void cmd_mqtt_connect(const String &args)
{

    String hostname, username, password;

    int i = args.indexOf(' ');
    if (i > 0)
    {
        hostname = args.substring(0, i);
        String s = args.substring(i + 1);

        i = s.indexOf(' ');
        if (i > 0)
        {
            username = s.substring(0, i);
            password = s.substring(i + 1);
        }
        else
        {
            username = s;
        }
    }
    else
    {
        hostname = args;
    }

    attr(RESET);
    print(F("Connecting to MQTT broker "));
    attr(CYAN);
    print(hostname);
    attr(RESET);
    print(F(" as "));
    attr(CYAN);
    print(username);
    attr(RESET);
    println();

    mqtt::connect(hostname, username, password);
}

void cmd_mqtt_reset()
{
    mqtt::reset();
    println(F("MQTT connection settings were deleted"));
}

bool cmd_mqtt(const String &args)
{
    int i = args.indexOf(' ');
    String command;
    String command_args;
    if (i > 0)
    {
        command = args.substring(0, i);
        command_args = args.substring(i + 1);
    }
    else
    {
        command = args;
    }

    if (command.equalsIgnoreCase(""))
    {
        cmd_mqtt_status();
        return true;
    }
    if (command.equalsIgnoreCase("connect"))
    {
        cmd_mqtt_connect(command_args);
        return true;
    }
    if (command.equalsIgnoreCase("reset"))
    {
        cmd_mqtt_reset();
        return true;
    }

    return false;
}
