#include "datatable.h"

DataTable::DataTable()
{
}

DataTable::~DataTable()
{
}


void DataTable::set_data(QVector<QVector<QString>>& data)
{
    this->data = std::make_shared<QVector<QVector<QString>>>(data);
}

//void set(std::vector<int>& rowIndices, DataTable& referenceDataTable); //TODO

std::shared_ptr<QVector<QVector<QString>>> DataTable::get_data()
{
    return data;
}

void DataTable::set_fields(QVector<enum FieldType> &fields)
{
    mapField2ColumnIndex.clear();
    this->fields = std::make_shared<QVector<enum FieldType>>(fields);
    for (int i = 0; i < fields.size(); ++i) {
        mapField2ColumnIndex.insert(std::make_pair(fields[i], i));
    }
}

std::shared_ptr<QVector<enum FieldType>> DataTable::get_fields()
{
    return fields;
}

FieldType DataTable::get_field(int columnIndex)
{
    if (check_column_index(columnIndex)) {
        return get_fields()->at(columnIndex);
    } else {
        return FieldType::COUNT;
    }
}

void DataTable::set_unknownFieldColumns(std::vector<int> &unknownFieldColumns)
{
    this->unknownFieldColumns = std::make_shared<std::vector<int>>(unknownFieldColumns);
}

std::shared_ptr<std::vector<int>> DataTable::get_unknownFieldColumns()
{
    return unknownFieldColumns;
}

void DataTable::set_columnNames(QVector<QString> &columnNames)
{
    mapName2ColumnIndex.clear();
    this->columnNames = std::make_shared<QVector<QString>>(columnNames);
    for (int i = 0; i < columnNames.size(); ++i) {
        mapName2ColumnIndex.insert(columnNames[i], i);
    }
}

std::shared_ptr<QVector<QString>> DataTable::get_columnNames()
{
    return std::make_shared<QVector<QString>>(columnNames);
}

QString DataTable::get_columnName(int columnIndex)
{
    if (check_column_index(columnIndex)) {
        return get_columnNames()->at(columnIndex);
    } else {
        return NULL;
    }
}

int DataTable::get_column_index(enum FieldType field)
{
    //std::unordered_map<enum FieldType, int>::iterator it;
    auto it = mapField2ColumnIndex.find(field);
    if (it != mapField2ColumnIndex.end()) {
        return it->second;
    } else {
        return -1;
    }
}


int DataTable::get_column_index(const QString& columnName)
{
    auto it = mapName2ColumnIndex.find(columnName);
    if (it != mapName2ColumnIndex.end()) {
        return it.value();
    } else {
        return -1;
    }
}

bool DataTable::get_column_indices(QVector<enum FieldType>& fields, std::vector<int>& outputColumnIndices, bool retainUnfoundColumns)
{
    outputColumnIndices.clear();
    for (FieldType t : fields) {
        int index = get_column_index(t);
        if (index < 0 && !retainUnfoundColumns) {
            return false;
        } else {
            outputColumnIndices.push_back(index);
        }
    }
    return true;
}


bool DataTable::get_column_indices(QVector<QString>& columnNames, std::vector<int>& outputColumnIndices, bool retainUnfoundColumns)
{
    outputColumnIndices.clear();
    for (QString columnName : columnNames) {
        int index = get_column_index(columnName);
        if (index < 0 && !retainUnfoundColumns) {
            return false;
        } else {
            outputColumnIndices.push_back(index);
        }
        return true;
    }
}

// get_row_index: 返回指定列为指定值的行，如果行数超过1，则返回数量的相反数，如果行数等于0，则返回-1
int DataTable::get_row_index(enum FieldType field, QString& fieldValue)
{
    std::vector<int> rowIndices = get_row_indices(field, fieldValue);
    if (rowIndices.size() > 1) {
        return -rowIndices.size();
    } else if (rowIndices.size() == 0) {
        return -1;
    } else {
        return rowIndices[0];
    }
}

// get_row_indices: 返回指定列为指定值的所有行序号，如果指定列不存在则返回空vector
std::vector<int> DataTable::get_row_indices(enum FieldType field, QString& fieldValue)
{
    std::vector<int> result;
    int columnIndex = get_column_index(field);
    if (columnIndex < 0) {
        qWarning("字段不存在");
        return result;
    }
    for (int i = 0; i < data->size(); ++i) {
        if ((*data)[i][columnIndex].compare(fieldValue, Qt::CaseInsensitive) == 0) {
            result.push_back(i);
        }
    }
    return result;
}

