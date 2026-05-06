// SubCore1 プログラム: サーボ制御、ディスプレイ表示、スイッチ入力、MainCoreへの通信を担当
#include <MP.h>

// 開発環境のCore設定がSubCore1になっているか確認
#if (SUBCORE != 1)
#error "Core selection is wrong!! CoreをSubCore1に設定して書き込んでください。"
#endif

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "UTF8_Adafruit_GC9A01A.h"
#include <Servo.h>
#include <math.h>

// サーボオブジェクトの宣言
static Servo s_servo0; // Pan（水平方向）用
static Servo s_servo1; // Tilt（垂直方向）用
int pan, tilt;
int prev_pan = -1, prev_tilt = -1; // 前回の値を保持（表示更新判定用）

// ★ スムーズ移動用：現在のサーボ角度をfloatで管理
float current_pan  = 80.0;
float current_tilt = 100.0;

// ★ 1ステップあたりの最大移動量（deg/10ms）。大きいほど速い
const float SERVO_SPEED = 0.5;

// タイマー制御用変数
unsigned long previousMillis = 0;
const long interval = 5000;  // 自動モード時の移動間隔（5秒）
bool autoMode = true;        // 自動巡回モードの有効フラグ
bool circleMode = false;     // 円運動モードの有効フラグ

// 円運動（Circle Mode）用のパラメータ
float angle = 0;              // 現在の角度（ラジアン）
const float angleStep = 0.3;  // 1ステップあたりの回転角
const int panCenter = 90;     // 水平方向の中心角
const int tiltCenter = 130;   // 垂直方向の中心角
const int panRadius = 20;     // 水平方向の振幅（半径）
const int tiltRadius = 20;    // 垂直方向の振幅（半径）

// 自動巡回（Auto Mode）時の座標パターン
int pan_positions[] = {80, 80, 100, 100};
int tilt_positions[] = {100, 140, 140, 100};
int current_position = 0;
const int num_positions = 4;

// ディスプレイのピン定義
#define TFT_DC  2
#define TFT_CS 32

// ディスプレイ制御用インスタンス
Adafruit_GC9A01A tft(TFT_CS, TFT_DC);

// ★ スイッチのエッジ検出用：前回の状態を保持する変数
bool prev_sw21 = HIGH;
bool prev_sw20 = HIGH;
bool prev_sw19 = HIGH;

// Stack-chan風の表情管理
enum FaceExpression {
  FACE_NEUTRAL,
  FACE_HAPPY,
  FACE_SAD,
  FACE_SLEEPY
};

FaceExpression currentExpression = FACE_NEUTRAL;
unsigned long blinkTimer = 0;
bool blinking = false;

// 関数プロトタイプ宣言
void displayServoAngles();
void updateServos();
void drawFace(FaceExpression expression, bool force = false);
void drawEye(int cx, int cy, int w, int h, bool closeEye = false);
void drawMouth(FaceExpression expression);

void setup() {
  MP.begin();
  Serial.begin(115200);
  delay(1000);
  Serial.println("Servo Display Control - Auto Mode Startup");

  int ret = 0;

  s_servo0.attach(PIN_D06);
  s_servo1.attach(PIN_D05);

  pinMode(21, INPUT_PULLUP);
  pinMode(20, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);

  tft.begin();
  tft.setRotation(2);

#if defined(TFT_BL)
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
#endif

  tft.fillScreen(GC9A01A_BLACK);

  pan = pan_positions[0];
  tilt = tilt_positions[0];
  current_pan  = (float)pan;
  current_tilt = (float)tilt;

  s_servo0.write(pan);
  s_servo1.write(tilt);

  displayServoAngles();
  drawFace(FACE_NEUTRAL, true);

  Serial.println("Setup complete! Auto movement every 5 seconds.");
  Serial.println("Commands: 'h' = Home, 'a' = Auto, 'c' = Circle");
}

