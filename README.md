# tim-tam-tom
A tic-tac-toe game in curses

Should be fairly easy to build on any properly-configured Linux system with
NCurses (which, I would think, should be any Linux system!).

Just run `make`.

I hope to get a Windows build at some point that is reliable. I have gotten
it to work before, but it's not as clean as I would like it. Perhaps using
pdcurses in place of NCurses... I don't know.
***

UPDATE 1/27/2017
I just successfully built and played tim-tam-tom on the new Bash for Windows, available on Windows 10. I had gcc, make, libncurses5, and libncurses5-dev installed:

$ sudo apt-get install gcc make libncurses5 libncurses5-dev

I'm not entirely sure this is all you need; I already had a build environment prepared from another project. If you have an environment suitable for building C programs, the only special thing you MIGHT need to install is the curses libraries.

This approach will *probably* work with Cygwin as well. I don't have Cygwin installed at present.

Still need a windows build, but I think my current windows machine has a messed up curses library, or some other aspect of the toolset is damaged or misconfigured. I don't use it that much lately, but at some point I will have to take a look. When I do I will use this project as a way to test it.
***
