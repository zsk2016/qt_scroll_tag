#ifndef LEFTRIGHTROLLWIDGET_H
#define LEFTRIGHTROLLWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include "buttonlabel.h"

typedef struct rollData
{
    QString uuid;
    QString name;
    ButtonLabel *buttonLabel;
}RollData;
Q_DECLARE_METATYPE(RollData)

class LeftRightRollWidget : public QWidget
{
    Q_OBJECT
public:
    LeftRightRollWidget(QWidget *parent = 0);
    ~LeftRightRollWidget();
    void AddTitleData(QString uuid, QString name);
    void SwitchPdfReader(QString uuid);
    QList<RollData> GetRollData(){return m_listRollData;}
    QString GetCurrentUuid(){return m_currentUuid;}
    void SetCurrentLabel(QString uuid);
signals:
    void DoSwitchPdfReader(QString uuid);
    void DoDeletePdfReader(QString uuid);
    void DoStateChange();
private slots:
    void OnButtonClicked();
    void OnDeleteLabel();
private:
    void ChangeLabelState(QString uuid);
    void ResizeSize();
private:
    QList<RollData> m_listRollData;
    QHBoxLayout *m_hbox;
    QString m_currentUuid;
    QSpacerItem *m_horizontalSpacer;
};


#endif // LEFTRIGHTROLLWIDGET_H