void loop() {
  int ret;
  int8_t sndid = 100;
  int snddata;

  if (Serial.available() > 0) {
    char input = Serial.read();

    if (input == 'h' || input == 'H') {
      autoMode = false;
      circleMode = false;
      pan = 90;
      tilt = 140;
      currentExpression = FACE_SLEEPY;
      drawFace(currentExpression, true);
      displayServoAngles();
    }
    else if (input == 'a' || input == 'A') {
      autoMode = true;
      circleMode = false;
      previousMillis = millis();
      current_position = 0;
      pan = pan_positions[current_position];
      tilt = tilt_positions[current_position];
      currentExpression = FACE_NEUTRAL;
      drawFace(currentExpression, true);
      displayServoAngles();
    }
    else if (input == 'c' || input == 'C') {
      autoMode = false;
      circleMode = true;
      angle = 0;
      currentExpression = FACE_HAPPY;
      drawFace(currentExpression, true);
    }
  }

  bool sw21 = digitalRead(21);
  bool sw20 = digitalRead(20);
  bool sw19 = digitalRead(19);

  if (sw21 == LOW && prev_sw21 == HIGH) {
    autoMode = false;
    circleMode = true;
    angle = 0;
    currentExpression = FACE_HAPPY;
    drawFace(currentExpression, true);
  }

  if (sw20 == LOW && prev_sw20 == HIGH) {
    autoMode = false;
    circleMode = false;
    pan = 90;
    tilt = 140;
    currentExpression = FACE_SLEEPY;
    drawFace(currentExpression, true);

    snddata = 30;
    ret = MP.Send(sndid, snddata);
    if (ret < 0) {
      Serial.println("MP send error");
    }
  }

  if (sw19 == LOW && prev_sw19 == HIGH) {
    autoMode = true;
    circleMode = false;
    currentExpression = FACE_NEUTRAL;
    drawFace(currentExpression, true);

    snddata = -30;
    ret = MP.Send(sndid, snddata);
    if (ret < 0) {
      Serial.println("MP send error");
    }
  }

  prev_sw21 = sw21;
  prev_sw20 = sw20;
  prev_sw19 = sw19;

  if (circleMode) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 100) {
      previousMillis = currentMillis;
      pan  = panCenter  + (int)(panRadius  * cos(angle));
      tilt = tiltCenter + (int)(tiltRadius * sin(angle));
      angle += angleStep;
      if (angle >= 2 * PI) angle = 0;
    }
  }
  else if (autoMode) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      current_position = (current_position + 1) % num_positions;
      pan  = pan_positions[current_position];
      tilt = tilt_positions[current_position];
      currentExpression = (current_position % 2 == 0) ? FACE_NEUTRAL : FACE_HAPPY;
      drawFace(currentExpression, true);
      displayServoAngles();
    }
  }

  // 3秒ごとにまばたき
  if (millis() - blinkTimer > 3000) {
    blinkTimer = millis();
    blinking = true;
    drawFace(currentExpression, true);
  }
  if (blinking && millis() - blinkTimer > 130) {
    blinking = false;
    drawFace(currentExpression, true);
  }

  updateServos();
}

void updateServos() {
  unsigned long now = millis();
  static unsigned long lastUpdate = 0;

  if (now - lastUpdate < 10) return;
  lastUpdate = now;

  if (abs(current_pan - (float)pan) > SERVO_SPEED) {
    current_pan += (pan > current_pan) ? SERVO_SPEED : -SERVO_SPEED;
  } else {
    current_pan = (float)pan;
  }

  if (abs(current_tilt - (float)tilt) > SERVO_SPEED) {
    current_tilt += (tilt > current_tilt) ? SERVO_SPEED : -SERVO_SPEED;
  } else {
    current_tilt = (float)tilt;
  }

  s_servo0.write((int)current_pan);
  s_servo1.write((int)current_tilt);
}

void displayServoAngles() {
  if (pan == prev_pan && tilt == prev_tilt) {
    return;
  }

  tft.fillRect(0, 200, 240, 40, GC9A01A_BLACK);

  tft.setCursor(60, 200);
  tft.setTextColor(GC9A01A_GREEN);
  tft.print("Pan : ");
  tft.setTextColor(GC9A01A_WHITE);
  tft.print(pan);
  tft.print(" deg");

  tft.setCursor(60, 205);
  tft.setTextColor(GC9A01A_YELLOW);
  tft.print("Tilt: ");
  tft.setTextColor(GC9A01A_WHITE);
  tft.print(tilt);
  tft.print(" deg");

  tft.setCursor(60, 220);
  tft.setTextColor(GC9A01A_MAGENTA);
  if (circleMode) {
    tft.print("CIRCLE MODE");
  } else if (autoMode) {
    tft.print("Position: ");
    tft.print(current_position + 1);
    tft.print("/");
    tft.print(num_positions);
  } else {
    tft.print("STOPPED - Home Pos");
  }

  prev_pan = pan;
  prev_tilt = tilt;
}

void drawFace(FaceExpression expression, bool force) {
  static FaceExpression prevExpression = FACE_SAD;
  static bool prevBlink = false;

  if (!force && expression == prevExpression && blinking == prevBlink) {
    return;
  }

  // 顔エリアのみクリア（下部情報は残す）
  tft.fillRect(0, 0, 240, 190, GC9A01A_BLACK);

  // Stack-chan風: 横長の丸角目
  drawEye(72, 84, 46, 28, blinking);
  drawEye(168, 84, 46, 28, blinking);

  // 口
  drawMouth(expression);

  prevExpression = expression;
  prevBlink = blinking;
}

void drawEye(int cx, int cy, int w, int h, bool closeEye) {
  if (closeEye) {
    tft.fillRoundRect(cx - w / 2, cy - 2, w, 4, 2, GC9A01A_WHITE);
    return;
  }

  tft.fillRoundRect(cx - w / 2, cy - h / 2, w, h, 8, GC9A01A_WHITE);
}

void drawMouth(FaceExpression expression) {
  switch (expression) {
    case FACE_HAPPY:
      tft.fillRoundRect(88, 148, 64, 14, 7, GC9A01A_WHITE);
      break;

    case FACE_SAD:
      tft.drawFastHLine(92, 156, 56, GC9A01A_WHITE);
      tft.drawFastHLine(90, 158, 60, GC9A01A_WHITE);
      break;

    case FACE_SLEEPY:
      tft.fillRoundRect(106, 152, 28, 8, 4, GC9A01A_WHITE);
      break;

    case FACE_NEUTRAL:
    default:
      tft.fillRoundRect(96, 152, 48, 10, 5, GC9A01A_WHITE);
      break;
  }
}
