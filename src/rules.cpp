////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2010 - 2017, Paul Beckingham, Federico Hernandez.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// http://www.opensource.org/licenses/mit-license.php
//
////////////////////////////////////////////////////////////////////////////////

#include <cmake.h>
#include <Rule.h>
#include <FS.h>
#include <shared.h>
// If <iostream> is included, put it after <stdio.h>, because it includes
// <stdio.h>, and therefore would ignore the _WITH_GETLINE.
#ifdef FREEBSD
#define _WITH_GETLINE
#endif
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

////////////////////////////////////////////////////////////////////////////////
// - Read rc file
// - Strip comments
// - Parse rules
//
// Note that it is an error to not have an rc file.
bool loadRules (const std::string& file, std::vector <Rule>& rules)
{
  std::ifstream rc (file.c_str ());
  if (rc.good ())
  {
    std::string::size_type comment;
    std::string line;
    while (std::getline (rc, line)) // Strips \n
    {
      // Remove comments.
      if ((comment = line.find ('#')) != std::string::npos)
        line.resize (comment);

      // Process each non-trivial line as a rule.
      if (line.length () > 1)
      {
        auto words = split (line);
        if (words.size () == 2 &&
            words[0] == "include")
        {
          // File::File expands relative paths, and ~user.
          File f (words[1]);
          if (! loadRules (f._data, rules))
            return false;
        }
        else
        {
          try
          {
            rules.push_back (Rule (line));
          }
          catch (int)
          {
            // Deliberately ignored - error handling.
          }
        }
      }
    }

    rc.close ();
    return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////
