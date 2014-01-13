
/***************************************************************************
 *  pcl_db_merge_roscomm_thread.cpp - ROS communication for pcl-db-merge
 *
 *  Created: Thu Dec 06 13:54:45 2012
 *  Copyright  2012  Tim Niemueller [www.niemueller.de]
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

#include "pcl_db_roscomm_thread.h"
#include <interfaces/PclDatabaseMergeInterface.h>
#include <interfaces/PclDatabaseRetrieveInterface.h>

#include <core/threading/wait_condition.h>
#include <blackboard/utils/on_update_waker.h>

#include <ros/ros.h>

using namespace fawkes;

#define CFG_PREFIX "/perception/pcl-db-merge/ros/"

/** @class PointCloudDBROSCommThread "pcl_db_merge_roscomm_thread.h"
 * Thread to merge point clouds from database on request.
 * @author Tim Niemueller
 */


/** Constructor. */
PointCloudDBROSCommThread::PointCloudDBROSCommThread()
  : Thread("PointCloudDBROSCommThread", Thread::OPMODE_WAITFORWAKEUP),
    BlockedTimingAspect(BlockedTimingAspect::WAKEUP_HOOK_WORLDSTATE)
{
}


/** Destructor. */
PointCloudDBROSCommThread::~PointCloudDBROSCommThread()
{
}


void
PointCloudDBROSCommThread::init()
{
  merge_if_ =
    blackboard->open_for_reading<PclDatabaseMergeInterface>("PCL Database Merge");

  merge_waitcond_ = new WaitCondition();
  merge_update_waker_ = new BlackBoardOnUpdateWaker(blackboard, merge_if_, this);

  retrieve_if_ =
    blackboard->open_for_reading<PclDatabaseRetrieveInterface>("PCL Database Retrieve");

  retrieve_waitcond_ = new WaitCondition();
  retrieve_update_waker_ = new BlackBoardOnUpdateWaker(blackboard, retrieve_if_, this);

  srv_merge_ = new ros::ServiceServer();
  srv_retrieve_ = new ros::ServiceServer();
  srv_record_ = new ros::ServiceServer();

  *srv_merge_ = rosnode->advertiseService("/pcl_db_merge/merge",
					  &PointCloudDBROSCommThread::merge_cb, this);

  *srv_retrieve_ = rosnode->advertiseService("/pcl_db_merge/retrieve",
					     &PointCloudDBROSCommThread::retrieve_cb, this);

  *srv_record_ = rosnode->advertiseService("/pcl_db_merge/record",
					   &PointCloudDBROSCommThread::record_cb, this);
}

void
PointCloudDBROSCommThread::finalize()
{
  srv_merge_->shutdown();
  srv_retrieve_->shutdown();
  srv_record_->shutdown();
  delete srv_merge_;
  delete srv_retrieve_;
  delete srv_record_;

  delete merge_update_waker_;
  delete retrieve_update_waker_;
  delete merge_waitcond_;
  delete retrieve_waitcond_;

  blackboard->close(merge_if_);
  blackboard->close(retrieve_if_);
}


void
PointCloudDBROSCommThread::loop()
{
  merge_if_->read();
  if (merge_if_->changed()) {
    logger->log_info(name(), "Merge interface has changed");

    logger->log_info(name(), "%u vs. %u   final: %s",
		     merge_if_->msgid(), merge_msg_id_, merge_if_->is_final() ? "yes" : "no");

    if ((merge_if_->msgid() == merge_msg_id_) && merge_if_->is_final()) {
      logger->log_info(name(), "Merge final");
      merge_waitcond_->wake_all();
    }
  }

  retrieve_if_->read();
  if (retrieve_if_->changed()) {
    logger->log_info(name(), "Retrieve interface has changed");

    logger->log_info(name(), "%u vs. %u   final: %s",
		     retrieve_if_->msgid(), retrieve_msg_id_,
		     retrieve_if_->is_final() ? "yes" : "no");

    if ((retrieve_if_->msgid() == retrieve_msg_id_) && retrieve_if_->is_final()) {
      logger->log_info(name(), "Retrieve final");
      retrieve_waitcond_->wake_all();
    }
  }
}

