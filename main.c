/*********************************************************
*
* @Propòsit: gestionar les bandes, els membres d'aquestes, i
* els llocs dels concerts per facilitar la organització d'un
* festival
* @Autor/s: Gerard Vives Pérez
* @Data de creació: 07/05/2021
* @Data de l’última modificació: 09/05/2021
*
*********************************************************/

// Standard Libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// User-defined libraries
#include "linkedlist.h"

// Constants
#define MAX_FILENAME_LEN 30
#define MAX_NUM_ENTRIES_LEN 5
#define MAX_NAME_LEN 50
#define MAX_CAPACITY_LEN 10
#define MAX_AGE_LEN 5
#define MAX_CACHE_LEN 10


// User-defined types
typedef struct {
    char name[MAX_NAME_LEN];
    char band[MAX_NAME_LEN];
    int age;
} MusicianEntry;

typedef struct {
    char name[MAX_NAME_LEN];
    int age;
} Musician;

typedef struct {
    char name[MAX_NAME_LEN];
    int cache;
    int num_musicians;
    Musician *musicians;
} Band;

// Venue data type imported from linkedlist module
// (linkedlist.h)


/***********************************************
*
* @Finalitat: eliminar el caràcter que indica una nova
* línia al final d'una cadena, si aquest existeix
* @Paràmetres: 
* in: line = cadena que s'ha de modificar
* @Retorn: none
*
************************************************/
void removeNewlineChar(char *line) {
    int new_line = 0;
    
    new_line = strlen(line) - 1;
    if (line[new_line] == '\n')
        line[new_line] = '\0';
}


/***********************************************
*
* @Finalitat: obtenir totes les bandes a partir de la lectura
* d'un fitxer, guardar la informació en un bloc de memòria
* dinàmica i assignar memòria pels membres de cada banda
* @Paràmetres:
* in: src = punter al wfitxer que s'ha carregat a la memòria
* in: num_bands = punter a número de bandes que es guarden
* @Retorn: bands = punter a bloc de memòria on s'emmagatzemen 
* totes les bandes
*
************************************************/
Band *getBands(FILE *src, int *num_bands) {
    char str_num_bands[MAX_NUM_ENTRIES_LEN];
    char str_cache[MAX_CACHE_LEN];
    char str_num_musicians[MAX_NUM_ENTRIES_LEN];
    int size_to_allocate = 0;
    int num_musicians = 0;
    int is_bad_alloc = 0;
    int i = 0, j = 0;
    Band *bands;

    fgets(str_num_bands, MAX_NUM_ENTRIES_LEN, src);
    removeNewlineChar(str_num_bands);
    *num_bands = atoi(str_num_bands);

    bands = (Band *)malloc(sizeof(Band) * (*num_bands));
    // If the memory couldn't be allocated, return a NULL
    // pointer
    if (bands != NULL) {
        for (i = 0; i < *num_bands && !is_bad_alloc; i++) {
            // Get name of band and write
            fgets(bands[i].name, MAX_NAME_LEN, src);
            removeNewlineChar(bands[i].name);

            // Get cache of band, format, and write
            fgets(str_cache, MAX_CACHE_LEN, src);
            removeNewlineChar(str_cache);
            bands[i].cache = atoi(str_cache);

            // Get number of musicians, format, and write
            fgets(str_num_musicians, MAX_NUM_ENTRIES_LEN, src);
            removeNewlineChar(str_num_musicians);
            num_musicians = atoi(str_num_musicians);
            bands[i].num_musicians = num_musicians;

            // Allocate block of dynamic memory to store band's
            // members
            size_to_allocate = sizeof(Musician) * num_musicians;
            bands[i].musicians = (Musician *)malloc(size_to_allocate);
            // If a block of memory couldn't be allocated to store the
            // musicians, the program won't work as expected. Therefore,
            // free the memory allocated to band and set the pointer
            // to NULL to stop the execution of other functions later on
            // main()
            if (bands[i].musicians == NULL) {
                for (j = 0; j < i; j++) {
                    free(bands[j].musicians);
                }
                free(bands);
                bands = NULL;
                is_bad_alloc = 1;
            }
        }
    }
    
    return bands;
}


