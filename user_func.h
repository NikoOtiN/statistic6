#ifndef USER_FUNC_H
#define USER_FUNC_H
#include <QVector>
#include <chrono>
#include <random>
#include <QDebug>

struct calc_data{
    QVector<QVector<double>> matrix;
    double alpha;
    double betta;
    double deltaB;
    double deltaA;
    QVector<double> axisX;
    QVector<double> axisY;
    double mx;
    double my;
    double sx;
    double sy;
    double dos;
    double d0;
    double F;
    double Fcr;
};

using namespace std;

calc_data do_calc(int k, int n, double m, double g, double inc,double level);
double sample_mean(QVector<double> X);
double sample_variance(QVector<double> X, double sample_mean);
QVector<QVector<double> > createMatrix(int k, int n, double m, double g, double inc);
QVector<double> create_sample(int size, double m, double g,int k);
double t_test_cr(int v, double level);
double quantile(double level);
void print_v(QVector<double> v);
double F_test_cr(int v1, int v2, double level);
#endif // USER_FUNC_H
