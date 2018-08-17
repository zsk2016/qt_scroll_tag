#include "filetitlewidget.h"
#include "leftrightrollwidget.h"
#include <QScrollArea>
#include <QScrollBar>
#include <QHBoxLayout>
#include <QMenu>

FileTitleWidget::FileTitleWidget(QWidget *parent)
    :QWidget(parent)
{
    QWidget *headWidget = new QWidget;
    headWidget->setObjectName("headWidget");
    headWidget->setFixedSize(42, 39);
    QPushButton *btnFileLeft = new QPushButton(headWidget);
    btnFileLeft->setObjectName("btnFileLeft");
    btnFileLeft->setFixedSize(8, 8);
    QHBoxLayout *headWidgetHbox = new QHBoxLayout;
    headWidgetHbox->addWidget(btnFileLeft);
    headWidget->setLayout(headWidgetHbox);

    QWidget *tailWidget = new QWidget;
    tailWidget->setObjectName("tailWidget");
    tailWidget->setFixedSize(48, 39);
    QPushButton *btnFileList = new QPushButton(tailWidget);
    btnFileList->setObjectName("btnFileList");
    btnFileList->setFixedSize(8, 8);
    QPushButton *btnFileRight = new QPushButton(tailWidget);
    btnFileRight->setObjectName("btnFileRight");
    btnFileRight->setFixedSize(8, 8);
    QHBoxLayout *tailWidgetHbox = new QHBoxLayout;
    tailWidgetHbox->addWidget(btnFileRight);
    tailWidgetHbox->addWidget(btnFileList);
    tailWidget->setLayout(tailWidgetHbox);



    QScrollArea *lrwScrollArea = new QScrollArea;
    lrwScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    lrwScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    lrwScrollArea->setObjectName("lrwScrollArea");
    LeftRightRollWidget *leftRightRollWidget = new LeftRightRollWidget;
    leftRightRollWidget->setObjectName("leftRightRollWidget");
    lrwScrollArea->setWidget(leftRightRollWidget);
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setContentsMargins(0,0,0,0);
    hbox->addWidget(headWidget);
    hbox->addWidget(lrwScrollArea);
    hbox->addWidget(tailWidget);
    setLayout(hbox);
    connect(leftRightRollWidget, SIGNAL(DoSwitchPdfReader(QString)), this, SIGNAL(DoSwitchPdfReader(QString)));
    connect(leftRightRollWidget, SIGNAL(DoDeletePdfReader(QString)), this, SIGNAL(DoDeletePdfReader(QString)));
    connect(leftRightRollWidget, SIGNAL(DoStateChange()), this, SLOT(OnFromChildStateChang()));
    connect(btnFileLeft, &QPushButton::clicked, this, &FileTitleWidget::OnBtnScroll);
    connect(btnFileRight, &QPushButton::clicked, this, &FileTitleWidget::OnBtnScroll);
    connect(btnFileList, &QPushButton::clicked, this, &FileTitleWidget::OnBtnMenu);
}

FileTitleWidget::~FileTitleWidget()
{

}

void FileTitleWidget::AddTitle(QString uuid, QString name)
{
    findChild<LeftRightRollWidget *>("leftRightRollWidget")->AddTitleData(uuid, name);
    SetButtonState();
}

void FileTitleWidget::SwitchPdfReader(QString uuid)
{
    findChild<LeftRightRollWidget *>("leftRightRollWidget")->SwitchPdfReader(uuid);
}

void FileTitleWidget::resizeEvent(QResizeEvent *event)
{
    SetButtonState();
    QWidget::resizeEvent(event);
}

void FileTitleWidget::SetButtonState()
{
    QScrollBar *sb = findChild<QScrollArea *>("lrwScrollArea")->horizontalScrollBar();
    int maxv = sb->maximum();
    int minv = sb->minimum();

    int value = sb->value();
    QList<RollData> listRollData = findChild<LeftRightRollWidget *>("leftRightRollWidget")->GetRollData();
    if(listRollData.size() == 0)
    {
        ScrollButtonChange(-1);
    }
    else if(value == 0 && maxv == 0)
    {
        ScrollButtonChange(0);
    }
    else if(maxv > 0 && value == 0)
    {
        ScrollButtonChange(1);
    }
    else if(maxv > 0 && value > 0 && value < maxv)
    {
        ScrollButtonChange(2);
    }
    else if(maxv > 0 && value > 0 && value == maxv)
    {
        ScrollButtonChange(3);
    }
}

