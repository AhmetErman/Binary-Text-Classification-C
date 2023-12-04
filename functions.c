#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX 160
#define DICTIONARY 5450

double f_x(bool x[], double w[], int n);
void gradient_descent(bool dataset[MAX][DICTIONARY], int y[], double W[], int m, int n, double learning_rate, int iteration);
void stochastic_gradient_descent(bool dataset[MAX][DICTIONARY], int y[], double W[], int m, int n, double learning_rate, int iteration);
void Adam(bool dataset[MAX][DICTIONARY], int y[], double W[], int m, int n, double learning_rate, int iteration);
double Loss(bool dataset[MAX][DICTIONARY], int y[], double W[], int m, int n);
int predict(int x[MAX], double W[MAX], int n);
void genW(double W[], float value);
void fillY(int y[MAX]);
void makeDataset(bool dataset[MAX][DICTIONARY]);

int isLetter(char c);
int appendStr(char* str,char letter);
int getWord(char *str, FILE *fin);
int countWords(FILE *fout);
int countOnes(int dataset[DICTIONARY]);
int checkDict(char *word, FILE *fin);
void printArr(int arr[]);
void printMat(int dataset[MAX][DICTIONARY]);

//halil
/*int main() {

    int x[] = {1,2,3};
    double w[] = {0.5,0.25,0.166};
    int n = 3;
    printf("%f\n", f_x(x, w, n));
    printf("%d", predict(x, w, n));

    return 0;
}*/

//ahmet
int main() {
    int y[MAX], i;
    double W[DICTIONARY];
    bool dataset[MAX][DICTIONARY];

    makeDataset(dataset);
    //printMat(dataset);
    fillY(y);
    //printArr(y);
    genW(W, 0);

    //gradient_descent(dataset,y,W,MAX,DICTIONARY,0.1,100);
    //stochastic_gradient_descent(dataset,y,W,MAX,DICTIONARY,0.1,100);
    //Adam(dataset, y, W, MAX, DICTIONARY, 0.08, 10);



    return 0;
}

void gradient_descent(bool dataset[MAX][DICTIONARY], int y[], double W[], int m, int n, double learning_rate, int iteration){

    int i=0, j, k, y_transformed;
    double f_der=0, y_hat_transformed;

    while(i<iteration) {
        for (j = 0; j < n; j++) {


            for (k = 0; k < m; k++) {

                y_transformed = (y[k]+1)/2;
                y_hat_transformed = (f_x(dataset[k], W, n)+1)/2;

                f_der += (y_hat_transformed - y_transformed) * dataset[k][j];//gradyan
            }
            f_der /= m;

            W[j] = W[j] - learning_rate * f_der;
            f_der = 0;
        }
        i++;
        printf("Epoch %d, Loss = %f\n", i, Loss(dataset, y, W, m, n));
    }
}

void stochastic_gradient_descent(bool dataset[MAX][DICTIONARY], int y[], double W[], int m, int n, double learning_rate, int iteration){

    int i=0, j, random, y_transformed;
    double f_der, y_hat_transformed;

    srand(time(NULL));

    while(i<iteration){

        random = rand() % m;

        y_transformed = (y[random]+1)/2;
        y_hat_transformed = (f_x(dataset[random], W, n)+1)/2;

        for(j=0;j<n;j++){

            f_der = (y_hat_transformed - y_transformed) * dataset[random][j];
            W[j] = W[j] - learning_rate * f_der;
        }
        i++;
        printf("Epoch %d, Loss = %lf\n", i, Loss(dataset, y, W, m, n));
    }
}

void Adam(bool dataset[MAX][DICTIONARY], int y[], double W[], int m, int n, double learning_rate, int iteration){

    int i=0, j, k, y_transformed;
    double f_der=0, y_hat_transformed, B1=0.9, B2=0.99, epsilon = 0.0001;
    double mt[DICTIONARY] = {0};
    double vt[DICTIONARY] = {0};

    while(i<iteration) {
        i++;

        for (j = 0; j < n; j++) {
            for (k = 0; k < m; k++) {

                y_transformed = (y[k] + 1) / 2;
                y_hat_transformed = (f_x(dataset[k], W, n) + 1) / 2;

                f_der += (y_hat_transformed - y_transformed) * dataset[k][j];
            }
            f_der /= m;

            mt[j] = B1*mt[j] + (1-B1)*f_der;
            vt[j] = B2*vt[j] + (1-B2)*pow(f_der, 2);

            mt[j] = mt[j] / (1-pow(B1, i));
            vt[j] = vt[j] / (1-pow(B2, i));

            W[j] = W[j] - learning_rate * (mt[j] / (sqrt(vt[j]) + epsilon));
            f_der = 0;
        }
        printf("Epoch %d, Loss = %lf\n", i, Loss(dataset, y, W, m, n));
    }

}

