#include "datatable.h"

DataTable::DataTable()
{
}

DataTable::~DataTable()
{
}


void DataTable::set(QVector<QVector<QString>>& data)
{
    this->data = std::make_shared<QVector<QVector<QString>>>(data);
}

void set(std::vector<int>& rowIndices, DataTable& referenceDataTable); //TODO

std::shared_ptr<QVector<QVector<QString>>> DataTable::get_data()
{
    return data;
}

void DataTable::set(QVector<enum FieldType>& fields)
{
    field2column.clear();
    this->fields = std::make_shared<QVector<enum FieldType>>(fields);
    for (int i = 0; i < fields.size(); ++i) {
        field2column.insert(std::make_pair(fields[i], i));
    }
}

std::shared_ptr<QVector<enum FieldType>> DataTable::get_fields()
{
    return fields;
}

int DataTable::get_column_index(enum FieldType field)
{
    //std::unordered_map<enum FieldType, int>::iterator it;
    auto it = field2column.find(field);
    if (it == field2column.end()) {
        return -1;
    } else {
        return it->second;
    }
}

//int get_column_index(QString& fieldName); //TODO

bool DataTable::get_column_indices(QVector<enum FieldType>& fields, std::vector<int>& outputColumnIndices, bool retainUnfoundColumns)
{
    outputColumnIndices.clear();
    for (FieldType t : fields) {
        int index = get_column_index(t);
        if (index < 0 && !retainUnfoundColumns) return false;
        outputColumnIndices.push_back(index);
    }
    return true;
}

//bool get_column_indices(QVector<QString>& fieldNames, std::vector<int>& outputColumnIndices); //TODO

// get_row_index: 返回指定列为指定值的行，如果行数超过1，则返回数量的相反数，如果行数等于0，则返回-1
//int get_row_index(QString& fieldName, QString& fieldValue); //TODO

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
//std::vector<int> get_row_indices(QString& fieldName, QString& fieldValue); //TODO

std::vector<int> DataTable::get_row_indices(enum FieldType field, QString& fieldValue)
{
    std::vector<int> result;
    int columnIndex = get_column_index(field);
    if (columnIndex < 0) {
        qDebug("字段不存在");
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
//std::vector<int> get_unique_row_indices(QString& fieldName, QSet<QString>& fieldValues); //TODO

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
//std::vector<int> get_multiple_row_indices(QString& fieldName, QSet<QString>& fieldValues);

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

bool DataTable::check_row_column_index(int rowIndex, int columnIndex)
{
    return check_row_index(rowIndex) && (columnIndex >= 0)
            && (columnIndex < (*data)[rowIndex].size());
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

bool DataTable::update_cell(int rowIndex, int columnIndex, QString value)
{
    if (check_row_column_index(rowIndex, columnIndex)) {
        (*data)[rowIndex][columnIndex] = value;
        return true;
    } else {
        return false;
    }
}

bool DataTable::update_cell(int rowIndex, enum FieldType field, QString value)
{
    int columnIndex = get_column_index(field);
    return update_cell(rowIndex, columnIndex, value);
}






void DataTable::readExcelFile(QXlsx::Document& assetDocument, PtrQMapS2F mapS2F, const QString& sheetName, int columnNameRow, int dataStartRow)
{
    assetDocument.selectSheet(sheetName);
    QXlsx::AbstractSheet* sheet = assetDocument.sheet(sheetName);
    QXlsx::Worksheet* worksheet = (QXlsx::Worksheet*) sheet;
    worksheet->getFullCells(&maxRow, &maxCol);
    qDebug() << maxRow << "行，" << maxCol << "列";
    readExcelData(worksheet, dataStartRow);
    readExcelColumnNames(worksheet, mapS2F, columnNameRow);
    delete worksheet;
    delete sheet;
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

void DataTable::readExcelFile(const QString &filename, PtrQMapS2F mapS2F, int sheetIndex, int columnNameRow, int dataStartRow)
{
    QXlsx::Document document(filename);
    if (document.load()) {
        QStringList sheetNames = document.sheetNames();
        if (sheetNames.size() <= sheetIndex) {
            qDebug() << "文件中没有足够的表格，目前只有" << sheetNames.size() << "个sheet.";
        } else {
            QString sheetName = sheetNames[sheetIndex];
            readExcelFile(document, mapS2F, sheetName, columnNameRow, dataStartRow);
        }
    } else {
        qDebug() << "读取文件" << filename << "失败！";
    }
}

void DataTable::readExcelColumnNames(QXlsx::Worksheet* worksheet, PtrQMapS2F mapS2F, int columnNameRow)
{
    if (maxCol > 0) {
        excelColumnNames.resize(maxCol);
    } else {
        qWarning("maxCol不是正数！");
    }
    fields->clear();
    field2column.clear();
    for (int i = 1; i <= maxCol; ++i) {
        excelColumnNames[i-1] = worksheet->read(columnNameRow, i).toString();
        if (mapS2F->find(excelColumnNames[i-1]) != mapS2F->end()) {
            enum FieldType field = mapS2F->find(excelColumnNames[i-1]).value();
            fields->push_back(field);
            field2column.insert(std::make_pair(field, i - 1));
        } else {
            qCritical() << QString("未识别到列名").append(excelColumnNames[i-1]).toStdString().c_str();
        }
    }
}

void DataTable::updateWith(DataTable* newTable, enum FieldType primaryKeyField)
{
    //获取新增表的主键序号
    int primaryKeyColumnIndexOfNewTable = newTable->get_column_index(primaryKeyField);
    //对每一行数据做同样的操作，i为新增表的行序号
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
                enum FieldType field = newTable->get_fields()->at(columnIndexOfNewTable);
                //对主表对应的字段进行更新
                this->update_cell(rowIndexOfMainTable, field, newTable->get_cell_value(rowIndexOfNewTable, columnIndexOfNewTable, true));
            }
        } //否则（主表中不存在对应的行），则新增一行数据
        else {
            // 将新增表的行按照主表的字段进行格式化
            std::shared_ptr<QVector<QString>> formattedRow = newTable->get_formatted_row(rowIndexOfNewTable, *(this->get_fields()));
            // 在主表中新增行
            this->add_row(*formattedRow);
        }
    }
}

void DataTable::writeExcelFile(const QString& filename, PtrQMapF2S mapF2S)
{
    ;
}






