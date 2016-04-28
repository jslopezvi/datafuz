#include "datafuz.h"

#include <QDebug>
#include <QMessageBox>
#include <QProgressBar>
#include <QFileDialog>
#include <QVector>
#include <QDateTime>
#include <iostream>
#include <fstream>

#include <stdlib.h>
#include <math.h>
#include <fftw3.h>

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

	ui.sel_vib_channels_grbox->setVisible(false);
	ui.layout_vib_checks->setAlignment(Qt::AlignCenter);	
	
	for (int i = 0; i < MAX_VIB_CHANNELS_N; i++) {
		QCheckBox * checkbox = new QCheckBox("Vib"+QString::number(i + 1));
		checkbox->setChecked(true);
		vib_channels_checks.push_back(checkbox);

		QComboBox * combobox = new QComboBox();				
		combobox->setFixedWidth(30);
		vib_channels_combos.push_back(combobox);

		QSpacerItem * spacer = new QSpacerItem(20, 30, QSizePolicy::Expanding, QSizePolicy::Fixed);
		vib_channels_spaces.push_back(spacer);

		QLineEdit * line_edit = new QLineEdit("Channel " + QString::number(i + 1));
		line_edit->setFixedWidth(60);
		vib_channels_edits.push_back(line_edit);

		DatafuzPlot *timePlot = new DatafuzPlot();
		timePlot->hideYValue();		
		vib_channels_time_plots.push_back(timePlot);

		DatafuzPlot *freqPlot = new DatafuzPlot();
		freqPlot->hideYValue();		
		vib_channels_freq_plots.push_back(freqPlot);
	}

	val_hex_pos_max = 16777216.0;
	val_volt_max = 5.0;
	val_zero_volt = 2.5;

	signals_time = new QVector<double>();
	signals_data = new QVector<QVector<double>*>();

	spectrums_time = new QVector<double>();
	spectrums_data = new QVector<QVector<double>*>();

	ui.signals_config_button->setEnabled(false);
	ui.signals_tab_widget->setEnabled(false);
}

Datafuz::~Datafuz()
{

}

void Datafuz::plotTime(DatafuzPlot * timePlot, const QString title, QVector<double> x, QVector<double> y, int fixed_height, QString x_legend, QString y_legend, double y_scaling) {
	timePlot->clearGraphs();
	timePlot->clearItems();
	timePlot->clearPlottables();

	timePlot->setNoAntialiasingOnDrag(false);

	//timePlot->update();
	//timePlot->setWindowTitle(title + " - Time");

	//	timePlot->plotLayout()->clear();

	// add two new graphs and set their look:
	timePlot->addGraph();
	timePlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph	

	// configure right and top axis to show ticks but no labels:
	// (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
	timePlot->xAxis->setVisible(true);
	timePlot->xAxis->setTickLabels(true);
	timePlot->yAxis->setVisible(true);
	timePlot->yAxis->setTickLabels(true);

	timePlot->xAxis->setLabel(x_legend);
	timePlot->yAxis->setLabel(y_legend);

	// Note: we could have also just called timePlot->rescaleAxes(); instead
	// Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
	timePlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

	timePlot->setFixedHeight(fixed_height);

	// Update sensivilities
	for (int i = 0; i < y.size(); i++) {		
		y[i] = (float)(y[i]*val_volt_max) / ((float)val_hex_pos_max);

		if (y[i] >= val_zero_volt) {
			y[i] += -val_volt_max;
		}

		y[i] = y[i] / y_scaling;
	}

	// pass data points to graphs:
	timePlot->graph(0)->setData(x, y);

	// let the ranges scale themselves so graph 0 fits perfectly in the visible area:
	timePlot->graph(0)->rescaleAxes();
	if (x.size() >= 3000) {
		timePlot->xAxis->setRange(0, x[3000]);
	}	

	/*QCPLayoutElement * element = timePlot->plotLayout()->element(0, 0);

	if (element != NULL) {
	timePlot->plotLayout()->remove(element);
	}*/

	timePlot->plotLayout()->insertRow(0);
	timePlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(timePlot, title + " - Time"));
	timePlot->replot();
}

