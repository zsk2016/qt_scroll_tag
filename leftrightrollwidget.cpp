#include "leftrightrollwidget.h"

LeftRightRollWidget::LeftRightRollWidget(QWidget *parent)
    :QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground,true);
    setObjectName(QStringLiteral("LeftRightRollWidget"));
    m_hbox = new QHBoxLayout;
    m_hbox->setContentsMargins(0,0,0,0);
    m_hbox->setSpacing(0);
    setLayout(m_hbox);
    m_horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_hbox->addItem(m_horizontalSpacer);
}

LeftRightRollWidget::~LeftRightRollWidget()
{

}

void LeftRightRollWidget::SwitchPdfReader(QString uuid)
{
    emit DoSwitchPdfReader(uuid);
    ChangeLabelState(uuid);
    m_currentUuid = uuid;
}

void LeftRightRollWidget::SetCurrentLabel(QString uuid)
{
    emit DoSwitchPdfReader(uuid);
    ChangeLabelState(uuid);
}

void LeftRightRollWidget::AddTitleData(QString uuid, QString name)
{
    ButtonLabel *label = new ButtonLabel;
    label->SetLabelIsSelected(true);
    label->SetText(name);
    connect(label, SIGNAL(Clicked()), this, SLOT(OnButtonClicked()));
    connect(label, SIGNAL(BtnClicked()), this, SLOT(OnDeleteLabel()));
    label->setFixedSize(190, 30);
    RollData rd;
    rd.uuid = uuid;
    rd.name = name;
    rd.buttonLabel = label;
    m_listRollData.push_back(rd);
    m_hbox->removeItem(m_horizontalSpacer);
    m_hbox->addWidget(label, 0, Qt::AlignBottom);
    m_hbox->addItem(m_horizontalSpacer);
    ChangeLabelState(rd.uuid);
    emit DoSwitchPdfReader(rd.uuid);
    m_currentUuid = rd.uuid;
    ResizeSize();
}

void LeftRightRollWidget::ChangeLabelState(QString uuid)
{
    for(int i=0; i<m_listRollData.size(); i++)
    {
        if(uuid == m_listRollData.at(i).uuid)
        {
            m_listRollData.at(i).buttonLabel->SetLabelIsSelected(true);

        }
        else
        {
            m_listRollData.at(i).buttonLabel->SetLabelIsSelected(false);

        }
    }
    m_currentUuid = uuid;
}

void LeftRightRollWidget::OnButtonClicked()
{
    QLabel *btn = qobject_cast<QLabel*>(sender());
    for(int i=0; i<m_listRollData.size(); i++)
    {
        if(btn == m_listRollData.at(i).buttonLabel)
        {
            emit DoSwitchPdfReader(m_listRollData.at(i).uuid);
            ChangeLabelState(m_listRollData.at(i).uuid);
            break;
        }
    }
}

void LeftRightRollWidget::OnDeleteLabel()
{
    QLabel *btn = qobject_cast<QLabel*>(sender());
    int ii = -1;
    for(int i=0; i<m_listRollData.size(); i++)
    {
        if(btn == m_listRollData.at(i).buttonLabel)
        {
            emit DoDeletePdfReader(m_listRollData.at(i).uuid);
            btn->setParent(NULL);
            m_hbox->removeWidget(btn);
            ii = i;
            int nextNum = -1;
            if(i-1>=0)
                nextNum = i-1;
            else if(i+1<m_listRollData.size())
                nextNum = i+1;
            if(nextNum != -1)
            {
                if(m_listRollData.at(i).uuid == m_currentUuid)
                {
                    emit DoSwitchPdfReader(m_listRollData.at(nextNum).uuid);
                    ChangeLabelState(m_listRollData.at(nextNum).uuid);
                }
            }
            break;
        }
    }
    if(ii>-1)
    {
        m_listRollData.removeAt(ii);
    }
    ResizeSize();
    emit DoStateChange();
}

void LeftRightRollWidget::ResizeSize()
{
    int w = 42;
    for(int i=0; i<m_listRollData.size(); i++)
    {
        w += m_listRollData.at(i).buttonLabel->width();
    }
    setFixedSize(w, 40);
}
