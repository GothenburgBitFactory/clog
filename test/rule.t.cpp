////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2015 - 2017, Paul Beckingham, Federico Hernandez.
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
#include <test.h>

////////////////////////////////////////////////////////////////////////////////
void testRule (
  UnitTest& t,
  const std::string& line,
  const std::string& section,
  const Color& color,
  const std::string& context,
  const std::string& fragment)
{
  try
  {
    Rule r (line);
    t.ok (r._section  == section,  "<" + line + "> section match");
    t.ok (r._color    == color,    "<" + line + "> color match");
    t.ok (r._context  == context,  "<" + line + "> context match");
    t.ok (r._fragment == fragment, "<" + line + "> fragment match");
  }
  catch (...)
  {
    t.fail ("<" + line + "> section match");
    t.fail ("<" + line + "> color match");
    t.fail ("<" + line + "> context match");
    t.fail ("<" + line + "> fragment match");
  }
}

////////////////////////////////////////////////////////////////////////////////
int main (int, char**)
{
  UnitTest t (40);

  testRule (t, "default rule /bar/ --> suppress",     "default", {},       "suppress", "");
  testRule (t, "default rule /foo/ --> red line",     "default", {"red"},  "line",     "");
  testRule (t, "default rule /foo/ --> red match",    "default", {"red"},  "match",    "");
  testRule (t, "default rule \"abc\" --> red match",  "default", {"red"},  "match",    "abc");
  testRule (t, "default rule \"bar\" --> blank",      "default", {},       "blank",    "bar");
  testRule (t, "default rule \"cd\"  --> blue match", "default", {"blue"}, "match",    "cd");
  testRule (t, "default rule \"foo\" --> blank",      "default", {},       "blank",    "foo");
  testRule (t, "default rule \"foo\" --> red line",   "default", {"red"},  "line",     "foo");
  testRule (t, "default rule \"foo\" --> red match",  "default", {"red"},  "match",    "foo");
  testRule (t, "default rule \"foo\" --> suppress",   "default", {},       "suppress", "foo");

  return 0;
}

////////////////////////////////////////////////////////////////////////////////

