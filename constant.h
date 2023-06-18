#ifndef CONSTANT_H
#define CONSTANT_H

#include <QString>
#include <QVector>
#include <QMap>
#include <memory>


enum class FieldType {
    XULIEHAO,         //序列号（设备出厂编号）
    KAPIANBIANHAO,    //卡片编号
    SHIWUZICHANBIANHAO, //实物资产编号
    ZAIJIANZICHANBIANHAO, //在建资产编号
    GUDINGZICHANDAIMA,  //固定资产代码
    GUDINGZICHANLEIBIE, //固定资产类别
    ZICHANMINGCHENG,    //资产名称
    SHEBEIMINGCHENG,    //设备名称
    ZICHANPINPAI,       //资产品牌
    SHEBEIBIANMA,       //设备编码
    GUIGEXINGHAO,       //规格型号
    LISHUGONGCHENG,     //隶属工程
    JINGFEILAIYUAN,     //经费来源
    SUOSHUXITONG,       //所属系统
    SUOSHUSHEJIZIXITONG, //所属设计子系统
    DANWEI,              //计数单位
    CAIGOUHETONGHAO,     //采购合同号
    JICHENGHETONGHAO,    //集成合同号
    JICHENGSHANG,        //集成商
    SHULIANGPIFUGAISUAN, //数量批复概算
    SHEBEIDANJIAPIFUGAISUAN, //设备单价批复概算
    ANZHUANGFEIDANJIAPIFUGAISUAN, //安装费单价批复概算
    HEJIPIFUGAISUAN,              //合计批复概算
    SHULIANGSHIJICAIGOU,        //数量实际采购
    SHEBEIDANJIASHIJICAIGOU,    //设备单价实际采购
    ANZHUANGFEIDANJIASHIJICAIGOU, //安装费单价实际采购
    HEJISHIJICAIGOU,            //合计实际采购
    YUANZHI,                //原值
    SUOSHUJIFANG,        //所属机房
    SUOZAIJIGUI,         //所在机柜
    SUOZAIUSHU,          //所在U数
    CUNFANGDIDIAN,       //存放地点
    ZICHANGUANLIBUMEN,   //资产管理部门
    SHIYONGBUMEN,        //使用部门
    SHIYONGREN,          //使用人
    GOUZHIRIQI,          //购置日期
    KAISHISHIYONGRIQI,   //开始使用日期
    SHIYONGZHUANGKUANG,  //使用状况
    MIJI,                //密集
    SHIFOUWEIGUOCHANHUA, //是否为国产化
    ZHUSHEBEIKAPIANBIANHAO,//主设备卡片编号
    ZHUSHEBEIXULIEHAO,   //主设备序列号
    ZHUSHEBEIMINGCHENG,  //主设备名称
    SUOSHUZHUSHEBEINEIBUWEIZHI, //所属主设备内部位置
    XINXILURURIQI,              //信息录入日期
    XINXIZUIHOUGENGXINRIQI,    //信息最后更新日期
    PEIZHI,                     //配置
    CAOZUOXITONG,               //操作系统
    SUOSHUWANGLUO,              //所属网络
    ZHUIPDIZHI,                 //主IP地址
    PUTONGIPDIZHI,              //普通IP地址
    YONGTU,                     //用途
    RUANJIANCHANGSHANG,         //软件厂商
    RUANJIANFUZEREN,            //软件负责人
    SHIYONGRENBEIZHU,           //使用人备注
    ZICHANGUANLIYUANBEIZHU,     //资产管理员备注
    UNKNOWN,                    //未知字段
    COUNT
};

