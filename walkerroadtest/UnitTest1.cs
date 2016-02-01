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
            TestServer listener = new TestServer();
            TestClient client = new TestClient();

            Thread threadServer = new Thread(new ThreadStart(listener.StartServer));
            Thread threadClient = new Thread(new ThreadStart(client.StartClient));

            threadServer.Start();
            Thread.Sleep(100);
            threadClient.Start();
            threadClient.Join();
            threadServer.Abort();
        }
    }

    public class TestServer : AsynchronousSocketListener
    {
        public TestServer()
        {
            OnReceiveData += OnReceiveDataHandler;
        }

        private string OnReceiveDataHandler(string response)
        {
            string reply = string.Format("Server received the message: {0}", response);
            Console.WriteLine(reply);
            return reply;
        }

        public void StartServer()
        {
            Start();
        }
    }


    public class TestClient: AsynchronousClient
    {
        public TestClient()
        {
            OnReceiveCompleted += OnReceiveCompletedHandler;
        }

        private void OnReceiveCompletedHandler(string response)
        {
            Console.WriteLine(response);
        }

        public void StartClient()
        {
            Start();
            Send("This is a test<EOF>");
            CloseClient();
        }
    }
}