// get_unique_row_indices: 返回指定列为指定值的所有行序号，如果某个值的行数超过1，则对应行序号为行数的相反数
std::vector<int> DataTable::get_unique_row_indices(enum FieldType field, QSet<QString>& fieldValues)
{
    std::vector<int> result;
    //std::unordered_set<QString>::iterator it;
    //for (auto it = fieldValues.begin(); it != fieldValues.end(); ++it) {
    for (QString v : fieldValues){
        //int rowIndex = get_row_index(field, *it);
        int rowIndex = get_row_index(field, v);
        result.push_back(rowIndex);
    }
    return result;
}

// get_multiple_row_indices: 返回指定列为指定值的所有行序号，如果某个值的行数超过1，则所有对应行序号都会返回
std::vector<int> DataTable::get_multiple_row_indices(enum FieldType field, QSet<QString>& fieldValues)
{
    std::vector<int> result;
    for (QString v: fieldValues) {
        std::vector<int> tempRowIndices = get_row_indices(field, v);
        for (int i = 0; i < tempRowIndices.size(); ++i) {
            result.push_back(tempRowIndices[i]);
        }
    }
    return result;
}

bool DataTable::check_row_index(int rowIndex)
{
    return (rowIndex >= 0 && rowIndex < data->size());
}

bool DataTable::check_column_index(int columnIndex)
{
    return (columnIndex >= 0 && columnIndex < maxCol);
}

bool DataTable::check_row_column_index(int rowIndex, int columnIndex)
{
    return check_row_index(rowIndex) && check_column_index(columnIndex);
}

QString DataTable::get_cell_value(int rowIndex, int columnIndex, bool ignoreUnfoundColumn)
{
    bool checked = check_row_column_index(rowIndex, columnIndex);
    if (checked == false && ignoreUnfoundColumn == false) {
        assert(checked);
    } else if (checked == false && ignoreUnfoundColumn == true) {
        return "";
    } else {
        return (*data)[rowIndex][columnIndex];
    }
}

QString DataTable::get_cell_value(int rowIndex, enum FieldType field, bool ignoreUnfoundColumn)
{
    int columnIndex = get_column_index(field);
    return get_cell_value(rowIndex, columnIndex, ignoreUnfoundColumn);
}

std::shared_ptr<QVector<QString>> DataTable::get_formatted_row(int rowIndex, std::vector<int>& referenceColumnIndices)
{
    QVector<QString> result;
    assert(check_row_index(rowIndex));
    for (int columnIndex : referenceColumnIndices) {
        result.push_back(std::move(get_cell_value(rowIndex, columnIndex, true)));
    }
    return std::make_shared<QVector<QString>>(result);
}


std::shared_ptr<QVector<QString>> DataTable::get_formatted_row(int rowIndex, QVector<enum FieldType>& referenceFields)
{
    QVector<QString> result;
    assert(check_row_index(rowIndex));
    for (enum FieldType field : referenceFields) {
        result.push_back(std::move(get_cell_value(rowIndex, field, true)));
    }
    return std::make_shared<QVector<QString>>(result);
}

std::shared_ptr<QVector<QVector<QString>>> DataTable::get_formatted_row(std::vector<int>& rowIndices, std::vector<int>& referenceColumnIndices)
{
    QVector<QVector<QString>> result;
    for (int rowIndex : rowIndices) {
        result.push_back(*get_formatted_row(rowIndex, referenceColumnIndices));
    }
    return std::make_shared<QVector<QVector<QString>>>(result);
}

std::shared_ptr<QVector<QVector<QString>>> DataTable::get_formatted_data(std::vector<int>& rowIndices, QVector<enum FieldType>& referenceFields)
{
    QVector<QVector<QString>> result;
    for (int rowIndex : rowIndices) {
        result.push_back(*get_formatted_row(rowIndex, referenceFields));
    }
    return std::make_shared<QVector<QVector<QString>>>(result);
}

// 对数据进行操作，成功返回true，失败返回false
bool DataTable::add_row(QVector<QString>& dataRow)
{
    if (dataRow.size() != fields->size()) {
        qDebug("添加的行中字段数量为%d，与要求数量%d不一致！", dataRow.size(), fields->size());
        return false;
    } else {
        data->push_back(dataRow);
        return true;
    }
}

