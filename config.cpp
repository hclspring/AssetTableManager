#include "config.h"

Config::Config()
{

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


bool Config::read_book_names()
{
    return read_subdirs(getRecordBookRootPath(), recordBookNames, QDir::Dirs);
}

QString Config::get_book_file_path(QString& bookName)
{
    return getRecordBookRootPath() + bookName + QDir::separator();
}

QString Config::get_book_file_name(QString& bookName)
{
    return get_book_file_path(bookName) + bookName;
}

bool Config::read_mapping_names()
{
    return read_subdirs(getFieldMappingRootPath(), fieldMappingNames, QDir::Files);
}

QVector<QString> Config::get_field_names(QString& mappingName)
std::vector<enum FieldType> get_field_types(QString& mappingName);
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

