/*
 * mainwindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void chooseLesson();
    void chooseLayout();
    void update();
    void chooseLessonsMode();
    void choosePhrasesMode();
    void nextLesson();
    void prevLesson();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    QAction *actLessonsSeparator_;
    QAction *curLessonAction_;
    QAction *curLayoutAction_;
    QStringList phrasesList_;
    QList<QAction*> lessonList_;
    QList<QAction*> lessonGroupList_;
};

#endif // MAINWINDOW_H
