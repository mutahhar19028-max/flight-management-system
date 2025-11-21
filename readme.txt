# Flight Reservation System

A comprehensive flight reservation system built in C++ with SQLite3 database integration.

## Project Structure

```
flightReservationSystem/
├── main.cpp
├── CMakeLists.txt
│
├── entities/              # Data models
│   ├── customer.h/cpp
│   ├── flight.h/cpp
│   ├── airport.h/cpp
│   ├── plane.h/cpp
│   ├── ticket.h/cpp
│   └── offer.h/cpp
│   └── manager.h/cpp
│
├── dsLib/                 # Data structures
│   ├── bst.hpp
│   ├── linkedList.hpp
│   ├── doublyLinkedList.hpp
│   └── queue.hpp
│
├── utilities/             # Helper functions
│   ├── idGenerator.h/cpp
│   ├── dateTime.h/cpp
│   └── validator.h/cpp
│
├── fileIO/                # File operations
│   ├── textReader.h/cpp
│   ├── textWriter.h/cpp
│   └── dbHandler.h/cpp
│
├── ui/                    # UI components
│   └── menu.h/cpp
│
├── sqlite/                # SQLite3 library
│   └── sqlite3.h
│   └── sqlite3.c
│
└── data/                  # Data files
    ├── customers.txt
    ├── flights.txt
    ├── airports.txt
    ├── planes.txt
    ├── tickets.txt
    ├── offers.txt
    └── system-data.db      # Generated at runtime
```

## Features

### Customer Management
- Add, update, delete customers
- View customer profiles with ticket history
- Auto-generate unique customer IDs
- Track miles traveled and number of flights

### Flight Management
- Add, update, delete flights
- Search flights by destination
- View flight details with plane and airport info
- Support multiple ticket classes (Economy, Business)

### Reservation System
- Reserve seats on flights
- Cancel reservations
- Change reservations to different flights
- Automatic seat number assignment
- Discount calculation based on loyalty (miles/flights)

### Waiting List
- Add customers to waiting list when flight is full
- Automatic processing when seats become available
- Maximum size: 5 per 100 seats (5:100 ratio)
- View waiting list status

### Data Persistence
- SQLite3 database for persistent storage
- Auto-save after every operation
- First run: loads from .txt files and creates database
- Subsequent runs: loads directly from database

## Compilation

### Prerequisites
- C++17 compatible compiler (g++ 7.0 or higher)
- SQLite3 library (included in `sqlite/` folder)

### Build
```bash
make
```

### Run
```bash
make run
```

### Clean
```bash
make clean
```

## Usage

### First Run
1. Ensure all .txt files are in the `data/` folder
2. Run the program: `./flightSystem`
3. System will load data from text files and create `systemData.db`

### Subsequent Runs
1. Run the program: `./flightSystem`
2. System will load data directly from `systemData.db`

### Menu Navigation
The system provides an interactive menu with the following options:

**Main Menu:**
1. Flight Reservation - Search, reserve, cancel, change flights
2. Customers - Add, update, delete, view customer profiles
3. Flights - Add, update, delete, view flights
4. Save Data - Manually save to database
5. View System Info - View all airports and planes
6. Exit - Save and exit the system

## Data Structures Used

- **BST (Binary Search Tree)**: Customer storage and searching (O(log n))
- **Doubly Linked List**: Flight storage with bidirectional traversal
- **Singly Linked List**: Airport and plane storage
- **Queue**: Waiting lists per flight (FIFO)
- **Vector**: Tickets and offers storage
- **Map**: Waiting lists mapping (flight ID → queue)

## Key Design Decisions

1. **Single SystemManager**: All operations managed by one class for clean main.cpp
2. **Auto-save**: Database updated after every operation
3. **In-memory + Persistence**: Data structures work in-memory, SQLite for persistence
4. **Template-based DS**: Generic data structures in `ds` namespace
5. **Modular Architecture**: Separation of concerns (entities, logic, UI, I/O)

## Sample Operations

### Reserve a Flight
1. Select "Flight Reservation" → "Search for Flights"
2. Enter destination (e.g., "Paris")
3. Select "Reserve Flight"
4. Enter Customer ID and Flight Number
5. Enter ticket type (Economy/Business)
6. System generates ticket with discount (if eligible)

### View Customer Profile
1. Select "Customers" → "View Customer Profile"
2. Enter Customer ID
3. System displays customer info and all tickets

### Add New Flight
1. Select "Flights" → "Add Flight"
2. View available planes
3. Enter plane ID, date, time, destination
4. System generates unique flight number

## Database Schema

### Tables
- `customers`: Customer information
- `flights`: Flight basic information
- `flight_pricing`: Ticket prices per class per flight
- `flight_seats_reserved`: Reserved seats tracking
- `airports`: Airport information
- `planes`: Plane information
- `plane_seats`: Seat configuration per plane
- `plane_pricing`: Default pricing per plane
- `tickets`: All ticket records
- `offers`: Discount offers
- `waiting_lists`: Waiting list per flight
- `metadata`: System state (last IDs)

## File Formats

### customers.txt
```
customerId,name,phone,dob,numOfFlights,milesTraveled
1001,John Doe,555-1234,15/05/1985,13,51999
```

### flights.txt
```
flightNumber,planeId,day,month,year,departureTime,destination,Class:Price,...
101,1,15,6,2023,00:00,Paris,Economy:400
```

### airports.txt
```
name,country,weather,timeFromIslamabad,milesFromIslamabad
Paris,France,Cloudy,5,2000
```

### planes.txt
```
planeId,description,businessSeats,economySeats
1,Boeing 737,20,80
```

### tickets.txt
```
customerId,customerName,flightNum,ticketNumber,ticketType,cost,discount,reason,dateTime,seatNumber
1001,John Doe,101,5001,Economy,400,0,,15/06/2023 09:00,0
```

### offers.txt
```
offerId,flightNumber(-1=all),offerType,discountPercent
1,-1,miles,10
2,3,flights,15
```

## Error Handling

- Invalid input validation (dates, names, phone numbers)
- Database connection errors
- File I/O errors
- Flight full scenarios → waiting list option
- Attempting to delete customers/flights with existing tickets

## Future Enhancements

- User authentication system
- Multi-threading for concurrent reservations
- Email notifications for waiting list processing
- Payment integration
- Seat map visualization
- Flight status tracking (delayed, cancelled)
- Recurring flight templates

## Authors

- Built for Fast NUCES Flight Reservation System Project
- Instructor: Rana Waqas

## License

Academic Project - Fast NUCES School of Computing