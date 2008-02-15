
/***************************************************************************
 *  ballposlog_plugin.h - Fawkes ball position log plugin for demonstration
 *
 *  Created: Thu Jan 24 16:58:01 2008
 *  Copyright  2008  Tim Niemueller [www.niemueller.de]
 *
 *  $Id$
 *
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __PLUGINS_BALLPOSLOG_BALLPOSLOG_PLUGIN_H_
#define __PLUGINS_BALLPOSLOG_BALLPOSLOG_PLUGIN_H_

#include <core/plugin.h>

class BallPosLogPlugin : public Plugin
{
 public:
      BallPosLogPlugin();
};

#endif