#include "collectlist.h"

#include "collectlistmodel.h"
#include "collectlistdelegate.h"

CollectList::CollectList(QWidget *parent)
    : ElaListView(parent)
    , _model(new CollectListModel(this))
    , _delegate(new CollectListDelegate(this))
{
    setObjectName("CollectList");
    setStyleSheet("#CollectList { background-color: transparent; border: none; }");

    setModel(_model);
    setItemDelegate(_delegate);

    initTestData();
}

CollectList::~CollectList()
{
}

void CollectList::initTestData()
{
    QStringList dates = {
        "2026-07-20", "2026-07-19", "2026-07-18", "2026-07-17", "2026-07-16",
        "2026-07-15", "2026-07-14", "2026-07-13", "2026-07-12", "2026-07-11",
        "2026-07-10", "2026-07-09", "2026-07-08", "2026-07-07", "2026-07-06"
    };

    QStringList contents = {
        "今天天气真好，出去散步了",
        "分享一篇关于Qt开发的技术文章",
        "周末去哪里玩比较好呢",
        "推荐一本好书给大家",
        "新买的耳机音质真不错",
        "学习C++的心得体会",
        "美食推荐：这家餐厅很不错",
        "电影观后感分享",
        "旅行日记：海边的日落",
        "健身打卡第100天",
        "读书笔记：设计模式",
        "摄影作品分享",
        "编程技巧总结",
        "生活小窍门分享",
        "音乐推荐：这首歌曲很好听"
    };

    QStringList sourceNames = {
        "张三", "李四", "王五", "赵六", "钱七",
        "孙八", "周九", "吴十", "郑十一", "王十二",
        "刘十三", "陈十四", "林十五", "黄十六", "杨十七"
    };

    for (int i = 0; i < 15; i++) {
        CollectType type = (i % 2 == 0) ? ChatMessage : SpacePost;
        CollectData data(dates[i], "来源", QPixmap(), contents[i], sourceNames[i], type);
        if (i % 2 == 0) {
            data.cover = QPixmap(":/resource/image/rupa.jpg");
        }
        _model->addCollect(data);
    }
}
