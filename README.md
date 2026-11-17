# Library Simulator

A console program in C that runs a small library: register books and users, borrow and return books,
search by title and list everything. The data lives in two text files, so every change is still there
the next time you open the program.

## What it does

| Option | What happens |
|---|---|
| 1. Register book | Asks for the title, author and year, and adds the book to `books.txt`. |
| 2. Register new user | Asks for a name and gives the user a new ID, saved in `users.txt`. |
| 3. Borrow a book | Asks for your ID and a title. Each user can have one book at a time. |
| 4. Return a book | Asks for your ID and the title of the book you have. |
| 5. Search for a book | Finds a book by title and says whether it is available. |
| 6. List the books | Shows every book and the ID of the user who has it. |
| 7. List the users | Shows every user and the book they borrowed. |

Option 0 closes the program.

## How to run

On Windows, double-click `library_simulator.exe`, or run it from a terminal opened in this folder.

To build it yourself with GCC:

```
gcc main.c functions.c -o library_simulator
```

Run the program from this folder: it reads `books.txt` and `users.txt` from the current directory. The
menu clears the screen with `system("cls")`, which is a Windows command, so on Linux and macOS the screen
is not cleared between menus.

## Data files

Each line is one record, with the fields separated by commas.

| File | Fields | Example |
|---|---|---|
| `books.txt` | title, author, year, ID of the user who has the book (`0` means available) | `Pride and Prejudice,Jane Austen,1813,0` |
| `users.txt` | name, ID | `Hermione Granger,1001` |

The repository comes with 24 books and 20 users as sample data. Titles and names cannot contain commas.
`books.txt` is saved again after every loan and every return.

## How the code is organized

| File | Contents |
|---|---|
| `main.c` | Loads the two files, opens the menu and frees the memory at the end |
| `functions.c` | The menu, reading and saving the files, and every library operation |
| `header.h` | The `Book` and `User` types, the linked-list nodes and the function declarations |
| `debugmalloc.h` | A memory-leak checker from a third party, turned off by default |
| `memlog.txt` | Where the leak checker writes its report |
| `library_simulator.exe` | The program, already built for Windows |

Books and users are kept in two linked lists, built from the text files when the program starts. The
comments in the code are in Hungarian.

To check for memory leaks, uncomment `#include "debugmalloc.h"` at the top of `main.c` and `functions.c`
and the line `debugmalloc_log_file("memlog.txt");` in `main.c`, then build the program again.
