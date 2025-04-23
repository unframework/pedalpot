// #include <BLEMIDI_Transport.h>
// #include <hardware/BLEMIDI_ESP32_NimBLE.h>

// BLEMIDI_CREATE_INSTANCE("PedalPot", MIDI)

#include "MIDIUSB.h"

const int readPin = A0;
float accumulator = 0.0;

// min and max values as constrained by the safety resistors
// (observed on the NXP pedal)
const float minRange = 0.023f;
const float maxRange = 0.630f;
// (observed on test 9.6kOhm pot)
// const float minRange = 0.042;
// const float maxRange = 0.956;

const float dirThreshold = 0.004f;
float reported = 0.0f;
float highWat = 0.0f;
float lowWat = 0.0f;

int lastSentValue = -1;         // to avoid continuously sending the same value
int skippedCount = 0;           // occasionally resend the same value to refresh
const int maxSkippedCount = 60; // resend once about every second

float mapAnalogValue(int value) {
  float rawVal = value / 1023.0f;

  // may return outside of 0..1 range! clamp this elsewhere
  // (accumulator should not be clamped to 0..1)
  return (rawVal - minRange) / (maxRange - minRange);
}

void setup() {
  Serial.begin(115200);

  pinMode(readPin, INPUT);
  accumulator = mapAnalogValue(analogRead(readPin));
  reported = accumulator;
  highWat = reported;
  lowWat = reported;

  // pinMode(LED_BUILTIN, OUTPUT);
  // digitalWrite(LED_BUILTIN, LOW);

  // BLEMIDI.setHandleConnected(OnConnected);
  // BLEMIDI.setHandleDisconnected(OnDisconnected);

  // MIDI.begin();
}

const byte channel = 0; // zero-based count
const byte control = 4; // MIDI CC Foot Controller

void loop() {
  int rawRead = analogRead(readPin);
  float rawVal = mapAnalogValue(rawRead);
  accumulator = accumulator * 0.5f + rawVal * 0.5f;

  // update reported value if the watermarks are crossed
  if (accumulator > highWat) {
    highWat = reported = accumulator;
    lowWat = accumulator - dirThreshold;
  } else if (accumulator < lowWat) {
    lowWat = reported = accumulator;
    highWat = accumulator + dirThreshold;
  }

  // computed value may still be slightly outside of 0..1 range, clamp it to
  // MIDI range
  int val = min(127, max(0, (int)(reported * 127)));
  if (val != lastSentValue || skippedCount > maxSkippedCount) {
    lastSentValue = val;
    skippedCount = 0;

    Serial.println(val);

    midiEventPacket_t controlChange = {0x0B, 0xB0 | channel, control, val};
    MidiUSB.sendMIDI(controlChange);
    MidiUSB.flush();
  } else {
    skippedCount++;
  }

  delay(15);
}

// void OnConnected() { digitalWrite(LED_BUILTIN, HIGH); }

// void OnDisconnected() { digitalWrite(LED_BUILTIN, LOW); }
