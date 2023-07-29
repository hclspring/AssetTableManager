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
"recordBookRootPath": "D:\\QtProjects\\示例3",
"__exportSettings": "设置导出的相关属性，例如字段列表和顺序",
"exportSettings": [
    {"exportBookType": "固定资产台账",
    "sheetIndex": 0,
    "columnNameRow": 1,
    "dataStartRow": 2,
    "__columnNames": "导出台账时的指定字段，而不是该台账只能识别的字段",
    "columnNames": [
        "卡片编号",
        "实物资产编号",
        "在建资产编号",
        "设备名称",
        "资产名称",
        "资产品牌",
        "规格型号",
        "序列号",
        "存放地点",
        "...",
        "备注"
    ]},
    {"exportBookType": "无形资产台账",
    "sheetIndex": 0,
    "columnNameRow": 1,
    "dataStartRow": 2,
    "__columnNames": "导出台账时的指定字段，而不是该台账只能识别的字段",
    "columnNames": [
        "卡片编号",
        "无形资产编号",
        "在建资产编号",
        "资产名称",
        "存放地点",
        "...",
        "备注"
    ]},
    {"exportColumnDefinition":[
        {"target": "存放地点", "source": "所在机房"},
        {"target": "房间内部位置", "source": "所在机柜&所在U数"}
    ]}
    ],
"__importSettings": "设置导入的相关属性，具体内容待定",
"importSettings": {
    "__importColumnDefinition": "import和update（尤其是update）较为依赖此对应关系，目前仅支持单列数据的字段名转化，不支持多列数据字段名合并和分割",
    "importColumnDefinition": [
        {"target": "实物资产编号", "source": "资产编号"}
    ]
    },
"primaryKeyColumns": [
        "卡片编号",
        "实物资产编号",
        "无形资产编号",
        "在建资产编号",
        "序列号"
    ]
}
 */

class Config
{

public:
    Config();
    Config(QString& newConfigFilePathName);

private:
    QString configFilePathName = "./config.v2.txt";
    QString recordBookRootPath;
    QVector<QString> exportBookTypes;
    QVector<QString> primaryKeyColumnNames;

    QMapString2Int mappingS2SheetIndex;
    QMapString2Int mappingS2ColumnNameRow;
    QMapString2Int mappingS2DataStartRow;
    QMapStr2Vec mappingExportBookType2ColumnNames;
    QMapStr2Str mappingExportTarget2Source;
    QMapStr2Str mappingImportTarget2Source;
    QMapStr2Str mappingImportSource2Target;

public:
    // getter and setter functions
    const QString &getConfigFilePathName() const;
    void setConfigFilePathName(const QString &newConfigFileName);
    const QString &getRecordBookRootPath() const;
    void setRecordBookRootPath(const QString &newRecordBookRootPath);

    QVector<QString> getExportBookTypes();
    QVector<QString> getPrimaryKeyColumnNames();

    std::shared_ptr<QMapStr2Str> getMappingImportSource2Target();
    std::shared_ptr<QMapStr2Str> getMappingExportTarget2Source();


    // 解析配置文件
    bool parse_config_file_v2();

    // 获取指定台账类型的表格参数（sheetIndex, columnNameRow, dataStartRow）
    int get_sheetIndex(QString& exportBookType);
    int get_columnNameRow(QString& exportBookType);
    int get_dataStartRow(QString& exportBookType);

    // mapping变量相关存取函数
    bool readImportColumnNameNeedConvert(QString& columnName);
    QString getConvertedImportColumnName(QString& columnName);
    std::shared_ptr<QVector<QString>> getExportColumnNames(QString& exportBookType);

private:
    bool read_subdirs(const QString& rootPathStr, QVector<QString>& result, enum QDir::Filter filter);
    int get_map_value_index(QMapString2Int& map, QString& key);
    // 读取配置json文件里的索引型数据，并插入到map里
    void parse_config_index(QJsonObject& object, QMapString2Int& map, const QString& key, QString& bookType);
    void parse_config_string(QJsonObject& object, const QString& key, QString& value);
    void parse_config_vecstr(QJsonObject& object, const QString& key, QVector<QString>& value);




    /*
     * 以下成员函数预计后续用不到，待删除
     *
private:
    const QVector<QString> &getRecordBookNames() const;
    void setRecordBookNames(const QVector<QString> &newRecordBookNames);

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
*/



};

#endif // CONFIG_H
