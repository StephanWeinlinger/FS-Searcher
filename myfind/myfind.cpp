#include <iostream>

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <assert.h>

/* globale Variable fuer den Programmnamen */
char* program_name = NULL;

/* Funktion print_usage() zur Ausgabe der usage Meldung */
void print_usage()
{
    fprintf(stderr, "Usage: %s [-R] [-i] searchpath filename1 [filename2] …[filenameN]\n", program_name);
    exit(EXIT_FAILURE);
}

/* main Funktion mit Argumentbehandlung */
int main(int argc, char* argv[])
{
    int input = 0;
    int error = 0;
    char* inputFile = NULL;
    int flag_R = 0;
    int flag_i = 0;

    program_name = argv[0];

    while ((input = getopt(argc, argv, "Ri")) != EOF)
    {
        switch (input)
        {
        case 'R': /* Option Rekursive Suche */
            if (flag_R) /* mehrmalige Verwendung? */
            {
                error = 1;
                break;
            }
            flag_R = 1;
            printf("%s: parsing option R(recursive mode)\n", program_name);
            break;
        case 'i': /* Option case in-sensetive */
            if (flag_i) /* mehrmalige Verwendung? */
            {
                error = 1;
                break;
            }
            flag_i = 1;
            printf("%s: parsing option i(case in-sensetive search)\n", program_name);
            break;
        case '?': /* unguelgtiges Argument */
            error = 1;
            break;
        default: /* unmoegliech */
            assert(0);
        }
    }
    if (error) /* Optionen fehlerhaft ? */
    {
        print_usage();
    }

    if (argc < optind + 1) /* falsche Anzahl an Optionen */
    {
        print_usage();
    }


   

    /* Die restlichen Argumente, die keine Optionen sind, befinden sich in
     * argv[optind] bis argv[argc-1]
     */
    while (optind < argc)
    {
        /* aktuelles Argument: argv[optind] */
        printf("%s: parsing argument %s\n", program_name, argv[optind]);

        optind++;
    }
    return EXIT_SUCCESS;
}