/***********************************************
*
* @Finalitat: obtenir tots els músics a partir de la lectura
* d'un fitxer i guardar-los en blocs de memòria dinàmica
* diferents en funció de la banda a la qual pertanyen
* @Paràmetres: 
* in: src = punter al fitxer que s'ha carregat a la memòria
* in: bands = punter a bloc de memòria on s'emmagatzemen totes 
* les bandes
* in: num_bands = punter a número de bandes que es guarden
* @Retorn: none
*
************************************************/
void getMusicians(FILE *src, Band *bands, int *num_bands) {
    char str_num_musicians[MAX_NUM_ENTRIES_LEN];
    char str_age[MAX_CAPACITY_LEN];
    char buffer[MAX_NUM_ENTRIES_LEN];
    int num_musicians = 0;
    int i = 0, j = 0, k = 0;
    MusicianEntry musician_entry;

    fgets(str_num_musicians, MAX_NUM_ENTRIES_LEN, src);
    removeNewlineChar(str_num_musicians);
    num_musicians = atoi(str_num_musicians);

    for (j = 0; j < *num_bands; j++) {
        k = 0;
        // Set the position indicator to the beginning of the
        // file every time it has to be read again
        fseek(src, 0, SEEK_SET);
        // Skip the first line every time we re-read the file
        // It contains the number of musicians, which is already
        // known
        fgets(buffer, MAX_NUM_ENTRIES_LEN, src);

        for (i = 0; i < num_musicians; i++) {
            fgets(musician_entry.name, MAX_NAME_LEN, src);
            removeNewlineChar(musician_entry.name);

            fgets(musician_entry.band, MAX_NAME_LEN, src);
            removeNewlineChar(musician_entry.band);

            fgets(str_age, MAX_AGE_LEN, src);
            removeNewlineChar(str_age);
            musician_entry.age = atoi(str_age);

            if (!strcmp(musician_entry.band, bands[j].name)) {
                strcpy(bands[j].musicians[k].name, musician_entry.name);
                bands[j].musicians[k].age = musician_entry.age;
                // If a member of the band has been found and stored, jump
                // to the next position of the array
                k++;
            }
        }
    }
}


/***********************************************
*
* @Finalitat: obtenir tots els llocs per fer el festival a
* partir de la lectura d'un fitxer i guardar la informació
* en una linked list dinàmica (estructura de dades lineal)
* @Paràmetres: 
* in: src = punter al fitxer que s'ha carregat a la memòria
* in: venues = punter a linked list on s'emmagatzemen tots
* els escenaris
* @Retorn: none
*
************************************************/
void getVenues(FILE *src, LinkedList *venues) {
    char str_capacity[MAX_CAPACITY_LEN];
    Element el;

    LINKEDLIST_goToHead(venues);

    fgets(el.name, MAX_VENUE_NAME_LEN, src);
    removeNewlineChar(el.name);

    fgets(str_capacity, MAX_CAPACITY_LEN, src);
    removeNewlineChar(str_capacity);
    el.capacity = atoi(str_capacity);

    while (!feof(src)) {
        LINKEDLIST_add(venues, el);

        fgets(el.name, MAX_VENUE_NAME_LEN, src);
        removeNewlineChar(el.name);

        fgets(str_capacity, MAX_CAPACITY_LEN, src);
        removeNewlineChar(str_capacity);
        el.capacity = atoi(str_capacity);
    }
}


