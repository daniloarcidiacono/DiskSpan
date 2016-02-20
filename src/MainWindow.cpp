#include <QtWidgets>
#include <QStyle>
#include <QDesktopWidget>
#include <QTextStream>
#include <QDebug>
#include <QtAlgorithms>
#include "MainWindow.h"
#include "Model.h"
#include "ItemsModel.h"
#include "ItemDetailsModel.h"
#include "FSTreeProxyFilter.h"
#include "AddDiskDialog.h"
#include "FSUtils.h"
#include "Disk.h"
#include "DiskModel.h"
#include "DiskDetailsModel.h"

extern "C"
{
    #include "mulknap.h"
}

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

    ui.editPath->setText("/home/danilo/Desktop");

    showCentered();
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
    ui.btnRemoveDisk->setEnabled(index != -1);
    if (index != -1)
        diskDetailsModel->setDisk(document->getDisk(index));
}

void MainWindow::on_btnAddItem_clicked()
{
    const QModelIndexList indexes = ui.treeFileSystem->selectionModel()->selectedRows();
    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Create the new item
    Item *item = new Item();

    // Build the individual entries
    foreach (QModelIndex index, indexes)
        item->addEntry(entryFromIndex(index));

    QApplication::restoreOverrideCursor();
    addItem(item);
}

void MainWindow::on_btnAddItemSeparate_clicked()
{
    const QModelIndexList indexes = ui.treeFileSystem->selectionModel()->selectedRows();
    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Build the individual entries
    foreach (QModelIndex index, indexes)
    {
        // Create the new item
        Item *item = new Item();
        item->addEntry(entryFromIndex(index));

        addItem(item);
    }

    QApplication::restoreOverrideCursor();
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

void MainWindow::on_btnAddDisk_clicked()
{
    AddDiskDialog *dlg = new AddDiskDialog(this);

    switch (dlg->exec())
    {
        case QDialog::Accepted:
        {
            Disk *disk = new Disk(dlg->getCapacity());
            addDisk(disk);
            break;
        }
    }
}

void MainWindow::on_btnRemoveDisk_clicked()
{
}

/*
template<typename T> class CompareIndicesByAnotherVectorValues
{ 
    T *values;

public: 
    CompareIndicesByAnotherVectorValues(T *_values)
        : values(_values)
    {
    } 

    bool operator()(const int &a, const int &b) const
    { 
        return (values)[a] > (values)[b]; 
    } 
}; 

template <typename T> int *sort_indexes(T *v, const int vSize)
{
    // initialize original index locations
    int *idx = new int[vSize];

    for (int i = 0; i != vSize; i++)
        idx[i] = i;

    // sort indexes based on comparing values in v
    sort(idx.begin(), idx.end(), CompareIndicesByAnotherVectorValues<int>(v));

    return idx;
}*/

void MainWindow::on_actionComputeDisks_triggered()
{
    /*
    n: numero di oggetti
    m: numero di zaini
    p: profitto per oggetto (interi positivi)
    w: peso per oggetto (interi positivi)
    c: capacità per zaino (interi positivi)
    x: risultati*/

    const int n = document->getItemCount();
    const int m = document->getDiskCount();
    int *p = new int[n];
    int *w = p;
    int *c = new int[m];
    int *x = new int[n];
    
    for (int i = 0; i < n; i++)
        p[i] = document->getItem(i)->getTotalSize();

    for (int i = 0; i < m; i++)
        c[i] = document->getDisk(i)->getCapacity();

    const int *cIndexes = sort_indexes(c, m);

    qDebug() << "Before invoking mulknap..." << endl;
    
    //qSort(list.begin(), list.end(), CompareIndicesByAnotherVectorValues<int>(c));

    QApplication::setOverrideCursor(Qt::WaitCursor);   
    mulknap(n, m, p, w, x, c);
    QApplication::restoreOverrideCursor();

    for (int i = 0; i < m; i++)
        document->getDisk(i)->clear();

    for (int i = 0; i < n; i++)
    {
        qDebug() << QString("x[%1] = %2").arg(i).arg(x[i]) << endl;

        // i: indice item
        // x[i]: indice disco
        Item *item = document->getItem(i);
        Disk *disk = document->getDisk(x[i]);
        disk->addItem(item);
    }

    delete []p;
    delete []c;
    delete []x;
    //delete []cIndexes;
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About DiskSpan"),
            tr("The <b>DiskSpan</b> example demonstrates how to "
               "write modern GUI applications using Qt, with a menu bar, "
               "toolbars, and a status bar."));
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