////////////////////////////////////////////////////////////////////////////////
// clog - colorized log tail
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

#include <text.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <strings.h>
#include <ctype.h>

////////////////////////////////////////////////////////////////////////////////
void split (
  std::vector<std::string>& results,
  const std::string& input,
  const char delimiter)
{
  results.clear ();
  std::string::size_type start = 0;
  std::string::size_type i;
  while ((i = input.find (delimiter, start)) != std::string::npos)
  {
    results.push_back (input.substr (start, i - start));
    start = i + 1;
  }

  if (input.length ())
    results.push_back (input.substr (start));
}

////////////////////////////////////////////////////////////////////////////////
void split (
  std::vector<std::string>& results,
  const std::string& input,
  const std::string& delimiter)
{
  results.clear ();
  std::string::size_type length = delimiter.length ();

  std::string::size_type start = 0;
  std::string::size_type i;
  while ((i = input.find (delimiter, start)) != std::string::npos)
  {
    results.push_back (input.substr (start, i - start));
    start = i + length;
  }

  if (input.length ())
    results.push_back (input.substr (start));
}

////////////////////////////////////////////////////////////////////////////////
void join (
  std::string& result,
  const std::string& separator,
  const std::vector<std::string>& items)
{
  std::stringstream s;
  unsigned int size = items.size ();
  for (unsigned int i = 0; i < size; ++i)
  {
    s << items[i];
    if (i < size - 1)
      s << separator;
  }

  result = s.str ();
}

////////////////////////////////////////////////////////////////////////////////
void join (
  std::string& result,
  const std::string& separator,
  const std::vector<int>& items)
{
  std::stringstream s;
  unsigned int size = items.size ();
  for (unsigned int i = 0; i < size; ++i)
  {
    s << items[i];
    if (i < size - 1)
      s << separator;
  }

  result = s.str ();
}

////////////////////////////////////////////////////////////////////////////////
std::string trimLeft (const std::string& in, const std::string& t /*= " "*/)
{
  std::string out = in;
  return out.erase (0, in.find_first_not_of (t));
}

////////////////////////////////////////////////////////////////////////////////
std::string trimRight (const std::string& in, const std::string& t /*= " "*/)
{
  std::string out = in;
  return out.erase (out.find_last_not_of (t) + 1);
}

////////////////////////////////////////////////////////////////////////////////
std::string trim (const std::string& in, const std::string& t /*= " "*/)
{
  std::string out = in;
  return trimLeft (trimRight (out, t), t);
}

////////////////////////////////////////////////////////////////////////////////
std::string lowerCase (const std::string& input)
{
  std::string output = input;
  std::transform (output.begin (), output.end (), output.begin (), tolower);
  return output;
}

////////////////////////////////////////////////////////////////////////////////
std::string upperCase (const std::string& input)
{
  std::string output = input;
  std::transform (output.begin (), output.end (), output.begin (), toupper);
  return output;
}

////////////////////////////////////////////////////////////////////////////////
const std::string format (char value)
{
  std::stringstream s;
  s << value;
  return s.str ();
}

////////////////////////////////////////////////////////////////////////////////
const std::string format (int value)
{
  std::stringstream s;
  s << value;
  return s.str ();
}

////////////////////////////////////////////////////////////////////////////////
const std::string formatHex (int value)
{
  std::stringstream s;
  s.setf (std::ios::hex, std::ios::basefield);
  s << value;
  return s.str ();
}

////////////////////////////////////////////////////////////////////////////////
const std::string format (float value, int width, int precision)
{
  std::stringstream s;
  s.width (width);
  s.precision (precision);
  s << value;
  return s.str ();
}

////////////////////////////////////////////////////////////////////////////////
const std::string format (double value, int width, int precision)
{
  std::stringstream s;
  s.width (width);
  s.precision (precision);
  s << value;
  return s.str ();
}

////////////////////////////////////////////////////////////////////////////////
const std::string format (double value)
{
  std::stringstream s;
  s << std::fixed << value;
  return s.str ();
}

////////////////////////////////////////////////////////////////////////////////
static void replace_positional (
  std::string& fmt,
  const std::string& from,
  const std::string& to)
{
  std::string::size_type pos = 0;
  while ((pos = fmt.find (from, pos)) != std::string::npos)
  {
    fmt.replace (pos, from.length (), to);
    pos += to.length ();
  }
}

////////////////////////////////////////////////////////////////////////////////
const std::string format (
  const std::string& fmt,
  const std::string& arg1)
{
  std::string output = fmt;
  replace_positional (output, "{1}", arg1);
  return output;
}

////////////////////////////////////////////////////////////////////////////////
const std::string format (
  const std::string& fmt,
  int arg1)
{
  std::string output = fmt;
  replace_positional (output, "{1}", format (arg1));
  return output;
}

////////////////////////////////////////////////////////////////////////////////
const std::string format (
  const std::string& fmt,
  const std::string& arg1,
  const std::string& arg2)
{
  std::string output = fmt;
  replace_positional (output, "{1}", arg1);
  replace_positional (output, "{2}", arg2);
  return output;
}

////////////////////////////////////////////////////////////////////////////////
const std::string format (
  const std::string& fmt,
  const std::string& arg1,
  int arg2)
{
  std::string output = fmt;
  replace_positional (output, "{1}", arg1);
  replace_positional (output, "{2}", format (arg2));
  return output;
}

////////////////////////////////////////////////////////////////////////////////
const std::string format (
  const std::string& fmt,
  const std::string& arg1,
  double arg2)
{
  std::string output = fmt;
  replace_positional (output, "{1}", arg1);
  replace_positional (output, "{2}", trim (format (arg2, 6, 3)));
  return output;
}

////////////////////////////////////////////////////////////////////////////////
const std::string format (
  const std::string& fmt,
  int arg1,
  const std::string& arg2)
{
  std::string output = fmt;
  replace_positional (output, "{1}", format (arg1));
  replace_positional (output, "{2}", arg2);
  return output;
}

////////////////////////////////////////////////////////////////////////////////
const std::string format (
  const std::string& fmt,
  int arg1,
  int arg2)
{
  std::string output = fmt;
  replace_positional (output, "{1}", format (arg1));
  replace_positional (output, "{2}", format (arg2));
  return output;
}

////////////////////////////////////////////////////////////////////////////////
const std::string format (
  const std::string& fmt,
  int arg1,
  double arg2)
{
  std::string output = fmt;
  replace_positional (output, "{1}", format (arg1));
  replace_positional (output, "{2}", trim (format (arg2, 6, 3)));
  return output;
}

////////////////////////////////////////////////////////////////////////////////
const std::string format (
  const std::string& fmt,
  const std::string& arg1,
  const std::string& arg2,
  const std::string& arg3)
{
  std::string output = fmt;
  replace_positional (output, "{1}", arg1);
  replace_positional (output, "{2}", arg2);
  replace_positional (output, "{3}", arg3);
  return output;
}

////////////////////////////////////////////////////////////////////////////////
