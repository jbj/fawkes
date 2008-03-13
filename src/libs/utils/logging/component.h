
/***************************************************************************
 *  component.h - Component logger
 *
 *  Created: Wed Mar 12 23:45:32 2008
 *  Copyright  2006-2008  Tim Niemueller [www.niemueller.de]
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
 *  along with this program; if not, write to the Free Software Foundation,
 *  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.
 */

#ifndef __UTILS_LOGGING_COMPONENT_H_
#define __UTILS_LOGGING_COMPONENT_H_

#include <core/exception.h>
#include <string>

class Logger;

class ComponentLogger
{
 public:
  ComponentLogger(Logger *logger, const char *component);
  ~ComponentLogger();

  void log_debug(const char *format, ...); 
  void log_info(const char *format, ...);
  void log_warn(const char *format, ...);
  void log_error(const char *format, ...);

  void log_debug(std::string message); 
  void log_info(std::string message);
  void log_warn(std::string message);
  void log_error(std::string message);

  void log_debug(Exception &e);
  void log_info(Exception &e);
  void log_warn(Exception &e);
  void log_error(Exception &e);

 private:
  Logger *__logger;
  char *__component;
};

#endif

