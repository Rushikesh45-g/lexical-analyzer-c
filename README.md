# 🔍 Lexical Analyzer in C

## 📌 Overview
This project implements a simple Lexical Analyzer in C that scans source code and identifies different tokens such as keywords, identifiers, operators, constants, and special symbols. It reads input from a file and processes it character by character.

---

## 🚀 Features
- Identifies keywords, identifiers, operators, constants
- Handles special symbols and delimiters
- Reads source code from file input
- Simple and efficient implementation in C

---

## 🛠️ Technologies Used
- C Programming
- File Handling
- String Manipulation
- Pointers and Arrays
- Character Processing (ctype.h)

---

## ⚙️ How It Works
The program reads the input source code file and scans it sequentially. It classifies each token based on predefined rules and displays the identified tokens along with their types.

---

## ▶️ How to Run
```bash
gcc lexical_analyzer.c -o analyzer
./analyzer input.txt
