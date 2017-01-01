#!/usr/bin/env python2.7
# -*- coding: utf-8 -*-
###############################################################################
#
# Copyright 2006 - 2017, Paul Beckingham, Federico Hernandez.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# http://www.opensource.org/licenses/mit-license.php
#
###############################################################################

import sys
import os
import unittest
from datetime import datetime
# Ensure python finds the local simpletap module
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

from basetest import Clog, TestCase

# Test methods available:
#     self.assertEqual(a, b)
#     self.assertNotEqual(a, b)
#     self.assertTrue(x)
#     self.assertFalse(x)
#     self.assertIs(a, b)
#     self.assertIsNot(substring, text)
#     self.assertIsNone(x)
#     self.assertIsNotNone(x)
#     self.assertIn(substring, text)
#     self.assertNotIn(substring, text
#     self.assertRaises(e)
#     self.assertRegexpMatches(text, pattern)
#     self.assertNotRegexpMatches(text, pattern)
#     self.tap("")

class TestPatternLine(TestCase):
    def setUp(self):
        """Executed before each test in the class"""
        self.t = Clog()

    def test_pattern_line(self):
        """Test matching a pattern and coloring a line"""
        self.t.config('default rule "foo" --> red line')

        code, out, err = self.t("", input='a foo\na bar\na baz\n')
        self.assertIn('\x1b[31ma foo\x1b[0m\n', out)
        self.assertRegexpMatches(out, r'\na bar\n')
        self.assertRegexpMatches(out, r'\na baz\n')

class TestPatternMatch(TestCase):
    def setUp(self):
        """Executed before each test in the class"""
        self.t = Clog()

    def test_pattern_match(self):
        """Test matching a pattern and coloring a match"""
        self.t.config('default rule "foo" --> red match')

        code, out, err = self.t("", input='a foo\na bar\na baz\n')
        self.assertIn('a \x1b[31mfoo\x1b[0m\n', out)
        self.assertRegexpMatches(out, r'\na bar\n')
        self.assertRegexpMatches(out, r'\na baz\n')

class TestPatternOverlap(TestCase):
    def setUp(self):
        """Executed before each test in the class"""
        self.t = Clog()

    def test_pattern_overlap_1(self):
        """Test matching a pattern and coloring a match, red before blue"""
        self.t.config('default rule "abc" --> red match')
        self.t.config('default rule "cd" --> blue match')

        code, out, err = self.t("", input='abcdabcdabcd\n')
        self.tap(out)
        self.assertIn('\x1b[31mab\x1b[0m\x1b[34mcd\x1b[0m\x1b[31mab\x1b[0m\x1b[34mcd\x1b[0m\n', out)

    def test_pattern_overlap_2(self):
        """Test matching a pattern and coloring a match, blue before red"""
        self.t.config('default rule "cd" --> blue match')
        self.t.config('default rule "abc" --> red match')

        code, out, err = self.t("", input='abcdabcdabcd\n')
        self.tap(out)
        self.assertIn('\x1b[31mabc\x1b[0m\x1b[34md\x1b[0m\x1b[31mabc\x1b[0m\x1b[34md\x1b[0m\n', out)

if __name__ == "__main__":
    from simpletap import TAPTestRunner
    unittest.main(testRunner=TAPTestRunner())



