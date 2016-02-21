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
#include <QDataStream>
#include "DocumentComponent.h"
#include "../model/Model.h"

DocumentComponent::DocumentComponent(QMainWindow *_context, Model *_document, QObject *parent)
	: QObject(parent)
{
	// Setup members
	context = _context;
	document = _document;

	setCurrentFile("");
    context->setUnifiedTitleAndToolBarOnMac(true);

    // Wiring
    QObject::connect(document, SIGNAL(onItemChanged()), this, SLOT(markModified()));
    QObject::connect(document, SIGNAL(onDiskChanged()), this, SLOT(markModified()));
}

DocumentComponent::~DocumentComponent()
{
}

void DocumentComponent::setCurrentFile(const QString &fileName)
{
    curFile = fileName;    
    unmarkModified();

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = tr("untitled");

    context->setWindowFilePath("DiskSpan - [" + shownName + "]");
}

void DocumentComponent::onCloseEvent(QCloseEvent *event)
{
	if (maybeSave())
        event->accept();
    else 
        event->ignore();
}

bool DocumentComponent::maybeSave()
{
	if (isModified)
	{
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(context, tr("DiskSpan"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (ret == QMessageBox::Save)
            return save();
        else 
        	if (ret == QMessageBox::Cancel)
            	return false;
    }

    return true;
}

void DocumentComponent::markModified()
{
	isModified = true;
	context->setWindowModified(true);
}

void DocumentComponent::unmarkModified()
{
	isModified = false;
	context->setWindowModified(false);
}

bool DocumentComponent::save()
{
    if (curFile.isEmpty())
        return saveAs();
    else
        return saveFile(curFile);
}

bool DocumentComponent::saveAs()
{
    QFileDialog dialog(context);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    QStringList files;
    if (dialog.exec())
        files = dialog.selectedFiles();
    else
        return false;

    return saveFile(files.at(0));
}

bool DocumentComponent::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) 
    {
        QMessageBox::warning(context, tr("DiskSpan"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QDataStream out(&file);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << *document;
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    context->statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void DocumentComponent::newFile()
{
    if (maybeSave()) 
    {
        // Reset the document
        document->reset();
        setCurrentFile("");
    }
}

void DocumentComponent::open()
{
    if (maybeSave()) 
    {
        QString fileName = QFileDialog::getOpenFileName(context);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

void DocumentComponent::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(context, tr("DiskSpan"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QDataStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    in >> *document;
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    context->statusBar()->showMessage(tr("File loaded"), 2000);
}