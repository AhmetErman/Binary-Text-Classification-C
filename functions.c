#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define MAX 15000

double f_x(int x[], double w[], int n);
void gradient_descent(int dataset[][MAX], int y[], double W[], int m, int n, int eps, int iteration);
void stochastic_gradient_descent(int dataset[][MAX], int y[], double W[], int m, int n, int eps, int iteration);
double Loss(int dataset[][MAX], int y[], double W[], int m, int n);
int predict(int x[], double W[], int n);

int main() {

    int x[] = {1,2,3};
    double w[] = {0.5,0.25,0.166};

    int n = 3;

    printf("%f\n", f_x(x, w, n));

    printf("%d", predict(x, w, n));

    return 0;
}

void gradient_descent(int dataset[][MAX], int y[], double W[], int m, int n, int eps, int iteration){

    int i=0, j, k;
    double f_der=0;

    while(i<iteration) {
        for (j = 0; j < n; j++) {
            for (k = 0; k < m; k++) {
                f_der += (f_x(dataset[k], W, n) - (((y[k])+ 1) / 2.0)) * dataset[k][j];
            }
            f_der /= m;

            W[j] = W[j] - eps * f_der;
            f_der = 0;
        }
        i++;
        printf("iteration %d, Loss = %f\n", iteration, Loss(dataset, y, W, m, n));
    }
}

void stochastic_gradient_descent(int dataset[][MAX], int y[], double W[], int m, int n, int eps, int iteration){

    int i=0, j, random;
    double f_der=0;

    srand(time(NULL));

    while(i<iteration){

        random = rand() % m;

        for(j=0;j<n;j++){
            f_der += (f_x(dataset[random], W, n) - ((y[random])+ 1) / 2.0) * dataset[random][j];
            W[j] = W[j] - eps * f_der;
            f_der = 0;
        }
        i++;
        printf("iteration %d, Loss = %f\n", iteration, Loss(dataset, y, W, m, n));
    }
}

double f_x(int x[], double W[], int n){

    double WX = 0;
    int i;

    for(i=0;i<n;i++) {
        WX += W[i] * x[i];
    }
    return tanh(WX);
}

double Loss(int dataset[][MAX], int y[], double W[], int m, int n){
    int i;
    double cost=0;
    for(i=0;i<m;i++){

        cost += (((1+y[i])/2.0)*log(f_x(dataset[i], W, n))) + (((1-y[i])/2.0)*log(1 - f_x(dataset[i], W, n)));
    }
    cost /= m;
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