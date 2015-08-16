# OSC-Midi-Live

This is an experimental project. Our target is a live performane at the end of August (2015).

The main goal is to create a library that allows a trustworthy and solid communication between a set of sensor and a computer running [OSCulator][osculator]. Our idea involves:

- some Arduino, each connected to a set of sensors, and each communicating over USB serial to a Raspberry Pi;
- a Raspberry Pi, running Debian, and receiving the sensor readings from the Arduino, and implementing a (primitive) policy to send out OSC messages (over ethernet) to the computer running OSCulator;
- the computer running OSCulator, along with all the other software involved in the live performance.

The communication is unidirectional: `Arduino => Raspberry Pi => OSCulator`.

The main sensor we're using are DIY drumpads.

## Drumpad (and schematics)

Each drumpad is a piezo element, stitched to a metal plate, and surrounded in foam, following the many examples ([1][spikenzie1], [2][spikenzie2], [3][arduinodrumkit]) found on the web.

The circuit's schematics are roughly the following:

![piezo schematics][piezo]

Again, I'm following the examples found online, mainly [4][knocksensor], [5][use_a_zener], and some inspiration from [1][spikenzie1] and [2][spikenzie2].

I don't have the necessary knowledge to understand why and how this particular part of the project is working. It's frustrating most of the times, but I'll learn some day.

The 10k &#8486; resistor gave each blow on the drumpad a shorter "life", which is something we strongly want.



[osculator]: http://www.osculator.net "OSCulator"
[spikenzie1]: http://www.spikenzielabs.com/SpikenzieLabs/dkkai.html "Drum Kit - Kit AI"
[spikenzie2]: http://www.spikenzielabs.com/SpikenzieLabs/DrumKitKit.html "Drum Kit - Kit"
[arduinodrumkit]: https://arduinodrumkit.wordpress.com/2013/10/27/video-3-piece-drum-kit-demo-with-prototype-drum-pads/ "Video: drum kit demo"
[use_a_zener]: http://forum.arduino.cc/index.php?topic=41149.msg299767#msg299767 "Arduino Forum: reading piezo velocity"
[knocksensor]: https://www.arduino.cc/en/Tutorial/KnockSensor "Arduino: knock sensor"

[piezo]: docs/img/schematics_piezo.svg "Piezo schematics - made with iCircuit (http://icircuitapp.com)"