miot-arc-lamp-firmware
======================

Compile
-------

```shell
platformio run
```

Compile and upload to board
---------------------------

```shell
platformio run --target upload
```

Configure
---------

1. Connect via COM terminal (ANSI compatible, like `putty`)
2. Type `help` to get some help
3. Connect to WiFi (see `wifi up` command)
4. Check Internet connection (see `wifi test` command)
5. Connect to MQTT broker (see `mqtt connect` command)