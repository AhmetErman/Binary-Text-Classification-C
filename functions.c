#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX 160
#define TEST_MAX 40
#define DICTIONARY 5450
#define TEST_SET "test.txt"
#define TRAIN_SET "train.txt"

double f_x(bool x[], double w[]);
void gradient_descent(bool dataset[MAX][DICTIONARY], bool test_set[][DICTIONARY], int y[], int y_test[], double W[], double learning_rate, int iteration);
void stochastic_gradient_descent(bool dataset[MAX][DICTIONARY], bool test_set[][DICTIONARY], int y[], int y_test[], double W[], double learning_rate, int iteration);
void Adam(bool dataset[MAX][DICTIONARY], bool test_set[][DICTIONARY], int y[], int y_test[], double W[], double learning_rate, int iteration);
double Loss(bool dataset[][DICTIONARY], int m, int y[], double W[]);
void predict(bool test_set[][DICTIONARY], double W[], int predictions[]);
void compare(int predictions[], int y_test[]);
void genW(double W[], float value);
void fillY(int y[],char *filename, int m);
void fillDataset(bool dataset[][DICTIONARY],char *filename, int m);

int isLetter(char c);
int appendStr(char* str,char letter);
int getWord(char *str, FILE *fin);
int countWords(FILE *fout);
int countOnes(int dataset[DICTIONARY]);
int checkDict(char *word, FILE *fin);
void printArr(int arr[]);
void printMat(int dataset[MAX][DICTIONARY]);


int main() {
    int y[MAX];
    int y_test[TEST_MAX];
    double W[DICTIONARY];
    int predictions[TEST_MAX];
    bool dataset[MAX][DICTIONARY];
    bool test_set[TEST_MAX][DICTIONARY];

    fillDataset(dataset,TRAIN_SET,MAX);
    fillDataset(test_set,TEST_SET,TEST_MAX);

    fillY(y,TRAIN_SET,MAX);
    fillY(y_test,TEST_SET,TEST_MAX);

    genW(W, 0);

    //gradient_descent(dataset, test_set, y, y_test, W,0.07,100);
    stochastic_gradient_descent(dataset, test_set, y, y_test, W,0.1,50);
    Adam(dataset,test_set, y,y_test, W, 0.5, 10);

    return 0;
}

void gradient_descent(bool dataset[MAX][DICTIONARY], bool test_set[TEST_MAX][DICTIONARY], int y[], int y_test[], double W[], double learning_rate, int iteration){

    int i=0, j, k;
    double f_der=0, y_hat;
    clock_t t;
    double t_seconds;

    t = clock();
    for(i= 0; i<iteration;i++) {
        for (j = 0; j < DICTIONARY; j++) {

            for (k = 0; k < MAX; k++) {

                y_hat = f_x(dataset[k], W);
                f_der += (y_hat - y[k]) * dataset[k][j];//gradyan
            }
            f_der /= MAX;

            W[j] = W[j] - learning_rate * f_der;
            f_der = 0;
        }
        t_seconds= ((double)(clock()-t))/CLOCKS_PER_SEC; // in seconds

        printf("Epoch %d,Time %.3f : Loss = %f, TEST Loss = %f\n\n", i+1, t_seconds, Loss(dataset, MAX, y, W), Loss(test_set, TEST_MAX, y_test, W));
        //printf("Epoch %d, TEST Loss = %f\n\n", i+1, Loss(test_set, TEST_MAX, y_test, W)); // silebiliriz
    }
}

void stochastic_gradient_descent(bool dataset[MAX][DICTIONARY], bool test_set[][DICTIONARY], int y[], int y_test[], double W[], double learning_rate, int iteration){

    int i, j, random;
    double f_der, y_hat;
    clock_t t;
    double t_seconds;

    srand(time(NULL));

    t = clock();
    for(i=0; i<iteration; i++){

        random = rand() % MAX;

        y_hat = f_x(dataset[random], W);

        for(j=0;j<DICTIONARY;j++){

            f_der = (y_hat - y[random]) * dataset[random][j];
            W[j] = W[j] - learning_rate * f_der;
        }
        t_seconds= ((double)(clock()-t))/CLOCKS_PER_SEC; // in seconds

        printf("Epoch %d,Time %.3f : Loss = %f, TEST Loss = %f\n\n", i+1, t_seconds, Loss(dataset, MAX, y, W), Loss(test_set, TEST_MAX, y_test, W));
        //printf("Epoch %d, TEST Loss = %f\n", i+1, Loss(test_set, TEST_MAX, y_test, W));
    }
}

