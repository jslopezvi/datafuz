#include "datafuz.h"

#include <QDebug>
#include <QMessageBox>
#include <QProgressBar>
#include <QFileDialog>
#include <QVector>
#include <QDateTime>
#include <iostream>
#include <fstream>

#define MAX_CHANNELS_N 9
#define MAX_VIB_CHANNELS_N 7

// Adquisicion minimo de 1 canal
#define NUMBER_OF_MIN_ADQ_CHANNELS 1

// Adquisicion maximo de 7 canales
#define NUMBER_OF_MAX_ADQ_CHANNELS 7

// Tiempo de adquisicion minimo: 1 segundo
#define NUMBER_OF_MIN_ADQ_SECONDS 1

// Tiempo de adquisicion maximo: 600 segundos equivalente a 10 minutos
#define NUMBER_OF_MAX_ADQ_SECONDS 600

#define ADQ_BYTES_PER_SAMPLE 3
#define ADQ_SAMPLES_PER_SECOND 52734
#define ADQ_MAX_BUFFER_SIZE ADQ_SAMPLES_PER_SECOND * NUMBER_OF_MAX_ADQ_SECONDS

using namespace std;

Datafuz::Datafuz(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);	

	signals_config_dialog = new QDialog(this);
	signals_config_dialog_ui.setupUi(signals_config_dialog);	

	connect(ui.load_file_button, SIGNAL(clicked()), this, SLOT(loadSignalsFile()));
	connect(ui.signals_config_button, SIGNAL(clicked()), this, SLOT(showSignalsConfig()));
	connect(ui.update_vibrations_opts, SIGNAL(clicked()), this, SLOT(updateVibrationsOpts()));
	connect(ui.plot_freq_check, SIGNAL(stateChanged(int)), this, SLOT(updateFreqOpts(int)));

	connect(signals_config_dialog_ui.apply_vib_config, SIGNAL(clicked()), this, SLOT(applyVibConfig()));
	connect(signals_config_dialog_ui.default_vib_config, SIGNAL(clicked()), this, SLOT(defaultVibConfig()));

	ui.sel_vib_channels_grbox->setVisible(false);
	ui.layout_vib_checks->setAlignment(Qt::AlignCenter);	

	ui.fs_val->setEnabled(false);
	ui.fs_label->setEnabled(false);

	QStringList channels_list = QStringList();
	
	for (int i = 0; i < MAX_CHANNELS_N; i++) {
		channels_list << QString::number(i + 1);
	}

	for (int i = 0; i < MAX_VIB_CHANNELS_N; i++) {
		QCheckBox * checkbox = new QCheckBox("Vib"+QString::number(i + 1));
		checkbox->setChecked(true);
		vib_channels_checks.push_back(checkbox);

		QComboBox * combobox = new QComboBox();		
		combobox->addItems(channels_list);
		combobox->setFixedWidth(30);
		vib_channels_combos.push_back(combobox);

		QSpacerItem * spacer = new QSpacerItem(20, 30, QSizePolicy::Expanding, QSizePolicy::Fixed);
		vib_channels_spaces.push_back(spacer);

		QLineEdit * line_edit = new QLineEdit("Channel " + QString::number(i + 1));
		line_edit->setFixedWidth(60);
		vib_channels_edits.push_back(line_edit);

		DatafuzPlot *timePlot = new DatafuzPlot();
		timePlot->hideYValue();
		plotTime(timePlot, line_edit->text(), QVector<double>(), QVector<double>(), 350);
		vib_channels_time_plots.push_back(timePlot);

		DatafuzPlot *freqPlot = new DatafuzPlot();
		freqPlot->hideYValue();
		plotFreq(freqPlot, line_edit->text(), QVector<double>(), QVector<double>(), 350);
		vib_channels_freq_plots.push_back(freqPlot);

		/*QDoubleSpinBox * spinbox = new QDoubleSpinBox();
		spinbox->setMinimum(-1000.00);
		spinbox->setMaximum(1000.00);
		spinbox->setValue(100.00);
		spinbox->setFixedWidth(100);
		vib_channels_sensitivities.push_back(spinbox);*/
	}

	val_hex_pos_max = 16777216.0;
	val_volt_max = 5.0;
	val_zero_volt = 2.5;

	signals_data = new QVector<QVector<float>*>();

	ui.signals_config_button->setEnabled(false);
	ui.signals_tab_widget->setEnabled(false);
}

