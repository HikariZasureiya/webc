# WebC

## Description
WebC is a simple web server written in c that serves HTML pages. It listens for incoming HTTP requests and responds with the appropriate HTML content.

## Features
- Serves static HTML pages
- Supports basic routing
- Lightweight and easy to set up

## Installation
### 1. Clone the repository
```sh
git clone https://github.com/HikariZasureiya/webc.git
cd webc
```

### 2. Install Dependencies

to get started you first need to install llhttp http header parser from
https://github.com/nodejs/llhttp if you don't have it

when it's done, use

```sh
make all
```

## Usage
### Start the Server
```sh
./serverout
```

### Access the Web Server
Open a browser and visit:
```
http://localhost:8000
```
