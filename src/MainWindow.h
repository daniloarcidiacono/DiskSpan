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

// Forward declarations
class Model;
class DocumentComponent;
class FileSystemComponent;
class TransferComponent;
class ItemsComponent;
class DisksComponent;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindowClass ui;

    // Document (OWNED by this class)
	Model *document;

	// Components
    DocumentComponent *cmpDocument;
    FileSystemComponent *cmpFileSystem;
	TransferComponent *cmpTransfer;
	ItemsComponent *cmpItems;
	DisksComponent *cmpDisks;

protected:
	void createComponents();
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

protected slots:
	void on_actionComputeDisks_triggered();
    void about();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif