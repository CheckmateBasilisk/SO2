#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<utils.h>

//returns a null-terminated,dynamically allocated string, tokenized by delimiter, read from stream
// returns NULL if EOF was found
char *readLine(FILE *stream, int delimiter){
    char *line = NULL;
    char c = '\0';
    int counter = 0;

    do{
        c = fgetc(stream);
        if(feof(stream)){
            if(line != NULL) free(line);
            return NULL; //EOF met! aborting
        }
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

//returns a null-terminated,dynamically allocated string, with the contents of str
char *copyString(char *str){
    if(str == NULL) return NULL;

    char *copy = (char *) malloc(sizeof(char)*strlen(str)+1);
    strncpy(copy,str,strlen(str)+1);//+1 for the \0
    return copy;
}

//counts how many 'c' appear at the beginning of str
int countWhiteSpaces(char *str, int c){
    if(str == NULL) return 0;
    int i;
    for(i = 0; str[i]== c; i++);
    return i;
}

//tokenizes the src string according to delimiter
//returns a null-terminated dynamically allocated array of null-terminated dyn-allocated strings
//does not disturb the original string
//foo bar -> {"foo","bar",NULL} , rest = NULL
//if fullstopDelimiter is met, the rest will point to a copy of the rest of the string and the tokenization process will end.
// if not, rest will be set to NULL
// the array of tokens will still be NULL terminated.
//foo bar | boo -> {"foo", "bar", NULL}, rest = "boo"
// WILL change the contents of rest regardless!!!
//TODO  strToWords('ls', ' ') does not work!
// it puts somthing bizarre between the last word and null {'ls', ???, NULL}
//TODO for some reason '.' breaks the tokenization on its tracks "foo . bar" => {"foo", ".", NULL}
//TODO honestly this looks a little disgusting... :c
char **strToWordsWithFullstop(char *src, int delimiter, int fullstopDelimiter, char **rest){
    *rest = NULL;
    char **word = NULL;
    char *newWord;//just a temporary holder for each word
    int i = 0;//counts through src
    int wordcount = 0;

    //gets the words
    do{
        word = (char **) realloc(word, sizeof(char*) * (wordcount+1));
        //skips trailing delimiters before continuing
        i += countWhiteSpaces(&src[i], delimiter);
        newWord = tokenize(&src[i], delimiter);
        word[wordcount] = newWord;
        wordcount += 1;
        i += strlen(newWord);
        //if the next word would start with fullstopDelimiter
        // eat the fullsotp delimiter and put the rest of the string on the *rest variable
        //if didnt reach the end but reached a fsDelimiter
        if(src[i] != '\0' && src[i+countWhiteSpaces(&src[i], delimiter)] == fullstopDelimiter){
            *rest = copyString(&src[i+1]);
            break;
        }
    //while the last word tokenized isnt "\0"
    }while( src[i] != '\0');
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

//returns a trimmed null-terminated, dynamically allocated string
//trims all 'c' from the beginning of str if side == 'l'
//trims all 'c' from the end of str if side == 'r'
// trim("    foo", ' ', 'l') -> "foo"
char *trim(char *str, int c, char side){
    int i;
    char *trimmed;
    if(str == NULL) return NULL;
    if(side == 'l'){
        for(i=0; i<strlen(str) && str[i]==c; i++);
        trimmed = copyString(&str[i]);
    } else if (side == 'r'){
        for(i=strlen(str)-1; i>=0 && str[i]==c; i--);
        trimmed = copyString(str);
        trimmed[i+1] = '\0';
        //readjusting allocated space
        trimmed = (char *) realloc(trimmed, sizeof(char)*strlen(trimmed));
    }

    return trimmed;
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
//TODO Will not skip repeated delimiters
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
