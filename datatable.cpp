#include "datatable.h"
#include "config.h"

DataTable::DataTable()
{
    this->dataPtr = std::make_shared<std::vector<VecPtrCell>>();
    this->columnNameCellVecPtr = std::make_shared<VecPtrCell>();
    this->mapColumnName2IndexPtr = std::make_shared<QMapString2Int>();
}

DataTable::~DataTable()
{
}


void DataTable::set_data(std::vector<VecPtrCell>& data)
{
    this->dataPtr = std::make_shared<std::vector<VecPtrCell>>(data);
}

std::shared_ptr<std::vector<VecPtrCell>>& DataTable::get_dataPtr()
{
    return dataPtr;
}

void DataTable::set_columnNameCellVecPtr(PtrVecPtrCell &newColumnNameCellVecPtr)
{
    //mapName2ColumnIndex.clear();
    mapColumnName2IndexPtr->clear();
    this->columnNameCellVecPtr = newColumnNameCellVecPtr;
    for (int i = 0; i < newColumnNameCellVecPtr->size(); ++i) {
        mapColumnName2IndexPtr->insert(newColumnNameCellVecPtr->at(i)->value().toString(), i);
    }
}

PtrVecPtrCell DataTable::get_columnNameCellVecPtr()
{
    return columnNameCellVecPtr;
}

PtrCell DataTable::get_columnNameCellPtr(int columnIndex)
{
    if (check_column_index(columnIndex)) {
        return get_columnNameCellVecPtr()->at(columnIndex);
    } else {
        return nullptr;
    }
}

int DataTable::get_column_index(const QString& columnName)
{
    qDebug() << "Enter function get_column_index in class DataTable.";
    auto it = mapColumnName2IndexPtr->find(columnName.trimmed());
    if (it != mapColumnName2IndexPtr->end()) {
        return it.value();
    } else {
        return -1;
    }
}

bool DataTable::get_column_indices(QVector<QString>& columnNames, std::vector<int>& outputColumnIndices)
{
    outputColumnIndices.clear();
    bool allExist = true;
    for (QString columnName : columnNames) {
        int index = get_column_index(columnName);
        if (index < 0) {
            allExist = false;
        }
        outputColumnIndices.push_back(index);
    }
    return allExist;
}

// get_row_index: 返回指定列为指定值的行，如果行数超过1，则返回数量的相反数，如果行数等于0，则返回-1
int DataTable::get_row_index(const QString& columnName, QString& cellValue)
{
    std::vector<int> rowIndices = get_row_indices(columnName, cellValue);
    if (rowIndices.size() > 1) {
        return -rowIndices.size();
    } else if (rowIndices.size() == 0) {
        return -1;
    } else {
        return rowIndices[0];
    }
}

// get_row_indices: 返回指定列为指定值的所有行序号，如果指定列不存在则返回空vector
std::vector<int> DataTable::get_row_indices(const QString& columnName, QString& cellValue)
{
    std::vector<int> result;
    int columnIndex = get_column_index(columnName);
    if (columnIndex < 0) {
        qWarning("字段不存在");
        return result;
    }
    for (int i = 0; i < dataPtr->size(); ++i) {
        if (get_dataCellPtr(i, columnIndex)->value().toString().trimmed().compare(cellValue.trimmed(), Qt::CaseInsensitive) == 0) {
            result.push_back(i);
        }
    }
    return result;
}

bool DataTable::check_row_index(int rowIndex)
{
    return (rowIndex >= 0 && rowIndex < dataPtr->size());
}

bool DataTable::check_column_index(int columnIndex)
{
    return (columnIndex >= 0 && columnIndex < maxCol);
}

bool DataTable::check_row_column_index(int rowIndex, int columnIndex)
{
    return check_row_index(rowIndex) && check_column_index(columnIndex);
}

QString DataTable:: get_cell_str(int rowIndex, int columnIndex)
{
    bool checked = check_row_column_index(rowIndex, columnIndex);
    if (checked == false) {
        qWarning("行号或列号超出限制：rowIndex = %d，columnIndex = %d，返回元素值为空。", rowIndex, columnIndex);
        return "";
    } else {
        return get_dataCellPtr(rowIndex, columnIndex)->value().toString();
    }
}


