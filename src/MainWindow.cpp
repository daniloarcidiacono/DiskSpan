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
