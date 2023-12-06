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

double f_x(bool x[DICTIONARY], double w[DICTIONARY]);
void gradient_descent(bool dataset[MAX][DICTIONARY], bool test_set[][DICTIONARY], int y[], int y_test[], double W[], double learning_rate, int iteration);
void stochastic_gradient_descent(bool dataset[MAX][DICTIONARY], bool test_set[][DICTIONARY], int y[], int y_test[], double W[], double learning_rate, int iteration);
void Adam(bool dataset[MAX][DICTIONARY], bool test_set[][DICTIONARY], int y[], int y_test[], double W[], double learning_rate, int iteration);
double Loss(bool dataset[][DICTIONARY], int m, int y[], double W[]);
void predict(bool test_set[][DICTIONARY], double W[], int predictions[]);
double compare(int predictions[], int y_test[]);
void autoplay(bool dataset[MAX][DICTIONARY], bool test_set[][DICTIONARY], int y[], int y_test[], double learning_rate);
void genW(double W[], double value);
void fillY(int y[],char *filename, int m);
void fillDataset(bool dataset[][DICTIONARY],char *filename, int m);

int isLetter(char c);
int appendStr(char* str,char letter);
int getWord(char *str, FILE *fin);
int countWords(FILE *fout);
int countOnes(int dataset[DICTIONARY]);
int checkDict(char *word, FILE *fin);
void printResults(int i, double t_seconds,bool dataset[MAX][DICTIONARY],int y[DICTIONARY],int y_test[TEST_MAX],double W[DICTIONARY],bool test_set[TEST_MAX][DICTIONARY],int mod);



int main() {
    int y[MAX];
    int y_test[TEST_MAX];
    double W[DICTIONARY];
    int predictions[TEST_MAX];
    bool dataset[MAX][DICTIONARY];
    bool test_set[TEST_MAX][DICTIONARY];
    double learning_rate = 0.6;

    fillDataset(dataset,TRAIN_SET,MAX);
    fillDataset(test_set,TEST_SET,TEST_MAX);

    fillY(y,TRAIN_SET,MAX);
    fillY(y_test,TEST_SET,TEST_MAX);

    //genW(W, 0.2);

    autoplay(dataset, test_set, y, y_test,learning_rate);

//    printf("gradient, eps: %.4f\n",learning_rate);
//    gradient_descent(dataset, test_set, y, y_test, W,learning_rate,500);
//
//    printf("stochastic, eps: %.4f\n",learning_rate);
//    stochastic_gradient_descent(dataset, test_set, y, y_test, W,learning_rate,2000);

//    printf("Adam, eps: %.4f\n",learning_rate);
//    Adam(dataset,test_set, y,y_test, W, learning_rate, 100);

//    predict(test_set, W, predictions);
//    compare(predictions, y_test);
    return 0;
}

//GD optimization algorithm
void gradient_descent(bool dataset[MAX][DICTIONARY], bool test_set[TEST_MAX][DICTIONARY], int y[], int y_test[], double W[], double learning_rate, int iteration){

    int i, j, k;
    double f_der=0, y_hat[MAX];
    clock_t t;
    double t_seconds;

    t = clock();
    for(i= 0; i<iteration;i++) {
        for(k=0;k<MAX;k++){
            y_hat[k] = f_x(dataset[k], W);
        }

        for (j = 0; j < DICTIONARY; j++) {

            for (k = 0; k < MAX; k++) {

                f_der += (y_hat[k] - y[k]) * ((1 -y_hat[k]*y_hat[k])*dataset[k][j]);//gradyan
            }
            f_der /= MAX;

            W[j] = W[j] - learning_rate * f_der;
            f_der = 0;
        }
        t_seconds= ((double)(clock()-t))/CLOCKS_PER_SEC; // in seconds

        printResults(i,t_seconds,dataset,y,y_test,W,test_set,1);
    }
}

//SGD optimization algorithm
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

            f_der = (y_hat - y[random]) * ((1 -y_hat*y_hat)*dataset[random][j]);
            W[j] = W[j] - learning_rate * f_der;
        }
        t_seconds= ((double)(clock()-t))/CLOCKS_PER_SEC; // in seconds

        printResults(i,t_seconds,dataset,y,y_test,W,test_set,2);
    }
}

