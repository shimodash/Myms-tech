// ==============================================================================
// UIAPduino ジョイコンコントローラー・サーボ＆カウントダウン・スケッチ (完全版)
// ==============================================================================
// 【接続構成】
//  - ジョイコン X軸   ： アナログ入力 A0 端子 (サーボX可動域 & LED点滅速度制御)
//  - ジョイコン Y軸   ： アナログ入力 A1 端子 (サーボY可動域 & 通常時ブザーピッチ制御)
//  - トリガースイッチ ： デジタル入力 7番ピン (H->Lでカウントダウン再生)
//  - サーボ X軸 (8番) ： 90度 ± 45度 (45度 〜 135度)
//  - サーボ Y軸 (9番) ： 90度 ± 45度 (45度 〜 135度)
//  - 圧電ブザー       ： デジタル出力 10番ピン
//  - コントロールLED   ： PD4ピン (UIAPDUINO基板上の制御対象)
// ==============================================================================

#define JOYCON_X_PIN   A0
#define JOYCON_Y_PIN   A1
#define TRIGGER_PIN    7      // カウントダウン再生用入力ピン
#define BUZZER_PIN     10
#define SERVO_X_PIN    8      // X軸サーボ
#define SERVO_Y_PIN    9      // Y軸サーボ
#define LED_PIN        PD4

// ------------------------------------------------------------------------------
// キャリブレーション & サーボ設定
// ------------------------------------------------------------------------------
#define JOYSTICK_CENTER  512
#define DEADZONE         60

// X軸サーボ パルス幅制限 (90度 ± 45度 => 45度〜135度)
#define SERVO_X_MIN_PULSE 975   //  45度相当 (us)
#define SERVO_X_MAX_PULSE 1925  // 135度相当 (us)

// Y軸サーボ パルス幅制限 (90度 ± 45度 => 45度〜135度)
#define SERVO_Y_MIN_PULSE 975   //  45度相当 (us)
#define SERVO_Y_MAX_PULSE 1925  // 135度相当 (us)

// ------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------
// LED点滅用
unsigned long previousLedMillis = 0;
bool ledState = LOW;

// サーボパルス制御用タイマー
unsigned long previousServoMillis = 0;
int currentPulseWidthX = 1450; // 初期位置 (90度 = 1450us)
int currentPulseWidthY = 1450; // 初期位置 (90度 = 1450us)

// 7番ピン トリガー検知 & カウントダウン用変数
bool lastTriggerState = HIGH;
bool isCountingDown = false;
unsigned long countdownStartMillis = 0;

void setup() {
  // --- UIAPDUINO 必須記述セクション ---
  if (FLASH->STATR & (1 << 14)) NVIC_SystemReset();
  SystemReset_StartMode(Start_Mode_BOOT);
  pinMode(PD4, OUTPUT);
  // ----------------------------------

  // ピン入力設定
  pinMode(JOYCON_X_PIN, INPUT);
  pinMode(JOYCON_Y_PIN, INPUT);
  pinMode(TRIGGER_PIN, INPUT_PULLUP); // 7番ピン: 内部プルアップ有効

  // 出力ピン設定
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(SERVO_X_PIN, OUTPUT);
  pinMode(SERVO_Y_PIN, OUTPUT);

  // 初期出力状態設定
  digitalWrite(SERVO_X_PIN, LOW);
  digitalWrite(SERVO_Y_PIN, LOW);

  // シリアル通信開始 (115200bps)
  Serial.begin(115200);
  delay(100);
  Serial.println("UIAPDUINO Joycon & Dual Servo Controller Ready!");
}

