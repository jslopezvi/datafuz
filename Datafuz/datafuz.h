#ifndef DATAFUZ_H
#define DATAFUZ_H

#include <QtWidgets/QMainWindow>
#include "ui_datafuz.h"
#include "ui_signals_config.h"
#include "datafuzplot.h"

class Datafuz : public QMainWindow
{
	Q_OBJECT

public:
	Datafuz(QWidget *parent = 0);
	~Datafuz();

private:
	Ui::DatafuzClass ui;

	Ui::Dialog signals_config_dialog_ui;
	QDialog *signals_config_dialog;

	float val_hex_pos_max;
	float val_volt_max;
	float val_zero_volt;

	int n_channels;
	int n_seconds;
	
	QVector<QVector<float>*> *signals_data;

	QVector<QCheckBox*> vib_channels_checks;
	QVector<QComboBox*> vib_channels_combos;
	QVector<QSpacerItem*> vib_channels_spaces;
	QVector<QLineEdit*> vib_channels_edits;

	QVector<DatafuzPlot*> vib_channels_time_plots;
	QVector<DatafuzPlot*> vib_channels_freq_plots;

	QVector<QDoubleSpinBox*> vib_channels_sensitivities;

	void scaleData(QVector<QVector<QVector<int>>> *total_bytes, QVector<QVector<QVector<float>>> *scaled_bytes);
	void plotTime(DatafuzPlot * timePlot, const QString title, QVector<double> x, QVector<double> y, int fixed_height);
	void plotFreq(DatafuzPlot * freqPlot, const QString title, QVector<double> x, QVector<double> y, int fixed_height);

private slots:
	void loadSignalsFile(void);
	void showSignalsConfig(void);
	void updateVibrationsOpts(void);
	void updateFreqOpts(int);

	void applyVibConfig(void);
	void defaultVibConfig(void);
};

#endif // DATAFUZ_H
