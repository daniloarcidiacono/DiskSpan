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
#include "AddDiskDialog.h"
#include "FSUtils.h"

AddDiskDialog::AddDiskDialog(QWidget *parent)
		: QDialog(parent, Qt::WindowCloseButtonHint)
{
	// Setup the ui
	ui.setupUi(this);

	// Signals
	//QObject::connect(ui.chkKernelOnly, SIGNAL(stateChanged(int)), ui.graphicsView, SLOT(showKernelOnly(int)));
}

AddDiskDialog::~AddDiskDialog()
{	
}

quint64 AddDiskDialog::getCapacity() const
{	
	if (ui.cboUnit->currentIndex() == 0) return ui.spinCapacity->value() * 1;
	if (ui.cboUnit->currentIndex() == 1) return ui.spinCapacity->value() * FSUtils::KB;
	if (ui.cboUnit->currentIndex() == 2) return ui.spinCapacity->value() * FSUtils::MB;
	if (ui.cboUnit->currentIndex() == 3) return ui.spinCapacity->value() * FSUtils::GB;

	return -1;
}