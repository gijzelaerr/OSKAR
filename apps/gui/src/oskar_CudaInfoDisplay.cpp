/*
 * Copyright (c) 2012-2014, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "apps/gui/oskar_CudaInfoDisplay.h"

#include <QtCore/QProcess>
#include <QtGui/QApplication>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QScrollBar>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

oskar_CudaInfoDisplay::oskar_CudaInfoDisplay(QString processName,
        QWidget *parent) : QDialog(parent)
{
    // Set up the GUI.
    setWindowTitle("CUDA System Info");
    QVBoxLayout* vLayoutMain = new QVBoxLayout(this);

    // Create terminal output display.
    display_ = new QTextEdit(this);
    display_->setReadOnly(true);
    display_->setMinimumSize(600, 300);
    QFont terminalFont;
#ifdef Q_OS_WIN32
    terminalFont.setFamily("Lucida Console");
#else
    terminalFont.setFamily("DejaVu Sans Mono");
#endif
    terminalFont.setPointSize(10);
    terminalFont.setStyleHint(QFont::TypeWriter);
    display_->setFont(terminalFont);
    vLayoutMain->addWidget(display_);

    // Create close button.
    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok,
            Qt::Horizontal, this);
    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    vLayoutMain->addWidget(buttons);

    // Run the process.
    process_ = new QProcess(this);
    process_->setProcessChannelMode(QProcess::MergedChannels);
    connect(process_, SIGNAL(readyRead()), this, SLOT(readProcess()));
    process_->start(processName, QProcess::ReadOnly);
}

// Private slots.

void oskar_CudaInfoDisplay::readProcess()
{
    QString result = QString(process_->readAll());
    if (result.size() > 0)
    {
        // Append the output text.
        QTextCursor cursor = display_->textCursor();
        cursor.movePosition(QTextCursor::End);
        display_->setTextCursor(cursor);
        display_->insertPlainText(result);
        display_->verticalScrollBar()->setValue(0);
    }
}
