#include "config.h"

Config::Config()
{
    parse_config_file();
}

Config::Config(QString& newConfigFilePathName)
{
    setConfigFilePathName(newConfigFilePathName);
    parse_config_file();
}

const QString &Config::getConfigFilePathName() const
{
    return configFilePathName;
}

void Config::setConfigFilePathName(const QString &newConfigFileName)
{
    configFilePathName = newConfigFileName;
}

const QString &Config::getRecordBookRootPath() const
{
    return recordBookRootPath;
}

void Config::setRecordBookRootPath(const QString &newRecordBookRootPath)
{
    recordBookRootPath = QDir::toNativeSeparators(newRecordBookRootPath) + QDir::separator();
}

const QVector<QString> &Config::getRecordBookNames() const
{
    return recordBookNames;
}

void Config::setRecordBookNames(const QVector<QString> &newRecordBookNames)
{
    recordBookNames = newRecordBookNames;
}

/*
const QString &Config::getFieldMappingRootPath() const
{
    return fieldMappingRootPath;
}

void Config::setFieldMappingRootPath(const QString &newFieldMappingRootPath)
{
    fieldMappingRootPath = QDir::toNativeSeparators(newFieldMappingRootPath) + QDir::separator();
}

const QVector<QString> &Config::getFieldMappingNames() const
{
    return fieldMappingNames;
}

void Config::setFieldMappingNames(const QVector<QString> &newFieldMappingNames)
{
    fieldMappingNames = newFieldMappingNames;
}
*/

/*
bool Config::read_book_names()
{
    return read_subdirs(getRecordBookRootPath(), recordBookNames, QDir::Dirs);
}
*/

bool Config::parse_config_file()
{
    if (getConfigFilePathName().isEmpty()) {
        qWarning("配置文件路径和名称为空。");
        return false;
    }
    QFile file(getConfigFilePathName());
    if (!file.open(QFile::ReadOnly)) {
        qCritical("无法打开文件%s", getConfigFilePathName().toStdString().c_str());
        return false;
    }
    QByteArray qba = file.readAll();
    file.close();

    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(qba, &jsonError);
    if (document.isNull() || (jsonError.error != QJsonParseError::NoError)) {
        qCritical("解析文件%s出现错误！", getConfigFilePathName().toStdString().c_str());
        return false;
    }

    if (document.isObject()) {
        QJsonObject object = document.object();
        if (object.contains("recordBookRootPath")) {
            QJsonValue value = object.value("recordBookRootPath");
            if (value.type() == QJsonValue::String) {
                setRecordBookRootPath(value.toString());
            } else {
                qWarning("台账存放路径为空。");
            }
        }
        if (object.contains("bookSettings")) {
            QJsonValue value1 = object.value("bookSettings");
            if (value1.type() != QJsonValue::Array) {
                qWarning("bookSettings的值应当是列表。");
            } else {
                QJsonArray array = value1.toArray();
                int arraySize = array.size();
                this->recordBookNames.clear();
                this->fieldMappingF2S.clear();
                this->fieldMappingS2F.clear();
                for (int i = 0; i < arraySize; ++i) {
                    QJsonObject object2 = array[i].toObject();
                    QString newBookName;
                    QMapString2Field mapS2F;
                    QMapField2String mapF2S;
                    if (object2.contains("bookName")) {
                        QJsonValue value2 = object2.value("bookName");
                        if (value2.type() == QJsonValue::String) {
                            newBookName = value2.toString();
                            recordBookNames.push_back(newBookName);
                        } else {
                            qWarning("未能识别合法的bookName。");
                        }
                    } else {
                        qWarning("未能识别到bookName的key。");
                    } // end for reading key named "bookName"
                    if (object2.contains("fieldMapping")) {
                        QJsonValue value2 = object2.value("fieldMapping");
                        if (value2.type() != QJsonValue::Array) {
                            QJsonArray array2 = value2.toArray();
                            for (int j = 0; j < array2.size(); ++j) {
                                QJsonObject object3 = array2[j].toObject();
                                QString fieldTypeStr, fieldNameStr;
                                if (object3.contains("fieldType")) {
                                    QJsonValue value3 = object3.value("fieldType");
                                    if (value3.type() == QJsonValue::String) {
                                        fieldTypeStr = value3.toString();
                                    } else {
                                        qWarning("未能识别到合法的fieldType。");
                                    }
                                }
                                if (object3.contains("fieldName")) {
                                    QJsonValue value3 = object3.value("fieldName");
                                    if (value3.type() == QJsonValue::String) {
                                        fieldNameStr = value3.toString();
                                    } else {
                                        qWarning("未能识别到合法的fieldName。");
                                    }
                                }
                                if (fieldTypeStr.length() > 0 && fieldNameStr.length() > 0) {
                                    enum FieldType fieldType = getFieldType(fieldTypeStr);//TODO：从fieldTypeStr获取fieldType
                                    mapS2F.insert(fieldNameStr, fieldType);
                                    mapF2S.insert(fieldType, fieldNameStr);
                                } else {
                                    qWarning("fieldType或fieldName长度为0（fieldType: %s；fieldName: %s）。", fieldTypeStr.toStdString().c_str(), fieldNameStr.toStdString().c_str());
                                }
                            } // end for iterating the array of fieldMapping
                        } else {
                            qWarning("fieldMapping的值应当是列表。");
                        }
                    } else {
                        qWarning("未能识别到fieldMapping的key。");
                    } // end for reading key named "fieldMapping"
                    if (newBookName.length() > 0 && mapF2S.size() > 0 && mapS2F.size() > 0) {
                        fieldMappingF2S.insert(newBookName, std::make_shared<QMapField2String>(mapF2S));
                        fieldMappingS2F.insert(newBookName, std::make_shared<QMapString2Field>(mapS2F));
                    }
                } // end for iterating the array of bookSettings
            }
        } // end for reading key named "bookSettings"
    }
}

