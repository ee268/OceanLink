#include "collectlist.h"

#include <QMouseEvent>

#include "ElaIcon.h"

CollectList::CollectList(QWidget *parent)
    : ElaListView(parent)
    , _model(new CollectListModel(this))
    , _delegate(new CollectListDelegate(this))
    , _rightOptions(nullptr)
    , _detailDialog(nullptr)
{
    setObjectName("CollectList");
    setStyleSheet("#CollectList { background-color: transparent; border: none; }");

    setModel(_model);
    setItemDelegate(_delegate);

    initTestData();

    _rightOptions = new MoreOptPopup(this);
    _multiSelectKey = "multiSelectKey";
    _delKey = "delKey";
    _rightOptions->addOption(
        "多选",
        ElaIcon::getInstance()->getElaIcon(ElaIconType::CircleCheck),
        _multiSelectKey, nullptr);
    _rightOptions->addOption(
        "删除",
        ElaIcon::getInstance()->getElaIcon(ElaIconType::TrashCan),
        _delKey, nullptr);
}

CollectList::~CollectList()
{
    if (_detailDialog) {
        _detailDialog->close();
        _detailDialog->deleteLater();
        _detailDialog = nullptr;
    }
}

void CollectList::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        QModelIndex index = this->indexAt(event->pos());

        if (index.isValid()) {
            _rightOptions->show();
        }
    }
    else if (event->button() == Qt::LeftButton) {
        QModelIndex index = this->indexAt(event->pos());

        if (index.isValid()) {
            QString sourceName = index.data(CollectListModel::SourceName).toString();
            QString date = index.data(CollectListModel::Date).toString();
            CollectDetailData data;
            data.name = sourceName;
            data.date = date;
            data.type = index.data(CollectListModel::Type).value<CollectType>();
            data.content = "1231231231231231231231231231313131313131";
            data.imgList = {QPixmap(":/resource/image/avatar.jpg"),
                            QPixmap(":/resource/image/rupa.jpg"),
                            QPixmap(":/resource/image/rupa.jpg")};

            if (!_detailDialog) {
                _detailDialog = new CollectDetailDialog(data, nullptr);
            }
            else {
                _detailDialog->updateData(data);
            }

            _detailDialog->open();
        }
    }

    ElaListView::mousePressEvent(event);
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
