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

#include <dlfcn.h>
#include <malloc.h>

#include "qmalloc.h"

typedef void *(*mallocptr)(size_t);
typedef void (*freeptr)(void*);
typedef void *(*reallocptr)(void*, size_t);

size_t MallocStats::m_totalAllocations = 0;
size_t MallocStats::m_totalReallocations = 0;
size_t MallocStats::m_totalFrees = 0;
size_t MallocStats::m_totalBytesAllocated = 0;
size_t MallocStats::m_totalBytesFreed = 0;

size_t MallocStats::totalAllocations()
{
    return m_totalAllocations;
}

size_t MallocStats::totalReallocations()
{
    return m_totalReallocations;
}

size_t MallocStats::totalFrees()
{
    return m_totalFrees;
}

size_t MallocStats::totalBytesAllocated()
{
    return m_totalBytesAllocated;
}

size_t MallocStats::totalBytesFreed()
{
    return m_totalBytesFreed;
}


void MallocStats::clearStats()
{
    m_totalAllocations = 0;
    m_totalReallocations = 0;
    m_totalFrees = 0;
    m_totalBytesAllocated = 0;
    m_totalBytesFreed = 0;
}

/*****************************************************************************
 * intercepted alloc-related functions
 *****************************************************************************/

void *malloc(size_t size)
{
    static mallocptr real_malloc = (mallocptr)dlsym(RTLD_NEXT, "malloc");
    void *ptr = real_malloc(size);
    MallocStats::m_totalAllocations++;
    MallocStats::m_totalBytesAllocated += malloc_usable_size(ptr);
    return ptr;
}

void free(void *ptr)
{
    MallocStats::m_totalFrees++;
    MallocStats::m_totalBytesFreed += malloc_usable_size(ptr);

    static freeptr real_free = (freeptr)dlsym(RTLD_NEXT, "free");
    real_free(ptr);
}

void *realloc(void *ptr, size_t size)
{
    MallocStats::m_totalReallocations++;
    MallocStats::m_totalBytesAllocated -= malloc_usable_size(ptr);

    static reallocptr real_realloc = (reallocptr)dlsym(RTLD_NEXT, "realloc");
    void *nptr = real_realloc(ptr, size);

    MallocStats::m_totalBytesAllocated += malloc_usable_size(nptr);
    return nptr;
}