# Đồ án: Hướng Đối Tượng Hóa Trò Chơi Rắn Săn Mồi

## Nhóm thực hiện

| Họ tên               | MSSV      | Vai trò      |
|----------------------|-----------|--------------|
| Nguyễn Thiện Nhân    | 23120064  | Nhóm trưởng  |
| Hà Quốc Kiên         | 23120056  | Thành viên   |
| Nguyễn Thanh Khánh Hà| 23120037  | Thành viên   |

---

## Phân chia công việc
### Nguyễn Thiện Nhân - 23120064 (35%)
- Thiết kế sơ bộ các class, class diagram tổng thể, xác định kiến trúc OOP và các pattern sử dụng.
- Thiết kế và cài đặt lớp `Snake` (quản lý thân rắn, di chuyển, va chạm, logic điều khiển từ bàn phím).
- Tham gia xây dựng `GameStateManager` (quản lý trạng thái game).
- Tham gia xây dựng logic lưu/khôi phục session, high score.
- Viết tài liệu kiến trúc phần mềm, hướng dẫn sử dụng, báo cáo.

### Hà Quốc Kiên - 23120056 (35%)
- Thiết kế và cài đặt các lớp `Fruit`, `Bomb`, `SuperFruit` (trong ObjectManager).
- Xây dựng toàn bộ logic spawn, collision, quản lý điểm số, hiệu ứng động cho object.
- Xây dựng và hoàn thiện `ObjectManager` quản lý các đối tượng động.
- Tham gia xây dựng UI (hiển thị điểm số, hiệu ứng, layout).
- Tham gia kiểm thử, tối ưu code, đóng góp ý tưởng UI/UX.

### Nguyễn Thanh Khánh Hà - 23120037 (30%)
- Thiết kế và cài đặt lớp `Game` (điều phối chính, vòng lặp game, cập nhật trạng thái, render).
- Tích hợp các module: Snake, ObjectManager, UI, Settings, SaveManager.
- Xử lý logic kết thúc game, pause, resume, chuyển đổi trạng thái, xác nhận (confirmation).
- Xây dựng và hoàn thiện các chức năng settings (âm lượng, tốc độ, độ phân giải).
- Tham gia kiểm thử, đóng góp tài liệu, hướng dẫn sử dụng.

