
/***************************************************************************
 *  robot_memory_thread.cpp - Robot Memory thread
 *
 *  Created: Sun May 01 13:41:45 2016
 *  Copyright  2016 Frederik Zwilling
 *             2017 Tim Niemueller [www.niemueller.de]
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

#include "robot_memory_thread.h"

#include "interfaces/RobotMemoryInterface.h"

#include <core/threading/mutex.h>
#include <core/threading/mutex_locker.h>
#ifdef USE_TIMETRACKER
#	include <utils/time/tracker.h>
#endif
#include <utils/time/tracker_macros.h>
#include <utils/time/wait.h>

#include <bsoncxx/json.hpp>
#include <chrono>
#include <memory>
#include <string>

using namespace fawkes;

/** @class RobotMemoryThread "robot_memory_thread.h"
 * Thread that provides a robot memory with MongoDB
 * @author Frederik Zwilling
 */

/** Constructor for thread */
RobotMemoryThread::RobotMemoryThread()
: Thread("RobotMemoryThread", Thread::OPMODE_CONTINUOUS),
  AspectProviderAspect(&robot_memory_inifin_)
{
}

/** Destructor. */
RobotMemoryThread::~RobotMemoryThread()
{
}

void
RobotMemoryThread::init()
{
	//init RobotMemory itself
	robot_memory = new RobotMemory(config, logger, clock, mongodb_connmgr, blackboard);
	robot_memory->init();
	//prepare aspect initializer
	robot_memory_inifin_.set_robot_memory(robot_memory);

	//register computables
	blackboard_computable = new BlackboardComputable(robot_memory, blackboard, logger, config);
	transform_computable  = new TransformComputable(robot_memory, tf_listener, logger, config);

	int loop_time_microsec;
	try {
		float loop_interval = config->get_float("/plugins/robot-memory/loop-interval");
		loop_time_microsec  = (int)loop_interval * 1e6;

	} catch (Exception &e) {
		int main_loop_time = config->get_int("/fawkes/mainapp/desired_loop_time");
		if (main_loop_time > 0) {
			loop_time_microsec = main_loop_time;
		} else {
			// use default of 0.1s
			loop_time_microsec = 1e5;
		}
	}
	timewait_ = new TimeWait(clock, loop_time_microsec);

#ifdef USE_TIMETRACKER
	tt_           = new TimeTracker();
	tt_loopcount_ = 0;
	ttc_msgproc_  = tt_->add_class("Message Processing");
	ttc_rmloop_   = tt_->add_class("Robot Memory Processing Loop");
#endif
}

void
RobotMemoryThread::finalize()
{
	delete blackboard_computable;
	delete transform_computable;
	robot_memory_inifin_.set_robot_memory(NULL);
	delete robot_memory;
	delete timewait_;
#ifdef USE_TIMETRACKER
	delete tt_;
#endif
}

void
RobotMemoryThread::loop()
{
	TIMETRACK_START(ttc_msgproc_);
	timewait_->mark_start();
	// process interface messages
	while (!robot_memory->rm_if_->msgq_empty()) {
		if (robot_memory->rm_if_->msgq_first_is<RobotMemoryInterface::QueryMessage>()) {
			RobotMemoryInterface::QueryMessage *msg =
			  (RobotMemoryInterface::QueryMessage *)robot_memory->rm_if_->msgq_first();
			std::string      query = msg->query();
			mongocxx::cursor res   = robot_memory->query(bsoncxx::from_json(query), msg->collection());
			//output result
			std::string result = "Result of query " + query + ":\n";
			while (true) {
				auto doc = res.begin();
				if (doc == res.end()) {
					break;
				}
				result += bsoncxx::to_json(*doc) + "\n";
			}
			logger->log_info(name(), "%s", result.c_str());
			robot_memory->rm_if_->set_result(result.c_str());
		} else if (robot_memory->rm_if_->msgq_first_is<RobotMemoryInterface::InsertMessage>()) {
			RobotMemoryInterface::InsertMessage *msg =
			  (RobotMemoryInterface::InsertMessage *)robot_memory->rm_if_->msgq_first();
			robot_memory->insert(msg->insert()), msg->collection();
		} else if (robot_memory->rm_if_->msgq_first_is<RobotMemoryInterface::UpdateMessage>()) {
			RobotMemoryInterface::UpdateMessage *msg =
			  (RobotMemoryInterface::UpdateMessage *)robot_memory->rm_if_->msgq_first();
			robot_memory->update(bsoncxx::from_json(msg->query()),
			                     bsoncxx::from_json(msg->update()),
			                     msg->collection());
		} else if (robot_memory->rm_if_->msgq_first_is<RobotMemoryInterface::RemoveMessage>()) {
			RobotMemoryInterface::RemoveMessage *msg =
			  (RobotMemoryInterface::RemoveMessage *)robot_memory->rm_if_->msgq_first();
			robot_memory->remove(bsoncxx::from_json(msg->query()), msg->collection());
		} else {
			logger->log_warn(name(), "Unknown message received");
		}

		robot_memory->rm_if_->msgq_pop();
	}
	TIMETRACK_END(ttc_msgproc_);

	TIMETRACK_START(ttc_rmloop_);
	robot_memory->loop();
	TIMETRACK_END(ttc_rmloop_);
#ifdef USE_TIMETRACKER
	if (++tt_loopcount_ % 5 == 0) {
		tt_->print_to_stdout();
	}
#endif

	timewait_->wait_systime();
}
