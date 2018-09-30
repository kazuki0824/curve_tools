using DotNetScilab;
using modeling;
using System.IO.Ports;

namespace Control
{
    class Program
    {
        private static Scilab m_oSCilab;
        private static MotorDriver device;
        static MotorModel motor = new MotorModel { Voltage = 24, Inertial_load = 5.85 * 0.0001, K = 1265 / (156 - 2.8) * 0.001, Resistance = 0.299, Inductance = 82.3 * 0.000001, PwmFreq_Hz = 343.0 };
        static void Main(string[] args)
        {

            Scilab.SetDllDirectory(@"C:\Program Files\scilab-6.0.0\bin");
            m_oSCilab = new Scilab(true);
            if (m_oSCilab.execScilabScript(System.AppDomain.CurrentDomain.BaseDirectory + "synth_core.sce") != 0)
            {
                //err
            }
            var cmd = string.Format("[sys_c_p, sys_u_p, sys_u, sys_f]=Motor(24,{0},{1},{2},{3},{4},{5});", 0, motor.Inertial_load, motor.K, motor.Resistance, motor.Inductance, 1.0 / motor.PwmFreq_Hz);
            m_oSCilab.SendScilabJob(cmd);

            m_oSCilab.SendScilabJob("[Fpd, f_u, Ap, Bp]=getFpd(2.0,1.0);");
            var F = m_oSCilab.readNamedMatrixOfDouble("Fpd");

            try
            {

                device = new MotorDriver(args[1]);

                //Set Fpd, ABCD, and C1
            }
            catch (System.Exception)
            {

                throw;
            }

            while (true)
            {
                
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
    }
}
