Utility
=======

First a few points.

1) this folder and read me is my way of communicating to you what this project is about, and to help me keep track of all
   the things still neeeded to be done, obviously.
2) Please read all of it, as the code is not well organized (I did it in a very big hurry), so your educated about 
the project before really digging into the not so pretty code.

3) If you use any of this ( not that I have a belief that anyone will ) please read and adhere to the Liscense!

This project encompasses some General utilities for windows devleopment using mingw32/mingw64 32bit and 64 bit tool chains 
(some of it does compile and work with msvc as well, as such the headers include approperate #pragma's for linking). These
for the most part are considered untested beyond the simple "check to see if it works" projects I use to debug the code. 
Their purpose is to prevent me from reinventing the wheel so to speak when creating utilities to use on my own machine 
and not having to link with a very combursome library, or use one that's potentially approperate however has a code base
and ui look of say, windows 95. Secondly to make programming for the Windows platform with mingw tools a bit more fun.

Under no circumstances what so ever should anyone change the interface to the classes in this repository, unless noted.

If you feel the need to do so, please fork!

notes about the classes:

the template classes covering windows BCrypt include some very handy typedef's,and in fact if you try to use anything 
other than the types defined in that header for the implimentation classes the templates will fail to compile. this WAS by design.

There is a BaseWindow template class that impliments a window using thunks, I included macro's for implimenting the huge
switch statement for windows messages much like ATL or MFC just because it is actually less cumbersome to impliment a small
standlone ultra fast window with mingw, and to note, it fails to comile with MSVC on the 32 bit x86 platform. I could potentially
fix this but dont really feel like it because if your going to be using MSVC, frankly, there's ATL/WTL.

There is a ClientSocket class that impliments an asyncrious client socket class, it requires the use of some enums in 
it's constructer to desginate what kind of socket you want to create, currently only stream sockets really are supported by
it's interface (THIS IS THE ONE INTERFACE THAT FOLKS CAN EXTEND).
There is a SslClientSocket class that impliments a Stream socket for ssl. Yes, it works, at least to pull say, google's 
search page, it's quite unfinished, probably buggy as all get at, so if you use it, be awaire of this, and do so carefully. Known issues include the execption struct in it, FormatMessage isn't called correcly and frankly, the complexity of getting this class so much as working has taken up my enegery directed to this file.

The socket classes use std::function for their callbacks (ie connect, recieve, and close).

Check back periodically, I'll add new stuff as I get done with it. 
