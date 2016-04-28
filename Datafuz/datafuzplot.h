#ifndef DATAFUZPLOT_H
#define DATAFUZPLOT_H

#include <qcustomplot.h>
#include <QObject>

class DatafuzPlot : public QCustomPlot
{
	Q_OBJECT

public:
	DatafuzPlot();
	~DatafuzPlot();	

	void showXValue() {
		this->show_x_value = true;
	}

	void hideXValue() {
		this->show_x_value = false;
	}

	void showYValue() {
		this->show_y_value = true;
	}

	void hideYValue() {
		this->show_y_value = false;
	}

private:
	QPoint cursor_pos;
	bool show_x_value;
	bool show_y_value;

	void mouseMoveEvent(QMouseEvent * event);
	void paintEvent(QPaintEvent *event);	
	void paintCoordinate();
};

#endif // DATAFUZPLOT_H