PtrCell DataTable::get_dataCellPtr(int rowIndex, int columnIndex)
{
    bool checked = check_row_column_index(rowIndex, columnIndex);
    if (checked == false) {
        qWarning("行号或列号超出限制：rowIndex = %d，columnIndex = %d，返回元素值为空。", rowIndex, columnIndex);
        return nullptr;
    } else {
        return dataPtr->at(rowIndex).at(columnIndex);
    }
}

/*
std::shared_ptr<QVector<QString>> DataTable::get_formatted_row(int rowIndex, QVector<QString>& referenceColumnNames)
{
    QVector<QString> result;
    assert(check_row_index(rowIndex));
    for (QString referenceColumnName : referenceColumnNames) {
        int columnIndex = get_column_index(referenceColumnName);
        if (columnIndex >= 0) {
            result.push_back(std::move(get_cell_str(rowIndex, columnIndex)));
        } else {
            result.push_back("");
        }
    }
    return std::make_shared<QVector<QString>>(result);
}
*/

PtrVecPtrCell DataTable::get_sorted_row(int rowIndex, VecPtrCell& referenceColumnNameCells)
{
    VecPtrCell result;
    assert(check_row_index(rowIndex));
    for (PtrCell referenceColumnNameCellPtr : referenceColumnNameCells) {
        QString referenceColumnName = referenceColumnNameCellPtr->value().toString().trimmed();
        int columnIndex = get_column_index(referenceColumnName);
        if (columnIndex >= 0) {
            result.push_back(get_dataCellPtr(rowIndex, columnIndex));
        } else {
            result.push_back(std::make_shared<QXlsx::Cell>());
        }
    }
    return std::make_shared<VecPtrCell>(result);
}

/*
std::shared_ptr<VecPtrCell> DataTable::get_sorted_row(int rowIndex, QVecString& referenceColumnNames)
{
    VecPtrCell result;
    assert(check_row_index(rowIndex));
    for (QString referenceColumnName : referenceColumnNames) {
        int columnIndex = get_column_index(referenceColumnName);
        if (columnIndex >= 0) {
            result.push_back(std::move(get_cell_copy(rowIndex, columnIndex)));
        } else {
            result.push_back(new QXlsx::Cell(""));
        }
    }
    return std::make_shared<VecPtrCell>(result);
}
*/

// 对数据进行操作，成功返回true，失败返回false
bool DataTable::add_row(VecPtrCell& dataRow)
{
    if (dataRow.size() != columnNameCellVecPtr->size()) {
        qWarning("添加的行中字段数量为%d，与要求数量%d不一致！", dataRow.size(), columnNameCellVecPtr->size());
        return false;
    } else {
        add_row(dataRow);
        return true;
    }
}

bool DataTable::update_cell(int rowIndex, int columnIndex, const PtrCell& cell)
{
    if (check_row_column_index(rowIndex, columnIndex)) {
        dataPtr->at(rowIndex).at(columnIndex) = cell;
        return true;
    } else {
        qWarning("rowIndex或columnIndex不符合要求（在0~N-1之间），无法更新。");
        return false;
    }
}

bool DataTable::update_cell(int rowIndex, const QString& columnName, const PtrCell& value)
{
    int columnIndex = get_column_index(columnName);
    return update_cell(rowIndex, columnIndex, value);
}

void DataTable::readExcelFile(const QString &filename, std::shared_ptr<QMapStr2Str> importColumnNameMap, int sheetIndex, int columnNameRow, int dataStartRow)
{
    QXlsx::Document document(filename);
    if (document.load()) {
        QStringList sheetNames = document.sheetNames();
        if (sheetNames.size() <= sheetIndex) {
            qDebug() << "文件中没有足够的表格，目前只有" << sheetNames.size() << "个sheet.";
        } else {
            QString sheetName = sheetNames[sheetIndex];
            readExcelFile(filename, importColumnNameMap, sheetName, columnNameRow, dataStartRow);
        }
    } else {
        qCritical() << "读取文件" << filename << "失败！";
    }
    document.deleteLater();
}

