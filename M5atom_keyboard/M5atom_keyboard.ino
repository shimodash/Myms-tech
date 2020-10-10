/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */
#include "M5Atom.h" 
#include <BleKeyboard.h>

BleKeyboard bleKeyboard("M5atomkeyboard");

void setup() {
  M5.begin(true, false, true);
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
}

void loop() {
  M5.update();
  if (M5.Btn.wasPressed()){
  if(bleKeyboard.isConnected()) {
    Serial.println("Sending 'Hello world'...");
    bleKeyboard.print("Hello world");
    

//    delay(1000);

    Serial.println("Sending Enter key...");
    bleKeyboard.write(KEY_RETURN);

    Serial.println("win + d");
    bleKeyboard.press(KEY_LEFT_GUI);
    bleKeyboard.press('d');
    delay(100);
    bleKeyboard.releaseAll();

//    delay(1000);
/*
    Serial.println("Sending Play/Pause media key...");
    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);

    delay(1000);

    Serial.println("Sending Ctrl+Alt+Delete...");
    bleKeyboard.press(KEY_LEFT_CTRL);
    bleKeyboard.press(KEY_LEFT_ALT);
    bleKeyboard.press(KEY_DELETE);
    delay(100);
    bleKeyboard.releaseAll();
*/
  }
  }
//  Serial.println("Waiting 5 seconds...");
  delay(200);
}
