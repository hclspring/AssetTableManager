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
    QString rootPathStr = getRecordBookRootPath();
    QDir rootPath(rootPathStr);
    QString folder = rootPath.fromNativeSeparators(folder);
    if (rootPath.Readable == false) {
        qWarning("无法读取目录%s", rootPathStr.toStdString().c_str());
    }
    QStringList allFolders = QStringList("");
    rootPath.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    rootPath.setSorting(QDir::Name);
    allFolders = rootPath.entryList();
    recordBookNames.clear();
    for (QString temp: allFolders) {
        recordBookNames.push_back(temp);
        qDebug("发现目录%s", temp.toStdString().c_str());
    }
    return true;
}

QString Config::get_book_file_path(QString& bookName)
{
    return getRecordBookRootPath() + bookName + QDir::separator();
}

QString get_book_file_name(QString& bookName);
bool read_mapping_names();
QVector<QString> get_field_names(QString& bookName);
std::vector<enum FieldType> get_field_types(QString& bookName);
enum FieldType get_field_type(QString& bookName, QString& fieldName);
QString get_field_name(QString& bookName, enum FieldType fieldType);
