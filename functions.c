#include "header.h"
// #include "debugmalloc.h"

// Gyönyörű menü
void menu_art(void)
{
  system("cls");
  puts(
      " \t\t  _      _ _                                                         \n"
      " \t\t | |    (_) |                                                        \n"
      " \t\t | |     _| |__  _ __ __ _ _ __ _   _     _ __ ___   ___ _ __  _   _ \n"
      " \t\t | |    | | '_ \\/ '__/ _` | '__| | | |   | '_ ` _ \\ / _ \\ '_ \\| | | |\n"
      " \t\t | |____| | |_) | | | (_| | |  | |_| |   | | | | | |  __/ | | | |_| |_| |\n"
      " \t\t |______|_|_.__/|_|  \\__,_|_|  \\__,  |   |_| |_| |_|\\___|_| |_|\\___,_|\n"
      " \t\t                                 __/ |                               \n"
      " \t\t                                |___/                                \n\n\n"
      "\t\t\t   Select 0 to exit.\n"
      "\t\t\t   Select 1 to register book.\n"
      "\t\t\t   Select 2 to register new user.\n"
      "\t\t\t   Select 3 to borrow a book.\n"
      "\t\t\t   Select 4 to return a book.\n"
      "\t\t\t   Select 5 to search for a book by title.\n"
      "\t\t\t   Select 6 to list the books in the library.\n"
      "\t\t\t   Select 7 to list the users registered in the library.\n");
}

