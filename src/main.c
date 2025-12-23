#include <stdio.h>
#include "files.h"
#include "search.h"

int main(/*int argc, char *argv[]*/) {

    /*if (argc == 3) {
        // Fall 1. Wir suchen nach spezieller Datei
        char *keyword = argv[1];
        char *filename = argv[2];
        char *content = getContent(filename);
        searchWord(filename, content);
        // Fall 2. Wir suchen nach mehreren Files
        char *keyword = argv[1];
        char *filepattern = argv[2];
        char *content = traverse(filename);
        searchWord(filename, content);
    }
*/

    //traverse("/Users/christianthorwarth/git");
    //searchWord();

    // 1. Dateisystem zugriff -> bla.txt

    // 2. Durchsuche bla.txt nach "Hallo"

    printf("Hello World\n");
    return 0;
}
