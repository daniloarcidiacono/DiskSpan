#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindowClass ui;

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void showCentered();
    
protected slots:
    void about();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif