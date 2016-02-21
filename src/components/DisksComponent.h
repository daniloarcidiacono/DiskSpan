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
#ifndef DISKSCOMPONENT_H
#define DISKSCOMPONENT_H
#include <QObject>
#include <QList>
#include "ui_diskswidget.h"

// Forward declarations
class QMainWindow;
class Model;
class DiskModel;
class DiskDetailsModel;

// Component that handles the disks
class DisksComponent : public QObject
{
	Q_OBJECT

private:
	// Reference to the main window (NOT owned by this class)
	QMainWindow *context;

	// Reference to the document (NOT owned by this class)
	Model *document;

	// Ui
	Ui::DisksWidget ui;

	// Models
	DiskModel *diskModel;
	DiskDetailsModel *diskDetailsModel;

	void createModels();

protected slots:
	void onCurrentDiskChanged();
	void onDisksChanged();
	
public slots:
	void removeAllDisks();

public:
	DisksComponent(QMainWindow *_context, Model *_document, QObject *parent = 0);
	virtual ~DisksComponent();

	// Returns the current global disk capacity
	quint64 getGlobalDiskCapacity() const;

	// Returns TRUE if the current settings are solvable
	bool isSolvable() const;

	// Setups the ui
	void setupUi(QWidget *container);
};

#endif