void DataTable::readExcelFile(const QString& filename, std::shared_ptr<QMapStr2Str> importColumnNameMap, const QString& sheetName, int columnNameRow, int dataStartRow)
{
    QXlsx::Document document(filename);
    if (document.load()) {
        if (document.selectSheet(sheetName)) {
            QXlsx::AbstractSheet* sheet = document.sheet(sheetName);
            QXlsx::Worksheet* worksheet = (QXlsx::Worksheet*) sheet;
            worksheet->getFullCells(&maxRow, &maxCol);
            qDebug() << maxRow << "行，" << maxCol << "列";
            qDebug() << "开始读取表格数据";
            readExcelData(worksheet, dataStartRow);
            qDebug() << "表格数据读取完毕，开始读取分析字段名";
            readExcelColumnNames(worksheet, importColumnNameMap, columnNameRow);
            qDebug() << "字段名读取完毕";
        } else {
            qCritical() << "文件" << filename << "不含名为" << sheetName << "的sheet，读取失败！";
        }
    } else {
        qCritical() << "读取文件" << filename << "失败！";
    }
    document.deleteLater();
}

void DataTable::readExcelData(QXlsx::Worksheet* worksheet, int dataStartRow)
{
    VecPtrCell row;
    row.resize(maxCol, std::make_shared<QXlsx::Cell>());
    qDebug() << "maxRow = " << maxRow << ", dataStartRow = " << dataStartRow << ", maxCol = " << maxCol;
    std::vector<VecPtrCell> temp;
    temp.resize(maxRow - dataStartRow + 1, row);
    set_data(temp);
    qDebug() << "完成data初始化";
    for (int r = dataStartRow; r <= maxRow; ++r) {
        for (int c = 1; c <= maxCol; ++c) {
            if (worksheet->cellAt(r, c) == nullptr){
                qDebug() << "读取第" << r << "行、第" << c << "列：空值";
                //不做任何处理，数据中该元素为空值
            } else {
                qDebug() << "读取第" << r << "行、第" << c << "列：" << worksheet->cellAt(r, c)->value().toString();
                get_dataPtr()->at(r-dataStartRow).at(c-1)->value() = worksheet->cellAt(r, c)->value();
                get_dataPtr()->at(r-dataStartRow).at(c-1)->format() = worksheet->cellAt(r, c)->format();
            }
        }
    }
}

void DataTable::readExcelColumnNames(QXlsx::Worksheet* worksheet, std::shared_ptr<QMapStr2Str> importColumnNameMap, int columnNameRow)
{
    if (maxCol <= 0) {
        qWarning("maxCol不是正数！");
        return;
    }
    QVector<QString> newColumnNames;
    for (int i = 1; i <= maxCol; ++i) {
        QString newColumnName = worksheet->read(columnNameRow, i).toString().trimmed();
        qDebug() << "读取Excel文件第 " << i << " 个列名是：" << newColumnName;
        if (importColumnNameMap == nullptr) qDebug() << "importColumnNameMap == nullptr";
        if (importColumnNameMap->find(newColumnName) != importColumnNameMap->end()) {
            newColumnName = importColumnNameMap->find(newColumnName).value().trimmed();
            qDebug() << "将列名更新为" << newColumnName;
        }
        int newIndex = get_column_index(newColumnName);
        qDebug() << "获取列名" << newColumnName << "的当前index为" << newIndex;
        if (newIndex >= 0) {
            newColumnName.append("（重复列名）");
            qWarning() << "发现重复列名，已更名为“" << newColumnName << "”";
        }
        qDebug() << "添加列名" << newColumnName;
        addColumnName(newColumnName);
        qDebug() << "获取列名" << newColumnName << "的当前index为" << get_column_index(newColumnName);
    }
    //以下均为打印日志需要，与函数主要功能无关
    QString printTemp = "读取的列名清单：";
    //for (QXlsx::Cell s : *(this->columnNameCellsPtr)) {
    for (int i = 0; i < this->columnNameCellVecPtr->size(); ++i) {
        PtrCell s = columnNameCellVecPtr->at(i);
        printTemp.append(s->value().toString()).append("; ");
    }
    qDebug() << printTemp;
}

