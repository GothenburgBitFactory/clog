////////////////////////////////////////////////////////////////////////////////
// clog - a colorizing log filter
//
// Copyright 2006 - 2011, Göteborg Bit Factory.
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
#include <iostream> // TODO Remove

#include <Rule.h>
#include <Nibbler.h>
#include <rx.h>
#include <text.h>

////////////////////////////////////////////////////////////////////////////////
// <section> rule /<pattern>/  --> <color> <context>
// taskd     rule /code:"2.."/ --> green   line
Rule::Rule (const std::string& value)
{
  Nibbler n (value);
  n.skipWS ();

  if (n.getUntilWS (section)     &&
      n.skipWS ()                &&
      n.getLiteral ("rule")      &&
      n.skipWS ()                &&
      n.skip ('/')               &&
      n.getRx ("[^/]+", pattern) &&
      n.skip ('/')               &&
      n.skipWS ()                &&
      n.getLiteral ("-->"))
  {
    n.skipWS ();

    std::string rest;
    n.getRemainder (rest);

    std::string color_name;
    std::vector <std::string> words;
    split (words, rest, ' ');
    std::vector <std::string>::iterator i;
    for (i = words.begin (); i != words.end (); ++i)
    {
      if (i->length ())
      {
             if (*i == "line")      context = *i;
        else if (*i == "match")     context = *i;
        else if (*i == "suppress")  context = *i;
        // TODO Support context "datetime", "time"
        else
        {
          if (color_name.length ())
            color_name += " ";

          color_name += *i;
        }
      }
    }

    color = Color (color_name);

/*
    std::cout << "# section=" << section
              << " pattern=/" << pattern    << "/"
              << " color='"   << color_name << "'"
              << " context="  << context
              << "\n";
*/

    // Now for "match" context patterns, add an enclosing ( ... ) is not already
    // present.
    if (context == "match")
      if (pattern.find ('(') == std::string::npos)
        pattern = "(" + pattern + ")";

    return;
  }

  // Indicates that 'value' was not a rule def, but a blank line or similar.
  throw int (1);
}

////////////////////////////////////////////////////////////////////////////////
Rule::Rule (const Rule& other)
{
  if (this != &other)
  {
    section = other.section;
    pattern = other.pattern;
    color   = other.color;
    context = other.context;
  }
}

////////////////////////////////////////////////////////////////////////////////
Rule::~Rule ()
{
}

////////////////////////////////////////////////////////////////////////////////
Rule& Rule::operator= (const Rule& other)
{
  if (this != &other)
  {
    section = other.section;
    pattern = other.pattern;
    color   = other.color;
    context = other.context;
  }

  return *this;
}

////////////////////////////////////////////////////////////////////////////////
void Rule::apply (const std::string& section, std::string& line)
{
  // TODO Support caseless regexes.
  bool case_sensitive = false;

  if (section == this->section)
  {
    if (context == "suppress")
    {
      if (regexMatch (line, pattern, case_sensitive))
        line = "";
    }

    else if (context == "line")
    {
      if (regexMatch (line, pattern, case_sensitive))
        line = color.colorize (line);
    }

    else if (context == "match")
    {
      std::vector <int> start;
      std::vector <int> end;
      if (regexMatch (start, end, line, pattern, case_sensitive))
        line = line.substr (0, start[0])
             + color.colorize (line.substr (start[0], end[0] - start[0]))
             + line.substr (end[0]);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
