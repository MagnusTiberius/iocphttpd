using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace walkerroadlib
{
    public class ServerBase : AsynchronousSocketListener
    {
        public ServerBase()
        {
            OnReceiveData += OnReceiveDataHandler;
        }

        private string OnReceiveDataHandler(string response)
        {
            var header = JsonConvert.DeserializeObject<model.Header>(response);
            if (header.Method.Equals("LOGIN") && header.Status.Equals(model.Status.Login.REQUEST.ToString()))
            {
                var body = header.Body.ToString();
                var login = JsonConvert.DeserializeObject<model.Login>(body);

                model.Header h = new model.Header();
                h.Method = "LOGIN";
                h.Source = "SERVER";
                h.Status = model.Status.Login.GRANTED.ToString();
                var jsonReply = JsonConvert.SerializeObject(h);
                return jsonReply;
            }

            string reply = string.Format("Server received the message: {0}", response);
            Console.WriteLine(reply);
            return reply;
        }

        public void StartServer()
        {
            Start();
        }
    }
}
