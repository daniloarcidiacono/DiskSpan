/*  
 *	Copyright 2016 Danilo Arcidiacono. All Rights Reserved.
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
 *
 */
#include <QtWidgets>
#include <QMainWindow>
#include <QDebug>
#include "ItemsComponent.h"
#include "../model/Model.h"
#include "../model/Item.h"
#include "../FSUtils.h"
#include "../ItemsModel.h"
#include "../ItemDetailsModel.h"

ItemsComponent::ItemsComponent(QMainWindow *_context, Model *_document, QObject *parent)
	: QObject(parent)
{
	// Setup members
	context = _context;
    document = _document;    
}

ItemsComponent::~ItemsComponent()
{
}

void ItemsComponent::setupUi(QWidget *container)
{
    ui.setupUi(container);
    createModels();

    // Wiring
    QObject::connect(ui.tblItems->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(onItemsSelectionChanged()));
    QObject::connect(ui.tblItemDetails->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(onItemDetailsSelectionChanged()));
    QObject::connect(ui.btnRemoveFromItem, SIGNAL(clicked()), this, SLOT(removeEntriesFromCurrentItem()));
    QObject::connect(document, SIGNAL(onItemChanged()), this, SLOT(updateTotalSizeLabel()));
}

void ItemsComponent::createModels()
{
    // Items
    itemsModel = new ItemsModel(document);
    ui.tblItems->setModel(itemsModel);

    // Item details
    itemDetailsModel = new ItemDetailsModel(NULL);
    ui.tblItemDetails->setModel(itemDetailsModel);
}

void ItemsComponent::onItemsSelectionChanged()
{
    const QModelIndexList indexes = ui.tblItems->selectionModel()->selectedRows();
    QList<Item *> selectedItems;
    foreach (QModelIndex index, indexes)
        selectedItems.push_back(itemsModel->getItem(index));

    emit itemsSelected(selectedItems);    

    // Single selection only
    if (indexes.size() != 1)
        itemDetailsModel->setItem(NULL);
    else
        itemDetailsModel->setItem(itemsModel->getItem(indexes.at(0)));
}

void ItemsComponent::onItemDetailsSelectionChanged()
{
    const QModelIndexList indexes = ui.tblItemDetails->selectionModel()->selectedRows();
    ui.btnRemoveFromItem->setEnabled(!indexes.empty());
}

void ItemsComponent::removeEntriesFromCurrentItem()
{
    const QModelIndexList indexes = ui.tblItemDetails->selectionModel()->selectedRows();

    // Collect the entries
    QList<Item::ItemEntry *> itemsToRemove;
    foreach (QModelIndex index, indexes)
        itemsToRemove.push_back(itemDetailsModel->getItemEntry(index));

    // Remove the entries
    Item *item = itemDetailsModel->getItem();
    item->removeEntries(itemsToRemove);

    if (item->getEntryCount() == 0)
    {
        // Remove the detail view
        itemDetailsModel->setItem(NULL);

        // Remove the items
        document->removeItem(item);

        // Reset the selection
        ui.tblItems->selectionModel()->clearSelection();
    }

    // Reset the selection
    ui.tblItemDetails->selectionModel()->clearSelection();
}

void ItemsComponent::updateTotalSizeLabel()
{
    // Invalidate eventual items distribution
    //removeAllDisks();

    //  Compute the total item size
    const quint64 totalSize = document->getTotalItemSize();

    // Update the total item size label
    ui.lblTotalSize->setText(FSUtils::formatSize(totalSize) + ".");
}