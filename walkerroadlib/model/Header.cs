using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace walkerroadlib.model
{
    public class Header
    {
        public string Method { get; set; }
        public string Source { get; set; }
        public object Body { get; set; }
        public string Status { get; set; }
    }

    public class Status
    {
        public enum Login
        {
            REQUEST = 800001,
            GRANTED,
            DENIED
        }
    }
}
