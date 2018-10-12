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

        private byte[] Compose(double[] mat, char prefix)
        {
            //TODO:
            byte row, column;
            switch (prefix)         
            {
                case 'a':
                    row = 3; column = 3;
                    break;
                case 'b':
                    row = 3; column = 4;
                    break;
                case 'c':
                    row = 1;column = 3;
                    break;
                case 'f':
                    row = 3; column = 4;
                    break;
                case 'r':
                    row = 2; column = 1;
                    break;
                case 'V':
                    row = 8; column = 8;
                    break;
                default:
                    row = 0; column = 0;
                    break;
            }
            //Write header
            var size = 6 + 4 * (row * column);
            byte[] packet = new byte[size];
            packet[0] = 0xFF;
            var szbyte = BitConverter.GetBytes((ushort)size);
            packet[1] = szbyte[0];
            packet[2] = szbyte[1];
            packet[3] = (byte)prefix;
            packet[4] = row;
            packet[5] = column;
            var offset = 6;

            //Write data
            foreach (var item in mat)
            {
                var u_data = BitConverter.GetBytes((float)item);
                packet[offset + 0] = u_data[0];
                packet[offset + 1] = u_data[1];
                packet[offset + 2] = u_data[2];
                packet[offset + 3] = u_data[3];
                offset += 4;
            }
            return packet;
        }
        public void WriteMatrix(double[] mat, char prefix)
        {
            if (this.s.IsOpen)
            {
                var bytes = this.Compose(mat, prefix);
                this.s.Write(bytes, 0, bytes.Length);
            }
        }
    }
}