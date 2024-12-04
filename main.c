#include "header.h"
// #include "debugmalloc.h" 

int main(void)
{
    // Könyvek betöltése fájlból "books.txt"
    BookNode* bookList = Loading_books("books.txt");

    // Felhasználók betöltése fájlból "users.txt"
    UserNode* userList = Loading_users("users.txt", bookList);

    if(bookList == NULL && userList == NULL)
        return 0;
    
    //Nyissa meg a főmenüt
    menu_selection(bookList, userList);
    
    // Felszabadítja a lefoglalt memóriát
    free_book_list(bookList);
    free_user_list(userList);

    // debugmalloc_log_file("memlog.txt");

    return 0; 
}