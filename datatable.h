#ifndef DATATABLE_H
#define DATATABLE_H

#include <QString>
#include <QVector>
#include <QSet>
#include <QDebug>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "xlsxdocument.h"

#include "constant.h"


class DataTable
{
public:
    DataTable();
    ~DataTable();

private:
    // data指表格台账数据，第一维为数据行，第二维为字段列
    std::shared_ptr<QVector<QVector<QString>>> data;
    // fields指表格各列含义，与data第二维的维度一致
    std::shared_ptr<QVector<enum FieldType>> fields;
    // field2column指根据field获取列序号
    std::unordered_map<enum FieldType, int> field2column;

    // maxRow指excel文件的最大行数，maxCol指excel文件的最大列数
    int maxRow, maxCol;
    QVector<QString> excelColumnNames;

public:
    //一、对成员变量的set/get函数
    void set(QVector<QVector<QString>>& data);
    void set(std::vector<int>& rowIndices, DataTable& referenceDataTable);
    std::shared_ptr<QVector<QVector<QString>>> get_data();
    void set(QVector<enum FieldType>& fields);
    std::shared_ptr<QVector<enum FieldType>> get_fields();

    //二、根据已知信息判断行序号或列序号
    // get_column_index: 能找到，返回序号，否则返回-1
    int get_column_index(enum FieldType field);
    //int get_column_index(QString& fieldName);
    // get_column_indices：如果所有字段都能找到，返回true；如果出现找不到的字段，根据retainUnfoundColumns，若为false则返回false，若为true则在参数中对应位置填写-1
    bool get_column_indices(QVector<enum FieldType>& fields, std::vector<int>& outputColumnIndices, bool retainUnfoundColumns);
    //bool get_column_indices(QVector<QString>& fieldNames, std::vector<int>& outputColumnIndices);

    // get_row_index: 返回指定列为指定值的行，如果行数超过1，则返回数量的相反数，如果行数等于0，则返回-1
    // 这里做字符串比较时，大小写不敏感，如有需要可在设置里提供自定义大小写敏感判断
    //int get_row_index(QString& fieldName, QString& fieldValue);
    int get_row_index(enum FieldType field, QString& fieldValue);

    // get_row_indices: 返回指定列为指定值的所有行序号，如果指定列不存在则返回空vector
    //std::vector<int> get_row_indices(QString& fieldName, QString& fieldValue);
    std::vector<int> get_row_indices(enum FieldType field, QString& fieldValue);

    // get_unique_row_indices: 返回指定列为指定值的所有行序号，如果某个值的行数超过1，则对应行序号为行数的相反数
    //std::vector<int> get_unique_row_indices(QString& fieldName, std::unordered_set<QString>& fieldValues);
    std::vector<int> get_unique_row_indices(enum FieldType field, QSet<QString>& fieldValues);

    // get_multiple_row_indices: 返回指定列为指定值的所有行序号，如果某个值的行数超过1，则所有对应行序号都会返回
    //std::vector<int> get_multiple_row_indices(QString& fieldName, std::unordered_set<QString>& fieldValues);
    std::vector<int> get_multiple_row_indices(enum FieldType field, QSet<QString>& fieldValues);

    //三、获取数据：如果ignoreUnfoundColumn为true，则没有对应字段时返回""，否则报错退出
    bool check_row_index(int rowIndex);
    bool check_row_column_index(int rowIndex, int columnIndex);
    QString get_cell_value(int rowIndex, int columnIndex, bool ignoreUnfoundColumn);
    QString get_cell_value(int rowIndex, enum FieldType field, bool ignoreUnfoundColumn);

    //四、格式化数据：如果columnIndex或referenceField不合法，则对应字段填写空字符串（即""）
    std::shared_ptr<QVector<QString>> get_formatted_row(int rowIndex, std::vector<int>& referenceColumnIndices);
    std::shared_ptr<QVector<QString>> get_formatted_row(int rowIndex, QVector<enum FieldType>& referenceFields);
    std::shared_ptr<QVector<QVector<QString>>> get_formatted_row(std::vector<int>& rowIndices, std::vector<int>& referenceColumnIndices);
    std::shared_ptr<QVector<QVector<QString>>> get_formatted_data(std::vector<int>& rowIndices, QVector<enum FieldType>& referenceFields);

    //四、更新数据
    // 对数据进行操作，成功返回true，失败返回false
    bool add_row(QVector<QString>& dataRow);
    bool update_cell(int rowIndex, int columnIndex, QString& value);
    bool update_cell(int rowIndex, enum FieldType field, QString& value);


    //五、读取表格文件
    void readExcelData(QXlsx::Document& assetDocument, const QString& sheetName, int columnNameRow = 1, int dataStartRow = 2);
    void readExcelData(QXlsx::Worksheet* worksheet, int dataStartRow = 2);
    void readExcelData(const QString& filename, int sheetIndex = 0, int columnNameRow = 1, int dataStartRow = 2);
    void readExcelColumnNames(QXlsx::Worksheet* worksheet, int columnNameRow = 1);


};

#endif // DATATABLE_H
