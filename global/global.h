#ifndef GLOBAL_H
#define GLOBAL_H

#define PROJECT_VERSION "0.1"
#define PROjECT_NAME "OceanLink"

#include <QString>
#include <QPixmap>

const QString ServerUrl = "http://127.0.0.1:8080";

//好友审核状态
enum FriendNotifyStatus {
    Passed = 1,         //已通过
    NotPassed,          //未通过
    WaitingPassed       //等待通过
};

//性别
enum Sex{
    Female,       //女
    Male,         //男
    Secret        //保密
};

//收藏类别
enum CollectType {
    None,               //无
    ChatMessage,        //聊天记录
    SpacePost           //空间文章
};

Q_DECLARE_METATYPE(CollectType)

//请求类别
enum RequestID {
    ID_GET_VERIFY_CODE    //请求验证码
};

//发出请求的模块
enum Modules {
    REGISTER,      //注册模块
};

//错误码
enum ErrorCodes {
    NoError,
    NetworkError
};

#endif // GLOBAL_H