/***********************************************
*
* @Finalitat: mostrar les bandes o els llocs dels festivals
* en funció de què desitgi l'usuari
* @Paràmetres: 
* in: bands = punter a bloc de memòria on s'emmagatzemen totes 
* les bandes
* in: venues = punter a linked list on s'emmagatzemen tots
* els escenaris
* in: num_bands = punter a número de bandes que es guarden
* @Retorn: none
*
************************************************/
void showBandsVenues(Band *bands, LinkedList *venues, int *num_bands) {
    int selection = 0;
    int i = 0;
    Element e;

    do {
        printf("\n1. Show bands | 2. Show venues\n");
        printf("Select option: ");
        scanf("%d", &selection);
        if (selection < 1 || selection > 2) {
            printf("ERROR: Wrong option number\n");
        }
    } while (selection < 1 || selection > 2);

    if (selection == 1) {
        printf("\nBands:\n");

        for (i = 0; i < *num_bands; i++) {
            printf("\t%d - %s\n", i+1, bands[i].name);
        }
    }
    else if (selection == 2) {
        printf("\nVenues:\n");

        // Make sure we are at head to start reading from
        // the first line of the file
        LINKEDLIST_goToHead(venues);

        i = 1;
        while (!LINKEDLIST_isAtEnd(*venues)) {
            e = LINKEDLIST_get(venues);

            if (LINKEDLIST_getErrorCode(*venues) == LIST_NO_ERROR) {
                printf("\t%d - %s\n", i, e.name);
                LINKEDLIST_next(venues);
                i++;
            }
        }
    }
}



/***********************************************
*
* @Finalitat: calcular el preu de cada ticket en funció
* del que pretén guanyar la banda i la capacitat de cada
* lloc (es permet escollir a l'usuari quina banda i lloc
* vol escollir)
* @Paràmetres: 
* in: bands = punter a bloc de memòria on s'emmagatzemen totes 
* les bandes
* in: venues = punter a linked list on s'emmagatzemen tots
* els escenaris
* in: num_bands = punter a número de bandes que es guarden
* @Retorn: none
*
************************************************/
void calculateTicketPrice(Band *bands, LinkedList *venues, int *num_bands) {
    int band_sel = 0, venue_sel = 0;
    float ticket_price = 0;
    int i = 0;
    int pov_out = 0;
    int found = 0;
    Element e;

    do {
        printf("\nSelect band number: ");
        scanf("%d", &band_sel);

        if (band_sel > *num_bands || band_sel < 1) {
            printf("ERROR: Invalid band number\n");
        }
    } while (band_sel > *num_bands || band_sel < 1);

    do {
        printf("\nSelect venue number: ");
        scanf("%d", &venue_sel);

        found = 0;
        i = 0;
        pov_out = 0;

        LINKEDLIST_goToHead(venues);
        while (!found) {
            e = LINKEDLIST_get(venues);
            if (!LINKEDLIST_isAtEnd(*venues)) {
                if (i+1 == venue_sel) {
                    found = 1;
                }
                else {
                    LINKEDLIST_next(venues);
                    i++;
                }
            }
            else {
                found = 1;
                pov_out = 1;
                printf("ERROR: Invalid venue number\n");
            }
        }
    } while (pov_out);

    // Type-cast (explicit) from int to float the cache of the band
    // and the capacity of the venue to get the ticket price as float
    ticket_price = (float)bands[band_sel-1].cache/(float)e.capacity;

    printf("\nThe minimum ticket price is %.2f euros\n", ticket_price);
}


/***********************************************
*
* @Finalitat: mostrar els membres que pertanyen a una banda
* escollida per l'usuari
* @Paràmetres: 
* in: bands = punter a bloc de memòria on s'emmagatzemen totes 
* les bandes
* in: num_bands = punter a número de bandes que es guarden
* @Retorn: none
*
************************************************/
void showBandMembers(Band *bands, int *num_bands) {
    int band_sel = 0;
    int i = 0;

    do {
        printf("\nSelect band number: ");
        scanf("%d", &band_sel);

        if (band_sel > *num_bands || band_sel < 1) {
            printf("ERROR: Invalid band number\n");
        }
    } while (band_sel > *num_bands || band_sel < 1);

    printf("\nMembers:\n");
    for (i = 0; i < bands[band_sel-1].num_musicians; i++) {
        printf("\t- %s\n", bands[band_sel-1].musicians[i].name);
    }
}


