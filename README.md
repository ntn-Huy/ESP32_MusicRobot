# 🤖 ESP32 Smart Music Robot

Một dự án Robot phát nhạc thông minh sử dụng vi điều khiển ESP32. Robot có khả năng kết nối Wi-Fi, stream nhạc trực tiếp từ một Local HTTP Server và hiển thị khuôn mặt cảm xúc sinh động trên màn hình TFT (chuyển động miệng đồng bộ với thời gian thực).

---

## 🎥 Video Demo

> **Lưu ý:** Bấm vào hình ảnh bên dưới để xem video demo thực tế của Robot trên YouTube. Cập nhật link video của bạn vào phần `YOUR_VIDEO_ID`.

[![Robot Demo Video](https://img.youtube.com/vi/YOUR_VIDEO_ID/maxresdefault.jpg)](https://www.youtube.com/watch?v=YOUR_VIDEO_ID)

---

## 🌟 Tính năng nổi bật

- **Kết nối không dây:** Tự động kết nối Wi-Fi để nhận dữ liệu.
- **Audio Streaming:** Stream và phát nhạc định dạng MP3 trực tiếp từ HTTP Server cục bộ mà không cần lưu trữ thẻ nhớ.
- **Âm thanh chất lượng cao:** Sử dụng giao thức I2S kết hợp với mạch DAC để xuất âm thanh rõ nét.
- **Giao diện sinh động (UI):** Hiển thị khuôn mặt Robot trên màn hình TFT. Miệng Robot có hiệu ứng nhấp nháy/chuyển động khi đang phát nhạc và tự động đóng lại khi bài hát kết thúc.
- **Tối ưu hiển thị:** Thuật toán cập nhật UI cục bộ không gây giật lag (chống block vòng lặp giải mã âm thanh).

---

## 🛠️ Linh kiện cần thiết

| STT | Tên linh kiện | Số lượng | Ghi chú |
|:---:|:---|:---:|:---|
| 1 | **ESP32 Development Board** | 1 | NodeMCU ESP32 (38 pin hoặc 30 pin) |
| 2 | **Màn hình TFT LCD** | 1 | Sử dụng giao tiếp SPI (VD: TFT 2.4", 2.8") |
| 3 | **Module I2S DAC Audio** | 1 | Khuyên dùng mạch **MAX98357A** hoặc PCM5102 |
| 4 | **Loa mini** | 1 | 3W - 4Ω hoặc tương đương (phù hợp với mạch DAC) |
| 5 | **Nguồn & Phụ kiện** | 1 | Dây cắm breadboard, cáp kết nối |

---

## 🔌 Sơ đồ kết nối (Wiring Diagram)

### 1. Kết nối ESP32 với Module âm thanh I2S (MAX98357A)
Dựa theo cấu hình `out->SetPinout(26, 25, 27);` trong mã nguồn:

| Module MAX98357A | Chân trên ESP32 | Chức năng |
| :---: | :---: | :--- |
| **BCLK** | GPIO 26 | Bit Clock |
| **LRC** | GPIO 25 | Left/Right Clock (Word Select) |
| **DIN** | GPIO 27 | Data Input |
| **VCC** | 5V / 3.3V | Cấp nguồn |
| **GND** | GND | Nối mass |
| **Loa + / -** | Nối ra Loa | Xuất âm thanh |

### 2. Kết nối ESP32 với Màn hình TFT (SPI)
*(Lưu ý: Các chân này cần được thiết lập trong file `User_Setup.h` của thư viện `TFT_eSPI`. Dưới đây là cấu hình tham khảo phổ biến)*

| Màn hình TFT | Chân trên ESP32 | Chức năng |
| :---: | :---: | :--- |
| **VCC** | 3.3V | Cấp nguồn 3.3V |
| **GND** | GND | Nối mass |
| **CS** | GPIO 15 | Chip Select |
| **RESET** | GPIO 4 | Reset |
| **DC / RS** | GPIO 2 | Data / Command |
| **SDI / MOSI** | GPIO 23 | Master Out Slave In |
| **SCK** | GPIO 18 | Serial Clock |
| **LED / BLK**| 3.3V | Bật đèn nền |

---

## 📚 Thư viện sử dụng

Để biên dịch thành công dự án, hãy cài đặt các thư viện sau thông qua Library Manager của Arduino IDE:

1. **TFT_eSPI** (by Bodmer): Điều khiển và hiển thị giao diện lên màn hình TFT.
2. **ESP8266Audio** (by earlephilhower): Xử lý HTTP stream, giải mã âm thanh MP3 và xuất tín hiệu qua giao thức I2S.

---

## 🚀 Hướng dẫn cài đặt và chạy dự án

### Bước 1: Chuẩn bị môi trường
1. Cài đặt các thư viện yêu cầu trong Arduino IDE.
2. Cấu hình file `User_Setup.h` trong thư mục thư viện `TFT_eSPI` để đảm bảo Arduino IDE nhận đúng loại màn hình và các chân kết nối SPI tương ứng với phần cứng của bạn.

### Bước 2: Thiết lập Local HTTP Server
1. Đảm bảo ESP32 và máy chủ chứa nhạc (máy tính hoặc điện thoại) **kết nối chung một mạng Wi-Fi**.
2. Khởi tạo một HTTP Server cục bộ (VD: Live Server trên VSCode, Python HTTP server, hoặc app trên điện thoại).
3. Đặt file nhạc `Thiên Lý Ơi (HITStory Live Version).mp3` vào thư mục gốc của Server.
4. Mở file `.ino` và cập nhật cấu hình mạng:
   ```cpp
   const char* ssid = "Tên_WiFi_Của_Bạn";
   const char* password = "Mật_khẩu_WiFi";
   ```
5. Cập nhật IP tĩnh và port của máy chủ vào đường dẫn nhạc:
   ```cpp
   const char* URL_NHAC = "http://<IP_MÁY_CHỦ>:<PORT>/Thi%C3%AAn%20L%C3%BD%20%C6%A0i%20(HITStory%20Live%20Version).mp3"; 
   ```

### Bước 3: Nạp Code
1. Cắm ESP32 vào máy tính.
2. Chọn đúng board (VD: ESP32 Dev Module) và cổng COM.
3. Nhấn **Upload**. Bật Serial Monitor (Baudrate: `115200`) để theo dõi quá trình kết nối Wi-Fi và load nhạc.

---

## 🧠 Cấu trúc hoạt động của Code (Logic Flow)

1. **Khởi tạo (`setup`):** 
   - Vẽ giao diện khuôn mặt khởi động lên TFT.
   - Kết nối vào mạng Wi-Fi được chỉ định.
   - Khởi tạo luồng đọc HTTP (`AudioFileSourceHTTPStream`).
   - Cấu hình chân I2S cho DAC (`AudioOutputI2S`) và gắn vào bộ giải mã (`AudioGeneratorMP3`).
2. **Vòng lặp chính (`loop`):**
   - Liên tục nạp dữ liệu nhạc qua hàm `mp3->loop()`.
   - Sử dụng `millis()` (non-blocking delay) mỗi 150ms để cập nhật trạng thái đóng/mở miệng Robot.
   - Chỉ vẽ lại vùng miệng nhỏ bằng `tft.fillRect()` và `tft.drawLine()` để tránh hiện tượng màn hình nhấp nháy toàn bộ (flicker) và giữ cho luồng stream nhạc luôn mượt mà.
