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

#include "mainwindow.h"
#include <QKeyEvent>
#include <QDebug>
#include <QMessageBox>
#include <QFontDatabase>
#include <QScreen>
#include <QProcess>
#include <QFile>
#include <QFileInfo>
#include <QtSerialPort/QtSerialPort>


#include <QBluetoothHostInfo>


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <wait.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <netinet/in.h>

#include "ui_mainwindow.h"


#include "plat/plat.h"
#include "apprun.h"

#include "autoversion.h"

extern plat_thread *PLAT;
extern FILE *LOG;
extern QString SN;


QScreen *SCREEN;
AppRun *APPRUN;


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SCREEN = QGuiApplication::primaryScreen();

    /* Set app to max screen size */
    setWindowState(Qt::WindowFullScreen);
    Qt::WindowFlags flags = windowFlags();
    flags |= (Qt::Window | Qt::BypassWindowManagerHint);
    setWindowFlags(flags);


    /* 1000ms, 1S UI timer */
    ui_DlgTimeout = -1;
    connect(&ui_Timer, SIGNAL(timeout()), this, SLOT(uiTimerTick()));
    ui_Timer.start(1000);


    activateWindow();    
    QTimer::singleShot(0, this, SLOT(asyncInit()));
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::asyncInit()
{
    bleController.startBLE();
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MainWindow::uiTimerTick()
{

}






