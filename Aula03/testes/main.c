#include<stdio.h>
#include<stdlib.h>

extern char *readLine(FILE *,int);

int main(int argc, char *argv[]){

    printf("Hello, World!\n");

    char *line = readLine(stdin, '\n');
    printf("%s\n", line);
    free(line);

    return 0;
}

/*
preprocessing
    cc -E hello_world.c
    resolves macros, adds shit, prepares for translation to .as
    output is .c if one wants it, rearely useful
compilation
    cc -S hello_world.c
    translates preprocessed .c to incomplete .s (assembly)
    architecture-dependant?
assembling
    cc -c hello_world.c
    creates incomplete object code .o
    not linked to other .o such as libraries
    object code is incomplete machine code with tags for linking!
linking
    cc hello_world.c
    links the various .o that are part of the project
        (custom libraries, various .c ,etc)
    also here static vs dynamic linking happens
        -static linking -> the final exec has all the needed code
        -dynamic linking-> the final exec calls for pieces of code alredy loaded by the OS
    one can specify Archives to be used as libs.
        $ ar cr arc.a in1.o in2.o ... (cr = create opt)
        $ cc -o exec input.o -L./dirToSearchForLib -llibname1
*/

/*
DÚVIDAS
- como funciona esse último argumento -ltest ? qual o padrão?
    - https://youtu.be/_OGqNbB_1yg?t=1733
    - se eu quiser fazer o mesmo com um arquivo chamado libfoo.c->lib1.o? o q devo usar? -lfoo ???


*/
