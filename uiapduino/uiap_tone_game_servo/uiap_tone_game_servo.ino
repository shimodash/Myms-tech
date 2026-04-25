#include "pitches.h"

// ========================================
// ★ 設定エリア
// ========================================
const bool MISS_SOUND_ENABLED =false;  // ミス音: true=あり / false=なし
// ========================================

const int LED_PIN   = 2;
const int SW_PIN    = 4;
const int TONE_PIN  = 8;
const int SERVO_PIN = 7;

int hitCount = 0;

// ---- サーボ制御（ライブラリなし）----
void servoWrite(int pin, int angle, int durationMs) {
  int pulseWidth = map(angle, 0, 180, 1000, 2000);
  long start = millis();
  while (millis() - start < durationMs) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(pin, LOW);
    delayMicroseconds(20000 - pulseWidth);
  }
}

// ---- ヒット時サーボ動作 ----
void servoHit() {
  servoWrite(SERVO_PIN, 180, 300);
  servoWrite(SERVO_PIN,   0, 300);
}

// ---- ミス時サーボ動作 ----
void servoMiss() {
  for (int i = 0; i < 4; i++) {
    servoWrite(SERVO_PIN,  60, 80);
    servoWrite(SERVO_PIN, 120, 80);
  }
  servoWrite(SERVO_PIN, 90, 200);
}

// ---- 通常ヒット音 ----
void playHit() {
  tone(TONE_PIN, NOTE_C5, 100);
  delay(100);
  tone(TONE_PIN, NOTE_E5, 100);
  delay(100);
  tone(TONE_PIN, NOTE_G5, 150);
  delay(150);
  noTone(TONE_PIN);
}

// ---- 5連続ヒット！ファンファーレ ----
void playFanfare() {
  int melody[] = {
    NOTE_C5, NOTE_C5, NOTE_C5, NOTE_G4,
    NOTE_E5, NOTE_E5, NOTE_E5, NOTE_C5,
    NOTE_G5, NOTE_E5, NOTE_C5
  };
  int durations[] = {
    8, 8, 8, 2,
    8, 8, 8, 2,
    4, 4, 2
  };
  for (int i = 0; i < 11; i++) {
    int dur = 1000 / durations[i];
    tone(TONE_PIN, melody[i], dur);
    delay(dur * 1.3);
    noTone(TONE_PIN);
  }
}

// ---- ミス音（設定により再生しない）----
void playMiss() {
  if (!MISS_SOUND_ENABLED) return;  // falseなら即リターン
  tone(TONE_PIN, NOTE_B2, 300);
  delay(300);
  noTone(TONE_PIN);
}

void setup() {
  pinMode(LED_PIN,   OUTPUT);
  pinMode(SW_PIN,    INPUT_PULLUP);
  pinMode(SERVO_PIN, OUTPUT);
  randomSeed(analogRead(0));
  servoWrite(SERVO_PIN, 0, 500);
}

void loop() {
  // ---- 消灯インターバル（0.5〜2秒）----
  digitalWrite(LED_PIN, LOW);
  long offTime  = random(500, 2000);
  long offStart = millis();

  while (millis() - offStart < offTime) {
    if (digitalRead(SW_PIN) == LOW) {
      playMiss();
      servoMiss();
      hitCount = 0;
      delay(500);
      while (digitalRead(SW_PIN) == LOW) delay(10);
      delay(50);
      return;
    }
  }

  // ---- 点灯（0.3〜1秒）----
  digitalWrite(LED_PIN, HIGH);
  long onTime  = random(300, 1000);
  long onStart = millis();
  bool hit     = false;

  while (millis() - onStart < onTime) {
    if (digitalRead(SW_PIN) == LOW) {
      hit = true;
      break;
    }
  }

  digitalWrite(LED_PIN, LOW);

  if (hit) {
    hitCount++;
    if (hitCount >= 5) {
      playFanfare();
      servoHit();
      hitCount = 0;
    } else {
      playHit();
      servoHit();
    }
    while (digitalRead(SW_PIN) == LOW) delay(10);
    delay(50);

  } else {
    playMiss();
    servoMiss();
    hitCount = 0;
  }
}