QString Config::get_book_file_path(QString& bookName)
{
    return getRecordBookRootPath() + bookName + QDir::separator();
}

QString Config::get_book_file_path_name(QString& bookName)
{
    return get_book_file_path(bookName) + bookName;
}

/*
bool Config::read_mapping_names()
{
    return read_subdirs(getFieldMappingRootPath(), fieldMappingNames, QDir::Files);
}
*/

QVector<QString> Config::get_field_names(QString& bookName)
{
    QVector<QString> result;
    if (fieldMappingS2F.find(bookName) != fieldMappingS2F.end()) {
        PtrQMapS2F s2f = fieldMappingS2F.find(bookName).value();
        for (QMapString2Field::iterator it = s2f->begin(); it != s2f->end(); ++it) {
            result.push_back(it.key());
        }
    } else {
        qWarning("在字段表中未找到合法的台账名。");
    }
    return result;
}

std::vector<enum FieldType> Config::get_field_types(QString& bookName)
{
    std::vector<enum FieldType> result;
    if (fieldMappingF2S.find(bookName) != fieldMappingF2S.end()) {
        PtrQMapF2S f2s = fieldMappingF2S.find(bookName).value();
        for (QMapField2String::iterator it = f2s->begin(); it != f2s->end(); ++it) {
            result.push_back(it.key());
        }
    } else {
        qWarning("在字段表中未找到合法的台账名。");
    }
    return result;
}

enum FieldType Config::get_field_type(QString& bookName, QString& fieldName)
{
    if (fieldMappingS2F.find(bookName) != fieldMappingS2F.end()) {
        PtrQMapS2F s2f = fieldMappingS2F.find(bookName).value();
        if (s2f->find(fieldName) != s2f->end()) {
            return s2f->find(fieldName).value();
        } else {
            qWarning("在字段表中未找到对应的字段名。");
        }
    } else {
        qWarning("在字段表中未找到合法的台账名。");
    }
    return FieldType::COUNT;
}

QString Config::get_field_name(QString& bookName, enum FieldType fieldType)
{
    if (fieldMappingF2S.find(bookName) != fieldMappingF2S.end()) {
        PtrQMapF2S f2s = fieldMappingF2S.find(bookName).value();
        if (f2s->find(fieldType) != f2s->end()) {
            return f2s->find(fieldType).value();
        } else {
            qWarning("在字段表中未找到对应的字段。");
        }
    } else {
        qWarning("在字段表中未找到合法的台账名。");
    }
    return getFieldTypeStr(FieldType::COUNT);
}


bool Config::read_subdirs(const QString& rootPathStr, QVector<QString>& result, enum QDir::Filter filter)
{
    QString rootPathStr2 = QDir::toNativeSeparators(rootPathStr) + QDir::separator();
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

