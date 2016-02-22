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
#include "DisksComponent.h"
#include "../DiskModel.h"
#include "../DiskDetailsModel.h"
#include "../model/Model.h"
#include "../model/Item.h"
#include "../FSUtils.h"

DisksComponent::DisksComponent(QMainWindow *_context, Model *_document, QObject *parent)
	: QObject(parent)
{
	// Setup members
	context = _context;
    document = _document;    
}

DisksComponent::~DisksComponent()
{
}

void DisksComponent::setupUi(QWidget *container)
{
    ui.setupUi(container);
    createModels();

    // Wiring
    QObject::connect(ui.cboDisks, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentDiskChanged()));
    QObject::connect(document, SIGNAL(onItemChanged()), this, SLOT(removeAllDisks()));
    QObject::connect(document, SIGNAL(onDiskChanged()), this, SLOT(onDisksChanged()));
}

void DisksComponent::createModels()
{
    // Disks
    diskModel = new DiskModel(document);
    ui.cboDisks->setModel(diskModel);

    // Disk details
    diskDetailsModel = new DiskDetailsModel(NULL);
    ui.tblDisk->setModel(diskDetailsModel);
}

void DisksComponent::onCurrentDiskChanged()
{    
    const int index = ui.cboDisks->currentIndex();
    if (index != -1)
        diskDetailsModel->setDisk(document->getDisk(index));
}

void DisksComponent::onDisksChanged()
{
    // Select the first disk by default
    if (document->getDiskCount() > 0)
        ui.cboDisks->setCurrentIndex(0);
}

void DisksComponent::removeAllDisks()
{
    diskDetailsModel->setDisk(NULL);
    document->removeDisks();
    ui.tblDisk->selectionModel()->clearSelection();
}

quint64 DisksComponent::getGlobalDiskCapacity() const
{
    if (ui.cboUnit->currentIndex() == 0) return ui.spinCapacity->value() * 1;
    if (ui.cboUnit->currentIndex() == 1) return ui.spinCapacity->value() * FSUtils::KB;
    if (ui.cboUnit->currentIndex() == 2) return ui.spinCapacity->value() * FSUtils::MB;
    if (ui.cboUnit->currentIndex() == 3) return ui.spinCapacity->value() * FSUtils::GB;

    return 0;
}

bool DisksComponent::isSolvable() const
{
    const quint64 capacity = getGlobalDiskCapacity();

    // Check if every item has capacity less than the disk size
    for (int i = 0; i < document->getItemCount(); i++)
        if (document->getItem(i)->getTotalSize() > capacity)
            return false;

    return true;
}