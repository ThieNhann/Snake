# Đồ án: Hướng Đối Tượng Hóa Trò Chơi Rắn Săn Mồi

##  Nhóm thực hiện

| Họ tên               | MSSV      | Vai trò      |
|----------------------|-----------|---------------|
| Nguyễn Thiện Nhân    | 23120064  | Nhóm trưởng   |
| Hà Quốc Kiên         | 23120056  | Thành viên    |
| Nguyễn Thanh Khánh Hà| 23120037  | Thành viên    |


---

## Mục tiêu:

Dựa trên mã nguồn ban đầu (được cung cấp dưới dạng 1 file `main.cpp`), nhóm sẽ:
- Phân tích và refactor chương trình theo mô hình **hướng đối tượng (OOP)**
- Áp dụng các nguyên lý **SOLID**
- Thiết kế lại theo **các mẫu thiết kế phần mềm (Design Patterns)**
- Tổ chức theo **Kiến trúc phân tầng (Layered Architecture)**
- Áp dụng **dependency injection** để tăng khả năng mở rộng và kiểm thử

---

## Đề xuất thực hiện

### 1. **Phân tích mã nguồn ban đầu**

- Xác định các thực thể chính: đối tượng game, vòng lặp game, xử lý va chạm, vẽ đồ họa, v.v.
- Tách các logic thành module/phân lớp rõ ràng

---

### 2. **Tái cấu trúc hướng đối tượng**

- Tách `main.cpp` thành nhiều lớp:
  - `Game`: điều phối chính
  - `Player`, `Food`, `Obstacle`: các entity
  - `Renderer`, `InputHandler`, `PhysicsManager`: các module phụ trợ

---

### 3. **Áp dụng các mẫu thiết kế và kiến trúc phần mềm**

| Pattern | Dự kiến sử dụng | Mục đích |
|--------|------------------|---------|
| Singleton | Game Engine / Resource Manager | Đảm bảo dùng 1 instance của các thành phần toàn cục |
| Factory Method | Khởi tạo các Entity | Tạo object linh hoạt |
| Observer | Quản lý sự kiện | Theo dõi các trạng thái (điểm số, cấp độ, ...) |
| Strategy | Xử lý input, logic game | Dễ thay đổi thuật toán, chiến thuật |
| State | Quản lý trạng thái Game | Menu / Playing / Paused / GameOver |

- Sử dụng kiến trúc **Layered Architecture** (tầng ứng dụng, tầng hiển thị, tầng logic)
- Quản lý bằng **namespace** hoặc **module**

---


## Công cụ & công nghệ

- **Ngôn ngữ**: C++
- **Thư viện đồ họa**: SFML
- **IDE**: Visual Studio Code 
- **Quản lý mã nguồn**: Git (Feature Branch Workflow)
- **Vẽ sơ đồ**: Draw.io

---

## Tài liệu sẽ cung cấp

-  README.md (file này)
- `coding_convention.md`: Quy tắc viết mã của nhóm
-  `class_diagram.png`: Sơ đồ lớp
-  `class_description.md`: Mô tả chi tiết các lớp và chức năng

---

## Liên hệ

Mọi phản hồi hoặc câu hỏi xin liên hệ nhóm trưởng:  
**Nguyễn Thiện Nhân – 23120064**  
Email: *23120064@student.hcmus.edu.vn*

## Lời nhắn
*Các kế hoạch và cấu trúc đã trình bày ở trên có thể được điều chỉnh trong quá trình phát triển, tùy thuộc vào tiến độ thực tế, khó khăn kỹ thuật hoặc các phát sinh khác.*  
*Nhóm rất mong nhận được sự thông cảm và góp ý từ giảng viên để có thể hoàn thiện đồ án một cách tốt nhất.*

