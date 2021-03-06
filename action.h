/*
 * action.h
 * Copyright (C) 2011  Ivan Romanov <drizt@land.ru>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef ACTION_H
#define ACTION_H

#include <QAction>

class Action : public QAction
{
    Q_OBJECT

public:
    Action(QObject *parent = NULL) : QAction(parent) {};
    Action(const QString &title, QObject *parent = NULL) : QAction(title, parent) {};
    Action(const QIcon &icon, const QString &title, QObject *parent = NULL) : QAction(icon, title, parent) {};

public slots:
    void enable() { setEnabled(true); };
    void disable(){ setEnabled(false); };
};

#endif // ACTION_H
