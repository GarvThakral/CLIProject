# GarvCli — tiny cross-platform developer CLI

_A lightweight developer toolbox CLI written in C++ (single-binary-ish). Useful shortcuts for creating projects, running quick tasks, and automating little dev chores._

This README explains how to build, install, and **use** GarvCli on Linux and Windows. It also documents commands, dependencies, and important safety notes so people can use it well when you distribute it (for example via an S3 link).

---

## Table of contents
- [Quick TL;DR](#quick-tldr)
- [Requirements](#requirements)
- [Build (Linux)](#build-linux)
- [Build (Windows)](#build-windows)
- [Download & install (example S3 distribution)](#download--install-example-s3-distribution)
- [Run it](#run-it)
- [Commands (quick reference + examples)](#commands-quick-reference--examples)
- [Important notes & caveats](#important-notes--caveats)
- [Troubleshooting](#troubleshooting)
- [Security & distribution guidance](#security--distribution-guidance)
- [Contributing & development tips](#contributing--development-tips)
- [License & contact](#license--contact)

---

## Quick TL;DR
1. Build with a C++17 compiler (`g++ -std=c++17`) or MSVC/MinGW.  
2. Run the produced binary (`./gex` on Linux, `gex.exe` on Windows).  
3. Type commands (e.g. `greet Alice`, `banao myfile.txt`, `dataset hf imdb`) at the prompt.

---

## Requirements
Minimum tooling you should have available on the host where you run GarvCli:

- A C++ compiler supporting **C++17** (`g++` 7+ recommended, or Visual Studio 2019+ for MSVC).
- `curl`, `npm`, `node`, `python`, `conda`, `g++` — **optional** but required for some commands (see below).
- C++ standard library with `<filesystem>` support.

> **Note:** Commands that call external programs assume those programs are on `PATH`. The CLI will attempt to run them but does not install or validate them automatically.

---

## Build (Linux)
From the project root (files: `cliproject.cpp`, `system_message.cpp`, `system_message.h`):

```bash
# simple one-liner:
g++ -std=c++17 cliproject.cpp system_message.cpp -o gex

# with optimizations and warnings:
g++ -std=c++17 -O2 -Wall cliproject.cpp system_message.cpp -o gex
```

**Notes**
- Static linking (`-static`) can be fragile on modern Linux and often unnecessary. Avoid unless you know what you need.  
- If you see *undefined reference to `SystemMessage`*, you probably forgot to link `system_message.cpp` — include it on the command line.

---

## Build (Windows)
Build with MinGW/MSYS or Visual Studio.

**MinGW / MSYS2 example:**
```bash
g++ -std=c++17 cliproject.cpp system_message.cpp -o gex.exe
```

**Visual Studio (Developer Command Prompt):**
```bat
cl /EHsc /std:c++17 cliproject.cpp system_message.cpp /Fe:gex.exe
```

**Notes**
- If `<filesystem>` errors appear with MSVC, update to a newer Visual Studio toolset.  
- The code contains `#ifdef _WIN32` where needed so it should compile with reasonable toolchains.

---

## Download & install (example S3 distribution)
If you distribute a build through S3, include install steps for users to fetch and run the binary.

**Linux example:**
```bash
curl -L -o garvcli-linux.tar.gz "https://bucket-main-garv.s3.us-east-1.amazonaws.com/dex"
tar -xzf garvcli-linux.tar.gz
cd garvcli
./gex
```

**Windows PowerShell example:**
```powershell
Invoke-WebRequest -Uri "https://your-bucket.s3.amazonaws.com/garvcli-win-x64.zip" -OutFile garvcli.zip
Expand-Archive garvcli.zip -DestinationPath .
.\gex.exe
```

_Distribute platform-specific builds and label them clearly._

---

## Run it
Start the CLI interactively:

```bash
# Linux / macOS
./gex

# Windows
gex.exe
```

You’ll see the prompt printed by `SystemMessage` (e.g. `GarvCli > `). Type a command and press Enter.

You can also run a single command non-interactively:

```bash
./gex greet Alice
```

---

## Commands (quick reference + examples)

**General**
- `greet <name>` — Greets the person.  
  Example: `greet Bob` → `GarvCli > Hello Bob!`

- `banao <fileName>` — Create a file.  
  Example: `banao todo.txt`

- `hatao <fileName>` — Remove a file.  
  Example: `hatao todo.txt`

- `bolo <text...>` — Echo text.  
  Example: `bolo hello world`

- `clear` — Clear screen (platform-specific).

- `datetime` — Prints current date/time.

- `exit` — Exit the CLI.

**Project scaffolding**
- `rinit [ -t ]` — Create a Vite + React project; `-t` adds Tailwind. (requires `npm`/`npx`)
- `einit` — Create an Express backend; interactive TypeScript option.
- `create <name...>` — Create wrapper scripts in a local `./bin` folder (`.bat` + POSIX wrapper).

**Git / packaging**
- `gpush <commit message tokens...>` — `git add . && git commit -m "<message>" && git push`

**Developer helpers**
- `nd` — Run `npm run dev`.  
- `runCFile <name>` — Compile + run C/C++: `g++ name.cpp -o name && ./name` (platform dependent).  
- `compileEXE` — Build a gex binary with `g++` (platform dependent).

**Maintenance**
- `clean` — Removes `node_modules`, `dist`, `.vite`, etc.  
- `backup` — Interactively copy project contents to a new folder.  
- `deps-check` — `npm outdated` / `pip list --outdated` depending on project.  
- `updateDeps` — `npm update` or pip upgrader flow.  
- `ml-init <env_name> <python_version> [packages ...]` — Convenience wrapper around `conda create` + installs.  
- `dataset kaggle <id>` — Download Kaggle dataset via `curl` (may need auth).  
- `dataset hf <id>` — Use `datasets` Python package to load a HuggingFace dataset: `python -c 'from datasets import load_dataset; ds = load_dataset("<id>")'`

**Utilities**
- `benchmark <command ...>` — Measure wall-clock execution time for a command.

---

## Important notes & caveats (READ THIS)
- **Shell execution & security** — Many commands build shell commands and pass them to `system()`. If the user supplies malicious input, it can execute arbitrary commands. **Do not run untrusted input.** Consider sandboxing or validating inputs before invoking shell commands.
- **External tool dependencies** — `npm`, `conda`, `curl`, `python`, `g++` must be available for related features to work. The CLI won't install them.
- **Kaggle downloads** — Kaggle often requires authentication. The `curl` approach may not work for private datasets — prefer using the official `kaggle` CLI with proper credentials.
- **Filesystem & permissions** — Creating/removing files can fail for permission reasons. Errors are printed but the CLI does not guarantee recovery.
- **Platform differences** — The code tries to be portable (`./bin` wrappers, `#ifdef _WIN32` branches). `$HOME` expansion is handled by the invoked shell on POSIX; Windows uses different environment variables (the code uses `std::filesystem` where possible).
- **Static linking warnings** — Avoid `-static` unless you know your target environment; static binaries can be brittle.
- **Buffer sizes and quoting** — `snprintf` is used in some places but extremely long or untrusted arguments may still cause problems. Prefer `std::filesystem` APIs for path creation and avoid concatenating shell commands when possible.
- **Mixing `cin >>` and `getline()`** — This can leave stray newlines in the input buffer; the code attempts to handle common cases but be aware of interactive quirks.

---

## Troubleshooting (common errors)
- **undefined reference to `SystemMessage`** → You didn't link `system_message.cpp`. Re-run compilation including `system_message.cpp`.
- **`<filesystem>` errors on MSVC** → Update Visual Studio or toolset to one that supports `<filesystem>`.
- **`mkdir` fails or path contains spaces** → Use `std::filesystem::create_directories()` for robustness, or quote paths and use `mkdir -p` on POSIX.
- **External commands do nothing** → Ensure `npm`, `python`, `conda`, `curl`, `g++` are installed and on PATH.
- **Kaggle download fails** → Authenticate using Kaggle CLI or set up the proper tokens—direct `curl` is not always sufficient.

---

## Security & distribution guidance
- Provide checksums (SHA256) and optionally signatures for distributed binaries on S3. That helps users verify integrity.
- Warn users about running the binary with untrusted inputs. Consider shipping a “safe” build that disables shell-launching features if you expect untrusted usage.
- Use secure S3 bucket policies and signed URLs when distributing binaries to limit unintended access.

---

## Contributing & development tips
- Prefer `std::filesystem` for file operations to avoid shell quirks. It’s portable and safer.
- Replace `system()` with platform-native process APIs (`posix_spawn`, `CreateProcess`) for better control and to avoid shell escaping issues.
- Add checks for external tool availability, e.g., test `which npm` or use platform-specific detection before attempting commands.
- Add a `--help` flag and usage printing for non-interactive usage.
- Consider adding unit tests for parsing logic (e.g., `parseInput`) and for any pure C++ logic.

---

## License
MIT — include a `LICENSE` file if you want to publish under MIT.

---

## Contact / Attribution
Authored by Garv. If you distribute via S3, include this README in the release artifact and publish SHA256 checksums alongside the binary.

---

_Use responsibly. This tool is handy — and, like a soldering iron, it will singe you if you poke the wrong thing._
