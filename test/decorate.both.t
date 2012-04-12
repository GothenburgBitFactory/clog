#! /usr/bin/perl
################################################################################
## clog - colorized log tail
##
## Copyright 2010-2012, Paul Beckingham, Federico Hernandez.
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
  print $fh "# empty\n";
  close $fh;
  ok (-r 'rc', 'Created rc');
}

# Test that string pattern line matches work.
my $output = qx{printf "foo\nbar\n" | ../src/clog -f rc --date --time};
like ($output, qr/^\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2} foo$/ms, 'foo decorated');
like ($output, qr/^\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2} bar$/ms, 'bar decorated');

# Cleanup.
unlink qw(rc);
ok (! -r 'rc', 'Cleanup');

exit 0;

