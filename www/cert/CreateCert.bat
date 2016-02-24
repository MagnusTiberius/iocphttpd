
makecert.exe -n "CN=IOCPHTTPRoot" -r -pe -a sha512 -len 4096 -cy authority -sv IOCPHTTPRoot.pvk -sr LocalMachine -ss Root IOCPHTTPRoot.cer
 
pvk2pfx.exe -pvk IOCPHTTPRoot.pvk -spc IOCPHTTPRoot.cer -pfx IOCPHTTPRoot.pfx -po test123