#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QVector>
#include <QMap>
#include <memory>

#include "constant.h"


typedef QMap<QString, enum FieldType> QMapString2Field;
typedef std::shared_ptr<QMapString2Field> PtrQMapS2F;
typedef QMap<QString, PtrQMapS2F> QMapPtrQMapS2F;
typedef QMap<enum FieldType, QString> QMapField2String;
typedef std::shared_ptr<QMapField2String> PtrQMapF2S;
typedef QMap<QString, PtrQMapF2S> QMapPtrQMapF2S;


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

};

#endif // CONFIG_H
