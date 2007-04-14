/***************************************************************************
 *  config.h - config helper - generated by genconfig 0.3
 *
 *  Config class generated: Sat Apr 14 11:56:45 2007
 *  Used template: ../../../../../src/modules/tools/genconfig/config.template.h
 *  Template created: Thu Apr 29 14:26:23 2004
 *  Copyright  2004  Tim Niemueller
 *  niemueller@i5.informatik.rwth-aachen.de
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
 
#ifndef ___CANNIKINCONFIG_H_
#define ___CANNIKINCONFIG_H_

/// @cond RCSOFTX_CONFIG

#include <utils/config_reader/config_reader.h>
#include <string>

class CannikinConfig
{
 public:
  CannikinConfig(CConfigReader *configFile);
  ~CannikinConfig();

  /** ScanlineGridYOffset value */
  int ScanlineGridYOffset;
  /** ScanlineModel value */
  std::string ScanlineModel;
  /** ClassifierType value */
  std::string ClassifierType;
  /** ScanlineGridXOffset value */
  int ScanlineGridXOffset;
  /** CameraOffsetY value */
  float CameraOffsetY;
  /** ColormapFilestem value */
  std::string ColormapFilestem;
  /** CameraHeight value */
  float CameraHeight;
  /** ColormapDirectory value */
  std::string ColormapDirectory;
  /** LookupTableWidth value */
  int LookupTableWidth;
  /** CameraBearing value */
  float CameraBearing;
  /** ColorModel value */
  std::string ColorModel;
  /** CameraSlope value */
  float CameraSlope;
  /** ShrinkerType value */
  std::string ShrinkerType;
  /** LookupTableHeight value */
  int LookupTableHeight;
  /** CameraString value */
  std::string CameraString;
  /** CameraOffsetX value */
  float CameraOffsetX;


 protected:
  /** Config reader */
  CConfigReader * m_pXMLConfigFile;
  /** Current config prefix. */
  std::string m_sPrefix;

};

/// @endcond

#endif /* ___CANNIKINCONFIG_H_ */

