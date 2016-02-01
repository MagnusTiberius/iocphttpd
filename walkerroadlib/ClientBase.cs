using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace walkerroadlib
{
    public class ClientBase : AsynchronousClient
    {
        public ClientBase()
        {
            OnReceiveCompleted += OnReceiveCompletedHandler;
        }

        private void OnReceiveCompletedHandler(string response)
        {
            Console.WriteLine(response);
            var header = JsonConvert.DeserializeObject<model.Header>(response);
            if (header.Method.Equals("LOGIN") && header.Status.Equals("OK"))
            {
                Console.WriteLine("Approved");
            }
        }

        public void StartClient()
        {
            Start();
            model.Header h = new model.Header();
            h.Method = "LOGIN";
            h.Source = "CLIENT";
            model.Login l = new model.Login();
            l.UserName = "Test";
            l.Password = "Test";
            h.Body = l;
            var json = JsonConvert.SerializeObject(h);

            Send(string.Format("{0}", json));
            CloseClient();
        }
    }
}
