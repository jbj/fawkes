
/***************************************************************************
 *  cascade.cpp - Laser data filter cascade
 *
 *  Created: Thu Jun 25 01:07:53 2009
 *  Copyright  2006-2009  Tim Niemueller [www.niemueller.de]
 *
 *  $Id$
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

#include "cascade.h"

/** @class LaserDataFilterCascade "filters/cascade.h"
 * Cascade of several laser filters to one.
 * The filters are executed in the order they are added to the cascade.
 * @author Tim Niemueller
 */

/** Constructor.
 * @param own_filters true to make the cascade own filters that are added, i.e.
 * they are deleted if the cascade is deleted.
 */
LaserDataFilterCascade::LaserDataFilterCascade(bool own_filters)
{
  _free_filtered_data = false;
  __own_filters       = own_filters;
}


/** Destructor. */
LaserDataFilterCascade::~LaserDataFilterCascade()
{
  if (__own_filters)  delete_filters();
}


/** Add a filter to the cascade.
 * @param filter filter to add
 */
void
LaserDataFilterCascade::add_filter(LaserDataFilter *filter)
{
  __filters.push_back(filter);
}


/** Remove a filter from the cascade.
 * @param filter filter to remove
 */
void
LaserDataFilterCascade::remove_filter(LaserDataFilter *filter)
{
  __filters.remove(filter);
}


/** Delete all filters. */
void
LaserDataFilterCascade::delete_filters()
{
  for (__fit = __filters.begin(); __fit != __filters.end(); ++__fit) {
    delete *__fit;
  }
  __filters.clear();
}


void
LaserDataFilterCascade::filter(const float *data, unsigned int data_size)
{
  float *d = (float *)data;
  for (__fit = __filters.begin(); __fit != __filters.end(); ++__fit) {
    (*__fit)->filter(d, data_size);
    (*__fit)->filtered_data(d, data_size);
  }
  _filtered_data = (float *)d;
  _filtered_data_size = data_size;
}
