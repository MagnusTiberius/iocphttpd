using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using walkerroadlib;

using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace walkerroadtest
{
    [TestClass]
    public class UnitTest2
    {
        [TestMethod]
        public void TestMethod1()
        {
            ServerBase listener = new ServerBase();
            ClientBase client = new ClientBase();

            Thread threadServer = new Thread(new ThreadStart(listener.StartServer));
            Thread threadClient = new Thread(new ThreadStart(client.StartClient));

            threadServer.Start();
            Thread.Sleep(100);
            threadClient.Start();
            threadClient.Join();
            threadServer.Abort();
        }
    }
}
