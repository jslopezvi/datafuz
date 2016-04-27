/********************************************************************************
** Form generated from reading UI file 'datafuz.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATAFUZ_H
#define UI_DATAFUZ_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DatafuzClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label;
    QComboBox *vib_channel_n;
    QSpacerItem *horizontalSpacer;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QSpacerItem *horizontalSpacer_5;
    QCheckBox *plot_freq_check;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *update_vibrations_opts;
    QSpacerItem *horizontalSpacer_4;
    QGroupBox *sel_vib_channels_grbox;
    QHBoxLayout *horizontalLayout_9;
    QHBoxLayout *layout_vib_checks;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_7;
    QVBoxLayout *vib_time_plots_layout;
    QVBoxLayout *vib_freq_plots_layout;
    QWidget *tab_2;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_4;
    QWidget *tab_3;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_5;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *DatafuzClass)
    {
        if (DatafuzClass->objectName().isEmpty())
            DatafuzClass->setObjectName(QStringLiteral("DatafuzClass"));
        DatafuzClass->resize(691, 550);
        centralWidget = new QWidget(DatafuzClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout_2->addWidget(lineEdit);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_2->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        horizontalLayout_3 = new QHBoxLayout(tab);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);

        label = new QLabel(tab);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout_6->addWidget(label);

        vib_channel_n = new QComboBox(tab);
        vib_channel_n->setObjectName(QStringLiteral("vib_channel_n"));

        horizontalLayout_6->addWidget(vib_channel_n);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer);

        label_2 = new QLabel(tab);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_6->addWidget(label_2);

        lineEdit_2 = new QLineEdit(tab);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_6->addWidget(lineEdit_2);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);

        plot_freq_check = new QCheckBox(tab);
        plot_freq_check->setObjectName(QStringLiteral("plot_freq_check"));

        horizontalLayout_6->addWidget(plot_freq_check);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);

        update_vibrations_opts = new QPushButton(tab);
        update_vibrations_opts->setObjectName(QStringLiteral("update_vibrations_opts"));

        horizontalLayout_6->addWidget(update_vibrations_opts);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_4);


        verticalLayout_3->addLayout(horizontalLayout_6);

        sel_vib_channels_grbox = new QGroupBox(tab);
        sel_vib_channels_grbox->setObjectName(QStringLiteral("sel_vib_channels_grbox"));
        horizontalLayout_9 = new QHBoxLayout(sel_vib_channels_grbox);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        layout_vib_checks = new QHBoxLayout();
        layout_vib_checks->setSpacing(6);
        layout_vib_checks->setObjectName(QStringLiteral("layout_vib_checks"));

        horizontalLayout_9->addLayout(layout_vib_checks);


        verticalLayout_3->addWidget(sel_vib_channels_grbox);

        scrollArea = new QScrollArea(tab);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 641, 324));
        verticalLayout_7 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        vib_time_plots_layout = new QVBoxLayout();
        vib_time_plots_layout->setSpacing(6);
        vib_time_plots_layout->setObjectName(QStringLiteral("vib_time_plots_layout"));

        horizontalLayout_7->addLayout(vib_time_plots_layout);

        vib_freq_plots_layout = new QVBoxLayout();
        vib_freq_plots_layout->setSpacing(6);
        vib_freq_plots_layout->setObjectName(QStringLiteral("vib_freq_plots_layout"));

        horizontalLayout_7->addLayout(vib_freq_plots_layout);


        verticalLayout_7->addLayout(horizontalLayout_7);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_3->addWidget(scrollArea);


        horizontalLayout_3->addLayout(verticalLayout_3);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        horizontalLayout_4 = new QHBoxLayout(tab_2);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));

        horizontalLayout_4->addLayout(verticalLayout_4);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        horizontalLayout_5 = new QHBoxLayout(tab_3);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));

        horizontalLayout_5->addLayout(verticalLayout_5);

        tabWidget->addTab(tab_3, QString());

        horizontalLayout->addWidget(tabWidget);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);

        DatafuzClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(DatafuzClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 691, 21));
        DatafuzClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(DatafuzClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        DatafuzClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(DatafuzClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        DatafuzClass->setStatusBar(statusBar);

        retranslateUi(DatafuzClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DatafuzClass);
    } // setupUi

    void retranslateUi(QMainWindow *DatafuzClass)
    {
        DatafuzClass->setWindowTitle(QApplication::translate("DatafuzClass", "Datafuz", 0));
        pushButton->setText(QApplication::translate("DatafuzClass", "Load File", 0));
        label->setText(QApplication::translate("DatafuzClass", "Channels", 0));
        vib_channel_n->clear();
        vib_channel_n->insertItems(0, QStringList()
         << QApplication::translate("DatafuzClass", "1", 0)
         << QApplication::translate("DatafuzClass", "2", 0)
         << QApplication::translate("DatafuzClass", "3", 0)
         << QApplication::translate("DatafuzClass", "4", 0)
         << QApplication::translate("DatafuzClass", "5", 0)
         << QApplication::translate("DatafuzClass", "6", 0)
         << QApplication::translate("DatafuzClass", "7", 0)
        );
        label_2->setText(QApplication::translate("DatafuzClass", "Fs", 0));
        plot_freq_check->setText(QApplication::translate("DatafuzClass", "Plot frequency", 0));
        update_vibrations_opts->setText(QApplication::translate("DatafuzClass", "Update", 0));
        sel_vib_channels_grbox->setTitle(QApplication::translate("DatafuzClass", "Selected Channels", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("DatafuzClass", "Vibrations", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("DatafuzClass", "Flow", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("DatafuzClass", "Load && Pression", 0));
    } // retranslateUi

};

namespace Ui {
    class DatafuzClass: public Ui_DatafuzClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATAFUZ_H
