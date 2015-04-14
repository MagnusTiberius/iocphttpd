# iocphttpd
A small HTTP server written in C++ using IO Completion Ports.

Project Website:
http://bgonzales20001.wix.com/iocphttp

Repository:
https://github.com/MagnusTiberius/iocphttpd

Continuous Integration:
https://ci.appveyor.com/project/MagnusTiberius/iocphttpd

Dependencies:

1) Uses jsoncpp, see https://github.com/open-source-parsers/jsoncpp for details.

TODO:

1) Setup AppVeyor to install bison.exe  -- done

JOURNAL:

4/10/15 - Configured win flex bison to generate output that could support multiple .y and .l files in one directory.

4/13/15 - Still figuring out how to copy a byte array to WSABUF.

Ref:

1) https://docs.freebsd.org/info/bison/bison.info.Pure_Calling.html
