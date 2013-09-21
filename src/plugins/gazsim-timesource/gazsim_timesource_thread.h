/***************************************************************************
 *  gazsim_timesource_plugin.cpp - Plugin sets the fawkes time
 *                                 to the simulation time
 *
 *  Created: Sat Sep 21 20:56:38 2013
 *  Copyright  2013  Frederik Zwilling
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

#ifndef __PLUGINS_GAZSIM__TIMESOURCE_THREAD_H_
#define __PLUGINS_GAZSIM__TIMESOURCE_THREAD_H_

#include <core/threading/thread.h>
#include <aspect/clock.h>
#include <aspect/logging.h>
#include <aspect/configurable.h>
#include <aspect/blocked_timing.h>
#include <boost/asio.hpp>
#include <google/protobuf/message.h>

#include <gazebo/physics/physics.hh>

class GazsimTimesourceThread
: public fawkes::Thread,
  public fawkes::ClockAspect,
  public fawkes::BlockedTimingAspect,
  public fawkes::ConfigurableAspect,
  public fawkes::LoggingAspect
{
 public:
  GazsimTimesourceThread();
  ~GazsimTimesourceThread();

  virtual void init();
  virtual void loop();
  virtual void finalize();

 private:

  //config values
  std::string world_name_;

  //world to get the time from
  //gazebo::physics::WorldPtr world_;
};

#endif
