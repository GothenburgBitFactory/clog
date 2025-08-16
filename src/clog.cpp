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
// If <iostream> is included, put it after <stdio.h>, because it includes
// <stdio.h>, and therefore would ignore the _WITH_GETLINE.
#ifdef FREEBSD
#define _WITH_GETLINE
#endif
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <ctime>
#include <shared.h>

extern bool loadRules (const std::string&, std::vector <Rule>&);

////////////////////////////////////////////////////////////////////////////////
// Applies all the rules in all the sections specified.
// Note that processing does not stop after the first rule match, it keeps going.
void applyRules (
  Composite& composite,
  bool& blanks,
  std::vector <Rule>& rules,
  const std::vector <std::string>& sections,
  const std::string& line)
{
  composite.add (line, 0, {0});

  for (const auto& section : sections)
    for (auto& rule : rules)
      rule.apply (composite, blanks, section, line);
}

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char** argv)
{
  int status = 0;

  try
  {
    // Locate $HOME.
    struct passwd* pw = getpwuid (getuid ());
    if (!pw)
      throw std::string ("Could not determine the home directory.");

    // Assume ~/.clogrc
    std::string rcFile = pw->pw_dir;
    rcFile += "/.clogrc";

    // Process arguments.
    std::vector <std::string> sections;
    bool prepend_date = false;
    bool prepend_time = false;

    for (int i = 1; i < argc; ++i)
    {
      if (! strcmp (argv[i], "-h") ||
          ! strcmp (argv[i], "--help"))
      {
        std::cout << '\n'
                  << "Usage: clog [<options>] [<section> ...]\n"
                  << '\n'
                  << "  -h|--help       Show this usage\n"
                  << "  -v|--version    Show this version\n"
                  << "  -d|--date       Prepend all lines with the current date\n"
                  << "  -t|--time       Prepend all lines with the current time\n"
                  << "  -f|--file       Override default ~/.clogrc\n"
                  << '\n';
        return status;
      }

      else if (! strcmp (argv[i], "-v") ||
               ! strcmp (argv[i], "--version"))
      {
        std::cout << "\n"
                  << PACKAGE_STRING
                  << " built for "
                  << osName ()
                  << "\n"
                  << "Copyright (C) 2010 - 2017 Gothenburg Bit Factory\n"
                  << "\n"
                  << "Clog may be copied only under the terms of the MIT "
                     "license, which may be found in the source kit.\n"
                  << "\n"
                  << "Documentation for clog can be found using 'man clog' "
                     "or at https://gothenburgbitfactory.org/clog/docs/\n"
                  << "\n";
        return status;
      }

      else if (! strcmp (argv[i], "-d") ||
               ! strcmp (argv[i], "--date"))
      {
        prepend_date = true;
      }

      else if (! strcmp (argv[i], "-t") ||
               ! strcmp (argv[i], "--time"))
      {
        prepend_time = true;
      }

      else if (argc > i + 1 &&
               (! strcmp (argv[i], "-f") ||
                ! strcmp (argv[i], "--file")))
      {
        rcFile = argv[++i];
      }

      else
      {
        sections.push_back (argv[i]);
      }
    }

    // Use a default section if one was not specified.
    if (sections.size () == 0)
      sections.push_back ("default");

    // Read rc file.
    std::vector <Rule> rules;
    if (loadRules (rcFile, rules))
    {
      Composite composite;

      // Main loop: read line, apply rules, write line.
      std::string line;
      while (getline (std::cin, line)) // Strips \n
      {
        auto length = line.length ();
        bool blanks = false;
        applyRules (composite, blanks, rules, sections, line);

        if (blanks)
          std::cout << "\n";

        auto output = composite.str ();
        if (output.length () || output.length () == length)
        {
          if (prepend_date || prepend_time)
          {
            time_t current;
            time (&current);
            struct tm* t = localtime (&current);

            if (prepend_date)
              std::cout << t->tm_year + 1900 << '-'
                        << std::setw (2) << std::setfill ('0') << t->tm_mon + 1 << '-'
                        << std::setw (2) << std::setfill ('0') << t->tm_mday    << ' ';

            if (prepend_time)
              std::cout << std::setw (2) << std::setfill ('0') << t->tm_hour << ':'
                        << std::setw (2) << std::setfill ('0') << t->tm_min  << ':'
                        << std::setw (2) << std::setfill ('0') << t->tm_sec  << ' ';
          }

          std::cout << output << "\n";
        }

        if (blanks)
          std::cout << "\n";

        composite.clear ();
      }
    }
    else
    {
      std::cout << "Cannot open " << rcFile << "\n"
                << "See 'man clog' for details and a sample file.\n";
      status = -1;
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
