
# Coding Convetion

Tài liệu này quy định phong cách viết mã C++ cho đồ án nhóm, giúp đồng nhất mã nguồn, dễ bảo trì và dễ đọc.

## 1. Quy tắc đặt tên

| Thành phần              | Kiểu chữ     | Ví dụ                          |
|-------------------------|--------------|--------------------------------|
| Biến thường             | camelCase    | userName, totalScore           |
| Hằng số (constant)      | UPPER_SNAKE  | MAX_USER, PI_VALUE             |
| Hàm                     | camelCase()  | getData(), calculateArea()     |
| Class / Struct          | PascalCase   | User, Rectangle                |
| File .cpp/.h            | snake_case   | user.cpp, math_utils.hpp       |
| Thành phần trong class  | _camelCase   | _id, _userName                 |

## 2. Quy tắc viết hàm

- Mỗi hàm nên ngắn gọn (≤ 30 dòng), chỉ làm một việc.
- Có mô tả đầu hàm nếu cần.

```cpp
// Tính diện tích hình tròn
double calculateCircleArea(double r) {
    return 3.14 * r * r;
}
```

## 3. Comment
- Comment rõ ràng cho các thuật toán phức tạp.
- Dùng `//` cho comment ngắn, `/* */` cho block lớn.
- Viết tiếng Việt có dấu hoặc tiếng Anh rõ ràng.

## 4. Quy ước khác

- Dùng chuẩn C++11 trở lên.
- Tránh `using namespace std;` trong header.
- Dùng `const` và `&` cho tham số không thay đổi:

```cpp
void printName(const std::string& name);
```

- Kiểm tra null, tránh crash:

```cpp
if (ptr != nullptr) {
    ptr->doSomething();
}
```

## 5. Làm việc nhóm

- Mỗi người phụ trách một module.
- Sử dụng Git, tuân thủ feature branch workflow.
- Commit rõ ràng, không đẩy code chưa chạy.
- Review kỹ trước khi merge vào main.

## 6. Tài nguyên tham khảo

- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
