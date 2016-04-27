#ifndef CREATELOBBYDIALOG_H
#define CREATELOBBYDIALOG_H

#include <QDialog>

#include "ui_CreateLobbyDialog.h"
#include <retroshare/rstypes.h>

class CreateLobbyDialog : public QDialog {
	Q_OBJECT

public:
	/*
	 *@param chanId The channel id to send request for
	 */
    CreateLobbyDialog(const std::set<RsPeerId>& friends_list, int privacyLevel = 0, QWidget *parent = 0);
	~CreateLobbyDialog() override;

protected:
	void changeEvent(QEvent *e) override;

private:
	Ui::CreateLobbyDialog *ui;

private slots:
	void createLobby();
	void checkTextFields();
};

#endif // CREATELOBBYDIALOG_H
