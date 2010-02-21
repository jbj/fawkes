
/***************************************************************************
 *  logreplay_thread.cpp - BB Log Replay Thread
 *
 *  Created: Wed Feb 17 01:53:00 2010
 *  Copyright  2010  Tim Niemueller [www.niemueller.de]
 *             2010  Masrur Doostdar <doostdar@kbsg.rwth-aachen.de>
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

#include "logreplay_thread.h"
#include "file.h"

#include <blackboard/blackboard.h>
#include <utils/logging/logger.h>
#include <core/threading/wait_condition.h>
#include <core/exceptions/system.h>
#include <utils/misc/autofree.h>

#include <blackboard/internal/instance_factory.h>


#include <memory>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <fcntl.h>
#include <endian.h>
#include <arpa/inet.h>
#include <sys/mman.h>

using namespace fawkes;

/** @class BBLogReplayThread "logreplay_thread.h"
 * BlackBoard log Replay thread.
 * Writes the data of the logfile into a blackboard interface, considering the
 * time-step differences between the data.
 * @author Masrur Doostdar
 * @author Tim Niemueller
 */

/** Constructor.
 * @param logfile_name filename of the log to be replayed
 * @param logdir directory containing the logfile
 * @param scenario ID of the log scenario
 * @param loop_replay specifies if the replay should be looped
 */
BBLogReplayThread::BBLogReplayThread(const char *logfile_name,
				     const char *logdir,
				     const char *scenario,
				     const bool loop_replay)
  : Thread("BBLogReplayThread", Thread::OPMODE_CONTINUOUS)
{
  set_name("BBLogReplayThread(%s)", logfile_name);
  set_prepfin_conc_loop(true);

  __logfile_name= strdup(logfile_name);
  __logdir      = strdup(logdir);
  __scenario    = strdup(scenario); // dont need this!?
  __filename    = NULL;
  __cfg_loop_replay = loop_replay;
}


/** Destructor. */
BBLogReplayThread::~BBLogReplayThread()
{
  free(__logfile_name);
  free(__logdir);
  free(__scenario);
}




void
BBLogReplayThread::init()
{
  __logfile = NULL;
  __interface = NULL;
  __filename = NULL;

  if (asprintf(&__filename, "%s/%s", __logdir, __logfile_name) == -1) {
    throw OutOfMemoryException("Cannot re-generate logfile-path");
  }

  try {
    __logfile = new BBLogFile(__filename, true);
  } catch (Exception &e) {
    finalize();
    throw;
  }

  if (! __logfile->has_next()) {
    finalize();
    throw Exception("Log file %s does not have any entries", __filename);
  }

  __interface = blackboard->open_for_writing(__logfile->interface_type(),
					     __logfile->interface_id());

  try {
    __logfile->set_interface(__interface);
  } catch (Exception &e) {
    finalize();
    throw;
  }

  logger->log_info(name(), "Replaying from %s:", __filename);
}


void
BBLogReplayThread::finalize()
{
  delete __logfile;
  if (__filename)  free(__filename);
  blackboard->close(__interface);
}


void
BBLogReplayThread::once()
{
  // Write first immediately, skip first offset
  __logfile->read_next();
  __interface->write();
  __last_offset = __logfile->entry_offset();
}

void
BBLogReplayThread::loop()
{
  if (__logfile->has_next()) {
    __logfile->read_next();
    __interface->write();

    __diff = __logfile->entry_offset() - __last_offset;
    __diff.wait();
    __last_offset = __logfile->entry_offset();

  } else {
    if(__cfg_loop_replay){
      logger->log_info(name(), "replay finished, looping");
      __logfile->rewind();
    } else {
      // block
      logger->log_info(name(), "replay finished, sleeping");
      WaitCondition waitcond;
      waitcond.wait();
    }
  }
}
