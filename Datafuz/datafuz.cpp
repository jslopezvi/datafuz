#include "datafuz.h"

#include <QMessageBox>

Datafuz::Datafuz(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);	
	connect(ui.update_vibrations_opts, SIGNAL(clicked()), this, SLOT(updateVibrationsOpts()));

	ui.sel_vib_channels_grbox->setVisible(false);
	ui.layout_vib_checks->setAlignment(Qt::AlignCenter);
}

Datafuz::~Datafuz()
{

}

void Datafuz::updateVibrationsOpts() {
	QMessageBox::information(this, "Vibrations options updated...", "Updating vibrations options...");
	ui.sel_vib_channels_grbox->setVisible(true);

	int vib_channels_n = ui.vib_channel_n->currentIndex() + 1;

	QVector<QCheckBox *> checkboxes;
	
	for (int i = 0; i < vib_channels_n; i++) {
		QCheckBox * checkbox = new QCheckBox(QString::number(i + 1));
		checkbox->setChecked(false);
		ui.layout_vib_checks->addWidget(checkbox);
		checkboxes.push_back(checkbox);
	}
}
