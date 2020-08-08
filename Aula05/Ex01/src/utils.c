#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<utils.h>

//returns a null-terminated,dynamically allocated string, tokenized by delimiter, read from stream
char *readLine(FILE *stream, int delimiter){
    char *line = NULL;
    char c = '\0';
    int counter = 0;

    do{
        c = fgetc(stream);
        line = (char *) realloc(line, sizeof(char) * (counter+1));
        line[counter] = c;
        counter+=1;
    }while(c!=delimiter);
    line[counter-1] = '\0';

    return line;
}

//returns a null-terminated,dynamically allocated string, with the contents of str1 appended by str2
char *appendString(char *str1, char *str2){
    char *dest = (char *) malloc(sizeof(char) * (strlen(str1) + strlen(str2)) +1 ); //+1 for the \0
/*
    //getting contents of str1
    strncpy(dest, str1, sizeof(char), strlen(str1)
    char *strncpy(char *dest, const char *src, size_t n);
*/

    strcat(dest, str1);
    strcat(dest, str2);

    return dest;
}

//tokenizes the src string according to delimiter
//returns a null-terminated dynamically allocated array of null-terminated dyn-allocated strings
//does not disturb the original string
//foo bar -> {"foo","bar",NULL}
//TODO  strToWords('ls', ' ') does not work!
// it puts somthing bizarre between the last word and null {'ls', ???, NULL}
//TODO for some reason '.' breaks the tokenization on its tracks "foo . bar" => {"foo", ".", NULL}
char **strToWords(char *src, int delimiter){
    char **word = NULL;
    char *newWord;//just a temporary holder for each word
    int i = 0;//counts through src
    int wordcount = 0;
    do{
        word = (char **) realloc(word, sizeof(char*) * (wordcount+1));
        newWord = tokenize(&src[i], delimiter);
        word[wordcount] = newWord;
        i += strlen(newWord)+1;
        wordcount += 1;
    //while the last word tokenized isnt "\0"
    }while( src[i-1] != '\0');
    //watch out for mem leaks

    //if the only word is '\0' it shuld be substituted by the NULL terminator instead of adding it after
    //ad-hoc solution :/
    if(strcmp(word[0],"")==0){
        free(word[0]);
        word[0] = NULL;
        return word;
    }

    //adding null terminator to word
    word = (char **) realloc(word, sizeof(char*) * (wordcount+1));
    word[wordcount] = NULL;

    return word;
}

//returns a null-terminated dynamically allocated string, tokenizing src with delimiter
// tokenize("foo bar", delimiter = ' ') => "foo"
//Will not skip repeated delimiters
// "   bar" => " "
char *tokenize(char *src, int delimiter){
    char *token = NULL;
    int counter = 0;//token length
    char c;
    //dava pra fazer com memcpy :/
    do{
        token = (char *) realloc(token, sizeof(char) * (counter+1));
        c = src[counter];
        token[counter] = c;
        counter++;
    //keep going until what has been placed == delimiter
    }while(c != delimiter && c != '\0');
    token[counter-1] = '\0';

    return token;
}
