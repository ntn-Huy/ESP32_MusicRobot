#include <Arduino.h>
#include <WiFi.h>
#include <TFT_eSPI.h>
#include "AudioFileSourceHTTPStream.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

// --- THÔNG TIN WI-FI CỦA BẠN ---
const char* ssid = "Bame";
const char* password = "31012006";

// --- LINK NHẠC CỦA BẠN (Phải là link http://) ---
const char* URL_NHAC = "http://192.168.31.253:8080/Thi%C3%AAn%20L%C3%BD%20%C6%A0i%20(HITStory%20Live%20Version).mp3"; 

TFT_eSPI tft = TFT_eSPI();
AudioGeneratorMP3 *mp3;
AudioFileSourceHTTPStream *file;
AudioOutputI2S *out;

bool mouthOpen = false;
unsigned long lastUIUpdate = 0;

// Hàm vẽ giao diện đơn giản
void drawFace(bool speaking, String message = "") {
  tft.fillScreen(TFT_BLACK);
  
  // Vẽ 2 con mắt
  tft.fillCircle(80, 100, 20, TFT_WHITE);
  tft.fillCircle(160, 100, 20, TFT_WHITE);
  
  // In thông báo (nếu có)
  if (message != "") {
    tft.setTextColor(TFT_YELLOW);
    tft.setTextDatum(MC_DATUM);
    tft.drawString(message, tft.width() / 2, 40, 4);
  }

  // Vẽ miệng
  if (speaking) {
    tft.fillRoundRect(90, 150, 60, 30, 10, TFT_RED); 
  } else {
    tft.drawLine(90, 165, 150, 165, TFT_WHITE);
  }
}

void setup() {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(1); // Xoay ngang màn hình (Đổi 0, 1, 2, 3 nếu bị ngược)

  // Kết nối Wi-Fi
  drawFace(false, "Dang ket noi Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Bắt đầu tải nhạc
  drawFace(true, "Dang tai nhac...");
  
  file = new AudioFileSourceHTTPStream(URL_NHAC);
  out = new AudioOutputI2S();
  out->SetPinout(26, 25, 27); // BCLK, LRC, DIN
  
  mp3 = new AudioGeneratorMP3();
  mp3->begin(file, out);

  // Xóa chữ "Đang tải nhạc" đi, chỉ để lại mặt robot
  drawFace(true); 
}

void loop() {
  if (mp3 && mp3->isRunning()) {
    if (!mp3->loop()) {
      mp3->stop();
      drawFace(false, "Da phat xong!");
    } else {
      // Nhấp nháy miệng mỗi 150ms để chống giật nhạc
      if (millis() - lastUIUpdate > 150) {
        lastUIUpdate = millis();
        mouthOpen = !mouthOpen;
        
        // Chỉ vẽ lại vùng miệng để màn hình không bị chớp nháy
        if (mouthOpen) {
          tft.fillRoundRect(90, 150, 60, 30, 10, TFT_RED);
        } else {
          tft.fillRect(90, 150, 60, 30, TFT_BLACK); // Xóa miệng đỏ
          tft.drawLine(90, 165, 150, 165, TFT_WHITE); // Vẽ vạch ngang
        }
      }
    }
  }
}