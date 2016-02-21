/*  Copyright 2016 Danilo Arcidiacono. All Rights Reserved.
 *  This file is part of DiskSpan.
 *
 *  DiskSpan is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  DiskSpan is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with DiskSpan.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <QtWidgets>
#include <QStyle>
#include <QDesktopWidget>
#include <QTextStream>
#include <QDebug>
#include "MainWindow.h"
#include "model/Model.h"
#include "model/Item.h"
#include "model/Disk.h"
#include "solver/BFDSolver.h"
#include "components/DocumentComponent.h"
#include "components/FileSystemComponent.h"
#include "components/TransferComponent.h"
#include "components/ItemsComponent.h"
#include "components/DisksComponent.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);    

    // Setup members
    document = new Model(this);

    // Create the components
    createComponents();

    // Signals
    QObject::connect(ui.actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    QObject::connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(about()));

    showMaximized();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createComponents()
{
    cmpDocument = new DocumentComponent(this, document, this);
    cmpFileSystem = new FileSystemComponent(this, document, this);
    cmpFileSystem->setupUi(ui.widgetFileSystem);
    cmpTransfer = new TransferComponent(this, document, this);
    cmpTransfer->setupUi(ui.widgetTransfer);
    cmpItems = new ItemsComponent(this, document, this);
    cmpItems->setupUi(ui.widgetItems);
    cmpDisks = new DisksComponent(this, document, this);
    cmpDisks->setupUi(ui.widgetDisks);

    // Wiring
    QObject::connect(ui.actionNew, SIGNAL(triggered()), cmpDocument, SLOT(newFile()));
    QObject::connect(ui.actionOpen, SIGNAL(triggered()), cmpDocument, SLOT(open()));
    QObject::connect(ui.actionSave, SIGNAL(triggered()), cmpDocument, SLOT(save()));
    QObject::connect(ui.actionSaveAs, SIGNAL(triggered()), cmpDocument, SLOT(saveAs()));

    // When the users selects one or more file from the filesystem tree, we update the transfer buttons
    QObject::connect(cmpFileSystem, SIGNAL(fileSelected(const QList<QFileInfo> &)), cmpTransfer, SLOT(setCurrentFiles(const QList<QFileInfo> &)));

    // When the users selects one or more items from the items widget, we update the transfer buttons
    QObject::connect(cmpItems, SIGNAL(itemsSelected(const QList<Item *> &)), cmpTransfer, SLOT(setCurrentItems(const QList<Item *> &)));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    cmpDocument->onCloseEvent(event);
    // if (event->isAccepted())
    //  cmpSettings->write
}

void MainWindow::on_actionComputeDisks_triggered()
{
    if (!cmpDisks->isSolvable())
    {
        QMessageBox::critical(this, tr("Error"), tr("Every item's size must be less or equal than the disk capacity!"));    
        return;
    }

    statusBar()->showMessage(tr("Distributing items..."));

    BinSolver *solver = new BFDSolver();

    // Init the solver
    const quint64 capacity = cmpDisks->getGlobalDiskCapacity();
    solver->setDiskCapacity(capacity);
    for (int i = 0; i < document->getItemCount(); i++)
        solver->addItem(document->getItem(i)->getTotalSize());

    // Solve
    QApplication::setOverrideCursor(Qt::WaitCursor);
    solver->solve();
    QApplication::restoreOverrideCursor();

    // Setup results    
    cmpDisks->removeAllDisks();

    // Create the disks
    QList<Disk *> newDisks;
    for (int i = 0; i < solver->getDiskCount(); i++)
        newDisks.push_back(new Disk(capacity));

    // Populate them
    for (int i = 0; i < document->getItemCount(); i++)
        newDisks[solver->getItemAssignment(i)]->addItem(document->getItem(i));

    // Add them to the document
    document->addDisks(newDisks);

    delete solver;
    statusBar()->showMessage(tr("Done (%1 disks created).").arg(document->getDiskCount()));
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About DiskSpan"),
            tr("<b>DiskSpan</b> v0.2.0-alpha by Danilo Arcidiacono<br>"
               "<a href=\"mailto:danilo.arcidiacono@gmail.com\">danilo.arcidiacono@gmail.com</a><br>"
               "<a href=\"https://github.com/daniloarcidiacono\">Github</a>"));
}