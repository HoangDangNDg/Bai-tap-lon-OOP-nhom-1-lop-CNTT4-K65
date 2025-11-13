#include "Customer.h"
#include "Showtime.h"
#include "Movie.h"
#include "Seat.h"
#include "Booking.h"
#include <iostream>
using namespace std;

void testCancelTicketOnly()
{
    cout << "\n===== TEST CHUC NANG HUY VE =====\n";

    // 1. Tao phim va suat chieu
    Movie mv("M100", "Interstellar", "Sci-Fi", 169, 9.2);
    Showtime st("ST100", mv, "19:30 01/11/2025", 120000);

    // 2. Tao khach hang va booking
    Customer kh("C003", "Tran Van C", "c@gmail.com", "0901122334");
    Booking booking;

    // 3. Dat truoc 2 ve
    kh.bookTicket(booking, st, 7);
    kh.bookTicket(booking, st, 8);

    cout << "\n--- Danh sach ve sau khi dat ---\n";
    for (auto &t : booking.getTickets())
    {
        t.info();
    }

    // 4. Huy ve ghe A7
    cout << "\n--- HUY VE GHE A7 ---\n";
    kh.cancelTicket(booking, 7);

    // 5. In lai danh sach ve
    cout << "\n--- Danh sach ve sau khi huy ---\n";
    for (auto &t : booking.getTickets())
    {
        t.info();
    }

    // 6. In lai lich su de xem trang thai da thay doi chua
    cout << "\n--- Lich su dat ve cua khach hang ---\n";
    kh.viewHistory();

    cout << "\n===== KET THUC TEST =====\n";
}

int main()
{
    testCancelTicketOnly();
    return 0;
}