bool DataTable::updateWith(DataTable* newTable, const QString& primaryKeyColumnName)
{
    //确认主表中存在主键字段
    int primaryKeyColumnIndex = get_column_index(primaryKeyColumnName);
    if (primaryKeyColumnIndex < 0) {
        qCritical() << "主表中不存在主键字段，更新失败!" << primaryKeyColumnName;
        return false;
    }
    //获取新增表的主键序号
    int primaryKeyColumnIndexOfNewTable = newTable->get_column_index(primaryKeyColumnName);
    qDebug() << "更新表的主键序号是" << primaryKeyColumnIndexOfNewTable;
    if (primaryKeyColumnIndexOfNewTable < 0) {
        qWarning() << "更新表中不存在主键字段，更新失败！" << primaryKeyColumnName;
        return  false;
    }
    //更新主表的字段
    for (PtrCell newColumnNameCell : *(newTable->get_columnNameCellVecPtr())) {
        QString newColumnName = newColumnNameCell->value().toString().trimmed();
        if (get_column_index(newColumnName) < 0) {
            qDebug() << "主表不存在字段" << newColumnName << ", 在主表中添加这一列";
            addNewEmptyColumn(newColumnName);
        }
    }
    //对每一行数据做同样的操作
    for (int rowIndexOfNewTable = 0; rowIndexOfNewTable < newTable->get_dataPtr()->size(); ++rowIndexOfNewTable) {
        //获取主键值
        PtrCell primaryKeyCell = newTable->get_dataPtr()->at(rowIndexOfNewTable).at(primaryKeyColumnIndexOfNewTable);
        QString primaryKeyValue = primaryKeyCell->value().toString().trimmed();
        if (primaryKeyValue.length() == 0) {
            qWarning() << "主键值为空，跳过更新相应数据";
            continue;
        }
        qDebug() << "更新表第" << rowIndexOfNewTable << "行的主键值为" << primaryKeyValue;
        //获取主表中的行序号
        int rowIndexOfMainTable = this->get_row_index(primaryKeyColumnName, primaryKeyValue);
        qDebug() << "该主键值在主表中的行号是" << rowIndexOfMainTable;
        //如果主表中存在对应的行，则做更新
        if (rowIndexOfMainTable >= 0) {
            //对新增表该行数据的每一列做同样的操作
            for (int columnIndexOfNewTable = 0; columnIndexOfNewTable < newTable->get_dataPtr()->at(rowIndexOfNewTable).size(); ++columnIndexOfNewTable) {
                //获取每一列的字段
                PtrCell columnNameCell = newTable->get_columnNameCellPtr(columnIndexOfNewTable);
                QString columnName = columnNameCell->value().toString().trimmed();
                qDebug() << "对字段名为 " << columnName << "的列开始处理";
                //对主表对应的字段进行更新
                this->update_cell(rowIndexOfMainTable, columnName, newTable->get_dataCellPtr(rowIndexOfNewTable, columnIndexOfNewTable));
            }
        } //否则（主表中不存在对应的行），则新增一行数据
        else if (rowIndexOfMainTable == -1){
            // 将新增表的行按照主表的字段进行格式化
            PtrVecPtrCell sortedRow = newTable->get_sorted_row(rowIndexOfNewTable, *(this->get_columnNameCellVecPtr()));
            QString printTemp = "主表不存在该主键值，因此新增一行，将更新表中数据行格式化为主表格式：";
            for (PtrCell s : *sortedRow) {
                printTemp.append(s->value().toString()).append("; ");
            }
            qDebug() << printTemp;
            // 在主表中新增行
            this->add_row(*sortedRow);
        } else {
            qWarning() << "台账中字段" << primaryKeyColumnName << "值为" << primaryKeyValue << "的行数超过1，等于" << QString(-rowIndexOfMainTable) << ", 未做更新。";
        }
    }
    return true;
}

void DataTable::addColumnName(const QString& columnName)
{
    if (get_column_index(columnName) >= 0) {
        qCritical("已存在名为%s的列！", columnName.toStdString().c_str());
        return;
    }
    int columnIndex = columnNameCellVecPtr->size();
    PtrCell cell = std::make_shared<QXlsx::Cell>(columnName.trimmed());
    qDebug() << columnName.trimmed() << " --- " << cell->value().toString();
    columnNameCellVecPtr->push_back(cell);
    int size = columnNameCellVecPtr->size();
    qDebug() << "当前columnNameCellsPtr最后一个列名为：" << columnNameCellVecPtr->at(size - 1)->value().toString();
    mapColumnName2IndexPtr->insert(columnName.trimmed(), columnIndex);
}

