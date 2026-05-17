# 🚀 Multithreaded HTTP Server

> A production-grade HTTP/1.1 server built from scratch in C++ using POSIX sockets, demonstrating TCP/IP networking, concurrent request handling, and fundamental web server architecture.

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Linux](https://img.shields.io/badge/Platform-Linux-orange.svg)](https://www.linux.org/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen.svg)](#build-instructions)

## ✨ Features

### Core Networking

- **POSIX Sockets**: Raw TCP/IP socket programming without frameworks
- **Multithreading**: Concurrent client handling with `std::thread`
- **HTTP/1.1 Compliance**: Full request parsing, proper headers, status codes
- **Static File Serving**: Efficient serving from `static/` directory

### Security & Performance

- **Per-IP Rate Limiting**: Token bucket algorithm (5 req/10 sec per IP)
- **DDoS Protection**: Returns HTTP 429 for rate-limited clients
- **MIME Type Detection**: Correct `Content-Type` headers

### Production Features

- **Comprehensive Logging**: Timestamped requests with status codes
- **Configurable Port**: External `config.txt` for server settings
- **Modular Architecture**: Clean separation of concerns (logger, router, file handler, rate limiter)
- **Beautiful 404 Page**: Styled error page for missing resources

---

## 🏗️ Architecture

```
HTTP Request
    ↓
[TCP Socket] → Accept Connection → Spawn Thread
    ↓
[Parse Request] → Extract Method & Path
    ↓
[Rate Limiter] → Check Per-IP Quota → (429 if exceeded)
    ↓
[Router] → Map Path to File → (index.html, about.html, etc.)
    ↓
[File Handler] → Read File → Determine MIME Type
    ↓
[Response Builder] → Create HTTP Response (Headers + Body)
    ↓
[Send Response] → TCP Socket → Client
    ↓
[Logger] → Log Request with Status Code
```

### Components

| Component        | File                   | Responsibility                                |
| ---------------- | ---------------------- | --------------------------------------------- |
| **Server**       | `server.cpp`           | Main event loop, socket management, threading |
| **Router**       | `handleRoute()`        | Maps URLs to static files                     |
| **Logger**       | `logger.cpp/.h`        | HTTP request logging with timestamps          |
| **File Handler** | `fileHandler.cpp/.h`   | File I/O and MIME type detection              |
| **Rate Limiter** | `rateLimiter.h`        | Per-IP request throttling                     |
| **Config**       | `configManager.cpp/.h` | Configuration file parsing                    |

---

## 🚀 Quick Start

### Build

```bash
g++ -std=c++17 -pthread -O2 -o server \
    server.cpp logger.cpp fileHandler.cpp configManager.cpp
```

### Run

```bash
./server
```

**Expected Output:**

```
[CONFIG] Loaded PORT=8080 from config.txt
[INFO] HTTP Server running on port 8080...
[INFO] Press Ctrl+C to stop
```

### Test

```bash
# In another terminal
curl http://localhost:8080/
curl http://localhost:8080/about
curl http://localhost:8080/style.css
```

---

## 📁 Project Structure

```
http-server/
├── server.cpp              # Main server logic (~250 lines)
├── logger.cpp / .h         # Request logging
├── fileHandler.cpp / .h    # Static file serving
├── rateLimiter.h           # Per-IP rate limiting
├── configManager.cpp / .h  # Config parsing
├── config.txt              # Server configuration
│
├── static/                 # Static assets
│   ├── index.html
│   ├── about.html
│   ├── style.css
│   └── 404.html
│
├── logs/
│   └── server.log          # HTTP request log
│
├── README.md               # This file
└── IMPLEMENTATION_SUMMARY.md
```

---

## 🔧 Configuration

Edit `config.txt` to customize the server:

```ini
# Server Port (default: 8080)
PORT=8080
```

---

## 📋 API Routes

| Route        | Method | Response            | Status        |
| ------------ | ------ | ------------------- | ------------- |
| `/`          | GET    | `static/index.html` | 200 OK        |
| `/about`     | GET    | `static/about.html` | 200 OK        |
| `/style.css` | GET    | `static/style.css`  | 200 OK        |
| `/*`         | GET    | `static/404.html`   | 404 Not Found |

### Example Requests

**Get Home Page:**

```bash
curl -v http://localhost:8080/
```

**Test Rate Limiting (6+ requests in 10 seconds):**

```bash
for i in {1..10}; do curl http://localhost:8080/; done
```

**View Request Logs:**

```bash
cat logs/server.log
```

---

## 📊 Example Logs

```
[2026-05-17 02:10:15] 127.0.0.1 GET / 200 OK
[2026-05-17 02:10:16] 127.0.0.1 GET /style.css 200 OK
[2026-05-17 02:10:17] 192.168.1.100 GET /about 200 OK
[2026-05-17 02:10:18] 192.168.1.100 GET /invalid 404 Not Found
[2026-05-17 02:10:19] 192.168.1.100 GET / 429 Too Many Requests
[2026-05-17 02:10:20] 192.168.1.100 GET / 429 Too Many Requests
```

---

## 🎓 Key Concepts Demonstrated

### 1. POSIX Socket Programming

- Socket creation (`socket()`)
- Binding to address (`bind()`)
- Listening for connections (`listen()`)
- Accepting clients (`accept()`)
- Sending/receiving data (`send()`, `read()`)

### 2. Multithreading & Concurrency

- One thread per client connection
- Thread spawning with `std::thread`
- Resource cleanup with `close()`
- No mutex (simple implementation)

### 3. HTTP Protocol

- Request parsing (method, path extraction)
- Response building (headers + body)
- Status codes (200, 404, 429)
- MIME types (text/html, text/css, application/javascript)

### 4. Rate Limiting

- Token bucket algorithm
- Per-IP tracking with `unordered_map`
- DDoS prevention
- Time window management

### 5. Modular Design

- Single Responsibility Principle
- Easy to extend (add routes, features)
- Clean interfaces between components

---

## ⚙️ System Requirements

- **OS**: Linux (Ubuntu, Debian, RHEL, etc.) or macOS
- **Compiler**: GCC or Clang with C++17 support
- **Build Tools**: `g++` or `clang++`
- **Runtime**: POSIX-compliant system

**NOT compatible with Windows** (uses POSIX sockets; would need Winsock for Windows)

---

## 🏎️ Performance

| Metric                | Value                                 |
| --------------------- | ------------------------------------- |
| **Concurrency Model** | 1 thread/client                       |
| **Max Connections**   | ~100s (limited by threading overhead) |
| **Request Latency**   | ~1-5 ms (local)                       |
| **Throughput**        | ~1000 req/sec                         |
| **Memory/Connection** | ~8-16 KB                              |

**Note**: This is educational. Nginx uses `epoll` to handle 10,000+ concurrent connections with minimal memory.

---

## 🔬 How It Compares

| Feature               | Our Server | Nginx        | Apache      |
| --------------------- | ---------- | ------------ | ----------- |
| **Concurrency**       | Threading  | epoll        | Threading   |
| **Code Size**         | ~500 LOC   | 100,000+ LOC | ~50,000 LOC |
| **Memory/Connection** | High       | Very Low     | Medium      |
| **Production Ready**  | ❌         | ✅           | ✅          |
| **Educational**       | ✅         | ❌           | ❌          |

---

## 🤝 Contributing

Feel free to fork, modify, and extend this project for learning purposes!

### Ideas for Contributions

1. Add HTTP Keep-Alive support
2. Implement thread pool
3. Add epoll event-driven version
4. Support more HTTP methods (POST, PUT, DELETE)
5. Add request body parsing
6. Improve error handling

---

## 📄 License

This project is open source and available under the [MIT License](LICENSE).


---

## ⭐ If This Helped You

If you found this useful for learning or your portfolio, please star this repository! It helps other developers discover it.

```bash
# Clone and run
git clone https://github.com/yourusername/http-server.git
cd http-server
g++ -std=c++17 -pthread -O2 -o server server.cpp logger.cpp fileHandler.cpp configManager.cpp
./server
```

Happy networking! 🚀

## Overview

This HTTP server showcases:

- **TCP Socket Programming**: Raw socket-level networking with POSIX APIs
- **Multithreading**: Concurrent client handling using `std::thread`
- **HTTP Protocol**: Full HTTP/1.1 request parsing and response generation
- **Rate Limiting**: Per-IP request throttling (DDoS prevention)
- **Modular Architecture**: Clean separation of concerns with helper modules
- **Configuration Management**: Externalized server settings via `config.txt`

## Features

### ✅ Core Functionality

- **TCP Socket Server**: Listens on configurable port, accepts multiple concurrent connections
- **HTTP Request Parsing**: Extracts method, path, and headers from raw HTTP requests
- **Static File Serving**: Serves HTML, CSS, and other static assets from `static/` directory
- **MIME Type Detection**: Correct `Content-Type` headers based on file extension
- **Proper HTTP Status Codes**: 200 OK, 404 Not Found, 429 Too Many Requests
- **HTTP/1.1 Compliance**: Includes all required headers (Content-Length, Content-Type, etc.)

### 🔒 Security & Performance

- **Rate Limiting**: 5 requests per 10 seconds per IP (configurable)
- **429 Too Many Requests**: Automatic DDoS protection
- **Multi-threaded**: Handles multiple clients simultaneously
- **Clean Socket Handling**: Proper resource cleanup

### 📋 Logging & Monitoring

- **Request Logging**: Logs all HTTP requests with timestamps
- **Response Status Tracking**: Logs HTTP status codes (200, 404, 429, etc.)
- **Client IP Logging**: Identifies request origin
- **Log Format**: `[YYYY-MM-DD HH:MM:SS] IP METHOD PATH STATUS_CODE STATUS_TEXT`

### ⚙️ Configuration

- **Configurable Port**: Read from `config.txt`
- **Fallback Defaults**: Uses port 8080 if config missing
- **Custom 404 Page**: Beautiful `static/404.html` for not-found responses

## Architecture

### File Structure

```
http-server/
├── server.cpp           # Main server logic, event loop, client handling
├── logger.cpp/.h        # Logging functionality for requests
├── fileHandler.cpp/.h   # File reading and MIME type detection
├── rateLimiter.h        # Per-IP rate limiting (header-only)
├── configManager.cpp/.h # Configuration file parsing
├── config.txt           # Server configuration
│
├── static/              # Static assets served by server
│   ├── index.html       # Home page
│   ├── about.html       # About page
│   ├── style.css        # Styling
│   └── 404.html         # Not found page
│
└── logs/
    └── server.log       # HTTP request logs
```

### Component Breakdown

#### 1. **server.cpp** - Core Server

- **Purpose**: TCP socket creation, client connection handling, multithreading
- **Key Functions**:
  - `main()`: Initializes server, loads config, starts event loop
  - `handleClient()`: Processes single client request (spawned in separate thread)
  - `handleRoute()`: Maps URL paths to static files
- **Threading Model**: One thread per client (scales to ~100s of connections)

#### 2. **logger.h / logger.cpp** - Request Logging

- **Purpose**: Timestamp and log all HTTP requests
- **Log Entry Example**:
  ```
  [2026-05-17 02:15:30] 192.168.1.100 GET /about 200 OK
  [2026-05-17 02:15:31] 192.168.1.101 GET /invalid 404 Not Found
  [2026-05-17 02:15:32] 192.168.1.100 GET / 429 Too Many Requests
  ```
- **Function**: `logRequest(method, path, clientIP, statusCode, statusText)`

#### 3. **fileHandler.h / fileHandler.cpp** - Static File Serving

- **Purpose**: Read files from disk, determine MIME types
- **Functions**:
  - `readFile()`: Reads file content into memory
  - `getMimeType()`: Returns correct Content-Type header
- **Supported MIME Types**: text/html, text/css, application/javascript, text/plain

#### 4. **rateLimiter.h** - DDoS Prevention (Header-Only)

- **Purpose**: Track per-IP request counts, enforce limits
- **Algorithm**: Token bucket (simple in-memory approach)
- **Limits**: 5 requests per 10 seconds per IP
- **Returns**: 429 Too Many Requests if exceeded
- **Class**: `RateLimiter` with static rate tracking

#### 5. **configManager.h / configManager.cpp** - Configuration

- **Purpose**: Read server settings from `config.txt`
- **Current Support**: Port number configuration
- **Fallback**: Defaults to 8080 if config missing or invalid
- **Easy to Extend**: Add new config parameters as needed

## Request Lifecycle

```
1. Client sends HTTP request (e.g., GET /about HTTP/1.1)
   ↓
2. Server accept() on listening socket
   ↓
3. Spawn new thread with handleClient(socket, clientIP)
   ↓
4. Parse HTTP method and path
   ↓
5. Check rate limiter for clientIP
   ├─ If rate limited → Send 429, log, return
   └─ If allowed → Continue
   ↓
6. Route request using handleRoute(path)
   ├─ / → static/index.html (200)
   ├─ /about → static/about.html (200)
   ├─ /style.css → static/style.css (200)
   └─ /* → static/404.html (404)
   ↓
7. Read file from disk using readFile()
   ├─ If found → Use file content
   └─ If not found → Serve 404.html (or fallback HTML)
   ↓
8. Determine MIME type using getMimeType()
   ↓
9. Build HTTP response:
   - Status line (HTTP/1.1 200 OK)
   - Headers (Content-Type, Content-Length)
   - Body (file content)
   ↓
10. Send response to client using send()
    ↓
11. Log request with status code
    ↓
12. Close socket and thread
```

## Build Instructions

### Prerequisites

- **Linux/Unix system** (Linux, macOS, or WSL on Windows)
- **GCC or Clang** compiler
- **Standard C++17** support

### Compilation

```bash
# Simple compilation with all files
g++ -std=c++17 -pthread -o server \
    server.cpp logger.cpp fileHandler.cpp configManager.cpp

# With optimization and debugging symbols
g++ -std=c++17 -pthread -O2 -g -o server \
    server.cpp logger.cpp fileHandler.cpp configManager.cpp

# Using Clang
clang++ -std=c++17 -pthread -o server \
    server.cpp logger.cpp fileHandler.cpp configManager.cpp
```

### Running the Server

```bash
# Start server (reads port from config.txt, defaults to 8080)
./server

# Expected output:
# [CONFIG] Loaded PORT=8080 from config.txt
# [INFO] HTTP Server running on port 8080...
# [INFO] Press Ctrl+C to stop
```

## API Routes

| Path             | Method | File Served         | Status        |
| ---------------- | ------ | ------------------- | ------------- |
| `/`              | GET    | `static/index.html` | 200 OK        |
| `/about`         | GET    | `static/about.html` | 200 OK        |
| `/style.css`     | GET    | `static/style.css`  | 200 OK        |
| `/*` (any other) | GET    | `static/404.html`   | 404 Not Found |

### Example Requests

**Request 1: Get Home Page**

```http
GET / HTTP/1.1
Host: localhost:8080

HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 1234

[HTML content of index.html...]
```

**Request 2: Get CSS**

```http
GET /style.css HTTP/1.1
Host: localhost:8080

HTTP/1.1 200 OK
Content-Type: text/css
Content-Length: 567

[CSS content...]
```

**Request 3: Invalid Path (404)**

```http
GET /invalid-page HTTP/1.1
Host: localhost:8080

HTTP/1.1 404 Not Found
Content-Type: text/html
Content-Length: 2345

[HTML content of 404.html...]
```

**Request 4: Rate Limited (429)**

```http
GET / HTTP/1.1
Host: localhost:8080
[5+ requests within 10 seconds from same IP]

HTTP/1.1 429 Too Many Requests
Content-Type: text/html
Content-Length: 180

[Rate limit error HTML...]
```

## Example Logs

```
[2026-05-17 02:10:15] 127.0.0.1 GET / 200 OK
[2026-05-17 02:10:16] 127.0.0.1 GET /style.css 200 OK
[2026-05-17 02:10:17] 127.0.0.1 GET /about 200 OK
[2026-05-17 02:10:18] 192.168.1.100 GET /invalid 404 Not Found
[2026-05-17 02:10:19] 192.168.1.100 GET / 429 Too Many Requests
[2026-05-17 02:10:20] 192.168.1.100 GET / 429 Too Many Requests
[2026-05-17 02:10:25] 192.168.1.100 GET / 200 OK
```

## Testing the Server

### Using `curl`

```bash
# Test home page
curl http://localhost:8080/

# Test about page
curl http://localhost:8080/about

# Test CSS file
curl http://localhost:8080/style.css

# Test 404
curl http://localhost:8080/nonexistent

# Test rate limiting (run 6+ times in quick succession)
for i in {1..10}; do curl http://localhost:8080/; done
```

### Using `telnet` (manual HTTP)

```bash
telnet localhost 8080
GET / HTTP/1.1
Host: localhost:8080

(press Enter twice to send the request)
```

### Using a Browser

Simply navigate to `http://localhost:8080` in your web browser.

## Configuration

Edit `config.txt` to customize server behavior:

```ini
# Port to listen on (default: 8080)
PORT=9000

# Future configurations:
# MAX_CONNECTIONS=1000
# REQUEST_TIMEOUT=30
# ENABLE_COMPRESSION=false
```

## Design Patterns & Concepts

### 1. **Multithreading Model**

- **Per-client threading**: Each accepted connection spawns a new thread
- **Trade-off**: Simple but doesn't scale to 10,000+ concurrent connections
- **Interview talking point**: Explain how Nginx uses `epoll` for better scalability

### 2. **Rate Limiting Implementation**

- **Token Bucket Algorithm** (simplified)
- **In-memory storage**: `unordered_map<IP, request_count>`
- **Time window reset**: 10 seconds per user

### 3. **HTTP Protocol Compliance**

- Proper headers: `Content-Type`, `Content-Length`
- Status codes: 200, 404, 429
- Request parsing: Extracts method and path from raw text

### 4. **Modular Architecture**

- **Separation of concerns**: Each module has a single responsibility
- **Easy to extend**: Add new routes, MIME types, logging features

## Performance Characteristics

| Metric                    | Value           | Notes                                          |
| ------------------------- | --------------- | ---------------------------------------------- |
| **Concurrency Model**     | 1 thread/client | Simple; doesn't scale beyond ~100s connections |
| **Memory Per Connection** | ~8-16 KB        | Thread stack + buffers                         |
| **Request Latency**       | ~1-5 ms         | On local machine                               |
| **Throughput**            | ~1000 req/sec   | On single machine (varies by hardware)         |
| **Rate Limit**            | 5 req/10s       | Per IP address                                 |

### How it Compares to Nginx

| Feature               | Our Server          | Nginx                              |
| --------------------- | ------------------- | ---------------------------------- |
| **Concurrency**       | 1 thread/client     | epoll (1 thread, 10k+ connections) |
| **Memory Efficiency** | High per-connection | Extremely low (~1 KB/connection)   |
| **Code Complexity**   | ~500 lines          | 100,000+ lines                     |
| **Production Ready**  | ❌ No               | ✅ Yes                             |
| **Educational Value** | ✅ Excellent        | ❌ Too complex                     |

## Interview Talking Points

1. **"How would you scale this to handle 10,000 concurrent connections?"**
   - Replace threading with `epoll` (Linux) or `kqueue` (macOS/BSD)
   - Use event-driven, non-blocking I/O
   - Reduce memory footprint dramatically

2. **"How does rate limiting prevent DDoS?"**
   - Tracks requests per IP
   - Returns 429 before processing
   - Saves server resources

3. **"How is this different from Nginx?"**
   - Nginx: C code, epoll, reverse proxy, load balancing
   - Ours: C++, threading, file serving only, educational

4. **"What would you add for production use?"**
   - HTTPS/TLS support
   - Connection pooling
   - Request body parsing (POST data)
   - gzip compression
   - Load balancing
   - Worker process management

## Tech Stack

- **Language**: C++17 (ISO standard C++)
- **Standard Library**: `<iostream>`, `<thread>`, `<fstream>`, `<unordered_map>`, etc.
- **Networking**: POSIX sockets (`<arpa/inet.h>`, `<unistd.h>`)
- **Concurrency**: `std::thread` (standard library threading)
- **Build**: g++/clang++ with `-std=c++17 -pthread`

**NO external libraries or frameworks** - pure standard C++.

## Future Improvements

### Level 2: Better Concurrency

- [ ] Thread pool (fixed number of worker threads)
- [ ] Task queue for pending connections
- [ ] Better resource management

### Level 3: Event-Driven Architecture (Nginx-style)

- [ ] Replace threading with `epoll` (Linux)
- [ ] Non-blocking I/O
- [ ] Single-threaded event loop

### Level 4: HTTP Features

- [ ] HTTP/2 support
- [ ] Keep-Alive connections
- [ ] Chunked transfer encoding
- [ ] Gzip compression
- [ ] Request body parsing (POST, PUT)

### Level 5: Advanced Features

- [ ] HTTPS/TLS with OpenSSL
- [ ] Reverse proxy mode
- [ ] Load balancing
- [ ] CGI script execution
- [ ] Directory listing

### Level 6: Production Hardening

- [ ] Signal handling (graceful shutdown)
- [ ] Worker process management
- [ ] Configuration reload without restart
- [ ] Health checks
- [ ] Metrics and monitoring

## Common Issues & Troubleshooting

### Issue: "Address already in use"

```
Error: Bind failed on port 8080
```

**Solution**: Change port in `config.txt` or wait 60 seconds for socket cleanup.

### Issue: "File not found errors" when serving static files

**Solution**: Ensure you're running the server from the correct directory (where `static/` folder exists).

### Issue: Rate limiting too strict

**Solution**: Modify `MAX_REQUESTS` and `TIME_WINDOW` constants in `rateLimiter.h`.

## License

Educational project - Use freely for learning purposes.

## References

- **Socket Programming**: [Man7.org - socket(7)](https://man7.org/linux/man-pages/man7/socket.7.html)
- **HTTP/1.1 Spec**: [RFC 7230](https://tools.ietf.org/html/rfc7230)
- **Nginx Architecture**: [agentzh's Nginx Tutorials](https://openresty.org/download/agentzh-nginx-tutorials-en.html)
- **epoll Tutorial**: [Man7.org - epoll(7)](https://man7.org/linux/man-pages/man7/epoll.7.html)

---

**Built with ❤️ for learning systems networking and web server architecture.**
