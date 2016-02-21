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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "Item.h"

// Forward declarations
class QFileSystemModel;
class ItemsModel;
class ItemDetailsModel;
class FSTreeProxyFilter;
class Model;
class Disk;
class DiskModel;
class DiskDetailsModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindowClass ui;

    // Document
    Model *document;

    // Models
	QFileSystemModel *model;
	ItemsModel *itemsModel;
	ItemDetailsModel *itemDetailsModel;
	DiskModel *diskModel;
	DiskDetailsModel *diskDetailsModel;

	// Proxy models
	FSTreeProxyFilter *fsProxyModel;

protected:
	void createModels();
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void showCentered();    

	// Service methods
	bool isSolvable() const;
	quint64 getGlobalDiskCapacity() const;
	Item::ItemEntry *entryFromIndex(const QModelIndex &index) const;
	void removeItems(const QList<Item *> &itemsToRemove);
	void removeCurrentItem();
	void addItem(Item *item);
	void removeEntriesFromCurrentItem(const QList<Item::ItemEntry *> &itemsToRemove);
	void addDisk(Disk *disk);
	void removeAllDisks();

protected slots:
	void onItemChanged();
	void onRootPathChanged();
	void onTreeFSSelectionChanged();
	void onItemsSelectionChanged();
	void onItemDetailsSelectionChanged();
	void onCurrentDiskChanged();
	void on_btnAddItem_clicked();
	void on_btnAddItemSeparate_clicked();
	void on_btnRemoveItem_clicked();
	void on_btnRemoveFromItem_clicked();
	void on_actionComputeDisks_triggered();
    void about();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif