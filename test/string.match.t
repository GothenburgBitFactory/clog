#! /usr/bin/perl
################################################################################
## clog - colorized log tail
##
## Copyright 2010-2013, Paul Beckingham, Federico Hernandez.
##
## Permission is hereby granted, free of charge, to any person obtaining a copy
## of this software and associated documentation files (the "Software"), to deal
## in the Software without restriction, including without limitation the rights
## to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
## copies of the Software, and to permit persons to whom the Software is
## furnished to do so, subject to the following conditions:
##
## The above copyright notice and this permission notice shall be included
## in all copies or substantial portions of the Software.
##
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
## OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
## THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
## LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
## OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
## SOFTWARE.
##
## http://www.opensource.org/licenses/mit-license.php
##
################################################################################

use strict;
use warnings;
use Test::More tests => 4;

# Create the rc file.
if (open my $fh, '>', 'rc')
{
  print $fh "default rule \"foo\" --> red match\n";
  close $fh;
  ok (-r 'rc', 'Created rc');
}

# Test that string pattern matches work.
my $output = qx{printf "a foo\na bar\n" | ../src/clog -f rc};
like ($output, qr/^a \033\[31mfoo\033\[0m$/ms, 'foo is red');
like ($output, qr/^a bar$/ms,                  'bar is unchanged');

# Cleanup.
unlink qw(rc);
ok (! -r 'rc', 'Cleanup');

exit 0;

