#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    if ((argc < 3) || (argc > 5)){
        printf("The usage should be: cmp <file1> <file2> than an option [-v]/[-i]\n");
        return 1;
    }

    char* file1 = argv[1];
    char* file2 = argv[2];
    int verbose = 0;             // default for -v flag
    int force = 0;               // default for -f flag

    // this loop checking for which flags the  user requested to use.
    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        } else if (strcmp(argv[i], "-f") == 0) {
            force = 1;
        }
    }

    //opening the files for reading
    FILE *f1Read, *f2Read;
    f1Read = fopen(file1, "rb");
    f2Read = fopen(file2, "rb");

    if (f2Read != NULL && !force) {
        printf("Error: target file exists\n");
        return 1;
    }

    FILE* f2Write = fopen(file2, "wb");          // open the second file for writing

    if (f1Read == NULL || f2Write == NULL) {
        printf("Error in opening one or both files\n");
        return 1;
    }

    // read from file1 and write to file2
    char buffer[BUFSIZ];
    size_t n;

    while ((n = fread(buffer, 1, BUFSIZ, f1Read)) > 0) {
        fwrite(buffer, 1, n, f2Write);
    }

    // close the files
    fclose(f1Read);
    fclose(f2Write);

    if (verbose) {
        printf("success\n");
    }

    return 0;
}