
/***************************************************************************
 *  info_thread.h - Kinova plugin Jaco information thread
 *
 *  Created: Thu Jun 13 19:14:20 2013
 *  Copyright  2013  Bahram Maleki-Fard
 *
 ****************************************************************************/

/*  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  Read the full text in the LICENSE.GPL file in the doc directory.
 */

#ifndef __PLUGINS_KINOVA_KINOVA_INFO_THREAD_H_
#define __PLUGINS_KINOVA_KINOVA_INFO_THREAD_H_

#include <libkindrv/types.h>

#include <core/threading/thread.h>
#include <aspect/blocked_timing.h>
#include <aspect/logging.h>
#include <aspect/configurable.h>
#include <aspect/blackboard.h>

#include <string>

namespace fawkes {
  typedef struct jaco_arm_struct jaco_arm_t;
}

class KinovaInfoThread
: public fawkes::Thread,
  public fawkes::BlockedTimingAspect,
  public fawkes::LoggingAspect,
  public fawkes::ConfigurableAspect,
  public fawkes::BlackBoardAspect
{
 public:
  KinovaInfoThread();
  virtual ~KinovaInfoThread();

  virtual void init();
  virtual void finalize();
  virtual void loop();

  virtual void register_arm(fawkes::jaco_arm_t *arm);
  virtual void unregister_arms();

 /** Stub to see name in backtrace for easier debugging. @see Thread::run() */
 protected: virtual void run() { Thread::run(); }

 private:
  std::list<fawkes::jaco_arm_t*>           *__arms;
  std::list<fawkes::jaco_arm_t*>::iterator  __arm;

  KinDrv::jaco_position_t    __cpos;
  KinDrv::jaco_position_t    __apos;
};


#endif
