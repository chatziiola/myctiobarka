# Sailing Game
Συγγραφείς: Λαμπρινός Χατζηιωάννου και Γεώργιος Παναγιωτίδης για το μάθημα
του δομημένου προγραμματισμού, 1ου εξαμήνου του τμήματος Ηλεκτρολόγων Μηχανικών
και Mηχανικών Υπολογιστών του Α.Π.Θ. , υπό την επίβλεψη του κ. Χρήστου
Σεβαστιάδη.

# Περίληψη

Το ΜΥCΤΙΟΒΑΡΚΑ είναι ένα ανταγωνιστικό παιχνίδι δύο παικτών οι οποίοι χειρίζονται από ένα ιστιοφόρο. Στόχος του παιχνιδιού είναι ο παίκτης να οδηγήσει το σκάφος του στην θέση εκκίνησής του αφού ολοκληρώσει πρώτα έναν περίπλου του νησιού. Οι κινήσεις διεξάγονται σε γύρους όπου ο κάθε παίχτης έχει την δυνατότητα να ορίσει την κατεύθυνση του σκάφους του με τρία key strokes. Παρόλα αυτά, η είσοδος του παίκτη δεν είναι ο μόνος παράγοντας που ρυθμίζει την πορεία του σκάφους, καθώς την πορεία του πλοίου επηρεάζει ο άνεμος, ο οποίος μεταβάλλεται ανά γύρο τυχαία, τόσο σε ένταση όσο και σε κατεύθυνση. Η ένταση του ανέμου λαμβάνει τιμές από 1 εώς 4 και καθορίζει πόσες θέσεις θα μετακινήσει το καράβι προς την κατεύθυνση του. Η κατεύθυνση του ανέμου προσδιορίζεται από ένα χρωματιστό βέλος. Το χρώμα του βέλους είναι άσπρο αν έχει τιμή 1, πράσινο αν έχει τιμή 2, μπλε αν έχει τιμή 3 και κόκκινο αν έχει τιμή 4. Έαν το σκάφος κάποιου παίκτη συγκρουστεί με τη στεριά, αυτός ξαναξεκινάει από την αρχική του θέση, ενώ αν συγκρουστούν μεταξύ τους ξαναξεκινούν και οι δύο από τις αρχικές τους θέσεις. Τα κουμπιά με τα οποία οι παίχτες δηλώνουν διαδοχικά την κίνησή του είναι τα βέλη κατεύθυνσης.

# Οδηγίες χρήσης - Ανάλυση

Το παιχνίδι ξεκινώντας ενεργοποιεί τις δυνατότητες παραθύρων που δίνονται από την
βιβλιοθήκη ncurses (`initCurses()`),
εμφανίζει ένα menu(`showMenu()`), με τρεις επιλογές: τον τερματισμό (`showExit()`) του
παιχνιδιού, την προβολή ενός *Credits* παραραθύρου/σελίδας (`showCredits()`), 
από το οποίο ο χρήστης επαναφέρεται στο κύριο κορμό του μενου, και την κύρια επιλογή
του να ξεκινάει το παιχνίδι (*PLAY*). Επιλέγοντας το (*PLAY*), στην οθόνη εμφανίζονται
οδηγίες "εισαγωγικές" στο παιχνίδι (`showIntro()`), μια προς μια για κάθε φορά που ο χρήστης πατάει
ένα κουμπί. Στις εισαγωγικές αυτές οδηγίες περιλαμβάνεται μια συντομη περιγραφή του
παιχνιδιού (των κανόνων) και του τρόπου λειτουργίας. Μετά από την εισαγωγή, ακολουθεί
η αρχικοποίηση των χρηστών του παιχνιδιού(`initPlayers()`) κατα την οποία οι
χρήστες ρωτώνται για τα ονόματα με τα οποία θα συμμετάσχουν στο παιχνίδι (`setPlayerName()`)
(τα αναγνωριστικά τους) και τον χαρακτήρα τους (`setPlayerChar()`),
(με τον οποίο θα απεικονίζεται το κάθε ιστιοφόρο). Μέχρι να νικήσει κάποιος το παιχνίδι, 
ώστε να εμφανιστεί η οθόνη τερματισμού (`showWinner()`), ή να πατηθεί το πλήκτρο F1 
που σημαίνει την διάθεση του χρήστη να τερματίσει το παιχνιδι επαναλαμβάνεται η εξής διαδικασία (`playATurn()`):
- Εμφανίζεται στην οθόνη ο χάρτης (`wprintCurrentState()`) και οι συντεταγμένες των παικτών
(`wprintPlayerData()`), όπως είναιεκείνη την στιγμή, μαζί με
τον άνεμο για τον γύρο που ακολουθεί (`wprintAirArrow()`)
- Εμφανίζεται μήνυμα για τον κάθε χρήστη (τον έναν μετά τον άλλον) (`setPlayerName()`),
ζητώντας του να πατήσει τα "βελάκια" για την κίνηση του. Έχει την δυνατότητα να επιλέξει ως
τρια από αυτά, "σβήνοντας" τα με το Backspace στην περίπτωση λάθους. Η διαδικασία
εισαγωγής κινήσεων για τον κάθε παίκτη ολοκληρώνεται όταν εκείνος πατήσει το Enter
- Εναλάσσονται στην οθόνη μια προς μια οι κινήσεις που οφείλονται στον άνεμο και
εκείνες που με την είσοδο του ζήτησε ο παίκτης, με τον άνεμο να "φυσάει" κάποια
τυχαία στιγμή μεταξύ (ή και πριν/μετα) των κινήσεων των παικτών.


