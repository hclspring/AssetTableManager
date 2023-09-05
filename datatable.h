#ifndef DATATABLE_H
#define DATATABLE_H

#include <QString>
#include <QVector>
#include <QSet>
#include <QDebug>
#include <QTextStream>
#include <QCoreApplication>
#include <QEventLoop>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>

#include "xlsxdocument.h"

#include "constant.h"

/*
20230826计划更新：
1.key字段的值如果为空，更新时如何处置
2.表头单元格颜色等属性如何定义和设置：台账使用初始属性，导出表使用默认属性
3.内容单元格颜色沿用初始属性，更新后使用更新表格的属性
QXlsx::Cell cell = *(worksheet->cellAt(r, c));
QVariant qv = worksheet->cellAt(r, c)->readValue();
QXlsx::Format format = worksheet->cellAt(r, c)->format();
*/


class Config;

class DataTable
{
public:
    DataTable();
    ~DataTable();

private:
    // data指表格台账数据，第一维为数据行，第二维为字段列
    std::shared_ptr<std::vector<VecPtrCell>> dataPtr;
    std::shared_ptr<VecPtrCell> columnNameCellVecPtr;
    std::shared_ptr<QMapString2Int> mapColumnName2IndexPtr;

    // maxRow指excel文件的最大行数，maxCol指excel文件的最大列数
    int maxRow, maxCol;

public:
    //第一部分全部已确认无误
    //一、对成员变量的set/get函数
    void set_data(std::vector<VecPtrCell>& data);
    //void set(std::vector<int>& rowIndices, DataTable& referenceDataTable);
    std::shared_ptr<std::vector<VecPtrCell>>& get_dataPtr();
    void set_columnNameCellVecPtr(PtrVecPtrCell& newColumnNameCells);
    PtrVecPtrCell get_columnNameCellVecPtr();
    PtrCell get_columnNameCellPtr(int columnIndex);//若不合法，则返回NULL


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
    QString get_cell_str(int rowIndex, int columnIndex); //已确认无误
    PtrCell get_dataCellPtr(int rowIndex, int columnIndex);

    //第四部分已确认无误
    //四、调整数据字段顺序：
    PtrVecPtrCell get_sorted_row(int rowIndex, VecPtrCell& referenceColumnNameCells);
    //std::shared_ptr<QVecCell> get_sorted_row(int rowIndex, QVecString& referenceColumnNames);

    //第四部分已确认无误
    //四、更新数据
    // 对数据进行操作，成功返回true，失败返回false
    bool add_row(VecPtrCell& dataRow);
    bool update_cell(int rowIndex, int columnIndex, const PtrCell& cell);
    bool update_cell(int rowIndex, const QString& columnName, const PtrCell& cell);
    /*
    bool update_cell(int rowIndex, int columnIndex, const QString& value); //已确认无误
    bool update_cell(int rowIndex, const QString& columnName, const QString& value); //已确认无误
    */


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
    DataTable* updateWith(DataTable* newDataTable, const QString& primaryKeyColumnName);

    //第七部分已确认无误
    //七、添加字段
    // addColumnName: 初始化dataTable时添加列名，不动maxCol和dataPtr
    void addColumnName(const QString& columnName);
    // addNewEmptyColumn: 新加空字段时给dataTable添加列，需要同步修改maxCol和dataPtr
    void addNewEmptyColumn(const QString& columnName);


    //七、写入表格文件
    bool writeExcelFile(const QString& filename, const PtrVecPtrCell& exportColumnNameCellsPtr, std::shared_ptr<QMapStr2Str> mappingExport);
    bool writeExcelFile(const QString& filename);

    //八、设置表格格式
    void setTitleFormat(QXlsx::Format& format);
    void setContentFormat(QXlsx::Format& format);

    void mySleep(uint usetime,ulong waittime);

    //九、获取无法更新的数据
    DataTable* initUpdateFailedColumnNames(DataTable* updateTable);
    bool addUpdateFailedRow(DataTable* updateTable, int rowIndex, DataTable* updateIgnoredTable);
    bool addUpdateFailedData(DataTable* updateTable, DataTable* updateIgnoredTable);



};

#endif // DATATABLE_H