###
- [Commit GitHub](https://github.com/ThieNhann/Snake/commits/main/)
- [git_logs](https://studenthcmusedu-my.sharepoint.com/:f:/g/personal/23120064_student_hcmus_edu_vn/Ei3VfumFN3JGm2Byd26J_YgBBVx_q8UWndRi93V6aIp8Zw?e=sFZUmg)
- Theo dõi tiến độ và thảo luận hầu hết đều ở nhóm chat Zalo.
---

## Đề xuất tỉ lệ điểm chia đều.

---

## 1. Tổng quan & Mục tiêu

Đồ án hoàn thiện trò chơi Snake với kiến trúc hướng đối tượng (OOP), tách biệt rõ ràng các module, áp dụng các nguyên lý SOLID, design pattern phổ biến (MVC, State, Singleton, Observer).

---

## 2. Kiến trúc phần mềm & OOP Design

### 2.1. Tổng quan kiến trúc

- **Model - View - Controller (MVC)**
    - **Model**: `Snake`, `Fruit`, `Bomb`, `SuperFruit`, `Settings`, `ObjectManager`
    - **View**: `UI` (vẽ giao diện, nút, HUD, hiệu ứng)
    - **Controller**: `Game` (điều phối trạng thái, xử lý input, vòng lặp game)

- **State Pattern**: Quản lý trạng thái game (`GameStateManager`), chuyển đổi giữa MENU, PLAYING, PAUSED, GAME_OVER, SETTINGS, CONFIRMATION, v.v.

- **Singleton**: Lớp `Settings` (biến toàn cục `setting`) đảm bảo cấu hình nhất quán toàn chương trình.

- **Observer**: UI cập nhật theo trạng thái game, điểm số, high score, v.v.

### 2.2. OOP Principles & SOLID

- **Encapsulation**: 
    - Dữ liệu và logic của từng thực thể (Snake, Fruit, Bomb, SuperFruit) được đóng gói trong class riêng, chỉ truy cập qua public method.
    - Ví dụ: `Snake::move()`, `ObjectManager::spawnFruit()`, `Settings::saveToFile()`.

- **Abstraction**:
    - Ẩn chi tiết cài đặt, chỉ cung cấp interface cần thiết. Ví dụ: `UI` chỉ cung cấp các hàm vẽ, không lộ chi tiết layout.
    - `ObjectManager` quản lý logic spawn/collision mà không lộ chi tiết ra ngoài.

- **Single Responsibility Principle**:
    - Mỗi class chỉ đảm nhiệm một vai trò:
        - `Snake`: quản lý thân rắn, di chuyển, va chạm
        - `ObjectManager`: quản lý các đối tượng động (fruit, bomb, superfruit)
        - `UI`: vẽ giao diện, hiệu ứng, layout
        - `Game`: điều phối trạng thái, vòng lặp, input, lưu/khôi phục session
        - `Settings`: quản lý cấu hình, lưu/đọc file
        - `SaveManager`: quản lý lưu/đọc session

- **Open/Closed Principle**:
    - Dễ dàng mở rộng thêm trạng thái, chức năng mới (ví dụ: thêm mode, thêm object, thêm menu settings) mà không sửa code cũ.
    - Ví dụ: Thêm `SETTINGS_SPEED` chỉ cần thêm enum, 1 case render, không ảnh hưởng logic cũ.


- **Dependency Inversion**:
    - Các module cấp cao (Game) không phụ thuộc trực tiếp vào chi tiết module cấp thấp (UI, ObjectManager), mà thông qua interface rõ ràng.

---

## 3. Design Pattern thực hiện

- **MVC**: Tách biệt rõ Model (logic/game data), View (UI), Controller (Game).
- **State Pattern**: `GameStateManager` quản lý trạng thái, chuyển đổi linh hoạt.
- **Singleton**: `Settings` (biến setting toàn cục).
- **Observer**: UI cập nhật theo trạng thái, điểm số, high score.
- **Factory (simple)**: `ObjectManager` sinh các object động (fruit, bomb, superfruit) theo điều kiện.

---

## 4. Chức năng & UI/UX

- Giao diện rõ ràng, responsive, tự động scale theo độ phân giải.
- Menu, pause, game over, settings, confirmation box, hiệu ứng hover, slider âm lượng, chọn tốc độ rắn, chọn kích thước màn chơi.
- Lưu/khôi phục session, high score, settings.
- Hiệu ứng động: flicker, pulse, hover, v.v.

---

## 5. Đảm bảo chất lượng

- Code tuân thủ coding convention, tách file rõ ràng, dễ bảo trì.
- Đã kiểm thử các case: chơi mới, pause, resume, game over, đổi resolution, đổi tốc độ, lưu/khôi phục session, v.v.
- Dễ mở rộng thêm mode, object, UI, logic mới.

---

## 6. Tài liệu & Demo
- [Repo GitHub](https://github.com/ThieNhann/Snake)
- [coding_convention](https://studenthcmusedu-my.sharepoint.com/:t:/g/personal/23120064_student_hcmus_edu_vn/EZ50s31JiBxJvcZ-5Tf6_r4BfEcvtKcYi-7UYWMwpxS_6g?e=H3eFTs)
- [Demo](https://youtu.be/hlZZ63k_C3s)
- [Software_Architecture](https://studenthcmusedu-my.sharepoint.com/:t:/g/personal/23120064_student_hcmus_edu_vn/ES-XdWo-wmJHltEqITrLoPkBCKBwncY23CvA8FOIGeu6Uw?e=gikSAI)

---


