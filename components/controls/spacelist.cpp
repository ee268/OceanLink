#include "spacelist.h"

SpaceList::SpaceList(QWidget *parent)
    : ElaListView(parent)
    , _model(new SpaceListModel(this))
    , _delegate(new SpaceListDelegate(this))
{
    this->setModel(_model);
    this->setItemDelegate(_delegate);

    QStringList names = {"张三", "李四", "王五", "赵六", "钱七",
                         "孙八", "周九", "吴十", "郑十一", "王十二"};

    QStringList titles = {"今天天气真好", "在忙，稍后回复", "晚上一起吃饭吗",
                          "明天开会记得带资料", "周末去爬山吧",
                          "刚到家，休息一下", "学习中，勿扰",
                          "工作中，有事留言", "旅行中，信号不好", "心情不错"};

    for (int i = 0; i < 10; i++) {
        SpaceListData data;
        data.name = names[i];
        data.latest_title = titles[i];
        data.account = QString("%1").arg(i);
        _model->addSpace(data);
    }
}
