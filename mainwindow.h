#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QTextEdit>
#include "dialog.h"
#include "user_func.h"
#include "qwt_plot.h"
#include "qwt_legend.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_layout.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QAction *enterDataAction;
    Dialog *dd;
    QTabWidget *mainTab;
    QwtPlot *mainPlot;
    QwtLegend *legend;
    QTextEdit *textEdit;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void showDialog();
    void calculate();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
