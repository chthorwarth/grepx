#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Zu testende Funktion */
void sayHello()
{
    printf("Hello World\n");
}

/* Test */
int test_sayHello()
{
    /* 1. Original stdout merken */
    FILE *originalStdout = stdout;

    /* 2. Temporäre Datei erstellen */
    FILE *tmp = tmpfile();
    if (!tmp)
        return 0;

    /* 3. stdout auf Datei umleiten */
    stdout = tmp;

    /* 4. Funktion aufrufen */
    sayHello();

    /* 5. Datei zum Lesen vorbereiten */
    fflush(tmp);
    fseek(tmp, 0, SEEK_SET);

    /* 6. Ausgabe prüfen */
    char buffer[100];
    fgets(buffer, sizeof(buffer), tmp);

    /* 7. Aufräumen */
    fclose(tmp);
    stdout = originalStdout;

    /* 8. Test-Ergebnis */
    return strcmp(buffer, "Hello World\n") == 0;
}

int main()
{
    if (test_sayHello())
        printf("TEST SUCCEEDED\n");
    else
        printf("TEST FAILED\n");

    return 0;
}