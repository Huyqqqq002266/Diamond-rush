# Diamond-rush
📖 Diamond Rush SDL2
Diamond Rush SDL2 Edition là một game phiêu lưu giải đố theo phong cách cổ điển, nơi bạn điều khiển nhân vật đi khám phá các hang động bí ẩn, thu thập kim cương và tránh né những mối nguy hiểm như đá rơi và rắn độc. Game được lập trình bằng C++ với SDL2.
📦 Cách tải và chạy game
Bước 1: Clone repo về:
git clone https://github.com/Huyqqqq002266/Diamond-rush.git

Bước 2: Cài đặt SDL2 và các thư viện cần thiết:
SDL2

SDL2_image

SDL2_mixer

SDL2_ttf

Bước 3: Mở Visual Studio 2022 , cấu hình project với thư viện SDL2.
Bước 4: Build project và chạy file diamond_rush.exe
🕹️ Bắt đầu game
Click NEW GAME để vào menu chọn màn chơi.

Nhấn phím mũi tên để di chuyển nhân vật.

Thu thập kim cương để mở cổng kết thúc màn chơi.

Tránh bị đá rơi hoặc rắn tấn công.

🎮 Các thành phần trong game
📦 Các đối tượng:
Nhân vật chính: Di chuyển theo từng ô, thu thập kim cương, né đá và rắn.

Đá: Rơi khi không có vật cản bên dưới. Đè chết nhân vật hoặc rắn.
![rock](https://github.com/user-attachments/assets/58d66bbb-c224-40ff-8227-0a94d45255f4)

Kim cương: Thu thập đủ số lượng để mở cửa exit.
![diamond-removebg-preview](https://github.com/user-attachments/assets/0510dbe1-e4ab-43ae-9cfa-aa01ff65a640)

Rắn: Di chuyển tự động. Chạm vào nhân vật là mất mạng.

Lá cây: Đi qua phát ra âm thanh.
![leaves](https://github.com/user-attachments/assets/a2674dd9-f2b1-48ac-8220-015b6e98676c)

Cổng Exit: Mở khi đủ số kim cương.
![exit](https://github.com/user-attachments/assets/85d271ee-f78d-459f-9725-35b170b3c450)

🎨 Đồ họa:
Bản đồ được tạo ra từng các tile ảnh.

📊 Các tính năng:
Hệ thống bản đồ dạng mảng 2D.

Di chuyển nhân vật theo từng ô.

Trọng lực cho đá.

Enemy di chuyển thông minh.

Mỗi mạng bị mất sẽ reset map về trạng thái ban đầu.

Hiệu ứng âm thanh khi thu thập hoặc tương tác.

Màn hình Game Over khi hết mạng.

Menu Start, Setting, Level Select đầy đủ.

📜 Cách chơi
🎮 Điều khiển:
Mũi tên: Di chuyển nhân vật.
Esc: Tạm dừng game.

📌 Luật chơi:
Thu thập kim cương, tránh đá và kẻ thù.

Khi đủ kim cương → đến cổng exit để qua màn.

Chết bởi đá/rắn → mất mạng, map trở về trạng thái gốc.

Hết mạng → Game Over.

🏆 Chiến thắng và thất bại
Chiến thắng: Thu thập đủ kim cương và đi qua cổng exit.

Thất bại: Bị đá đè hoặc rắn cắn hết số mạng.

🎵 Âm thanh
Âm nhạc nền và hiệu ứng sử dụng SDL2_mixer.

📑 Credit
Code: Huyqqqq

Ý tưởng: Dựa trên game Diamond Rush bản mobile cổ điển.

Thư viện: SDL2, SDL2_image, SDL2_mixer, SDL2_ttf.
