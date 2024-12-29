#include "CustomAlgorithms.h"


QFileInfoList getFolders(const QString &path, bool createIfNotExist) {
    QFileInfoList folderList;
    QDir dir(path);

    // 检查路径是否存在
    if (!dir.exists()) {
        if (createIfNotExist) {
            // 如果路径不存在且createIfNotExist为true，尝试创建路径
            if (dir.mkpath(path)) {
                // 成功创建路径
                messageBoxWindow("路径不存在: " + path + "\n已创建路径: " + path);
            } else {
                // 创建失败，显示错误信息
                messageBoxWindow("路径创建失败:" + path);
                return folderList;
            }
        } else {
            // 如果路径不存在且不创建，提示路径不存在
            messageBoxWindow("路径不存在:" + path);
            return folderList;
        }
    }

    // 设置只获取文件夹的过滤条件
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    // 获取当前目录下的所有文件夹
    folderList = dir.entryInfoList();

    return folderList;
}

QFileInfoList getFiles(const QString &path, int depth, QStringList filters) {
    QFileInfoList resultList;
    QDir dir(path);

    if (!dir.exists()) {
        messageBoxWindow("路径不存在:" + path);
        return resultList;
    }

    // 获取当前目录的所有文件和文件夹
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    dir.setNameFilters(filters);

    QFileInfoList entries = dir.entryInfoList();

    for (const QFileInfo &entry : entries) {
        if (entry.isFile()) {
            resultList.append(entry); // 添加符合过滤条件的文件
        } else if (entry.isDir() && depth > 0) {
            // 递归获取子文件夹内容，层数减1
            resultList.append(getFiles(entry.absoluteFilePath(), depth - 1));
        }
    }

    return resultList;
}

QVector<QVector<QString>> getRefKey(const QVector<QString> &reference,
                                     const QHash<QString, QHash<QString, QVector<QString>>> &target,
                                     const QVector<QString> &keys)
{
    QVector<QVector<QVector<QString>>> matchingResults(keys.size()); // 用于存储每个 key 的匹配结果
    QVector<QVector<QString>> matchingKeys(keys.size()); // 用于存储每个 key 对应的匹配键

    // 找到所有匹配的结果
    for (auto it = target.begin(); it != target.end(); ++it) {
        if (it.value().contains("番名") && vectorMatch(reference, it.value()["番名"])) {
            // 对于每个 key，检查是否匹配
            for (int i = 0; i < keys.size(); ++i) {
                if (it.value().contains(keys[i]) && !it.value()[keys[i]].empty()) {
                    matchingResults[i].append(it.value()[keys[i]]);
                    matchingKeys[i].append(it.key());
                }
            }
        }
    }

    // 处理每个 key 的匹配结果
    QVector<QVector<QString>> finalResults;

    for (int i = 0; i < keys.size(); ++i) {
        if (matchingKeys[i].isEmpty()) {
            finalResults.append({"无结果", keys[i]}); // 如果没有匹配到结果，返回 "无结果"
        } else if (matchingKeys[i].size() == 1) {
            finalResults.append(matchingResults[i][0]); // 如果只匹配到一个结果，返回第一个结果
        } else {
            // 如果有多个匹配结果，检查它们是否有冲突
            bool conflict = false;
            QHash<QString, QString> seenResults;

            for (int j = 0; j < matchingResults[i].size(); ++j) {

                QString currentString = stringVectorToString(matchingResults[i][j], ",");
                if (seenResults.empty())
                    seenResults[currentString] = matchingKeys[i][j];
                else if(!seenResults.contains(currentString)) {
                    conflict = true;
                    seenResults[currentString] = matchingKeys[i][j];
                }
            }

            if (!conflict) {
                finalResults.append(matchingResults[i][0]); // 如果没有冲突，返回第一个匹配结果
            } else {
                // 如果有冲突，返回冲突信息
                QVector<QString> conflictResult{"重复查询", keys[i]};
                conflictResult.append(matchingKeys[i]);
                finalResults.append(conflictResult);
            }
        }
    }

    return finalResults; // 返回所有 key 的匹配结果
}

