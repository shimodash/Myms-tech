#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiAP.h>
#include <FastLED.h>

const char* ssid = "wifi-coaster";
const char* password = "1223334444";

IPAddress local_IP(192, 168, 5, 1);          // 設定したいIPアドレス
IPAddress gateway(192, 168, 5, 1);           // ゲートウェイも同じアドレスに設定
IPAddress subnet(255, 255, 255, 0);          // サブネットマスク

WebServer server(80);

#define NUM_LEDS 24
CRGBArray<NUM_LEDS> leds;

unsigned long previousMillis = 0;            // 最後に明るさを変更した時間
const long interval = 100;                   // 明るさを変える間隔（0.1秒）
CRGB currentColor = CRGB::Black;             // 現在のLEDの色
int brightnessLevel = 0;                     // 現在の明るさレベル（0〜10）
int fadeDirection = 1;                       // 明るさの増減方向（1:増加, -1:減少）

void handleRoot() {
  server.send(200, "text/html",
    "<html><body>"
    "<h1>RGB Control</h1>"
    "<form action=\"/set_color\" method=\"get\">"
    "R: <input type=\"range\" name=\"r\" min=\"0\" max=\"255\" value=\"0\" oninput=\"rValue.value=value\">"
    "<output id=\"rValue\">0</output><br>"
    "G: <input type=\"range\" name=\"g\" min=\"0\" max=\"255\" value=\"0\" oninput=\"gValue.value=value\">"
    "<output id=\"gValue\">0</output><br>"
    "B: <input type=\"range\" name=\"b\" min=\"0\" max=\"255\" value=\"0\" oninput=\"bValue.value=value\">"
    "<output id=\"bValue\">0</output><br>"
    "<input type=\"submit\" value=\"Set Color\">"
    "</form>"
    "</body></html>");
}

void handleSetColor() {
  int r = server.arg("r").toInt();
  int g = server.arg("g").toInt();
  int b = server.arg("b").toInt();

  // 現在の色を更新
  currentColor = CRGB(r, g, b);

  server.send(200, "text/html", 
    "<html><body>"
    "<h1>Color Set</h1>"
    "<p>R: " + String(r) + " G: " + String(g) + " B: " + String(b) + "</p>"
    "<a href=\"/\">Go Back</a>"
    "</body></html>");
}

void setup(void) {
  Serial.begin(115200);

  // SoftAPのIPアドレスを設定
  if (!WiFi.softAPConfig(local_IP, gateway, subnet)) {
    Serial.println("Failed to configure SoftAP!");
  }

  // SoftAPを開始
  if (!WiFi.softAP(ssid, password)) {
    Serial.println("Soft AP creation failed.");
    while(1);
  }

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
  Serial.println("Server started");

  server.on("/", handleRoot);
  server.on("/set_color", handleSetColor);
  server.onNotFound([]() {
    server.send(404, "text/plain", "Not Found");
  });

  FastLED.addLeds<NEOPIXEL, 26>(leds, NUM_LEDS);
  FastLED.setBrightness(127);

  // 初期化としてLEDsをオフに設定
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Green;
  }
  FastLED.show();
}

void loop(void) {
  server.handleClient();

  // 現在の時間を取得
  unsigned long currentMillis = millis();

  // interval（0.1秒）ごとに明るさを変更
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // 現在の色の明るさを調整して設定
    CRGB adjustedColor = currentColor;
    adjustedColor.nscale8(brightnessLevel * 25);  // 0〜10の範囲を0〜255スケールに変換

    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = adjustedColor;
    }
    FastLED.show();

    // 明るさレベルを更新
    brightnessLevel += fadeDirection;
    if (brightnessLevel == 10 || brightnessLevel == 0) {
      fadeDirection = -fadeDirection;  // 明るさの増減方向を反転
    }
  }

  delay(2); // allow the CPU to switch to other tasks
}
