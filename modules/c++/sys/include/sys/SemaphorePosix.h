/* =========================================================================
 * This file is part of sys-c++ 
 * =========================================================================
 * 
 * (C) Copyright 2004 - 2014, MDA Information Systems LLC
 *
 * sys-c++ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this program; If not, 
 * see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef __SYS_SEMAPHORE_POSIX_H__
#define __SYS_SEMAPHORE_POSIX_H__

#include <sys/sys_config.h>

#if defined(HAVE_PTHREAD_H)// && !defined(__APPLE_CC__)

#include "sys/SemaphoreInterface.h"

#if defined(__APPLE_CC__)
//#error __APPLE_CC_H__ defined!
#  include </usr/include/sys/semaphore.h>
#else
//#error __APPLE_CC_H__ not defined!
#  include <semaphore.h>
#endif

namespace sys
{
  //typedef ::sem_t sem_t;
  //typedef int sem_t;

class SemaphorePosix : public SemaphoreInterface
{
public:
    SemaphorePosix(unsigned int count = 0);
    virtual ~SemaphorePosix();
    void wait();
    void signal();
    sem_t& getNative();
    
    /*!
     *  Return the type name.  This function is essentially free,
     *  because it is static RTTI.
     */
    const char* getNativeType() const
    {
        return typeid(mNative).name();
    }
private:
    sem_t mNative;
};
}

#endif
#endif
