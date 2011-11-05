////////////////////////////////////////////////////////////////////////////////
// clog - a colorizing log filter
//
// Copyright 2006-2011, Göteborg Bit Factory.
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
#ifndef INCLUDED_TEXT
#define INCLUDED_TEXT

#include <string>
#include <vector>

// text.cpp, Non-UTF-8 aware.
std::string trimLeft (const std::string& in, const std::string& t = " ");
std::string trimRight (const std::string& in, const std::string& t = " ");
std::string trim (const std::string& in, const std::string& t = " ");
void split (std::vector<std::string>&, const std::string&, const char);
void split (std::vector<std::string>&, const std::string&, const std::string&);
void join (std::string&, const std::string&, const std::vector<std::string>&);
void join (std::string&, const std::string&, const std::vector<int>&);
std::string lowerCase (const std::string&);
std::string upperCase (const std::string&);
std::string format (char);
std::string format (int);
std::string formatHex (int);
std::string format (float, int, int);
std::string format (double, int, int);

#endif
////////////////////////////////////////////////////////////////////////////////
