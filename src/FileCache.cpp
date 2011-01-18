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

#include <File.h>
#include <FileCache.h>

////////////////////////////////////////////////////////////////////////////////
FileCache::FileCache (const std::string& r)
: root (r)
, log (NULL)
{
}

////////////////////////////////////////////////////////////////////////////////
FileCache::~FileCache ()
{
}

////////////////////////////////////////////////////////////////////////////////
void FileCache::setLog (Log* l)
{
  log = l;
}

////////////////////////////////////////////////////////////////////////////////
std::string FileCache::load (const std::string& path)
{
  // If the path is not yet in the cache
  std::map <std::string, std::string>::iterator i;
  i = contents.find (path);
  if (i == contents.end ())
  {
    if (log)
      log->write ("FileCache cache miss for " + path);

    // Load file
    std::string content;
    File file (path);
    file.read (content);

    // Set cache
    contents[path] = content;
    mtime[path] = file.mtime ();

    return content;
  }

  return contents[path];
}

////////////////////////////////////////////////////////////////////////////////
void FileCache::refresh ()
{
/*
  for each file in cache
    if captured mtime is older than 5 mins
      if mtime has changed
        delete cache entry
*/
}

////////////////////////////////////////////////////////////////////////////////
