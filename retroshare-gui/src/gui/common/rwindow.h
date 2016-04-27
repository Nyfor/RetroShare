/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (c) 2006-2007, crypton
 * Copyright (c) 2006, Matt Edman, Justin Hipple
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


#ifndef _RETROSHAREWINDOW_H
#define _RETROSHAREWINDOW_H

#include <QString>
#include <QWidget>
#include <QVariant>
#include <QMainWindow>
#include <gui/settings/rsettings.h>


class RWindow : public QMainWindow
{
  Q_OBJECT

public:
  /** Default constructor. */
  RWindow(QString name, QWidget *parent = 0, Qt::WindowFlags flags = 0);
  /** Destructor. */
  ~RWindow() override;
  
  /** Associates a shortcut key sequence with a slot. */
  void setShortcut(QString shortcut, const char *slot);
  /** Saves the size and location of the window. */
  void saveWindowState();
  /** Restores the last size and location of the window. */
  void restoreWindowState();

  /** Gets the saved value of a property associated with this window object.
   * If no value was saved, the default value is returned. */
  QVariant getSetting(QString name, QVariant defaultValue);
  /** Saves a value associated with a setting name for this window object. */
  void saveSetting(QString name, QVariant value);

public slots:
  /** Shows or hides this window. */
  void setVisible(bool visible) override;
  /** Show this window. This method really just exists for subclasses to 
   * override, since QMainWindow::show() is non-virtual. */
  virtual void showWindow() { QMainWindow::show(); }
  
signals:
  /** Emitted when a RWindow requests help information on the specified
   * <b>topic</b>. */
  void helpRequested(const QString &topic);

protected:
  QString _name;  /**< Name associated with this window. */

private:
  bool m_bSaveStateOnClose; // is set to true in restoreWindowState
};

#endif

