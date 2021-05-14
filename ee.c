#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>

/*

Leonardo Javier Malcotti

      ___          ___
     /\  \        /\  \
    /::\  \      /::\  \
   /:/\:\  \    /:/\:\  \
  /::\~\:\  \  /::\~\:\  \
 /:/\:\ \:\__\/:/\:\ \:\__\
 \:\~\:\ \/__/\:\~\:\ \/__/
  \:\ \:\__\   \:\ \:\__\
   \:\ \/__/    \:\ \/__/
    \:\__\       \:\__\
     \/__/        \/__/


ee - Easy Editor

*/

#define CTRL_KEY(k) ((k) & 0x1f)

//Struttura contenente i flag originali del terminale
struct termios orig_termios;

//Stampa un messaggio d'errore e esce dal programma
void die(const char *s) {
    //controlla la variabile globale errno e ne stampa il messaggio descrittivo, assieme alla stringa s
    perror(s);
    exit(1);
}

//Fa tornare i flag al loro stato originale
void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) die("tcsetattr");
}


//Disattiva i flag necessari per entrare in raw mode
//
//crea una istanza della struttura termios per leggere le flag del terminale
//per leggere le flag viene utilizzato tcgetattr() con un riferimento alla struttura sopradefinita
//alla fine verranno riscritti usando tcsetattr()
//
//  disattiva ECHO : raw.c_lflag &= ~(ECHO);
//  ECHO serve a far stampare a schermo ciò che viene premuto sulla tastiera
//
//  disattiva ICANON : raw.c_lflag &= ~(ICANON);
//  Permette di disattivare la modalità canonica e quindi di ricevere un input byte per byte invece che per linea
//
//  disattiva ISIG : raw.c_lflag &= ~(ISIG);
//  Disattiva i segnali che possono essere mandati con ctrl-c ctrl-z
//
//  disattiva IXON : raw.c_iflag &= ~(IXON);
//  Disattivi i segnali che possono essere mandati con ctrl-s e ctrl-q
//
//  disattiva ICRNL : raw.c_iflag &= ~(ICRNL);
//  Sistema il funzionamento di ctrl-m
//
//  disattiva OPOST : raw.c_oflag &= ~(OPOST);
//  Disattiva la conversione automatica di \n in \r\n, per andare a caporiga sarà necessario scrivere \r\n
//
//  vengono disattivate/modificate anche BRKINT, INPCK, ISTRIP e CS8 per tradizione nel passaggio a raw mode
//
void enableRawMode() {
    //salva i setting dei flag originali
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    //imposta la chiamata alla funzione per ripristinare i flag alla uscita del programma
    atexit(disableRawMode);


    struct termios raw = orig_termios;

    //modifica le flag di input
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

    raw.c_cflag |= (CS8);

    //modifica le flag di output
    raw.c_oflag &= ~(OPOST);

    //modifica le flag locali
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);

    //non davvero sicuro di aver capito l'utilità di questi
    //raw.c_cc[VMIN] = 0;
    //raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

/*--- MAIN --- */

int main(int argc, char *argv[]){

	return 0;
}