static const char* FIELDTYPESTR[] = {
    "XULIEHAO",         //序列号（设备出厂编号）
    "KAPIANBIANHAO",    //卡片编号
    "SHIWUZICHANBIANHAO", //实物资产编号
    "ZAIJIANZICHANBIANHAO", //在建资产编号
    "GUDINGZICHANDAIMA",  //固定资产代码
    "GUDINGZICHANLEIBIE", //固定资产类别
    "ZICHANMINGCHENG",    //资产名称
    "SHEBEIMINGCHENG",    //设备名称
    "ZICHANPINPAI",       //资产品牌
    "SHEBEIBIANMA",       //设备编码
    "GUIGEXINGHAO",       //规格型号
    "LISHUGONGCHENG",     //隶属工程
    "JINGFEILAIYUAN",     //经费来源
    "SUOSHUXITONG",       //所属系统
    "SUOSHUSHEJIZIXITONG", //所属设计子系统
    "DANWEI",              //计数单位
    "CAIGOUHETONGHAO",     //采购合同号
    "JICHENGHETONGHAO",    //集成合同号
    "JICHENGSHANG",        //集成商
    "SHULIANGPIFUGAISUAN", //数量批复概算
    "SHEBEIDANJIAPIFUGAISUAN", //设备单价批复概算
    "ANZHUANGFEIDANJIAPIFUGAISUAN", //安装费单价批复概算
    "HEJIPIFUGAISUAN",              //合计批复概算
    "SHULIANGSHIJICAIGOU",        //数量实际采购
    "SHEBEIDANJIASHIJICAIGOU",    //设备单价实际采购
    "ANZHUANGFEIDANJIASHIJICAIGOU", //安装费单价实际采购
    "HEJISHIJICAIGOU",            //合计实际采购
    "YUANZHI",                //原值
    "SUOSHUJIFANG",        //所属机房
    "SUOZAIJIGUI",         //所在机柜
    "SUOZAIUSHU",          //所在U数
    "CUNFANGDIDIAN",       //存放地点
    "ZICHANGUANLIBUMEN",   //资产管理部门
    "SHIYONGBUMEN",        //使用部门
    "SHIYONGREN",          //使用人
    "GOUZHIRIQI",          //购置日期
    "KAISHISHIYONGRIQI",   //开始使用日期
    "SHIYONGZHUANGKUANG",  //使用状况
    "MIJI",                //密集
    "SHIFOUWEIGUOCHANHUA", //是否为国产化
    "ZHUSHEBEIKAPIANBIANHAO",//主设备卡片编号
    "ZHUSHEBEIXULIEHAO",   //主设备序列号
    "ZHUSHEBEIMINGCHENG",  //主设备名称
    "SUOSHUZHUSHEBEINEIBUWEIZHI", //所属主设备内部位置
    "XINXILURURIQI",              //信息录入日期
    "XINXIZUIHOUGENGXINRIQI",    //信息最后更新日期
    "PEIZHI",                     //配置
    "CAOZUOXITONG",               //操作系统
    "SUOSHUWANGLUO",              //所属网络
    "ZHUIPDIZHI",                 //主IP地址
    "PUTONGIPDIZHI",              //普通IP地址
    "YONGTU",                     //用途
    "RUANJIANCHANGSHANG",         //软件厂商
    "RUANJIANFUZEREN",            //软件负责人
    "SHIYONGRENBEIZHU",           //使用人备注
    "ZICHANGUANLIYUANBEIZHU",     //资产管理员备注
    "UNKNOWN",                    //未知字段
    "COUNT"
};



typedef QMap<QString, enum FieldType> QMapString2Field;
typedef std::shared_ptr<QMapString2Field> PtrQMapS2F;
typedef QMap<QString, PtrQMapS2F> QMapPtrQMapS2F;

typedef QMap<enum FieldType, QString> QMapField2String;
typedef std::shared_ptr<QMapField2String> PtrQMapF2S;
typedef QMap<QString, PtrQMapF2S> QMapPtrQMapF2S;

typedef QMap<QString, int> QMapString2Int;

static QMap<QString, enum FieldType> MAPFIELDSTR2TYPE;
//QMap<enum FieldType, QString> MAPFIELDTYPE2STR;
static void constructMapFieldType();
static enum FieldType getFieldType(const QString& fieldTypeStr);
static QString getFieldTypeStr(const enum FieldType& fieldType);

enum ValueType {
    STRING,
    DATE,
    INTEGER,
    FLOATNUMBER,
};

typedef struct {
    enum FieldType fieldType;
    QString fieldName;
} ColumnType;


static void constructMapFieldType() {
    for (int i = 0; i <= static_cast<int>(FieldType::COUNT); ++i) {
        MAPFIELDSTR2TYPE.insert(QString(FIELDTYPESTR[i]), static_cast<enum FieldType>(i));
        qDebug("constructMapFieldType: %s", FIELDTYPESTR[i]);
    }
}

static enum FieldType getFieldType(const QString& fieldTypeStr) {
    QMap<QString, enum FieldType>::iterator it = MAPFIELDSTR2TYPE.find(fieldTypeStr);
    if (it != MAPFIELDSTR2TYPE.end()) {
        return it.value();
    } else {
        return FieldType::COUNT;
    }
}

static QString getFieldTypeStr(const enum FieldType& fieldType) {
    int index = static_cast<int>(fieldType);
    return QString(FIELDTYPESTR[index]);
}


#endif // CONSTANT_H
