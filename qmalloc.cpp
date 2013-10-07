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
#include <assert.h>
#include <string.h>

#include "qmalloc.h"

MallocStats::MallocStats()
{
    m_totalAllocations = 0;
    m_totalBytesAllocated = 0;
    m_totalFrees = 0;
    m_totalBytesFreed = 0;
    m_totalReallocations = 0;
    m_totalBytesFreedOnRealloc = 0;
    m_totalBytesAllocedOnRealloc = 0;
    m_lineNumber = 0;
}

void MallocStats::setFileName(const char *fileName)
{
    strncpy(m_fileName, fileName, sizeof(m_fileName));
}

const char *MallocStats::fileName() const
{
    return m_fileName;
}

void MallocStats::setLineNumber(int lineNumber)
{
    m_lineNumber = lineNumber;
}

int MallocStats::lineNumber() const
{
    return m_lineNumber;
}

size_t MallocStats::totalAllocations()
{
    return m_totalAllocations;
}

size_t MallocStats::totalBytesAllocated()
{
    return m_totalBytesAllocated;
}

size_t MallocStats::totalFrees()
{
    return m_totalFrees;
}

size_t MallocStats::totalBytesFreed()
{
    return m_totalBytesFreed;
}

size_t MallocStats::totalReallocations()
{
    return m_totalReallocations;
}

size_t MallocStats::totalBytesFreedOnReallocation()
{
    return m_totalBytesFreedOnRealloc;
}

size_t MallocStats::totalBytesAllocedOnReallocation()
{
    return m_totalBytesAllocedOnRealloc;
}


MallocStats MallocStack::m_allocStack[MAX_STACK_SIZE];
int MallocStack::m_stackPointer = 0;

MallocStats *MallocStack::last()
{
    return &m_allocStack[m_stackPointer];
}

MallocStats *MallocStack::push()
{
    assert(m_stackPointer < MAX_STACK_SIZE);
    m_stackPointer++;
    memset(&m_allocStack[m_stackPointer], 0, sizeof(MallocStats));
    return last();
}

void MallocStack::pop()
{
    m_stackPointer--;
}



/*****************************************************************************
 * intercepted alloc-related functions
 *****************************************************************************/

typedef void *(*mallocptr)(size_t);
typedef void (*freeptr)(void*);
typedef void *(*reallocptr)(void*, size_t);

static mallocptr real_malloc = 0;
static freeptr real_free = 0;
static reallocptr real_realloc = 0;

void *malloc(size_t size)
{
    if (!real_malloc)
        real_malloc = (mallocptr)dlsym(RTLD_NEXT, "malloc");

    void *ptr = real_malloc(size);
    MallocStats *item = MallocStack::last();
    if (item) {
        item->m_totalAllocations++;
        item->m_totalBytesAllocated += malloc_usable_size(ptr);
    }
    return ptr;
}

void free(void *ptr)
{
    if (!real_free)
        real_free = (freeptr)dlsym(RTLD_NEXT, "free");

    MallocStats *item = MallocStack::last();
    if (item) {
        item->m_totalFrees++;
        item->m_totalBytesFreed += malloc_usable_size(ptr);
    }

    real_free(ptr);
}

void *realloc(void *ptr, size_t size)
{
    if (!real_realloc)
        real_realloc = (reallocptr)dlsym(RTLD_NEXT, "realloc");

    MallocStats *item = MallocStack::last();
    if (item) {
        item->m_totalReallocations++;
        item->m_totalBytesFreedOnRealloc += malloc_usable_size(ptr);
    }

    void *nptr = real_realloc(ptr, size);

    if (item) {
        item->m_totalBytesAllocedOnRealloc += malloc_usable_size(nptr);
    }
    return nptr;
}
