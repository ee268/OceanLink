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

//收藏类型
enum CollectType {
    None,               //无
    ChatMessage = 1,    //聊天记录
    SpacePost           //空间文章
};

Q_DECLARE_METATYPE(CollectType)

#endif // GLOBAL_H
