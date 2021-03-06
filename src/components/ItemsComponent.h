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
#ifndef ITEMSCOMPONENT_H
#define ITEMSCOMPONENT_H
#include <QObject>
#include <QList>
#include <QFileInfo>
#include "ui_itemswidget.h"

// Forward declarations
class QMainWindow;
class Model;
class Item;
class ItemsModel;
class ItemDetailsModel;

// Component that handles the items and their details
class ItemsComponent : public QObject
{
	Q_OBJECT

private:
	// Reference to the main window (NOT owned by this class)
	QMainWindow *context;

	// Reference to the document (NOT owned by this class)
	Model *document;

	// Ui
	Ui::ItemsWidget ui;

	// Models
	ItemsModel *itemsModel;
	ItemDetailsModel *itemDetailsModel;

	void createModels();

protected slots:
	void onItemsSelectionChanged();
	void onItemDetailsSelectionChanged();
	void removeEntriesFromCurrentItem();
	void updateTotalSizeLabel();
	
signals:
	void itemsSelected(const QList<Item *> &selectedItems);

public:
	ItemsComponent(QMainWindow *_context, Model *_document, QObject *parent = 0);
	virtual ~ItemsComponent();

	// Setups the ui
	void setupUi(QWidget *container);
};

#endif