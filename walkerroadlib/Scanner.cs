using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace walkerroadlib
{
    public class Scanner
    {
        private byte[] strInput;
        private int m_pos = 0;

        public void Input(byte[] str)
        {
            strInput = str;
        }

        public void Input(string str)
        {
            strInput = ASCIIEncoding.ASCII.GetBytes(str);
        }

        public List<byte> AcceptUntil(byte[] str)
        {
            List<byte> list = new List<byte>();
            while (!Accept(str) && m_pos <= strInput.Length)
            {
                byte b = strInput[m_pos];
                list.Add(b);
                m_pos++;
            }
            return list;
        }

        public List<byte> AcceptRun(byte[] str)
        {
            List<byte> list = new List<byte>();
            while (Accept(str) && m_pos <= strInput.Length)
            {
                byte b = strInput[m_pos];
                list.Add(b);
                m_pos++;
            }
            return list;
        }

        bool Accept(byte[] str)
        {
            byte b = strInput[m_pos];
            for (int i = 0; i < str.Length; i++)
            {
                if (b.Equals(str[i]))
                {
                    return true;
                }
            }
            return false;
        }


        public void Backup()
        {
            if (m_pos > 0)
            {
                m_pos++;
            }
            else
            {
                m_pos = 0;
            }
        }

        public byte Next()
        {
            if (m_pos < strInput.Length)
            {
                m_pos++;
                return strInput[m_pos];
            }
            else
            {
                return 0;
            }
        }

        public void SkipEmpty()
        {
            while (IsEmpty())
            {
                m_pos++;
            }
        }

        public bool IsEmpty()
        {
            if (m_pos < strInput.Length)
            {
                byte b = strInput[m_pos];
                if (b.Equals('\t') || b.Equals('\r') || b.Equals('\0') || b.Equals(' ') || b.Equals('\n') || b == 13 || b == 10 || b == 0 || b == 32)
                {
                    return true;
                }
            }
            return false;
        }
    }
}