void loop() {
  unsigned long currentMillis = millis();

  // アナログ値読み込み (0 〜 1023)
  int xVal = analogRead(JOYCON_X_PIN);
  int yVal = analogRead(JOYCON_Y_PIN);

  // ============================================================================
  // [1] サーボモーター制御 (20ms周期 / 非ブロッキング)
  // ============================================================================
  if (currentMillis - previousServoMillis >= 20) {
    previousServoMillis = currentMillis;

    // --- X軸サーボ 目標パルス幅計算 (975us 〜 1925us) ---
    int targetPulseX = SERVO_X_MIN_PULSE + ((long)xVal * (SERVO_X_MAX_PULSE - SERVO_X_MIN_PULSE)) / 1023;
    // 整数のみを用いたフィルタリング（スムーズな動きを実現）
    currentPulseWidthX += (targetPulseX - currentPulseWidthX) * 3 / 20;

    // --- Y軸サーボ 目標パルス幅計算 (975us 〜 1925us) ---
    int targetPulseY = SERVO_Y_MIN_PULSE + ((long)yVal * (SERVO_Y_MAX_PULSE - SERVO_Y_MIN_PULSE)) / 1023;
    // 整数のみを用いたフィルタリング
    currentPulseWidthY += (targetPulseY - currentPulseWidthY) * 3 / 20;

    // --- パルス出力実行 ---
    // X軸サーボ (8番ピン)
    digitalWrite(SERVO_X_PIN, HIGH);
    delayMicroseconds(currentPulseWidthX);
    digitalWrite(SERVO_X_PIN, LOW);

    // Y軸サーボ (9番ピン)
    digitalWrite(SERVO_Y_PIN, HIGH);
    delayMicroseconds(currentPulseWidthY);
    digitalWrite(SERVO_Y_PIN, LOW);
  }

  // ============================================================================
  // [2] X軸によるLED点滅コントロール
  // ============================================================================
  int xDeviation = abs(xVal - JOYSTICK_CENTER);
  if (xDeviation > DEADZONE) {
    // 傾きが大きいほど点滅間隔を短くする (30ms 〜 500ms)
    long blinkInterval = 500 - ((long)(xDeviation - DEADZONE) * 470) / (512 - DEADZONE);
    if (blinkInterval < 30) blinkInterval = 30;

    if (currentMillis - previousLedMillis >= (unsigned long)blinkInterval) {
      previousLedMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  // ============================================================================
  // [3] 7番ピンのトリガー検知 & カウントダウン再生
  // ============================================================================
  bool currentTriggerState = digitalRead(TRIGGER_PIN);

  // HIGH -> LOW (立ち下がり) の変化を検知
  if (lastTriggerState == HIGH && currentTriggerState == LOW) {
    isCountingDown = true;
    countdownStartMillis = currentMillis;
  }
  lastTriggerState = currentTriggerState;

  // --- カウントダウンシーケンス処理 ---
  if (isCountingDown) {
    unsigned long elapsed = currentMillis - countdownStartMillis;

    if (elapsed < 3000) {
      // 3秒間：1秒ごとに「ピッ」と短く鳴らす (880Hz / 150ms間)
      unsigned long phase = elapsed % 1000;
      if (phase < 150) {
        tone(BUZZER_PIN, 880);
      } else {
        noTone(BUZZER_PIN);
      }
    } 
    else if (elapsed < 4000) {
      // 3〜4秒目：「ピー！」と長く伸ばす (1760Hz / 1秒間)
      tone(BUZZER_PIN, 1760);
    } 
    else {
      // 4秒経過後にカウントダウン終了
      noTone(BUZZER_PIN);
      isCountingDown = false;
    }
  } 
  // ============================================================================
  // [4] 通常時のY軸によるブザー音コントロール (カウントダウン非実行時)
  // ============================================================================
  else {
    int yDeviation = abs(yVal - JOYSTICK_CENTER);
    if (yDeviation > DEADZONE) {
      int toneFrequency = 10 + ((long)(yDeviation - DEADZONE) * 2490) / (512 - DEADZONE);
      // tone(BUZZER_PIN, toneFrequency);
    } else {
      noTone(BUZZER_PIN);
    }
  }

  // ============================================================================
  // [5] デバッグ情報のシリアル出力 (200ms間隔)
  // ============================================================================
  static unsigned long previousLogMillis = 0;
  if (currentMillis - previousLogMillis >= 200) {
    previousLogMillis = currentMillis;

    // パルス幅から角度概算値を算出 (500us=0deg, 2400us=180deg)
    int angleX = ((long)(currentPulseWidthX - 500) * 180) / 1900;
    int angleY = ((long)(currentPulseWidthY - 500) * 180) / 1900;

    Serial.print("X:");
    Serial.print(xVal);
    Serial.print(" SX:");
    Serial.print(angleX);
    Serial.print("deg | Y:");
    Serial.print(yVal);
    Serial.print(" SY:");
    Serial.print(angleY);
    Serial.print("deg | Trig:");
    Serial.println(currentTriggerState ? "H" : "L");
  }

  // 動作安定化用の極小ウェイト
  delay(2);
}