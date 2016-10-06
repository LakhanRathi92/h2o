# h2o-port-MSVC

## H2o

H2o is a standalone HTTP server that supports http1.x and http2, developed by Kazuho Oku and others and released under MIT license.

## libh2o

Is a software library for H2o. There are few examples that come with libh2o. Instructions as how to run those examples with Visual Studio are given in Instructions.txt file.

## Our Contribution

We wanted to port libh2o to Windows environment, specifically Visual Studio,  while avoiding any kind
of GPL-LGPL based libraries. The work is done under dozens of ifdefs so it should be compilable on both
Windows and Posix based systems. 

### Note 
Stand-alone h2o server isn't supported yet

### Testing Environment
Visual Studio 2015, Windows 10. 

### Few of the changings 
To get an idea of what we changed to make libh2o work under Visual studio please have a look at win-changings.txt

#### External Libraries used (licensed under MIT)
  - [MMAN](https://code.google.com/archive/p/mman-win32/)
  - [dirent.h](https://github.com/tronkko/dirent)
  
### Acknowledgement



