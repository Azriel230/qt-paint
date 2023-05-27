#ifndef SIZEDIALOG_H
#define SIZEDIALOG_H

#include <QDialog>
#include <QString>
#include <QLineEdit>


class sizeDialog : public QDialog
{
public:
    sizeDialog();

    QString get_widht() const;
    QString get_height() const;

private:
    QLineEdit* m_lineEditWidht;
    QLineEdit* m_lineEditHeight;

};

#endif // SIZEDIALOG_H