bool
PointCloudDBROSCommThread::merge_cb(hybris_c1_msgs::MergePointClouds::Request  &req,
				    hybris_c1_msgs::MergePointClouds::Response &resp)
{
  PclDatabaseMergeInterface::MergeMessage *mm =
    new PclDatabaseMergeInterface::MergeMessage();

  if (req.timestamps.size() > mm->maxlenof_timestamps()) {
    logger->log_warn(name(), "Number of requested timestamps (%zu) "
		     "exceeds maximum number allowed (%zu)",
		     req.timestamps.size(), mm->maxlenof_timestamps());
    resp.ok = false;
    resp.error = "Number of requested timestamps exceeds maximum number allowed";
    delete mm;
    return true;
  }
  if (req.timestamps.empty()) {
    logger->log_warn(name(), "No times given in request");
    resp.ok = false;
    resp.error = "No times given in request";
    delete mm;
    return true;
  }

  size_t num_timestamps = mm->maxlenof_timestamps();
  std::vector<int64_t> timestamps(num_timestamps, 0);
  for (size_t i = 0; i < req.timestamps.size(); ++i) {
    timestamps[i] = (int64_t)req.timestamps[i].sec * 1000L
      + (int64_t)req.timestamps[i].nsec / 1000000L;
  }
  sort(timestamps.begin(), timestamps.begin() + req.timestamps.size());
  mm->set_timestamps(&timestamps[0]);
  mm->set_collection(req.collection.c_str());

  mm->ref();
  merge_if_->msgq_enqueue(mm);
  merge_msg_id_ = mm->id();
  mm->unref();

  // wait for result
  merge_waitcond_->wait();

  // Check result
  merge_if_->read();
  if (merge_if_->is_final() && (std::string("") == merge_if_->error())) {
    resp.ok = true;
  } else {
    resp.ok = false;
    resp.error = merge_if_->error();
  }
  return true;
}


bool
PointCloudDBROSCommThread::retrieve_cb(hybris_c1_msgs::RetrievePointCloud::Request  &req,
				       hybris_c1_msgs::RetrievePointCloud::Response &resp)
{
  PclDatabaseRetrieveInterface::RetrieveMessage *mm =
    new PclDatabaseRetrieveInterface::RetrieveMessage();

  int64_t timestamp = (int64_t)req.timestamp.sec * 1000L
    + (int64_t)req.timestamp.nsec / 1000000L;

  logger->log_info(name(), "Restoring %lli from %s", timestamp, req.collection.c_str());

  mm->set_timestamp(timestamp);
  mm->set_collection(req.collection.c_str());

  mm->ref();
  retrieve_if_->msgq_enqueue(mm);
  retrieve_msg_id_ = mm->id();
  mm->unref();

  // wait for result
  retrieve_waitcond_->wait();

  // Check result
  retrieve_if_->read();
  if (retrieve_if_->is_final() && (std::string("") == retrieve_if_->error())) {
    resp.ok = true;
  } else {
    resp.ok = false;
    resp.error = retrieve_if_->error();
  }
  return true;
}


bool
PointCloudDBROSCommThread::record_cb(hybris_c1_msgs::RecordData::Request  &req,
				     hybris_c1_msgs::RecordData::Response &resp)
{
  logger->log_info(name(), "Recording ordered for %f sec", req.range.toSec());
  ros::Time begin = ros::Time::now();
  ros::Time end   = begin + req.range;
  ros::Time::sleepUntil(end);
  resp.begin = begin;
  resp.end   = end;
  resp.ok    = true;
  logger->log_info(name(), "Recording done");
  return true;
}
