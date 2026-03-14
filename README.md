# grepx – Unix Command-Line Text Filter

## About the Project

This project was developed as a team effort during the System-Level Programming module at the Technical University of Applied Sciences Würzburg-Schweinfurt (THWS). It’s a **Unix command-line tool** inspired by `grep`, designed to filter text streams and files line-by-line.

### Motivation

We created grepx to explore Unix philosophy in practice: *“Do one thing and do it right”*. During development, we learned to identify the core functionality of grep (line-based text filtering) and clearly separate it from auxiliary tasks. We gained insight into the internal workings and design concepts of Unix command-line tools, such as order-independent option parsing, pipeline integration, and standard Unix exit code conventions.

## Key Features

* **grep-inspired filtering:** Line-by-line text matching using POSIX regular expressions.  
* **Pipeline-compatible:** Seamlessly works with other Unix tools like `cat` or `find`.  
* **Order-independent option parsing:** Options can be specified in any order, following Unix conventions.  
* **Multithreaded file processing:** Parallelizes file handling for improved performance.  
* **Unix exit codes:** Returns standard codes for match, no match, and errors.  
* **Modular architecture:** Clear separation of parsing, validation, and search logic for easy testing and extensibility.

## Tech Stack

* **Language:** C  
* **Build system:** Make  
* **Multithreading:** pthreads  
* **Testing:** Unit and integration tests with I/O redirection  

## Presentation

The project presentation slides are available here:  
- [grepx Presentation Slides](presentation/grepxPräsentation.pdf)

## How to start

1. Clone the project from GitLab
   ```bash
   git clone <your-repository-url>
2. Change into the project directory
    ```bash
   cd grepx
3. Build the project
   ```bash
   make all
4. Run the grepx binary, for example
   ```bash
   ./bin/grepx "Tech Stack" README.md
5. General usage follows the standard Linux grep syntax:
   ```bash
   grepx [OPTIONS] PATTERN [FILE...]
   
## How to Run Tests

1. Make sure the project is built
2. Run all tests
   ```bash
   make test
3. The test suite will execute unit and integration tests

## Coding Assistant Usage

We used the following coding assistants during development: **ChatGPT** and **Google Gemini**.

We did **not** use full autonomous coding tools such as *Claude Code* or *Codex*. The overall **software architecture, design decisions, and implementation strategy were created entirely by us**.

The coding assistants were used only in a **supporting role**, mainly for:
- **Generating small, isolated code snippets** within a clearly defined context
- **Producing unit and integration tests** that were written **independently of our implementation** to help detect bugs and verify correctness
- **Assisting with refactoring, documentation, explanations, and general knowledge or idea exploration**

All generated code was reviewed, adapted, and integrated manually.

