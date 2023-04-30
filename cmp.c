#include <stdio.h>
#include <string.h>
#include <ctype.h>

long get_file_size(char *filename);
int main(int argc, char *argv[]) {
    if ((argc < 3) || (argc > 5)) {
        printf("The usage should be: cmp <file1> <file2> than [-v]/[-i]\n");
        return 1;
    }

    char *file1 = argv[1];
    char *file2 = argv[2];
    int verbose = 0;             // default for -v flag
    int ignore_case = 0;         // default for -i flag

    // this loop checking for which flags the  user requested to use.
    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        } else if (strcmp(argv[i], "-i") == 0) {
            ignore_case = 1;
        }
    }

    //opening the files for reading
    FILE *f1Read, *f2Read;
    f1Read = fopen(file1, "rb");
    f2Read = fopen(file2, "rb");

    if (f1Read == NULL || f2Read == NULL) {
        printf("Error in opening one or both files\n");
        return 1;
    }

    int equal = 1;
    int c1, c2;

    // checking letter by letter
    while (fread(&c1, sizeof(char), 1, f1Read) == 1 && fread(&c2, sizeof(char), 1, f2Read) == 1) {
        if (ignore_case) {
            c1 = tolower(c1);     // because of -i flag We will make all characters lowercase
            c2 = tolower(c2);
        }

        if (c1 != c2) {          //And here if there was the same letter it came out as equal
            equal = 0;
            break;
        }
    }

    //If we have finished going through all the letters in the file and there is still equality
    if (equal && fgetc(f1Read) == EOF && fgetc(f2Read) == EOF && get_file_size(file1) == get_file_size(file2)) {
        if (verbose) {
            printf("equal\n");
        }
        return 0;
    } else {
        if (verbose) {
            printf("distinct\n");
        }
        return 1;
    }
    fclose(f1Read);    //The fclose() function closes a stream pointed to by stream
    fclose(f2Read);
}

long get_file_size(char *filename) {
    FILE *fp = fopen(filename, "r");

    if (fp==NULL)
        return -1;

    if (fseek(fp, 0, SEEK_END) < 0) {
        fclose(fp);
        return -1;
    }

    long size = ftell(fp);
    // release the resources when not required
    fclose(fp);
    return size;
}