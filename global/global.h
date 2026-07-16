#ifndef GLOBAL_H
#define GLOBAL_H

#define PROJECT_VERSION "0.1"
#define PROjECT_NAME "OceanLink"

#include <QString>
#include <QPixmap>


//好友审核状态
enum FriendNotifyStatus {
    Passed = 1,         //已通过
    NotPassed,          //未通过
    WaitingPassed       //等待通过
};

//性别
enum Sex{
    Female,
    Male,
    Secret
};

#endif // GLOBAL_H
