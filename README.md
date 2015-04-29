# iocphttpd
A small HTTP server written in C++ using IO Completion Ports.

Project Website:
http://bgonzales20001.wix.com/iocphttp

Repository:
https://github.com/MagnusTiberius/iocphttpd

Continuous Integration:
https://ci.appveyor.com/project/MagnusTiberius/iocphttpd

iocphttpd features:

1.  Small footprint means you can use it on a low spec machine. One good example is a Virtual Server having 1GB Ram.
2.  URL routing makes it easy for you to write a handler for a given URL route.
3.  URL parameters, you can simply retrieve parameters from a list.
4.  JSON support is available in the API, see jsoncpp for details.


Here is screenshot showing browser load timing, http://postimg.org/image/jeyhl63ur/

Dependencies:

1) Uses jsoncpp, see https://github.com/open-source-parsers/jsoncpp for details.



