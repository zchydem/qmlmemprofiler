 /****************************************************************************
 **
 ** Copyright (C) 2013 Jolla Ltd. <robin.burchell@jollamobile.com>
 ** Contact: http://www.qt-project.org/legal
 **
 ** This file is part of the QtQml module of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:LGPL$
 ** Commercial License Usage
 ** Licensees holding valid commercial Qt licenses may use this file in
 ** accordance with the commercial license agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and Digia.  For licensing terms and
 ** conditions see http://qt.digia.com/licensing.  For further information
 ** use the contact form at http://qt.digia.com/contact-us.
 **
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Digia gives you certain additional
 ** rights.  These rights are described in the Digia Qt LGPL Exception
 ** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
 **
 ** GNU General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU
 ** General Public License version 3.0 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU General Public License version 3.0 requirements will be
 ** met: http://www.gnu.org/copyleft/gpl.html.
 **
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#include <QDebug>

#include "qmalloc.h"


extern "C" {
void qmlmemprofile_stats(int *allocCount, int *bytesAllocated);
void qmlmemprofile_clear();
void qmlmemprofile_enable();
void qmlmemprofile_disable();
void qmlmemprofile_push_location(const char *fileName, int lineNumber);
void qmlmemprofile_pop_location();
void qmlmemprofile_save(const char *filename);
int qmlmemprofile_is_enabled();
}

Q_CORE_EXPORT void qmlmemprofile_stats(int *allocCount, int *bytesAllocated)
{
    qWarning() << "STUB";
    allocCount = 0;
    bytesAllocated = 0;
}

Q_CORE_EXPORT void qmlmemprofile_clear()
{
    qWarning() << "STUB";
}

Q_CORE_EXPORT void qmlmemprofile_enable()
{
    qWarning() << "STUB";
}

Q_CORE_EXPORT void qmlmemprofile_disable()
{
    qWarning() << "STUB";
}

Q_CORE_EXPORT void qmlmemprofile_push_location(const char *fileName, int lineNumber)
{
    MallocStats *item = MallocStack::push();
    item->setFileName(fileName);
    item->setLineNumber(lineNumber);
//    qDebug() << "Recorded " << fileName << " bytes is " << R.ru_maxrss;
}

Q_CORE_EXPORT void qmlmemprofile_pop_location()
{
    MallocStats *item = MallocStack::last();
    qDebug() << "Popped " << item->fileName() << " stats:";

    qDebug() << "    Total allocations: " << item->totalAllocations();
    qDebug() << "    Total reallocations: " << item->totalAllocations();
    qDebug() << "    Total bytes allocated: " << item->totalBytesAllocated();
    qDebug() << "    Total frees: " << item->totalFrees();
    qDebug() << "    Total bytes freed: " << item->totalBytesFreed();

    MallocStack::pop();
}

Q_CORE_EXPORT void qmlmemprofile_save(const char *filename)
{
    qWarning() << "STUB";
}

Q_CORE_EXPORT int qmlmemprofile_is_enabled()
{
    // TODO: environment variable enablement?
    return 1;
}

