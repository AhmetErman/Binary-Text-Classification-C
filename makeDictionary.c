#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void toLowerStr(char *p);
int isLetter(char c);
int checkDict(char *word);
void addDict(FILE *fout,char *word);

int main() {

    char letter, str[50]={0};

    // Opening files
    FILE *fin = fopen("dataset.txt", "r");
    if (fin == NULL) {

        printf("Error: Unable to open the file in.\n");
        return 1;
    }
    FILE *fout = fopen("dictionaryTest.txt", "a");
    if (fout == NULL) {
        printf("Error: Unable to open the file out.\n");
        return 1;
    }

    while((letter = fgetc(fin)) != EOF){

        if( letter == ' '){
            // write a word
            while((letter = fgetc(fin)) != EOF){
                if(isLetter(letter)){
                    letter = tolower(letter);
                    strncat(str, &letter, 1);
                }
                else if(letter == ' '){
                    if(!checkDict(str))
                        addDict(fout, str);

                    str[0] = 0;
                }

            }
            str[0] = 0;
        }
    }

    fclose(fin);
    fclose(fout);
}

// çalıntıdır
void toLowerStr(char *p){
    for ( ; *p; ++p) *p = tolower(*p);
}

int isLetter(char c){
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c<= 'z'))
        return 1;
    return 0;
}

// checks Dictionary. if it finds the word returns 1
int checkDict(char *word){
    int flag1,isSame;
    char str[100];
    FILE *fin;
    fin = fopen("dictionaryTest.txt", "r");

    if (fin == NULL) {
        printf("Error: Unable to open the file out.\n");
        return 1;
    }

    do{
        str[0] = 0;
        flag1 = fscanf(fin,"%s",str);
        isSame = strcmp(str,word);
        
    }while(flag1 !=-1 && isSame);// sıkıntı

    fclose(fin);

    if(isSame == 0)
        return 1;
    return 0;
}

void addDict(FILE *fout,char *word){
    strncat(word, "\n", 1);
    fputs(word, fout);
    rewind(fout);
}