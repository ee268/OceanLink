#include "contactlist.h"
#include "contactlistmodel.h"
#include "contactlistdelegate.h"

ContactList::ContactList(QWidget *parent)
    : ElaTreeView(parent)
    , _model(new ContactListModel(this))
    , _delegate(new ContactListDelegate(this))
{
    this->setModel(_model);
    this->setItemDelegate(_delegate);
    this->setHeaderHidden(true);

    // 添加测试数据
    QStringList groups = {"好友", "同事", "家人", "同学", "工作",
                          "游戏", "社团", "邻居", "客户", "其他"};

    QStringList names = {"张三", "李四", "王五", "赵六", "钱七",
                         "孙八", "周九", "吴十", "郑十一", "王十二"};

    QStringList signs = {"今天天气真好", "在忙，稍后回复", "在线",
                         "开会中", "外出就餐", "学习中",
                         "休息中", "工作中", "旅行中", "心情不错"};

    // 添加分组和联系人
    for (int i = 0; i < groups.size(); i++) {
        _model->addGroup(groups[i]);

        for (int j = 0; j < names.size(); j++) {
            QString name = names[j];
            QString sign = signs[j];
            bool status = (j % 2 == 0);  // 偶数在线，奇数离线
            QString nickname = name;

            ContactData data;
            data.name = name;
            data.sign = sign;
            data.status = status;
            data.group = groups[i];
            data.nickname = nickname;
            data.age = 23;
            data.sex = 2;
            data.nickname = "哈哈哈哈哈哈";
            data.birthday = "3月12日";
            if (status) {
                data.avatar = QPixmap(":/resource/image/avatar.jpg");
            }
            _model->addContact(data);
        }
    }

    connect(this, &ContactList::clicked, this, &ContactList::slotContactClicked);
}

QStringList ContactList::getGroupNames() const
{
    return _model->getGroupNames();
}

void ContactList::slotContactClicked(const QModelIndex &index)
{
    bool isGroupItem = index.data(ContactListModel::IsGroup).toBool();
    if (!isGroupItem) {
        emit sigContactClicked(index);
    }
}
