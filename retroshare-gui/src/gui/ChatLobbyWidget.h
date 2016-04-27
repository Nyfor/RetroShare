#pragma once

#include <QTreeWidget>
#include <retroshare/rsmsgs.h>
#include "ui_ChatLobbyWidget.h"
#include "RsAutoUpdatePage.h"
#include "chat/ChatLobbyUserNotify.h"

#define IMAGE_CHATLOBBY			    ":/icons/chat_128.png"

#define CHAT_LOBBY_PRIVACY_LEVEL_PUBLIC  1
#define CHAT_LOBBY_PRIVACY_LEVEL_PRIVATE 2

class RSTreeWidgetItemCompareRole;
class ChatTabWidget ;
class ChatLobbyDialog ;
class QTextBrowser ;

struct ChatLobbyInfoStruct
{
	QIcon default_icon ;
	ChatLobbyDialog *dialog ;
	time_t last_typing_event ;
};

class ChatLobbyWidget : public RsAutoUpdatePage
{
	Q_OBJECT

public:
	/** Default constructor */
	ChatLobbyWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);

	/** Default destructor */
	~ChatLobbyWidget() override;

	QIcon iconPixmap() const override { return QIcon(IMAGE_CHATLOBBY) ; } //MainPage
	QString pageName() const override { return tr("Chat Lobbies") ; } //MainPage
	QString helpText() const override { return ""; } //MainPage

	UserNotify *getUserNotify(QObject *parent) override; //MainPage

	void updateDisplay() override;

	void setCurrentChatPage(ChatLobbyDialog *) ;	// used by ChatLobbyDialog to raise.
	void addChatPage(ChatLobbyDialog *) ;
	void showLobbyAnchor(ChatLobbyId id, QString anchor) ;

	uint unreadCount();

signals:
	void unreadCountChanged(uint unreadCount);

protected slots:
	void lobbyChanged();
	void lobbyTreeWidgetCustomPopupMenu(QPoint);
	void createChatLobby();
	void subscribeItem();
	void unsubscribeItem();
	void itemDoubleClicked(QTreeWidgetItem *item, int column);
	void updateCurrentLobby() ;
    void displayChatLobbyEvent(qulonglong lobby_id, int event_type, const RsGxsId& gxs_id, const QString& str);
	void readChatLobbyInvites();
	void showLobby(QTreeWidgetItem *lobby_item) ;
	void showBlankPage(ChatLobbyId id) ;
    void unsubscribeChatLobby(ChatLobbyId id) ;
    void createIdentityAndSubscribe();
    void subscribeChatLobbyAs() ;
    void updateTypingStatus(ChatLobbyId id) ;
	void resetLobbyTreeIcons() ;
	void updateMessageChanged(bool incoming, ChatLobbyId, QDateTime time, QString senderName, QString msg);
	void updatePeerEntering(ChatLobbyId);
	void updatePeerLeaving(ChatLobbyId);
	void autoSubscribeItem();

private slots:
	void filterColumnChanged(int);
	void filterItems(const QString &text);
	
    void setShowUserCountColumn(bool show);
    void setShowTopicColumn(bool show);
    void setShowSubscribeColumn(bool show);

	void updateNotify(ChatLobbyId id, unsigned int count) ;

private:
	void autoSubscribeLobby(QTreeWidgetItem *item);
    void subscribeChatLobby(ChatLobbyId id) ;
    void subscribeChatLobbyAtItem(QTreeWidgetItem *item) ;

	bool filterItem(QTreeWidgetItem *item, const QString &text, int filterColumn);

	RSTreeWidgetItemCompareRole *compareRole;
	QTreeWidgetItem *privateLobbyItem;
	QTreeWidgetItem *publicLobbyItem;
    QTreeWidgetItem *privateSubLobbyItem;
    QTreeWidgetItem *publicSubLobbyItem;
	QTreeWidgetItem *getTreeWidgetItem(ChatLobbyId);

	ChatTabWidget *tabWidget ;

	std::map<ChatLobbyId,ChatLobbyInfoStruct> _lobby_infos ;

	std::map<QTreeWidgetItem*,time_t> _icon_changed_map ;

    bool m_bProcessSettings;
    void processSettings(bool bLoad);

    /** Defines the actions for the header context menu */
    QAction* showUserCountAct;
	QAction* showTopicAct;
	QAction* showSubscribeAct;
	int getNumColVisible();

	ChatLobbyUserNotify* myChatLobbyUserNotify;

	/* UI - from Designer */
	Ui::ChatLobbyWidget ui;
};

