////////////////////////////////////////////////////////////////////////////////
// clog - a colorizing log filter
//
// Copyright 2011, Göteborg Bit Factory.
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

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <time.h>

#include <Rule.h>

////////////////////////////////////////////////////////////////////////////////
// - Locate $HOME
// - Read $HOME/.clogrc
// - Strip comments
// - Parse rules
void loadRules (std::vector <Rule>& rules)
{
  // Locate $HOME.
  struct passwd* pw = getpwuid (getuid ());
  if (!pw)
    throw std::string ("Could not read home directory from the passwd file.");

  std::string file = pw->pw_dir;
  file += "/.clogrc";

  // Read $HOME/.clogrc
  std::ifstream rc (file.c_str ());
  if (rc.good ())
  {
    std::string::size_type comment;
    std::string line;
    while (getline (rc, line)) // Strips \n
    {
      // Remove comments.
      if ((comment = line.find ('#') != std::string::npos))
        line = line.substr (0, comment);

      // Process each non-trivial line as a rule.
      if (line.length () > 1)
      {
        try
        {
          rules.push_back (Rule (line));
        }
        catch (int)
        {
        }
      }
    }

    rc.close ();
  }
  else
    std::cout << "Cannot open file.cpp" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
void applyRules (
  std::vector <Rule>& rules,
  std::vector <std::string>& sections,
  std::string& line)
{
  std::vector <std::string>::const_iterator section;
  for (section = sections.begin (); section != sections.end (); ++section)
  {
    std::vector <Rule>::iterator rule;
    for (rule = rules.begin (); rule != rules.end (); ++rule)
    {
      // Modify line accordingly.
      rule->apply (*section, line);

/*
      // Stop after first rule match.
      // TODO Configurable?
      return;
*/
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char** argv)
{
  int status = 0;

  try
  {
    std::vector <std::string> sections;
    bool prepend_date = false;
    bool prepend_time = false;

    for (int i = 1; i < argc; ++i)
    {
      if (!strcmp (argv[i], "-h") ||
          !strcmp (argv[i], "--help"))
      {
        std::cout << "\n"
                  << "Usage: clog [--help] [--version] [--date] [--time] "
                  << "[ <section> ... ]\n"
                  << "\n";
        return status;
      }

      else if (!strcmp (argv[i], "-v") ||
               !strcmp (argv[i], "--version"))
      {
        std::cout << "\n"
                  << "clog 0.9.0 build for xxxx\n"
                  << "\n"
                  << "Copyright (C) 2011 Göteborg Bit Factory\n"
                  << "\n"
                  << "clog may be copied only under the terms of the GNU "
                     "General Public License, which may be found in the "
                     "source kit.\n"
                  << "\n"
                  << "Documentation for clog can be found using 'man clog' "
                     "or at http://yootabory.org\n"
                  << "\n";
        return status;
      }

      else if (!strcmp (argv[i], "-d") ||
               !strcmp (argv[i], "--date"))
      {
        prepend_date = true;
      }

      else if (!strcmp (argv[i], "-t") ||
               !strcmp (argv[i], "--time"))
      {
        prepend_time = true;
      }

      else
      {
        sections.push_back (argv[i]);
        std::cout << "# section '" << argv[i] << "'\n";
      }
    }

    // Use a default section if one was not specified.
    if (sections.size () == 0)
      sections.push_back ("default");

    // Read ~/.clogrc
    std::vector <Rule> rules;
    loadRules (rules);

    // Main loop: read line, apply rules, write line.
    std::string line;
    while (getline (std::cin, line)) // Strips \n
    {
      applyRules (rules, sections, line);
      if (line.length ())
      {
        if (prepend_date || prepend_time)
        {
          time_t current;
          time (&current);
          struct tm* t = localtime (&current);

          if (prepend_date)
          {
            std::cout << t->tm_year + 1900 << '-'
                      << std::setw (2) << std::setfill ('0') << t->tm_mon + 1 << '-'
                      << std::setw (2) << std::setfill ('0') << t->tm_mday    << ' ';
          }

          if (prepend_time)
          {
            std::cout << std::setw (2) << std::setfill ('0') << t->tm_hour << ':'
                      << std::setw (2) << std::setfill ('0') << t->tm_min  << ':'
                      << std::setw (2) << std::setfill ('0') << t->tm_sec  << ' ';
          }
        }

        std::cout << line << std::endl;
      }
    }
  }

  catch (std::string& error)
  {
    std::cout << error << "\n";
    return -1;
  }

  catch (...)
  {
    std::cout << "Unknown error\n";
    return -2;
  }

  return status;
}

////////////////////////////////////////////////////////////////////////////////