QString getHash(const QHash<QString, QHash<QString, QHash<QString, QVector<QString>>>> &target,
                QString key1, QString key2, QString key3, int vecTarget) {
    QString result;
    if (target.contains(key1) && target[key1].contains(key2) &&
        target[key1][key2].contains(key3) && target[key1][key2][key3].size() > vecTarget) {
        result = target[key1][key2][key3][vecTarget];
    }
    return result;
}

QVector<QString> getHash(const QHash<QString, QHash<QString, QHash<QString, QVector<QString>>>> &target,
                         QString key1, QString key2, QString key3) {
    QVector<QString> result;
    if (target.contains(key1) && target[key1].contains(key2) && target[key1][key2].contains(key3)) {
        result = target[key1][key2][key3];
    }
    return result;
}

QHash<QString, QVector<QString>> getHash(const QHash<QString, QHash<QString, QHash<QString, QVector<QString>>>> &target,
                                                           QString key1, QString key2) {
    QHash<QString, QVector<QString>> result;
    if (target.contains(key1) && target[key1].contains(key2)) {
        result = target[key1][key2];
    }
    return result;
}

QHash<QString, QHash<QString, QVector<QString>>> getHash(const QHash<QString, QHash<QString, QHash<QString, QVector<QString>>>> &target,
                                          QString key1) {
    QHash<QString, QHash<QString, QVector<QString>>> result;
    if (target.contains(key1)) {
        result = target[key1];
    }
    return result;
}

QString getHash(const QHash<QString, QHash<QString, QVector<QString>>> &target, QString key1, QString key2, int vecTarget){
    QString result;
    if(target.contains(key1) && target[key1].contains(key2) && target[key1][key2].size() > vecTarget)
        result = target[key1][key2][vecTarget];
    return result;
}
QVector<QString> getHash(const QHash<QString, QHash<QString, QVector<QString>>> &target, QString key1, QString key2){
    QVector<QString> result;
    if(target.contains(key1) && target[key1].contains(key2))
        result = target[key1][key2];
    return result;
}
QHash<QString, QVector<QString>> getHash(const QHash<QString, QHash<QString, QVector<QString>>> &target, QString key1){
    QHash<QString, QVector<QString>> result;
    if(target.contains(key1))
        result = target[key1];
    return result;
}

QString getHash(const QHash<QString, QVector<QString>> &target, QString key1, int vecTarget){
    QString result;
    if(target.contains(key1) && target[key1].size() > vecTarget)
        result = target[key1][vecTarget];
    return result;
}
QVector<QString> getHash(const QHash<QString, QVector<QString>> &target, QString key1){
    QVector<QString> result;
    if(target.contains(key1))
        result = target[key1];
    return result;
}

QVector<QVector<QString>> vectorStringCut(const QVector<QString> &input, const QString &cutType) {
    QVector<QVector<QString>> result;

    int maxSize = 0;
    // 首先确定最大子数组长度
    for (const QString &str : input) {
        QVector<QString> splitResult = str.split(cutType).toVector();
        maxSize = qMax(maxSize, splitResult.size());
    }

    result.resize(maxSize);  // 将 result 直接调整为最大子数组长度

    // 遍历输入的每个 QString
    for (int i = 0; i < input.size(); ++i) {
        QVector<QString> splitResult = input[i].split(cutType).toVector();

        // 如果当前数组大小小于最大长度，则填充空字符串
        while (splitResult.size() < maxSize) {
            splitResult.append("");  // 可以根据需要填充占位符，例如 "N/A"
        }

        // 将当前的分割结果一一对应到 result 中
        for (int j = 0; j < splitResult.size(); ++j) {
            result[j].append(splitResult[j]);  // 向 result 中添加每个分割的元素
        }
    }

    return result;  // 返回最终的二维QVector
}