void DataTable::addNewEmptyColumn(const QString& columnName)
{
    if (get_column_index(columnName) >= 0) {
        qCritical("已存在名为%s的列！", columnName.toStdString().c_str());
        return;
    }
    ++maxCol;
    for (int i = 0; i < get_dataPtr()->size(); ++i) {
        get_dataPtr()->at(i).push_back(std::make_shared<QXlsx::Cell>());
    }
    columnNameCellVecPtr->push_back(std::make_shared<QXlsx::Cell>(columnName.trimmed()));
    int columnIndex = maxCol - 1;
    mapColumnName2IndexPtr->insert(columnName.trimmed(), columnIndex);
}


bool DataTable::writeExcelFile(const QString& filename, const PtrVecPtrCell& exportColumnNameCellsPtr, std::shared_ptr<QMapStr2Str> mappingExport)
/* 参考代码：
 * https://blog.csdn.net/qq_37603131/article/details/128555121
 */
{
    QXlsx::Document xlsx;
    xlsx.addSheet("Sheet0");
    xlsx.selectSheet("Sheet0");
    QXlsx::Format titleFormat;
    setTitleFormat(titleFormat);
    PtrVecPtrCell exportColumnNameCellsPtr4Use;
    //QVecString exportColumnNames;
    //确定输出字段，如果参数为空指针，则输出所有字段；如果参数不空，则输出指定的字段
    if (exportColumnNameCellsPtr == nullptr) {
        exportColumnNameCellsPtr4Use = get_columnNameCellVecPtr();
    } else {
        exportColumnNameCellsPtr4Use = exportColumnNameCellsPtr;
    }
    //输出字段名
    for (int i = 0; i < exportColumnNameCellsPtr4Use->size(); ++i) {
        QString cellContent = exportColumnNameCellsPtr4Use->at(i)->value().toString().trimmed();
        QXlsx::Format cellFormat = exportColumnNameCellsPtr4Use->at(i)->format();
        xlsx.write(1, i+1, cellContent, cellFormat);
    }
    QXlsx::Format contentFormat;
    setContentFormat(contentFormat);
    //对每一行数据进行输出
    for (int i = 0; i < get_dataPtr()->size(); ++i) {
        //对每一个需要输出的字段进行读取和输出
        for (int j = 0; j < exportColumnNameCellsPtr4Use->size(); ++j) {
            PtrCell cell = nullptr;
            QString exportColumnName = exportColumnNameCellsPtr4Use->at(j)->value().toString().trimmed();
            if (mappingExport->find(exportColumnName) != mappingExport->end()) {
                //如果对应输出字段有定义到原生字段的映射关系，则找到原生字段
                QString dataTableColumnName = mappingExport->find(exportColumnName).value();
                if (dataTableColumnName.contains('&')) {
                    //如果原生字段包括&，则通过&分割成多个字段，分别读取这几个字段的值，再拼接到一起
                    QString cellContent;
                    QStringList columns = dataTableColumnName.split("&");
                    for (QString columnName : columns) {
                        if (get_column_index(columnName.trimmed()) >= 0) {
                            cellContent.append(get_dataCellPtr(i, get_column_index(columnName.trimmed()))->value().toString());
                        }
                    }
                    cell = std::make_shared<QXlsx::Cell>(cellContent);
                } else if (get_column_index(dataTableColumnName) >= 0) {
                    //如果原生字段不包括 &，但数据台账中有此字段，则正常处理
                    cell = get_dataCellPtr(i, get_column_index(dataTableColumnName.trimmed()));
                } //数据台账中不含需要的字段，不对cell做任何处理，保持nullptr
            } else {
                //没找到，说明不需要转换字段，什么都不做
                cell = get_dataCellPtr(i, get_column_index(exportColumnName.trimmed()));
            }
            if (cell != nullptr) {
                xlsx.write(i+2, j+1, cell->value(), cell->format());
            } else {
                xlsx.write(i+2, j+1, "", contentFormat);
            }
        }
    }
    bool saveResult = xlsx.saveAs(filename);
    qDebug() << (saveResult ? "成功保存到Excel文件！" : "保存Excel文件失败！");
    xlsx.deleteLater();
    return saveResult;
}

void DataTable::setTitleFormat(QXlsx::Format& format)
{
    format.setBorderStyle(QXlsx::Format::BorderThin);
    format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    format.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    format.setTextWrap(true);
    format.setLocked(true);
}

void DataTable::setContentFormat(QXlsx::Format& format)
{
    format.setBorderStyle(QXlsx::Format::BorderThin);
    format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    format.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    format.setTextWrap(true);
    format.setLocked(false);
}




