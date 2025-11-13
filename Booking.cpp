#include <bits/stdc++.h>
#include "Booking.h"
using namespace std;

// them ve
void Booking::addTicket(const Ticket &t)
{
    tickets.push_back(t);
}

// huy ve
void Booking::removeTicket(string id)
{
    for (auto it = tickets.begin(); it != tickets.end(); ++it)
    {
        if (it->getId() == id)
        {
            tickets.erase(it);
            cout << "Da xoa ve " << id << " khoi danh sach." << endl;
            return;
        }
    }
    cout << "Khong tim thay ve co ma " << id << " de xoa." << endl;
}

vector<Ticket> &Booking::getTickets()
{
    return tickets;
}

Ticket *Booking::findTicket(const string &id)
{
    for (auto &it : tickets)
    {
        if (it.getId() == id)
        {
            return &it;
        }
    }
    return nullptr;
}

void Booking::cancelTicket(const string &id, bool removeAfterCancel)
{
    for (auto &t : tickets)
    {
        if (t.getId() == id)
        {
            t.cancel(); // goi phuong thuc trong Ticket
            if (removeAfterCancel)
            {
                removeTicket(id); // neu chon xoa sau khi huy
            }
            return;
        }
    }
    cout << "Khong tim thay ve co ma " << id << " de huy." << endl;
}
