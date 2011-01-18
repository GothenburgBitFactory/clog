////////////////////////////////////////////////////////////////////////////////
// taskwarrior - a command line task list manager.
//
// Copyright 2010 - 2011, Paul Beckingham, Federico Hernandez.
// All rights reserved.
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the
//
//     Free Software Foundation, Inc.,
//     51 Franklin Street, Fifth Floor,
//     Boston, MA
//     02110-1301
//     USA
//
////////////////////////////////////////////////////////////////////////////////
#ifndef INCLUDED_FILECACHE
#define INCLUDED_FILECACHE

#include <map>
#include <string>
#include <Log.h>

class FileCache
{
public:
  FileCache (const std::string&);
  FileCache (const FileCache&);            // Copy constructor
  FileCache& operator= (const FileCache&); // Assignment operator
  ~FileCache ();                           // Destructor

  void setLog (Log*);
  std::string load (const std::string&);

private:
  void refresh ();

private:
  std::string root;
  std::map <std::string, std::string> contents;
  std::map <std::string, time_t> mtime;
  Log* log;
};

#endif
////////////////////////////////////////////////////////////////////////////////
