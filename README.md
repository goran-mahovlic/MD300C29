# MD300C29

Fingertip Pulse Oximeter reading data

To get access to pin you only need to dissasemble upper part of device (blue cover)

![Image description](pic/upper.jpg)

![Image description](pic/mainCPU.jpg)

![Image description](pic/sensor.jpg)

![Image description](pic/botom.jpg)

![Image description](pic/wires.jpg)

No finger in device (last two bytes are 0)

![Image description](pic/noFinger.png)

Finger (check two last bytes (DEC))

![Image description](pic/finger.png)

Readings on display (same as two last bytes)

![Image description](pic/readings.jpg)

![Image description](pic/ArduinoData.png)

![Image description](pic/data.png)

![Image description](pic/graph.png)

Serial in PulseView is set to 115200

PulseView file is in data folder

CPU datasheet in datasheet folder
