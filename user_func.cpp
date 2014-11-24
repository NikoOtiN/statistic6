#include "user_func.h"
#include <QDebug>


calc_data do_calc(int k, int n, double m, double g, double inc,double level)
{
    QVector<QVector<double> > matrix =  createMatrix(k, n, m, g, inc);

    QVector<double> meanYs;
    QVector<double> meanXs;
    QVector<double> varianceYs;
    QVector<double> gYs;
    QVector<double> axisX;
    int i =0;
    for(auto s :matrix)
    {
        QVector<double> tmp(s.size(),i++);
        axisX+=tmp;
        meanXs.push_back(i);
        meanYs.push_back(sample_mean(s));
        varianceYs.push_back(sample_variance(s,meanYs.back()));
        gYs.push_back(sqrt(varianceYs.back()));
    }

    QVector<double> totalV;
    for(auto s : matrix)
    {
        totalV+=s;
    }

    double xysum=0,xsum=0,ysum=0,xsum2=0;
    for(int i = 0; i < meanYs.size(); i++)
    {
        xysum += meanYs.at(i)*i;
        ysum += meanYs.at(i);
        xsum += i;
        xsum2 += i*i;
    }

    double betta = (k*xysum - xsum*ysum)/(k*xsum2 - xsum*xsum);
    double alpha = (ysum - betta*xsum)/k;

    double general_meanY=sample_mean(meanYs);
    double quantile_st = t_test_cr(k-1,level);
    double sy = sqrt(sample_variance(meanYs,general_meanY));
    double deltaA = sy*quantile_st;

    qDebug() << "alpha" << alpha;
    double general_meanX=sample_mean(meanXs);
    qDebug() << "x mean" << general_meanX;
    double sx = sqrt(sample_variance(meanXs,general_meanX));
    double quantile_B = t_test_cr(k-2,level);

    double rkoeff = (k-1)*sx*sy;
    double rsum = 0;
    for(int i = 0; i < k; i++)
    {
        rsum+= (i-general_meanX)*(meanYs.at(i) - general_meanY);
    }
    double r = rsum/rkoeff;
    qDebug() << "r " << r;

    double koeffB = sy*sqrt(1-r)/(sx*sqrt(k-2));
    double deltaB = quantile_B*koeffB;

    double Dos=0;
    for(int i =0 ; i< meanYs.size();i++)
    {
        Dos+=(meanYs.at(i)-alpha-betta*i)*(meanYs.at(i)-alpha-betta*i);
    }
    Dos/=(k-2);

    QVector<double> variance;
    for(int i = 0 ; i < matrix.size(); i++)
    {
        variance.push_back(sample_variance(matrix.at(i),meanYs.at(i)));
    }
    int sum=0;
    for(auto v : variance)
    {
        sum+=v;
    }
    double d0 =sum/=k;
    double test_f = Dos/d0;
    double f_cr =  F_test_cr( k-1, k*(n-1), 1 - level);


    calc_data result;
    result.matrix= matrix;
    result.axisX = axisX;
    result.axisY = totalV;
    result.deltaB = deltaB;
    result.deltaA = deltaA;
    result.alpha = alpha;
    result.betta = betta;
    result.d0=d0;
    result.dos=Dos;
    result.mx=general_meanX;
    result.my=general_meanY;
    result.sx=sx;
    result.sy=sy;
    result.F= test_f;
    result.Fcr=f_cr;
    return result;
}

void print_v(QVector<double> v)
{
    for(auto e : v)
    {
       qDebug() << e ;
    }
}

QVector<QVector<double> > createMatrix(int k, int n, double m, double g, double inc)
{
    QVector<QVector<double> > matrix;
    for(int i = 0; i<k; i++)
    {
        matrix.push_back(create_sample(n,m+i*inc,g,i));
    }
    return matrix;
}

QVector<double> create_sample(int size, double m, double g,int k)
{
    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count()*k;
    std::default_random_engine generator(seed1);
    std::normal_distribution<double> distribution(m,g);
    QVector<double> tmp;
    for(int i = 0; i <size; i++)
    {
        tmp.push_back(distribution(generator));
    }
    return tmp;
}



//Выброчное среднее
double sample_mean(QVector<double> X)
{
    double sum=0;
    if(X.empty())
    {
        //throw length_error("sample_mean: error empty vector!");
    }
    for(auto i : X)
    {
        sum+=i;
    }
    return sum/X.size();
}

//Выборочная дисперсия
double sample_variance(QVector<double> X, double sample_mean)
{
    double sum=0;
    if(X.empty())
    {
       // throw length_error("sample_variance: error empty vector!");
    }
    for(auto i: X)
    {
        sum+=pow(i-sample_mean,2);
    }
    return sum/(X.size()-1);
}

double t_test_cr(int v, double level)
{
    double u = quantile(1-level/2);
    double q1 = (u*u+1)*u/4;
    double q2 = ((5*u*u+16)*pow(u,2) + 3)*u/96;
    double q3 = (((3*u*u+19)*u*u+17)*u*u-15)*u/384;
    double q4 = ((((79*u*u+776)*u*u + 1482)*u*u -1920)*u*u-945)*u/92160;
    double t = u + q1/v + q2/pow(v,2) + q3/pow(v,3) + q4/pow(v,4);
    return t;
}

//Расчет квантили норм. распределения.
double quantile(double level)
{
   double q, t;
   double one = 1;

   t = level < 0.5 ? level : one - level;
   t = sqrt(-2 * log(t));
   q = t - ((0.010328 * t + 0.802853) * t + 2.515517) /
      (((0.001308 * t + 0.189269) * t + 1.432788) * t + 1);
   return level > 0.5 ? q : -q;
}



//Критическое значение статистики Фишера
double F_test_cr(int v1, int v2, double level)
{
    double u = quantile(level);
    double h = 2.0*(v1-1)*(v2-1)/(v1+v2-2);
    double l = (pow(u,2)-3)/6;
    double w = (u*sqrt(h+l)/h) - (1.0/(v1-1)-1.0/(v2-1))*(l+5.0/6.0-2.0/(3.0*h));
    return exp(2*w);
}
