/*
 * resultdlg.cpp
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

#include <QDebug>

#include "resultdlg.h"
#include "ui_resultdlg.h"

ResultDlg::ResultDlg(int elapsed, int typed, int totalSymbols, int errors, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ResultDlg)
{
    ui->setupUi(this);
    adjustSize();

    QString elapsedStr = QString("%1 : %2").arg(elapsed / 60).arg(elapsed % 60);
    ui->lblElapsed->setText(elapsedStr);

    if (totalSymbols)
        ui->lblTyped->setText(QString(tr("%1 of %2")).arg(typed).arg(totalSymbols));
    else
        ui->lblTyped->setText(QString::number(typed));

    ui->lblSpeed->setText(QString(tr("%1 cpm")).arg(60 * typed / elapsed));
    ui->lblErrors->setText(QString("%1 %").arg(errors * 100.0 / typed));

    if (typed == totalSymbols)
        ui->btnResume->setEnabled(false);
}

void ResultDlg::finish()
{
    done(Finish);
}

void ResultDlg::resume()
{
    done(Resume);
}
