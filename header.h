#ifndef NAME_HEADER_LIB_
#define NAME_HEADER_LIB_

/*======================= Includes Standard C Libraries ========================*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

/*======================= structok ========================*/

typedef struct {
  char *title;
  char *author;
  unsigned year;
  unsigned user_id; 
} Book;

typedef struct {
  char *name;
  unsigned id;
  Book** borrowedBook;
} User;

typedef struct BookNode {
  Book book;
  struct BookNode *next;
} BookNode;

typedef struct UserNode {
  User user;
  struct UserNode *next;
} UserNode;

// Menü
enum 
{
  EXIT, 
  REGISTER_BOOK, 
  REGISTER_USER, 
  BORROW, 
  RETURN_BOOK, 
  SEARCH, 
  LIST_BOOKS, 
  LIST_USER
};

/*======================= függvények ========================*/

void menu_art();
void return_menu();
void menu_selection(BookNode *bookList, UserNode *userList);

unsigned generate_user_id(UserNode *user_list);
UserNode* findUser(UserNode* user_list, unsigned int user_id);
BookNode* findBook(BookNode* book_list, char* title);

BookNode* Loading_books(char *file_name);
UserNode* Loading_users(char *file_name, BookNode *book_list);

void registerBook(BookNode *book_list, char *file_name);
void registerUser(UserNode *user_list, char *file_name);

void borrowBook(BookNode* book_list, UserNode* user_list);
void returnBook(BookNode *book_list, UserNode *user_list);

void saveBooksToFile(BookNode* book_list, char* file_name);
void searchBook(BookNode *book_list);

void listBooks(BookNode *bookList);
void listUsers(UserNode *userList);

void free_book_list(BookNode *head);
void free_user_list(UserNode *head);

#endif