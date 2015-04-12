# DT5 Project

Unix has a `date` command which takes a format string and outputs the current date/time in the requested format. Windows also has a `date` but it does not take parameters, so is useless.

This little project is to create something similar to the unix date command, for windows, allowing the current date to be output in various formats supported by the `strftime` system function.

It is not exactly equivalent to the unix `date` command, but suit my purpose, which was mainly to output the date in the simple number form YYYYMMDD for say dating archives. This can be done by `dt5 "+%Y%m%d"`.

#### Building:

This project uses cmake to generate native build files. So in windows this is -

 1. cd build
 2. cmake ..
 3. cmake --build . --config Release
 
In unix, you could probably use the same, but generally most prefer -

 1. cd build
 2. cmake ..
 3. make
 
#### Running:

As usual `dt5 -?` will show the list of format characters that can be used, and the default format if none is given on the command line.

Have FUN ;=))

Geoff.  
20150412

; eof