Datafuz::~Datafuz()
{

}

void Datafuz::plotTime(DatafuzPlot * timePlot, const QString title, QVector<double> x, QVector<double> y, int fixed_height) {
	timePlot->clearGraphs();
	timePlot->clearItems();
	timePlot->clearPlottables();
	//timePlot->update();
	//timePlot->setWindowTitle(title + " - Time");

	//	timePlot->plotLayout()->clear();

	// add two new graphs and set their look:
	timePlot->addGraph();
	timePlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
	timePlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue

	// configure right and top axis to show ticks but no labels:
	// (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
	timePlot->xAxis2->setVisible(true);
	timePlot->xAxis2->setTickLabels(false);
	timePlot->yAxis2->setVisible(true);
	timePlot->yAxis2->setTickLabels(false);

	// Note: we could have also just called timePlot->rescaleAxes(); instead
	// Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
	timePlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

	timePlot->setFixedHeight(fixed_height);

	// pass data points to graphs:
	timePlot->graph(0)->setData(x, y);

	// let the ranges scale themselves so graph 0 fits perfectly in the visible area:
	timePlot->graph(0)->rescaleAxes();

	/*QCPLayoutElement * element = timePlot->plotLayout()->element(0, 0);

	if (element != NULL) {
	timePlot->plotLayout()->remove(element);
	}*/

	timePlot->plotLayout()->insertRow(0);
	timePlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(timePlot, title + " - Time"));
	timePlot->replot();
}

void Datafuz::plotFreq(DatafuzPlot * freqPlot, const QString title, QVector<double> x, QVector<double> y, int fixed_height) {
	freqPlot->clearGraphs();
	freqPlot->clearItems();
	freqPlot->clearPlottables();
	//freqPlot->setWindowTitle(title + " - Freq");

	//freqPlot->plotLayout()->clear();

	// create empty bar chart objects:
	QCPBars *freq = new QCPBars(freqPlot->xAxis, freqPlot->yAxis);
	freqPlot->addPlottable(freq);

	freqPlot->xAxis->setVisible(true);
	freqPlot->yAxis->setVisible(true);
	// set names and colors:
	QPen pen;
	pen.setWidthF(1);
	freq->setName("Freq");
	pen.setColor(QColor(255, 131, 0));
	freq->setPen(pen);
	freq->setBrush(QColor(255, 131, 0, 50));

	freq->setData(x, y);
	freqPlot->rescaleAxes();

	freqPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

	freqPlot->setFixedHeight(fixed_height);

	freqPlot->plotLayout()->insertRow(0);
	freqPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(freqPlot, title + " - Freq"));
	freqPlot->replot();
}

void clearLayout(QLayout *layout) {
	QLayoutItem *item;
	if (layout != NULL) {
		while ((item = layout->takeAt(0)) != 0) {
			if (item->layout()) {
				clearLayout(item->layout());
				delete item->layout();
				continue;
			}
			if (item != NULL && item->widget()) {
				delete item->widget();
				continue;
			}
			if (item != NULL && item->spacerItem()) {
				delete item->spacerItem();
				continue;
			}

			if (item != NULL) {
				delete item;
			}
		}
	}
}

void clearLayouts(QLayout *layout) {
	QLayoutItem *item;
	while ((item = layout->takeAt(0))) {
		clearLayout(item->layout());

		if (item != NULL) {
			delete item;
		}
	}
}

