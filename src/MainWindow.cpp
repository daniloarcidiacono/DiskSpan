#include <QtWidgets>
#include <QStyle>
#include <QDesktopWidget>
#include <QTextStream>
#include <QDebug>
#include "MainWindow.h"
#include "Model.h"
#include "ItemsModel.h"
#include "ItemDetailsModel.h"
#include "FSTreeProxyFilter.h"
#include "FSUtils.h"
#include "Disk.h"
#include "DiskModel.h"
#include "DiskDetailsModel.h"
#include "BFDSolver.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);    

    // Create the document
    document = new Model();

    // Create the models
    createModels();

    // Signals
    // This signal will not be emitted when the item model is reset
    QObject::connect(ui.treeFileSystem->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(onTreeFSSelectionChanged()));
    QObject::connect(ui.tblItems->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(onItemsSelectionChanged()));
    QObject::connect(ui.tblItemDetails->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(onItemDetailsSelectionChanged()));
    QObject::connect(ui.actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    QObject::connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    QObject::connect(ui.editPath, SIGNAL(textChanged(const QString &)), this, SLOT(onRootPathChanged()));
    QObject::connect(ui.cboDisks, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentDiskChanged()));
    QObject::connect(document, SIGNAL(onItemChanged()), this, SLOT(onItemChanged()));

    //ui.editPath->setText("/home/danilo/Desktop");

    showMaximized();
    //showCentered();
}

MainWindow::~MainWindow()
{
    delete model;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void MainWindow::createModels()
{
    // File system
    model = new QFileSystemModel();
    model->setRootPath(QDir::currentPath());
    // ui.treeFileSystem->setModel(model);

    // Items
    itemsModel = new ItemsModel(document);
    ui.tblItems->setModel(itemsModel);

    // Item details
    itemDetailsModel = new ItemDetailsModel(NULL);
    ui.tblItemDetails->setModel(itemDetailsModel);

    // Disks
    diskModel = new DiskModel(document);
    ui.cboDisks->setModel(diskModel);

    // Disk details
    diskDetailsModel = new DiskDetailsModel(NULL);
    ui.tblDisk->setModel(diskDetailsModel);

    // Proxy
    fsProxyModel = new FSTreeProxyFilter(document);
    fsProxyModel->setSourceModel(model);
    ui.treeFileSystem->setModel(fsProxyModel);
}

void MainWindow::showCentered()
{
    setGeometry(QStyle::alignedRect(Qt::LeftToRight,
                                    Qt::AlignCenter,
                                    size(),
                                    qApp->desktop()->availableGeometry())
                );
}

void MainWindow::onItemChanged()
{
     // Invalidate eventual items distribution
    removeAllDisks();

    //  Compute the total item size
    const quint64 totalSize = document->getTotalItemSize();

    // Update the total item size label
    ui.lblTotalSize->setText(FSUtils::formatSize(totalSize) + ".");
}

void MainWindow::onRootPathChanged()
{
    const QString newPath = ui.editPath->text();
    ui.treeFileSystem->setRootIndex(fsProxyModel->mapFromSource(model->setRootPath(newPath)));
}

void MainWindow::onTreeFSSelectionChanged()
{
    const QModelIndexList indexes = ui.treeFileSystem->selectionModel()->selectedRows();
    ui.btnAddItem->setEnabled(!indexes.empty());
    ui.btnAddItemSeparate->setEnabled(!indexes.empty());
}

void MainWindow::onItemsSelectionChanged()
{
    const QModelIndexList indexes = ui.tblItems->selectionModel()->selectedRows();
    ui.btnRemoveItem->setEnabled(!indexes.empty());

    // Single selection only
    if (indexes.size() != 1)
        itemDetailsModel->setItem(NULL);
    else
        itemDetailsModel->setItem(itemsModel->getItem(indexes.at(0)));
}

void MainWindow::onItemDetailsSelectionChanged()
{
    const QModelIndexList indexes = ui.tblItemDetails->selectionModel()->selectedRows();
    ui.btnRemoveFromItem->setEnabled(!indexes.empty());
}

void MainWindow::onCurrentDiskChanged()
{    
    const int index = ui.cboDisks->currentIndex();    
    //ui.btnRemoveDisk->setEnabled(index != -1);
    if (index != -1)
        diskDetailsModel->setDisk(document->getDisk(index));
}

void MainWindow::on_btnAddItem_clicked()
{
    const QModelIndexList indexes = ui.treeFileSystem->selectionModel()->selectedRows();
    statusBar()->showMessage(tr("Computing size..."));
    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Create the new item
    Item *item = new Item();

    // Build the individual entries
    foreach (QModelIndex index, indexes)
        item->addEntry(entryFromIndex(index));

    QApplication::restoreOverrideCursor();
    statusBar()->clearMessage();
    addItem(item);
}

void MainWindow::on_btnAddItemSeparate_clicked()
{
    const QModelIndexList indexes = ui.treeFileSystem->selectionModel()->selectedRows();
    statusBar()->showMessage(tr("Computing size..."));
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QList<Item *> newItems;

    // Build the individual entries
    foreach (QModelIndex index, indexes)
    {
        // Create the new item
        Item *item = new Item();
        item->addEntry(entryFromIndex(index));

        // Collect it
        newItems.push_back(item);
    }

    // Add
    foreach (Item *item, newItems)
        addItem(item);

    QApplication::restoreOverrideCursor();
    statusBar()->clearMessage();
}

void MainWindow::on_btnRemoveItem_clicked()
{
    const QModelIndexList indexes = ui.tblItems->selectionModel()->selectedRows();

    // Collect the items
    QList<Item *> itemsToRemove;
    foreach (QModelIndex index, indexes)
        itemsToRemove.push_back(itemsModel->getItem(index));
    
    removeItems(itemsToRemove);
}

void MainWindow::on_btnRemoveFromItem_clicked()
{
    const QModelIndexList indexes = ui.tblItemDetails->selectionModel()->selectedRows();

    // Collect the entries
    QList<Item::ItemEntry *> itemsToRemove;
    foreach (QModelIndex index, indexes)
        itemsToRemove.push_back(itemDetailsModel->getItemEntry(index));

    // Remove the entries
    removeEntriesFromCurrentItem(itemsToRemove);
}

void MainWindow::on_actionComputeDisks_triggered()
{
    if (!isSolvable())
    {
        QMessageBox::critical(this, tr("Error"), tr("Every item's size must be less or equal than the disk capacity!"));    
        return;
    }

    statusBar()->showMessage(tr("Distributing items..."));

    BinSolver *solver = new BFDSolver();

    // Init the solver
    const quint64 capacity = getGlobalDiskCapacity();
    solver->setDiskCapacity(capacity);
    for (int i = 0; i < document->getItemCount(); i++)
        solver->addItem(document->getItem(i)->getTotalSize());

    // Solve
    QApplication::setOverrideCursor(Qt::WaitCursor);
    solver->solve();
    QApplication::restoreOverrideCursor();

    // Setup results
    removeAllDisks();

    for (int i = 0; i < solver->getDiskCount(); i++)
        addDisk(new Disk(capacity));

    for (int i = 0; i < document->getItemCount(); i++)
        document->getDisk(solver->getItemAssignment(i))->addItem(document->getItem(i));

    delete solver;

    // Select the first disk by default
    if (ui.cboDisks->count() > 0)
        ui.cboDisks->setCurrentIndex(0);

    statusBar()->showMessage(tr("Done (%1 disks created).").arg(document->getDiskCount()));
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About DiskSpan"),
            tr("<b>DiskSpan</b> v0.1.0 by Danilo Arcidiacono<br>"
               "<a href=\"mailto:danilo.arcidiacono@gmail.com\">danilo.arcidiacono@gmail.com</a><br>"
               "<a href=\"https://github.com/daniloarcidiacono\">Github</a>"));
}

