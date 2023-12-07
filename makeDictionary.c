#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isLetter(char c);
int checkDict(char *word, FILE *fin);
void addDict(FILE *fout,char *word);

int main() {

    char letter, str[50]={0};

    // Opening files
    FILE *fin = fopen("train.txt", "r");
    if (fin == NULL) {

        printf("Error: Unable to open the file in.\n");
        return 1;
    }
    //deleting old file
    FILE *file = fopen("dictionary.txt", "w");
    if (file == NULL) {
        printf("Error: Unable to open the file out.\n");
        return 1;
    }
    fclose(file);
    //
    FILE *fout = fopen("dictionary.txt", "a+");
    if (fout == NULL) {
        printf("Error: Unable to open the file out.\n");
        return 1;
    }

    // checking a words, then add to the dictionary
    while((letter = fgetc(fin)) != EOF){
        if(isLetter(letter)){
            letter = tolower(letter);
            strncat(str, &letter, 1);
        }
        else if(letter == ' '){
            if( str[0] != 0 && !checkDict(str,fout))
                addDict(fout, str);

            str[0] = 0;
        }
    }

    fclose(fin);
    fclose(fout);
}

//checks the char if it is letter returns 1 else returns 0
int isLetter(char c){
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c<= 'z'))
        return 1;
    return 0;
}

// checks Dictionary. if it finds the word returns 1 else returns 0
int checkDict(char *word, FILE *fin){
    int flag1,isSame;
    char str[60];

    rewind(fin);
    do{
        str[0] = 0;
        flag1 = fscanf(fin,"%s",str);
        isSame = strcmp(str,word);
    }while(flag1 !=-1 && isSame);

    if(isSame == 0)
        return 1;
    return 0;
}

// appends the word into dictionary.txt
void addDict(FILE *fout,char *word){
        strncat(word, "\n", 1);
        fputs(word, fout);
        rewind(fout);

}