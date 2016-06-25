////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2010 - 2016, Paul Beckingham, Federico Hernandez.
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
#include <Pig.h>
#include <RX.h>
#include <shared.h>

////////////////////////////////////////////////////////////////////////////////
// <section> rule /<pattern>/  --> <color> <context>
// taskd     rule /code:"2.."/ --> green   line
Rule::Rule (const std::string& line)
{
  _fragment = "";

  Pig pig (line);
  pig.skipWS ();

  std::string pattern;
  if (pig.getUntilWS (_section) &&
      pig.skipWS ()             &&
      pig.skipLiteral ("rule")  &&
      pig.skipWS ())
  {
    // <section> rule /<pattern>/
    if (pig.getQuoted ('/', pattern) &&
        pig.skipWS ()                &&
        pig.skipLiteral ("-->"))
    {
      pig.skipWS ();

      std::string rest;
      pig.getRemainder (rest);

      std::string color_name;
      std::vector <std::string> words = split (rest, ' ');
      for (auto& word : words)
      {
        if (word.length ())
        {
               if (word == "line")     _context = word;
          else if (word == "match")    _context = word;
          else if (word == "suppress") _context = word;
          else if (word == "blank")    _context = word;
          else
          {
            if (color_name.length ())
              color_name += " ";

            color_name += word;
          }
        }
      }

      _color = Color (color_name);

      // Now for "match" context patterns, add an enclosing ( ... ) if not
      // already present.
      if (_context == "match")
        if (pattern.find ('(') == std::string::npos)
          pattern = "(" + pattern + ")";

      _rx = RX (pattern, true);
      return;
    }

    // <section> rule "<pattern>"
    else if (pig.getQuoted ('"', pattern) &&
             pig.skipWS ()                &&
             pig.skipLiteral ("-->"))
    {
      pig.skipWS ();

      std::string rest;
      pig.getRemainder (rest);

      std::string color_name;
      std::vector <std::string> words = split (rest, ' ');
      for (auto& word : words)
      {
        if (word.length ())
        {
               if (word == "line")     _context = word;
          else if (word == "match")    _context = word;
          else if (word == "suppress") _context = word;
          else if (word == "blank")    _context = word;
          // TODO Support _context "datetime", "time"
          else
          {
            if (color_name.length ())
              color_name += " ";

            color_name += word;
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
//   - suppress  Eats the whole line, including \n
//   - line      Colorizes the line
//   - match     Colorizes the matching part
//   - blank     Adds a blank line before and after
//
bool Rule::apply (Composite& composite, bool& blanks, const std::string& section, const std::string& line)
{
  if (_section == section)
  {
    if (_context == "suppress")
    {
      if (_fragment != "")
      {
        if (line.find (_fragment) != std::string::npos)
        {
          composite.clear ();
          return true;
        }
      }
      else
      {
        if (_rx.match (line))
        {
          composite.clear ();
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
          composite.add (line, 0, _color);
          return true;
        }
      }
      else
      {
        if (_rx.match (line))
        {
          composite.add (line, 0, _color);
          return true;
        }
      }
    }

    else if (_context == "match")
    {
      if (_fragment != "")
      {
        bool found = false;
        std::string::size_type pos = line.find (_fragment);
        while (pos != std::string::npos)
        {
          composite.add (line.substr (pos, _fragment.length ()), pos, _color);
          pos = line.find (_fragment, pos + 1);
          found = true;
        }

        if (found)
          return true;
      }
      else
      {
        std::vector <int> start;
        std::vector <int> end;
        if (_rx.match (start, end, line))
        {
          for (unsigned int i = 0; i < start.size (); ++i)
            composite.add (line.substr (start[i], end[i] - start[i]), start[i], _color);

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
          blanks = true;
          return true;
        }
      }
      else
      {
        if (_rx.match (line))
        {
          blanks = true;
          return true;
        }
      }
    }
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////