void FileTitleWidget::OnBtnScroll()
{
    QScrollBar *sb = findChild<QScrollArea *>("lrwScrollArea")->horizontalScrollBar();
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if(btn == findChild<QWidget *>("tailWidget")->findChild<QPushButton *>("btnFileRight"))
    {
        int value = sb->value();
        int newValue = value + 30;
        if(newValue >= sb->maximum())
            newValue = sb->maximum();
        sb->setValue(newValue);
    }
    if(btn == findChild<QWidget *>("headWidget")->findChild<QPushButton *>("btnFileLeft"))
    {
        int value = sb->value();
        int newValue = value - 30;
        if(newValue <= sb->minimum())
            newValue = sb->minimum();
        sb->setValue(newValue);
    }
    SetButtonState();
}

void FileTitleWidget::OnBtnMenu()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    LeftRightRollWidget *plrrw = findChild<LeftRightRollWidget *>("leftRightRollWidget");
    QMenu titleMenu(this);
    QAction *action;
    titleMenu.setObjectName("titleMenu");
    int actionw = 0;
    QList<RollData> listRollData = plrrw->GetRollData();
    QString currentUuid = plrrw->GetCurrentUuid();
    for(int i=0; i<listRollData.size(); i++)
    {
        action = titleMenu.addAction(QIcon(), listRollData.at(i).name);
        QVariant v;
        v.setValue(listRollData.at(i));
        action->setData(v);
        if(currentUuid == listRollData.at(i).uuid)
        {
            action->setCheckable(true);
            action->setChecked(true);
        }
        actionw = titleMenu.actionGeometry(action).width();
    }
    QPoint p = button->mapToGlobal(QPoint(-actionw, 10));
    QAction *ac = titleMenu.exec(p);
    if(ac)
    {
        QVariant v = ac->data();
        RollData rValue = v.value<RollData>();
        plrrw->SetCurrentLabel(rValue.uuid);
    }
}

void FileTitleWidget::OnFromChildStateChang()
{
    SetButtonState();
}

void FileTitleWidget::ScrollButtonChange(int states)
{
    int wh = 8;
    QPushButton *btnl = findChild<QWidget *>("headWidget")->findChild<QPushButton *>("btnFileLeft");
    QPushButton *btnr = findChild<QWidget *>("tailWidget")->findChild<QPushButton *>("btnFileRight");
    QPushButton *btnlist = findChild<QWidget *>("tailWidget")->findChild<QPushButton *>("btnFileList");
    if(states == -1)
    {
        btnl->hide();
        btnr->hide();
        btnlist->hide();
    }
    else if(states == 0)
    {
        QMatrix rightmatrix;
        rightmatrix.rotate(90);
        btnl->hide();

        btnr->hide();

        btnlist->setIcon(QIcon(QPixmap(":/lcpdf/icon_info_bar_arrow1").transformed(rightmatrix,Qt::SmoothTransformation)));
        btnlist->show();
        btnlist->setIconSize(QSize(wh, wh));
    }
    else if(states == 1)
    {
        QMatrix rightmatrix;
        rightmatrix.rotate(90);
        btnl->hide();

        btnr->show();
        btnr->setIcon(QIcon(QPixmap(":/lcpdf/icon_info_bar_arrow1")));
        btnr->setIconSize(QSize(wh, wh));

        btnlist->setIcon(QIcon(QPixmap(":/lcpdf/icon_info_bar_arrow1").transformed(rightmatrix,Qt::SmoothTransformation)));
        btnlist->show();
        btnlist->setIconSize(QSize(wh, wh));
    }
    else if(states == 2)
    {
        QMatrix rightmatrix;
        rightmatrix.rotate(180);
        btnl->show();
        btnl->setIcon(QIcon(QPixmap(":/lcpdf/icon_info_bar_arrow1").transformed(rightmatrix,Qt::SmoothTransformation)));
        btnl->setIconSize(QSize(wh, wh));

        btnr->show();
        btnr->setIcon(QIcon(QPixmap(":/lcpdf/icon_info_bar_arrow1")));
        btnr->setIconSize(QSize(wh, wh));

        QMatrix rightmatrix1;
        rightmatrix1.rotate(90);
        btnlist->setIcon(QIcon(QPixmap(":/lcpdf/icon_info_bar_arrow1").transformed(rightmatrix1,Qt::SmoothTransformation)));
        btnlist->show();
        btnlist->setIconSize(QSize(wh, wh));
    }
    else if(states == 3)
    {
        QMatrix rightmatrix;
        rightmatrix.rotate(180);
        btnl->show();
        btnl->setIcon(QIcon(QPixmap(":/lcpdf/icon_info_bar_arrow1").transformed(rightmatrix,Qt::SmoothTransformation)));
        btnl->setIconSize(QSize(wh, wh));

        btnr->hide();

        QMatrix rightmatrix1;
        rightmatrix1.rotate(90);
        btnlist->setIcon(QIcon(QPixmap(":/lcpdf/icon_info_bar_arrow1").transformed(rightmatrix1,Qt::SmoothTransformation)));
        btnlist->show();
        btnlist->setIconSize(QSize(wh, wh));
    }
}
