#include <stdio.h>
#include <math.h>
#include <fftw3.h>


#define NUM_POINTS 1001

#define REAL 0
#define IMAG 1

double min=-30;
double max=30;

//初始化函数
void initialize(fftw_complex* signal) {
    /*
     * Generate two sine waves of different frequencies and
     * amplitudes.
     */

    int i;
    for (i = 0; i <= NUM_POINTS/2; i++) {

        double delta = (max-min) / (double)(NUM_POINTS-1);
        double x = delta*i;
        //保证对称性
        signal[i][REAL] = exp(-0.5*x*x);
        signal[i][IMAG] = 0;

        signal[NUM_POINTS-i-1][REAL] = exp(-0.5*x*x);
        signal[NUM_POINTS-i-1][IMAG] = 0;
    }
}

int main() {

    //输入和输出的数组
    fftw_complex signal[NUM_POINTS];
    fftw_complex result[NUM_POINTS];


    FILE *f;

//声明plan
    fftw_plan plan = fftw_plan_dft_1d(NUM_POINTS,
                                      signal,
                                      result,
                                      FFTW_FORWARD,
                                      FFTW_ESTIMATE);

    initialize(signal);

    double delta = (max-min) / (double)(NUM_POINTS-1);
    f = fopen("in.txt","w");
    int i;
    for (i = 0; i < NUM_POINTS; i++){
        double x = delta*i;
        if (x > max){
            x -= (max-min);
        }

        fprintf(f,"%f\t%f\t%f\n",x,signal[i][REAL],signal[i][IMAG]);
    }
    fclose(f);
//执行plan
    fftw_execute(plan);

    f = fopen("out.txt","w");
    for (i = 0; i < NUM_POINTS; i++){
        double  dw= 2*M_PI/(max-min);
        double  x;
        if (i <= NUM_POINTS/2){
            x = i*dw;
        }
        else{
            x = -1*(NUM_POINTS-i)*dw;
        }

        fprintf(f,"%f\t%f\t%f\n",x,result[i][REAL],result[i][IMAG]);
    }
    fclose(f);

//kill plan
    fftw_destroy_plan(plan);

//如果有动态数组，释放内存。

    return 0;
}