
----------------------------------------------------------------------------
--  stringext.lua - string package extensions
--
--  Created: Tue Mar 18 14:43:14 2008
--  Copyright  2008  Tim Niemueller [www.niemueller.de]
--
--  $Id$
--
----------------------------------------------------------------------------

--  This program is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation; either version 2 of the License, or
--  (at your option) any later version.
--
--  This program is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU Library General Public License for more details.
--
--  You should have received a copy of the GNU General Public License
--  along with this program; if not, write to the Free Software Foundation,
--  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.

local _G = _G;
local string = string;

module("general.stringext");

-- Concatenate a list of strings to one string with the given delimiter.
-- @param delimiter string to separate strings
-- @param list the list of strings to concatenate
-- @return string with concatenated strings
function string.join(delimiter, list)
   local len = #list;
   if len == 0 then 
      return "";
   end
   local rv = list[1];
   for i = 2, len do
      rv = rv .. delimiter .. list[i];
   end
   return rv;
end


-- Split string in sub strings.
-- @param delimiter string to split the string at
-- @param text text to split
-- @return list of substrings
function string.split(delimiter, text)
   local list = {};
   local pos = 1;
   if delimiter == "" then
      error("delimiter matches empty string!")
   end
   while 1 do
      local first, last = string.find(text, delimiter, pos)
      if first then -- found?
	 table.insert(list, string.sub(text, pos, first-1))
	 pos = last+1
      else
	 table.insert(list, string.sub(text, pos))
	 break
      end
   end
   return list
end
