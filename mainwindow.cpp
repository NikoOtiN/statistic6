#include "mainwindow.h"
#include "ui_mainwindow.h"


QString text = "<p>Результаты расчетов</p> <p>M<sub>x</sub>=%1 \
        <p>M<sub>y</sub>=%2</p> <p>s<sub>x</sub>=%3 <p> <p>s<sub>y</sub>=%4 </p> \
        <p>D<sub>ост</sub>=%5</p> <p> D <sub>0</sub>=%6</p> ";
QString text2 = "<p>Параметры уравнения регрессии </p> <p> &alpha; = %1 </p> \
        <p> &beta; = %2 </p> ";
QString correct = "<p>Проверка гипотезы об адекватности линейной регрессии</p> \
        <p> D<sub>ост</sub>/D <sub>0</sub> = %1, F <sub>1-%3,%4,%5</sub> = %2 \
        <p> D<sub>ост</sub>/D <sub>0</sub> &lt; F <sub>1-%3,%4,%5</sub> </p> \
        <p> Гипотеца подтверждена </p> ";
QString uncorrect = "Гипотеза отвергнута.";


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dd = new Dialog(this);
    mainTab = new QTabWidget(this);

    mainPlot = new QwtPlot(this);
    mainPlot->setTitle("Результы эксперимента");
    mainPlot->setAxisTitle(QwtPlot::xBottom,"Свободная переменная");
    mainPlot->setAxisTitle(QwtPlot::yLeft,"Зависимая перменная");
    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setPalette( Qt::gray );
    canvas->setBorderRadius( 10 );
    mainPlot->setCanvas(canvas);
    legend = new QwtLegend();
    legend->setDefaultItemMode(QwtLegendData::Checkable);
    mainPlot->insertLegend(legend);
    QwtPlotLayout *plotLayout = mainPlot->plotLayout();
    plotLayout->setAlignCanvasToScales( true );
    mainPlot->setAutoReplot(true);


    mainTab->addTab(mainPlot,"Визуализация данных");


    enterDataAction = new QAction("Ввод данных",this);
    connect(enterDataAction,SIGNAL(triggered()),this,SLOT(showDialog()));
    connect(dd,SIGNAL(accepted()),this,SLOT(calculate()));
    menuBar()->addAction(enterDataAction);

    textEdit = new QTextEdit(this);
    mainTab->addTab(textEdit,"Результаты решения");



    setCentralWidget(mainTab);
    emit calculate();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showDialog()
{
    dd->show();
}


void MainWindow::calculate()
{
    mainPlot->detachItems();
    dialog_data data = dd->getParam();
    calc_data res = do_calc(data.count_level,data.n,data.expected,data.deviation,data.inc,data.a);
    QVector<double> pointsY= res.axisY;
    QVector<double> pointsX = res.axisX;

    QwtPlotCurve *samples = new QwtPlotCurve();
    samples->setStyle(QwtPlotCurve::Dots);
    samples->setPen(Qt::yellow,3);
    samples->setSamples(pointsX,pointsY);
    samples->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    samples->attach(mainPlot);

    QwtPlotCurve *regLine = new QwtPlotCurve();
    double xline[] = { pointsX.front(), pointsX.back()};
    double yline[] = { res.alpha + res.betta * pointsX.front(), res.alpha + res.betta * pointsX.back()};
    regLine->setSamples(xline,yline,2);
    regLine->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    regLine->attach(mainPlot);

    QwtPlotCurve *minLine = new QwtPlotCurve();
    double xline2[] = { pointsX.front(), pointsX.back()};
    double yline2[] = { (res.alpha - res.deltaA) + (res.betta - res.deltaB) * pointsX.front(),
                        (res.alpha - res.deltaA) + (res.betta - res.deltaB) * pointsX.back() };
    minLine->setSamples(xline2,yline2,2);
    minLine->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    minLine->attach(mainPlot);

    QwtPlotCurve *maxLine = new QwtPlotCurve();
    double xline3[] = { pointsX.front(), pointsX.back()};
    double yline3[] = { (res.alpha + res.deltaA) + (res.betta+res.deltaB) * pointsX.front(),
                        (res.alpha + res.deltaA) + (res.betta+res.deltaB) * pointsX.back()};
    maxLine->setSamples(xline3,yline3,2);
    maxLine->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    maxLine->attach(mainPlot);
    textEdit->clear();
    textEdit->append(text.arg(QString::number(res.mx),QString::number(res.my),
                              QString::number(res.sx),QString::number(res.sy),
                              QString::number(res.dos),QString::number(res.d0)));
    textEdit->append(text2.arg(QString::number(res.alpha),QString::number(res.betta)));
    if(res.F<res.Fcr)
    {
        textEdit->append(correct.arg(QString::number(res.F),QString::number(res.Fcr),
                                     QString::number(data.a),QString::number(data.count_level-1),
                                     QString::number(data.count_level*(data.n-1))));
    }

}
