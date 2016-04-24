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
  fragment = "";

  Pig pig (line);
  pig.skipWS ();

  std::string pattern;
  if (pig.getUntilWS (section)     &&
      pig.skipWS ()                &&
      pig.skipLiteral ("rule")     &&
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
               if (word == "line")      context = word;
          else if (word == "match")     context = word;
          else if (word == "suppress")  context = word;
          else if (word == "blank")     context = word;
          else
          {
            if (color_name.length ())
              color_name += " ";

            color_name += word;
          }
        }
      }

      color = Color (color_name);

      // Now for "match" context patterns, add an enclosing ( ... ) if not
      // already present.
      if (context == "match")
        if (pattern.find ('(') == std::string::npos)
          pattern = "(" + pattern + ")";

      rx = RX (pattern, true);
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
               if (word == "line")      context = word;
          else if (word == "match")     context = word;
          else if (word == "suppress")  context = word;
          else if (word == "blank")     context = word;
          // TODO Support context "datetime", "time"
          else
          {
            if (color_name.length ())
              color_name += " ";

            color_name += word;
          }
        }
      }

      color = Color (color_name);
      fragment = pattern;
      return;
    }
  }

  // Indicates that 'line' was not a rule def, but a blank line or similar.
  throw int (1);
}

////////////////////////////////////////////////////////////////////////////////
// There are two kinds of matching:
//   - regex     (when fragment is     "")
//   - substring (when fragment is not "")
//
// There are several corresponding actions:
//   - suppress  Eats the whole line, including \n
//   - line      Colorizes the line
//   - match     Colorizes the matching part
//   - blank     Adds a blank line before and after
//
bool Rule::apply (const std::string& section, std::string& line)
{
  if (section == section)
  {
    if (context == "suppress")
    {
      if (fragment != "")
      {
        if (line.find (fragment) != std::string::npos)
        {
          line = "";
          return true;
        }
      }
      else
      {
        if (rx.match (line))
        {
          line = "";
          return true;
        }
      }
    }

    else if (context == "line")
    {
      if (fragment != "")
      {
        if (line.find (fragment) != std::string::npos)
        {
          line = color.colorize (line);
          return true;
        }
      }
      else
      {
        if (rx.match (line))
        {
          line = color.colorize (line);
          return true;
        }
      }
    }

    else if (context == "match")
    {
      if (fragment != "")
      {
        std::string::size_type pos = line.find (fragment);
        if (pos != std::string::npos)
        {
          line = line.substr (0, pos)
               + color.colorize (line.substr (pos, fragment.length ()))
               + line.substr (pos + fragment.length ());
          return true;
        }
      }
      else
      {
        std::vector <int> start;
        std::vector <int> end;
        if (rx.match (start, end, line))
        {
          line = line.substr (0, start[0])
               + color.colorize (line.substr (start[0], end[0] - start[0]))
               + line.substr (end[0]);
          return true;
        }
      }
    }

    else if (context == "blank")
    {
      if (fragment != "")
      {
        if (line.find (fragment) != std::string::npos)
        {
          line = "\n" + line + "\n";
          return true;
        }
      }
      else
      {
        if (rx.match (line))
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
