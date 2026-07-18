#include "contactlistmodel.h"

#include <QCollator>

ContactListModel::ContactListModel(QObject *parent)
    : QAbstractItemModel{parent}
    , _root(std::make_unique<ContactData>())
{
    // 添加测试数据
    this->addGroup("我");
    this->addContact(ContactData(
        "ee268",
        "hello world",
        true,
        "我",
        "",
        false,
        21,
        "3月13",
        "123123123",
        1));

    QStringList groups = {"好友", "同事", "家人", "同学", "工作",
                          "游戏", "社团", "邻居", "客户", "其他"};

    QStringList names = {"张三", "李四", "王五", "赵六", "钱七",
                         "孙八", "周九", "吴十", "郑十一", "王十二"};

    QStringList signs = {"今天天气真好", "在忙，稍后回复", "在线",
                         "开会中", "外出就餐", "学习中",
                         "休息中", "工作中", "旅行中", "心情不错"};

    // 添加分组和联系人
    for (int i = 0; i < groups.size(); i++) {
        this->addGroup(groups[i]);

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
            this->addContact(data);
        }
    }

    this->sortContacts();
}

QModelIndex ContactListModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    ContactData* parentNode = getNode(parent);
    if (row >= 0 && row < static_cast<int>(parentNode->children.size())) {
        return createIndex(row, column, parentNode->children.at(row).get());
    }
    return QModelIndex();
}

QModelIndex ContactListModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    ContactData* childNode = getNode(index);
    ContactData* parentNode = childNode->parent;

    if (parentNode == _root.get() || parentNode == nullptr)
        return QModelIndex();

    return createIndex(getRow(parentNode), 0, parentNode);
}

int ContactListModel::rowCount(const QModelIndex &parent) const
{
    ContactData* parentNode = getNode(parent);
    return static_cast<int>(parentNode->children.size());
}

int ContactListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant ContactListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    ContactData* node = getNode(index);

    switch (role) {
    case Name:
        return node->name;
    case Sign:
        return node->sign;
    case Status:
        return node->status;
    case Group:
        return node->group;
    case Nickname:
        return node->nickname;
    case IsGroup:
        return node->isGroupItem;
    case Avatar:
        return node->avatar;
    case Age:
        return node->age;
    case Birthday:
        return node->birthday;
    case Account:
        return node->account;
    case Sex:
        return node->sex;
    default:
        return QVariant();
    }
}

void ContactListModel::addGroup(const QString& groupName)
{
    // 检查是否已存在
    for (const auto& child : _root->children) {
        if (child->name == groupName && child->isGroupItem)
            return;
    }

    int row = static_cast<int>(_root->children.size());

    beginInsertRows(QModelIndex(), row, row);

    auto group = std::make_unique<ContactData>(groupName, _root.get());
    _root->children.push_back(std::move(group));

    endInsertRows();
}

void ContactListModel::addContact(const ContactData& contact)
{
    // 查找分组（通过isGroupItem判断）
    ContactData* groupNode = nullptr;
    for (const auto& child : _root->children) {
        if (child->isGroupItem && child->name == contact.group) {
            groupNode = child.get();
            break;
        }
    }

    // 如果分组不存在，自动创建
    if (!groupNode) {
        addGroup(contact.group);
        groupNode = _root->children.back().get();
    }

    QModelIndex groupIndex = createIndex(getRow(groupNode), 0, groupNode);
    int row = static_cast<int>(groupNode->children.size());

    beginInsertRows(groupIndex, row, row);

    auto newContact = std::make_unique<ContactData>(
        contact.name, contact.sign, contact.status, contact.group,
        contact.nickname, false, contact.age, contact.birthday,
        contact.account, contact.sex, contact.avatar, groupNode);
    groupNode->children.push_back(std::move(newContact));

    endInsertRows();
}

void ContactListModel::updateContact(const QModelIndex& index)
{
    if (!index.isValid())
        return;

    emit dataChanged(index, index);
}