/// @brief Funkció, amely megvárja, hogy a felhasználó befejezze az olvasást, mielőtt visszatérne a főmenübe
void return_menu()
{
  // Kiír egy üzenetet a felhasználónak, hogy nyomja meg az entert a menübe való visszatéréshez.
  printf("\nPlease press enter to return to the menu.\n");
  int c;
  // Addig olvassa a bemenetet karakterenként, amíg egy sortörés karaktert (\n) vagy a fájl végét (EOF) nem talál.
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

/// @brief Változások esetén frissítse a books.tx fájlt
/// @param book_list Paraméterként megkapja a lancol könyvlistát
/// @param file_name fájlnév
void saveBooksToFile(BookNode *book_list, char *file_name)
{
  // Megnyitja a megadott nevű fájlt írásra ("w" mód).
  FILE *file = fopen(file_name, "w");
  // Ellenőrzi, hogy sikerült-e megnyitni a fájlt. Ha nem, hibaüzenetet ír ki és kilép a függvényből.
  if (file == NULL)
  {
    perror("Error opening books file!\n");
    return;
  }

  // Beállít egy pointert a könyvek láncolt listájának elejére.
  BookNode *current = book_list;
  // Ciklusban végigmegy a listán, amíg a végét el nem éri.
  while (current != NULL)
  {
    // Kiírja az aktuális könyv adatait (cím, szerző, év, felhasználó azonosító) a fájlba.
    fprintf(file, "%s,%s,%u,%u\n", current->book.title, current->book.author, current->book.year, current->book.user_id);
    // Továbblép a lista következő elemére.
    current = current->next;
  }
  // Bezárja a fájlt.
  fclose(file);
}

// Hozzon létre új azonosítót az új felhasználók számára
unsigned generate_user_id(UserNode *user_list)
{
  if (user_list == NULL)
  {
    return 1000;
  }
  else
  {
    UserNode *current = user_list;
    while (current->next != NULL)
    {
      current = current->next;
    }
    return current->user.id + 1;
  }
}

/// @brief Töltsön be elemeket a book.txt fájlból egy listába
/// @param file_name .txt fájlnév
/// @return a lista első elemét (head) adja vissza
BookNode *Loading_books(char *file_name)
{
  // Megnyitja a fájlt olvasásra ("r" mód).
  FILE *file = fopen(file_name, "r");
  // Ellenőrzi, hogy sikerült-e megnyitni a fájlt.
  if (file == NULL)
  {
    perror("Error opening books file!");
    return NULL;
  }

  BookNode *head = NULL;    // A könyvek láncolt listájának feje.
  BookNode *current = NULL; // Az aktuális könyv a listában.
  char line[256];           // Buffer a sorok beolvasásához.

  // Ciklusban olvassa a fájlt soronként, amíg el nem éri a fájl végét.
  while (fgets(line, sizeof(line), file) != NULL)
  {
    // Memóriát foglal egy új BookNode-nak.
    BookNode *newNode = (BookNode *)malloc(sizeof(BookNode));
    // Ellenőrzi, hogy sikerült-e memóriát foglalni.
    if (newNode == NULL)
    {
      perror("Memory allocation error");
      fclose(file);
      return head;
    }

    // Kiszámítja a szerző és a cím karakterláncának méretét
    int size_title = 0;
    int size_author = 0;
    int i = 0;

    for (i = 0; line[i] != ',' && line[i] != '\0'; i++)
    {
      size_title++;
    }
    size_title++; // + '/0' a string végének jelzésére.
    for (i++; line[i] != ',' && line[i] != '\0'; i++)
    {
      size_author++;
    }
    size_author++; // + '/0' a string végének jelzésére.

    // dinamikusan lefoglalja a memóriát a címhez
    newNode->book.title = (char *)malloc(sizeof(char) * size_title);
    // Ellenőrzi, hogy sikerült-e memóriát foglalni a címnek.
    if (newNode->book.title == NULL)
    {
      perror("Error allocating memory for title!\n");
      free_book_list(head); // Felszabadítja a már lefoglalt memóriát.
      fclose(file);
      exit(EXIT_FAILURE);
    }

    // dinamikusan lefoglalja a memóriát a szerzőnek
    newNode->book.author = (char *)malloc(sizeof(char) * size_author);
    // Ellenőrzi, hogy sikerült-e memóriát foglalni a szerzőnek.
    if (newNode->book.author == NULL)
    {
      perror("Error allocating memory for author!\n");
      free_book_list(head);
      fclose(file);
      exit(EXIT_FAILURE);
    }

    // Beolvassa a könyv adatait (cím, szerző, év, felhasználó azonosító) a sorból.
    if (sscanf(line, "%[^,],%[^,],%u,%u", newNode->book.title, newNode->book.author,
               &(newNode->book.year), &(newNode->book.user_id)) != 4)
    {
      fprintf(stderr, "Error reading line from books file\n");
      free(newNode->book.title);
      free(newNode->book.author);
      free(newNode);
      continue;
    }
    // Beállítja a next pointert NULL-ra.
    newNode->next = NULL;
    // Hozzáadja az új csomópontot a láncolt listához.
    if (head == NULL)
    {
      head = newNode;
      current = newNode;
    }
    else
    {
      current->next = newNode;
      current = newNode;
    }
  }
  // Bezárja a könyvek fájlját.
  fclose(file);
  // Visszaadja a könyvek láncolt listájának fejét.
  return head;
}

/// @brief Töltsön be elemeket a users.txt fájlból egy listába
/// @param file_name .txt fájlnév
/// @return a lista első elemét (head) adja vissza
UserNode *Loading_users(char *file_name, BookNode *book_list)
{
  // Megnyitja a felhasználók fájlját olvasásra ("r" mód).
  FILE *file = fopen(file_name, "r");
  // Ellenőrzi, hogy sikerült-e megnyitni a fájlt.
  if (file == NULL)
  {
    perror("Error opening users file!");
    return NULL;
  }

  UserNode *head = NULL;    // A felhasználók láncolt listájának feje.
  UserNode *current = NULL; // Az aktuális felhasználó a listában.
  char line[256];           // Buffer a sorok beolvasásához.

  // Ciklusban olvassa a fájlt soronként, amíg el nem éri a fájl végét.
  while (fgets(line, sizeof(line), file) != NULL)
  {
    // Memóriát foglal egy új UserNode-nak.
    UserNode *newNode = (UserNode *)malloc(sizeof(UserNode));
    // Ellenőrzi, hogy sikerült-e memóriát foglalni.
    if (newNode == NULL)
    {
      perror("Memory allocation error");
      fclose(file);
      return head;
    }

    int size_name = 0;
    // Kiszámolja a felhasználó nevének hosszát.
    for (int i = 0; line[i] != ',' && line[i] != '\0'; i++)
    {
      size_name++;
    }
    size_name++;

    // Memóriát foglal a felhasználó nevének.
    newNode->user.name = (char *)malloc(sizeof(char) * size_name);
    // Inicializálja a kölcsönzött könyvek pointerét NULL-ra.
    newNode->user.borrowedBook = NULL;

    // Ellenőrzi, hogy sikerült-e memóriát foglalni a névnek.
    if (newNode->user.name == NULL)
    {
      perror("Error allocating memory for user name!\n");
      free_user_list(head);
      fclose(file);
      exit(EXIT_FAILURE);
    }
    // Beolvassa a felhasználó nevét és azonosítóját a sorból.
    if (sscanf(line, "%[^,],%d", newNode->user.name, &(newNode->user.id)) != 2)
    {
      fprintf(stderr, "Error reading line from users file\n");
      free(newNode->user.name);
      free(newNode);
      continue;
    }
    // Beállítja a next pointert NULL-ra.
    newNode->next = NULL;

    // Hozzáadja az új csomópontot a láncolt listához.
    if (head == NULL)
    {
      head = newNode;
      current = newNode;
    }
    else
    {
      current->next = newNode;
      current = newNode;
    }
  }

  // Bezárja a felhasználók fájlját.
  fclose(file);

  // Bejárja a könyvek listáját.
  BookNode *currentBook = book_list;
  while (currentBook != NULL)
  {
    // Ha a könyv ki van kölcsönözve (user_id != 0).
    if (currentBook->book.user_id != 0)
    {
      // Megkeresi a kölcsönző felhasználót.
      UserNode *user = findUser(head, currentBook->book.user_id);
      // Ha megtalálta a felhasználót.
      if (user != NULL)
      {
        // Memóriát foglal a kölcsönzött könyv pointerének.
        user->user.borrowedBook = malloc(sizeof(Book *));
        // Ellenőrzi, hogy sikerült-e memóriát foglalni.S
        if (user->user.borrowedBook == NULL)
        {
          perror("Error allocating memory for borrowed book!\n");
          free_user_list(head);
          return NULL;
        }
        // Memóriát foglal a kölcsönzött könyv adatainak.
        user->user.borrowedBook[0] = malloc(sizeof(Book));
        // Ellenőrzi, hogy sikerült-e memóriát foglalni.
        if (user->user.borrowedBook[0] == NULL)
        {
          perror("Error allocating memory for book!\n");
          free_user_list(head);
          return NULL;
        }
        // Memóriát foglal a könyv címének.
        user->user.borrowedBook[0]->title = malloc(sizeof(char) * (strlen(currentBook->book.title) + 1));
        // Ellenőrzi, hogy sikerült-e memóriát foglalni.
        if (user->user.borrowedBook[0]->title == NULL)
        {
          perror("Error allocating memory for book title!\n");
          free_user_list(head);
          return NULL;
        }
        // Bemásolja a könyv címét a kölcsönzött könyv adatai közé.
        strcpy(user->user.borrowedBook[0]->title, currentBook->book.title);
      }
    }
    // Továbblép a következő könyvre.
    currentBook = currentBook->next;
  }
  // Visszaadja a felhasználók láncolt listájának fejét.
  return head;
}

/// @brief Regisztrálja az új könyvet a lancol listáról és a books.txt fájlban
/// @param book_list könyvek lánclistája kap paraméterként
/// @param file_name Fájlnév
void registerBook(BookNode *book_list, char *file_name)
{
  char title[100], author[100];
  unsigned int year;

  // Bekéri a könyv címét a felhasználótól.
  printf("Please enter the title of the book: ");
  fgets(title, 100, stdin);
  title[strcspn(title, "\n")] = '\0'; // Eltávolítja a sortörés karaktert a címből.

  // Bekéri a könyv szerzőjét a felhasználótól.
  printf("\nPlease enter the author of the book: ");
  fgets(author, 100, stdin);
  author[strcspn(author, "\n")] = '\0'; // Eltávolítja a sortörés karaktert a szerző nevéből.

  // Bekéri a könyv kiadási évét a felhasználótól.
  printf("\nPlease enter the year the book was released: ");
  scanf("%u", &year);
  getchar(); // Kiüríti a bemeneti buffert

  // Létrehoz egy új BookNode csomópontot és memóriát foglal le hozzá.
  BookNode *newBook = (BookNode *)malloc(sizeof(BookNode));
  if (newBook == NULL)
  {
    fprintf(stderr, "Error allocating memory for new workbook!\n");
    exit(EXIT_FAILURE);
  }

  // Memóriát foglal le a címnek
  newBook->book.title = malloc(sizeof(char) * (strlen(title) + 1));
  // Ellenőrzi, hogy sikerült-e memóriát foglalni a címnek.
  if (newBook->book.title == NULL)
  {
    fprintf(stderr, "Erro ao alocar memória para o título!\n");
    free(newBook);
    exit(EXIT_FAILURE);
  }
  // Bemásolja a címet az új BookNode-ba.
  strcpy(newBook->book.title, title);

  // Memóriát foglal le a szerzőnek
  newBook->book.author = malloc(sizeof(char) * (strlen(author) + 1));
  // Ellenőrzi, hogy sikerült-e memóriát foglalni a szerzőnek.
  if (newBook->book.author == NULL)
  {
    fprintf(stderr, "Erro ao alocar memória para o autor!\n");
    free(newBook->book.title);
    free(newBook);
    exit(EXIT_FAILURE);
  }
  // Bemásolja a szerzőt az új BookNode-ba.
  strcpy(newBook->book.author, author);

  // Bemásolja a könyv adatait az új csomópontba
  newBook->book.year = year;
  newBook->book.user_id = 0; // Kezdetben a könyv nem kölcsönözhető
  newBook->next = NULL;

  // Beszúrja az új csomópontot a book_list láncolt listába
  if (book_list == NULL)
    book_list = newBook; // Ha a lista üres, az új csomópont az első
  else
  {
    // Ha a lista nem üres, megkeresi a lista végét.
    BookNode *current = book_list;
    while (current->next != NULL)
    {
      current = current->next;
    }
    // Beszúrja az új csomópontot a lista végére.
    current->next = newBook; // Beszúrja a lista végére
  }

  // Hozzáadja a könyvet a "books.txt" fájlhoz
  FILE *file = fopen(file_name, "a");
  // Ellenőrzi, hogy sikerült-e megnyitni a fájlt.
  if (file == NULL)
  {
    perror("Error opening users file!");
    free(newBook->book.author);
    free(newBook->book.title);
    free(newBook);
    exit(EXIT_FAILURE);
  }
  // Kiírja az új könyv adatait a fájlba.
  fprintf(file, "%s,%s,%u,%u\n", newBook->book.title, newBook->book.author, newBook->book.year, newBook->book.user_id);
  // Kiírja a sikeres regisztrációról szóló üzenetet.
  printf("\nNew book successfully registered!\n\n");

  // Bezárja a fájlt.
  fclose(file);
}

/// @brief Új felhasználó regisztrálása a könyvtárban
/// @param user_list Paraméterként megkapja a hivatkozott felhasználók listáját
/// @param file_name Paraméterként megkapja a fájlnév
void registerUser(UserNode *user_list, char *file_name)
{
  char name[100];
  // Bekéri a felhasználó nevét.
  printf("Enter username: ");
  fgets(name, 100, stdin);
  name[strcspn(name, "\n")] = '\0'; // Eltávolítja az újsor karaktert a névből.

  // Felhasználói azonosító automatikus generálása
  unsigned int user_id = generate_user_id(user_list);

  // Létrehoz egy új UserNode-ot.
  UserNode *newUser = (UserNode *)malloc(sizeof(UserNode));
  // Ellenőrzi, hogy sikerült-e memóriát foglalni.
  if (newUser == NULL)
  {
    fprintf(stderr, "Error allocating memory for new user!\n");
    exit(EXIT_FAILURE);
  }
  // Memóriát foglal a felhasználó nevének.
  newUser->user.name = malloc(sizeof(char) * (strlen(name) + 1));
  // Ellenőrzi, hogy sikerült-e memóriát foglalni a névnek.
  if (newUser->user.name == NULL)
  {
    fprintf(stderr, "Error allocating memory for name!\n");
    free(newUser);
    exit(EXIT_FAILURE);
  }
  // Bemásolja a nevet az új UserNode-ba.
  strcpy(newUser->user.name, name);

  // Beállítja az új UserNode adatait (azonosító, kölcsönzött könyv, következő elem).
  newUser->user.id = user_id;
  newUser->user.borrowedBook = NULL;
  newUser->next = NULL;

  // Hozzáadja az új UserNode-ot a láncolt listához.
  if (user_list == NULL)
  {
    user_list = newUser;
  }
  else
  {
    UserNode *current = user_list;
    while (current->next != NULL)
    {
      current = current->next;
    }
    current->next = newUser;
  }

  // Megnyitja a "users.txt" fájlt hozzáfűzésre ("a" mód).
  FILE *file = fopen(file_name, "a");
  // Ellenőrzi, hogy sikerült-e megnyitni a fájlt.
  if (file == NULL)
  {
    fprintf(stderr, "Error opening users.txt file!\n");
    free(newUser->user.name);
    free(newUser);
    exit(EXIT_FAILURE);
  }

  // Kiírja az új felhasználó adatait a fájlba.
  fprintf(file, "%s,%u\n", newUser->user.name, newUser->user.id);
  // Kiírja a sikeres regisztrációról szóló üzenetet és a felhasználó azonosítóját.
  printf("\nNew user successfully registered!\n\n");
  printf("This is your ID: %u\n", newUser->user.id);

  // Bezárja a fájlt.
  fclose(file);
}

// Ez a függvény megkeresi a megadott azonosítójú felhasználót a felhasználók láncolt listájában.
UserNode *findUser(UserNode *user_list, unsigned int user_id)
{
  // Beállít egy pointert a lista elejére.
  UserNode *current = user_list;
  // Ciklusban végigmegy a listán, amíg a végét el nem éri.
  while (current != NULL)
  {
    // Ha az aktuális elem azonosítója megegyezik a keresett azonosítóval, akkor visszaadja az aktuális elemet.
    if (current->user.id == user_id)
    {
      return current;
    }
    // Továbblép a lista következő elemére.
    current = current->next;
  }
  // Ha nem találta a felhasználót, NULL pointert ad vissza.
  return NULL;
}

// Ez a függvény megkeresi a megadott című könyvet a könyvek láncolt listájában.
BookNode *findBook(BookNode *book_list, char *title)
{
  // Beállít egy pointert a lista elejére.
  BookNode *current = book_list;
  // Ciklusban végigmegy a listán, amíg a végét el nem éri.
  while (current != NULL)
  {
    // Ha az aktuális könyv címe megegyezik a keresett címmel (kis- és nagybetűket figyelmen kívül hagyva),
    // akkor visszaadja az aktuális elemet.
    if (strcasecmp(current->book.title, title) == 0)
    {
      return current;
    }
    // Továbblép a lista következő elemére.
    current = current->next;
  }
  // Ha nem találta a könyvet, NULL pointert ad vissza.
  return NULL;
}

/// @brief Ez a funkció megkeresi a kívánt könyvet, és ellenőrzi, hogy kölcsönözhető-e
/// @param book_list Paraméterként megkapja a lancol könyvlistát
/// @param user_list Paraméterként megkapja a lancol felhasználók listáját
void borrowBook(BookNode *book_list, UserNode *user_list)
{
  unsigned user_id;
  // Bekéri a felhasználó azonosítóját.
  printf("Please enter your ID: ");
  scanf("%u", &user_id);

  getchar(); // Eltávolítja a \n karaktert az stdin-ből.

  // Megkeresi a felhasználót az azonosító alapján.
  UserNode *user = findUser(user_list, user_id);

  // Ellenőrzi, hogy a felhasználó létezik-e.
  if (user == NULL)
  {
    printf("\nNo user found.\n");
    return;
  }

  // Üdvözli a felhasználót.
  printf("\nWelcome %s!\n", user->user.name);

  char title[100];
  // Bekéri a könyv címét.
  printf("\nPlease enter the title of the book: ");
  fgets(title, 100, stdin);
  title[strcspn(title, "\n")] = '\0'; // Eltávolítja a sortörés karaktert a címből.

  // Megkeresi a könyvet a cím alapján.
  BookNode *book = findBook(book_list, title);

  // Ellenőrzi, hogy a könyv létezik-e.
  if (book == NULL)
  {
    printf("\nNo book found.\n");
    return;
  }

  // Ellenőrzi, hogy a felhasználó már kölcsönözte-e a maximális számú könyvet.
  if (user->user.borrowedBook != NULL)
  {
    printf("\nYou already have the maximum amount of books borrowed.\n");
    return;
  }

  // Ellenőrzi, hogy a könyv elérhető-e.
  if (book->book.user_id != 0)
  {
    printf("\nThe book is currently unavailable.\n");
    return;
  }

  // Kölcsönzi a könyvet:
  // - Beállítja a könyv user_id mezőjét a felhasználó azonosítójára.
  book->book.user_id = user_id;
  // Memóriát foglal a kölcsönzött könyv pointerének.
  user->user.borrowedBook = malloc(sizeof(Book *));
  // Ellenőrzi, hogy a memória foglalás sikeres volt-e.
  if (user->user.borrowedBook == NULL)
  {
    fprintf(stderr, "\nError allocating memory for borrowed book!\n");
    exit(EXIT_FAILURE);
  }

  // Beállítja a felhasználó borrowedBook pointerét a kölcsönzött könyvre.
  user->user.borrowedBook[0] = &book->book;

  // Kiírja a sikeres kölcsönzésről szóló üzenetet.
  printf("\nBook successfully borrowed!\n");

  // Elmenti a változásokat a "books.txt" fájlba.
  saveBooksToFile(book_list, "books.txt");
}

/// @brief Add vissza a kölcsönzött könyvet
/// @param book_list Paraméterként megkapja a lancol könyvlistát
/// @param user_list Paraméterként megkapja a lancol felhasználók listáját
void returnBook(BookNode *book_list, UserNode *user_list)
{
  unsigned user_id;
  // Bekéri a felhasználó azonosítóját.
  printf("Please enter your ID: ");
  scanf("%u", &user_id);

  // Megkeresi a felhasználót az azonosító alapján.
  UserNode *user = findUser(user_list, user_id);

  // Ellenőrzi, hogy a felhasználó létezik-e.
  if (user == NULL)
  {
    printf("No user found.\n");
    return;
  }

  // Üdvözli a felhasználót.
  printf("\nWelcome %s!\n", user->user.name);

  getchar(); // Eltávolítja a \n karaktert az stdin-ből.

  char title[100];
  // Bekéri a könyv címét.
  printf("\nPlease enter the title of the book: ");
  fgets(title, 100, stdin);
  title[strcspn(title, "\n")] = '\0'; // Eltávolítja a sortörés karaktert a címből.

  // Megkeresi a könyvet a cím alapján.
  BookNode *book = findBook(book_list, title);

  // Ellenőrzi, hogy a könyv létezik-e.
  if (book == NULL)
  {
    printf("\nNo book found in the library.\n");
    return;
  }

  // Ellenőrzi, hogy a felhasználó kölcsönözte-e a könyvet.
  if (user->user.borrowedBook == NULL || user->user.borrowedBook[0] != &book->book)
  {
    printf("\nYou do not have this book borrowed.\n");
    return;
  }

  // Visszaadja a könyvet:
  // - Beállítja a könyv user_id mezőjét 0-ra (elérhető).
  book->book.user_id = 0;
  // Felszabadítja a kölcsönzött könyvhöz lefoglalt memóriát.
  free(user->user.borrowedBook);
  // Beállítja a felhasználó borrowedBook pointerét NULL-ra.
  user->user.borrowedBook = NULL;

  // Kiírja a sikeres visszavételről szóló üzenetet.
  printf("\nBook returned successfully!\n");

  // Elmenti a változásokat a "books.txt" fájlba.
  saveBooksToFile(book_list, "books.txt");
}

/// @brief Keresse meg a könyvet cím szerint
/// @param book_list könyvek lánclistája kap paraméterként
void searchBook(BookNode *book_list)
{
  // Ellenőrzi, hogy a könyvtár üres-e. Ha igen, akkor kiír egy üzenetet és kilép a függvényből.
  if (book_list == NULL)
  {
    printf("No book found in the library.\n");
    return;
  }

  char title[100];
  // Bekéri a felhasználótól a keresett könyv címét.
  printf("Please enter the name of the book you are looking for:\n");
  fgets(title, 100, stdin);
  title[strcspn(title, "\n")] = '\0'; // Eltávolítja a sortörés karaktert a beolvasott címből.

  BookNode *current_book = book_list;
  while (current_book != NULL)
  {
    // Összehasonlítja a beolvasott címet az aktuális könyv címével (kis- és nagybetűket figyelmen kívül hagyva).
    if (strcasecmp(current_book->book.title, title) == 0)
    {
      // Ha megtalálta a könyvet, kiírja az adatait (cím, szerző, év) és a kölcsönözhetőségi állapotát.
      printf("\nTitle: %s\n", current_book->book.title);
      printf("Author: %s\n", current_book->book.author);
      printf("Year: %u\n", current_book->book.year);
      if (current_book->book.user_id != 0)
        printf("This book is unavailable to borrow\n\n");
      else
        printf("This book is available to borrow\n\n");
      return; // Kilép a függvényből, miután megtalálta a könyvet
    }
    current_book = current_book->next;
  }
  printf("Book not found.\n");
}

/// @brief Könyvtári könyvek nyomtatása
/// @param bookList könyvek lánclistája kap paraméterként
void listBooks(BookNode *bookList)
{
  system("cls");
  if (bookList == NULL)
  {
    printf("No books found in the library.\n");
    return;
  }

  puts(
      "  _                 _        \n"
      " | |__   ___   ___ | | _____ \n"
      " | '_ \\ / _ \\ / _ \\| |/ / __|\n"
      " | |_) | (_) | (_) |   <\\__ \\\n"
      " |_.__/ \\___/ \\___/|_|\\_\\___/\n\n\n");

  BookNode *currentBook = bookList;
  while (currentBook != NULL)
  {
    printf("Title: %s\n", currentBook->book.title);
    printf("Author: %s\n", currentBook->book.author);
    printf("Year: %u\n", currentBook->book.year);
    if (currentBook->book.user_id == 0)
      printf("Book available\n\n");
    else
      printf("This user %u borrowed this book.\n\n", currentBook->book.user_id);
    currentBook = currentBook->next;
  }
}

/// @brief Nyomtassa ki a könyvtárhasználók listáját
/// @param userList felhasználó lánclistája kap paraméterként
void listUsers(UserNode *userList)
{
  system("cls");

  if (userList == NULL)
  {
    printf("No users found in the library.\n");
    return;
  }

  puts(
      " _   _                   \n"
      "| | | |___  ___ _ __ ___ \n"
      "| | | / __|/ _ \\ '__/ __|\n"
      "| |_| \\__ \\  __/ |  \\__ \\\n"
      " \\___/|___/\\___|_|  |___/\n\n");

  UserNode *currentUser = userList;
  while (currentUser != NULL)
  {
    printf("Name: %s\n", currentUser->user.name);
    printf("ID: %u\n", currentUser->user.id);

    // Ellenőrizze, hogy a felhasználó kölcsönzött-e könyveket
    if (currentUser->user.borrowedBook != NULL)
    {
      printf("Borrowed books:\n");
      Book **borrowedBooks = currentUser->user.borrowedBook;
      printf("  - %s\n", borrowedBooks[0]->title);
    }
    else
    {
      printf("No books borrowed.\n");
    }

    printf("\n");
    currentUser = currentUser->next;
  }
}

/// @brief Menüválasztás
/// @param bookList könyvek lánclistája kap paraméterként
/// @param userList felhasználó lánclistája kap paraméterként
void menu_selection(BookNode *bookList, UserNode *userList)
{
  int select = -1;

  while (select)
  {
    menu_art();

    scanf("%d", &select);
    getchar(); // Az újsor karakter felhasználása

    switch (select)
    {
    case EXIT:
      break;

    case REGISTER_BOOK:
      registerBook(bookList, "books.txt");
      return_menu();
      break;

    case REGISTER_USER:
      registerUser(userList, "users.txt");
      return_menu();
      break;

    case BORROW:
      borrowBook(bookList, userList);
      return_menu();
      break;

    case RETURN_BOOK:
      returnBook(bookList, userList);
      return_menu();
      break;

    case SEARCH:
      searchBook(bookList);
      return_menu();
      break;

    case LIST_BOOKS:
      listBooks(bookList);
      return_menu();
      break;

    case LIST_USER:
      listUsers(userList);
      return_menu();
      break;

    default:
      printf("Please, select a number from 1-7 or 0 to exit.\n\n");
      return_menu();
      break;
    }
  }
}

/// @brief Felszabadítja a dinamikusan lefoglalt könyvlista memóriát
/// @param head Paraméterként megkapja a lancol lista head
void free_book_list(BookNode *head)
{
  BookNode *current = head;
  while (current != NULL)
  {
    BookNode *next = current->next;
    // Felszabadítja a könyv címéhez lefoglalt memóriát.
    free(current->book.title);
    // Felszabadítja a könyv szerzőjéhez lefoglalt memóriát.
    free(current->book.author);
    // Felszabadítja a BookNode struktúrához lefoglalt memóriát.
    free(current);
    current = next;
  }
}

/// @brief Felszabadítja a felhasználói lista dinamikusan lefoglalt memóriát
/// @param head Paraméterként megkapja a lancol lista head
void free_user_list(UserNode *head)
{
  UserNode *current = head;
  while (current != NULL)
  {
    UserNode *next = current->next;
    // Felszabadítja a felhasználó nevéhez lefoglalt memóriát.
    free(current->user.name);

    // Felszabadítja a kikölcsönzött könyvekhez lefoglalt memóriát, ha van ilyen.
    if (current->user.borrowedBook != NULL)
    {
      free(current->user.borrowedBook[0]->title);
      free(current->user.borrowedBook[0]);
      free(current->user.borrowedBook);
    }
    // Felszabadítja a UserNode struktúrához lefoglalt memóriát.
    free(current);
    current = next;
  }
}