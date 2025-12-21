# Flight Reservation System - Technical Documentation

## Table of Contents
- [Overview](#overview)
- [System Architecture](#system-architecture)
- [Technology Stack](#technology-stack)
- [Project Structure](#project-structure)
- [Data Flow](#data-flow)
- [Database Schema](#database-schema)
- [Backend Implementation](#backend-implementation)
- [Frontend Implementation](#frontend-implementation)
- [Communication Protocol](#communication-protocol)
- [Data Structures](#data-structures)
- [Build System](#build-system)
- [Installation Guide](#installation-guide)
- [API Reference](#api-reference)
- [Performance Analysis](#performance-analysis)
- [Testing Strategy](#testing-strategy)
- [Troubleshooting](#troubleshooting)

---

## Overview

### Purpose
A production-grade flight reservation system demonstrating advanced software engineering principles including clean architecture, custom data structure implementation, inter-process communication, and full-stack development.

### Key Features
- **Real-time Flight Booking** with seat availability tracking
- **Customer Management** with loyalty benefits (miles/flights based)
- **Waiting List System** with automatic processing (5:100 ratio)
- **Dual-Interface Architecture** (Console + Electron GUI)
- **Custom Data Structures** (BST, LinkedList, DoublyLinkedList, Queue)
- **SQLite Integration** with ACID transaction support
- **JSON-based IPC** for frontend-backend communication

### Technical Highlights
- **Zero Runtime Overhead**: Template-based data structures compiled inline
- **O(log n) Lookups**: Binary Search Tree for customer operations
- **Asynchronous IPC**: Non-blocking communication with timeout handling
- **Auto-persistence**: Every operation immediately synced to database
- **Memory Safety**: RAII principles, no memory leaks

---

## System Architecture

### Three-Tier Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    PRESENTATION LAYER                        │
│  ┌───────────────────────────────────────────────────────┐  │
│  │  Electron Framework (Chromium + Node.js)              │  │
│  │  ├─ Main Process (main.js)                            │  │
│  │  │  ├─ Window Management                              │  │
│  │  │  ├─ IPC Handler                                    │  │
│  │  │  ├─ Process Spawner                                │  │
│  │  │  └─ SQLite Direct Access (Read-Only)              │  │
│  │  └─ Renderer Process                                  │  │
│  │     ├─ HTML/CSS/JavaScript                            │  │
│  │     ├─ jQuery + Bootstrap                             │  │
│  │     └─ DataTables.js                                  │  │
│  └───────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
                            ↕ IPC (stdin/stdout JSON)
┌─────────────────────────────────────────────────────────────┐
│                    BUSINESS LOGIC LAYER                      │
│  ┌───────────────────────────────────────────────────────┐  │
│  │  C++ Backend (SystemManager)                          │  │
│  │  ├─ JSON Parser (nlohmann/json)                       │  │
│  │  ├─ Command Processor (json-manager.cpp)              │  │
│  │  ├─ Business Logic (manager.cpp)                      │  │
│  │  │  ├─ Reservation Management                         │  │
│  │  │  ├─ Waiting List Processing                        │  │
│  │  │  ├─ Discount Calculation                           │  │
│  │  │  └─ Validation & Error Handling                    │  │
│  │  └─ Custom Data Structures (dsLib)                    │  │
│  │     ├─ BST<Customer>                                  │  │
│  │     ├─ DoublyLinkedList<Flight>                       │  │
│  │     ├─ LinkedList<Airport/Plane>                      │  │
│  │     └─ Queue<int> (Waiting Lists)                     │  │
│  └───────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
                            ↕ SQLite3 C API
┌─────────────────────────────────────────────────────────────┐
│                    DATA PERSISTENCE LAYER                    │
│  ┌───────────────────────────────────────────────────────┐  │
│  │  SQLite Database (system-data.db)                     │  │
│  │  ├─ 12 Normalized Tables                              │  │
│  │  ├─ B-tree Indexes on Primary Keys                    │  │
│  │  ├─ ACID Transaction Support                          │  │
│  │  └─ Foreign Key Constraints                           │  │
│  └───────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
```

### Design Patterns Used

#### 1. **Single Manager Pattern**
- `SystemManager` acts as facade for all operations
- Encapsulates data structures and business logic
- Provides clean API surface
- Simplifies main.cpp to 15 lines

#### 2. **Command Pattern**
- JSON commands encapsulate operations
- `JsonManager::processCommand()` dispatches to handlers
- Supports undo/redo potential
- Easy to extend with new actions

#### 3. **Repository Pattern**
- `DBHandler` abstracts database operations
- Separation between data access and business logic
- Swappable persistence layer (SQLite → PostgreSQL)

#### 4. **Observer Pattern (Implicit)**
- Frontend waits for backend responses
- ActionId-based correlation
- Timeout handling for failed operations

#### 5. **Template Method Pattern**
- Generic data structure implementations
- Type-safe instantiation
- Code reuse across different entity types

---

## Technology Stack

### Backend (C++)
| Component | Technology | Version | Purpose |
|-----------|-----------|---------|---------|
| Language | C++ | 17 | Core logic |
| Compiler | GCC/Clang/MSVC | 7.0+ | Compilation |
| Build System | CMake | 3.16+ | Cross-platform builds |
| Database | SQLite3 | 3.x | Persistence |
| JSON Library | nlohmann/json | 3.11+ | Serialization |
| Data Structures | Custom Templates | - | In-memory storage |

### Frontend (Electron)
| Component | Technology | Version | Purpose |
|-----------|-----------|---------|---------|
| Framework | Electron | 27.0+ | Desktop UI |
| Runtime | Node.js | 18.0+ | JavaScript engine |
| UI Framework | Bootstrap | 4.5+ | Responsive design |
| Tables | DataTables.js | 1.13+ | Data visualization |
| HTTP Library | jQuery | 3.7+ | DOM manipulation |
| Database Access | sqlite3 (npm) | 5.1+ | Direct DB reads |

### Development Tools
- **Version Control**: Git
- **Code Editor**: VS Code / CLion recommended
- **Database Viewer**: DB Browser for SQLite
- **Debugging**: GDB (C++), Chrome DevTools (Electron)

---

## Project Structure

```
flightReservationSystem/
│
├── CMakeLists.txt                      # Build configuration
├── package.json                        # Node.js dependencies
├── main.js                             # Electron main process
├── backend.cpp                         # C++ JSON entry point
├── main.cpp                            # C++ console entry point
│
├── frontend/                           # Electron UI files
│   ├── index.html                      # Main page loader
│   ├── booking.html                    # Booking interface (4 tabs)
│   ├── database.html                   # Database viewer
│   ├── header.html                     # Navigation header
│   ├── footer.html                     # Page footer
│   ├── deals.html                      # Promotional content
│   ├── preload.js                      # IPC bridge (secure)
│   ├── renderer.js                     # Event handlers
│   ├── styles.css                      # Styling
│   └── images/                         # Static assets
│
├── entities/                           # Data models
│   ├── customer.h/cpp                  # Customer entity
│   ├── flight.h/cpp                    # Flight entity
│   ├── airport.h/cpp                   # Airport entity
│   ├── plane.h/cpp                     # Plane entity
│   ├── ticket.h/cpp                    # Ticket entity
│   ├── offer.h/cpp                     # Offer entity
│   └── manager.h/cpp                   # SystemManager
│
├── dsLib/                              # Data structure templates
│   ├── bst.hpp                         # Binary Search Tree
│   ├── linkedList.hpp                  # Singly Linked List
│   ├── doublyLinkedList.hpp            # Doubly Linked List
│   └── queue.hpp                       # FIFO Queue
│
├── json/                               # JSON communication
│   ├── json.hpp                        # nlohmann/json library
│   ├── json-manager.h                  # Command processor header
│   └── json-manager.cpp                # Command handlers
│
├── utilities/                          # Helper functions
│   ├── idGenerator.h/cpp               # Unique ID generation
│   ├── dateTime.h/cpp                  # Date/time utilities
│   └── validator.h/cpp                 # Input validation
│
├── fileIO/                             # File operations
│   ├── textReader.h/cpp                # Parse .txt files
│   ├── textWriter.h/cpp                # Export to .txt
│   └── dbHandler.h/cpp                 # SQLite operations
│
├── ui/                                 # Console interface (optional)
│   └── menu.h/cpp                      # Interactive menu
│
├── sqlite/                             # SQLite library
│   ├── sqlite3.h                       # Header
│   └── sqlite3.c                       # Implementation
│
├── data/                               # Runtime data
│   ├── customers.txt                   # Initial customer data
│   ├── flights.txt                     # Initial flight data
│   ├── airports.txt                    # Airport information
│   ├── planes.txt                      # Aircraft specs
│   ├── tickets.txt                     # Existing bookings
│   ├── offers.txt                      # Discount rules
│   └── system-data.db                  # SQLite database (generated)
│
└── build/                              # Build artifacts
    ├── flightSystem                    # Console executable
    └── flightSystemJSON                # Electron backend
```

---

## Data Flow

### 1. Application Startup

```
┌─────────────────┐
│  npm start      │
└────────┬────────┘
         │
         ▼
┌─────────────────────────────────────────┐
│  main.js: app.whenReady()              │
│  ├─ Create BrowserWindow                │
│  ├─ Load frontend/index.html           │
│  └─ Call launchCppBackend()            │
└────────┬────────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────────┐
│  spawn("build/flightSystemJSON")        │
│  ├─ Attach stdout listener              │
│  ├─ Attach stderr listener              │
│  └─ Attach close listener               │
└────────┬────────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────────┐
│  backend.cpp: main()                    │
│  ├─ Initialize SystemManager            │
│  │  ├─ Check system-data.db exists?    │
│  │  │  ├─ YES: Load from database      │
│  │  │  └─ NO: Load .txt → Create DB    │
│  │  └─ Populate data structures        │
│  └─ Enter stdin read loop              │
└─────────────────────────────────────────┘
```

### 2. Booking Flow (Detailed)

```
USER ACTION: Click "Book Flight" button
         │
         ▼
┌─────────────────────────────────────────┐
│  renderer.js: $("#book-flight-btn")     │
│  ├─ Read form inputs                    │
│  │  ├─ customerId                       │
│  │  ├─ flightNumber                     │
│  │  └─ ticketType                       │
│  └─ Call window.api.cppAction({...})   │
└────────┬────────────────────────────────┘
         │ (IPC invoke)
         ▼
┌─────────────────────────────────────────┐
│  preload.js: contextBridge              │
│  └─ Secure bridge to main process      │
└────────┬────────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────────┐
│  main.js: ipcMain.handle("cpp-action") │
│  ├─ Generate unique actionId           │
│  ├─ Store resolver in responseQueue    │
│  ├─ Set 10s timeout                    │
│  └─ Write JSON to cppBackend.stdin     │
└────────┬────────────────────────────────┘
         │ (stdin pipe)
         ▼
┌─────────────────────────────────────────┐
│  backend.cpp: std::getline(std::cin)   │
│  └─ Parse JSON command                 │
└────────┬────────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────────┐
│  json-manager.cpp: processCommand()    │
│  ├─ Switch on action type              │
│  └─ Call handleReserveSeat()           │
└────────┬────────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────────┐
│  json-manager.cpp: handleReserveSeat() │
│  ├─ Validate inputs                    │
│  ├─ Find/Create customer               │
│  └─ Call system.reserveSeat()          │
└────────┬────────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────────┐
│  manager.cpp: reserveSeat()             │
│  ├─ Find customer in BST               │
│  ├─ Find flight in DoublyLinkedList    │
│  ├─ Find plane details                 │
│  ├─ Check seat availability            │
│  ├─ Calculate price + discounts        │
│  ├─ Generate ticket                    │
│  ├─ Reserve seat on flight             │
│  ├─ Update customer stats              │
│  └─ Call _saveToDatabase()             │
└────────┬────────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────────┐
│  dbHandler.cpp: saveDatabase()          │
│  ├─ BEGIN TRANSACTION                  │
│  ├─ Serialize all data structures      │
│  │  ├─ customers (BST → vector)        │
│  │  ├─ flights (List → vector)         │
│  │  ├─ tickets (vector)                │
│  │  └─ waiting lists (map)             │
│  ├─ Execute SQL INSERT/UPDATE          │
│  └─ COMMIT                              │
└────────┬────────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────────┐
│  json-manager.cpp: Build response       │
│  └─ {success: true, ticketNumber: ...} │
└────────┬────────────────────────────────┘
         │ (stdout pipe)
         ▼
┌─────────────────────────────────────────┐
│  backend.cpp: std::cout << json.dump() │
│  └─ Write to stdout + flush            │
└────────┬────────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────────┐
│  main.js: cppBackend.stdout.on("data") │
│  ├─ Parse JSON response                │
│  ├─ Match actionId                     │
│  ├─ Resolve promise in responseQueue   │
│  └─ Delete actionId from queue         │
└────────┬────────────────────────────────┘
         │ (Promise resolution)
         ▼
┌─────────────────────────────────────────┐
│  renderer.js: Response received         │
│  ├─ Check response.success              │
│  ├─ Display alert with message          │
│  └─ Clear form inputs                   │
└─────────────────────────────────────────┘
```

### 3. Database Viewing Flow

```
USER ACTION: Click "View Database" button
         │
         ▼
┌─────────────────────────────────────────┐
│  renderer.js: $("#show-database-btn")   │
│  └─ Call window.api.showDatabase()     │
└────────┬────────────────────────────────┘
         │ (IPC invoke)
         ▼
┌─────────────────────────────────────────┐
│  main.js: ipcMain.handle("show-db")    │
│  └─ mainWindow.loadFile("database.html")│
└────────┬────────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────────┐
│  database.html: $(document).ready()     │
│  └─ Call window.api.getAllTables()     │
└────────┬────────────────────────────────┘
         │ (IPC invoke)
         ▼
┌─────────────────────────────────────────┐
│  main.js: ipcMain.handle("get-tables")  │
│  ├─ Open system-data.db (read-only)    │
│  ├─ Query all 6 tables                 │
│  │  ├─ SELECT * FROM customers         │
│  │  ├─ SELECT * FROM flights           │
│  │  ├─ SELECT * FROM tickets           │
│  │  ├─ SELECT * FROM planes            │
│  │  ├─ SELECT * FROM airports          │
│  │  └─ SELECT * FROM offers            │
│  ├─ Close database                     │
│  └─ Return {customers: [...], ...}     │
└────────┬────────────────────────────────┘
         │ (Promise resolution)
         ▼
┌─────────────────────────────────────────┐
│  database.html: Display with DataTables │
│  ├─ For each table:                    │
│  │  ├─ Create HTML table               │
│  │  ├─ Populate with rows              │
│  │  └─ Initialize DataTable({...})     │
│  └─ Enable search/sort/pagination      │
└─────────────────────────────────────────┘
```

---

## Database Schema

### Entity Relationship Diagram

```
┌──────────────┐       ┌──────────────┐       ┌──────────────┐
│  customers   │       │   tickets    │       │   flights    │
├──────────────┤       ├──────────────┤       ├──────────────┤
│ id (PK)      │◄──────┤ customerId   │──────►│ flightNumber │
│ name         │       │ ticketNumber │       │ (PK)         │
│ phone        │       │ (PK)         │       │ planeId (FK) │
│ dob          │       │ flightNum(FK)│       │ day          │
│ numOfFlights │       │ ticketType   │       │ month        │
│ milesTraveled│       │ cost         │       │ year         │
└──────────────┘       │ seatNumber   │       │ departureTime│
                       │ dateTime     │       │ destination  │
                       │ discount     │       └──────┬───────┘
                       └──────────────┘              │
                                                     │
       ┌──────────────┐       ┌──────────────┐     │
       │   planes     │       │  airports    │     │
       ├──────────────┤       ├──────────────┤     │
       │ planeId (PK) │◄──────┤              │◄────┘
       │ description  │       │ name (PK)    │
       │ milesTraveled│       │ country      │
       └──────┬───────┘       │ weather      │
              │               │ timeFromIslamabad│
              │               │ milesFromIslamabad│
              │               └──────────────┘
              ▼
  ┌─────────────────┐
  │  plane_seats    │
  ├─────────────────┤
  │ planeId (FK)    │
  │ className       │
  │ numSeats        │
  └─────────────────┘

  ┌─────────────────┐
  │ plane_pricing   │
  ├─────────────────┤
  │ planeId (FK)    │
  │ className       │
  │ price           │
  └─────────────────┘
```

### Table Definitions

#### 1. **customers**
```sql
CREATE TABLE customers (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    phone TEXT NOT NULL,
    dob TEXT NOT NULL,
    numOfFlights INTEGER DEFAULT 0,
    milesTraveled INTEGER DEFAULT 0
);
```
**Purpose**: Store customer information and loyalty metrics  
**Indexes**: Primary key on `id`  
**Size**: ~100 bytes per record

#### 2. **flights**
```sql
CREATE TABLE flights (
    flightNumber INTEGER PRIMARY KEY,
    planeId INTEGER NOT NULL,
    day INTEGER NOT NULL,
    month INTEGER NOT NULL,
    year INTEGER NOT NULL,
    departureTime TEXT NOT NULL,
    destination TEXT NOT NULL,
    FOREIGN KEY(planeId) REFERENCES planes(planeId)
);
```
**Purpose**: Store flight schedules  
**Indexes**: Primary key on `flightNumber`  
**Size**: ~80 bytes per record

#### 3. **flight_pricing**
```sql
CREATE TABLE flight_pricing (
    flightNumber INTEGER NOT NULL,
    className TEXT NOT NULL,
    price INTEGER NOT NULL,
    FOREIGN KEY(flightNumber) REFERENCES flights(flightNumber)
);
```
**Purpose**: Store per-class pricing for each flight  
**Normalized**: Avoids redundancy when flight has multiple classes

#### 4. **flight_seats_reserved**
```sql
CREATE TABLE flight_seats_reserved (
    flightNumber INTEGER NOT NULL,
    className TEXT NOT NULL,
    reserved INTEGER NOT NULL DEFAULT 0,
    FOREIGN KEY(flightNumber) REFERENCES flights(flightNumber)
);
```
**Purpose**: Track occupied seats per class per flight  
**Updated**: On every reservation/cancellation

#### 5. **tickets**
```sql
CREATE TABLE tickets (
    ticketNumber INTEGER PRIMARY KEY,
    customerId INTEGER NOT NULL,
    customerName TEXT NOT NULL,
    flightNum INTEGER NOT NULL,
    ticketType TEXT NOT NULL,
    cost INTEGER NOT NULL,
    discount INTEGER NOT NULL,
    reasonForDiscount TEXT,
    dateTime TEXT NOT NULL,
    seatNumber INTEGER NOT NULL,
    FOREIGN KEY(customerId) REFERENCES customers(id),
    FOREIGN KEY(flightNum) REFERENCES flights(flightNumber)
);
```
**Purpose**: Booking records  
**Indexes**: Primary key on `ticketNumber`, index on `customerId`

#### 6. **planes**
```sql
CREATE TABLE planes (
    planeId INTEGER PRIMARY KEY,
    description TEXT NOT NULL,
    milesTraveled INTEGER DEFAULT 0
);
```
**Purpose**: Aircraft information  
**Size**: ~60 bytes per record

#### 7-8. **plane_seats & plane_pricing**
```sql
CREATE TABLE plane_seats (
    planeId INTEGER NOT NULL,
    className TEXT NOT NULL,
    numSeats INTEGER NOT NULL,
    FOREIGN KEY(planeId) REFERENCES planes(planeId)
);

CREATE TABLE plane_pricing (
    planeId INTEGER NOT NULL,
    className TEXT NOT NULL,
    price INTEGER NOT NULL,
    FOREIGN KEY(planeId) REFERENCES planes(planeId)
);
```
**Purpose**: Seat configuration and default pricing per plane

#### 9. **airports**
```sql
CREATE TABLE airports (
    name TEXT PRIMARY KEY,
    country TEXT NOT NULL,
    weather TEXT,
    timeFromIslamabad INTEGER NOT NULL,
    milesFromIslamabad INTEGER NOT NULL
);
```
**Purpose**: Destination information  
**Indexes**: Primary key on `name`

#### 10. **offers**
```sql
CREATE TABLE offers (
    offerId INTEGER PRIMARY KEY,
    flightNumber INTEGER NOT NULL,
    offerType TEXT NOT NULL,
    discountAmount INTEGER NOT NULL
);
```
**Purpose**: Discount rules  
**-1 flightNumber**: Applies to all flights  
**offerType**: "miles" or "flights"

#### 11. **waiting_lists**
```sql
CREATE TABLE waiting_lists (
    flightId INTEGER NOT NULL,
    customerId INTEGER NOT NULL,
    FOREIGN KEY(flightId) REFERENCES flights(flightNumber),
    FOREIGN KEY(customerId) REFERENCES customers(id)
);
```
**Purpose**: Queue customers when flights are full  
**Order**: Maintained by insertion order (FIFO)

#### 12. **metadata**
```sql
CREATE TABLE metadata (
    key TEXT PRIMARY KEY,
    value INTEGER NOT NULL
);
```
**Purpose**: System state  
**Keys**: lastCustomerId, lastTicketId, lastFlightId

---

## Backend Implementation

### Core Components

#### 1. SystemManager (manager.cpp)

**Responsibility**: Orchestrate all business operations

```cpp
class FlightReservationSystem {
private:
    ds::BST<Customer> _m_customers;                    // O(log n) lookup
    ds::DoublyLinkedList<Flight> _m_flights;          // Bidirectional
    ds::LinkedList<Airport> _m_airports;              // Simple storage
    ds::LinkedList<Plane> _m_planes;                  // Simple storage
    std::vector<Ticket> _m_tickets;                   // Sequential
    std::vector<Offer> _m_offers;                     // Sequential
    std::map<int, ds::Queue<int>> _m_waitingLists;   // Per-flight queues
    
public:
    bool initialize(const std::string& dbPath);
    bool reserveSeat(int customerId, int flightNumber, const std::string& ticketType);
    bool cancelReservation(int ticketNumber);
    bool changeReservation(int ticketNumber, int newFlight, const std::string& newType);
    std::vector<Ticket> getCustomerTickets(int customerId);
    // ... other methods
};
```

**Key Methods**:

- **initialize()**: Load data from database or text files
- **reserveSeat()**: Full booking flow with validation
- **cancelReservation()**: Release seat + process waiting list
- **changeReservation()**: Cancel old + reserve new atomically
- **_saveToDatabase()**: Persist all changes immediately

**Design Decisions**:
- Single class for all operations (Facade pattern)
- In-memory data structures for performance
- Immediate persistence after every mutation
- RAII for automatic cleanup

#### 2. JsonManager (json-manager.cpp)

**Responsibility**: Bridge between JSON commands and C++ methods

```cpp
class JsonManager {
private:
    FlightReservationSystem& system;

public:
    json processCommand(const json& cmd);

private:
    json handleReserveSeat(const json& cmd);
    json handleCancelReservation(const json& cmd);
    json handleChangeReservation(const json& cmd);
    json handleGetCustomerTickets(const json& cmd);
    // ... other handlers
    
    json ticketToJson(Ticket* ticket);
    json flightToJson(Flight* flight);
    json customerToJson(Customer* customer);
};
```

**Command Processing Flow**:
1. Parse JSON from stdin
2. Extract `action` field
3. Dispatch to appropriate handler
4. Call SystemManager methods
5. Serialize response to JSON
6. Write to stdout with flush

**Error Handling**:
```cpp
try {
    // Business logic
} catch (const std::exception& e) {
    response["success"] = false;
    response["message"] = std::string("Exception: ") + e.what();
}
```

#### 3. DBHandler (dbHandler.cpp)

**Responsibility**: SQLite database operations

```cpp
class DBHandler {
public:
    static bool databaseExists(const std::string& filename);
    static bool initializeDatabase(const std::string& filename);
    static bool saveDatabase(const std::string& filename, const SystemData& data);
    static bool loadDatabase(const std::string& filename, SystemData& data);

private:
    static bool createTables(sqlite3* db);
    static bool saveCustomers(sqlite3* db, const std::vector<Customer>& customers);
    static bool saveFlights(sqlite3* db, const std::vector<Flight>& flights);
    // ... other save methods
    
    static bool loadCustomers(sqlite3* db, std::vector<Customer>& customers);
    static bool loadFlights(sqlite3* db, std::vector<Flight>& flights);
    // ... other load methods
};
```

**Transaction Management**:
```cpp
sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
// ... multiple operations
if (success) {
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
} else {
    sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
}
```

**Prepared Statements**:
```cpp
const char* sql = "INSERT INTO customers (id, name, phone) VALUES (?, ?, ?);";
sqlite3_stmt* stmt;
sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
sqlite3_bind_int(stmt, 1, customer.getCustomerId());
sqlite3_bind_text(stmt, 2, customer.getName().c_str(), -1, SQLITE_TRANSIENT);
sqlite3_bind_text(stmt, 3, customer.getPhone().c_str(), -1, SQLITE_TRANSIENT);
sqlite3_step(stmt);
sqlite3_finalize(stmt);
```

---
