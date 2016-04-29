/****************************************************************
 *  RetroShare is distributed under the following license:
 *
 *  Copyright (C) 2006,2007,2008 crypton
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

#ifndef DLLISTDELEGATE_H
#define DLLISTDELEGATE_H

#include <QAbstractItemDelegate>
#include "xprogressbar.h"

#define MAX_CHAR_TMP 128

class QModelIndex;
class QPainter;

enum{
    COLUMN_NAME,
    COLUMN_SIZE,
    COLUMN_COMPLETED,
    COLUMN_DLSPEED,
    COLUMN_PROGRESS,
    COLUMN_SOURCES,
    COLUMN_STATUS,
    COLUMN_PRIORITY,
    COLUMN_REMAINING,
    COLUMN_DOWNLOADTIME,
    COLUMN_ID,
    COLUMN_LASTDL,
    COLUMN_PATH,
    COLUMN_COUNT
};

class DLListDelegate: public QAbstractItemDelegate {

	Q_OBJECT

	public:
		DLListDelegate(QObject *parent=0);
		~DLListDelegate();
		void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
        QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const;

	private:

	public slots:

	signals:
};
#endif