double f_x(bool x[], double W[], int n){

    double WX = 0, a;
    int i;

    for(i=0;i<n;i++) {
        WX += W[i] * x[i];
    }
    a = tanh(WX);

    if(a==1)
        a -= 0.000001;
    else if(a==-1)
        a += 0.000001;

    return a;
}

double Loss(bool dataset[MAX][DICTIONARY], int y[], double W[], int m, int n){
    int i, y_transformed;
    double cost=0, y_hat_transformed;
    for(i=0;i<m;i++){


        y_transformed = (y[i]+1)/2;
        y_hat_transformed = (f_x(dataset[i], W, n)+1)/2;

        cost += (y_transformed * log(y_hat_transformed)) + ((1-y_transformed) * log(1 - y_hat_transformed));
        //printf("%lf ", cost);
        //if(isnan(cost))
        //printf("peki %d ", i);
    }
    cost /= -m;

    return cost;
}

int predict(int x[], double W[], int n){

    double prediction;

    prediction = f_x(x, W, n);
    if(prediction>=0)
        return 1;
    else
        return -1;
}

void genW(double W[], float value){

    int i;
    for(i=0;i<DICTIONARY;i++){
        W[i] = value;
    }
}

void fillY(int y[]){
    int c,i=0;
    FILE *fin = fopen("dataset.txt", "r");
    if (fin == NULL) {
        printf("Error: Unable to open the file in.\n");
    }


    rewind(fin);

    //ilk sayı için
    c = fgetc(fin);
    if(c == '1'){
        y[i] = 1;
        i++;
    }
    else if( c =='-'){
        y[i] = -1;
        i++;
    }
    //
    while (i < MAX && (c = fgetc(fin)) != EOF){
        if( c == '\n'){
            c = fgetc(fin);
            if(c == '1'){
                y[i] = 1;
                i++;
            }
            else if( c =='-'){
                y[i] = -1;
                i++;
            }
        }

    }

    fclose(fin);
}

void makeDataset(bool dataset[MAX][DICTIONARY]){
    int i,j,n;//i: cumleler
    char str[60];

    FILE *fdata = fopen("dataset.txt", "r");
    if (fdata == NULL) {
        printf("Error: Unable to open the file in.\n");
    }
    FILE *fdict = fopen("dictionary.txt", "r");
    if (fdict == NULL) {
        printf("Error: Unable to open the file in.\n");
    }

    for(i =0; i<MAX; i++){
        n = countWords(fdata)+1;
        for(j=0; j<n; j++){
            getWord(str,fdata);
            dataset[i][checkDict(str,fdict)-1] = 1;

        }
        //printf("%d words are in the dict out of %d words\n", countOnes(dataset[i]),n);
    }

    fclose(fdata);
    fclose(fdict);
}

int isLetter(char c){
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c<= 'z'))
        return 1;
    return 0;
}

int appendStr(char* str,char letter){

    if(isLetter(letter)){
        letter = tolower(letter);
        strncat(str, &letter, 1);
        return 1;
    }
    return 0;
}

int getWord(char *str, FILE *fin){
    int flag =1;
    char letter;
    char endText=0;

    str[0] = 0;
    while(flag && (letter = fgetc(fin)) != EOF){

        appendStr(str,letter);

        if(letter == ' ' && str[0] !=0)
            flag =0;
        else if(letter == '\n'){
            flag = 0;
            endText=1;
        }
    }
    //printf("%s ",str); // this helps to see words

    if(letter == EOF || endText)
        return 0;
    return 1;
}

int countWords(FILE *fout){
    fpos_t position;
    int words=0;
    char str[60];
    fgetpos(fout, &position);

    while(getWord(str, fout))
        words++;

    fsetpos(fout,&position);
    return words;
}

int countOnes(int dataset[DICTIONARY]){
    int i,count=0;
    for(i = 0; i < DICTIONARY; i++){
        if(dataset[i] == 1)
            count++;
    }
    return count;
}

int checkDict(char *word, FILE *fin){
    int flag1,isSame,n=0;
    char str[60];

    rewind(fin);
    do{
        str[0] = 0;
        flag1 = fscanf(fin,"%s",str);
        isSame = strcmp(str,word);
        n++;
    }while(flag1 !=-1 && isSame);

    if(isSame == 0)
        return n;
    return 0;
}

void printArr(int arr[]){
    int i;

    for (i = 0; i < MAX; i++) {
        printf("%d ",arr[i]);

    }
}
void printMat(int dataset[MAX][DICTIONARY]){
    int i,j;

    for (i = 0; i < MAX; i++) {
        for (j = 0; j < sizeof(dataset[i])/sizeof(int); j++) {
            printf("%d ",dataset[i][j]);
        }
        printf("\n");
    }
}