void clearItems(QLayout *layout) {
	QLayoutItem *item;
	if (layout != NULL) {
		while ((item = layout->takeAt(0))) {
			layout->removeItem(item);
			///*if (item != NULL && item->layout()) {
			//	clearItems(item->layout());
			//	layout->removeItem(item);				
			//	continue;
			//}*/
			//if (item != NULL && item->widget()) {
			//	layout->removeWidget(item->widget());
			//	//continue;
			//}
			//if (item != NULL) {
			//	layout->removeItem(item);
			//}
		}
	}
}

void clearRows(QFormLayout *layout) {
	QLayoutItem *item;
	if (layout != NULL) {
		while ((item = layout->takeAt(0))) {
			//layout->removeItem(item);
			if (item != NULL && item->layout()) {
				clearItems(item->layout());
				layout->removeItem(item);				
				continue;
			}
			if (item != NULL && item->widget()) {
				layout->removeWidget(item->widget());
				//continue;
			}
			//if (item != NULL) {
			//	layout->removeItem(item);
			//}
		}
	}
}

void Datafuz::loadSignalsFile(void) {
	QString adq_file_path = QFileDialog::getOpenFileName(this, tr("Load signals file..."), ".", tr("Binary files (*.bin, *.BIN);; Text files (*.txt, *.TXT)"));

	if (adq_file_path == NULL) {
		QMessageBox::warning(this, "Warning", "You must select a path for signals file!");
		return;
	}

	ui.signals_file_path->setText(adq_file_path);

	if (adq_file_path.endsWith(".bin") || adq_file_path.endsWith(".BIN")) {
		n_channels = QInputDialog::getInt(this, "Number of vibration channels", "Please provide number of vibration channels in signals file:", 7);

		if (n_channels < NUMBER_OF_MIN_ADQ_CHANNELS || n_channels > NUMBER_OF_MAX_ADQ_CHANNELS) {
			QMessageBox::critical(this, "Error", "Number of vibrations channels must be between " + QString::number(NUMBER_OF_MIN_ADQ_CHANNELS) + " and " + QString::number(NUMBER_OF_MAX_ADQ_CHANNELS) +"!");
			return;
		}

		n_seconds = QInputDialog::getInt(this, "Number of seconds of vibration signals", "Please provide number of seconds of vibrations signals", 1);

		if (n_seconds < NUMBER_OF_MIN_ADQ_SECONDS || n_seconds > NUMBER_OF_MAX_ADQ_SECONDS) {
			QMessageBox::critical(this, "Error", "Number of seconds of vibration signals must be between " + QString::number(NUMBER_OF_MIN_ADQ_SECONDS) + " and " + QString::number(NUMBER_OF_MAX_ADQ_SECONDS) + "!");
			return;
		}

		signals_data->clear();

		QString adq_file_txt_path = adq_file_path.section(".", 0, 0) + ".txt";

		ifstream infile;
		infile.open(adq_file_path.toStdString(), ios::binary | ios::in);

		ofstream outfile;
		outfile.open(adq_file_txt_path.toStdString(), ios::out | ios::trunc);

		QVector<QVector<QVector<int>>> *total_bytes = new QVector<QVector<QVector<int>>>();

		unsigned char buffer[ADQ_BYTES_PER_SAMPLE];		

		unsigned int step_counter = 0;
		unsigned int step_max = n_seconds*n_channels*ADQ_SAMPLES_PER_SECOND;

		QProgressDialog *reading_progress = new QProgressDialog("Leyendo archivo ADQ...", "Cancelar", 0, step_max, this);
		reading_progress->setWindowModality(Qt::WindowModal);
		reading_progress->setWindowTitle("Leyendo...");

		reading_progress->setValue(0);

		for (unsigned short k = 0; k<n_seconds; k++) {
			QVector<QVector<int>> *seconds_bytes = new QVector<QVector<int>>();
			for (unsigned char j = 0; j<n_channels; j++) {
				QVector<int> *channel_bytes = new QVector<int>();
				for (unsigned int i = 0; i<ADQ_SAMPLES_PER_SECOND; i++) {
					infile.read((char *)&buffer, sizeof(buffer));

					unsigned int value = (buffer[0] << 16) + (buffer[1] << 8) + (buffer[2] << 0);
					channel_bytes->push_back(value);

					reading_progress->setValue(step_counter++);

					if (reading_progress->wasCanceled())
						return;
				}
				seconds_bytes->push_back(*channel_bytes);
			}
			total_bytes->push_back(*seconds_bytes);
		}

		reading_progress->setValue(step_max);

		QVector<QVector<QVector<float>>> *scaled_bytes = new QVector<QVector<QVector<float>>>();
		scaleData(total_bytes, scaled_bytes);

		step_counter = 0;

		QProgressDialog *saving_progress = new QProgressDialog("Guardando datos procesados...", "Cancelar", 0, step_max, this);
		saving_progress->setWindowModality(Qt::WindowModal);
		saving_progress->setWindowTitle("Guardando...");

		saving_progress->setValue(0);

		char printf_buffer[255] = { 0 };

		// Free signals_data.
		for (int i = 0; i < signals_data->size(); i++) {
			delete signals_data->at(i);
		}

		signals_data->clear();
		
		// Init signals data.
		for (int j = 0; j < n_channels; j++) {
			signals_data->push_back(new QVector<float>());
		}

		for (unsigned short k = 0; k<n_seconds; k++) {
			for (int i = 0; i<ADQ_SAMPLES_PER_SECOND; i++) {
				for (int j = 0; j<n_channels; j++) {					
					float value = scaled_bytes->at(k).at(j).at(i);
					sprintf(printf_buffer, "%f\t", value);
					signals_data->at(j)->push_back(value);
					outfile << printf_buffer;

					saving_progress->setValue(step_counter++);

					if (saving_progress->wasCanceled())
						return;
				}
				outfile << "\n";
			}
		}

		saving_progress->setValue(step_max);

		outfile.close();
		infile.close();

		QMessageBox::information(this, "Informacion", "Archivo ADQ procesado con éxito!");		

		clearLayout(signals_config_dialog_ui.channel_sensitivities_layout);
		vib_channels_sensitivities.clear();

		for (int i = 0; i < n_channels; i++) {			
			QDoubleSpinBox * spinbox = new QDoubleSpinBox();
			spinbox->setMinimum(-1000.00);
			spinbox->setMaximum(1000.00);
			spinbox->setValue(100.00);
			spinbox->setFixedWidth(100);
			vib_channels_sensitivities.push_back(spinbox);
			signals_config_dialog_ui.channel_sensitivities_layout->addRow("Channel " + QString::number(i + 1), vib_channels_sensitivities.at(i));
		}		
		ui.signals_config_button->setEnabled(true);
	}
	else if (adq_file_path.endsWith(".txt") || adq_file_path.endsWith(".TXT")) {
		QProgressBar * progressBar = new QProgressBar(this);
		progressBar->setRange(0, 0);
		progressBar->setTextVisible(false);

		QProgressDialog progress("Loading text file...", "Cancel", 0, 0, this);
		progress.setBar(progressBar);
		progress.setWindowModality(Qt::WindowModal);
		progress.open();
		progressBar->setValue(0);
		progress.setValue(0);
		QApplication::processEvents();

		ifstream txt_file(adq_file_path.toStdString());
		int line_counter = 0;
		string file_line;

		getline(txt_file, file_line);

		if (!file_line.empty()) {
			line_counter++;
			
			// Read .txt file column count;
			n_channels = QString::fromStdString(file_line).count("\t");

			if (n_channels < NUMBER_OF_MIN_ADQ_CHANNELS || n_channels > NUMBER_OF_MAX_ADQ_CHANNELS) {
				progress.cancel();
				QMessageBox::critical(this, "Error", "Number of vibrations channels must be between " + QString::number(NUMBER_OF_MIN_ADQ_CHANNELS) + " and " + QString::number(NUMBER_OF_MAX_ADQ_CHANNELS) + "!");				
				return;
			}

			// Free signals_data.
			for (int i = 0; i < signals_data->size(); i++) {				
				delete signals_data->at(i);
			}

			signals_data->clear();

			// Init signals data.
			for (int j = 0; j < n_channels; j++) {
				signals_data->push_back(new QVector<float>());
			}

			QRegExp rx("(\\d+\\.\\d+)");			
			QStringList list;
			int pos = 0;

			while ((pos = rx.indexIn(QString::fromStdString(file_line), pos)) != -1) {
				list << rx.cap(1);
				pos += rx.matchedLength();
			}

			for (int i = 0; i < list.size(); i++) {
				//qDebug() << " Val " << QString::number(i + 1) << " " << QString::number(list.at(i).toFloat());
				signals_data->at(i)->push_back(list.at(i).toFloat());
			}

			while (std::getline(txt_file, file_line)) {
				// Process str
				line_counter++;

				QRegExp rx("(\\d+\\.\\d+)");
				QStringList list;
				int pos = 0;

				while ((pos = rx.indexIn(QString::fromStdString(file_line), pos)) != -1) {
					list << rx.cap(1);
					pos += rx.matchedLength();
				}

				for (int i = 0; i < list.size(); i++) {
					//qDebug() << " Val " << QString::number(i + 1) << " " << QString::number(list.at(i).toFloat());
					signals_data->at(i)->push_back(list.at(i).toFloat());
				}

				QApplication::processEvents();
			}

			// Count .txt file rows and divide byt ADQ_SAMPLES_PER_SECOND. Must be int number, otherwise, return and show error.
			n_seconds = (int)(line_counter / ADQ_SAMPLES_PER_SECOND);
			
			if (n_seconds < NUMBER_OF_MIN_ADQ_SECONDS || n_seconds > NUMBER_OF_MAX_ADQ_SECONDS) {
				progress.cancel();
				QMessageBox::critical(this, "Error", "Number of seconds of vibration signals must be between " + QString::number(NUMBER_OF_MIN_ADQ_SECONDS) + " and " + QString::number(NUMBER_OF_MAX_ADQ_SECONDS) + "!");				
				return;
			}

			progress.cancel();
			QMessageBox::information(this, "Text file successfully processed", "Text file successfully processed for " + QString::number(n_channels) + " channels and " + QString::number(n_seconds) +" seconds!");	

			clearLayout(signals_config_dialog_ui.channel_sensitivities_layout);
			vib_channels_sensitivities.clear();

			for (int i = 0; i < n_channels; i++) {
				QDoubleSpinBox * spinbox = new QDoubleSpinBox();
				spinbox->setMinimum(-1000.00);
				spinbox->setMaximum(1000.00);
				spinbox->setValue(100.00);
				spinbox->setFixedWidth(100);
				vib_channels_sensitivities.push_back(spinbox);
				signals_config_dialog_ui.channel_sensitivities_layout->addRow("Channel " + QString::number(i + 1), vib_channels_sensitivities.at(i));
			}
			ui.signals_config_button->setEnabled(true);
		} else {
			progress.cancel();
			QMessageBox::warning(this, "Warning", "First line of .txt file empty!");			
			return;
		}				
	}
}

