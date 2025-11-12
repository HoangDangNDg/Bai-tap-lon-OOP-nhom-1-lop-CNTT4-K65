#include <iostream>
#include <vector>
#include <string>
#include <limits>    // Dùng cho getMenuChoice()
#include <stdexcept> // Dùng cho stod()

// BAO GỒM TẤT CẢ CÁC FILE cpp CỦA HỆ THỐNG
#include "Entity.cpp"
#include "Person.cpp"
#include "Movie.cpp"
#include "Seat.cpp"
#include "Showtime.cpp"
#include "Ticket.cpp"
#include "Booking.cpp"
#include "Customer.cpp"
#include "Staff.cpp"
#include "datveo.cpp" // Định nghĩa lớp BookingAlgorithm

// LƯU Ý QUAN TRỌNG KHI BIÊN DỊCH:
// Bạn KHÔNG ĐƯỢC #include các file .cpp vào đây.
// Bạn PHẢI biên dịch tất cả các file .cpp cùng một lúc.
//
// Lệnh biên dịch mẫu (ví dụ: g++):
// g++ main.cpp Entity.cpp Person.cpp Movie.cpp Seat.cpp Showtime.cpp Ticket.cpp Booing.cpp customer.cpp Staff.cpp datveo.cpp -o chuongtrinh.exe
//

using namespace std;

