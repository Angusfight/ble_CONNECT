/*------------------------------------------------------------------------------
  The MIT License (MIT)

  Copyright (c) 2018 AcroComm Corp. WWW: http://www.acrocomm.com

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Softwarec"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
------------------------------------------------------------------------------*/

#include "mainwindow_ui_impl.h"
#include <QDebug>
#include <QPushButton>
#include <QKeyEvent>

MainWindow_ui_impl::MainWindow_ui_impl(QObject *parent)
    : QObject(parent)
{
  m_lastFocusButton = NULL;
}

bool MainWindow_ui_impl::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        return true;
    }
    return QObject::eventFilter(obj, event);
}

void MainWindow_ui_impl::focusButton(QPushButton *button)
{
    QFont font;

    button->setFocus();
    if (m_lastFocusButton) {
        font = m_lastFocusButton->font();
        font.setPixelSize(12);
        font.setBold(false);
        m_lastFocusButton->setFont(font);
        m_lastFocusButton->setMinimumHeight(50);
    }

    m_lastFocusButton = button;
    font = m_lastFocusButton->font();
    font.setPixelSize(16);
    font.setBold(true);
    m_lastFocusButton->setFont(font);
    m_lastFocusButton->setMinimumHeight(55);

    /****
    m_animationButton(&m_lastFocusButton, "geometry");
    m_animationButton.setDuration(100);
    m_animationButton.setStartValue(QRect(0, 0, 100, 30));
    m_animationButton.setEndValue(QRect(250, 250, 100, 30));
    m_animationButton.start();
    ****/
}
























