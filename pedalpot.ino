// #include <BLEMIDI_Transport.h>
// #include <hardware/BLEMIDI_ESP32_NimBLE.h>

// BLEMIDI_CREATE_INSTANCE("PedalPot", MIDI)

// #include "MIDIUSB.h"

void setup() {
  Serial.begin(115200);

  // pinMode(LED_BUILTIN, OUTPUT);
  // digitalWrite(LED_BUILTIN, LOW);

  // BLEMIDI.setHandleConnected(OnConnected);
  // BLEMIDI.setHandleDisconnected(OnDisconnected);

  // MIDI.begin();
}

const byte channel = 0; // zero-based count
const byte pitch = 60;
const byte velocity = 64;

void loop() {
  int val = analogRead(A0);
  Serial.println(val);

  delay(500);
  // // MIDI.sendNoteOn(60, 127, 1);
  // midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  // MidiUSB.sendMIDI(noteOn);
  // MidiUSB.flush();
  // delay(500);

  // // MIDI.sendNoteOff(60, 127, 1);
  // midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  // MidiUSB.sendMIDI(noteOff);
  // MidiUSB.flush();
  // delay(1000);
}

// void OnConnected() { digitalWrite(LED_BUILTIN, HIGH); }

// void OnDisconnected() { digitalWrite(LED_BUILTIN, LOW); }
