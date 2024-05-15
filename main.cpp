#include <iostream>
#include <vector>
#include <map>
#include <iomanip>
#include <queue>
using namespace std;

class Passenger;
class Ticket;
class Main;

class Passenger
{
public:
    static int id;
    string name;
    int age;
    char gender;
    string berthPreferance;
    int passengerId;
    string alloted;
    int seatNumber;
    Passenger(string name = " ", int age = 0, char gender = ' ', string berthPreferance = " ")
    {
        this->name = name;
        this->age = age;
        this->gender = gender;
        this->berthPreferance = berthPreferance;
        this->passengerId = id;
        id = id + 1;
    }
};
int Passenger::id = 1;

class Ticket
{
public:
    int lowerBerths;
    int upperBerths;
    int middleBerths;
    int rac;
    int waitingList;
    vector<int> lowerPositions;
    vector<int> middlePositions;
    vector<int> upperPositions;
    vector<int> racPositions;
    vector<int> waitingListPositions;
    map<int, Passenger> confirmedTickets;
    queue<int> racPassengers;
    queue<int> waitingListPassengers;

    Ticket()
    {
        lowerPositions.push_back(1);
        middlePositions.push_back(1);
        upperPositions.push_back(1);
        racPositions.push_back(1);
        waitingListPositions.push_back(1);
        lowerBerths = 1;
        upperBerths = 1;
        middleBerths = 1;
        rac = 1;
        waitingList = 1;
    }
    void printBookedTickets()
    {
        cout << setw(25) << "Passenger ID" << setw(25) << "Name" << setw(25) << "Age" << setw(30) << "Gender" << setw(30) << "Seat Alloted" << endl;
        cout << "------------------------------------------------------------------------------------------------------------------------------------------------\n";
        for (auto it = confirmedTickets.begin(); it != confirmedTickets.end(); it++)
        {
            cout << setw(25) << it->second.passengerId << setw(25) << it->second.name << setw(25) << it->second.age << setw(30) << it->second.gender << setw(24) << it->second.seatNumber << " " << it->second.alloted << endl;
            cout << "------------------------------------------------------------------------------------------------------------------------------------------------\n";
        }
    }
    void viewAvailableTickets()
    {
        int totalTickets = lowerBerths + upperBerths + middleBerths;
        if (totalTickets > 0)
        {
            cout << endl
                 << "Available Tickets: " << totalTickets << " Sleeper Berths" << endl;
        }
        else if (rac > 0)
        {
            cout << "\nNo Sleeper Berths Available\n";
            cout << "\nTotal RACs Available:" << rac << "\n";
        }
        else if (waitingList > 0)
        {
            cout << "\nNo Sleeper Berths and RACs Available\n";
            cout << "\nWaiting List:" << waitingList << "\n";
        }
        else
        {
            cout << "\nNo Tickets Available\n";
        }
    }
    void cancelTicket()
    {
        int cId;
        cout << "Enter Passenger ID to be cancelled:";
        cin >> cId;
        for (auto it = confirmedTickets.begin(); it != confirmedTickets.end(); it++)
        {
            if (it->first == cId)
            {
                if (it->second.alloted == "lower")
                {
                    lowerBerths++;
                    lowerPositions.push_back(it->second.seatNumber);
                }
                else if (it->second.alloted == "middle")
                {
                    middleBerths++;
                    middlePositions.push_back(it->second.seatNumber);
                }
                else if (it->second.alloted == "upper")
                {
                    upperBerths++;
                    upperPositions.push_back(it->second.seatNumber);
                }
                else if (it->second.alloted == "RAC")
                {
                    rac++;
                    racPositions.push_back(it->second.seatNumber);
                }
                else if (it->second.alloted == "Waiting List")
                {
                    waitingList++;
                    waitingListPositions.push_back(it->second.seatNumber);
                }
                confirmedTickets.erase(it->first);
                break;
            }
        }
        if (racPassengers.size() > 0 && (lowerBerths > 0 || upperBerths > 0 || middleBerths > 0))
        {
            int id = racPassengers.front();
            Passenger p = confirmedTickets[id];
            int racSeat = p.seatNumber;
            if ((p.berthPreferance == "lower" && lowerBerths > 0) || (p.berthPreferance == "middle" && middleBerths > 0) || (p.berthPreferance == "upper" && upperBerths > 0))
            {
                if (lowerBerths > 0 && p.berthPreferance == "lower")
                {
                    confirmedTickets[id].alloted = "lower";
                    confirmedTickets[id].seatNumber = lowerPositions[0];
                    lowerPositions.erase(lowerPositions.begin());
                    lowerBerths--;
                }
                else if (middleBerths > 0 && p.berthPreferance == "middle")
                {
                    confirmedTickets[id].alloted = "middle";
                    confirmedTickets[id].seatNumber = middlePositions[0];
                    middlePositions.erase(middlePositions.begin());
                    middleBerths--;
                }
                else
                {
                    confirmedTickets[id].alloted = "upper";
                    confirmedTickets[id].seatNumber = upperPositions[0];
                    upperPositions.erase(upperPositions.begin());
                    upperBerths--;
                }
            }
            else if (lowerBerths > 0)
            {
                confirmedTickets[id].alloted = "lower";
                confirmedTickets[id].seatNumber = lowerPositions[0];
                lowerPositions.erase(lowerPositions.begin());
                lowerBerths--;
            }
            else if (middleBerths > 0)
            {
                confirmedTickets[id].alloted = "middle";
                confirmedTickets[id].seatNumber = middlePositions[0];
                middlePositions.erase(middlePositions.begin());
                middleBerths--;
            }
            else if (upperBerths > 0)
            {
                confirmedTickets[id].alloted = "upper";
                confirmedTickets[id].seatNumber = upperPositions[0];
                upperPositions.erase(upperPositions.begin());
                upperBerths--;
            }
            racPassengers.pop();
            rac++;
            racPositions.push_back(racSeat);
        }
        if (waitingListPassengers.size() > 0 && rac > 0)
        {
            int id = waitingListPassengers.front();
            Passenger p = confirmedTickets[id];
            int waitingListSeat = p.seatNumber;
            confirmedTickets[id].alloted = "RAC";
            confirmedTickets[id].seatNumber = racPositions[0];
            racPositions.erase(racPositions.begin());
            racPassengers.push(id);
            waitingListPassengers.pop();
            waitingList++;
            rac--;
            waitingListPositions.push_back(waitingListSeat);
        }
    }
};