// =============================================
// CÁC HÀM TIỆN ÍCH GIAO DIỆN (UI Helpers)
// =============================================

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pressEnterToContinue() {
    cout << "\nNhan Enter de tiep tuc...";
    // Xóa bộ đệm input cũ
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    // Nếu cin đang ở trạng thái lỗi, phục hồi lại
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int getMenuChoice() {
    int choice;
    cout << "\n> Nhap lua chon cua ban: ";
    while (!(cin >> choice)) {
        cout << "[!] Gia tri khong hop le. Vui long nhap lai: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    // Xóa bộ đệm sau khi nhập số để tránh trôi lệnh cho getStringInput
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

string getStringInput(string prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

// =============================================
// KHỞI TẠO DỮ LIỆU (DATABASE GIẢ LẬP)
// =============================================

void setupInitialData(vector<Movie>& movies, vector<Showtime>& showtimes,
                      vector<Customer>& customers, vector<Staff>& staff)
{
    cout << "Dang khoi tao du lieu he thong..." << endl;

    movies.push_back(Movie("M001", "Interstellar", "Sci-Fi", 169, 9.2, "Mot nhom tham hiem du hanh qua lo sau..."));
    movies.push_back(Movie("M002", "Oppenheimer", "Lich su", 180, 8.6, "Cau chuyen ve nha khoa hoc J. Robert Oppenheimer..."));
    movies.push_back(Movie("M003", "The Dark Knight", "Hanh dong", 152, 9.0, "Batman doi dau voi ten toi pham Joker..."));

    showtimes.push_back(Showtime("S001", movies[0], "02/11/2025 19:30", 120000, 5, 8)); // 40 ghế
    showtimes.push_back(Showtime("S002", movies[1], "02/11/2025 20:00", 135000, 6, 10)); // 60 ghế
    showtimes.push_back(Showtime("S003", movies[2], "02/11/2025 21:30", 110000, 5, 8)); // 40 ghế

    customers.push_back(Customer("C001", "Nguyen Van A", "a@gmail.com", "0901234567"));
    customers.push_back(Customer("C002", "Le Thi B", "b@gmail.com", "0908889999"));

    staff.push_back(Staff("E001", "Tran Minh C", "c.staff@cinema.com", "011223344", "Quan ly rap"));

    cout << "Khoi tao thanh cong!" << endl;
}

// =============================================
// CÁC CHỨC NĂNG MENU (LOGIC CỦA NGƯỜI 5)
// =============================================

// --- Chức năng của Khách Hàng ---

void handleCustomerBooking(Customer& customer, Booking& bookingSystem, vector<Showtime>& showtimes) {
    clearScreen();
    cout << "--- CHON SUAT CHIEU ---\n";
    if (showtimes.empty()) {
        cout << "Hien khong co suat chieu nao.\n";
        pressEnterToContinue();
        return;
    }

    for (size_t i = 0; i < showtimes.size(); ++i) {
        cout << "  [" << (i + 1) << "] " << showtimes[i].getMovie().getTitle()
             << " - " << showtimes[i].getDateTime() << endl;
    }

    int showChoice = getMenuChoice();
    if (showChoice <= 0 || showChoice > (int)showtimes.size()) {
        cout << "[!] Lua chon khong hop le.\n";
        pressEnterToContinue();
        return;
    }

    Showtime& selectedShow = showtimes[showChoice - 1];

    BookingAlgorithm::displayAvailableSeats(selectedShow);

    string seatId = getStringInput("> Nhap ma ghe muon dat (vi du: A5, C10): ");

    // Gọi thuật toán đặt vé (Người 4)
    bool success = BookingAlgorithm::bookSeat(selectedShow, bookingSystem,
                                          seatId, selectedShow.getPrice());

    // *** SỬA LỖI TẠI ĐÂY ***
    // Nếu đặt vé thành công, gọi hàm addTicketToHistory để cập nhật
    if (success) {
        // Lấy mã vé vừa được tạo (BookingAlgorithm tự tạo ID)
        string newTicketId = "T" + to_string(bookingSystem.getTickets().size());
        Ticket* newTicket = bookingSystem.findTicket(newTicketId);
        
        if (newTicket) {
            
            // ===== ĐÂY LÀ DÒNG SỬA LỖI (THAY THẾ CHO CÚ "HACK" BỊ LỖI) =====
            // Giả sử bạn đã thêm hàm addTicketToHistory vào Customer.h/.cpp
            customer.addTicketToHistory(*newTicket);
            // =======================================================
            
            cout << "Da them ve vao lich su cua ban.\n";
        } else {
             cout << "[!] Loi: Khong tim thay ve " << newTicketId << " de thêm vào l?ch s?.\n";
        }
    }
    // Dòng "? Seat not found!" sẽ không còn xuất hiện nữa

    pressEnterToContinue();
}

void handleCustomerCancel(Customer& customer, Booking& bookingSystem, vector<Showtime>& showtimes) {
    clearScreen();
    cout << "--- HUY VE ---\n";

    // 1. In lịch sử để người dùng biết mã vé
    // (Giả sử bạn đã thêm hàm getBookingHistory() vào Customer.h/.cpp
    //  như hướng dẫn sửa lỗi trước đó)
    customer.viewHistory();
    if (customer.getBookingHistory().empty()) { 
        pressEnterToContinue();
        return;
    }

    string ticketId = getStringInput("> Nhap MA VE (vi du: T1) ban muon huy: ");

    // 3. Tìm vé trong hệ thống TỔNG
    Ticket* ticketToCancel = bookingSystem.findTicket(ticketId);
    if (ticketToCancel == nullptr) {
        cout << "[!] Khong tim thay ve " << ticketId << " trong he thong.\n";
        pressEnterToContinue();
        return;
    }

    // 4. Lấy đúng Showtime để cập nhật lại ghế
    Showtime* showOfTicket = ticketToCancel->getShowtime();
    Showtime* targetShow = nullptr;

    if (showOfTicket == nullptr) {
         cout << "[!] Loi: Ve nay khong liên k?t v?i su?t chi?u nào.\n";
         pressEnterToContinue();
         return;
    }
    
    // Phải tìm đúng đối tượng showtime trong danh sách chính (allShowtimes)
    // để cập nhật ghế
    for(auto& s : showtimes) {
        if(s.getId() == showOfTicket->getId()) {
            targetShow = &s;
            break;
        }
    }

    if (targetShow == nullptr) {
        cout << "[!] Loi: Khong tim thay suat chieu S" << showOfTicket->getId() << " trong he thong.\n";
        pressEnterToContinue();
        return;
    }

    // 5. Gọi thuật toán hủy vé (Người 4)
    BookingAlgorithm::cancelTicket(*targetShow, bookingSystem, ticketId);
    
    // 6. Cập nhật lịch sử khách hàng (Logic này cần được thêm vào Customer.cpp)
    // Hiện tại, customer.viewHistory() sẽ hiển thị vé chưa được hủy
    // vì nó lưu bản sao. 
    // Chúng ta cần thêm một hàm như `customer.updateTicketInHistory(ticketId, CANCELED)`
    // Tạm thời, chúng ta chấp nhận hạn chế này của thiết kế:
    
    cout << "Luu y: Ve da duoc huy tren he thong trung tam.\n";
    cout << "(Lich su dat ve cua ban se khong cap nhat trang thai huy do thiet ke luu tru ban sao).\n";

    pressEnterToContinue();
}

// Menu chính cho khách hàng
void showCustomerMenu(Customer& customer, Booking& bookingSystem, vector<Showtime>& showtimes) {
    bool running = true;
    while (running) {
        clearScreen();
        cout << "--- MENU KHACH HANG ---\n";
        cout << "Chao mung, " << customer.getName() << "!\n";
        cout << "1. Dat ve\n";
        cout << "2. Huy ve\n";
        cout << "3. Xem lich su dat ve\n";
        cout << "0. Dang xuat (Quay lai menu chinh)\n";

        int choice = getMenuChoice();

        switch (choice) {
            case 1:
                handleCustomerBooking(customer, bookingSystem, showtimes);
                break;
            case 2:
                handleCustomerCancel(customer, bookingSystem, showtimes);
                break;
            case 3:
                clearScreen();
                customer.viewHistory(); // Gọi Người 3
                pressEnterToContinue();
                break;
            case 0:
                running = false;
                break;
            default:
                cout << "[!] Lua chon khong hop le.\n";
                pressEnterToContinue();
        }
    }
}

// --- Chức năng của Nhân Viên ---

void handleStaffAddShowtime(Staff& staff, vector<Showtime>& allShowtimes, const vector<Movie>& allMovies) {
    clearScreen();
    cout << "--- THEM SUAT CHIEU MOI ---\n";

    if (allMovies.empty()) {
        cout << "Chua co phim nao trong he thong. Vui long them phim truoc.\n";
        pressEnterToContinue();
        return;
    }
    
    for (size_t i = 0; i < allMovies.size(); ++i) {
        cout << "  [" << (i + 1) << "] " << allMovies[i].getTitle() << endl;
    }
    cout << "Chon phim cho suat chieu: ";
    int movieChoice = getMenuChoice();
    if (movieChoice <= 0 || movieChoice > (int)allMovies.size()) {
        cout << "[!] Lua chon phim khong hop le.\n";
        pressEnterToContinue();
        return;
    }
    const Movie& selectedMovie = allMovies[movieChoice - 1];

    string showId = "S" + to_string(allShowtimes.size() + 101); // Tạo ID mới
    string time = getStringInput("> Nhap thoi gian (DD/MM/YYYY HH:MM): ");
    double price;
    cout << "> Nhap gia ve: ";
    while (!(cin >> price)) {
        cout << "[!] Gia tri khong hop le. Vui long nhap lai: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa bộ đệm

    Showtime newShow(showId, selectedMovie, time, price, 5, 8); // mặc định 5x8

    staff.addShowtime(newShow); // Thêm vào vector của staff (N3)
    allShowtimes.push_back(newShow); // Thêm vào vector tổng của hệ thống (main)

    cout << "\nDa them suat chieu moi thanh cong!\n";
    newShow.info();
    pressEnterToContinue();
}

// CHỨC NĂNG MỚI: Xóa Suất Chiếu
void handleStaffRemoveShowtime(Staff& staff, vector<Showtime>& allShowtimes) {
    clearScreen();
    cout << "--- XOA SUAT CHIEU ---\n";

    if (allShowtimes.empty()) {
        cout << "Khong co suat chieu nao de xoa." << endl;
        pressEnterToContinue();
        return;
    }
    for (size_t i = 0; i < allShowtimes.size(); ++i) {
        cout << "  [" << (i + 1) << "] ID: " << allShowtimes[i].getId() 
             << " - " << allShowtimes[i].getMovie().getTitle()
             << " - " << allShowtimes[i].getDateTime() << endl;
    }

    string idToDelete = getStringInput("> Nhap ID (vi du: S001) cua suat chieu can xoa: ");

    bool found = false;
    for (auto it = allShowtimes.begin(); it != allShowtimes.end(); ++it) {
        if (it->getId() == idToDelete) {
            
            // Lấy thời gian (vì hàm của Staff.cpp dùng 'time' để xóa)
            string time = it->getDateTime();
            
            // 1. Xóa khỏi danh sách TỔNG (main)
            allShowtimes.erase(it);
            
            // 2. Gọi hàm xóa khỏi danh sách NỘI BỘ của Staff (Người 3)
            staff.removeShowtime(time); // Dùng 'time' như định nghĩa trong Staff.h
            
            cout << "\nDa xoa thanh cong suat chieu " << idToDelete << " khoi he thong." << endl;
            found = true;
            break; 
        }
    }

    if (!found) {
        cout << "[!] Khong tim thay suat chieu voi ID: " << idToDelete << endl;
    }
    pressEnterToContinue();
}

// CHỨC NĂNG MỚI: Quản lý Suất Chiếu (ĐÃ SỬA LỖI LOGIC)
void handleStaffManageShowtime(Staff& staff, vector<Showtime>& allShowtimes) {
    clearScreen();
    cout << "--- QUAN LY (CAP NHAT) SUAT CHIEU ---\n";

    if (allShowtimes.empty()) {
        cout << "Khong co suat chieu nao de quan ly." << endl;
        pressEnterToContinue();
        return;
    }
    for (size_t i = 0; i < allShowtimes.size(); ++i) {
        cout << "  [" << (i + 1) << "] ID: " << allShowtimes[i].getId()
             << " - " << allShowtimes[i].getMovie().getTitle()
             << " - " << allShowtimes[i].getDateTime() << endl;
    }

    string idToEdit = getStringInput("> Nhap ID (vi du: S001) cua suat chieu can cap nhat: ");
    
    // Tìm suất chiếu đó (lấy bằng THAM CHIẾU để có thể sửa)
    Showtime* targetShow = nullptr;
    for (auto& show : allShowtimes) {
        if (show.getId() == idToEdit) {
            targetShow = &show; // Lấy địa chỉ của suất chiếu trong vector tổng
            break;
        }
    }

    if (targetShow == nullptr) {
        cout << "[!] Khong tim thay suat chieu voi ID: " << idToEdit << endl;
        pressEnterToContinue();
        return;
    }

    // Lấy thông tin mới (và lấy thông tin CŨ)
    cout << "--- Dang cap nhat cho suat chieu " << idToEdit << " ---" << endl;
    cout << "   (Bo trong va nhan Enter n?u khong muon thay d?i)" << endl;

    // Lấy thông tin CŨ để phòng
    string oldTime = targetShow->getDateTime();
    double oldPrice = targetShow->getPrice();

    // Hỏi thông tin MỚI
    string newTimeStr = getStringInput("> Nhap thoi gian moi (Cu: " + oldTime + "): ");
    string newPriceStr = getStringInput("> Nhap gia ve moi (Cu: " + to_string(oldPrice) + "): ");

    // Quyết định giá trị cuối cùng
    string finalTime = oldTime;
    double finalPrice = oldPrice;

    if (!newTimeStr.empty()) {
        finalTime = newTimeStr; // Nếu người dùng nhập, sử dụng giá trị mới
    }

    if (!newPriceStr.empty()) {
        try {
            finalPrice = stod(newPriceStr); // Nếu người dùng nhập, sử dụng giá trị mới
        } catch (const std::exception& e) {
            cout << "[!] Gia ve khong hop le, se giu lai gia cu." << endl;
            finalPrice = oldPrice; // Nếu nhập bậy, dùng giá trị cũ
        }
    }

    // Gọi hàm của Staff với dữ liệu đã được chuẩn hóa
    // Hàm Staff::manageShowtime sẽ cập nhật trực tiếp `targetShow`
    // vì `targetShow` là con trỏ trỏ đến đối tượng gốc trong `allShowtimes`.
    staff.manageShowtime(*targetShow, finalTime, finalPrice);
    
    pressEnterToContinue();
}

// Menu chính cho nhân viên (ĐÃ CẬP NHẬT)
void showStaffMenu(Staff& staff, vector<Showtime>& showtimes, vector<Movie>& movies) {
    bool running = true;
    while (running) {
        clearScreen();
        cout << "--- MENU NHAN VIEN ---\n";
        cout << "Nhan vien: " << staff.getName() << "\n";
        cout << "1. Them suat chieu\n";
        cout << "2. Xoa suat chieu\n";         // Đã cập nhật
        cout << "3. Quan ly suat chieu\n"; // Đã cập nhật
        cout << "4. Xem thong tin ca nhan\n";
        cout << "0. Dang xuat (Quay lai menu chinh)\n";

        int choice = getMenuChoice();

        switch (choice) {
            case 1:
                handleStaffAddShowtime(staff, showtimes, movies);
                break;
            case 2:
                // GỌI HÀM MỚI
                handleStaffRemoveShowtime(staff, showtimes);
                break;
            case 3:
                // GỌI HÀM MỚI
                handleStaffManageShowtime(staff, showtimes);
                break;
            case 4:
                clearScreen();
                staff.info(); // Gọi Người 3
                pressEnterToContinue();
                break;
            case 0:
                running = false;
                break;
            default:
                cout << "[!] Lua chon khong hop le.\n";
                pressEnterToContinue();
        }
    }
}


// =============================================
// HÀM MAIN (NGƯỜI 5: TÍCH HỢP)
// =============================================

int main() {
    // === 1. KHỞI TẠO HỆ THỐNG ===
    Booking bookingSystem; // Hệ thống Quản lý Vé trung tâm (Người 2)

    vector<Movie> allMovies;
    vector<Showtime> allShowtimes;
    vector<Customer> allCustomers;
    vector<Staff> allStaff;

    setupInitialData(allMovies, allShowtimes, allCustomers, allStaff);
    pressEnterToContinue(); // Dừng để xem thông báo "Khoi tao thanh cong"

    // === 2. VÒNG LẶP CHƯƠNG TRÌNH CHÍNH ===
    bool appRunning = true;
    while (appRunning) {
        clearScreen();
        cout << "*****************************************\n";
        cout << "* CHAO MUNG DEN VOI HE THONG DAT VE \n";
        cout << "*****************************************\n";
        cout << "1. Dang nhap (Khach hang)\n";
        cout << "2. Dang nhap (Nhan vien)\n";
        cout << "0. Thoat chuong trinh\n";

        int mainChoice = getMenuChoice();

        switch (mainChoice) {
            case 1: {
                // Giả lập đăng nhập thành công với Customer 1
                Customer& currentUser = allCustomers[0];
                showCustomerMenu(currentUser, bookingSystem, allShowtimes);
                break;
            }
            case 2: {
                // Giả lập đăng nhập thành công với Staff 1
                Staff& currentStaff = allStaff[0];
                showStaffMenu(currentStaff, allShowtimes, allMovies);
                break;
            }
            case 0:
                appRunning = false;
                cout << "Cam on ban da su dung he thong!\n";
                break;
            default:
                cout << "[!] Lua chon khong hop le.\n";
                pressEnterToContinue();
        }
    }

    return 0;
}