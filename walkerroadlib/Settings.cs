using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace walkerroadlib
{

    public delegate string OnReceiveData(string response);
    public delegate string OnSendData(string message);
    public delegate string OnClientLoop();


    public class Settings
    {
        public const int PORTNUM = 11000;
    }
}
