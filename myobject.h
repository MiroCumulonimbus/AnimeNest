#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <QObject>
#include <QWidget>
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>
#include <QMouseEvent>
#include <QSlider>
#include <QDate>

class myobject
{
public:
    myobject();
};

class myRadioButton : public QRadioButton
{
    Q_OBJECT
public:
    myRadioButton();
    explicit myRadioButton(QWidget *parent = nullptr);

    void wheelEvent(QWheelEvent *event);

    QLabel *conLabel;
    double AddValue = 1;
};

class myCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    myCheckBox();
    explicit myCheckBox(QWidget *parent = nullptr);

    void wheelEvent(QWheelEvent *event);

    QLabel *conLabel;
    double AddValue = 1;
};

class myLabel : public QLabel
{
    Q_OBJECT
signals:
    void textChanged(const QString &text);

public:
    myLabel();
    explicit myLabel(QWidget *parent = nullptr);

    void wheelEvent(QWheelEvent *event);
    void setText(const QString &text);

    void configSet(int labelType, double addValue, double limitMin, double limitMax){
        LabelType = labelType;
        AddValue = addValue;
        this->limitMin = limitMin;
        this->limitMax = limitMax;
    }

//  Tpye 1 : 增加值为 1 的权重型
//  Tpye 2 : 增加值为 0.1 的分数段型
//  Tpye 3 : 增加值为 1天 的日期型
//  Tpye 4 : 增加值为 3个月 的季度
    int LabelType = 1;

    double AddValue = 1;
    double limitMin = 0;
    double limitMax = 100;
};

class mySlider : public QSlider
{
    Q_OBJECT
public:
    mySlider();
    explicit mySlider(QWidget *parent = nullptr);

    void wheelEvent(QWheelEvent *event);

};

#endif // MYOBJECT_H
