
/***************************************************************************
 *  navgraph_inifin.cpp - Fawkes NavGraphAspect initializer/finalizer
 *
 *  Created: Mon Dec 06 22:33:03 2010
 *  Copyright  2006-2010  Tim Niemueller [www.niemueller.de]
 *             2014       Sebastian Reuter
 ****************************************************************************/

/*  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version. A runtime exception applies to
 *  this software (see LICENSE.GPL_WRE file mentioned below for details).
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  Read the full text in the LICENSE.GPL_WRE file in the doc directory.
 */

#include <plugins/navgraph/aspect/navgraph_inifin.h>
#include <plugins/navgraph/aspect/navgraph.h>
#include <core/threading/thread_finalizer.h>
#include <utils/graph/topological_map_graph.h>
#include <plugins/navgraph/constraints/constraint_repo.h>
#include <cstddef>

namespace fawkes {
#if 0 /* just to make Emacs auto-indent happy */
}
#endif

/** @class NavGraphAspectIniFin <plugins/navgraph/aspect/navgraph_inifin.h>
 * NavGraphAspect initializer/finalizer.
 * @author Tim Niemueller
 */

/** Constructor. */
NavGraphAspectIniFin::NavGraphAspectIniFin()
  : AspectIniFin("NavGraphAspect")
{
}


/** Set navgraph.
 * @param navgraph navgraph to pass to thread with the NavGraphAspect
 */
void
NavGraphAspectIniFin::set_navgraph(LockPtr<TopologicalMapGraph> &navgraph)
{
  navgraph_ = navgraph;
}

/** Set Constraint Repository.
 * @param constraint_repo to pass to thread with the NavGraphAspect
 */
void
NavGraphAspectIniFin::set_constraint_repo(LockPtr<ConstraintRepo> &constraint_repo)
{
  constraint_repo_ = constraint_repo;
}

void
NavGraphAspectIniFin::init(Thread *thread)
{
  NavGraphAspect *navgraph_thread;
  navgraph_thread = dynamic_cast<NavGraphAspect *>(thread);
  if (navgraph_thread == NULL) {
    throw CannotInitializeThreadException("Thread '%s' claims to have the "
					  "NavGraphAspect, but RTTI says it "
					  "has not. ", thread->name());
  }

  navgraph_thread->navgraph = navgraph_;
  navgraph_thread->constraint_repo = constraint_repo_;
}

void
NavGraphAspectIniFin::finalize(Thread *thread)
{
  NavGraphAspect *navgraph_thread;
  navgraph_thread = dynamic_cast<NavGraphAspect *>(thread);
  if (navgraph_thread == NULL) {
    throw CannotFinalizeThreadException("Thread '%s' claims to have the "
					"NavGraphAspect, but RTTI says it "
					"has not. ", thread->name());
  }

  navgraph_thread->navgraph.clear();
  navgraph_thread->constraint_repo.clear();
}



} // end namespace fawkes
