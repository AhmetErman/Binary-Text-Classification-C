#include "functions.h"

int mainMenu(int y[MAX],int y_test[TEST_MAX],bool dataset[MAX][DICTIONARY],bool test_set[TEST_MAX][DICTIONARY]){
    int c;
    double W[DICTIONARY],w;
    int predictions[TEST_MAX];
    double learning_rate = 0.5;//default value
    genW(W, 0.2);//default value


    printf("   ---MAIN MENU---   \n(choose a method)\n");
    printf("1)Gradient Descent \n2)Stochastic Gradient Descent\n3)ADAM\n4)Autoplay (runs all three algorithms with 5 different w values)\n");
    scanf("%d",&c);

    if(c<4){
        printf("Please enter the learning_rate value: ");
        scanf("%lf",&learning_rate);
        printf("Please enter the starting point(W) value: ");
        scanf("%lf",&w);
        genW(W,w);
    }


    if(c == 1){
        printf("Gradient, learning_rate: %.3f, W: %.3f\n",learning_rate,w);

        printf("Please enter the iteration number: ");
        scanf("%d",&c);

        gradient_descent(dataset, test_set, y, y_test, W,learning_rate,c);
    }
    else if(c == 2){
        printf("Stochastic, learning_rate: %.3f, W: %.3f\n",learning_rate,w);

        printf("Please enter the iteration number: ");
        scanf("%d",&c);

        stochastic_gradient_descent(dataset, test_set, y, y_test, W,learning_rate,c);
    }
    else if(c == 3){
        printf("Adam, learning_rate: %.3f, W: %.3f\n",learning_rate,w);

        printf("Please enter the iteration number: ");
        scanf("%d",&c);

        Adam(dataset,test_set, y,y_test, W, learning_rate, c);
    }
    else{
        printf("Autoplay, learning_rate: %.4f\n",learning_rate);
        autoplay(dataset, test_set, y, y_test,learning_rate);
    }

    printf("\nif you want to leave press 0.To return main menu press a button");
    scanf("%d",&c);
    return c;
}

int main() {
    int num =1;
    int y[MAX],y_test[TEST_MAX];
    bool dataset[MAX][DICTIONARY];
    bool test_set[TEST_MAX][DICTIONARY];

    printf("Loading...\n");

    fillDataset(dataset,TRAIN_SET,MAX);
    fillDataset(test_set,TEST_SET,TEST_MAX);

    fillY(y,TRAIN_SET,MAX);
    fillY(y_test,TEST_SET,TEST_MAX);

    FILE *file = fopen("results.txt","w");
    fclose(file);


    while (num)
        num = mainMenu(y, y_test,dataset,test_set);


    return 0;
}
