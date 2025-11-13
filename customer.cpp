#include "Customer.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Constructor
Customer::Customer(const string &id, const string &name,
                   const string &email, const string &phone)
    : Person(id, name, email, phone) {}

// Constructor sao chep
Customer::Customer(const Customer &other)
    : Person(other), bookingHistory(other.bookingHistory) {}

// Operator gan
Customer &Customer::operator=(const Customer &other)
{
    if (this != &other)
    {
        Person::operator=(other);
        bookingHistory = other.bookingHistory;
    }
    return *this;
}

// Destructor
Customer::~Customer() {}

// Dat ve
void Customer::bookTicket(Booking &booking, Showtime &show, int seatNumber)
{
    // Lay ghe
    vector<Seat> seats = show.getSeats();

    // Kiem tra so ghe hop le
    if (seatNumber <= 0 || seatNumber > (int)seats.size())
    {
        cout << "Loi: Khong tim thay ghe!\n";
        return;
    }

    Seat s = seats[seatNumber - 1];

    // Kiem tra ghe da dat chua
    if (!s.isAvailable())
    {
        cout << "Loi: Ghe da duoc dat!\n";
        return;
    }

    // Danh dau ghe da dat va tao Ticket

    // Danh dau trong doi tuong Seat
    s.occupy();

    // Cap nhat lai ghe trong showtime (can setSeats trong Showtime.h)
    seats[seatNumber - 1] = s;
    show.setSeats(seats);

    // Tao ticket ID dang string
    string ticketId = "T" + to_string(booking.getTickets().size() + 1); // Can Booking getTickets()

    // Tao ticket moi
    Ticket newTicket(ticketId, &show, s, show.getPrice(), TicketStatus::BOOKED);

    // Them vao booking (Can Booking addTicket)
    booking.addTicket(newTicket);

    // Them vao lich su
    bookingHistory.push_back(newTicket);

    cout << "Dat ve thanh cong! Ma ve: " << ticketId << "\n";
}

// Huy ve - VERSION HOAN CHINH
void Customer::cancelTicket(Booking &booking, int seatNumber)
{
    string seatId = "A" + to_string(seatNumber);
    bool found = false;

    for (auto &t : booking.getTickets())
    {
        if (t.getSeatInfo().getId() == seatId)
        {
            t.cancel();
            found = true;
            break;
        }
    }

    if (!found)
        cout << "Khong tim thay ve trong lich su!\n";
    else
        cout << "Huy ve thanh cong!\n";
}

// Xem lich su
void Customer::viewHistory() const
{
    cout << "\n--- Lich su dat ve cua " << getName() << " ---\n";
    if (bookingHistory.empty())
    {
        cout << "(Chua co ve nao duoc dat trong lich su.)\n";
        return;
    }
    for (const auto &ticket : bookingHistory)
    {
        ticket.info();
    }
}

void Customer::addTicketToHistory(const Ticket &ticket)
{
    //  them ve vao lich su
    bookingHistory.push_back(ticket);
}

const vector<Ticket> &Customer::getBookingHistory() const
{
    return bookingHistory;
}