void dataDisplay(const QHash<QString,QHash<QString,QVector<QString>>> &data, QTreeWidget *displayWidget){
    displayWidget->clear();
    // 遍历QHash
    for(auto outerIt = data.constBegin(); outerIt != data.constEnd(); ++outerIt) {
        // 创建一个顶级项
        QTreeWidgetItem* topLevelItem = new QTreeWidgetItem(displayWidget);
        topLevelItem->setText(0, outerIt.key());

        // 遍历内部的QHash
        for(auto innerIt = outerIt.value().constBegin(); innerIt != outerIt.value().constEnd(); ++innerIt) {
            // 创建一个子项
            QTreeWidgetItem* childItem = new QTreeWidgetItem(topLevelItem);
            childItem->setText(0, innerIt.key());

            // 遍历QVector
            for(const QString& str : innerIt.value()) {
                // 创建一个子项
                QTreeWidgetItem* grandChildItem = new QTreeWidgetItem(childItem);
                grandChildItem->setText(0, str);
            }
        }
    }
}


// 求日期间隔时间
int daysBetween(const QDate& date1, const QDate& date2) {
    if (date1.year() < 1800 || date1.year() > 2500 || date2.year() < 1800 || date2.year() > 2500) {
        QString logsInfo = "日期无效";
        if (date1.isValid() || date2.isValid())
            logsInfo = "日期超出范围2000 - 2100：" + date1.toString("yyyy.MM.dd") + " / " + date2.toString("yyyy.MM.dd");
        setLogsInfo(logsInfo);
    }

    return date1.daysTo(date2);
}

int daysBetween(const QString& date1, const QString& date2) {
    QDate d1 = QDate::fromString(date1, "yyyy.MM.dd");
    QDate d2 = QDate::fromString(date2, "yyyy.MM.dd");

    if (!d1.isValid() || !d2.isValid()) {
        setLogsInfo("无效日期格式：" + date1 + " / " + date2);
    }
    return daysBetween(d1, d2);
}

int daysBetween(const QDateTime& datetime1, const QDateTime& datetime2) {
    QDate d1 = datetime1.date();
    QDate d2 = datetime2.date();

    return daysBetween(d1, d2);
}


//  生成当前时间/年月日 / 周 / 星期几 / 当前季度
QString getLocalTime() {
    return QDateTime::currentDateTime().toString("yyyy.MM.dd");
}

QString getLocalWeek() {
    return QDateTime::currentDateTime().toString("ddd");
}

QString getLocalSeason() {
    int month = QDate::currentDate().month();
    int year = QDate::currentDate().year();
    return QString("%1年%2月").arg(year).arg((month - 1) / 3 * 3 + 1);
}

// 给定一个QString日期 yyyy年M月d日 和 一个int总集数 n 返回季度
QVector<QString> getQStringSeasons(QString season, int totalEpisode) {
    QVector<QString> result;

    // 解析输入日期，例如 "2024年7月1日"
    QDate startDate = QDate::fromString(season, "yyyy年M月d日");
    if (!startDate.isValid()) {
        return result; // 日期无效，返回空结果
    }

    // 计算结束日期，总集数的时间范围
    int intervalDays = 7; // 每集的间隔天数
    QDate endDate = startDate.addDays(intervalDays * (totalEpisode - 1));

    // 季度起始日期
    QVector<QDate> quarterStartDates = {
        QDate(startDate.year(), 1, 1),
        QDate(startDate.year(), 4, 1),
        QDate(startDate.year(), 7, 1),
        QDate(startDate.year(), 10, 1),
        QDate(startDate.year()+1, 1, 1),
        QDate(startDate.year()+1, 4, 1)
    };

    // 遍历每个季度，判断与时间段是否有交集
    for (int i = 0; i < quarterStartDates.size() - 1; ++i) {
        QDate quarterStart = quarterStartDates[i];
        QDate quarterEnd = quarterStartDates[i + 1];

        // 如果开始日期和结束日期与当前季度有交集
        if (startDate <= quarterEnd || endDate >= quarterStart) {
            // 计算覆盖当前季度的天数
            QDate effectiveStart = startDate > quarterStart ? startDate : quarterStart;
            QDate effectiveEnd = endDate < quarterEnd ? endDate : quarterEnd;
            int coveredDays = effectiveStart.daysTo(effectiveEnd) + 1;

            // 如果覆盖天数超过该季度一半，返回当前季度的起始日期（yyyy年M月）
            if (coveredDays > 45) {
                QString quarterString = quarterStart.toString("yyyy年M月");
                result.append(quarterString);
            }
        }
    }

    return result;
}

