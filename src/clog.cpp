////////////////////////////////////////////////////////////////////////////////
// clog - Colorized Log Filter
//
// Copyright 2011, Paul Beckingham, Federico Hernandez.
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
#include <vector>
#include <string>
#include <string.h>

void loadConfig ();
void applyRules (const std::vector <std::string>&, std::string&);

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char** argv)
{
  int status = 0;

  try
  {
    std::vector <std::string> sections;

    for (int i = 1; i < argc; ++i)
    {
      if (!strcmp (argv[i], "-h") ||
          !strcmp (argv[i], "--help"))
      {
        std::cout << "\n"
                  << "Usage: clog [--help] [--version] [ <section> ... ]\n"
                  << "\n";
        return status;
      }

      else if (!strcmp (argv[i], "-v") ||
               !strcmp (argv[i], "--version"))
      {
        std::cout << "\n"
                  << "clog 1.0.0 build for xxxx\n"
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

      else
        sections.push_back (argv[i]);
    }

    // Use a default section if one was not specified.
    if (sections.size () == 0)
      sections.push_back ("default");

    // Read ~/.clogrc
    loadConfig ();

    // TODO read/apply/write
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
// <section> rule /<pattern>/ --> <color> <context>
void loadConfig ()
{
  // TODO Read file line by line, removing comments.
  // TODO Combine into a single string.
  // TODO Nibble the string.
}

////////////////////////////////////////////////////////////////////////////////
void applyRules (const std::vector <std::string>& sections, std::string& line)
{
  std::vector <std::string>::const_iterator i;
  for (i = sections.begin (); i != sections.end (); ++i)
  {
    // TODO Foreach rule in section
      // TODO if (rule matches)
        // TODO colorize line
  }
}

////////////////////////////////////////////////////////////////////////////////
