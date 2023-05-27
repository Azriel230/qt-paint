#include "sizedialog.h"
#include <QRegularExpressionValidator>
#include <QtWidgets>
#include <QDialog>

sizeDialog::sizeDialog()
{
    m_lineEditWidht = new QLineEdit;
    m_lineEditHeight = new QLineEdit;

    m_lineEditWidht->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]{4}")));
    m_lineEditHeight->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]{4}")));

    QLabel* labelWidht = new QLabel("&Widht");
    QLabel* labelHeight = new QLabel("&Height");

    labelWidht->setBuddy(m_lineEditWidht);
    labelHeight->setBuddy(m_lineEditHeight);

    QPushButton* pbOk = new QPushButton("&Ok");
    QPushButton* pbCancel = new QPushButton("&Cancel");

    connect(pbOk, SIGNAL(clicked()), this, SLOT(accept()));
    connect(pbCancel, SIGNAL(clicked()), this, SLOT(reject()));

    QGridLayout* newGrLayout = new QGridLayout;
    newGrLayout->addWidget(labelWidht, 0, 0);
    newGrLayout->addWidget(labelHeight, 1, 0);
    newGrLayout->addWidget(m_lineEditWidht, 0, 1);
    newGrLayout->addWidget(m_lineEditHeight, 1, 1);
    newGrLayout->addWidget(pbOk, 2, 0);
    newGrLayout->addWidget(pbCancel, 2, 1);
    setLayout(newGrLayout);
}

QString sizeDialog::get_widht() const
{
    return m_lineEditWidht->text();
}

QString sizeDialog::get_height() const
{
    return m_lineEditWidht->text();
}
