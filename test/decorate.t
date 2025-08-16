#!/usr/bin/env python3

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


class TestRegexLine(TestCase):
    def setUp(self):
        """Executed before each test in the class"""
        self.t = Clog()

    def test_decorate_date(self):
        """Test decorate a line with the date"""
        self.t.config('')

        code, out, err = self.t("--date", input='foo\nbar\n'.encode())
        self.assertRegex(out, r'^\d{4}-\d{2}-\d{2} foo\n')
        self.assertRegex(out, r'\n\d{4}-\d{2}-\d{2} bar$')

    def test_decorate_time(self):
        """Test decorate a line with time"""
        self.t.config('')

        code, out, err = self.t("--time", input='foo\nbar\n'.encode())
        self.assertRegex(out, r'^\d{2}:\d{2}:\d{2} foo\n')
        self.assertRegex(out, r'\n\d{2}:\d{2}:\d{2} bar$')

    def test_decorate_date_and_time(self):
        """Test decorate a line with date and time"""
        self.t.config('')

        code, out, err = self.t("--date --time", input='foo\nbar\n'.encode())
        self.assertRegex(out, r'^\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2} foo\n')
        self.assertRegex(out, r'\n\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2} bar$')


if __name__ == "__main__":
    from simpletap import TAPTestRunner
    unittest.main(testRunner=TAPTestRunner())
