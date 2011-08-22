/*
 * mainwindow.cpp
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

#include <QDomNode>
#include <QFile>
#include <QDebug>
#include <QTimer>
#include <QDir>

#include "action.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , actLessonsSeparator_(NULL)
    , curLessonAction_(NULL)
    , curLayoutAction_(NULL)
    , phrasesList_(QStringList())
    , lessonList_(QList<QAction*>())
    , lessonGroupList_(QList<QAction*>())
{
    ui->setupUi(this);

    // qdesigner mayn't setup separator name. I must to do it here
    actLessonsSeparator_ = ui->menuMode->insertSeparator(ui->actPrevLesson);

    qsrand(QTime::currentTime().msec() + QTime::currentTime().second() * 1000);

    // Get all lesson files
    QDir dir(QString(), "lessons_*.xml", QDir::Name, QDir::Files);
    int defaultLesson = 0;
    int added = 0;
    foreach (QString filename, dir.entryList())
    {
        QFile f(filename);
        if (!f.open(QIODevice::ReadOnly))
            continue;

        QDomDocument doc("document");
        bool b = doc.setContent(&f);
        f.close();

        if (!b)
            continue;

        QDomElement el(doc.documentElement());
        QString layoutname = el.attribute("layout");
        QAction *act = ui->menuLayout->addAction(layoutname);
        act->setProperty("layout", filename.mid(8, -4).left(filename.size() - 12));
        act->setCheckable(true);
        connect(act, SIGNAL(triggered()), SLOT(chooseLayout()));

        if (filename == "lessons_us.xml")
            defaultLesson = added;
        added++;
    }

    // Choose first layout by default
    ui->menuLayout->actions().takeAt(defaultLesson)->trigger();


    // Lessons parsing
    // Timer
    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), SLOT(update()));

    // Start and stop menu
    Action *actStart = new Action(tr("Start"), ui->menuBar);
    Action *actStop = new Action(tr("Stop"), ui->menuBar);
    actStop->setEnabled(false);
    ui->menuBar->insertAction(ui->menuMode->menuAction(), actStart);
    ui->menuBar->insertAction(ui->menuMode->menuAction(), actStop);
    connect(actStart, SIGNAL(triggered()), ui->widget, SLOT(start()));
    connect(actStop, SIGNAL(triggered()), ui->widget, SLOT(stop()));

    connect(ui->widget, SIGNAL(started()), actStart, SLOT(disable()));
    connect(ui->widget, SIGNAL(started()), actStop, SLOT(enable()));
    connect(ui->widget, SIGNAL(stopped()), actStart, SLOT(enable()));
    connect(ui->widget, SIGNAL(stopped()), actStop, SLOT(disable()));

    // Progress menu
    Action *actProgress = new Action(tr("Progress"), ui->menuBar);
    ui->menuBar->insertAction(ui->menuOptions->menuAction(), actProgress);

    ui->actLessons->trigger();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::chooseLesson()
{
    if (curLessonAction_)
    {
        curLessonAction_->setIcon(QIcon());
        qobject_cast<QAction*>(curLessonAction_->parent())->setIcon(QIcon());
    }

    curLessonAction_ = qobject_cast<QAction*>(sender());
    curLessonAction_->setIcon(QIcon(":/icons/arrow.png"));
    qobject_cast<QAction*>(curLessonAction_->parent())->setIcon(QIcon(":/icons/arrow.png"));
    ui->widget->setText(curLessonAction_->property("lessonText").toString());
    ui->widget->setPhrases(QStringList());
    setWindowTitle("Dexty - " + curLessonAction_->text());
}

void MainWindow::chooseLayout()
{
    QAction *act = qobject_cast<QAction*>(sender());
    act->setChecked(true);
    if (act == curLayoutAction_)
        return;

    phrasesList_.clear();

    if (curLayoutAction_)
        curLayoutAction_->setChecked(false);

    curLayoutAction_ = act;

    QFile f(QString("lessons_%1.xml").arg(curLayoutAction_->property("layout").toString()));
    if (!f.open(QIODevice::ReadOnly))
        return;

    // First it need to clean menus for old layout
    foreach (QAction *action, lessonGroupList_)
    {
        ui->menuMode->removeAction(action);
        delete action;
    }
    lessonGroupList_.clear();
    lessonList_.clear();

    QDomDocument doc("document");
    doc.setContent(&f);
    f.close();

    QDomElement el(doc.documentElement());

    QDomNode nodeGroup = el.firstChild();
    while (!nodeGroup.isNull())
    {
        // Group proccessing
        if (nodeGroup.isElement() && nodeGroup.toElement().tagName() == "group")
        {
            QString value = nodeGroup.toElement().attributeNode("name").value();
            QMenu *menu = new QMenu(value, ui->menuMode);
            lessonGroupList_ << ui->menuMode->insertMenu(actLessonsSeparator_, menu);

            QDomNode lessonGroup = nodeGroup.toElement().firstChild();
            while (!lessonGroup.isNull())
            {
                // Lesson proccessing
                if (lessonGroup.isElement() && lessonGroup.toElement().tagName() == "lesson")
                {
                    QString value = lessonGroup.toElement().attributeNode("name").value();
                    QAction *actLesson = new QAction(value, menu->menuAction());
                    menu->addAction(actLesson);
                    actLesson->setProperty("lessonText", lessonGroup.toElement().text());
                    connect(actLesson, SIGNAL(triggered()), SLOT(chooseLesson()));
                    lessonList_ << actLesson;
                }
                lessonGroup = lessonGroup.nextSibling();
            }
        }
        nodeGroup = nodeGroup.nextSibling();
    }
    curLessonAction_ = NULL;
    lessonList_[0]->trigger();
    ui->actLessons->trigger();
}

void MainWindow::update()
{
    QMainWindow::update();
    ui->lblTime->setText(ui->widget->getTime().toString("mm:ss"));
}

void MainWindow::chooseLessonsMode()
{
    ui->actLessons->setChecked(true);
    ui->actPhrases->setChecked(false);
    curLessonAction_->trigger();
}

void MainWindow::choosePhrasesMode()
{
    ui->actPhrases->setChecked(true);
    ui->actLessons->setChecked(false);

    // Load phrases
    if (phrasesList_.isEmpty())
    {
        QString filename = QString("phrases_%1.txt").arg(curLayoutAction_->property("layout").toString());
        QFile f(filename);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        while (!f.atEnd())
        {
            QString str = f.readLine().trimmed();
            if (!str.isEmpty())
                phrasesList_ << str + " ";
        }
        f.close();
    }

    setWindowTitle(tr("Dexty - phrases"));
    ui->widget->setPhrases(phrasesList_);
}

void MainWindow::nextLesson()
{
    int index = lessonList_.indexOf(curLessonAction_) + 1;
    if (index < lessonList_.size())
        lessonList_[index]->trigger();
}

void MainWindow::prevLesson()
{
    int index = lessonList_.indexOf(curLessonAction_) - 1;
    if (index >= 0)
        lessonList_[index]->trigger();
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
