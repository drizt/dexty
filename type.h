/*
 * type.h
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

#ifndef TYPE_H
#define TYPE_H

#include <QWidget>
#include <QStringList>
#include <QColor>
#include <QTime>
#include <QDateTime>

class QPaintEvent;
class QKeyEvent;

class Type : public QWidget
{
    Q_OBJECT

public:
    explicit Type(QWidget *parent = 0);
    void setPhrases(const QStringList &phrases);
    void setText(const QString &text) { text_ = text; };
    void setFont(const QFont &font) { font_ = font; };
    void setBorderColor(const QColor &color) { colorBorder_ = color; };
    void setBorderWidth(int width) { widthBorder_ = width; };
    void setForegroundLeft(const QColor &color) { fgndLeft_ = color; };
    void setForegroundRight(const QColor &color) { fgndRight_ = color; };
    void setBackgroundLeft(const QColor &color) { bgndLeft_ = color; };
    void setBackgroundRight(const QColor &color) { bgndRight_ = color; };

    QTime getTime();
    float getErrors();

signals:
    void started();
    void stopped();
    void paused();

public slots:
    void start();
    void stop();
    void pause();


protected:
    void paintEvent(QPaintEvent *e);
    void keyPressEvent(QKeyEvent *e);

private:
    enum {Started, Stopped, Paused};

    QStringList phrases_; // For phrases mode only
    QString text_; // String of text
    int position_; // Position of waiting letter
    int widthBorder_; // Width of border
    QColor colorBorder_; // Color of border
    QColor fgndLeft_; // Color of typed text
    QColor fgndRight_; // Color of not yet typed text
    QColor bgndLeft_; // Color of left background
    QColor bgndRight_; // Color of right background
    float sizeLeft_; // Size of left part in procents
    QFont font_; // Font of text
    int errors_; // Number of errors in type procces
    int typedLetters_; // Number of typed letters
    QTime timer_; // Timer
    int allTime_; // Time in mcs elapsed from begin lesson without pauses
    int state_; // State of typing

    int magicLeftPos_;
    int magicRightPos_;
};

#endif // TYPE_H
