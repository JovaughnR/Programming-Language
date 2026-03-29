# CxLang Programming Language

<p align="center">
  <b>A fast, expressive, general-purpose programming language designed for scripting, education, and systems programming.</b>
</p>

---

## What is CX?

CX is a modern programming language with a clean, Python-like syntax that feels immediately familiar. It is designed to be easy to learn, fast to write, and powerful enough for real-world use — from quick scripts to full applications.

CX supports object-oriented programming, async/await concurrency, threading, modules, and a rich standard library — all out of the box.

---

## Features

- **Classes and inheritance** — full OOP with constructors, methods, and MRO
- **Async/await** — built-in concurrency with a thread pool
- **Threading** — native threads, mutexes, and condition variables
- **Rich standard library** — math, os, io, json, regex, http, socket, db, and more
- **F-strings** — string interpolation with `f"Hello {name}"`
- **Enums** — first-class enum support
- **Lambdas and closures** — full functional programming support
- **Exception handling** — try/catch/finally/throw
- **REPL** — interactive shell for quick experimentation

---

## Installation

### macOS (from source)

**Requirements:** `gcc`, `flex`, `bison`, `make`

```bash
git clone https://github.com/JovaughnR/Programming-Language.git
cd Programming-Language
make
sudo cp cxlang /usr/local/bin/cxlang
```

### Verify installation

```bash
cxlang --version
```

---

## Quick Start

Create a file `hello.cx`:

```cx
print("Hello, World!");
```

Run it:

```bash
cxlang hello.cx
```

---

## Language Tour

### Variables

```cx
name = "CX";
version = 1.0;
is_fast = true;
nothing = none;
```

### Functions

```cx
func add(x, y) {
    return x + y;
}

print(add(3, 4));  # 7
```

### Classes

```cx
class Animal {
    func __struct__(this, name, sound) {
        this.name = name;
        this.sound = sound;
    }

    func speak(this) {
        print(f"{this.name} says {this.sound}!");
    }
}

class Dog(Animal) {
    func __struct__(this, name) {
        Animal.__struct__(this, name, "woof");
    }
}

dog = Dog("Rex");
dog.speak();  # Rex says woof!
```

### Control Flow

```cx
for (i in range(5)) {
    if i % 2 == 0 {
        print(f"{i} is even");
    } else {
        print(f"{i} is odd");
    }
}
```

### Async / Await

```cx
async func fetch_data(url) {
    response = net.get(url);
    return response;
}

result = await fetch_data("https://api.example.com/data");
print(result);
```

### Threading

```cx
import thread;

func worker(n) {
    sum = 0;
    for i in range(n) {
        sum += i;
    }
    return sum;
}

t = thread.create(worker, [1000]);
result = thread.join(t);
print(result);  # 499500
```

### Exception Handling

```cx
try {
    x = 1 / 0;
} catch (ZeroDivisionError as e) {
    print(f"{e}");
} finally {
    print("Done");
}

```

### Enums

```cx
enum Color {
    RED,
    GREEN,
    BLUE
}

print(Color.RED);   # 0
print(Color.GREEN); # 1
print(Color.BLUE);  # 2
```

### Lambdas

```cx
double = lambda x : x * 2;
print(double(5));  # 10

nums = [1, 2, 3, 4, 5];
evens = filter(lambda x : x % 2 == 0, nums);
```

### Modules

```cx
import math;
import json;
import os;

print(math.sqrt(16));   # 4.0
print(math.pi);         # 3.14159...

data = json.parse('{"name": "cx", "version": 1}');
print(data["name"]);    # cx
print(data["version"]); # 1
```

---

## Running Modes

```bash
# Run a script
cxlang script.cx

# Start the REPL
cxlang

```

---

## Standard Library

| Module    | Description                          |
| --------- | ------------------------------------ |
| `math`    | Mathematical functions and constants |
| `os`      | Operating system interface           |
| `io`      | File I/O                             |
| `json`    | JSON parsing and serialization       |
| `regex`   | Regular expressions                  |
| `time`    | Time and date utilities              |
| `random`  | Random number generation             |
| `thread`  | Threading and synchronization        |
| `net`     | HTTP client                          |
| `socket`  | TCP/UDP sockets                      |
| `http`    | HTTP server                          |
| `db`      | SQLite database                      |
| `path`    | File path utilities                  |
| `sys`     | System information                   |
| `hash`    | Hashing functions                    |
| `csv`     | CSV parsing                          |
| `stats`   | Statistical functions                |
| `log`     | Logging                              |
| `env`     | Environment variables                |
| `args`    | Command-line arguments               |
| `decimal` | Arbitrary precision decimals         |

---

## VS Code Extension

Install the **CX Language** extension for syntax highlighting, snippets, and run support:

1. Open VS Code
2. Go to Extensions (`Ctrl+Shift+X`)
3. Search for **CX Language**
4. Click Install

Or install from the command line:

```bash
code --install-extension JovaughnRose.cxlang
```

---

## Contributing

Contributions are welcome. Please open an issue or pull request on GitHub.

```bash
git clone https://github.com/JovaughnR/Programming-Language.git
cd Programming-Language
make
```

---

## License

MIT License — Copyright (c) 2026 Jovaughn Rose

---

## Author

**Jovaughn Rose** — [@JovaughnR](https://github.com/JovaughnR)

> CX is a language built from scratch — lexer, parser, — by one developer from Jamaica. 🇯🇲