//ADAM optimization algorithm
void Adam(bool dataset[MAX][DICTIONARY], bool test_set[][DICTIONARY], int y[], int y_test[], double W[], double learning_rate, int iteration){

    int i, j, k;
    double f_der=0, y_hat[MAX], B1=0.5, B2=0.7, epsilon = 0.0001;
    double mt[DICTIONARY] = {0};
    double vt[DICTIONARY] = {0};
    clock_t t;
    double t_seconds;

    t = clock();
    for(i=1;i<iteration+1;i++){

        for(k=0;k<MAX;k++){
            y_hat[k] = f_x(dataset[k], W);

        }

        for (j = 0; j < DICTIONARY; j++) {

            for (k = 0; k < MAX; k++) {

                f_der += (y_hat[k]- y[k]) * ((1 -y_hat[k]*y_hat[k])*dataset[k][j]);
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
        printResults(i,t_seconds,dataset,y,y_test,W,test_set,3);
    }

}

//calculates the tanh(WX) function
double f_x(bool x[DICTIONARY], double W[DICTIONARY]){

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

// calculates the mean squared error
double Loss(bool dataset[][DICTIONARY], int m, int y[], double W[]){
    int i;
    double cost=0;
    for(i=0;i<m;i++){

        cost += pow((f_x(dataset[i], W) - y[i]), 2);
    }
    cost /= 2*m;

    return cost;
}

//labels the test_set datas as 1 or -1
void predict(bool test_set[][DICTIONARY], double W[], int predictions[]){

    int i;

    for(i=0;i<TEST_MAX;i++){
        if(f_x(test_set[i], W) >=0)
            predictions[i] = 1;
        else
            predictions[i] = -1;
    }
}

//compares predictions of the optimization algorithms with the real test values. returns accuracy
double compare(int predictions[], int y_test[]){
    int i;
    double accuracy = 0;
    for(i=0;i<TEST_MAX;i++){
//        printf("%d. Real Value = %d, Estimated Value = %d\n", i+1, y_test[i], predictions[i]);
        if(y_test[i] == predictions[i])
            accuracy++;
    }
    accuracy = (accuracy/40)*100;
    printf("Accuracy = %f percent\n", accuracy);
    return accuracy;
}

//automatically runs with the 5 different w values
void autoplay(bool dataset[MAX][DICTIONARY], bool test_set[][DICTIONARY], int y[], int y_test[], double learning_rate){
    int i,predictions[TEST_MAX];
    double num = -0.2, W[DICTIONARY];

    for(i= 0; i < 5; i++){

        genW(W, num);
        printf("gradient, w: %.3f\n",num);
        gradient_descent(dataset, test_set, y, y_test, W,learning_rate,500);

        genW(W, num);
        printf("stochastic, w: %.3f\n",num);
        stochastic_gradient_descent(dataset, test_set, y, y_test, W,learning_rate,1000);

        genW(W, num);
        printf("Adam, w: %.3f\n",num);
        Adam(dataset,test_set, y,y_test, W, learning_rate, 100);

        num += 0.1;
    }
}

// fills w vector according to input value
void genW(double W[], double value){

    int i;
    for(i=0;i<DICTIONARY;i++){
        W[i] = value;
    }
}

//fills y vector according to class of the text. Inputs from train dataset or test dataset
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

//checks every word if they are in dictionary writes 1 leaves it 0
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

//checks if it is letter
int isLetter(char c){
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c<= 'z'))
        return 1;
    return 0;
}

// append a letter to str
int appendStr(char* str,char letter){

    if(isLetter(letter)){
        letter = tolower(letter);
        strncat(str, &letter, 1);
        return 1;
    }
    return 0;
}

// appends every letter until ' ' or '\n' chars. If it is end of the text returns 0 if not returns 1
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

// counts how many words in a text
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

//counts 1 numbers to see how many word are from the dictionary
int countOnes(int dataset[DICTIONARY]){
    int i,count=0;
    for(i = 0; i < DICTIONARY; i++){
        if(dataset[i] == 1)
            count++;
    }
    return count;
}

//Checks the dictionary if the word is in the dictionary returns the position of the word if not return 0
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

// prints every 10 iteration on terminal and every iteration on results.txt file.
void printResults(int i, double t_seconds,bool dataset[MAX][DICTIONARY],int y[DICTIONARY],int y_test[TEST_MAX],double W[DICTIONARY],bool test_set[TEST_MAX][DICTIONARY],int mod){
    int predictions[TEST_MAX];
    double accuracy;
    FILE *fout = fopen("results.txt","a");

    if(mod == 3)
        i--;

    if((i+1)%10 == 0){
        printf("Epoch %d,Time %.3f : Loss = %f, TEST Loss = %f\n\n", i+1, t_seconds, Loss(dataset, MAX, y, W), Loss(test_set, TEST_MAX, y_test, W));

        predict(test_set, W, predictions);
        accuracy = compare(predictions, y_test);
    }
    else if(i==0){
        if(mod == 1){
            printf("Epoch %d,Time %.3f : Loss = %f, TEST Loss = %f\n\n", i+1, t_seconds, Loss(dataset, MAX, y, W), Loss(test_set, TEST_MAX, y_test, W));
            fprintf(fout, "\n%s\n","Gradient");
        }
        else if(mod == 2){
            printf("Epoch %d,Time %.3f : Loss = %f, TEST Loss = %f\n\n", i+1, t_seconds, Loss(dataset, MAX, y, W), Loss(test_set, TEST_MAX, y_test, W));
            fprintf(fout, "\n%s\n","Stocastic");
        }
        else if(mod == 3){
            printf("Epoch %d,Time %.3f : Loss = %f, TEST Loss = %f\n\n", i+1, t_seconds, Loss(dataset, MAX, y, W), Loss(test_set, TEST_MAX, y_test, W));
            fprintf(fout, "\n%s\n","ADAM");
        }
    }


    //grafik için print
    fprintf(fout,"{%d,%.3f,%f,%f},\n", i+1, t_seconds, Loss(dataset, MAX, y, W), Loss(test_set, TEST_MAX, y_test, W));
    if((i+1)%100 == 0){
        fprintf(fout,"%d: Accuracy = %f percent\n",i+1, accuracy);
    }
    fclose(fout);
}