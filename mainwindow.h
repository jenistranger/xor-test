#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QLineEdit>
#include <QString>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;


    void setInputBytes();
    QTimer *timer;

private slots:
    void runProcess();

    void onComboType(int index);

    void on_strtBtn_clicked();

    void on_sourceDir_clicked();
    void on_resultDir_clicked();
};
#endif // MAINWINDOW_H
