#include "config.h"

Config::Config()
{
    //constructMapFieldType();
    parse_config_file_v2();
}

Config::Config(QString& newConfigFilePathName)
{
    //constructMapFieldType();
    setConfigFilePathName(newConfigFilePathName);
    parse_config_file_v2();
}

const QString &Config::getConfigFilePathName() const
{
    return configFilePathName;
}

void Config::setConfigFilePathName(const QString &newConfigFileName)
{
    configFilePathName = newConfigFileName.trimmed();
}

const QString &Config::getRecordBookRootPath() const
{
    return recordBookRootPath;
}

void Config::setRecordBookRootPath(const QString &newRecordBookRootPath)
{
    recordBookRootPath = (QDir::toNativeSeparators(newRecordBookRootPath.trimmed()) + QDir::separator()).trimmed();
}

QVector<QString> Config::getExportBookTypes()
{
    return  exportBookTypes;
}

QVector<QString> Config::getPrimaryKeyColumnNames()
{
    return primaryKeyColumnNames;
}

std::shared_ptr<QMapStr2Str> Config::getMappingImportSource2Target()
{
    return std::make_shared<QMapStr2Str>(mappingImportSource2Target);
}

std::shared_ptr<QMapStr2Str> Config::getMappingExportTarget2Source()
{
    return std::make_shared<QMapStr2Str>(mappingExportTarget2Source);
}


bool Config::parse_config_file_v2()
{
    if (getConfigFilePathName().isEmpty()) {
        qWarning("配置文件路径和名称为空。");
        return false;
    }
    QFile file(getConfigFilePathName());
    if (!file.open(QIODevice::ReadOnly)) {
        qCritical("无法打开文件%s", getConfigFilePathName().toStdString().c_str());
        return false;
    }
    QString cc = file.readAll();
    file.close();

    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(cc.toUtf8(), &jsonError);

    if (document.isNull() || (jsonError.error != QJsonParseError::NoError)) {
        qCritical("解析文件%s出现错误！", getConfigFilePathName().toStdString().c_str());
        return false;
    }

    if (document.isObject()) {
        QJsonObject object = document.object();
        if (object.contains("recordBookRootPath")) {
            qDebug()<<"读取recordBookRootPath";
            QJsonValue value = object.value("recordBookRootPath");
            if (value.type() == QJsonValue::String) {
                setRecordBookRootPath(value.toString());
            } else {
                qWarning("台账存放路径为空。");
            }
        }
        if (object.contains("exportSettings")) {
            qDebug()<<"读取exportSettings";
            QJsonValue value1 = object.value("exportSettings");
            if (value1.type() != QJsonValue::Array) {
                qWarning("exportSettings的值应当是列表。");
            } else {
                qDebug() << "开始解析exportSettings";
                QJsonArray array = value1.toArray();
                int arraySize = array.size();

                this->exportBookTypes.clear();
                this->mappingS2SheetIndex.clear();
                this->mappingS2ColumnNameRow.clear();
                this->mappingS2DataStartRow.clear();
                this->mappingExportBookType2ColumnNames.clear();

                for (int i = 0; i < arraySize; ++i) {
                    qDebug() << "开始解析第" << i << "个元素";
                    QJsonObject object2 = array[i].toObject();

                    if (object2.contains("exportBookType")) {
                        QString newBookType;
                        parse_config_string(object2, "exportBookType", newBookType);
                        qDebug() << "解析到exportBookType = " << newBookType;
                        if (newBookType.length() > 0) {
                            exportBookTypes.push_back(newBookType);
                        }
                        parse_config_index(object2, mappingS2SheetIndex, "sheetIndex", newBookType);
                        parse_config_index(object2, mappingS2ColumnNameRow, "columnNameRow", newBookType);
                        parse_config_index(object2, mappingS2DataStartRow, "dataStartRow", newBookType);
                        QVecString columnNames;
                        parse_config_vecstr(object2, "columnNames", columnNames);
                        mappingExportBookType2ColumnNames.insert(newBookType, std::make_shared<QVecString>(columnNames));
                    } //如果该项目用于提供输出表格字段
                    else if (object2.contains("exportColumnDefinition")) {
                        qDebug() << "开始解析exportColumnDefinition";
                        QJsonValue value2 = object2.value("exportColumnDefinition");
                        if (value2.type() == QJsonValue::Array) {
                            QJsonArray array2 = value2.toArray();
                            for (int j = 0; j < array2.size(); ++j) {
                                QJsonObject object3 = array2[j].toObject();
                                QString targetStr, sourceStr;
                                parse_config_string(object3, "target", targetStr);
                                parse_config_string(object3, "source", sourceStr);
                                qDebug() << "解析到第" << j << "对字段对，source = " << sourceStr << ", target = " << targetStr;
                                mappingExportTarget2Source.insert(targetStr, sourceStr);
                            }
                        } else {
                            qWarning() << "exportColumnDefinition的值应当是列表。";
                        }
                    } else {
                        qWarning() << "未发现已知关键词exportBookType或exportColumnDefinition";
                    }

                } // end for iterating the array of bookSettings
            }
            qDebug()<< "读取exportSettings结束。";
        } else  {
            qWarning() << "未发现已知关键词exportSettings";
        }
        if (object.contains("importSettings")) {
            qDebug() << "读取importSettings";
            QJsonObject object2 = object.value("importSettings").toObject();
            if (object2.contains("importColumnDefinition")) {
                qDebug() << "开始解析importColumnDefinition";
                QJsonValue value = object2.value("importColumnDefinition");
                if (value.type() == QJsonValue::Array) {
                    QJsonArray array = value.toArray();
                    for (int i = 0; i < array.size(); ++i) {
                        QJsonObject object3 = array[i].toObject();
                        QString targetStr, sourceStr;
                        parse_config_string(object3, "target", targetStr);
                        parse_config_string(object3, "source", sourceStr);
                        //mappingImportTarget2Source.insert(targetStr, sourceStr);
                        mappingImportSource2Target.insert(sourceStr, targetStr);
                    }
                } else {
                    qWarning() << "importColumnDefinition的值应当是列表。";
                }
            } else {
                qWarning() << "未发现已知关键词importColumnDefinition";
            }
        } else {
            qWarning() << "未发现已知关键词importSettings";
        }
        if (object.contains("primaryKeyColumns")) {
            primaryKeyColumnNames.clear();
            parse_config_vecstr(object, "primaryKeyColumns", primaryKeyColumnNames);
        } else {
            qWarning() << "未发现已知关键词primaryKeyColumns";
        }
    }

}


