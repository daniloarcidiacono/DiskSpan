#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"

// Forward declarations
class QFileSystemModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindowClass ui;
	QFileSystemModel *model;

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void showCentered();
    
protected slots:
	void onRootPathChanged(const QString &value);
    void about();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif