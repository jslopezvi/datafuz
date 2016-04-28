/********************************************************************************
** Form generated from reading UI file 'signals_config.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNALS_CONFIG_H
#define UI_SIGNALS_CONFIG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_7;
    QLabel *label;
    QSpinBox *fs_val;
    QSpacerItem *horizontalSpacer_8;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_2;
    QFormLayout *formLayout;
    QLabel *maxADCPositionLabel_2;
    QSpinBox *maxADCPositionSpinBox;
    QLabel *maxVoltLabel;
    QDoubleSpinBox *maxVoltDoubleSpinBox;
    QLabel *minVoltLabel;
    QDoubleSpinBox *minVoltDoubleSpinBox;
    QSpacerItem *horizontalSpacer_6;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_4;
    QFormLayout *channel_sensitivities_layout;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *apply_vib_config;
    QSpacerItem *horizontalSpacer_3;
    QWidget *tab_2;
    QWidget *tab_3;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(480, 289);
        Dialog->setModal(true);
        verticalLayout = new QVBoxLayout(Dialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        tabWidget = new QTabWidget(Dialog);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout_4 = new QVBoxLayout(tab);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        verticalLayout_5 = new QVBoxLayout(groupBox_3);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_7);

        label = new QLabel(groupBox_3);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_6->addWidget(label);

        fs_val = new QSpinBox(groupBox_3);
        fs_val->setObjectName(QStringLiteral("fs_val"));
        fs_val->setMinimumSize(QSize(100, 0));
        fs_val->setMaximumSize(QSize(100, 16777215));
        fs_val->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        fs_val->setMinimum(1);
        fs_val->setMaximum(99999);
        fs_val->setValue(52734);

        horizontalLayout_6->addWidget(fs_val);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_8);


        verticalLayout_5->addLayout(horizontalLayout_6);


        verticalLayout_3->addWidget(groupBox_3);

        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMaximumSize(QSize(16777215, 200));
        groupBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        horizontalLayout_4 = new QHBoxLayout(groupBox);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        maxADCPositionLabel_2 = new QLabel(groupBox);
        maxADCPositionLabel_2->setObjectName(QStringLiteral("maxADCPositionLabel_2"));
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        maxADCPositionLabel_2->setFont(font);
        maxADCPositionLabel_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(0, QFormLayout::LabelRole, maxADCPositionLabel_2);

        maxADCPositionSpinBox = new QSpinBox(groupBox);
        maxADCPositionSpinBox->setObjectName(QStringLiteral("maxADCPositionSpinBox"));
        maxADCPositionSpinBox->setMinimumSize(QSize(100, 0));
        maxADCPositionSpinBox->setMaximumSize(QSize(200, 16777215));
        maxADCPositionSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        maxADCPositionSpinBox->setMinimum(1);
        maxADCPositionSpinBox->setMaximum(999999999);
        maxADCPositionSpinBox->setValue(16777216);

        formLayout->setWidget(0, QFormLayout::FieldRole, maxADCPositionSpinBox);

        maxVoltLabel = new QLabel(groupBox);
        maxVoltLabel->setObjectName(QStringLiteral("maxVoltLabel"));
        maxVoltLabel->setFont(font);
        maxVoltLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(1, QFormLayout::LabelRole, maxVoltLabel);

        maxVoltDoubleSpinBox = new QDoubleSpinBox(groupBox);
        maxVoltDoubleSpinBox->setObjectName(QStringLiteral("maxVoltDoubleSpinBox"));
        maxVoltDoubleSpinBox->setMinimumSize(QSize(100, 0));
        maxVoltDoubleSpinBox->setMaximumSize(QSize(200, 16777215));
        maxVoltDoubleSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        maxVoltDoubleSpinBox->setMaximum(999.99);
        maxVoltDoubleSpinBox->setValue(5);

        formLayout->setWidget(1, QFormLayout::FieldRole, maxVoltDoubleSpinBox);

        minVoltLabel = new QLabel(groupBox);
        minVoltLabel->setObjectName(QStringLiteral("minVoltLabel"));
        minVoltLabel->setFont(font);
        minVoltLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(2, QFormLayout::LabelRole, minVoltLabel);

        minVoltDoubleSpinBox = new QDoubleSpinBox(groupBox);
        minVoltDoubleSpinBox->setObjectName(QStringLiteral("minVoltDoubleSpinBox"));
        minVoltDoubleSpinBox->setMinimumSize(QSize(100, 0));
        minVoltDoubleSpinBox->setMaximumSize(QSize(200, 16777215));
        minVoltDoubleSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        minVoltDoubleSpinBox->setMaximum(999.99);
        minVoltDoubleSpinBox->setValue(2.5);

        formLayout->setWidget(2, QFormLayout::FieldRole, minVoltDoubleSpinBox);


        horizontalLayout_5->addLayout(formLayout);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_6);


        horizontalLayout_4->addLayout(horizontalLayout_5);


        verticalLayout_3->addWidget(groupBox);


        verticalLayout_4->addLayout(verticalLayout_3);

        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        horizontalLayout = new QHBoxLayout(groupBox_2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        channel_sensitivities_layout = new QFormLayout();
        channel_sensitivities_layout->setObjectName(QStringLiteral("channel_sensitivities_layout"));

        horizontalLayout_2->addLayout(channel_sensitivities_layout);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);


        horizontalLayout->addLayout(horizontalLayout_2);


        verticalLayout_4->addWidget(groupBox_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        apply_vib_config = new QPushButton(tab);
        apply_vib_config->setObjectName(QStringLiteral("apply_vib_config"));

        horizontalLayout_3->addWidget(apply_vib_config);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout_4->addLayout(horizontalLayout_3);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        tabWidget->addTab(tab_3, QString());

        verticalLayout_2->addWidget(tabWidget);


        verticalLayout->addLayout(verticalLayout_2);


        retranslateUi(Dialog);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Signals Configuration", 0));
        groupBox_3->setTitle(QApplication::translate("Dialog", "Sampling Frequency", 0));
        label->setText(QApplication::translate("Dialog", "Fs", 0));
        groupBox->setTitle(QApplication::translate("Dialog", "Voltage Scaling", 0));
        maxADCPositionLabel_2->setText(QApplication::translate("Dialog", "Max ADC Position", 0));
        maxVoltLabel->setText(QApplication::translate("Dialog", "Max Volt Value", 0));
        minVoltLabel->setText(QApplication::translate("Dialog", "Zero Volt Value", 0));
        groupBox_2->setTitle(QApplication::translate("Dialog", "Channels Sensitivity (mV/g)", 0));
        apply_vib_config->setText(QApplication::translate("Dialog", "Apply Vibration Configuration", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Dialog", "Vibration", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Dialog", "Flow", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("Dialog", "Load && Pressure", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNALS_CONFIG_H