// 消息弹窗
void messageBoxWindow(QString message){
    QMessageBox mess;
    mess.setText(message);
    mess.exec();
}

// vector -> string
// {a , b , c} -> a<addType>b<addType>c
QString stringVectorToString(QVector<QString> vString, QString addType){
    QString ans = vString.empty() ? "" : vString[0];
    for(int i = 1 ; i < vString.size(); ++i){
        ans += addType + vString[i];
    }
    return ans;
}

// 数组匹配 两个数组有相同元素则返回true
bool vectorMatch(QVector<QString> v1, QVector<QString> v2){
    int size1 = v1.size();
    int size2 = v2.size();

    if (size1 == 0 || size2 == 0) {
        return false;  // 如果其中一个向量为空，返回 无匹配
    }

    for(auto i : v1){
        if(v2.contains(i)) {
            return true;
        }
    }
    return false;
}

int vectorMatchBlur(QVector<QString> v1, QVector<QString> v2, int similarityThreshold) {
    int size1 = v1.size();
    int size2 = v2.size();

    if (size1 == 0 || size2 == 0) {
        return 1;  // 如果其中一个向量为空，返回最低相似度1
    }

    int maxSimilarity = 1;

    // 遍历两个向量的所有组合，找出最高相似度
    for (int i = 0; i < size1; ++i) {
        for (int j = 0; j < size2; ++j) {
            QString str1 = v1[i];
            QString str2 = v2[j];

            int lengthlarge = qMax(str1.length(), str2.length());
            int lengthsmall = qMin(str1.length(), str2.length());

            // 如果两个字符串长度差距过大，直接跳过
            if ((lengthlarge == 0 || lengthsmall == 0) ||
                    (((float)lengthlarge / (float)lengthsmall) > 1.8 && lengthsmall < 4) ||
                    ((float)lengthlarge / (float)lengthsmall) > 2.5) {
                continue;
            }
            // 计算字符串之间的Levenshtein距离
            int maxDistance = std::max(str1.length(), str2.length());
            int distance = levenshteinDistance(str1, str2, maxDistance * (100 - similarityThreshold) / 100);

            if (distance < 0) { // 如果中止计算，跳过当前匹配
                continue;
            }

            // 映射Levenshtein距离到1-100之间
            int similarity = 100 - (maxDistance > 0 ? (distance * 100) / maxDistance : 0);
            similarity = std::max(1, std::min(similarity, 100)); // 限制在1-100之间

            // 更新最高相似度
            maxSimilarity = std::max(maxSimilarity, similarity);
        }
    }

    return maxSimilarity;
}

// 计算Levenshtein距离，增加中止条件
int levenshteinDistance(const QString &s1, const QString &s2, int threshold) {
    int len1 = s1.length();
    int len2 = s2.length();

    QVector<QVector<int>> dp(2, QVector<int>(len2 + 1, 0));

    // 初始化
    for (int j = 0; j <= len2; ++j) {
        dp[0][j] = j;
    }

    // 填充动态规划表，增加中止逻辑
    for (int i = 1; i <= len1; ++i) {
        dp[i % 2][0] = i;
        int minRowValue = INT_MAX;

        for (int j = 1; j <= len2; ++j) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            dp[i % 2][j] = std::min({dp[(i - 1) % 2][j] + 1, dp[i % 2][j - 1] + 1, dp[(i - 1) % 2][j - 1] + cost});
            minRowValue = std::min(minRowValue, dp[i % 2][j]);
        }

        // 如果当前行的最小值超过阈值，则提前中止计算
        if (minRowValue > threshold) {
            return -1;
        }
    }

    return dp[len1 % 2][len2];
}

QVector<QString> removeDuplicates(const QVector<QString>& vec) {
    QSet<QString> seen; // 用于跟踪已经遇到的元素
    QVector<QString> result; // 存储去重后的结果

    for (const QString& str : vec) {
        if (!str.isEmpty() && !seen.contains(str)) {
            seen.insert(str); // 标记为已见
            result.append(str); // 添加到结果中
        }
    }

    return result; // 返回去重后的结果
}

