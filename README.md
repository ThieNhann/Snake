# Đồ án: Hướng Đối Tượng Hóa Trò Chơi Rắn Săn Mồi

##  Nhóm thực hiện

| Họ tên               | MSSV      | Vai trò      |
|----------------------|-----------|---------------|
| Nguyễn Thiện Nhân    | 23120064  | Nhóm trưởng   |
| Hà Quốc Kiên         | 23120056  | Thành viên    |
| Nguyễn Thanh Khánh Hà| 23120037  | Thành viên    |


---
## Phân chia công việc
### Nguyễn Thiện Nhân - 23120064		(30%)
- Thiết kế sơ bộ các class và class diagram
- Thiết kế lớp `Snake` để quản lý thân rắn và các thuộc tính liên quan.
- Cài đặt logic điều khiển di chuyển từ bàn phím.
- Xử lý va chạm.

### Hà Quốc Kiên - 23120056 		(40%)
- Thiết kế lớp `Fruit`  (bên trong Object Manager) để quản lý vị trí và trạng thái của thức ăn.
- Cài đặt logic sinh ngẫu nhiên vị trí mới cho thức ăn sau khi bị ăn.
- Quản lý điểm số và giao diện hiển thị điểm cho người chơi.

### Nguyễn Thanh Khánh Hà - 23120037		(30%)
- Thiết kế lớp `Game` làm lớp điều phối chính, xử lý vòng lặp trò chơi và cập nhật trạng thái liên tục.
- Xử lý logic kết thúc trò chơi khi có va chạm hoặc các điều kiện thua.
- Tích hợp các lớp `Snake`, `Food` và quản lý toàn bộ luồng hoạt động của game.

---

## Đề xuất tỉ lệ điểm chia đều.

## Mô tả.

### 1. **Teamwork**

Họp ngày 28-04-2025: [bien_ban_hop_28_04](https://studenthcmusedu-my.sharepoint.com/:b:/g/personal/23120064_student_hcmus_edu_vn/EVVYrUSPU2tHtAqOAUfjMlYBV7mIAG0wskN9A1JZii524Q?e=rDHNL6).
[git log](https://studenthcmusedu-my.sharepoint.com/:i:/g/personal/23120064_student_hcmus_edu_vn/Eb5yDNz-Si5Iv6vKgEifw0gBWdoN3fld0sbwLKQbNxU4DA?e=mFL29f)
### 2. **UI/UX**

-   Cài đặt lại chức năng của mã nguồn mẫu theo hướng đối tượng: **Hoàn thành**
-   Các chức năng thêm và cải tiến: **Chưa hoàn thành tất cả**
-   Giao diện bố trí hợp lí: **Ổn**
---


## 3. Kiến trúc phần mềm + design pattern
#### Kiến trúc Model - View - Controller (có sửa đổi)
-   **Model**: 
    -   Lớp `Snake`, `Fruit`, `GameObjects`, `Settings` là phần mô hình dữ liệu và logic trò chơi.
    -   Lưu trữ trạng thái game (score, snake body, fruit position, v.v...).
-   **View**:
    -   Lớp `UI` chịu trách nhiệm hiển thị UI tương ứng với từng trạng thái `GameState`.
    -   Các sprite và background được load và render tương ứng với từng trạng thái.
-   **Controller (Game)**:
    -   Lớp `Game` điều phối luồng hoạt động: input → xử lý → render.
    -   Điều phối trạng thái (`GameState`), xử lý input, lưu/khôi phục session...

#### Encapsulation

-   Các thành phần như `Snake`, `Fruit`, `Settings`, `UI` là các lớp riêng biệt.
-   Dữ liệu được ẩn và chỉ truy cập thông qua các phương thức (`snake.getBody()`, `setting.save()`, ...).
    

####  Abstraction
-   Các lớp được thiết kế để giấu chi tiết cụ thể bên trong (như cách `UI` vẽ nút, hoặc `snake.move()` hoạt động ra sao không cần biết trong `Game`).

#### Single Responsibility Principle

-   Mỗi lớp đảm nhiệm một vai trò riêng:
    -   `Game`: điều phối trạng thái, logic chính.
    -   `UI`: vẽ giao diện người dùng.
    -   `Settings`: quản lý cấu hình.
    -   `Snake`, `Fruit`: logic thực thể game.
        

####  Open/Closed Principle
-   Code mở rộng được nhưng hạn chế chỉnh sửa.  
    Ví dụ: thêm state `SETTINGS_SOUND` hoặc `SETTINGS_RESOLUTION` chỉ cần thêm enum, 1 case render, không thay đổi logic cũ.


#### State Pattern:  Quản lý trạng thái game (MENU,  PLAYING,  PAUSED, v.v.). 
#### Singleton:  Quản lý cài đặt toàn cục (`Settings`).
#### Observer:  Theo dõi và cập nhật trạng thái UI (điểm số, trạng thái game).

## Đảm bảo chất lượng

[coding_convention](https://studenthcmusedu-my.sharepoint.com/:t:/g/personal/23120064_student_hcmus_edu_vn/EZ50s31JiBxJvcZ-5Tf6_r4BfEcvtKcYi-7UYWMwpxS_6g?e=H3eFTs)

---

## Tài liệu mô tả kiến trúc phần mềm và các module:
[Kien_Truc_Phan_Mem](https://studenthcmusedu-my.sharepoint.com/:t:/g/personal/23120064_student_hcmus_edu_vn/EfWT-Dt0K7dAkRIenLFUwZABEELvhgX0nWebudY-ucgRmg?e=9dwham)

## Demo:
[Demo](https://youtu.be/OtVR6b829zA)


