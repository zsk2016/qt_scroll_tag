#include "buttonlabel.h"

ButtonLabel::ButtonLabel(QWidget *parent)
    :QLabel(parent)
{
    setAutoFillBackground(true);
    m_btn = new QPushButton;
    m_btn->setFixedSize(12, 12);
    m_btn->setObjectName("m_btn");
    m_label = new QLabel;
    m_label->setFixedSize(135, 30);
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setContentsMargins(10,0,0,0);
    m_pixLabel = new QLabel;
    m_pixLabel->setFixedSize(20,20);
    hbox->addWidget(m_label);
    hbox->addWidget(m_btn);
    setLayout(hbox);
    connect(m_btn, SIGNAL(clicked()), this, SIGNAL(BtnClicked()));
    SetLabelIsSelected(false);
}

void ButtonLabel::SetText(QString str)
{
    m_label->setText(GetElidedText(m_label->font(), str, 130));
    QWidget::setToolTip(str);
}

QString ButtonLabel::GetElidedText(QFont font, QString str, int MaxWidth)
{
    QFontMetrics fontWidth(font);
    int width = fontWidth.width(str);  //计算字符串宽度
    if(width>=MaxWidth)  //当字符串宽度大于最大宽度时进行转换
    {
        str = fontWidth.elidedText(str,Qt::ElideRight,MaxWidth);  //右部显示省略号
    }
    return str;   //返回处理后的字符串
}

void ButtonLabel::SetLabelIsSelected(bool isSelect)
{
    QString imagePath = isSelect?":/lcpdf/Icon_TitleBar_Press":":/lcpdf/Icon_TitleBar_Default";
    QPixmap pixmap(imagePath);
    QPixmap fitpixmap=pixmap.scaled(width(), height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    setPixmap(fitpixmap);
}

void ButtonLabel::mousePressEvent(QMouseEvent *ev)
{
    emit Clicked();
    QLabel::mousePressEvent(ev);
}
