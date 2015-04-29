# iocphttpd
A small HTTP server written in C++ using IO Completion Ports.

Project Website:
http://bgonzales20001.wix.com/iocphttp

Repository:
https://github.com/MagnusTiberius/iocphttpd

Continuous Integration:
https://ci.appveyor.com/api/projects/status/lpsxd8dcbxapj3b5

iocphttpd features:

1.  Small footprint means you can use it on a low spec machine. One good example is a Virtual Server having 1GB Ram.
2.  URL routing makes it easy for you to write a handler for a given URL route.
3.  URL parameters, you can simply retrieve the parameters from a list.
4.  JSON support is available in the API, see jsoncpp for details.


Dependencies:

1) Uses jsoncpp, see https://github.com/open-source-parsers/jsoncpp for details.

TODO:

1) Setup AppVeyor to install bison.exe  -- done

JOURNAL:

4/10/15 - Configured win flex bison to generate output that could support multiple .y and .l files in one directory.

4/13/15 - Still figuring out how to copy a byte array to WSABUF.

4/21/15 - Read http://www.json.org/JSONRequest.html doc. Interesting read, see http://www.webalizer.org/simpleton.html

4/22/15 - Screenshot showing browser load timing, http://postimg.org/image/jeyhl63ur/

Ref:

1) https://docs.freebsd.org/info/bison/bison.info.Pure_Calling.html

2) http://www.informit.com/articles/article.aspx?p=2036582&seqNum=3
   Secure Coding in C and C++: Strings and Buffer Overflows

