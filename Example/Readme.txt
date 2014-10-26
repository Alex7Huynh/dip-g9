Hướng dẫn sử dụng chức năng Auto Transform trong ứng dụng.

1. Chuẩn bị các file ảnh cần test. Ví dụ như trong thư mục "3. Data\standard_test_images".
2. Chuẩn bị file txt chứa danh sách đường dẫn tất cả các hình ảnh cần test.
Xem ví dụ file "Script_AutoMode.txt".
	- Trong file này tui để đường dẫn tương đối so với file thực thi "2. Program\DemoHaarWavelet.exe" cho mọi người dễ test.
	- Nên để đường dẫn tuyệt đối cho an toàn.
3. Chạy tool "2. Program\DemoHaarWavelet.exe", chọn đường dẫn tới file cấu hình (Script_AutoMode.txt), sau đó chọn thư mục sẽ lưu kết quả.
4. Chương trình tự động chạy transform tất cả các hình ảnh có trong file Script_AutoMode.txt.
	- Hiện tại tui đang hard code để chỉ chạy iteration = 1...3.
	- Chương trình tạo ra file result.csv. Mỗi dòng bao gồm thông tin:
		Đường dẫn file gốc, size file gốc KBs, đường dẫn file transform, size file transform KBs, đường dẫn file invert, size file invert.