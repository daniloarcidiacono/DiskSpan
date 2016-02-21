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
#ifndef FILESYSTEMCOMPONENT_H
#define FILESYSTEMCOMPONENT_H
#include <QObject>
#include <QFileInfo>
#include "ui_filesystemwidget.h"

// Forward declarations
class QMainWindow;
class QFileSystemModel;
class FSTreeProxyFilter;
class Model;

// Component that handles the File System Tree panel
class FileSystemComponent : public QObject
{
	Q_OBJECT

private:
	// Reference to the main window (NOT owned by this class)
	QMainWindow *context;

	// Reference to the document (NOT owned by this class)
	Model *document;

	// Ui
	Ui::FileSystemWidget ui;

    // Models
	QFileSystemModel *model;

	// Proxy models
	FSTreeProxyFilter *fsProxyModel;

	void createModels();

protected slots:
	void onRootPathChanged();
	void onTreeFSSelectionChanged();

signals:
	void fileSelected(const QList<QFileInfo> &files);

public:
	FileSystemComponent(QMainWindow *_context, Model *_document, QObject *parent = 0);
	virtual ~FileSystemComponent();

	// Setups the ui
	void setupUi(QWidget *container);
};

#endif