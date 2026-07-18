#ifndef CONTACTLISTMODEL_H
#define CONTACTLISTMODEL_H

#include <QAbstractItemModel>
#include <QPixmap>
#include <memory>
#include <vector>

struct ContactData {
    QString name;       //联系人名称
    QString sign;       //签名
    bool status;        //在线状态
    QString group;      //所属分组
    QString nickname;   //昵称
    bool isGroupItem;   //是否为分组节点
    QPixmap avatar;     //头像
    int age;
    int sex;
    QString birthday;
    QString account;

    ContactData* parent;
    std::vector<std::unique_ptr<ContactData>> children;

    ContactData() : parent(nullptr){};

    ContactData(const QString& group, ContactData* parent = nullptr)
        : name(group)
        , isGroupItem(true)
        , parent(nullptr){}

    ContactData(const QString& name, const QString& sign, bool status,
                const QString& group, const QString& nickname, 
                bool isGroupItem, int age,
                const QString& birthday, const QString& account, int sex,
                const QPixmap& avatar = QPixmap(), ContactData* parent = nullptr)
        : name(name)
        , sign(sign)
        , status(status)
        , group(group)
        , nickname(nickname), isGroupItem(isGroupItem)
        , age(age), birthday(birthday)
        , account(account), sex(sex)
        , avatar(avatar), parent(parent)
    {}
};

class ContactListModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum ContactRole {
        Name = Qt::DisplayRole,
        Sign = Qt::ToolTipRole,
        Status,
        Group,
        Nickname,
        IsGroup,
        Avatar,
        Age,
        Birthday,
        Account,
        Sex
    };

    explicit ContactListModel(QObject *parent = nullptr);
    ~ContactListModel() = default;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addGroup(const QString& groupName);
    void addContact(const ContactData& contact);
    void removeContact(const QModelIndex& index);
    QModelIndex getGroupIndex(const QString& groupName) const;
    QStringList getGroupNames() const;

private:
    ContactData* getNode(const QModelIndex &index) const;
    int getRow(ContactData *node) const;

    std::unique_ptr<ContactData> _root;
};

#endif // CONTACTLISTMODEL_H
