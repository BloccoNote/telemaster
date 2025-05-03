il telemaster è un progettino arduinico che consiste in un sistema "di informazione" per il master di una campagna di dnd verso i giocatori.
il master ha una pulsantiera dove può accendere e scegliere cosa mostrare sullo schermino rivolto verso i giocatori.
è stato ideato per intrattenere i giocatori quando il master deve cercare su internet/tra i suoi appunti.
è alimentato da una batteria al litio 18650 e caricato con 5v usb

ho usato i seguenti pezzi:
    -arduino nano                       (1.50€)
    -lettore di schede sd per arduino   (1€)
    -schermo 16x2 con modulo I2C        (2.50€)
    -4 bottoni                          (10€)
    -4 resistenze da 10k                (0.05€)
    -1 interruttore x alimentazione     (2.50€)
    -caricatore tp4056                  (0.50€)
    -batteria 18650 di un portatile     (0€)
    -scheda forata, stagno, cavetti     (<1€)

i 4 bottoni hanno le seguenti funzioni:
    1) stampa sullo schermino "attendere prego"
    2) stampa sullo schermino un "slideshow" di consigli e suggerimenti inseriti nella scheda sd.
    3) stampa sullo schermino "SIETE FOTTUTI!!" lampeggiando
    4) spegne lo schermo senza spegnere il sistema
    5) interruttore per spegnere il sistema

formattazione file di testo:
le stringhe non possono eccedere x caratteri, e non ci possono essere singole parole più lunghe di 16 caratteri
il file si deve chiamare x.txt ed essere formattato così

001 stringa 1
002 stringa 2
003 stringa 3

