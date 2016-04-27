#include "datafuz.h"

#include "qcustomplot.h"

#include <QMessageBox>

#define MAX_VIB_CHANNELS_N 7

void Datafuz::plotTime(QCustomPlot * timePlot, const QString title, QVector<double> x, QVector<double> y, int fixed_height) {
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

void Datafuz::plotFreq(QCustomPlot * freqPlot, const QString title, QVector<double> x, QVector<double> y, int fixed_height) {
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

Datafuz::Datafuz(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);	
	connect(ui.update_vibrations_opts, SIGNAL(clicked()), this, SLOT(updateVibrationsOpts()));

	ui.sel_vib_channels_grbox->setVisible(false);
	ui.layout_vib_checks->setAlignment(Qt::AlignCenter);	

	for (int i = 0; i < MAX_VIB_CHANNELS_N; i++) {
		QCheckBox * checkbox = new QCheckBox(QString::number(i + 1));
		checkbox->setChecked(true);
		vib_channels_checks.push_back(checkbox);

		QSpacerItem * spacer = new QSpacerItem(20, 30, QSizePolicy::Expanding, QSizePolicy::Fixed);
		vib_channels_spaces.push_back(spacer);

		QLineEdit * line_edit = new QLineEdit("Channel " + QString::number(i + 1));
		line_edit->setFixedWidth(60);
		vib_channels_edits.push_back(line_edit);

		QCustomPlot *timePlot = new QCustomPlot();
		plotTime(timePlot, line_edit->text(), QVector<double>(), QVector<double>(), 350);
		vib_channels_time_plots.push_back(timePlot);

		QCustomPlot *freqPlot = new QCustomPlot();
		plotFreq(freqPlot, line_edit->text(), QVector<double>(), QVector<double>(), 350);
		vib_channels_freq_plots.push_back(freqPlot);		
	}
}

Datafuz::~Datafuz()
{

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
	ui.sel_vib_channels_grbox->setVisible(true);

	int vib_channels_n = ui.vib_channel_n->currentIndex() + 1;

	// Remove elements from layouts
	for (int i = 0; i < MAX_VIB_CHANNELS_N; i++) {
		vib_channels_checks.at(i)->setVisible(false);
		vib_channels_edits.at(i)->setVisible(false);
	}

	//delete ui.layout_vib_checks;
	//ui.layout_vib_checks = new QHBoxLayout(ui.sel_vib_channels_grbox);
	clearItems(ui.layout_vib_checks);	

	for (int i = 0; i < vib_channels_n; i++) {
		ui.layout_vib_checks->addSpacerItem(vib_channels_spaces.at(i));
		vib_channels_checks.at(i)->setVisible(true);
		ui.layout_vib_checks->addWidget(vib_channels_checks.at(i));
		vib_channels_edits.at(i)->setVisible(true);
		ui.layout_vib_checks->addWidget(vib_channels_edits.at(i));
	}

	QSpacerItem * end_spacer = new QSpacerItem(20, 30, QSizePolicy::Expanding, QSizePolicy::Fixed);	
	ui.layout_vib_checks->addSpacerItem(end_spacer);
			
	clearLayout(ui.vib_time_plots_layout);
	clearLayout(ui.vib_freq_plots_layout);
		
	for (int j = 0; j < vib_channels_n; j++) {
		if (vib_channels_checks.at(j)->isChecked()) {
			QCustomPlot * timePlot = vib_channels_time_plots[j];
			
			// generate some points of data (y0 for first, y1 for second graph):
			QVector<double> x(250), y0(250), y1(250);
			for (int i = 0; i<250; ++i)
			{
				x[i] = i;
				y0[i] = qExp(-i / 150.0)*qCos(i / 10.0); // exponentially decaying cosine
				y1[i] = qExp(-i / 150.0);              // exponential envelope
			}
					
			//delete timePlot;
			vib_channels_time_plots[j] = new QCustomPlot();
			timePlot = vib_channels_time_plots[j];

			plotTime(timePlot, vib_channels_edits.at(j)->text(), x, y0, 350);

			if (ui.plot_freq_check->isChecked()) {
				QCustomPlot *freqPlot = vib_channels_freq_plots[j];
				
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
				
				vib_channels_freq_plots[j] = new QCustomPlot();
				freqPlot = vib_channels_freq_plots[j];

				plotFreq(freqPlot, vib_channels_edits.at(j)->text(), x1, y1, 350);

				ui.vib_time_plots_layout->addWidget(timePlot);
				ui.vib_freq_plots_layout->addWidget(freqPlot);				
			} else {
				ui.vib_time_plots_layout->addWidget(timePlot);
			}
		}		
	}
}
