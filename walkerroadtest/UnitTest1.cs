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
    public class UnitTest1
    {
        [TestMethod]
        public void TestMethod1()
        {
            AsynchronousSocketListener listener = new AsynchronousSocketListener();
            AsynchronousClient client = new AsynchronousClient();

            Thread threadServer = new Thread(new ThreadStart(listener.Start));
            Thread threadClient = new Thread(new ThreadStart(client.Start));

            threadServer.Start();
            Thread.Sleep(5000);
            threadClient.Start();
            threadClient.Join();
            threadServer.Abort();
        }
    }
}
