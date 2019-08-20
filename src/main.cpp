/*
 * Copyright (c) Clinton Freeman 2018
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <ESP8266Wifi.h>
#include <WiFiUdp.h>
#include <Arduino.h>
#include <OSCMessage.h>
#include <SPI.h>
#include <Wire.h>
#include <APA102.h>

WiFiUDP Udp;

const char* ssid = "********";      // EditThis: The name of your WiFi access point.
const char* password = "********";  // EditThis: The password of your WiFi access point.
const IPAddress dstIp(10,1,1,3);    // EditThis: The IP address of the machine that recieves OSC messages.
const unsigned int dstPort = 8000;  // EditThis: The remote port that is listening for OSC messages.

// Setup the LED strip.
APA102<13, 14> ledStrip;
const uint16_t ledCount = 45;
const uint16_t controlCount = 10;
rgb_color colors[ledCount];

// The local listening port for UDP packets.
const unsigned int localPort = 1234;

// setup configures the underlying hardware for use in the main loop.
void setup() {
  Serial.begin(9600);
  Serial.print("WiFi");

  // Illuminate a strip yellow while we are booting.
  for (unsigned int i = 0; i < ledCount; i++) {
    colors[i] = (rgb_color){0, 0, 0};

    if (ledCount < 9) {
      colors[i] = (rgb_color){226, 158, 21};
    }
  }
  ledStrip.write(colors, ledCount, 2);

  // Prevent need for powercyle after upload.
  WiFi.disconnect();

  // Use DHCP to connect and obtain IP Address.
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait until we have connected to the WiFi AP.
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }

  // Init UDP to broadcast OSC messages.
  Udp.begin(localPort);
  Serial.println(" Connected!");

  // Switch the indicator strip to green after we have booted.
  for (unsigned int i = 0; i < ledCount; i++) {
    colors[i] = (rgb_color){0, 0, 0};

    if (ledCount < 9) {
      colors[i] = (rgb_color){135,180,137};
    }
  }
  ledStrip.write(colors, ledCount, 2);
}

uint8_t LERP(uint8_t left, uint8_t right, float ratio) {
  if (left < right) {
    return left + ((right - left) * ratio);
  } else {
    return left - ((left - right) * ratio);
  }
}

void render(unsigned int s, unsigned int l, OSCMessage &msg) {
  rgb_color control[controlCount];
  for (unsigned int i = 0; i < l; i++) {
    unsigned int j = i * 3;
    control[s + i] = (rgb_color){(uint8_t)msg.getFloat(j),
                                 (uint8_t)msg.getFloat(j + 1),
                                 (uint8_t)msg.getFloat(j + 2)};
  }

  // Interpolate all the other colors inbetween the control points.
  for (unsigned int i = 0; i < ledCount; i++) {
    unsigned int c = (i / 9) * 2;
    float ratio = (i % 9) / 9.0;

    if (c >= s && c < (s + l)) {
      rgb_color col = (rgb_color){LERP(control[c].red, control[c+1].red, ratio),
                                  LERP(control[c].green, control[c+1].green, ratio),
                                  LERP(control[c].blue, control[c+1].blue, ratio)};
      colors[i] = col;
    }
  }

  ledStrip.write(colors, ledCount, 20);
}

void updateB(OSCMessage &msg) {
  Serial.println("updatingB");
  render(10, 8, msg);
}

void update(OSCMessage &msg) {
  Serial.println("updating");
  render(0, 10, msg);
}

// loop executes over and over on the microcontroller. Reads messages from OSC
// and updates LED array accordingly.
void loop() {
  OSCMessage msg;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      msg.fill(Udp.read());
    }
    if (!msg.hasError()) {
      msg.dispatch("/isadora/1", update);
      msg.dispatch("/isadora/2", updateB);
    } else {
      OSCErrorCode error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
}