bool DataTable::update_cell(int rowIndex, int columnIndex, const QString& value)
{
    if (check_row_column_index(rowIndex, columnIndex)) {
        (*data)[rowIndex][columnIndex] = value;
        return true;
    } else {
        qWarning("rowIndex或columnIndex不符合要求（在0~N-1之间），无法更新。");
        return false;
    }
}

bool DataTable::update_cell(int rowIndex, enum FieldType field, const QString& value)
{
    int columnIndex = get_column_index(field);
    return update_cell(rowIndex, columnIndex, value);
}

bool DataTable::update_cell(int rowIndex, const QString& columnName, const QString& value)
{
    int columnIndex = get_column_index(columnName);
    return update_cell(rowIndex, columnIndex, value);
}







void DataTable::readExcelFile(const QString &filename, PtrQMapS2F mapS2F, int sheetIndex, int columnNameRow, int dataStartRow)
{
    QXlsx::Document document(filename);
    if (document.load()) {
        QStringList sheetNames = document.sheetNames();
        if (sheetNames.size() <= sheetIndex) {
            qDebug() << "文件中没有足够的表格，目前只有" << sheetNames.size() << "个sheet.";
        } else {
            QString sheetName = sheetNames[sheetIndex];
            document.selectSheet(sheetName);
            QXlsx::AbstractSheet* sheet = document.sheet(sheetName);
            QXlsx::Worksheet* worksheet = (QXlsx::Worksheet*) sheet;
            worksheet->getFullCells(&maxRow, &maxCol);
            qDebug() << maxRow << "行，" << maxCol << "列";
            readExcelData(worksheet, dataStartRow);
            readExcelColumnNames(worksheet, mapS2F, columnNameRow);
            delete worksheet;
            delete sheet;
        }
    } else {
        qDebug() << "读取文件" << filename << "失败！";
    }
}

void DataTable::readExcelData(QXlsx::Worksheet* worksheet, int dataStartRow)
{
    QVector<QString> row;
    row.resize(maxCol);
    data->fill(row, maxRow - dataStartRow + 1);
    for (int r = dataStartRow; r <= maxRow; ++r) {
        for (int c = 1; c <= maxCol; ++c) {
            (*data)[r-dataStartRow][c-1] = worksheet->cellAt(r, c)->value().toString();
        }
    }
}

void DataTable::readExcelColumnNames(QXlsx::Worksheet* worksheet, PtrQMapS2F mapS2F, int columnNameRow)
{
    if (maxCol > 0) {
        columnNames->resize(maxCol);
    } else {
        qWarning("maxCol不是正数！");
    }
    QVector<FieldType> newFields;
    QVector<QString> newColumnNames;
    std::vector<int> newUnknownFieldColumns;
    for (int i = 1; i <= maxCol; ++i) {
        newColumnNames.push_back(worksheet->read(columnNameRow, i).toString());
        newFields.push_back(mapS2F->find(newColumnNames[i-1]).value());
        if (mapS2F->find(newColumnNames[i-1]) != mapS2F->end()) {
            newFields.push_back(mapS2F->find(newColumnNames[i-1]).value());
        } else {
            newFields.push_back(FieldType::UNKNOWN);
            newUnknownFieldColumns.push_back(i-1);
        }
    }
    set_columnNames(newColumnNames);
    set_fields(newFields);
    set_unknownFieldColumns(newUnknownFieldColumns);
}

