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
#include "TransferComponent.h"
#include "../model/Model.h"
#include "../model/Item.h"
#include "../FSUtils.h"

TransferComponent::TransferComponent(QMainWindow *_context, Model *_document, QObject *parent)
	: QObject(parent)
{
	// Setup members
	context = _context;
    document = _document;    
}

TransferComponent::~TransferComponent()
{
}


void TransferComponent::setupUi(QWidget *container)
{
    ui.setupUi(container);

    // Wiring
    QObject::connect(ui.btnAddItem, SIGNAL(clicked()), this, SLOT(addItems()));
    QObject::connect(ui.btnAddItemSeparate, SIGNAL(clicked()), this, SLOT(addSeparatedItems()));
    QObject::connect(ui.btnRemoveItem, SIGNAL(clicked()), this, SLOT(removeItems()));
}

void TransferComponent::setCurrentFiles(const QList<QFileInfo> &files)
{
    currentFiles.clear();
    currentFiles += files;

    ui.btnAddItem->setEnabled(!currentFiles.empty());
    ui.btnAddItemSeparate->setEnabled(!currentFiles.empty());
}

void TransferComponent::setCurrentItems(const QList<Item *> &items)
{    
    currentItems.clear();
    currentItems += items;

    ui.btnRemoveItem->setEnabled(!currentItems.empty());
}

static Item::ItemEntry *entryFromInfo(const QFileInfo &info)
{    
    // Create the entry
    Item::ItemEntry *entry = new Item::ItemEntry();
    entry->path = info;
    entry->size = FSUtils::computeSize(info.absoluteFilePath());

    return entry;
}

void TransferComponent::addItems()
{
    context->statusBar()->showMessage(tr("Computing size..."));
    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Create the new item
    Item *item = new Item();

    // Build the individual entries
    foreach (const QFileInfo &info, currentFiles)
        item->addEntry(entryFromInfo(info));

    // Add the item to the document
    document->addItem(item);

    QApplication::restoreOverrideCursor();
    context->statusBar()->clearMessage();
}

void TransferComponent::addSeparatedItems()
{    
    context->statusBar()->showMessage(tr("Computing size..."));
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QList<Item *> newItems;

    // Build the individual entries
    foreach (const QFileInfo &info, currentFiles)
    {
        // Create the new item
        Item *item = new Item();
        item->addEntry(entryFromInfo(info));

        // Collect it
        newItems.push_back(item);
    }

    // Add the items to the document
    document->addItems(newItems);

    QApplication::restoreOverrideCursor();
    context->statusBar()->clearMessage();
}

void TransferComponent::removeItems()
{

    // Remove the detail view
    //itemDetailsModel->setItem(NULL);

    // Remove the items
    document->removeItems(currentItems);

    // Reset the selection
    //ui.tblItems->selectionModel()->clearSelection();
}