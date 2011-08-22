/*
 * type.cpp
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

#include <QPainter>
#include <QRect>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QDebug>

#include "resultdlg.h"
#include "type.h"

Type::Type(QWidget *parent)
    : QWidget(parent)
    , phrases_(QStringList())
    , text_("")
    , position_(0)
    , widthBorder_(10)
    , colorBorder_(QColor(Qt::darkCyan))
    , fgndLeft_(QColor(Qt::darkGray))
    , fgndRight_(QColor(Qt::black))
    , bgndLeft_(QColor(Qt::lightGray))
    , bgndRight_(QColor(Qt::white))
    , sizeLeft_(0.5)
    , font_(QFont("Libertation Mono", 24))
    , errors_(0)
    , typedLetters_(0)
    , timer_(QTime())
    , allTime_(0)
    , state_(Stopped)
    , magicLeftPos_(0)
    , magicRightPos_(0)
{
    update();
}

void Type::setPhrases(const QStringList &phrases)
{
    phrases_ = phrases;
    if (!phrases_.isEmpty())
    {
        QString left = phrases_[qrand() % phrases_.size()];
        QString right = phrases_[qrand() % phrases_.size()];
        text_ = left + right;
        magicRightPos_ = left.size();
    }
    else
    {
        magicRightPos_ = 0;
    }
}

QTime Type::getTime()
{
    QTime t(0, 0, 0);
    if (state_ == Started)
    {
        allTime_ += timer_.restart();
    }

    t = t.addMSecs(allTime_);
    return t;
}

float Type::getErrors()
{
    return errors_ / position_ * 100;
}

void Type::start()
{
    if (state_ == Stopped)
    {
        position_ = 0;
        errors_ = 0;
        typedLetters_ = 0;
        allTime_ = 0;
    }
    timer_.start();
    state_ = Started;
    emit started();
    update();
}

void Type::stop()
{
    state_ = Stopped;
    emit stopped();
}

void Type::pause()
{
    if (state_ == Started)
    {
        allTime_ += timer_.elapsed();
    }

    state_ = Paused;
    update();
    ResultDlg dlg(allTime_ / 1000, typedLetters_, magicRightPos_ ? 0 : text_.size(), errors_, this);
    if (dlg.exec() == ResultDlg::Finish)
        stop();
    else
        start();
}

void Type::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QFontMetrics metrics(font_);
    QRect rectLeft(widthBorder_, widthBorder_,
                   (width() - 2 * widthBorder_) * sizeLeft_, metrics.height());

    QRect rectRight(rectLeft.right(), rectLeft.top(),
                    (width() - 2 * widthBorder_) * (1.0 - sizeLeft_), metrics.height());

    int h = metrics.height() + widthBorder_*2;
    setMinimumHeight(h);
    setMaximumHeight(h);

    QString text;
    int pos;

    switch (state_)
    {
    case Started:
    case Paused:
        text = text_;
        pos = position_;
        break;

    case Stopped:
        text = tr("Press SPACE to start");
        pos = text.length() / 2;
        break;
    }


    // Left
    QPixmap pixmapLeft(rectLeft.size());
    pixmapLeft.fill(bgndLeft_);
    QPainter painter(&pixmapLeft);
    painter.setFont(font_);
    QString left = text.left(pos);
    painter.drawText(rectLeft.width() - metrics.width(left),
                     rectLeft.height() - metrics.descent(), left);
    painter.end();

    // Right
    QPixmap pixmapRight(rectRight.size());
    pixmapRight.fill(bgndRight_);
    painter.begin(&pixmapRight);

    painter.setFont(font_);
    QString right = text.right(text.length() - pos);
    painter.drawText(0, rectRight.height() - metrics.descent(), right);
    painter.end();

    painter.begin(this);
    painter.drawPixmap(rectLeft, pixmapLeft);
    painter.drawPixmap(rectRight, pixmapRight);

    // Border
    QPen pen(QBrush(colorBorder_), (qreal)widthBorder_, Qt::SolidLine,
             Qt::SquareCap, Qt::RoundJoin);

    painter.setPen(pen);
    painter.drawRect(widthBorder_ / 2, widthBorder_ / 2,
                     width() - widthBorder_, height() - widthBorder_);
    painter.end();
}

void Type::keyPressEvent(QKeyEvent *e)
{
    if (text_.isEmpty())
        return;

    QChar curCh = text_[position_];

    switch (state_)
    {
    case Started:
        if (e->key() == Qt::Key_Escape)
        {
            pause();
            return;
        }

        if (!e->text().isEmpty())
        {
            QChar ch = e->text()[0];
            if (curCh == ch)
            {
                typedLetters_++;
                if (++position_ == text_.length() && phrases_.isEmpty())
                {
                    ResultDlg dlg(allTime_ / 1000, typedLetters_, text_.size(), errors_, this);
                    dlg.exec();
                    stop();
                }
                else
                {
                    if (position_ == magicRightPos_)
                    {
                        text_ = text_.mid(magicLeftPos_);
                        position_ -= magicLeftPos_;
                        magicLeftPos_ = magicRightPos_ - magicLeftPos_;
                        magicRightPos_ = text_.size();
                        text_ += phrases_[qrand() % phrases_.size()];
                    }
                    update();
                }
            }
            else
            {
                errors_++;
            }
        }
        break;

    case Stopped:
    case Paused:
        if (e->text() == " ")
            start();
        break;
    }

}