/***********************************************
*
* @Finalitat: alliberar tota la memòria dinàmica assignada
* @Paràmetres: 
* in: bands = punter a bloc de memòria on s'emmagatzemen totes 
* les bandes
* in: num_bands = punter a número de bandes que es guarden
* @Retorn: none
*
************************************************/
void freeMemory(Band *bands, int *num_bands) {
    int i = 0;

    if (bands != NULL) {
        // Free blocks of memory storing the band members
        for (i = 0; i < *num_bands; i++) {
            free(bands[i].musicians);
            bands[i].musicians = NULL;
        }
    }
    // If we pass a NULL pointer to free, nothing happens
    // Therefore, there is no problem deallocating pointers
    // which were set to NULL when malloc() couldn't allocate
    // the memory
    free(bands);

    // Set pointers to NULL to avoid memory leaks (or accessing
    // memory which is not allocated to the script)
    bands = NULL;
}


/***********************************************
*
* @Finalitat: obrir un fitxer per llegir-ne el contingut
* @Paràmetres: 
* in: filename = cadena amb el nom del fitxer que s'ha d'obrir
* @Retorn: fp = punter al fitxer que s'ha carregat
* a la memòria
*
************************************************/
FILE *openFile(char *filename) {
    FILE *fp = NULL;

    fp = fopen(filename, "r");

    return fp;
}


/***********************************************
*
* @Finalitat: llegir totes les dades dels fitxers,
* cridar les funcions per emmagatzamar-les i gestionar
* el menú principal de l'aplicació
* @Paràmetres: none
* @Retorn: 0
*
************************************************/
int main() {
    Band *bands;
    LinkedList venues;
    FILE *read_fp = NULL;
    char filename[MAX_FILENAME_LEN];
    int num_bands = 0;
    int stop = 0;
    int selection = 0;

    printf("Welcome!\n\n");
    printf("Introduce the file names:\n");

    while (read_fp == NULL) {
        printf("\t- Bands: ");
        fgets(filename, MAX_FILENAME_LEN, stdin);
        removeNewlineChar(filename);

        read_fp = openFile(filename);
        if (read_fp == NULL) {
            printf("ERROR: Can't open file '%s'\n", filename);
        }
    }
    bands = getBands(read_fp, &num_bands);
    // Once the file has been read, close it and set the pointer
    // to NULL so that the following loop isn't skipped
    fclose(read_fp);
    read_fp = NULL;

    if (bands != NULL) {
        while (read_fp == NULL) {
            printf("\t- Musicians: ");
            fgets(filename, MAX_FILENAME_LEN, stdin);
            removeNewlineChar(filename);

            read_fp = openFile(filename);
            if (read_fp == NULL) {
                printf("ERROR: Can't open file '%s'\n", filename);
            }
        }

        getMusicians(read_fp, bands, &num_bands);
        fclose(read_fp);
        read_fp = NULL;

        venues = LINKEDLIST_create();
        if (LINKEDLIST_getErrorCode(venues) == LIST_NO_ERROR) {
            while (read_fp == NULL) {
                printf("\t- Venues: ");
                fgets(filename, MAX_FILENAME_LEN, stdin);
                removeNewlineChar(filename);

                read_fp = openFile(filename);
                if (read_fp == NULL) {
                    printf("ERROR: Can't open file '%s'\n", filename);
                }
            } 
    
            getVenues(read_fp, &venues);
            fclose(read_fp);
            read_fp = NULL;

            if (!LINKEDLIST_isEmpty(venues)) {
                while (!stop) {
                    printf("\n1. Show bands or venues | 2. Calculate ticket price "
                           "| 3. Show band members | 4. Exit\n");
                    printf("Select option: ");
                    scanf("%d", &selection);

                    switch (selection) {
                    case 1:
                        showBandsVenues(bands, &venues, &num_bands);
                        break;
                    case 2:
                        calculateTicketPrice(bands, &venues, &num_bands);
                        break;
                    case 3:
                        showBandMembers(bands, &num_bands);
                        break;
                    case 4:
                        printf("\nBye!\n");
                        stop = 1;
                        break;
                    default:
                        printf("ERROR: Wrong option number\n");
                        break;
                    }
                }
            }      
            // Destroy venues linked list if successfully allocated memory
            LINKEDLIST_destroy(&venues);
        }
        // Free dynamic memory allocated (exceptions are handled)
        freeMemory(bands, &num_bands);
    }

    return 0;
}