ContactData* ContactListModel::takeContact(const QModelIndex& index)
{
    if (!index.isValid())
        return nullptr;

    ContactData* node = getNode(index);
    if (node->isGroupItem)
        return nullptr;

    ContactData* parentNode = node->parent;
    QModelIndex parentIndex = createIndex(getRow(parentNode), 0, parentNode);
    int row = getRow(node);

    beginRemoveRows(parentIndex, row, row);
    ContactData* raw = node;
    parentNode->children[row].release();
    parentNode->children.erase(parentNode->children.begin() + row);
    endRemoveRows();

    return raw;
}

void ContactListModel::removeContact(const QModelIndex& index)
{
    ContactData* node = takeContact(index);
    delete node;
}

void ContactListModel::changeContactGroup(const QModelIndex& index, const QString& newGroup)
{
    if (!index.isValid())
        return;

    ContactData* node = getNode(index);
    if (node->isGroupItem || node->group == newGroup)
        return;

    ContactData* taken = takeContact(index);

    ContactData* newGroupNode = nullptr;
    for (const auto& child : _root->children) {
        if (child->isGroupItem && child->name == newGroup) {
            newGroupNode = child.get();
            break;
        }
    }

    if (!newGroupNode) {
        addGroup(newGroup);
        newGroupNode = _root->children.back().get();
    }

    taken->group = newGroup;
    taken->parent = newGroupNode;

    QModelIndex newParentIndex = createIndex(getRow(newGroupNode), 0, newGroupNode);
    int newRow = static_cast<int>(newGroupNode->children.size());

    beginInsertRows(newParentIndex, newRow, newRow);
    newGroupNode->children.push_back(std::unique_ptr<ContactData>(taken));
    endInsertRows();

    this->sortGroupContacts(newGroup);
}

QModelIndex ContactListModel::getGroupIndex(const QString& groupName) const
{
    for (size_t i = 0; i < _root->children.size(); i++) {
        if (_root->children.at(i)->isGroupItem && _root->children.at(i)->name == groupName) {
            return createIndex(static_cast<int>(i), 0, _root->children.at(i).get());
        }
    }
    return QModelIndex();
}

QStringList ContactListModel::getGroupNames() const
{
    QStringList res;
    for (size_t i = 1; i < _root->children.size(); i++) {
        if (_root->children.at(i)->isGroupItem) {
            res.append(_root->children.at(i)->name);
        }
    }

    return res;
}

void ContactListModel::sortContacts()
{
    beginResetModel();

    QCollator collator(QLocale::Chinese);
    for (auto& group : _root->children) {
        if (!group->isGroupItem)
            continue;

        std::sort(group->children.begin(), group->children.end(),
                  [&collator](const std::unique_ptr<ContactData>& a,
                              const std::unique_ptr<ContactData>& b)
                  {
                      return collator.compare(a->name, b->name) < 0;
                  });
    }

    endResetModel();
}

void ContactListModel::sortGroupContacts(const QString &groupName)
{
    beginResetModel();

    QCollator collator(QLocale::Chinese);
    for (auto& group : _root->children) {
        if (group->isGroupItem && group->name == groupName) {
            std::sort(group->children.begin(), group->children.end(),
                      [&collator](const std::unique_ptr<ContactData>& a,
                                  const std::unique_ptr<ContactData>& b)
                      {
                          return collator.compare(a->name, b->name) < 0;
                      });

            break;
        }
    }

    endResetModel();
}

ContactData* ContactListModel::getNode(const QModelIndex &index) const
{
    if (index.isValid()) {
        ContactData* node = static_cast<ContactData*>(index.internalPointer());
        if (node)
            return node;
    }
    return _root.get();
}

int ContactListModel::getRow(ContactData *node) const
{
    if (node && node->parent) {

        //获取contact节点在group中的下标
        for (size_t i = 0; i < node->parent->children.size(); i++) {
            if (node->parent->children.at(i).get() == node) {
                return static_cast<int>(i);
            }
        }
    }
    return 0;
}
