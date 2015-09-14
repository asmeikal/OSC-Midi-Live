**This is the first iteration of an ongoing project. The next iteration is in development.**

# OSC-Midi-Live

This is an experimental project. Our target is a live performane at the end of August (2015).

The main goal is to create a library that allows a trustworthy and solid communication between a set of sensor and a computer running [**OSCulator**][osculator]. Our idea involves:

- some **Arduino**, each connected to a set of sensors, and each communicating over USB serial to a Raspberry Pi;
- a **Raspberry Pi**, running Raspbian, and receiving the sensor readings from the Arduino, and implementing a (primitive) policy to send out OSC messages (over ethernet) to the computer running OSCulator;
- the computer running **OSCulator**, along with all the other software involved in the live performance.

The communication is unidirectional: `Arduino => Raspberry Pi => OSCulator`.

The main sensor we're using are DIY drumpads.

## OSC-Midi-Live Library how to

Let's assume `N` is the number of pins you're reading from. To use the library, in the `setup()` function, you'll need to start the serial at the desired speed (i.e. the baud rate you'll set on the receiving side), with:

    Serial.begin(BAUD_RATE);

You'll then need to create a vector of `struct pin_info` of size `N`. The `struct pin_info` is the following:

    struct pin_info {
      unsigned char pin;
      int (*callBack)(unsigned char);
    };

i.e., the identifier of a pin number, and a callback function with a single parameter (that will be our pin number) and returning an `int`. You could write, as an example:

    struct pin_info ReadInformation[3] = {
      {A0, analogRead},
      {A1, DRUMPAD_READ(A1)},
      {6,  digitalReadOSC}
    };

You'll also need a vector of `unsigned char` of the appropriate size, to build and hold the message. There's the `MSG_SIZE(x)` macro for that:

    unsigned char *message_buffer[MSG_SIZE(3)] = {0};

In your `loop()` all you have to do is call the `buildMessage()` function, which has the following signature:

    void buildMessage(unsigned char[], const struct pin_info[], const unsigned int);

The first parameter is our `message_buffer`, the second parameter is our `ReadInformation` vector, and the third parameter will be our `N`. Then, write the `message_buffer` to serial:

    Serial.write(message_buffer, MSG_SIZE(3));

That's it. Quite ugly.

On the receiving size, i.e. on the computer to which the Arduino (or the Arduinos) is connected, you'll have to run, in order:

    $ ./run.sh build
    $ ./run.sh configure
      [ip address]: ...
      [baud rate]: ...
      [devices]: ...
    $ ./run.sh

It'll guide you during the configuration.

## Arduino => Raspberry Pi protocol

Each Arduino will read the values from its connected sensors, and send those to the Raspberry Pi. The available read functions will return an integer in the range `0-1023`, which we'll scale down to fit in the range `0-255`, to fit in one byte.

Each message begins with the (hex) sequence `FE FE`, followed by couples of a data byte and a NULL byte, ending with the sequence `EF EF`. The values 240, 212, 180 will be sent out as:

    FE FE F0 00 D4 00 B4 00 EF EF 

This _should_ help recognize well formed messages, and discard garbage.

## Drumpad (and schematics)

Each drumpad is a piezo element, stitched to a metal plate, and surrounded in foam, following the many examples ([1][spikenzie1], [2][spikenzie2], [3][arduinodrumkit]) found on the web.

The circuit's schematics are roughly the following:

![piezo schematics][piezo]

Again, I'm following the examples found online, mainly [4][knocksensor], [5][use_a_zener], and some inspiration from [1][spikenzie1] and [2][spikenzie2].

I don't have the necessary knowledge to understand why and how this particular part of the project is working. It's frustrating most of the times, but I'll learn some day.

The 10k &#8486; resistor gave each blow on the drumpad a shorter "life" (compared to a 10M &#8486; resistor), which is something we strongly want.


[patternsinc]: https://leanpub.com/patternsinc "Patterns in C"
[osculator]: http://www.osculator.net "OSCulator"
[spikenzie1]: http://www.spikenzielabs.com/SpikenzieLabs/dkkai.html "Drum Kit - Kit AI"
[spikenzie2]: http://www.spikenzielabs.com/SpikenzieLabs/DrumKitKit.html "Drum Kit - Kit"
[arduinodrumkit]: https://arduinodrumkit.wordpress.com/2013/10/27/video-3-piece-drum-kit-demo-with-prototype-drum-pads/ "Video: drum kit demo"
[use_a_zener]: http://forum.arduino.cc/index.php?topic=41149.msg299767#msg299767 "Arduino Forum: reading piezo velocity"
[knocksensor]: https://www.arduino.cc/en/Tutorial/KnockSensor "Arduino: knock sensor"

[piezo]: http://asmeikal.me/images/schematics_piezo.svg "Piezo schematics - made with iCircuit (http://icircuitapp.com)"