class Main
{
public:
    Ticket ticket;
    void bookTicket(Passenger &p)
    {
        if ((p.berthPreferance == "lower" && ticket.lowerBerths > 0) || (p.berthPreferance == "middle" && ticket.middleBerths > 0) || (p.berthPreferance == "upper" && ticket.upperBerths > 0))
        {
            cout << "\nPreferred Berth Available";
            if (p.berthPreferance == "lower")
            {

                p.alloted = p.berthPreferance;
                ticket.lowerBerths--;
                p.seatNumber = ticket.lowerPositions[0];
                ticket.lowerPositions.erase(ticket.lowerPositions.begin());
                ticket.confirmedTickets[p.passengerId] = p;
                cout << "\nBooked Succesfully with seat number " << p.seatNumber << " " << p.alloted << endl;
            }
            else if (p.berthPreferance == "middle")
            {

                p.alloted = p.berthPreferance;
                ticket.middleBerths--;
                p.seatNumber = ticket.middlePositions[0];
                ticket.middlePositions.erase(ticket.middlePositions.begin());
                ticket.confirmedTickets[p.passengerId] = p;
                cout << "\nBooked Succesfully with seat number " << p.seatNumber << " " << p.alloted << endl;
            }
            else if (p.berthPreferance == "upper")
            {

                p.alloted = p.berthPreferance;
                ticket.upperBerths--;
                p.seatNumber = ticket.upperPositions[0];
                ticket.upperPositions.erase(ticket.upperPositions.begin());
                ticket.confirmedTickets[p.passengerId] = p;
                cout << "\nBooked Succesfully with seat number " << p.seatNumber << " " << p.alloted << endl;
            }
        }
        else if (ticket.lowerBerths > 0)
        {
            cout << "\nPreferred Berth Not Available";
            p.alloted = "lower";
            ticket.lowerBerths--;
            p.seatNumber = ticket.lowerPositions[0];
            ticket.lowerPositions.erase(ticket.lowerPositions.begin());
            ticket.confirmedTickets[p.passengerId] = p;
            cout << "\nBooked Succesfully with seat number " << p.seatNumber << " " << p.alloted << endl;
        }
        else if (ticket.middleBerths > 0)
        {
            cout << "\nPreferred Berth Not Available";
            p.alloted = "middle";
            ticket.middleBerths--;
            p.seatNumber = ticket.middlePositions[0];
            ticket.middlePositions.erase(ticket.middlePositions.begin());
            ticket.confirmedTickets[p.passengerId] = p;
            cout << "\nBooked Succesfully with seat number " << p.seatNumber << " " << p.alloted << endl;
        }
        else if (ticket.upperBerths > 0)
        {
            cout << "\nPreferred Berth Not Available";
            p.alloted = "upper";
            ticket.upperBerths--;
            p.seatNumber = ticket.upperPositions[0];
            ticket.upperPositions.erase(ticket.upperPositions.begin());
            ticket.confirmedTickets[p.passengerId] = p;
            cout << "\nBooked Succesfully with seat number " << p.seatNumber << " " << p.alloted << endl;
        }
        else if (ticket.rac > 0)
        {
            cout << "\nNo Berths Available...\n";
            cout << "1. Book RAC\n";
            cout << "2. Go Back\n";
            int ch;
            cout << "Enter your choice:";
            cin >> ch;
            switch (ch)
            {
            case 1:
            {
                p.alloted = "RAC";
                ticket.rac--;
                p.seatNumber = ticket.racPositions[0];
                ticket.racPositions.erase(ticket.racPositions.begin());
                ticket.confirmedTickets[p.passengerId] = p;
                ticket.racPassengers.push(p.passengerId);
                cout << "\nBooked RAC with seat number " << p.seatNumber << " " << p.alloted << endl;
            }
            break;
            case 2:
                run();
                break;
            default:
                run();
            }
        }
        else if (ticket.waitingList > 0)
        {
            cout << "\nNo Berths and RAC Available...\n";
            cout << "1. Book in Waiting List\n";
            cout << "2. Go Back\n";
            int ch;
            cout << "Enter your choice:";
            cin >> ch;
            switch (ch)
            {
            case 1:
            {
                p.alloted = "Waiting List";
                ticket.waitingList--;
                p.seatNumber = ticket.waitingListPositions[0];
                ticket.waitingListPositions.erase(ticket.waitingListPositions.begin());
                ticket.confirmedTickets[p.passengerId] = p;
                ticket.waitingListPassengers.push(p.passengerId);
                cout << "\nBooked in waiting List " << p.seatNumber << " " << p.alloted << endl;
            }
            break;
            case 2:
                run();
                break;
            default:
                run();
            }
        }
        else
        {
            cout << "\nNo Tickets Available!";
        }
    }
    void run()
    {
        while (true)
        {
            int choice;
            cout << "\n\t\t<------Railway Reservation System----->\n";
            cout << "1. Book\n";
            cout << "2. Cancel\n";
            cout << "3. View booked tickets\n";
            cout << "4. View Available tickets\n";
            cout << "5. Exit\n";
            cout << "Enter your choice:";
            cin >> choice;
            switch (choice)
            {
            case 1:
            {
                string name;
                int age;
                char gender;
                string berthPreferance;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter your name:";
                getline(cin, name);
                cout << "Enter the age:";
                cin >> age;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter Gender (M or F):";
                cin >> gender;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter Berth Preferance (lower or middle or upper):";
                getline(cin, berthPreferance);

                Passenger p(name, age, gender, berthPreferance);
                bookTicket(p);
            }
            break;

            case 2:
                ticket.cancelTicket();
                break;

            case 3:
                ticket.printBookedTickets();
                break;

            case 4:
                ticket.viewAvailableTickets();
                break;

            case 5:
                exit(0);
                break;
            default:
                run();
            }
        }
    }
};
int main()
{
    Main obj;
    obj.run();
}