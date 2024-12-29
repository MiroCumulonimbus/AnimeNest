#include "datadefine.h"


int Rstory = 3;
int Rscreen = 1;
int Rrole = 1;

QDateTime Today = QDateTime::currentDateTime();
QString LocalSeason = getLocalSeason();

int precisePartScore = 2;       // 单项保留的小数位数
int preciseTotalScore = 2;      // 总分保留的小数位数
double RfinalScore = 0.128395;  // 完结评分占比重

QString specialPlaceholder = "#p&p1#";  // 在网页中的特殊占位符，用于替换集数

int IniAnimeNameMatchPrecise = 60;    // 初始化匹配番名的容差（最低匹配度）

QVector<QString> OpenOrder = {"girigirilove", "Sakura", "Iyxdm"};  // 开启顺序
