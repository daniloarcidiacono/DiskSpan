#include <QtWidgets>
#include <QStyle>
#include <QDesktopWidget>
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // Signals
    QObject::connect(ui.actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    QObject::connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(about()));

    showCentered();
}

MainWindow::~MainWindow()
{
}

void MainWindow::showCentered()
{
    setGeometry(QStyle::alignedRect(Qt::LeftToRight,
                                    Qt::AlignCenter,
                                    size(),
                                    qApp->desktop()->availableGeometry())
                );
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About DiskSpan"),
            tr("The <b>DiskSpan</b> example demonstrates how to "
               "write modern GUI applications using Qt, with a menu bar, "
               "toolbars, and a status bar."));
}