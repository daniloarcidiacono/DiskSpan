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