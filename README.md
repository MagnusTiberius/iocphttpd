# iocphttpd
A small HTTP server written in C++ using IO Completion Ports.

Slack: Workspace URL: iocphttp.slack.com 

Demo Website:
http://74.208.133.86:8080/

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
5.  Websocket enabled, just added the demo for Websocket in index.html. Check it out.
6.  Key-Value store implemented using babudb library.

Screenshots:

1.  Browser load timing, http://postimg.org/image/jeyhl63ur/
2.  MelonJS test load, http://postimg.org/image/3zbkoqw0f/  another http://postimg.org/image/69jpvetsz/

Dependencies:

1) Uses jsoncpp, see https://github.com/open-source-parsers/jsoncpp for details.

2) Demo web game taken from melonJS, see https://github.com/melonjs/melonJS for details.

Progress:

12/30/2015 - Service is now able to load the executable. Start will run it and Stop will terminate the executable.

2/22/2016  - Adding websocket functionality. See websocket_l for details. websockettest.js is the test client and SocketCompletionPortServerWS.cpp is the server code.

3/17/2016 - Websocket handshake is now implemented.
