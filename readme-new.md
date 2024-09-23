# MYCTIOBAPKA
![Successfully graded](https://img.shields.io/badge/Successfully_graded-%E2%9C%94%EF%B8%8F-green)

Συγγραφείς: **Λαμπρινός Χατζηιωάννου** και **Γεώργιος Παναγιωτίδης** για το μάθημα του δομημένου προγραμματισμού, 1ου εξαμήνου του τμήματος Ηλεκτρολόγων Μηχανικών και Mηχανικών Υπολογιστών του Α.Π.Θ., υπό την επίβλεψη του κ. **Χρήστου Σεβαστιάδη**.

![Game Logo](logo.png)

# Περίληψη

Το **ΜΥCΤΙΟΒΑΡΚΑ** είναι ένα ανταγωνιστικό παιχνίδι δύο παικτών, οι οποίοι χειρίζονται ιστιοφόρα. Στόχος του παιχνιδιού είναι ο παίκτης να οδηγήσει το σκάφος του στην αρχική θέση αφού ολοκληρώσει έναν περίπλου του νησιού. Οι κινήσεις διεξάγονται σε γύρους, όπου ο κάθε παίκτης ορίζει την κατεύθυνση του σκάφους με τρία key strokes. Παράλληλα, την πορεία του πλοίου επηρεάζει ο άνεμος, ο οποίος μεταβάλλεται τυχαία, τόσο σε ένταση όσο και σε κατεύθυνση ανά γύρο.

Η ένταση του ανέμου κυμαίνεται από 1 έως 4, και καθορίζει πόσες θέσεις θα μετακινηθεί το καράβι. Η κατεύθυνση του ανέμου προσδιορίζεται με ένα χρωματιστό βέλος: άσπρο για τιμή 1, πράσινο για τιμή 2, μπλε για τιμή 3, και κόκκινο για τιμή 4.

Σε περίπτωση σύγκρουσης με τη στεριά, ο παίκτης ξαναξεκινάει από την αρχική του θέση. Αν οι παίκτες συγκρουστούν μεταξύ τους, επιστρέφουν και οι δύο στην αρχική τους θέση. Οι κινήσεις δηλώνονται με τα βέλη κατεύθυνσης.

# Οδηγίες Χρήσης - Ανάλυση

Το παιχνίδι αρχίζει ενεργοποιώντας τις δυνατότητες παραθύρων με τη βιβλιοθήκη `ncurses` μέσω της συνάρτησης `initCurses()`. Στη συνέχεια, εμφανίζεται το μενού (`showMenu()`), το οποίο περιλαμβάνει:
- Τερματισμό του παιχνιδιού (`showExit()`),
- Προβολή *Credits* (`showCredits()`), από όπου ο χρήστης μπορεί να επιστρέψει στο μενού, και
- Έναρξη του παιχνιδιού (*PLAY*).

Επιλέγοντας *PLAY*, ο παίκτης εισέρχεται στην εισαγωγική οθόνη του παιχνιδιού (`showIntro()`). Ακολουθεί η αρχικοποίηση των παικτών με την `initPlayers()`, όπου οι χρήστες δηλώνουν τα ονόματα (`setPlayerName()`) και τους χαρακτήρες τους (`setPlayerChar()`). Το παιχνίδι συνεχίζεται μέχρι να νικήσει κάποιος ή μέχρι να πατηθεί το πλήκτρο `F1`, οπότε τερματίζεται το παιχνίδι.

Η διαδικασία παιχνιδιού (`playATurn()`) περιλαμβάνει τα εξής βήματα:
- Εμφάνιση του χάρτη (`wprintCurrentState()`),
- Εμφάνιση των συντεταγμένων των παικτών (`wprintPlayerData()`),
- Εμφάνιση του ανέμου για τον τρέχοντα γύρο (`wprintAirArrow()`),
- Ζήτηση κινήσεων από κάθε παίκτη μέσω των βελών κατεύθυνσης, με δυνατότητα διόρθωσης (`Backspace`) και ολοκλήρωση με `Enter`,
- Εναλλαγή κινήσεων μεταξύ των παικτών και του ανέμου.

# Libraries Used

### unistd.h
- `sleep()`: Wait for a specified time.

### ncurses.h
- `box()`: Creates window borders.
- `cbreak()`: Disable line buffering.
- `curs_set()`: Controls cursor visibility (0 for invisible).
- `echo()`: Makes typed characters visible.
- `endwin()`: Ends the curses mode.
- `getch()`: Reads a key from `stdscr`.
- `getmaxyx()`: Retrieves the window size and coordinates.
- `init_pair()`: Sets foreground and background colors.
- `initscr()`: Starts curses mode.
- `keypad()`: Enables function key input.
- `mvwaddch()`: Moves cursor and adds character.
- `mvwinch()`: Returns a character from the window.
- `mvwprintw()`: Moves cursor and prints formatted output.
- `noecho()`: Disables character echoing.
- `refresh()`: Refreshes `stdscr`.
- `start_color()`: Enables color mode.
- `waddch()`: Adds character to the window.
- `attron()`, `attroff()`: Turns on/off attributes.
- `wclear()`: Clears window.
- `wgetch()`: Reads character from window.
- `wgetstr()`: Reads a string from the window.
- `wmove()`: Moves cursor within window.
- `wrefresh()`: Refreshes window.

### menu.h
- `current_item()`: Gets the current item in a menu.
- `free_item()`: Frees an item.
- `free_menu()`: Frees a menu.
- `item_name()`: Gets the name of a menu item.
- `menu_driver()`: Processes menu commands.
- `new_item()`: Creates a new menu item.
- `new_menu()`: Creates a menu.
- `post_menu()`: Displays the menu.
- `set_menu_mark()`: Sets the mark for the selected item.
- `set_menu_sub()`: Defines a submenu.
- `set_menu_win()`: Defines the window for the menu.
- `unpost_menu()`: Hides the menu.

### stdio.h
- `printf`: Sends formatted output.

### string.h
- `strcmp()`: Compares two strings.

### stdlib.h
- `calloc()`: Allocates memory.
- `exit()`: Terminates the program.
- `rand()`: Generates random numbers.
- `srand()`: Seeds the random number generator.

### time.h
- `srand(time())`: Seeds random number generator using the current time.

## Πηγές
- [NCURSES Programming HOWTO](http://www.ibiblio.org/pub/Linux/docs/HOWTO/other-formats/pdf/NCURSES-Programming-HOWTO.pdf)
- [NCURSES Introduction](https://invisible-island.net/ncurses/ncurses-intro.html)
- [Oracle Docs](https://docs.oracle.com/cd/E19455-01/806-0629/6j9vjcoae/index.html)
