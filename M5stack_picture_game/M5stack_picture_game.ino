#include <M5Stack.h>
#include <vector>

// グローバル変数
std::vector<String> imageFiles;
int currentImageIndex = 0;
bool isGameStarted = false;
bool isGameOver = false;
int imagesDisplayedCount = 0;
unsigned long startTime = 0;

// 関数のプロトタイプ宣言
void listImageFiles(fs::FS &fs, const char *dirname);
void displayImage(const String &path);
void showNextImage();
void displayStartScreen();
void displayEndScreen(float playTime);
void resetGame();

void setup() {
  M5.begin(true, true, true, false);
  M5.Power.begin();
  M5.Lcd.setTextSize(2);
  if (!SD.begin()) {
    M5.Lcd.println("SD Card Mount Failed");
    M5.Lcd.println("SDカードの読み込みに失敗しました");
    while (1);
  }

  displayStartScreen();

  M5.Speaker.tone(523, 200);
  delay(200);
  M5.Speaker.tone(587, 200);
  delay(200);
  M5.Speaker.tone(659, 200);
  delay(200);
  M5.Speaker.tone(698, 200);
  delay(200);
  M5.Speaker.tone(784, 400);
  delay(400);
}

void loop() {
  M5.update();

  if (isGameOver) {
    if (M5.BtnA.wasPressed()) {
      resetGame();
    }
    return;
  }

  if (!isGameStarted) {
    if (M5.BtnA.wasPressed()) {
      isGameStarted = true;
      startTime = millis();
      
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setCursor(0, 0);
      M5.Lcd.println("Scanning for JPG images...");
      M5.Lcd.println("JPG search ...");

      listImageFiles(SD, "/");

      if (imageFiles.empty()) {
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.println("No JPG images found.");
        M5.Lcd.println("JPG画像が見つかりません");
        while (1);
      }

      displayImage(imageFiles[currentImageIndex]);
    }
    return;
  }
  
  if (imageFiles.empty()) {
    return;
  }

  String fullPath = imageFiles[currentImageIndex];
  String fileName = fullPath.substring(fullPath.lastIndexOf('/') + 1);
  int underscoreIndex = fileName.indexOf('_');
  char buttonChar = ' ';
  if (underscoreIndex != -1 && underscoreIndex + 2 < fileName.length()) {
    buttonChar = fileName.charAt(underscoreIndex + 1);
  }

  if (M5.BtnA.wasPressed()) {
    if (buttonChar == 'R') {
      M5.Speaker.tone(880, 200);
      showNextImage();
    } else {
      M5.Speaker.tone(220, 200);
    }
  }

  if (M5.BtnB.wasPressed()) {
    if (buttonChar == 'G') {
      M5.Speaker.tone(880, 200);
      showNextImage();
    } else {
      M5.Speaker.tone(220, 200);
    }
  }

  if (M5.BtnC.wasPressed()) {
    if (buttonChar == 'B') {
      M5.Speaker.tone(880, 200);
      showNextImage();
    } else {
      M5.Speaker.tone(220, 200);
    }
  }
}

/**
 * @brief スタート画面を表示する
 */
void displayStartScreen() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(50, 80);
  M5.Lcd.println("Open Edge Device");
  M5.Lcd.setCursor(50, 100);
  M5.Lcd.println("Community");
  M5.Lcd.setCursor(50, 120);
  M5.Lcd.println("Simple Picture Game");
  M5.Lcd.setCursor(60, 140);
  M5.Lcd.println("Press Red to Start");
}

/**
 * @brief ゲーム終了画面を表示する
 * @param playTime プレイ時間（秒）
 */
void displayEndScreen(float playTime) {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(50, 80);
  M5.Lcd.println("Game Finish!");
  M5.Lcd.setCursor(10, 120);
  M5.Lcd.print("Play Time: ");
  M5.Lcd.print(playTime, 2); // 小数点第2位まで表示
  M5.Lcd.println(" seconds");
  M5.Lcd.setCursor(10, 160);
  M5.Lcd.println("Press Red to Play Again");
}

/**
 * @brief 指定されたディレクトリ内のJPGファイルを探し、リストに追加する
 * @param fs ファイルシステム
 * @param dirname ディレクトリ名
 */
void listImageFiles(fs::FS &fs, const char *dirname) {
  File root = fs.open(dirname);
  if (!root) {
    M5.Lcd.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    M5.Lcd.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (!file.isDirectory()) {
      String fileName = String(file.name());
      if (fileName.endsWith(".jpg") || fileName.endsWith(".JPG")) {
        imageFiles.push_back("/" + fileName);
      }
    }
    file = root.openNextFile();
  }
  root.close();
}

/**
 * @brief 指定されたパスの画像ファイルを表示する
 * @param path 表示する画像のパス
 */
void displayImage(const String &path) {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.drawJpgFile(SD, path.c_str(), 0, 24);
}

/**
 * @brief 次の画像に切り替えて表示する
 */
void showNextImage() {
  imagesDisplayedCount++;
  if (imagesDisplayedCount >= imageFiles.size()) {
    unsigned long endTime = millis();
    float playTime = (float)(endTime - startTime) / 1000.0;
    M5.Speaker.mute();
    displayEndScreen(playTime);
    isGameOver = true;
  } else {
    currentImageIndex++;
    displayImage(imageFiles[currentImageIndex]);
  }
}

/**
 * @brief ゲームの状態をリセットして再開する
 */
void resetGame() {
  isGameStarted = false;
  isGameOver = false;
  currentImageIndex = 0;
  imagesDisplayedCount = 0;
  startTime = 0;
  imageFiles.clear();
  displayStartScreen();
}
