////////////////////////////////////////////////////////////////////////////////
// clog - a colorizing log filter
//
// Copyright 2006-2012, Göteborg Bit Factory.
// All rights reserved.
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
