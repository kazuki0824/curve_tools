using modeling;
using System.Threading;
using System.IO.Ports;
using System.IO.Pipes;
using System.IO;
using System;

namespace Control
{
    class Program
    {
        private static MotorDriver device;
        static MotorModel motor = new MotorModel { Voltage = 24, Inertial_load = 5.85 * 0.0001, K = 1265 / (156 - 2.8) * 0.001, Resistance = 0.299, Inductance = 82.3 * 0.000001, PwmFreq_Hz = 343.0 };
        static void Main(string[] args) //arg = path com pipename
        {
            try
            {
                device = new MotorDriver(args[1]);

                //TODO:Set Fpd, ABCD, and C1
                var sys_f = call_scilab.getConfig(motor);

            }
            catch (System.Exception)
            {
                throw;
            }

            var Tr = 1.0 / motor.PwmFreq_Hz; //Sampling theorem?

            var t = Program.PipeServer;

            while (true)
            {
                System.Diagnostics.Stopwatch sw = System.Diagnostics.Stopwatch.StartNew();
                if (sw.Elapsed.TotalSeconds >= Tr)
                {
                    
                    
                    sw.Reset();
                }
            }
        }
        double[] GetC1Controller(double[] KalmanGain, MotorModel m)
        {
            m_oSCilab.createNamedMatrixOfDouble("Kf", 3, 1, KalmanGain);
            m_oSCilab.SendScilabJob("[C1]=C1synth(Ap, Bp, sys_u, sys_f, Fpd, Kf)");
            m_oSCilab.SendScilabJob("X =[C1.A C1.B ; C1.C C1.D];");
            var C2 = m_oSCilab.readNamedMatrixOfDouble("X");
            return C2;
        }

        static async void PipeServer(string name)
        {
            {
                using (var stream = new NamedPipeServerStream(name))
                {
                    // クライアントからの接続を待つ
                    await stream.WaitForConnectionAsync();

                    // メッセージを一定の周期で送信
                    using (var writer = new StreamWriter(stream))
                    {
                        writer.AutoFlush = true;
                        while(true)
                        {
                            if (!stream.IsConnected)
                                break;
                            Thread.Sleep(10);
                            writer.WriteLine("{0}, {1}",DateTime.Now.ToOADate());
                        }
                    }
                }


            }

        }
    }
}
