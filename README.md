# Πρόβλημα Producers – Consumers με πολλαπλούς Producers και έναν Consumer

<h2> Ζαπαλίδη Ιωάννα <br></h2>

_1115201400044_ 

<p align="center">
  <img src="./img/md5.jpg">
</p>

_Περιεχόμενα_

1. Δομή &amp; Ανάπτυξη Κώδικα
2. Σχεδιαστικές Επιλογές
3. Βιβλιογραφία
#

_Δομή &amp; Ανάπτυξη Κώδικα_

Τα αρχεία της εργασίας είναι τα εξής:

- `Semun.c` &amp; `Semun.h` – _συναρτήσεις σημαφόρων, από την προηγουμενη εργασία μου_
- `shm.c` &amp; `shm.h` – _συναρτήσεις shared memory, από την προηγούμενη εργασία μου, αλλαγμένες για να υποστηρίζουν τα δεδομένα που χρειάζονται_
- `help.c` &amp; `help.h` - _βοηθητικές συναρτήσεις
- `main.c`
- `Makefile`
- αυτό το αρχείο `readme`
- ένα `randomfile.txt` από όπου αντλούνται οι γραμμές με τυχαίο τρόπο
#
Εκτελούμε στο terminal, στο σωστό directory (όπου υπάρχουν και τα tracefiles):

- Για να γίνει compile: `$make`
- Για να γίνει εκτέλεση (με ενδεικτικές τιμές): `$make run`
- Για να γίνει εκτέλεση (με χρήση Valgrind): `$make val`
- Για καθαρισμό του directory: `$make clean`
#
_Σχεδιαστικές επιλογές:_

- Ο επιλεγμένος τρόπος IPC είναι, όπως και στην προηγούμενη εργασία, η χρήση shared memory, που συγχρονίζεται με χρήση σημαφόρων.
- Η διαμοιραζόμενη μνήμη in-ds έχει μέγεθος ενός struct που περιέχει ένα p\_id (του αποστολέα Ρ) και ένα char[] που περιέχει το line που επιλέχθηκε τυχαία από το αρχείο.
- Η διαμοιραζόμενη μνήμη out-ds έχει μέγεθος ενός struct που περιέχει ένα p\_id (του αποστολέα Ρ) και ένα int που περιέχει το hash.
- Η default τιμή για το πλήθος Ν των producers έχει οριστεί στο makefile, αλλά αλλάζει με επεξεργασία του makefile. Επιπλέον, αν δεν δωθεί από τον χρήστη έχει την τιμή που φαίνεται στο main.c
- Η  default τιμή για το πλήθος Κ των επαναλήψεων έχει οριστεί στο makefile, αλλά αλλάζει με επεξεργασία του makefile. Επιπλέον, αν δεν δωθεί από τον χρήστη έχει την τιμή που φαίνεται στο main.c
- Η συνάρτηση md5 έχει εισαχθεί μέσω της βιβλιοθήκης openssl.
- Στη διαμοιραζόμενη μνήμη χωράει κάθε φορά ένα μήνυμα in και ένα out, συνεπώς δεν χρειάζεται να γίνει συγχρονισμός με mutexes, λαμβάνοντας υπόψιν ότι κάθε producer μπορεί να γράψει νέο msgin στην μνήμη μόνο εφόσον έχει παραλάβει κάποιο από την μνήμη out-ds και το έχει τυπώσει.
- Επιπλέον στην διαμοιραζόμενη μνήμη υπάρχει ένας πίνακας από int, όπου κάθε Ρ στο τέλος της ζωής της γράφει την τιμή του μετρητή της. Αυτός ο πίνακας έπειτα διαβάζεται από την C, όπου και γίνεται η άθροιση των μετρητών που ζητείται.

#
_Βιβλιογραφία_

1. Σημειώσεις μαθήματος
2. Aρχεία προηγούμενης εργασίας μου – shm.c, shm.h, Semun.c, Semun.h
3. [MD5 man page](https://www.openssl.org/docs/man1.1.0/man3/MD5_Init.html)
