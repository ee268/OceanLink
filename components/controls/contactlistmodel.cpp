#include "contactlistmodel.h"

ContactListModel::ContactListModel(QObject *parent)
    : QAbstractItemModel{parent}
    , _root(std::make_unique<ContactData>())
{
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

void ContactListModel::removeContact(const QModelIndex& index)
{
    if (!index.isValid())
        return;

    ContactData* node = getNode(index);
    if (node->isGroupItem)
        return;  // 不能删除分组

    ContactData* parentNode = node->parent;
    QModelIndex parentIndex = createIndex(getRow(parentNode), 0, parentNode);
    int row = getRow(node);

    beginRemoveRows(parentIndex, row, row);
    parentNode->children.erase(parentNode->children.begin() + row);
    endRemoveRows();
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
    for (size_t i = 0; i < _root->children.size(); i++) {
        if (_root->children.at(i)->isGroupItem) {
            res.append(_root->children.at(i)->name);
        }
    }

    return res;
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
        for (size_t i = 0; i < node->parent->children.size(); i++) {
            if (node->parent->children.at(i).get() == node) {
                return static_cast<int>(i);
            }
        }
    }
    return 0;
}