bool MainWindow::isSolvable() const
{
    const quint64 capacity = getGlobalDiskCapacity();

    // Check if every item has capacity less than the disk size
    for (int i = 0; i < document->getItemCount(); i++)
        if (document->getItem(i)->getTotalSize() > capacity)
            return false;

    return true;
}

quint64 MainWindow::getGlobalDiskCapacity() const
{
    if (ui.cboUnit->currentIndex() == 0) return ui.spinCapacity->value() * 1;
    if (ui.cboUnit->currentIndex() == 1) return ui.spinCapacity->value() * FSUtils::KB;
    if (ui.cboUnit->currentIndex() == 2) return ui.spinCapacity->value() * FSUtils::MB;
    if (ui.cboUnit->currentIndex() == 3) return ui.spinCapacity->value() * FSUtils::GB;

    return 0;
}

Item::ItemEntry *MainWindow::entryFromIndex(const QModelIndex &index) const
{
    Item::ItemEntry *entry = new Item::ItemEntry();
    entry->path = model->fileInfo(fsProxyModel->mapToSource(index));
    entry->size = FSUtils::computeSize(entry->path.absoluteFilePath());

    return entry;
}

void MainWindow::removeItems(const QList<Item *> &itemsToRemove)
{
    // Remove the detail view
    itemDetailsModel->setItem(NULL);

    // Remove the items
    document->removeItems(itemsToRemove);

    // Reset the selection
    ui.tblItems->selectionModel()->clearSelection();
}

void MainWindow::removeCurrentItem()
{
    Item *item = itemDetailsModel->getItem();
    
    // Remove the detail view
    itemDetailsModel->setItem(NULL);

    // Remove the items
    document->removeItem(item);

    // Reset the selection
    ui.tblItems->selectionModel()->clearSelection();
}

void MainWindow::addItem(Item *item)
{
    // Add the item to the document
    document->addItem(item);

    // Reset the selection
    ui.treeFileSystem->selectionModel()->clearSelection();
}

void MainWindow::removeEntriesFromCurrentItem(const QList<Item::ItemEntry *> &itemsToRemove)
{
    Item *item = itemDetailsModel->getItem();
    item->removeEntries(itemsToRemove);

    if (item->getEntryCount() == 0)
        removeCurrentItem();    

    // Reset the selection
    ui.tblItemDetails->selectionModel()->clearSelection();
}

void MainWindow::addDisk(Disk *disk)
{
    document->addDisk(disk);
}

void MainWindow::removeAllDisks()
{
    diskDetailsModel->setDisk(NULL);
    document->removeDisks();
    ui.tblDisk->selectionModel()->clearSelection();
}