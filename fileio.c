#include <stdio.h>
#include <string.h>
//#include <cjson/cJSON.h>

void writeFile(FILE *fin, FILE *fout, char *flag, char buff[]);

int main() {

    int a,b,i,review;
    char rating,buff[156];
    char *flag;

    // Opening files
    FILE *fin = fopen("goodreads_reviews_spoiler_raw.json", "r");
    if (fin == NULL) {

        printf("Error: Unable to open the file in.\n");
        return 1;
    }
    FILE *fout = fopen("deneme7.txt", "w");
    if (fout == NULL) {
        printf("Error: Unable to open the file out.\n");
        return 1;
    }

    //writing on output file (fout)
    a = 0; b = 0; review = 0;
    while(a<500 || b<500){
        if(fgetc(fin) == '{'){
            review++;

            fgets(buff,156,fin);
            flag = strstr(buff,"\"rating\": ");
            i=10; //"\"rating\": ".length()
            rating = *(flag+i);

            if(rating - '0' <= 3 && a <500){
                a++;
                fputs("-1 ", fout);
                writeFile(fin, fout, flag, buff);
            }
            else if(rating - '0' > 3 && b <500){
                b++;
                fputs("1 ", fout);
                writeFile(fin, fout, flag, buff);
            }
        }
    }

    fclose(fout);fclose(fin);

    printf(" A class(Positive): %d, B class(Negative): %d, Review: %d\n",b,a,review);
    //scanf("%c",&a); to see the result

    return 0;
}

// write text flag to flag2
void writeFile(FILE *fin, FILE *fout, char *flag, char buff[]){

    char *flag2 = NULL;
    flag += 29; //flag needs to be added 29 to reach text
    int i;
    while(!flag2){
        i = 0;

        flag2 = strstr(buff,"\", \"d");
        if(flag2)
            while(fputc(*(flag + i++), fout) && (flag + i) < flag2);
        else{
            while(*(flag + i))
                fputc(*(flag + i++), fout);
            fgets(buff,156,fin);
            flag = buff;
        }


    }
    fputc('\n', fout);
}