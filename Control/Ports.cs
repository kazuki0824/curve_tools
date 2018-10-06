using System;
using System.IO.Ports;
namespace Control
{
    class MotorDriver : IDisposable
    {
        private SerialPort s;
        public MotorDriver(string device)
        {
            this.s = new SerialPort(device, 115200, Parity.None);
        }
        public bool Connect()
        {
            try
            {
                this.s.Open();
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }
        bool Disconnect()
        {
            throw new NotImplementedException();
        }

        void IDisposable.Dispose()
        {
            throw new NotImplementedException();
        }

        private void Senddoublearray_AsFloat(double[] a)
        {
            foreach (var item in a)
            {
                var tmp = BitConverter.GetBytes((float)item);
                s.Write(tmp, 0, tmp.Length);
            }
        }

        public bool Polling()
        {
            //TODO:
            if (s.IsOpen)
            {
                s.Write("2008");
                var str = s.ReadLine();
                return true;
            }
            else
            {
                return false;
            }
        }

        public bool WriteMatrix(double[] mat, char prefix)
        {
            //TODO:
            if (s.IsOpen)
            {
                s.Write(prefix.ToString());
                this.Senddoublearray_AsFloat(mat);
                return true;
            }
            else
            {
                return false;
            }

        }
    }
}