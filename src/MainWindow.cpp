#include <QtWidgets>
#include <QStyle>
#include <QDesktopWidget>
#include <QTextStream>
#include <QDebug>
#include "MainWindow.h"
#include "Model.h"
#include "ItemsModel.h"
#include "ItemDetailsModel.h"
#include "FSUtils.h"

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
    QObject::connect(ui.editPath, SIGNAL(textChanged(const QString &)), this, SLOT(onRootPathChanged(const QString &)));
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
    ui.treeFileSystem->setModel(model);

    // Items
    itemsModel = new ItemsModel(document);
    ui.tblItems->setModel(itemsModel);

    // Item details
    itemDetailsModel = new ItemDetailsModel(NULL);
    ui.tblItemDetails->setModel(itemDetailsModel);
}

void MainWindow::showCentered()
{
    setGeometry(QStyle::alignedRect(Qt::LeftToRight,
                                    Qt::AlignCenter,
                                    size(),
                                    qApp->desktop()->availableGeometry())
                );
}

void MainWindow::onRootPathChanged(const QString &value)
{
    QModelIndex pathIndex = model->index(value);

    if (pathIndex.isValid())
        ui.treeFileSystem->setRootIndex(pathIndex);
    else
        ui.treeFileSystem->setRootIndex(model->index(QDir::currentPath()));
}

void MainWindow::onTreeFSSelectionChanged()
{
    const QModelIndexList indexes = ui.treeFileSystem->selectionModel()->selectedRows();
    ui.btnAddItem->setEnabled(!indexes.empty());
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

void MainWindow::on_btnAddItem_clicked()
{
    const QModelIndexList indexes = ui.treeFileSystem->selectionModel()->selectedRows();

    // Create the new item
    Item *item = new Item();

    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Build the individual entries
    foreach (QModelIndex index, indexes)
    {
        Item::ItemEntry *entry = new Item::ItemEntry();
        entry->path = model->fileInfo(index);
        entry->size = FSUtils::computeSize(entry->path.absoluteFilePath());

        item->addEntry(entry);
    }

    QApplication::restoreOverrideCursor();
    addItem(item);
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

void MainWindow::about()
{
   QMessageBox::about(this, tr("About DiskSpan"),
            tr("The <b>DiskSpan</b> example demonstrates how to "
               "write modern GUI applications using Qt, with a menu bar, "
               "toolbars, and a status bar."));
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
