#include "myobject.h"

myobject::myobject()
{

}

//  自定义的 RadioButton 类
myRadioButton::myRadioButton(QWidget *parent) : QRadioButton(parent)
{

}

void myRadioButton::wheelEvent(QWheelEvent *event){
    if(this->conLabel == nullptr)
        return;
    if(event->delta()>0){
        this->conLabel->setText(QString::number(this->conLabel->text().toDouble()+this->AddValue));   //放大
    }
    else{
        this->conLabel->setText(QString::number(this->conLabel->text().toDouble()-this->AddValue));
    }
    this->click();
}

//  自定义的 Checkbox 类
myCheckBox::myCheckBox(QWidget *parent) : QCheckBox(parent)
{

}

void myCheckBox::wheelEvent(QWheelEvent *event){
    if(this->conLabel == nullptr)
        return;
    if(event->delta()>0){
        this->conLabel->setText(QString::number(this->conLabel->text().toDouble()+this->AddValue));   //放大
    }
    else{
        this->conLabel->setText(QString::number(this->conLabel->text().toDouble()-this->AddValue));
    }
    this->click();
    this->click();
}

//  自定义的 Slider 类
mySlider::mySlider(QWidget *parent) : QSlider(parent)
{

}

void mySlider::wheelEvent(QWheelEvent *event){
    if(event->delta()>0){
        this->setValue(this->value()+1);   //放大
    }
    else{
        this->setValue(this->value()-1);
    }
}

//  自定义的 Label 类
myLabel::myLabel(QWidget *parent) : QLabel(parent)
{

}

void myLabel::setText(const QString &text)
{
    QLabel::setText(text);
    emit textChanged(text);
}

void myLabel::wheelEvent(QWheelEvent *event){
    if (this->LabelType == 3){
        QDate date = QDate::fromString(this->text(), "M.d");
        if(event->delta()>0){
            date = date.addDays(1);
            this->setText(date.toString("M.d"));   //放大
        }
        else{
            date = date.addDays(-1);
            this->setText(date.toString("M.d"));
        }
    }
    else if(this->LabelType == 4){
        QDate date = QDate::fromString(this->text(), "yyyy年M月");
        if(event->delta()>0){
            date = date.addMonths(3);
            this->setText(date.toString("yyyy年M月"));
        }
        else{
            date = date.addMonths(-3);
            this->setText(date.toString("yyyy年M月"));
        }
    }
    else {
        if(event->delta()>0){
            if(this->text().toDouble() < limitMax)
                this->setText(QString::number(this->text().toDouble()+this->AddValue));   //放大
        }
        else{
            if(this->text().toDouble() > limitMin)
                this->setText(QString::number(this->text().toDouble()-this->AddValue));
        }
    }
}