void DataTable::updateWith(DataTable* newTable, enum FieldType primaryKeyField, PtrQMapS2F mapS2F)
{
    //获取新增表的主键序号
    int primaryKeyColumnIndexOfNewTable = newTable->get_column_index(primaryKeyField);
    //对每一行数据做同样的操作
    for (int rowIndexOfNewTable = 0; rowIndexOfNewTable < newTable->get_data()->size(); ++rowIndexOfNewTable) {
        //获取主键值
        QString primaryKeyValue = newTable->get_data()->at(rowIndexOfNewTable).at(primaryKeyColumnIndexOfNewTable);
        //获取主表中的行序号
        int rowIndexOfMainTable = this->get_row_index(primaryKeyField, primaryKeyValue);
        //如果主表中存在对应的行，则做更新
        if (rowIndexOfMainTable >= 0) {
            //对新增表该行数据的每一列做同样的操作
            for (int columnIndexOfNewTable = 0; columnIndexOfNewTable < newTable->get_data()->at(rowIndexOfNewTable).size(); ++columnIndexOfNewTable) {
                //获取每一列的字段
                enum FieldType field = newTable->get_field(columnIndexOfNewTable);
                QString columnName = newTable->get_columnName(columnIndexOfNewTable);
                //确认主表是否存在对应的字段，如果不存在，需要先添加字段，再做后续更新
                if (this->get_column_index(columnName) < 0) {
                    addColumn(columnName, mapS2F);
                }
                //对主表对应的字段进行更新
                if (field != FieldType::UNKNOWN && field != FieldType::COUNT) {
                    this->update_cell(rowIndexOfMainTable, field, newTable->get_cell_value(rowIndexOfNewTable, columnIndexOfNewTable, true));
                } else if (field == FieldType::UNKNOWN) {
                    this->update_cell(rowIndexOfMainTable, columnName, newTable->get_cell_value(rowIndexOfNewTable, columnIndexOfNewTable, true));
                } else {
                    qDebug() << "无法更新FieldType == COUNT的字段";
                }
            }
        } //否则（主表中不存在对应的行），则新增一行数据
        else if (rowIndexOfMainTable == -1){
            // 将新增表的行按照主表的字段进行格式化
            std::shared_ptr<QVector<QString>> formattedRow = newTable->get_formatted_row(rowIndexOfNewTable, *(this->get_fields()));
            // 在主表中新增行
            this->add_row(*formattedRow);
        } else {
            qWarning() << "台账中字段" << getFieldTypeStr(primaryKeyField) << "值为" << primaryKeyValue << "的行数超过1，等于" << QString(-rowIndexOfMainTable) << ", 未做更新。";
        }
    }
}



void DataTable::addColumn(const QString& columnName, PtrQMapS2F mapS2F)
{
    if (mapName2ColumnIndex.find(columnName) != mapName2ColumnIndex.end()) {
        qCritical("已存在名为%s的列！", columnName.toStdString().c_str());
        return;
    }
    ++maxCol;
    for (int i = 0; i < data->size(); ++i) {
        (*data)[i].push_back("");
    }
    columnNames->push_back(columnName);
    int columnIndex = maxCol - 1;
    mapName2ColumnIndex.insert(columnName, columnIndex);
    FieldType field;
    if (mapS2F->find(columnName) != mapS2F->end()) {
        field = mapS2F->find(columnName).value();
        fields->push_back(field);
        mapField2ColumnIndex.insert(std::make_pair(field, columnIndex));
    } else {
        field = FieldType::UNKNOWN;
        fields->push_back(field);
        unknownFieldColumns->push_back(columnIndex);
    }
}

void DataTable::addColumn(FieldType field, PtrQMapF2S mapF2S)
{
    if (mapField2ColumnIndex.find(field) != mapField2ColumnIndex.end()) {
        qCritical("已存在字段类为%s的列！", getFieldTypeStr(field).toStdString().c_str());
        return;
    }
    ++maxCol;
    for (int i = 0; i < data->size(); ++i) {
        (*data)[i].push_back("");
    }
    QString fieldStr;
    if (field == FieldType::COUNT) {
        fieldStr = "无意义字段";
    } else if (field == FieldType::UNKNOWN) {
        fieldStr = "未知字段";
    } else if (mapF2S->find(field) != mapF2S->end()) {
        fieldStr = mapF2S->find(field).value();
    } else {
        fieldStr = getFieldTypeStr(field);
    }
    columnNames->push_back(fieldStr);
    int columnIndex = maxCol - 1;
    mapName2ColumnIndex.insert(fieldStr, columnIndex);
    if (field != FieldType::UNKNOWN) {
        mapField2ColumnIndex.insert(std::make_pair(field, columnIndex));
    } else {
        unknownFieldColumns->push_back(columnIndex);
    }
}

void DataTable::writeExcelFile(const QString& filename, const std::vector<enum FieldType>& fieldTypes, PtrQMapF2S mapF2S)
/* 参考代码：
 * https://blog.csdn.net/qq_37603131/article/details/128555121
 */
{
    QXlsx::Document xlsx;
    xlsx.addSheet("Sheet0");
    xlsx.selectSheet("Sheet0");
    QXlsx::Format titleFormat;
    setTitleFormat(titleFormat);
    for (int i = 0; i < fieldTypes.size(); ++i) {
        xlsx.write(1, i+1, get_columnName(i));
    }
    QXlsx::Format contentFormat;
    setContentFormat(contentFormat);
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