QVector<double> computeFourier(double fs, QVector<double> y) {
	double *out_fft, *in_fft;
	fftw_plan p;

	//Allocate memory for DFT
	out_fft = (double*)fftw_malloc(sizeof(double) * fs);
	in_fft = (double*)fftw_malloc(sizeof(double) * fs);
	QVector<double> out_ffty;

	//Set DFT plan (parameters)
	p = fftw_plan_r2r_1d(fs, in_fft, out_fft, FFTW_R2HC, FFTW_ESTIMATE);

	//Copy Input data to DFT data vector type
	for (int i = 0; i<y.size(); i++)
	{
		in_fft[i] = y[i];
	}

	//Execute DFT
	fftw_execute(p);

	//Post-process (square and select positive side)
	for (int i = 1; i < ((fs / 2)); i++)
	{		
		out_ffty.push_back(out_fft[i] * out_fft[i]);
	}
	
	return out_ffty;
}

void Datafuz::plotFreq(DatafuzPlot * freqPlot, const QString title, QVector<double> x, QVector<double> y, int fixed_height, QString x_legend, QString y_legend) {
	freqPlot->clearGraphs();
	freqPlot->clearItems();
	freqPlot->clearPlottables();
	//freqPlot->setWindowTitle(title + " - Freq");
	
	freqPlot->setNoAntialiasingOnDrag(false);
	//freqPlot->plotLayout()->clear();

	// create empty bar chart objects:
	QCPBars *freq = new QCPBars(freqPlot->xAxis, freqPlot->yAxis);
	freqPlot->addPlottable(freq);

	freqPlot->xAxis->setVisible(true);
	freqPlot->yAxis->setVisible(true);

	freqPlot->xAxis->setLabel(x_legend);
	freqPlot->yAxis->setLabel(y_legend);
	// set names and colors:
	QPen pen;
	pen.setWidthF(1);
	freq->setName("Freq");
	pen.setColor(QColor(255, 131, 0));
	freq->setPen(pen);
	freq->setBrush(QColor(255, 131, 0, 50));

	freq->setData(x, y);
	freqPlot->rescaleAxes();

	if (x.size() >= 1000) {
		freqPlot->xAxis->setRange(0, 1000);
	}	

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
	QString adq_file_path = QFileDialog::getOpenFileName(this, tr("Load signals file..."), ".", tr("Text files (*.txt, *.TXT);; Binary files (*.bin, *.BIN)"));

	if (adq_file_path == NULL) {
		QMessageBox::warning(this, "Warning", "You must select a path for signals file!");
		return;
	}

	ui.signals_file_path->setText(adq_file_path);

	if (adq_file_path.endsWith(".bin") || adq_file_path.endsWith(".BIN")) {
		bool ok;
		n_channels = QInputDialog::getInt(this, "Number of vibration channels", "Please provide number of vibration channels in signals file:", 7, NUMBER_OF_MIN_ADQ_CHANNELS, NUMBER_OF_MAX_ADQ_CHANNELS, 1, &ok);

		if (!ok)
			return;

		if (n_channels < NUMBER_OF_MIN_ADQ_CHANNELS || n_channels > NUMBER_OF_MAX_ADQ_CHANNELS) {
			QMessageBox::critical(this, "Error", "Number of vibrations channels must be between " + QString::number(NUMBER_OF_MIN_ADQ_CHANNELS) + " and " + QString::number(NUMBER_OF_MAX_ADQ_CHANNELS) +"!");
			return;
		}

		ui.vib_channel_n->clear();

		QStringList channels_list = QStringList();

		for (int i = 0; i < n_channels; i++) {
			channels_list << QString::number(i + 1);
		}

		ui.vib_channel_n->addItems(channels_list);

		n_seconds = QInputDialog::getInt(this, "Number of seconds of vibration signals", "Please provide number of seconds of vibrations signals", 1, NUMBER_OF_MIN_ADQ_SECONDS, NUMBER_OF_MAX_ADQ_SECONDS, 1, &ok);

		if (!ok)
			return;

		if (n_seconds < NUMBER_OF_MIN_ADQ_SECONDS || n_seconds > NUMBER_OF_MAX_ADQ_SECONDS) {
			QMessageBox::critical(this, "Error", "Number of seconds of vibration signals must be between " + QString::number(NUMBER_OF_MIN_ADQ_SECONDS) + " and " + QString::number(NUMBER_OF_MAX_ADQ_SECONDS) + "!");
			return;
		}

		signals_time->clear();
		signals_data->clear();

		spectrums_time->clear();
		spectrums_data->clear();

		QString adq_file_txt_path = adq_file_path.section(".", 0, 0) + ".txt";

		ifstream infile;
		infile.open(adq_file_path.toStdString(), ios::binary | ios::in);

		ofstream outfile;
		outfile.open(adq_file_txt_path.toStdString(), ios::out | ios::trunc);

		QVector<QVector<QVector<int>>> *total_bytes = new QVector<QVector<QVector<int>>>();

		unsigned char buffer[ADQ_BYTES_PER_SAMPLE];		

		unsigned int step_counter = 0;
		unsigned int step_max = n_seconds*n_channels*ADQ_SAMPLES_PER_SECOND;

		QProgressDialog *reading_progress = new QProgressDialog("Reading signals file...", "Cancel", 0, step_max, this);
		reading_progress->setWindowModality(Qt::WindowModal);
		reading_progress->setWindowTitle("Reading...");

		reading_progress->setValue(0);

		// Read number of seconds
		//infile.read((char *)&buffer, sizeof(buffer));

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
		
		step_counter = 0;

		QProgressDialog *saving_progress = new QProgressDialog("Saving processed data...", "Cancel", 0, step_max, this);
		saving_progress->setWindowModality(Qt::WindowModal);
		saving_progress->setWindowTitle("Saving...");

		saving_progress->setValue(0);

		char printf_buffer[255] = { 0 };

		// Free signals_data.
		for (int i = 0; i < signals_data->size(); i++) {
			delete signals_data->at(i);
		}

		// Free spectrums_data.
		for (int i = 0; i < spectrums_data->size(); i++) {
			delete spectrums_data->at(i);
		}

		signals_time->clear();
		signals_data->clear();

		spectrums_time->clear();
		spectrums_data->clear();
		
		// Init signals data.
		for (int j = 0; j < n_channels; j++) {
			signals_data->push_back(new QVector<double>());
		}

		// Init spectrums data.
		for (int j = 0; j < n_channels; j++) {
			spectrums_data->push_back(new QVector<double>());
		}

		vib_fs = signals_config_dialog_ui.fs_val->value();
		double fs_step = 1 / vib_fs;

		for (int i = 0; i < n_seconds*ADQ_SAMPLES_PER_SECOND; i++) {
			signals_time->push_back(i*fs_step);
		}

		for (unsigned short k = 0; k<n_seconds; k++) {
			for (int i = 0; i<ADQ_SAMPLES_PER_SECOND; i++) {
				for (int j = 0; j<n_channels; j++) {					
					float value = total_bytes->at(k).at(j).at(i);
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

		QMessageBox::information(this, "Information", "Signals file successfully processed!");

		clearLayout(signals_config_dialog_ui.channel_sensitivities_layout);
		vib_channels_sensitivities.clear();

		for (int i = 0; i < n_channels; i++) {			
			QDoubleSpinBox * spinbox = new QDoubleSpinBox();
			spinbox->setMinimum(0.1);
			spinbox->setMaximum(2000);
			spinbox->setValue(100.00);
			spinbox->setFixedWidth(100);
			vib_channels_sensitivities.push_back(spinbox);
			signals_config_dialog_ui.channel_sensitivities_layout->addRow("Channel " + QString::number(i + 1), vib_channels_sensitivities.at(i));
		}

		spectrums_time->clear();

		for (int i = 1; i < ((vib_fs / 2)); i++) {
			spectrums_time->push_back(i);
		}

		for (int i = 0; i < n_channels; i++) {
			QVector<double> spectrum = computeFourier(vib_fs, *signals_data->at(i));

			spectrums_data->at(i)->clear();

			for (int m = 0; m < spectrum.size(); m++) {
				spectrums_data->at(i)->push_back(spectrum.at(m));
			}
		}

		clearLayout(ui.layout_vib_checks);
		vib_channels_checks.clear();
		vib_channels_combos.clear();
		vib_channels_spaces.clear();
		vib_channels_edits.clear();

		for (int i = 0; i < n_channels; i++) {
			QCheckBox * checkbox = new QCheckBox("Vib" + QString::number(i + 1));
			checkbox->setChecked(true);
			vib_channels_checks.push_back(checkbox);

			QComboBox * combobox = new QComboBox();
			combobox->setFixedWidth(30);
			vib_channels_combos.push_back(combobox);

			QSpacerItem * spacer = new QSpacerItem(20, 30, QSizePolicy::Expanding, QSizePolicy::Fixed);
			vib_channels_spaces.push_back(spacer);

			QLineEdit * line_edit = new QLineEdit("Channel " + QString::number(i + 1));
			line_edit->setFixedWidth(60);
			vib_channels_edits.push_back(line_edit);			
		}

		for (int i = 0; i < n_channels; i++) {
			vib_channels_combos.at(i)->clear();
			vib_channels_combos.at(i)->addItems(channels_list);
			vib_channels_combos.at(i)->setCurrentIndex(i);
		}

		clearLayout(ui.vib_time_plots_layout);
		clearLayout(ui.vib_freq_plots_layout);

		ui.signals_config_button->setEnabled(true);
		ui.sel_vib_channels_grbox->setVisible(false);
		ui.layout_vib_checks->setAlignment(Qt::AlignCenter);
		
		ui.signals_tab_widget->setEnabled(false);
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

			ui.vib_channel_n->clear();

			QStringList channels_list = QStringList();

			for (int i = 0; i < n_channels; i++) {
				channels_list << QString::number(i + 1);
			}

			ui.vib_channel_n->addItems(channels_list);

			// Free signals_data.
			for (int i = 0; i < signals_data->size(); i++) {				
				delete signals_data->at(i);
			}

			signals_time->clear();
			signals_data->clear();

			spectrums_time->clear();
			spectrums_data->clear();

			// Init signals data.
			for (int j = 0; j < n_channels; j++) {
				signals_data->push_back(new QVector<double>());
			}

			// Init spectrums data.
			for (int j = 0; j < n_channels; j++) {
				spectrums_data->push_back(new QVector<double>());
			}

			QRegExp rx("(-?\\d+\\.\\d+)");			
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

				QRegExp rx("(-?\\d+\\.\\d+)");
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

			vib_fs = signals_config_dialog_ui.fs_val->value();
			double fs_step = 1 / vib_fs;

			for (int i = 0; i < n_seconds*ADQ_SAMPLES_PER_SECOND; i++) {
				signals_time->push_back(i*fs_step);
			}			

			progress.cancel();
			QMessageBox::information(this, "Text file successfully processed", "Text file successfully processed for " + QString::number(n_channels) + " channels and " + QString::number(n_seconds) +" seconds!");	

			clearLayout(signals_config_dialog_ui.channel_sensitivities_layout);
			vib_channels_sensitivities.clear();

			for (int i = 0; i < n_channels; i++) {
				QDoubleSpinBox * spinbox = new QDoubleSpinBox();
				spinbox->setMinimum(0.1);
				spinbox->setMaximum(2000);
				spinbox->setValue(100.00);
				spinbox->setFixedWidth(100);
				vib_channels_sensitivities.push_back(spinbox);
				signals_config_dialog_ui.channel_sensitivities_layout->addRow("Channel " + QString::number(i + 1), vib_channels_sensitivities.at(i));
			}						

			spectrums_time->clear();

			for (int i = 1; i < ((vib_fs / 2)); i++) {
				spectrums_time->push_back(i);
			}

			for (int i = 0; i < n_channels; i++) {
				QVector<double> spectrum = computeFourier(vib_fs, *signals_data->at(i));

				spectrums_data->at(i)->clear();

				for (int m = 0; m < spectrum.size(); m++) {
					spectrums_data->at(i)->push_back(spectrum.at(m));
				}
			}

			clearLayout(ui.layout_vib_checks);
			vib_channels_checks.clear();
			vib_channels_combos.clear();
			vib_channels_spaces.clear();
			vib_channels_edits.clear();

			for (int i = 0; i < n_channels; i++) {
				QCheckBox * checkbox = new QCheckBox("Vib" + QString::number(i + 1));
				checkbox->setChecked(true);
				vib_channels_checks.push_back(checkbox);

				QComboBox * combobox = new QComboBox();
				combobox->setFixedWidth(30);
				vib_channels_combos.push_back(combobox);

				QSpacerItem * spacer = new QSpacerItem(20, 30, QSizePolicy::Expanding, QSizePolicy::Fixed);
				vib_channels_spaces.push_back(spacer);

				QLineEdit * line_edit = new QLineEdit("Channel " + QString::number(i + 1));
				line_edit->setFixedWidth(60);
				vib_channels_edits.push_back(line_edit);
			}

			for (int i = 0; i < n_channels; i++) {
				vib_channels_combos.at(i)->clear();
				vib_channels_combos.at(i)->addItems(channels_list);
				vib_channels_combos.at(i)->setCurrentIndex(i);
			}

			clearLayout(ui.vib_time_plots_layout);
			clearLayout(ui.vib_freq_plots_layout);

			ui.signals_config_button->setEnabled(true);
			ui.signals_config_button->setEnabled(true);
			ui.sel_vib_channels_grbox->setVisible(false);
			ui.layout_vib_checks->setAlignment(Qt::AlignCenter);
			
			ui.signals_tab_widget->setEnabled(false);
		} else {
			progress.cancel();
			QMessageBox::warning(this, "Warning", "First line of .txt file empty!");			
			return;
		}				
	}
}

void Datafuz::showSignalsConfig() {		
	signals_config_dialog->show();		
	signals_config_dialog->move(QPoint(signals_config_dialog->pos().x(), signals_config_dialog->pos().y() - n_channels * 10));
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
	for (int i = 0; i < n_channels; i++) {
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

	vib_fs = signals_config_dialog_ui.fs_val->value();

	spectrums_time->clear();

	for (int i = 1; i < ((vib_fs / 2)); i++) {
		spectrums_time->push_back(i);
	}
		
	for (int j = 0; j < vib_channels_n; j++) {
		if (vib_channels_checks.at(j)->isChecked()) {
			DatafuzPlot * timePlot = vib_channels_time_plots[j];
						
			int currentIndex = vib_channels_combos.at(j)->currentIndex();

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
			
			plotTime(timePlot, vib_channels_edits.at(j)->text(), *signals_time, *signals_data->at(currentIndex), 350, "Time (s)", "Amplitude (g)", vib_channels_sensitivities.at(currentIndex)->value()/1000);

			if (ui.plot_freq_check->isChecked()) {
				DatafuzPlot *freqPlot = vib_channels_freq_plots[j];								
				
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

				plotFreq(freqPlot, vib_channels_edits.at(j)->text(), *spectrums_time, *spectrums_data->at(currentIndex), 350, "Freq (Hz)", "");

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
}

void Datafuz::applyVibConfig(void) {
	val_hex_pos_max = signals_config_dialog_ui.maxADCPositionSpinBox->value();
	val_volt_max = signals_config_dialog_ui.maxVoltDoubleSpinBox->value();
	val_zero_volt = signals_config_dialog_ui.minVoltDoubleSpinBox->value();	

	ui.signals_tab_widget->setEnabled(true);
	
	signals_config_dialog->hide();
	signals_config_dialog->move(QPoint(signals_config_dialog->pos().x(), signals_config_dialog->pos().y() + n_channels * 10));
}