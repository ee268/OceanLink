#ifndef CHATPAGE_H
#define CHATPAGE_H

#include "basepage.h"
#include "../controls/sessionlist.h"
#include "../controls/moreoptpopup.h"
#include "../controls/messagelist.h"

#include "ElaSuggestBox.h"

#include "ElaText.h"
#include "ElaImageCard.h"
#include "ElaPlainTextEdit.h"

class ChatPage : public BasePage
{
    Q_OBJECT
public:
    explicit ChatPage(QWidget* parent = nullptr);
    ~ChatPage();

private:
    void initLeftWidget();
    void initRightWidget();
    void initRightWidPopup();

private:
    SessionList* _sessionList;
    ElaSuggestBox* _suggestBox;

    MoreOptPopup* _sessionItemPopup;
    QString _topKey;
    QString _copyKey;
    QString _markKey;
    QString _openKey;
    QString _notifyKey;
    QString _removeKey;
    QString _banKey;

    ElaText* _name;
    ElaImageCard* _statusIcon;
    ElaText* _statusText;

    MessageList* _msgList;

    ElaPlainTextEdit* _msgTextEdit;

    MoreOptPopup* _msgItemPopup;
    QString _msgCopyKey;
    QString _msgShareKey;
    QString _msgCollectKey;

private slots:
    void slotSessionItemRightClicked(const QModelIndex &index);

    void slotOpenMsgList(const QModelIndex &index);

    void slotClickedSendButton();

    void slotMsgItemRightClicked(const QModelIndex &index);
};

#endif // CHATPAGE_H
