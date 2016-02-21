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
#ifndef TRANSFERCOMPONENT_H
#define TRANSFERCOMPONENT_H
#include <QObject>
#include <QList>
#include <QFileInfo>
#include "ui_transferwidget.h"

// Forward declarations
class QMainWindow;
class Model;
class Item;

// Component that handles the transferring between the file system and the items
class TransferComponent : public QObject
{
	Q_OBJECT

private:
	// Reference to the main window (NOT owned by this class)
	QMainWindow *context;

	// Reference to the document (NOT owned by this class)
	Model *document;

	// Ui
	Ui::TransferWidget ui;

	// Files to transfer
	QList<QFileInfo> currentFiles;

	// Current items (NOT owned by this class)
	QList<Item *> currentItems;

protected slots:
	void setCurrentFiles(const QList<QFileInfo> &files);
	void setCurrentItems(const QList<Item *> &items);	
	void addItems();
	void addSeparatedItems();
	void removeItems();

public:
	TransferComponent(QMainWindow *_context, Model *_document, QObject *parent = 0);
	virtual ~TransferComponent();

	// Setups the ui
	void setupUi(QWidget *container);
};

#endif