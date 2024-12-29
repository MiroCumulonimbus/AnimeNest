#ifndef DATADEFINE_H
#define DATADEFINE_H

#include <QDate>
#include "CustomAlgorithms.h"

struct sAnimeData{
    double Story;
    double Screen;
    double Role;
    QDate Time;
};

extern int Rstory;  // 只声明，不定义
extern int Rscreen;  // 只声明，不定义
extern int Rrole;  // 只声明，不定义

extern QDateTime Today;
extern QString LocalSeason;

extern int precisePartScore;       // 单项保留的小数位数
extern int preciseTotalScore;      // 总分保留的小数位数
extern double RfinalScore;  // 完结评分占比重

extern QString specialPlaceholder;  // 在网页中的特殊占位符，用于替换集数

extern int IniAnimeNameMatchPrecise;    // 初始化匹配番名的容差（最低匹配度）

extern QVector<QString> OpenOrder;

#endif // DATADEFINE_H


