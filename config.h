#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QDir>
#include <QDirIterator>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QtDebug>
#include <QDebug>
#include <memory>

#include "constant.h"


typedef QMap<QString, enum FieldType> QMapString2Field;
typedef std::shared_ptr<QMapString2Field> PtrQMapS2F;
typedef QMap<QString, PtrQMapS2F> QMapPtrQMapS2F;
typedef QMap<enum FieldType, QString> QMapField2String;
typedef std::shared_ptr<QMapField2String> PtrQMapF2S;
typedef QMap<QString, PtrQMapF2S> QMapPtrQMapF2S;

/*
 * 使用json文件保存相关配置
 * 示例如下：
{
"recordBookRootPath": "xxx",
"bookSettings": [
    {"bookName": "固定资产台账",
    "__fieldMapping": "字段映射，给出每种台账的字段映射关系，即字段的type到name",
    "fieldMapping": [
    {"fieldType":"zichan", "fieldName":"资产"},
    {"fieldType":"xxx", "fieldName":"xxx"}
    ]},
    {"bookName": "无形资产台账",
    "__fieldMapping": "字段映射，给出每种台账的字段映射关系，即字段的type到name",
    "fieldMapping": [
    {"fieldType":"zichan", "fieldName":"资产"},
    {"fieldType":"xxx", "fieldName":"xxx"}
    ]},
    {blabla....}
]
}
 */

class Config
{

public:
    Config();

private:
    QString configFilePathName;
    QString recordBookRootPath;
    QVector<QString> recordBookNames;

    //QString fieldMappingRootPath;
    //QVector<QString> fieldMappingNames;

    QMapPtrQMapS2F fieldMappingS2F;
    QMapPtrQMapF2S fieldMappingF2S;

public:
    // getter and setter functions
    const QString &getConfigFilePathName() const;
    void setConfigFilePathName(const QString &newConfigFileName);
    const QString &getRecordBookRootPath() const;
    void setRecordBookRootPath(const QString &newRecordBookRootPath);
    const QVector<QString> &getRecordBookNames() const;
    void setRecordBookNames(const QVector<QString> &newRecordBookNames);
//    const QString &getFieldMappingRootPath() const;
//    void setFieldMappingRootPath(const QString &newFieldMappingRootPath);
//    const QVector<QString> &getFieldMappingNames() const;
//    void setFieldMappingNames(const QVector<QString> &newFieldMappingNames);

    // 解析配置文件
    bool parse_config_file();

    //bool read_book_names();
    // 获取台账文件路径
    // file_path: 指文件所在路径或目录，不包括该文件本身
    // file_path_name: 指文件完整路径，不仅仅是该文件的文件名
    QString get_book_file_path(QString& bookName);
    QString get_book_file_path_name(QString& bookName);
    //bool read_mapping_names();
    // 获取指定台账类型的字段
    QVector<QString> get_field_names(QString& bookName);
    std::vector<enum FieldType> get_field_types(QString& bookName);
    enum FieldType get_field_type(QString& bookName, QString& fieldName);
    QString get_field_name(QString& bookName, enum FieldType fieldType);

private:
    bool read_subdirs(const QString& rootPathStr, QVector<QString>& result, enum QDir::Filter filter);

};

#endif // CONFIG_H
