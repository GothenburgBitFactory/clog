////////////////////////////////////////////////////////////////////////////////
// clog - colorized log tail
//
// Copyright 2010 - 2015, Paul Beckingham, Federico Hernandez.
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
#include <Nibbler.h>
#include <RX.h>
#include <text.h>

////////////////////////////////////////////////////////////////////////////////
// <section> rule /<pattern>/  --> <color> <context>
// taskd     rule /code:"2.."/ --> green   line
Rule::Rule (const std::string& line)
{
  _fragment = "";

  Nibbler n (line);
  n.skipWS ();

  std::string pattern;
  if (n.getUntilWS (_section)     &&
      n.skipWS ()                &&
      n.getLiteral ("rule")      &&
      n.skipWS ())
  {
    // <section> rule /<pattern>/
    if (n.getQuoted ('/', pattern) &&
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
               if (*i == "line")      _context = *i;
          else if (*i == "match")     _context = *i;
          else if (*i == "suppress")  _context = *i;
          else if (*i == "blank")     _context = *i;
          else
          {
            if (color_name.length ())
              color_name += " ";

            color_name += *i;
          }
        }
      }

      _color = Color (color_name);

      // Now for "match" context patterns, add an enclosing ( ... ) is not
      // already present.
      if (_context == "match")
        if (pattern.find ('(') == std::string::npos)
          pattern = "(" + pattern + ")";

      _rx = RX (pattern, true);
      return;
    }

    // <section> rule "<pattern>"
    else if (n.getQuoted ('"', pattern) &&
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
               if (*i == "line")      _context = *i;
          else if (*i == "match")     _context = *i;
          else if (*i == "suppress")  _context = *i;
          else if (*i == "blank")     _context = *i;
          // TODO Support context "datetime", "time"
          else
          {
            if (color_name.length ())
              color_name += " ";

            color_name += *i;
          }
        }
      }

      _color = Color (color_name);
      _fragment = pattern;
      return;
    }
  }

  // Indicates that 'line' was not a rule def, but a blank line or similar.
  throw int (1);
}

////////////////////////////////////////////////////////////////////////////////
// There are two kinds of matching:
//   - regex     (when _fragment is     "")
//   - substring (when _fragment is not "")
//
// There are several corresponding actions:
//   - suppress  Eats the line
//   - line      Colorizes the line
//   - match     Colorizes the matching part
//   - blank     Adds a blank line before and after
//
bool Rule::apply (const std::string& section, std::string& line)
{
  if (section == _section)
  {
    if (_context == "suppress")
    {
      if (_fragment != "")
      {
        if (line.find (_fragment) != std::string::npos)
        {
          line = "";
          return true;
        }
      }
      else
      {
        if (_rx.match (line))
        {
          line = "";
          return true;
        }
      }
    }

    else if (_context == "line")
    {
      if (_fragment != "")
      {
        if (line.find (_fragment) != std::string::npos)
        {
          line = _color.colorize (line);
          return true;
        }
      }
      else
      {
        if (_rx.match (line))
        {
          line = _color.colorize (line);
          return true;
        }
      }
    }

    else if (_context == "match")
    {
      if (_fragment != "")
      {
        std::string::size_type pos = line.find (_fragment);
        if (pos != std::string::npos)
        {
          line = line.substr (0, pos)
               + _color.colorize (line.substr (pos, _fragment.length ()))
               + line.substr (pos + _fragment.length ());
          return true;
        }
      }
      else
      {
        std::vector <int> start;
        std::vector <int> end;
        if (_rx.match (start, end, line))
        {
          line = line.substr (0, start[0])
               + _color.colorize (line.substr (start[0], end[0] - start[0]))
               + line.substr (end[0]);
          return true;
        }
      }
    }

    else if (_context == "blank")
    {
      if (_fragment != "")
      {
        if (line.find (_fragment) != std::string::npos)
        {
          line = "\n" + line + "\n";
          return true;
        }
      }
      else
      {
        if (_rx.match (line))
        {
          line = "\n" + line + "\n";
          return true;
        }
      }
    }
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////
