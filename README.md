# Diamond-rush
📖 Diamond Rush SDL2

Diamond Rush SDL2  là một game phiêu lưu giải đố theo phong cách cổ điển, nơi bạn điều khiển nhân vật đi khám phá các hang động bí ẩn, thu thập kim cương và tránh né những mối nguy hiểm như đá rơi và rắn độc. Game được lập trình bằng C++ với SDL2.


Đây là video chi tiết về game: https://drive.google.com/file/d/18NfN-SykWDLlkkie-7rwwBLumxGv1mwc/view?usp=sharing


📦 Cách tải và chạy game

Bước 1: Clone repo về:
git clone https://github.com/Huyqqqq002266/Diamond-rush.git

Bước 2: Cài đặt SDL2 và các thư viện cần thiết:

SDL2

SDL2_image

SDL2_mixer

SDL2_ttf

Bước 3: Mở Visual Studio 2022 , cấu hình project với thư viện SDL2.

Bước 4: Build project và chạy

🕹️ Bắt đầu game

Click NEW GAME để vào menu chọn màn chơi.

![menu](https://github.com/user-attachments/assets/82b6bb38-1415-440f-a934-5351ef21c727)


Nhấn phím mũi tên để di chuyển nhân vật.

Thu thập kim cương để mở cổng kết thúc màn chơi.

Tránh bị đá rơi hoặc rắn tấn công.

🎮 Các thành phần trong game

📦 Các đối tượng:

Nhân vật chính: Di chuyển theo từng ô, thu thập kim cương, né đá và rắn.

![character_right](https://github.com/user-attachments/assets/89870971-c298-4ea4-8d7b-8aaec9fd60a1)


Đá: Rơi khi không có vật cản bên dưới. Đè chết nhân vật hoặc rắn.

![rock](https://github.com/user-attachments/assets/58d66bbb-c224-40ff-8227-0a94d45255f4)

Kim cương: Thu thập đủ số lượng để mở cửa exit.

![diamond-removebg-preview](https://github.com/user-attachments/assets/0510dbe1-e4ab-43ae-9cfa-aa01ff65a640)

Rắn: Di chuyển tự động. Chạm vào nhân vật là mất mạng.

![enemy](https://github.com/user-attachments/assets/fecd881e-129d-4bcf-b355-71f669d8750b)


Lá cây: Đi qua phát ra âm thanh.

![leaves](https://github.com/user-attachments/assets/a2674dd9-f2b1-48ac-8220-015b6e98676c)

Cổng Exit: Mở khi đủ số kim cương.

![exit](https://github.com/user-attachments/assets/85d271ee-f78d-459f-9725-35b170b3c450)

🎨 Đồ họa:
Bản đồ được tạo ra từng các tile ảnh.

📊 Các tính năng:
Hệ thống bản đồ dạng mảng 2D.

![background](https://github.com/user-attachments/assets/e67f0cfe-d6e7-451f-814c-e3ddb44461f6)


Di chuyển nhân vật theo từng ô.

Trọng lực cho đá.

Enemy di chuyển thông minh.

Mỗi mạng bị mất sẽ reset map về trạng thái ban đầu.

Hiệu ứng âm thanh khi thu thập hoặc tương tác.

Màn hình Game Over khi hết mạng.

![game_over](https://github.com/user-attachments/assets/fa5feb31-2632-4f92-9fe2-4c48f9d051ac)


Menu Start, Setting, Level Select đầy đủ.

![menu](https://github.com/user-attachments/assets/3a9c3093-387e-44e4-bcd7-f16f61252629)


📜 Cách chơi

🎮 Điều khiển:

Mũi tên: Di chuyển nhân vật.

Esc: Tạm dừng game.

📌 Luật chơi:

Thu thập kim cương, tránh đá và kẻ thù.

![Screenshot 2025-04-26 151047](https://github.com/user-attachments/assets/cb41dc1a-2289-43b7-9cac-f96df1e808f3)


Khi đủ kim cương → đến cổng exit để qua màn.

Chết bởi đá/rắn → mất mạng, map trở về trạng thái gốc.

Hết mạng → Game Over.

🏆 Chiến thắng và thất bại

Chiến thắng: Thu thập đủ kim cương và đi qua cổng exit.

Thất bại: Bị đá đè hoặc rắn cắn hết số mạng.

🎵 Âm thanh

Âm nhạc nền và hiệu ứng sử dụng SDL2_mixer.



***Về source code game Diamond Rush (SDL2 + C++) 

📄 main.cpp

Là file khởi động chính của game

Bao gồm:

#include <SDL_mixer.h>: Dùng cho xử lý nhạc và hiệu ứng âm thanh

#include <iostream>: Thư viện chuẩn C++

#include "CommonFunc.h": File chứa các hằng số, biến dùng chung, và các hàm tiện ích cơ bản cho toàn bộ game

Hàm InitData(): Khởi tạo SDL, SDL_image, SDL_ttf, SDL_mixer và tạo Window + Renderer chính cho game

📁 Các file logic chính

📄 Character.h / Character.cpp

Xây dựng class nhân vật chính

Xử lý:

Di chuyển theo từng ô

Chạm vào vật thể (đá, kim cương, lá, rắn…)

Cập nhật trạng thái (sống, chết)

Tính thời gian đá đè và quyết định trạng thái nhân vật

Kiểm tra game over

📄 Enemy.h / Enemy.cpp
Xây dựng class enemy

Điều khiển enemy di chuyển và tấn công nhân vật

Di chuyển ngang hay dọc

📄 Map.h / Map.cpp
Quản lý bản đồ game

Load map từ mảng tileMap

Hiển thị map lên màn hình

Reset map khi nhân vật chết hoặc qua màn

📄 RockManager.h / RockManager.cpp

Quản lý trọng lực và va chạm của các viên đá

Xử lý đá rơi, va vào nhân vật hoặc enemy

📄 HUD.h / HUD.cpp

Quản lý HUD (Head-up Display)

Hiển thị số mạng, số kim cương thu thập được

Các biểu tượng phụ (ví dụ: chìa khóa, vật phẩm đặc biệt)

📄 menu.h / menu.cpp

Tạo và xử lý các menu của game

Menu chính

Game Over

Menu Pause

Chọn màn

Thắng / Thua

📄 CommonFunc.h / CommonFunc.cpp

Chứa các hằng số dùng chung

Khai báo các biến toàn cục (Window, Renderer…)

Hàm set vị trí, xử lý âm thanh

📄 BaseObject.h / BaseObject.cpp

Class cha cho tất cả object có thể vẽ lên màn hình

Chứa:

Texture

Tọa độ

Kích thước

Các hàm LoadImg(), Render(), Free()



📑 Credit
Code: Huyqqqq

Ý tưởng: Dựa trên game Diamond Rush bản mobile cổ điển.

Thư viện: SDL2, SDL2_image, SDL2_mixer, SDL2_ttf.
