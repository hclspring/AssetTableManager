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
#include <QByteArray>
#include <QFile>
#include <QtDebug>
#include <QDebug>
#include <QTextCodec>
#include <memory>

#include "constant.h"


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
    Config(QString& newConfigFilePathName);

private:
    QString configFilePathName = "./config.txt";
    QString recordBookRootPath;
    QVector<QString> recordBookNames;

    //QString fieldMappingRootPath;
    //QVector<QString> fieldMappingNames;

    //以下是两层map，第一层是bookName -> 字段映射，第二层是该bookName下字段汉字名称 -> 字段枚举类型（或反过来）
    QMapPtrQMapS2F fieldMappingS2F;
    QMapPtrQMapF2S fieldMappingF2S;

    QMapString2Int mappingS2SheetIndex;
    QMapString2Int mappingS2ColumnNameRow;
    QMapString2Int mappingS2DataStartRow;

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

    // 根据台账文件完整路径判断台账类型
    QString get_bookName(const QString& bookFilePathName);

    //bool read_mapping_names();
    // 获取指定台账类型的字段
    QVector<QString> get_field_names(QString& bookName);
    std::vector<enum FieldType> get_field_types(QString& bookName);
    enum FieldType get_field_type(QString& bookName, QString& fieldName);
    QString get_field_name(QString& bookName, enum FieldType fieldType);

    // 获取指定台账类型的字段对应关系
    PtrQMapS2F get_ptr_mapS2F(QString& bookName);
    PtrQMapF2S get_ptr_mapF2S(QString& bookName);

    // 获取指定台账类型的表格参数（sheetIndex, columnNameRow, dataStartRow）
    int get_sheetIndex(QString& bookName);
    int get_columnNameRow(QString& bookName);
    int get_dataStartRow(QString& dataStartRow);


private:
    bool read_subdirs(const QString& rootPathStr, QVector<QString>& result, enum QDir::Filter filter);
    // get_map_value_index: 获取key为string、value为非负整数的value，如果key不存在则返回-1
    int get_map_value_index(QMapString2Int& map, QString& key);
    // 读取配置json文件里的索引型数据，并插入到map里
    void parse_config_index(QJsonObject& object, QMapString2Int& map, const QString& key, QString& bookName);
    void parse_config_string(QJsonObject& object, const QString& key, QString& value);

};

#endif // CONFIG_H
