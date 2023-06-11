#include "config.h"

Config::Config()
{

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
                                    ;//TODO：从fieldTypeStr获取fieldType
                                } else {
                                    qWarning("fieldType或fieldName长度为0（fieldType: %s；fieldName: %s）。", fieldTypeStr.toStdString().c_str(), fieldNameStr.toStdString().c_str());
                                }
                            } // end for iterating the array of fieldMapping
                        } else {
                            qWarning("fieldMapping的值应当是列表。");
                        }
                    } // end for reading key named "fieldMapping"
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

QVector<QString> get_field_names(QString& bookName);
std::vector<enum FieldType> get_field_types(QString& bookName);
enum FieldType get_field_type(QString& bookName, QString& fieldName);
QString get_field_name(QString& bookName, enum FieldType fieldType);


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