## Libraries used

### unistd.h

- sleep(): Wait for a specified time.

### ncurses.h

- box(): Creates window borders, horizontal and vertical lines.
- cbreak(): Disable line buffering so that we get each keypress in real time.
- curs_set(): It sets the appearance of the cursor based on the value of visibility, 0 in our case for invisibility.
- echo(): It is used to make the characters that are typed visible.
- endwin(): Ends the curses mode.
- getch(): Same with wgetch but for the stdscr.
- getmaxyx(): Store the current beginning coordinates and size of the specified window.
- init_pair(): Sets foreground and backgrond colors.
- initscr(): Starts the curses mode.
- keypad(): Makes the function keys readable.
- mvwaddch(): Does the same thing as addch but it firstly moves the cursor at the desired coordinates.
- mvwinch: Returns the character of type chtype().
- mvwprintw(): It moves the cursor to a specified coordinate of the associated window and sends formatted output.
- noecho(): It is used to make the characters that are typed not visible.
- refresh(): Same with wrefresh but for the stdscr.
- start_color(): Enables colors.
- waddch(): Prints a character at the associated window.
- attron(), attroff(): Turns on/off the named attributes without affecting any others.
- wclear(): Clears the associated window.
- wgetch(): Reads a character from the associated window.
- wgetstr(): Accepts character strings from curses terminal keyboard.
- wmove(): Moves the cursor associated with the window.
- wrefresh(): Refreshes the associated window so that the changes made are visible.

### menu.h

- current_item(): Returns a pointer to the current item in the given menu.
- free_item(): De-allocates an item.
- free_menu(): De-allocates the menu.
- item_name(): Returns the name part of the given item.
- menu_driver(): Command-processing loop of the menu system.
- new_item(): Creates an item.
- new_menu(): Creates a menu.
- post_menu(): Posts the menu.
- set_menu_mark(): Sets which character should point at our current option.
- set_menu_sub(): Sets the sub-window for our menu.
- set_menu_win(): Sets the main window for our menu.
- unpost_menu(): Unposts the menu.

### stdio.h

- printf: Sends formatted output

### string.h

- strcmp(): Compares two strings.

### stdlib.h

- calloc(): Allocates the requested memory and returns a pointer to it.
- exit(): Terminates the program
- rand(): Returns a random number.
- srand(): see time.h

### time.h

- srand(time()): It is used to generate random number sequences, the difference with rand is that the seed value is continuously changing in order to avoid repetition of number sequences

## Συναρτήσεις 

## Πηγές
- http://www.ibiblio.org/pub/Linux/docs/HOWTO/other-formats/pdf/NCURSES-Programming-HOWTO.pdf#%5B%7B%22num%22%3A475%2C%22gen%22%3A0%7D%2C%7B%22name%22%3A%22XYZ%22%7D%2C0%2C734%2C0%5D
- https://invisible-island.net/ncurses/ncurses-intro.html#introduction
- https://docs.oracle.com/cd/E19455-01/806-0629/6j9vjcoae/index.html
