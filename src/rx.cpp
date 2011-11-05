////////////////////////////////////////////////////////////////////////////////
// clog - a colorizing log filter
//
// Copyright 2010-2011, Göteborg Bit Factory.
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

#include <stdlib.h>
#include <regex.h>
#include <rx.h>

//#define _POSIX_C_SOURCE 1
#define MAX_MATCHES 8

////////////////////////////////////////////////////////////////////////////////
bool regexMatch (
  const std::string& in,
  const std::string& pattern,
  bool caseSensitive /* = true */)
{
  regex_t r = {0};
  int result;
  if ((result = regcomp (&r, pattern.c_str (),
                         REG_EXTENDED | REG_NOSUB | REG_NEWLINE |
                         (caseSensitive ? 0 : REG_ICASE))) == 0)
  {
    if ((result = regexec (&r, in.c_str (), 0, NULL, 0)) == 0)
    {
      regfree (&r);
      return true;
    }

    if (result == REG_NOMATCH)
      return false;
  }

  char message[256];
  regerror (result, &r, message, 256);
  throw std::string (message);

  return false;
}

////////////////////////////////////////////////////////////////////////////////
bool regexMatch (
  std::vector<std::string>& out,
  const std::string& in,
  const std::string& pattern,
  bool caseSensitive /* = true */)
{
  regex_t r = {0};
  int result;
  if ((result = regcomp (&r, pattern.c_str (),
                         REG_EXTENDED | REG_NEWLINE |
                         (caseSensitive ? 0 : REG_ICASE))) == 0)
  {
    regmatch_t rm[MAX_MATCHES];
    if ((result = regexec (&r, in.c_str (), MAX_MATCHES, rm, 0)) == 0)
    {
      for (unsigned int i = 1; i < 1 + r.re_nsub; ++i)
        out.push_back (in.substr (rm[i].rm_so, rm[i].rm_eo - rm[i].rm_so));

      regfree (&r);
      return true;
    }

    if (result == REG_NOMATCH)
      return false;
  }

  char message[256];
  regerror (result, &r, message, 256);
  throw std::string (message);

  return false;
}

////////////////////////////////////////////////////////////////////////////////
bool regexMatch (
  std::vector <int>& start,
  std::vector <int>& end,
  const std::string& in,
  const std::string& pattern,
  bool caseSensitive /* = true */)
{
  regex_t r = {0};
  int result;
  if ((result = regcomp (&r, pattern.c_str (),
                         REG_EXTENDED | REG_NEWLINE |
                         (caseSensitive ? 0 : REG_ICASE))) == 0)
  {
    regmatch_t rm[MAX_MATCHES];
    if ((result = regexec (&r, in.c_str (), MAX_MATCHES, rm, 0)) == 0)
    {
      for (unsigned int i = 1; i < 1 + r.re_nsub; ++i)
      {
        start.push_back (rm[i].rm_so);
        end.push_back   (rm[i].rm_eo);
      }

      regfree (&r);
      return true;
    }

    if (result == REG_NOMATCH)
      return false;
  }

  char message[256];
  regerror (result, &r, message, 256);
  throw std::string (message);

  return false;
}

////////////////////////////////////////////////////////////////////////////////