void Adam(bool dataset[MAX][DICTIONARY], bool test_set[][DICTIONARY], int y[], int y_test[], double W[], double learning_rate, int iteration){

    int i=0, j, k;
    double f_der=0, y_hat, B1=0.9, B2=0.99, epsilon = 0.0001;
    double mt[DICTIONARY] = {0};
    double vt[DICTIONARY] = {0};
    clock_t t;
    double t_seconds;

    while(i<iteration) {
        i++;

        for (j = 0; j < DICTIONARY; j++) {
            for (k = 0; k < MAX; k++) {

                y_hat= f_x(dataset[k], W);

                f_der += (y_hat - y[k]) * dataset[k][j];
            }
            f_der /= MAX;

            mt[j] = B1*mt[j] + (1-B1)*f_der;
            vt[j] = B2*vt[j] + (1-B2)*pow(f_der, 2);

            mt[j] = mt[j] / (1-pow(B1, i));
            vt[j] = vt[j] / (1-pow(B2, i));

            W[j] = W[j] - learning_rate * (mt[j] / (sqrt(vt[j]) + epsilon));
            f_der = 0;
        }

        t_seconds= ((double)(clock()-t))/CLOCKS_PER_SEC; // in seconds
        printf("Epoch %d,Time %.3f : Loss = %f, TEST Loss = %f\n\n", i, t_seconds, Loss(dataset, MAX, y, W), Loss(test_set, TEST_MAX, y_test, W));
        //printf("Epoch %d, TEST Loss = %f\n", i+1, Loss(test_set, TEST_MAX, y_test, W));
    }

}

double f_x(bool x[], double W[]){

    double WX = 0, a;
    int i;

    for(i=0;i<DICTIONARY;i++) {
        WX += W[i] * x[i];
    }
    a = tanh(WX);

    if(a==1)
        a -= 0.000001;
    else if(a==-1)
        a += 0.000001;

    return a;
}

double Loss(bool dataset[][DICTIONARY], int m, int y[], double W[]){
    int i;
    double cost=0;
    for(i=0;i<m;i++){

        cost += pow((f_x(dataset[i], W) - y[i]), 2);
    }
    cost /= 2*m;

    return cost;
}

void predict(bool test_set[][DICTIONARY], double W[], int predictions[]){

    int i;

    for(i=0;i<TEST_MAX;i++){
        if(f_x(test_set[i], W) >=0)
            predictions[i] = 1;
        else
            predictions[i] = -1;
    }
}

void compare(int predictions[], int y_test[]){
    int i;
    float accuracy = 0;
    for(i=0;i<TEST_MAX;i++){
        printf("%d. Real Value = %d, Estimated Value = %d\n", i+1, y_test[i], predictions[i]);
        accuracy +=1;
    }
    accuracy = (accuracy/40)*100;
    printf("Accuracy = %f percent\n", accuracy);
}

void genW(double W[], float value){

    int i;
    for(i=0;i<DICTIONARY;i++){
        W[i] = value;
    }
}

void fillY(int y[],char *filename, int m){
    int c,i=0;
    FILE *fin = fopen(filename, "r");
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
    while (i < m && (c = fgetc(fin)) != EOF){
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

void fillDataset(bool dataset[][DICTIONARY],char *filename, int m){
    int i,j,n;//i: cumleler
    char str[60];

    FILE *fdata = fopen(filename, "r");
    if (fdata == NULL) {
        printf("Error: Unable to open the file in.\n");
    }
    FILE *fdict = fopen("dictionary.txt", "r");
    if (fdict == NULL) {
        printf("Error: Unable to open the file in.\n");
    }

    for(i =0; i<m; i++){
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