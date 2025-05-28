# Tài liệu kiến trúc phần mềm & mô tả các module

## 1. Kiến trúc tổng thể

- **Mô hình áp dụng:** Model - View - Controller (MVC)
- **Các design pattern chính:** State, Singleton, Observer, Factory (simple)

### Sơ đồ tổng quan (mô tả logic)
```
+-------------------+      +-------------------+      +-------------------+
|      Model        |<---->|    Controller     |<---->|       View        |
|-------------------|      |-------------------|      |-------------------|
| Snake             |      | Game              |      | UI                |
| ObjectManager     |      | GameStateManager  |      | (draw, layout)    |
| Settings          |      |                   |      |                   |
| SaveManager       |      |                   |      |                   |
+-------------------+      +-------------------+      +-------------------+
```

---

## 2. Mô tả các module

### 2.1. Snake
- **Chức năng:** Quản lý thân rắn (vector SnakeSegment), di chuyển, tăng trưởng, kiểm tra va chạm bản thân, va chạm biên, lưu/khôi phục trạng thái.
- **OOP:** Đóng gói dữ liệu, chỉ truy cập qua các phương thức public (`move()`, `grow()`, `checkSelfCollision()`, ...).
- **Mở rộng:** Có thể thêm các thuộc tính mới (ví dụ: skin, hiệu ứng) mà không ảnh hưởng module khác.

### 2.2. ObjectManager
- **Chức năng:** Quản lý toàn bộ object động trên bản đồ: Fruit, Bomb, SuperFruit.
- **Nhiệm vụ:** Sinh mới, kiểm tra va chạm, cập nhật trạng thái, lưu/khôi phục trạng thái từng object.
- **Pattern:** Factory (simple) cho việc sinh object động.
- **Mở rộng:** Dễ thêm object mới (ví dụ: PowerUp, Portal).

### 2.3. UI
- **Chức năng:** Vẽ toàn bộ giao diện (menu, HUD, nút, hiệu ứng, slider, confirmation box, ...).
- **OOP:** Đóng gói layout, hiệu ứng hover, cập nhật giao diện theo trạng thái game.
- **Pattern:** Observer (UI cập nhật khi trạng thái game thay đổi).
- **Mở rộng:** Dễ thêm màn hình mới, hiệu ứng mới.

### 2.4. Game
- **Chức năng:** Điều phối vòng lặp chính, xử lý input, cập nhật trạng thái, tích hợp các module, lưu/khôi phục session.
- **OOP:** Tách biệt rõ logic điều phối với logic hiển thị và dữ liệu.
- **Pattern:** Controller trong MVC.

### 2.5. Settings
- **Chức năng:** Quản lý cấu hình (độ phân giải, âm lượng, tốc độ rắn), lưu/đọc file.
- **Pattern:** Singleton (biến setting toàn cục).
- **Mở rộng:** Dễ thêm các tuỳ chọn cấu hình mới.

### 2.6. SaveManager
- **Chức năng:** Lưu/đọc session, high score, validate file.
- **OOP:** Đóng gói toàn bộ logic file I/O, không lộ chi tiết ra ngoài.

### 2.7. GameStateManager
- **Chức năng:** Quản lý trạng thái game (MENU, PLAYING, PAUSED, GAME_OVER, SETTINGS, CONFIRMATION, ...), mode chơi, xác nhận thao tác.
- **Pattern:** State pattern.
- **Mở rộng:** Dễ thêm trạng thái mới.

---

## 3. Luồng hoạt động chính

1. **Khởi động:**  
   - Settings được load từ file (Singleton).
   - GameStateManager khởi tạo trạng thái (State pattern).
   - Game tạo các module, UI khởi tạo layout.

2. **Vòng lặp chính:**  
   - Game nhận input, cập nhật logic, gọi UI render theo trạng thái.
   - Khi cần lưu/khôi phục session, gọi SaveManager.

3. **Tương tác giữa các module:**  
   - Game điều phối, không truy cập trực tiếp vào chi tiết của Snake, ObjectManager, UI.
   - UI chỉ nhận dữ liệu cần thiết để vẽ, không xử lý logic game.

---

## 4. Ưu điểm kiến trúc

- **Tách biệt rõ ràng:** Dễ bảo trì, dễ mở rộng, dễ kiểm thử từng module.
- **Tuân thủ SOLID:** Mỗi class/module chỉ đảm nhiệm một vai trò, dễ mở rộng mà không ảnh hưởng code cũ.
- **Dễ mở rộng:** Thêm object, mode, UI, settings, trạng thái mới mà không ảnh hưởng các module khác.
- **Dễ tích hợp tính năng nâng cao:** (ví dụ: multiplayer, AI, skin, leaderboard).

---

## 5. Tham khảo

- [Repo GitHub](https://github.com/ThieNhann/Snake)

---
