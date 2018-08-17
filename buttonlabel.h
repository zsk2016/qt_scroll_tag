#ifndef BUTTONLABEL_H
#define BUTTONLABEL_H
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
class ButtonLabel : public QLabel
{
    Q_OBJECT
public:
    ButtonLabel(QWidget *parent = 0);
    void SetText(QString str);
    QString GetElidedText(QFont font, QString str, int MaxWidth);
    void SetLabelIsSelected(bool isSelect = false);
signals:
    void Clicked();
    void BtnClicked();
protected:
    void mousePressEvent(QMouseEvent *ev);
private:
    QPushButton *m_btn;
    QLabel *m_label;
    QLabel *m_pixLabel;
};


#endif // BUTTONLABEL_H
