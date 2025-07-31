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
#include <QApplication>
#include <QLoggingCategory>
#include <QFile>
#include <QDebug>
#include <X11/Xlib.h>
#include "plat/plat.h"
#include "plat/plat_signal.h"

#include "autoversion.h"


int main(int argc, char *argv[])
{
    int exit_code;

    setenv("TZ", "UTC-8", 0);
    setenv("DISPLAY", ":0.0", 0);

    /* Enable/Disable Qt Bluetooth debug message */
    QLoggingCategory::setFilterRules( "qt.bluetooth.bluez.debug=false\n"
                                      "qt.bluetooth.debug=false");


    QApplication app(argc, argv);

    /* Load qt style sheet from resource file */
    QFile qssFile( ":/resources/qss/app_ENG.qss" );
    if (qssFile.open( QFile::ReadOnly )) {
        /* Apply stylesheet */
        QString style( qssFile.readAll() );
        app.setStyleSheet( style );
        qDebug() << "Load qss file ok";
    } else {
        qDebug() << "Load qss file error";
    }


    MainWindow win;
    win.show();
    QApplication::setActiveWindow(&win);

    exit_code = app.exec();

    return exit_code;
}



