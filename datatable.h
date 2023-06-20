#ifndef DATATABLE_H
#define DATATABLE_H

#include <QString>
#include <QVector>
#include <QSet>
#include <QDebug>
#include <QTextStream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>

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
    // fields指表格各列含义，与data第二维的维度一致（存在unkown type的可能）
    std::shared_ptr<QVector<enum FieldType>> fields;
    // mapField2ColumnIndex用来根据field获取列序号
    std::unordered_map<enum FieldType, int> mapField2ColumnIndex;
    // unknownFieldColumn存放FieldType==UNKNOWN的列
    std::shared_ptr<std::vector<int>> unknownFieldColumns;
    // columnNames存放所有的列名（包括未能识别出含义的列名）
    std::shared_ptr<QVector<QString>> columnNames;
    // mapName2ColumnIndex用来根据列名获取列序号
    QMapString2Int mapName2ColumnIndex;

    // maxRow指excel文件的最大行数，maxCol指excel文件的最大列数
    int maxRow, maxCol;

public:
    //一、对成员变量的set/get函数
    void set_data(QVector<QVector<QString>>& data);
    //void set(std::vector<int>& rowIndices, DataTable& referenceDataTable);
    std::shared_ptr<QVector<QVector<QString>>> get_data();
    void set_fields(QVector<enum FieldType>& fields);
    std::shared_ptr<QVector<enum FieldType>> get_fields();
    FieldType get_field(int columnIndex);//若不合法，则返回FieldType::COUNT
    void set_unknownFieldColumns(std::vector<int>& unknownFieldColumns);
    std::shared_ptr<std::vector<int>> get_unknownFieldColumns();
    void set_columnNames(QVector<QString>& columnNames);
    std::shared_ptr<QVector<QString>> get_columnNames();
    QString get_columnName(int columnIndex);//若不合法，则返回NULL

    //二、根据已知信息判断行序号或列序号
    // get_column_index: 能找到，返回序号，否则返回-1
    int get_column_index(enum FieldType field);
    int get_column_index(const QString& columnName);
    // get_column_indices：如果所有字段都能找到，返回true；如果出现找不到的字段，根据retainUnfoundColumns，若为false则返回false，若为true则在参数中对应位置填写-1
    bool get_column_indices(QVector<enum FieldType>& fields, std::vector<int>& outputColumnIndices, bool retainUnfoundColumns);
    bool get_column_indices(QVector<QString>& columnNames, std::vector<int>& outputColumnIndices, bool retainUnfoundColumns);

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
    bool check_column_index(int columnIndex);
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
    bool update_cell(int rowIndex, int columnIndex, const QString& value);
    bool update_cell(int rowIndex, enum FieldType field, const QString& value);
    bool update_cell(int rowIndex, const QString& columnName, const QString& value);


    //五、读取表格文件
    void readExcelFile(const QString& filename, PtrQMapS2F mapS2F, int sheetIndex = 0, int columnNameRow = 1, int dataStartRow = 2);
    // readExcelData: 每一行每一列全部读取写入data，默认数据从第二行开始
    void readExcelData(QXlsx::Worksheet* worksheet, int dataStartRow = 2);
    // readExcelColumnNames: 每一个列名都写入excelColumnNames，对应的Field也会写入fields；但是，有意义的field会形成字典field2column，无意义的UNKNOWN列序号会写入unknownFieldColumns
    void readExcelColumnNames(QXlsx::Worksheet* worksheet, PtrQMapS2F mapS2F, int columnNameRow = 1);

    //六、合并更新台账
    void updateWith(DataTable* newTable, enum FieldType primaryKeyField, PtrQMapS2F mapS2F);

    //七、添加字段
    void addColumn(const QString& columnName, PtrQMapS2F mapS2F);
    void addColumn(FieldType field, PtrQMapF2S mapF2S);

    //七、写入表格文件
    void writeExcelFile(const QString& filename, const std::vector<enum FieldType>& fieldTypes, PtrQMapF2S mapF2S);

    //八、设置表格格式
    void setTitleFormat(QXlsx::Format& format);
    void setContentFormat(QXlsx::Format& format);


};

#endif // DATATABLE_H
