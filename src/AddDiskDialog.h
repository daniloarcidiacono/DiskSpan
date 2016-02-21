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
#ifndef ADDDISKDIALOG_H
#define ADDDISKDIALOG_H
#include "ui_adddiskdialog.h"

class AddDiskDialog : public QDialog
{
	Q_OBJECT

private:
	Ui::AddDiskDialog ui;

public:
	AddDiskDialog(QWidget *parent);
	virtual ~AddDiskDialog();

	// Returns the specified capacity in bytes
	quint64 getCapacity() const;
};

#endif