void Datafuz::showSignalsConfig() {		
	signals_config_dialog->show();	
}

void Datafuz::scaleData(QVector<QVector<QVector<int>>> *total_bytes, QVector<QVector<QVector<float>>> *scaled_bytes) {	
	unsigned short n_seconds = total_bytes->size();
	unsigned short n_channels = total_bytes->at(0).size();

	unsigned int step_counter = 0;
	unsigned int step_max = n_seconds*n_channels*ADQ_SAMPLES_PER_SECOND;

	QProgressDialog *scaling_progress = new QProgressDialog("Escalando datos...", "Cancelar", 0, step_max, this);
	scaling_progress->setWindowModality(Qt::WindowModal);
	scaling_progress->setWindowTitle("Escalando...");

	scaling_progress->setValue(0);

	for (unsigned short k = 0; k<n_seconds; k++) {
		QVector<QVector<float>> *second_bytes = new QVector<QVector<float>>();
		for (unsigned char j = 0; j<n_channels; j++) {
			QVector<float> *channel_bytes = new QVector<float>();
			for (unsigned int i = 0; i<ADQ_SAMPLES_PER_SECOND; i++) {
				float valor_escalado = (float)(total_bytes->at(k).at(j).at(i)*val_volt_max) / ((float)val_hex_pos_max);

				if (valor_escalado >= val_zero_volt) {
					valor_escalado += -val_volt_max;
				}

				channel_bytes->push_back(valor_escalado);

				scaling_progress->setValue(step_counter++);

				if (scaling_progress->wasCanceled())
					break;
			}
			second_bytes->push_back(*channel_bytes);
		}
		scaled_bytes->push_back(*second_bytes);
	}
	scaling_progress->setValue(step_max);
}

