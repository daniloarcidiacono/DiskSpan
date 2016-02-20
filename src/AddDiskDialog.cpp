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