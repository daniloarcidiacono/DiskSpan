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
#include <QFileSystemModel>
#include <QDebug>
#include <QDesktopServices>
#include "FileSystemComponent.h"
#include "../FSTreeProxyFilter.h"
#include "../model/Model.h"
#include "../FSUtils.h"

FileSystemComponent::FileSystemComponent(QMainWindow *_context, Model *_document, QObject *parent)
	: QObject(parent)
{
	// Setup members
	context = _context;
    document = _document;    
}

FileSystemComponent::~FileSystemComponent()
{
}

void FileSystemComponent::createModels()
{
    // File system
    model = new QFileSystemModel();
    model->setRootPath(QDir::currentPath());

    // Proxy
    fsProxyModel = new FSTreeProxyFilter(document);
    fsProxyModel->setSourceModel(model);
    ui.treeFileSystem->setModel(fsProxyModel);
}

void FileSystemComponent::setupUi(QWidget *container)
{
    ui.setupUi(container);
    ui.treeFileSystem->setContextMenuPolicy(Qt::CustomContextMenu);
    ui.editPath->setText("/home/danilo/Desktop");
    createModels();

    // Wiring
    QObject::connect(ui.treeFileSystem->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(onTreeFSSelectionChanged()));
    QObject::connect(ui.editPath, SIGNAL(textChanged(const QString &)), this, SLOT(onRootPathChanged()));
    QObject::connect(ui.treeFileSystem, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContextMenu(const QPoint &)));
}

void FileSystemComponent::showContextMenu(const QPoint &pos)
{
    const QModelIndex index = ui.treeFileSystem->indexAt(pos);
    if (index.isValid())
    {
        // Build the actions
        QAction *actionShowInExplorer = new QAction(QIcon(":/open.png"), tr("&Open containing folder..."), NULL);
        actionShowInExplorer->setStatusTip(tr("Opens the containing folder of the item"));
        QObject::connect(actionShowInExplorer, SIGNAL(triggered()), this, SLOT(openContainingFolder()));

        // Show the menu
        // Note: We must map the point to global from the viewport to account for the header.
        QMenu *menu = new QMenu();
        menu->addAction(actionShowInExplorer);
        menu->exec(ui.treeFileSystem->viewport()->mapToGlobal(pos));
       delete menu;
    }
}

void FileSystemComponent::openContainingFolder()
{
    const QModelIndex index = ui.treeFileSystem->currentIndex(); 
    if (index.isValid())
    {        
        // TODO: SEGMENTATION FAULT????
        // Get the file        
        qDebug() << "Before fileInfo";
        qDebug() << "row = " << index.row() << ", col = " << index.column();
        qDebug() << "name = " << model->filePath(index);

        const QFileInfo info = model->fileInfo(index);
        //qDebug() << info.absoluteDir().path();

        QDesktopServices::openUrl(QUrl::fromLocalFile(info.absoluteDir().path()));
    }
}

void FileSystemComponent::onRootPathChanged()
{
    const QString newPath = ui.editPath->text();
    ui.treeFileSystem->setRootIndex(fsProxyModel->mapFromSource(model->setRootPath(newPath)));
}

void FileSystemComponent::onTreeFSSelectionChanged()
{
    const QModelIndexList indexes = ui.treeFileSystem->selectionModel()->selectedRows();

    // Build the entries
    QList<QFileInfo> selectedEntries;
    foreach (QModelIndex index, indexes)
    {
        // Push it
        QFileInfo info = model->fileInfo(fsProxyModel->mapToSource(index));
        selectedEntries.push_back(info);
    }

    // Emit the signal
    emit fileSelected(selectedEntries);
}