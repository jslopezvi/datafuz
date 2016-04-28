#include "datafuzplot.h"

DatafuzPlot::DatafuzPlot()
	: QCustomPlot()
{

}

DatafuzPlot::~DatafuzPlot()
{

}

void DatafuzPlot::mouseMoveEvent(QMouseEvent * event)
{
	if (show_x_value || show_y_value) {
		cursor_pos = event->pos();
		replot();
	}	

	QCustomPlot::mouseMoveEvent(event);
}

void DatafuzPlot::paintEvent(QPaintEvent *event)
{
	QCustomPlot::paintEvent(event);

	if (show_x_value || show_y_value) {
		paintCoordinate();
	}
}

void DatafuzPlot::paintCoordinate() 
{		
	QPainter painter(this);
	double x = xAxis->pixelToCoord(cursor_pos.x());
	double y = yAxis->pixelToCoord(cursor_pos.y());

	if (show_x_value & show_y_value) {
		painter.drawText(cursor_pos, "X = " + QString::number(x)+" , Y = "+QString::number(y));
	}
	else if (show_x_value) {
		painter.drawText(cursor_pos, "X = " + QString::number(x));
	}
	else if (show_y_value) {
		painter.drawText(cursor_pos, "Y = " + QString::number(y));
	}	
}
