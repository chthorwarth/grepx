## Coding Assistant Usage

We used the following coding assistants during development: **ChatGPT** and **Google Gemini**.

We did **not** use full autonomous coding tools such as *Claude Code* or *Codex*. The overall **software architecture, design decisions, and implementation strategy were created entirely by us**.

The coding assistants were used only in a **supporting role**, mainly for:
- **Generating small, isolated code snippets** within a clearly defined context
- **Producing unit and integration tests** that were written **independently of our implementation** to help detect bugs and verify correctness
- **Assisting with refactoring, documentation, explanations, and general knowledge or idea exploration**

All generated code was reviewed, adapted, and integrated manually.

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
   ./bin/grepx "Coding Assistant" README.md
5. General usage follows the standard Linux grep syntax:
   ```bash
   grepx [OPTIONS] PATTERN [FILE...]
   
## How to Run Tests

1. Make sure the project is built
2. Run all tests
   ```bash
   make test
3. The test suite will execute unit and integration tests