//void clearWidgets(QLayout *layout) {
//	QLayoutItem *item;
//	if (layout != NULL) {
//		while ((item = layout->takeAt(0))) {
//			if (item != NULL && item->widget()) {
//				layout->removeWidget(item->widget());
//				delete item;
//			}
//		}
//	}
//}

//void clearLayout(QLayout *layout) {
//	QLayoutItem *item;
//	if (layout != NULL) {
//		while ((item = layout->takeAt(0)) != 0) {
//			if (item->layout()) {
//				clearLayout(item->layout());
//				layout->removeItem(item);
//				continue;
//			}
//			if (item != NULL && item->widget()) {
//				delete item->widget();
//				continue;
//			}
//			if (item != NULL && item->spacerItem()) {
//				delete item->spacerItem();
//				continue;
//			}
//
//			if (item != NULL) {
//				delete item;
//			}
//		}
//	}
//}
//
//void clearLayouts(QLayout *layout) {
//	QLayoutItem *item;
//	while ((item = layout->takeAt(0))) {
//		clearLayout(item->layout());
//
//		if (item != NULL) {
//			delete item;
//		}
//	}
//}

void Datafuz::updateVibrationsOpts() {
	//QMessageBox::information(this, "Vibrations options updated...", "Updating vibrations options...");	
	QProgressBar * progressBar = new QProgressBar(this);
	progressBar->setRange(0, 0);
	progressBar->setTextVisible(false);

	QProgressDialog progress("Updating plots...", "Cancel", 0, 0, this);
	progress.setBar(progressBar);
	progress.setWindowModality(Qt::WindowModal);
	progress.open();

	progressBar->setValue(0);
	progress.setValue(0);
	QApplication::processEvents();			

	ui.sel_vib_channels_grbox->setVisible(true);

	int vib_channels_n = ui.vib_channel_n->currentIndex() + 1;

	// Remove elements from layouts
	for (int i = 0; i < MAX_VIB_CHANNELS_N; i++) {
		vib_channels_checks.at(i)->setVisible(false);
		vib_channels_combos.at(i)->setVisible(false);
		vib_channels_edits.at(i)->setVisible(false);
	}

	//delete ui.layout_vib_checks;
	//ui.layout_vib_checks = new QHBoxLayout(ui.sel_vib_channels_grbox);
	clearItems(ui.layout_vib_checks);	

	for (int i = 0; i < vib_channels_n; i++) {
		ui.layout_vib_checks->addSpacerItem(vib_channels_spaces.at(i));
		vib_channels_checks.at(i)->setVisible(true);
		ui.layout_vib_checks->addWidget(vib_channels_checks.at(i));
		vib_channels_combos.at(i)->setVisible(true);
		ui.layout_vib_checks->addWidget(vib_channels_combos.at(i));
		vib_channels_edits.at(i)->setVisible(true);
		ui.layout_vib_checks->addWidget(vib_channels_edits.at(i));
	}

	QSpacerItem * end_spacer = new QSpacerItem(20, 30, QSizePolicy::Expanding, QSizePolicy::Fixed);	
	ui.layout_vib_checks->addSpacerItem(end_spacer);
			
	clearLayout(ui.vib_time_plots_layout);
	clearLayout(ui.vib_freq_plots_layout);
		
	for (int j = 0; j < vib_channels_n; j++) {
		if (vib_channels_checks.at(j)->isChecked()) {
			DatafuzPlot * timePlot = vib_channels_time_plots[j];
			
			// generate some points of data (y0 for first, y1 for second graph):
			QVector<double> x(250), y0(250), y1(250);
			for (int i = 0; i<250; ++i)
			{
				x[i] = i;
				y0[i] = qExp(-i / 150.0)*qCos(i / 10.0); // exponentially decaying cosine
				y1[i] = qExp(-i / 150.0);              // exponential envelope
			}
					
			//delete timePlot;
			vib_channels_time_plots[j] = new DatafuzPlot();
			timePlot = vib_channels_time_plots[j];
			if (ui.show_x_vals_check->isChecked()) {
				timePlot->showXValue();
			} else {
				timePlot->hideXValue();
			}

			if (ui.show_y_vals_check->isChecked()) {
				timePlot->showYValue();
			}
			else {
				timePlot->hideYValue();
			}

			plotTime(timePlot, vib_channels_edits.at(j)->text(), x, y0, 350);

			if (ui.plot_freq_check->isChecked()) {
				DatafuzPlot *freqPlot = vib_channels_freq_plots[j];
				
				// generate ideal sinc curve data and some randomly perturbed data for scatter plot:
				QVector<double> x0(250), y0(250);
				QVector<double> yConfUpper(250), yConfLower(250);
				for (int i = 0; i<250; ++i)
				{
					x0[i] = (i / 249.0 - 0.5) * 30 + 0.01; // by adding a small offset we make sure not do divide by zero in next code line
					y0[i] = qSin(x0[i]) / x0[i]; // sinc function
					yConfUpper[i] = y0[i] + 0.15;
					yConfLower[i] = y0[i] - 0.15;
					x0[i] *= 1000;
				}
				QVector<double> x1(50), y1(50), y1err(50);
				for (int i = 0; i<50; ++i)
				{
					// generate a gaussian distributed random number:
					double tmp1 = rand() / (double)RAND_MAX;
					double tmp2 = rand() / (double)RAND_MAX;
					double r = qSqrt(-2 * qLn(tmp1))*qCos(2 * M_PI*tmp2); // box-muller transform for gaussian distribution
					// set y1 to value of y0 plus a random gaussian pertubation:
					x1[i] = (i / 50.0 - 0.5) * 30 + 0.25;
					y1[i] = qSin(x1[i]) / x1[i] + r*0.15;
					x1[i] *= 1000;
					y1err[i] = 0.15;
				}
				
				vib_channels_freq_plots[j] = new DatafuzPlot();
				freqPlot = vib_channels_freq_plots[j];
				if (ui.show_x_vals_check->isChecked()) {
					freqPlot->showXValue();
				}
				else {
					freqPlot->hideXValue();
				}

				if (ui.show_y_vals_check->isChecked()) {
					freqPlot->showYValue();
				}
				else {
					freqPlot->hideYValue();
				}

				plotFreq(freqPlot, vib_channels_edits.at(j)->text(), x1, y1, 350);

				ui.vib_time_plots_layout->addWidget(timePlot);
				ui.vib_freq_plots_layout->addWidget(freqPlot);				
			} else {
				ui.vib_time_plots_layout->addWidget(timePlot);
			}
		}		
	}
	progress.cancel();
}

void Datafuz::updateFreqOpts(int state) {
	if (state == Qt::Unchecked) {
		ui.fs_val->setEnabled(false);
		ui.fs_label->setEnabled(false);
	}
	else if (state == Qt::Checked) {
		ui.fs_val->setEnabled(true);
		ui.fs_label->setEnabled(true);
	}
}

void Datafuz::applyVibConfig(void) {
	ui.signals_tab_widget->setEnabled(true);
	signals_config_dialog->hide();
}

void Datafuz::defaultVibConfig(void) {
	ui.signals_tab_widget->setEnabled(true);
	signals_config_dialog->hide();
}