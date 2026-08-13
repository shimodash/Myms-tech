# UIAPduino NeoPixel Scroll Text

8x8 NeoPixel マトリックスにテキストをスクロール表示するArduino スケッチです。

## 概要

このプロジェクトは、8x8のアドレス指定可能なLED (NeoPixel) マトリックスを使用して、テキストを上下にスクロール表示します。

## 機能

- **8x8 NeoPixel マトリックス対応**: 64個の WS2812B LED を制御
- **8x8 ドットフォント**: ASCII 文字 (32-126) をビットマップで定義
- **スクロールアニメーション**: テキストを上から下へスクロール表示
- **色カスタマイズ**: RGB色で文字の色を指定可能
- **スクロール速度調整**: デレイ値で表示速度をコントロール

## ハードウェア要件

- **Arduino互換マイクロコントローラー** (例: Arduino Nano, Arduino Micro など)
- **WS2812B NeoPixel LED**: 64個 (8x8 マトリックス)
- **電源**: 5V (最大 3A 推奨)
- **接続ピン**: GPIO 6 (デフォルト)

## 配線

```
NeoPixel GND ---- Arduino GND
NeoPixel VDD ---- Arduino 5V
NeoPixel DIN ---- Arduino Pin 6
```

## ソフトウェア要件

- Arduino IDE 1.8.0 以上
- Adafruit NeoPixel ライブラリ

### ライブラリのインストール

Arduino IDE で「ライブラリを管理」を開き、「Adafruit NeoPixel」を検索してインストールしてください。

```
Sketch → Include Library → Manage Libraries...
```

## 使用方法

1. Arduinoボードを選択 (Sketch → Board)
2. シリアルポート を選択 (Tools → Serial Port)
3. スケッチをアップロード (Sketch → Upload)

## コード解説

### 主要な機能

#### `xy_to_pixel_index(int x, int y)`
マトリックス座標 (x, y) を1次元のピクセルインデックスに変換します。

```cpp
int xy_to_pixel_index(int x, int y) {
  if (x < 0 || x >= 8 || y < 0 || y >= 8) {
    return -1; // 範囲外
  }
  return y * 8 + x;
}
```

#### `draw_char(char c, int offset_x, int offset_y, uint32_t color)`
1文字を指定した位置に指定した色で表示します。

```cpp
void draw_char(char c, int offset_x, int offset_y, uint32_t color)
```

#### `draw_string(const char* str, int offset_x, int offset_y, uint32_t color)`
複数文字を縦方向に配置して表示します。

```cpp
void draw_string(const char* str, int offset_x, int offset_y, uint32_t color)
```

### パラメータのカスタマイズ

`loop()` 関数内で以下の値を編集できます:

```cpp
const char* text = "uiapduino demo";  // 表示テキスト
int text_length = 14;                 // テキストの文字数
int scroll_speed = 70;                // スクロール速度 (ミリ秒、小さいほど速い)
pixels.Color(50, 80, 235)            // RGB色 (R, G, B)
pixels.setBrightness(50)             // 明度 (0-255)
```

## フォント

8x8 ドットフォントデータは`font8x8`配列に格納されており、ASCII文字32～126に対応しています。
各文字は8バイト (8x8ビット) で表現されます。

## カスタマイズ例

### 色を変更
```cpp
pixels.Color(255, 0, 0)    // 赤
pixels.Color(0, 255, 0)    // 緑
pixels.Color(0, 0, 255)    // 青
pixels.Color(255, 255, 0)  // 黄
```

### スクロール速度を変更
```cpp
int scroll_speed = 50;  // 高速
int scroll_speed = 100; // 低速
```

### 明度を変更
```cpp
pixels.setBrightness(100);  // より明るく
pixels.setBrightness(20);   // より暗く
```

## トラブルシューティング

### LEDが反応しない
- 電源接続を確認してください
- シリアルポートが正しく選択されているか確認してください
- ボーレート設定を確認してください

### 文字が正しく表示されない
- フォントデータが正しく読み込まれているか確認してください
- `PROGMEM` キーワードが指定されているか確認してください

### スクロール速度が遅い
- `scroll_speed` の値を小さくしてください

## ライセンス

このプロジェクトは自由に使用・改変・配布できます。

## 関連リソース

- [Adafruit NeoPixel ライブラリ](https://github.com/adafruit/Adafruit_NeoPixel)
- [WS2812B データシート](https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf)
- [Arduino 公式ドキュメント](https://www.arduino.cc/en/Guide)
