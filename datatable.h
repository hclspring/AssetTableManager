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

class Config;

class DataTable
{
public:
    DataTable();
    ~DataTable();

private:
    // data指表格台账数据，第一维为数据行，第二维为字段列
    std::shared_ptr<QVector<QVector<QString>>> dataPtr;
    std::shared_ptr<QVector<QString>> columnNamesPtr;
    std::shared_ptr<QMapString2Int> mapColumnName2IndexPtr;

    // maxRow指excel文件的最大行数，maxCol指excel文件的最大列数
    int maxRow, maxCol;

public:
    //第一部分全部已确认无误
    //一、对成员变量的set/get函数
    void set_data(QVector<QVector<QString>>& data);
    //void set(std::vector<int>& rowIndices, DataTable& referenceDataTable);
    std::shared_ptr<QVector<QVector<QString>>> get_data();
    void set_columnNames(QVector<QString>& newColumnNames);
    std::shared_ptr<QVector<QString>> get_columnNames();
    QString get_columnName(int columnIndex);//若不合法，则返回NULL


    //第二部分已全部确认无误
    //二、根据已知信息判断行序号或列序号
    // get_column_index: 能找到，返回序号，否则返回-1
    int get_column_index(const QString& columnName);
    // get_column_indices：如果所有字段都能找到，返回true；如果出现找不到的字段，则在参数中对应位置填写-1，返回false
    bool get_column_indices(QVector<QString>& columnNames, std::vector<int>& outputColumnIndices);
    // get_row_index: 返回指定列为指定值的行，如果行数超过1，则返回数量的相反数，如果行数等于0，则返回-1
    // 这里做字符串比较时，大小写不敏感，如有需要可在设置里提供自定义大小写敏感判断
    int get_row_index(const QString& columnName, QString& cellValue);
    // get_row_indices: 返回指定列为指定值的所有行序号，如果指定列不存在则返回空vector
    std::vector<int> get_row_indices(const QString& columnName, QString& cellValue);


    //第三部分已确认无误
    //三、获取数据：如果ignoreUnfoundColumn为true，则没有对应字段时返回""，否则报错退出
    bool check_row_index(int rowIndex);//已确认无误
    bool check_column_index(int columnIndex); //已确认无误
    bool check_row_column_index(int rowIndex, int columnIndex); //已确认无误
    QString get_cell_value(int rowIndex, int columnIndex); //已确认无误

    //第四部分已确认无误
    //四、格式化数据：
    std::shared_ptr<QVector<QString>> get_formatted_row(int rowIndex, QVector<QString>& referenceColumnNames);

    //第四部分已确认无误
    //四、更新数据
    // 对数据进行操作，成功返回true，失败返回false
    bool add_row(QVector<QString>& dataRow);
    bool update_cell(int rowIndex, int columnIndex, const QString& value); //已确认无误
    bool update_cell(int rowIndex, const QString& columnName, const QString& value); //已确认无误


    //第五部分已确认无误
    //五、读取表格文件
    void readExcelFile(const QString& filename, std::shared_ptr<QMapStr2Str> importColumnNameMap, int sheetIndex = 0, int columnNameRow = 1, int dataStartRow = 2); //已确认无误
    void readExcelFile(const QString& filename, std::shared_ptr<QMapStr2Str> importColumnNameMap, const QString& sheetName, int columnNameRow = 1, int dataStartRow = 2); //已确认无误
    // readExcelData: 每一行每一列全部读取写入data，默认数据从第二行开始
    void readExcelData(QXlsx::Worksheet* worksheet, int dataStartRow = 2); //已确认无误
    // readExcelColumnNames: 每一个列名都写入columnNames，同时会更新mapColumnName2IndexPtr;
    void readExcelColumnNames(QXlsx::Worksheet* worksheet, std::shared_ptr<QMapStr2Str> importColumnNameMap, int columnNameRow = 1); //已确认无误


    //第六部分已全部确认无误
    //六、合并更新台账
    bool updateWith(DataTable* newDataTable, const QString& primaryKeyColumnName);

    //第七部分已确认无误
    //七、添加字段
    // addColumnName: 初始化dataTable时添加列名，不动maxCol和dataPtr
    void addColumnName(const QString& columnName);
    // addNewEmptyColumn: 新加空字段时给dataTable添加列，需要同步修改maxCol和dataPtr
    void addNewEmptyColumn(const QString& columnName);


    //七、写入表格文件
    bool writeExcelFile(const QString& filename, const std::shared_ptr<QVector<QString>>& exportColumnNamesPtr, std::shared_ptr<QMapStr2Str> mappingExport);

    //八、设置表格格式
    void setTitleFormat(QXlsx::Format& format);
    void setContentFormat(QXlsx::Format& format);


};

#endif // DATATABLE_H
