
/***************************************************************************
 *  max_circle.h - Laser data circle data filter (example)
 *
 *  Created: Fri Oct 10 17:15:34 2008
 *  Copyright  2006-2011  Tim Niemueller [www.niemueller.de]
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

#ifndef _PLUGINS_LASER_FILTER_FILTERS_MAX_CIRCLE_H_
#define _PLUGINS_LASER_FILTER_FILTERS_MAX_CIRCLE_H_

#include "filter.h"

class LaserMaxCircleDataFilter : public LaserDataFilter
{
 public:
  LaserMaxCircleDataFilter(const std::string& filter_name, float radius,
                           unsigned int in_data_size, std::vector<LaserDataFilter::Buffer *> &in);

  void filter();

 private:
  float  radius_;
};

#endif
