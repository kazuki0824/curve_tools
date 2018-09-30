using System;
using System.IO.Ports;
namespace Control
{
    class MotorDriver : IDisposable
    {
        public MotorDriver(string device)
        {
            throw new NotImplementedException();
        }
        bool Connect()
        {
            throw new NotImplementedException();
        }
        bool Disconnect()
        {
            throw new NotImplementedException();
        }

        void IDisposable.Dispose()
        {
            throw new NotImplementedException();
        }

        bool Heartbeat()
        {
            //TODO:
        }
    }
}