QVector<QString> removeDuplicateLabels(const QVector<QString>& vec) {
    QHash<QString, QString> seen; // 用于跟踪已经遇到的元素
    QVector<QString> result; // 存储去重后的结果

    QVector<QString> originalOrder;

    for (const QString& str : vec) {
        if (!str.isEmpty() && !seen.contains(str)) {
            if(str.contains(" / ")){
                auto splitResult = str.split(" / ");
                if(splitResult.size() > 1){
                    seen[splitResult[0]] = splitResult[1];
                    if(!originalOrder.contains(splitResult[0]))
                        originalOrder.push_back(splitResult[0]);
                }
            }
            else{
                seen[str] = "";
                originalOrder.push_back(str);
            }
        }
    }
    for(auto i : originalOrder){
        if(seen[i] != "")
            result.push_back(i + " / " + seen[i]);
        else
            result.push_back(i);
    }

    return result; // 返回去重后的结果
}



QString dataMatch(QVector<QString> v1, QVector<QString> v2, QVector<QString>& result){
    QString ans = "";
    result = v1;
    for(int i = 0 ; i < v2.size() ; ++i){
        if(!result.contains(v2[i])){
            result.push_back(v2[i]);
            ans += ans == "" ? "新增：" + v2[i] : ", " + v2[i];
        }
    }
    return ans;
}

QString dataMatch(QHash<QString,QHash<QString,QVector<QString>>> v1,
                  QHash<QString,QHash<QString,QVector<QString>>> v2,
                  QHash<QString,QHash<QString,QVector<QString>>>& result,
                  QHash<QString,QHash<QString,QVector<QString>>>& addedData){
    QString ans = "";
    result = v1;
    addedData.clear();
    for(auto i = v2.begin() ; i != v2.end() ; ++i){
        if(!result.contains(i.key())){
            result.insert(i.key(), i.value());
            addedData.insert(i.key(), i.value());
            if(ans != "")
                ans += "\n";
            ans += "将为：" + i.key();
            for(auto j = i.value().begin() ; j != i.value().end() ; ++j){
                ans += "\n新增词条：" + j.key() + " 内容：" + stringVectorToString(j.value(), ", ");
            }
        }
        else{
            for(auto j = i.value().begin() ; j != i.value().end() ; ++j){
                if(!result[i.key()].contains(j.key())){
                    result[i.key()][j.key()] = j.value();
                    addedData[i.key()][j.key()] = j.value();
                    if(ans != "")
                        ans += "\n";
                    ans += "将为：" + i.key();
                    ans += "\n新增词条：" + j.key() + " 内容：" + stringVectorToString(j.value(), ", ");
                }
                else{
                    for(auto k = 0 ; k < j.value().size() ; ++k){
                        if(!result[i.key()][j.key()].contains(j.value()[k])){
                            result[i.key()][j.key()].push_back(j.value()[k]);
                            addedData[i.key()][j.key()].push_back(j.value()[k]);
                            if(ans != "")
                                ans += "\n";
                            ans += "\n将为：" + i.key() + " 的 " + j.key() + " 添加：" + j.value()[k];
                        }
                    }
                }
            }
        }
    }
    return ans;
}

QString dataMatch(QHash<QString,QVector<QString>> v1,
                  QHash<QString,QVector<QString>> v2,
                  QHash<QString,QVector<QString>>& result,
                  QHash<QString,QVector<QString>>& addedData){
    QString ans = "";
    result = v1;
    addedData.clear();

    for(auto j = v2.begin() ; j != v2.end() ; ++j){
        if(!result.contains(j.key())){
            result[j.key()] = j.value();
            addedData[j.key()] = j.value();
            if(ans != "")
                ans += "\n";
            ans += "新增词条：" + j.key() + " 内容：" + stringVectorToString(j.value(), ", ");
        }
        else{
            for(auto k = 0 ; k < j.value().size() ; ++k){
                if(!result[j.key()].contains(j.value()[k])){
                    result[j.key()].push_back(j.value()[k]);
                    addedData[j.key()].push_back(j.value()[k]);
                    if(ans != "")
                        ans += "\n";
                    ans += "\n将为：" + j.key() + " 添加：" + j.value()[k];
                }
            }
        }
    }

    return ans;
}




