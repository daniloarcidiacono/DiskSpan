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
#ifndef DOCUMENTCOMPONENT_H
#define DOCUMENTCOMPONENT_H
#include <QObject>

// Forward declarations
class Model;
class QMainWindow;
class QCloseEvent;

// Component that handles the document mechanisms (save, open, ask for save, window title, etc..)
class DocumentComponent : public QObject
{
	Q_OBJECT

private:
	// Reference to the main window (NOT owned by this class)
	QMainWindow *context;

    // Reference to the document (NOT owned by this class)
    Model *document;

	// Current file name
	QString curFile;

	// Flag 
	bool isModified;

	// Sets the current file name
	void setCurrentFile(const QString &fileName);
	bool maybeSave();
	bool saveFile(const QString &fileName);
	void loadFile(const QString &fileName);
	void unmarkModified();


protected slots:
	// Marks the document as modified
	void markModified();	

	// File operations
	void newFile();
	void open();
	bool save();
	bool saveAs();

public:
	DocumentComponent(QMainWindow *_context, Model *_document, QObject *parent = 0);
	virtual ~DocumentComponent();

	// Method to be called when the main window is closing
	void onCloseEvent(QCloseEvent *event);
};

#endif