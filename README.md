# Graduation Research 1 - HESPI K64

## Nhiệm vụ

Sử dụng ngôn ngữ lập trình C để cài đặt cấu trúc dữ liệu **đơn đồ thị vô hướng** `G = (V,E)`

## Chi tiết

- Sinh viên lập trình đọc tệp tin văn bản từ thư viện đồ thị [SNAP](http://snap.stanford.edu/data/index.html) với các đồ thị lớp lớn

| Name       | Type       | Nodes     | Edges     | Description                  |
| ---------- | ---------- | --------- | --------- | ---------------------------- |
| roadNet-CA | Undirected | 1,965,206 | 2,766,607 | Road network of California   |
| roadNet-PA | Undirected | 1,088,092 | 1,541,898 | Road network of Pennsylvania |
| roadNet-TX | Undirected | 1,379,917 | 1,921,660 | Road network of Texas        |

- Sinh viên thực hiện được các thao tác cơ bản

  - Duyệt đồ thị theo chiều rộng (BFS) + Duyệt đồ thị theo chiều sâu (DFS)
  - Đếm số thành phần liên thông (# of [connected components](<https://en.wikipedia.org/wiki/Component_(graph_theory)>)) cùng các thông số đi kèm:
    - Số **đỉnh** của từng thành phần
    - Số **cạnh** của từng thành phần
    - Số **bậc nhỏ nhất** của từng thành phần
    - Số **bậc nhiều nhất** của từng thành phần

- Sinh viên viết một báo cáo (10-12 trang) về nội dung của đồ án

  - Vấn đề
  - Lựa chọn CTDL
  - Cài đặt giải thuật (mã giả)
  - Kết quả thực nghiệm

## Hướng dẫn sử dụng và bảo trì phần mềm

Chạy chương trình

```bash
  sh run.sh main
```

Build lại chương trình với CMake

```bash
  sh build.sh main
```
