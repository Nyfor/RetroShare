/****************************************************************
 *  RetroShare is distributed under the following license:
 *
 *  Copyright (C) 2014, RetroShare Team
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA  02110-1301, USA.
 ****************************************************************/

#ifndef POSTEDPAGE_H
#define POSTEDPAGE_H

#include <retroshare-gui/configpage.h>

namespace Ui {
class PostedPage;
}

class PostedPage : public ConfigPage
{
	Q_OBJECT

public:
	PostedPage(QWidget * parent = 0, Qt::WindowFlags flags = 0);
	~PostedPage() override;

	/** Saves the changes on this page */
	bool save(QString &errmsg) override;
	/** Loads the settings for this page */
	void load() override;

	QPixmap iconPixmap() const override { return QPixmap(":/images/posted_32.png") ; }
	QString pageName() const override { return tr("Posted") ; }
	QString helpText() const override { return ""; }

private:
	Ui::PostedPage *ui;
};

#endif // !POSTEDPAGE_H

