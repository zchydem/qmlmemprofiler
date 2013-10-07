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

#ifndef QMALLOC_H
#define QMALLOC_H

#include <linux/limits.h>

class MallocStats
{
public:
    MallocStats();

    size_t totalAllocations();
    size_t totalReallocations();
    size_t totalFrees();
    size_t totalBytesAllocated();
    size_t totalBytesFreed();

    void setFileName(const char *fileName);
    const char *fileName() const;
    void setLineNumber(int lineNumber);
    int lineNumber() const;

    // use the accessors, I'm just too lazy to friend the alloc funcs
    size_t m_totalAllocations;
    size_t m_totalReallocations;
    size_t m_totalFrees;
    size_t m_totalBytesAllocated;
    size_t m_totalBytesFreed;
    char m_fileName[PATH_MAX];
    int m_lineNumber;
};


#include <QVector>

#define MAX_STACK_SIZE 1000

class MallocStack
{
public:
    static MallocStats *last();
    static MallocStats *push();
    static void pop();

private:
    static MallocStats m_allocStack[MAX_STACK_SIZE];
    static int m_stackPointer;
};

#endif
