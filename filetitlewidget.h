#ifndef FILETITLEWIDGET_H
#define FILETITLEWIDGET_H
#include <QWidget>

class FileTitleWidget : public QWidget
{
    Q_OBJECT
public:
    FileTitleWidget(QWidget *parent = 0);
    ~FileTitleWidget();
    void AddTitle(QString uuid, QString name);
    void SwitchPdfReader(QString uuid);
protected:
    void resizeEvent(QResizeEvent *event);
private:
    void ScrollButtonChange(int states);
    void SetButtonState();
private slots:
    void OnBtnScroll();
    void OnBtnMenu();
    void OnFromChildStateChang();
signals:
    void DoSwitchPdfReader(QString uuid);
    void DoDeletePdfReader(QString uuid);
};

#endif // FILETITLEWIDGET_H
