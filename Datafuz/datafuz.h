#ifndef DATAFUZ_H
#define DATAFUZ_H

#include <QtWidgets/QMainWindow>
#include "ui_datafuz.h"

class Datafuz : public QMainWindow
{
	Q_OBJECT

public:
	Datafuz(QWidget *parent = 0);
	~Datafuz();

private:
	Ui::DatafuzClass ui;

private slots:
	void updateVibrationsOpts(void);
};

#endif // DATAFUZ_H
