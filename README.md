# Đồ án Game Rắn Săn Mồi (Snake) - SS004.10

Đây là dự án cuối kỳ môn học SS004.10, xây dựng lại trò chơi Rắn Săn Mồi (Snake) kinh điển trên nền tảng Windows Console bằng ngôn ngữ C++.

Trò chơi tái hiện lại lối chơi đơn giản nhưng đầy cuốn hút, mang đến những giờ phút giải trí đầy thử thách với đồ họa pixel art, hệ thống điểm cao, và các cấp độ khó khác nhau.



## 👥 Thành viên nhóm 12

Dự án được thực hiện bởi nhóm sinh viên từ Trường Đại học Công nghệ Thông tin - ĐHQG-HCM.

| Vai trò | Họ & Tên | MSSV |
| :--- | :--- | :--- |
| **Trưởng nhóm** | Huỳnh Công Bằng | 22730067 |
| **Thành viên** | Phạm Trần Khánh Vũ | 24730246 |
| **Thành viên** | Phạm Hoàng Tú | 25730086 |
| **Thành viên** | Nguyễn Hiếu Cảnh | 25730015 |
| **Thành viên** | Hồ Quyết Tùng | 25730088 |

## ✨ Tính năng nổi bật

* **Lối chơi cổ điển:** Rắn di chuyển 4 hướng (lên, xuống, trái, phải), ăn mồi để dài ra và tăng điểm.
* **Hệ thống điểm cao:** Lưu lại 5 người chơi có điểm số cao nhất.
* **Nhiều độ khó:** Cho phép người chơi lựa chọn các cấp độ Dễ, Vừa, Khó.
* **Điều kiện thua:** Trò chơi kết thúc khi rắn tự cắn vào thân hoặc đâm vào tường.
* **Chơi lại tiện lợi:** Dễ dàng bắt đầu ván mới ngay từ menu mà không cần khởi động lại game.
* **Giao diện Menu đầy đủ:** Bao gồm các mục Bắt đầu, Điểm cao, Độ khó, Giới thiệu và Thoát.

## 🛠️ Cài đặt & Sử dụng

### Yêu cầu kỹ thuật
* **Nền tảng:** Windows
* **Ngôn ngữ:** C++
* **Trình biên dịch:** GCC

### Hướng dẫn biên dịch
Để đảm bảo tính nhất quán, dự án được biên dịch bằng GCC trên Terminal. Mở terminal trong thư mục gốc của dự án và chạy lệnh sau:

```bash
g++ Food.cpp Game.cpp Renderer.cpp Snake.cpp main.cpp -o main
```

Lệnh này sẽ tạo ra một file thực thi là `main.exe`.

### Cách chơi
* Chạy file `main.exe` để bắt đầu trò chơi.
* **Trong Menu:** Sử dụng phím `↑` `↓` để di chuyển và `Enter` để chọn.
* **Trong Game:**
    * Sử dụng `↑` `↓` `←` `→` hoặc `W` `A` `S` `D` để điều khiển rắn.
    * Nhấn `ESC` để tạm dừng/tiếp tục trò chơi.

## 🏗️ Xây dựng với

Dự án sử dụng các thư viện C++ tiêu chuẩn và thư viện của Windows để thao tác với Console.

* **Thư viện chuẩn:** `iostream`, `vector`, `string`, `deque`, `chrono`, `random`, `algorithm`, `sstream`, `fstream`.
* **Thư viện hệ thống:**
    * `windows.h`: Để thao tác nâng cao và tối ưu hóa hiệu năng cho console.
    * `conio.h`: Để bắt sự kiện nhấn phím từ người dùng.

---
*Báo cáo được hoàn thành vào ngày 8 tháng 9 năm 2025.*