QString strCut(const QString& animeList)
{
    QString ans;
    int countSize = 0;
    for (int i = 0; i < animeList.size(); ++i)
    {
        QChar ch = animeList.at(i);
        int increment = (ch.isPrint() && ch.toLatin1() >= 0x0020 && ch.toLatin1() <= 0x007e) ? 1 : 2;

        if (countSize + increment > 18) break;

        countSize += increment;
        ans += ch;
    }

    return (countSize >= 18) ? ans + "..." : ans;
}


void loadData(QString fileName, QHash<QString, QHash<QString, QVector<QString>>>& hashOfSS_V) {
    hashOfSS_V.clear();
    QFile openfile(fileName);

    // 如果文件不存在，则创建文件
    if (!openfile.exists()) {
        if (!openfile.open(QFileDevice::WriteOnly)) {
            qDebug() << "无法创建文件：" << fileName;
            return; // 创建失败直接返回
        }
        QJsonObject emptyObject;   // 创建空的 QJsonObject 对象
        QJsonDocument emptyDoc(emptyObject);   // 使用空的 QJsonObject 创建 QJsonDocument 对象
        openfile.write(emptyDoc.toJson());
        openfile.close();
        qDebug() << "文件已创建：" << fileName;
    }

    // 读取文件
    if (!openfile.open(QFileDevice::ReadOnly)) {
        qDebug() << "读取文件失败：" << fileName;
        return;
    }

    QByteArray bitb = openfile.readAll();
    QJsonParseError parseError;
    QJsonDocument JsonDocument = QJsonDocument::fromJson(bitb, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "无法解析JSON：" << fileName;
        return;
    }

    QJsonObject temptest1 = JsonDocument.object();
    for (QJsonObject::iterator i = temptest1.begin(); i != temptest1.end(); ++i) {
        QJsonArray second = i.value().toArray();
        for (QJsonArray::iterator j = second.begin(); j != second.end(); ++j) {
            QJsonObject second = j->toObject();
            for (QJsonObject::iterator k = second.begin(); k != second.end(); ++k) {
                QJsonArray third = k.value().toArray();
                for (int q = 0; q < third.size(); ++q) {
                    hashOfSS_V[i.key()][k.key()].push_back(third[q].toString());
                }
            }
        }
    }
}

void saveData(QString fileName, QHash<QString, QHash<QString, QVector<QString>>>& hashOfSS_V) {
    QFile file(fileName);

    // 获取文件的目录路径
    QFileInfo fileInfo(fileName);
    QDir dir = fileInfo.dir();

    // 如果目录不存在，创建目录
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qWarning() << "创建路径失败:" << dir.absolutePath();
            return;
        }
    }

    if (!file.open(QFileDevice::WriteOnly)) {
        // setLogsInfo("写入文件失败：" + fileName);
        return;
    }

    QByteArray bitA;
    QJsonDocument JsonData;
    QJsonObject Sum;

    QHash<QString, QHash<QString, QVector<QString>>>::iterator i;
    for (i = hashOfSS_V.begin(); i != hashOfSS_V.end(); ++i) {
        QJsonObject a;
        QJsonArray d;

        QHash<QString, QVector<QString>>::iterator j;
        for (j = hashOfSS_V[i.key()].begin(); j != hashOfSS_V[i.key()].end(); ++j) {
            QJsonObject b;
            QJsonArray c;
            for (int k = 0; k < j.value().size(); ++k) {
                c.append(j.value()[k]);
            }
            b.insert(j.key(), c);
            d.append(b);
        }
        Sum.insert(i.key(), d);
    }

    JsonData.setObject(Sum);
    bitA = JsonData.toJson();
    file.write(bitA);
    file.close();
}
//  设置日志信息
void setLogsInfo(QString logs){
    QHash<QString,QHash<QString,QVector<QString>>> systemLogs;
    loadData("logs.json", systemLogs);
    QString today = getLocalTime();
    systemLogs["日志"][today].push_back(logs);
    saveData("logs.json", systemLogs);
}
