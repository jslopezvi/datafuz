#ifndef DATAFUZ_H
#define DATAFUZ_H

#include <QtWidgets/QMainWindow>
#include "ui_datafuz.h"
#include "qcustomplot.h"

class Datafuz : public QMainWindow
{
	Q_OBJECT

public:
	Datafuz(QWidget *parent = 0);
	~Datafuz();

private:
	Ui::DatafuzClass ui;	
	
	QVector<QCheckBox*> vib_channels_checks;
	QVector<QSpacerItem*> vib_channels_spaces;
	QVector<QLineEdit*> vib_channels_edits;

	QVector<QCustomPlot*> vib_channels_time_plots;
	QVector<QCustomPlot*> vib_channels_freq_plots;	

	void plotTime(QCustomPlot * timePlot, const QString title, QVector<double> x, QVector<double> y, int fixed_height);
	void plotFreq(QCustomPlot * freqPlot, const QString title, QVector<double> x, QVector<double> y, int fixed_height);

private slots:
	void updateVibrationsOpts(void);
};

#endif // DATAFUZ_H
