# Company Manager

A robust, console-based C application designed to streamline company personnel management, project tracking, and feature development.

## Overview
Company Manager provides a clean and efficient interface for administrators to oversee workforce operations. The system allows you to maintain a registry of workers, initiate and manage company-wide projects, and track specific development features within those projects using linked-list structures.

## Key Features
- **Workforce Administration**: Easily onboard new team members and manage their roles within the company.
- **Dynamic Project Tracking**: Create new projects and assign workers. The system dynamically manages the relationships between people and their assigned tasks.
- **Feature Management**: Manage individual project tasks ("features") using a linked-list implementation, allowing for flexible addition and removal of development goals.
- **Efficient Memory Management**: Built with dynamic memory allocation (`malloc`/`realloc`), including a comprehensive cleanup function to prevent memory leaks.
- **Input Validation**: Safeguards against invalid selections, duplicate names, and empty inputs to ensure data integrity.

## Getting Started

### Prerequisites
- A C compiler (e.g., `gcc` via MinGW-w64 on Windows, or standard `gcc` on Linux/macOS).

### Compilation
1. Ensure both `ex_5.c` and `ex_5.h` are in the same folder.
2. Open your terminal in that folder and run:
   ```bash
   gcc ex_5.c -o CompanyManager
