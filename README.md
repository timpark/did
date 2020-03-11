did - Simple command-line task logging
======================================
----------------------------------------------------------------------------
This is a "don't break the chain" style task logging program.
See [http://dontbreakthechain.com/what](http://dontbreakthechain.com/what)
for more information behind the motivation.

Every day you perform whatever task you wish to log, you simply type:

    did task

Note that while you must type the task name the same each time, only the
first five characters of the task name are printed in the report.  Since
people can be up past midnight, an option is given to easily add a task
done yesterday.

There are defines at the top of the program where you can change the
characters used for the report and the filename used.  You may wish to
give it an absolute filename in a backed-up or synced directory.

A task can be done more than once a day.  It will be marked in the
grid as 2-9, then A-Z (for 10-35), then with a plus sign for
higher counts.

Usage:

    did               : Show report
    did -h            : Show usage
    did -d YYYY/MM/DD : Show report ending at date
    did task          : Log task as done today
    did task yest     : Log task as done yesterday

Example output: ("exe" is short for "exercise")

    > did exe yest
    "2013/01/01 exe" added

    > did exe
    "2013/01/02 exe" added

    > did code
    "2013/01/02 code" added

    > did
    did - Simple command-line task logging ("did -h" for usage)

          ########################################################################
     code #@@@@--@@@@@--@@@@@--@@@@@--@@@@@--@@@@@--@@@@@--@@@@@--@@@@@--@@@@@--@# code (50)
      exe #@@-@@-@@-@@-@@-@@-@@-@@-@@-@@-@@-@@-@@-@@-@@-@@-@@-@@-@@-@@-@@-@@-@@-@# exe (47)
    write #-----------------------------------------------------------@@@@@@@@2@@# write (12)
          ######################################################################## (70)
     2013  0000000001111111111222222222233000000000111111111122222222200000000011
    01-03  1234567890123456789012345678901123456789012345678901234567812345678901
           TWTFSSMTWTFSSMTWTFSSMTWTFSSMTWTFSSMTWTFSSMTWTFSSMTWTFSSMTWTFSSMTWTFSSM

License
=======
----------------------------------------------------------------------------
Copyright (c) 2013 Tim Park

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
