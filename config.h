#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QDir>
#include <QDirIterator>
#include <QSettings>
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
"__recordBookNames": "台账名称，不包括更新日期、后缀等信息",
"recordBookNames": [
"固定资产台账",
"无形资产台账",
"xxx"
],
"__fieldMapping": "字段映射，给出每种台账的字段映射关系，即字段的type到name“，
"fieldMapping": [
    "固定资产台账": [
    {"fieldType":"zichan", "fieldName":"资产"},
    {"fieldType":"xxx", "fieldName":"xxx"}
    ],
    "无形资产台账": [
    {"xxx":"xxx", "xxx":"xxx"}
    ]
]
}
 */

class Config
{

public:
    Config();

private:
    QString recordBookRootPath;
    QVector<QString> recordBookNames;

    QString fieldMappingRootPath;
    QVector<QString> fieldMappingNames;

    QMapPtrQMapS2F fieldMappingS2F;
    QMapPtrQMapF2S fieldMappingF2S;

public:
    // getter and setter functions
    const QString &getRecordBookRootPath() const;
    void setRecordBookRootPath(const QString &newRecordBookRootPath);
    const QVector<QString> &getRecordBookNames() const;
    void setRecordBookNames(const QVector<QString> &newRecordBookNames);
    const QString &getFieldMappingRootPath() const;
    void setFieldMappingRootPath(const QString &newFieldMappingRootPath);
    const QVector<QString> &getFieldMappingNames() const;
    void setFieldMappingNames(const QVector<QString> &newFieldMappingNames);

    bool read_book_names();
    QString get_book_file_path(QString& bookName);
    QString get_book_file_name(QString& bookName);
    bool read_mapping_names();
    QVector<QString> get_field_names(QString& mappingName);
    std::vector<enum FieldType> get_field_types(QString& mappingName);
    enum FieldType get_field_type(QString& bookName, QString& fieldName);
    QString get_field_name(QString& bookName, enum FieldType fieldType);

private:
    bool read_subdirs(const QString& rootPathStr, QVector<QString>& result, enum QDir::Filter filter);

};

#endif // CONFIG_H