bool Config::read_subdirs(const QString& rootPathStr, QVector<QString>& result, enum QDir::Filter filter)
{
    QString rootPathStr2 = (QDir::toNativeSeparators(rootPathStr.trimmed()) + QDir::separator()).trimmed();
    QDir rootPath(rootPathStr2);
    result.clear();
    if (rootPath.Readable == false) {
        qWarning("无法读取目录%s", rootPathStr.toStdString().c_str());
        return false;
    }
    QStringList allFolders = QStringList("");
    //rootPath.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    rootPath.setFilter(filter | QDir::NoDotAndDotDot);
    rootPath.setSorting(QDir::Name);
    allFolders = rootPath.entryList();
    for (QString temp: allFolders) {
        result.push_back(temp);
        qDebug("发现目录%s", temp.toStdString().c_str());
    }
    return true;
}

int Config::get_sheetIndex(QString& bookName)
{
    return get_map_value_index(mappingS2SheetIndex, bookName);
}

int Config::get_columnNameRow(QString& bookName)
{
    return get_map_value_index(mappingS2ColumnNameRow, bookName);
}

int Config::get_dataStartRow(QString& bookName)
{
    return get_map_value_index(mappingS2DataStartRow, bookName);
}


bool Config::readImportColumnNameNeedConvert(QString& columnName)
{
    QMapStr2Str::iterator it = mappingImportSource2Target.find(columnName.trimmed());
    if (it == mappingImportSource2Target.end()) {
        return false;
    } else {
        return  true;
    }
}

QString Config::getConvertedImportColumnName(QString& columnName)
{
    QMapStr2Str::iterator it = mappingImportSource2Target.find(columnName.trimmed());
    assert(it != mappingImportSource2Target.end());
    return it.value().trimmed();
}

std::shared_ptr<QVector<QString>> Config::getExportColumnNames(QString& exportBookType)
{
    QMapStr2Vec::iterator it = mappingExportBookType2ColumnNames.find(exportBookType.trimmed());
    if (it != mappingExportBookType2ColumnNames.end()) {
        return it.value();
    } else {
        qWarning() << "未识别到合法的输出台账类型，要求的类型为" << exportBookType;
        return nullptr;
    }
}

int Config::get_map_value_index(QMapString2Int& map, QString& key)
{
    QMapString2Int::iterator it = map.find(key.trimmed());
    if (it != map.end()) {
        return it.value();
    } else {
        return -1;
    }
}

void Config::parse_config_index(QJsonObject& object2, QMapString2Int& map, const QString& key, QString& bookType)
{
    if (object2.contains(key)) {
        QJsonValue value2 = object2.value(key.trimmed());
        if (value2.type() == QJsonValue::Double) {
            int indexValue = value2.toInt();
            if (bookType.length() > 0) {
                map.insert(bookType, indexValue);
            } else {
                qWarning() << "识别到合法的" << key << " = " << indexValue << "，但没有合法的台账名";
            }
        } else {
            qWarning() << "未能识别合法的" << key;
        }
    } else {
        qWarning() << "未能识别到" << key << "的key";
    }
}

void Config::parse_config_string(QJsonObject& object, const QString& key, QString& value)
{
    if (object.contains(key)) {
        QJsonValue value3 = object.value(key);
        if (value3.type() == QJsonValue::String) {
            value = value3.toString().trimmed();
            qDebug() << "识别到新的" << key << ": " << value;
        } else {
            qWarning() << "未能识别到合法的" << key;
        }
    } else {
        qWarning() << "未能识别到" << key << "的key";
    }
}

void Config::parse_config_vecstr(QJsonObject& object, const QString& key, QVector<QString>& value)
{
    if (object.contains(key)) {
        QJsonValue value3 = object.value(key);
        if (value3.type() == QJsonValue::Array) {
            value.clear();
            for (int i = 0; i < value3.toArray().size(); ++i) {
                value.push_back(value3.toArray().at(i).toString().trimmed());
            }
        } else  {
            qWarning() << "未能识别到合法的" << key;
        }
    } else {
        qWarning() << "未能识别到" << key << "的key";
    }
}
