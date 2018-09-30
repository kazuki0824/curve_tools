using DotNetScilab;
using modeling;
namespace Synth_main
{
    class MyCode
    {
        private static Scilab m_oSCilab;
        static MotorModel motor = new MotorModel { Inertial_load = 142 * 10 ^ -7 , K =};
        static void Main(string[] args)
        {
            m_oSCilab = new Scilab(true);
            if (m_oSCilab.execScilabScript("system_synthesis.sce")!=0)
            {
                //err
            }
            m_oSCilab.SendScilabJob(string.Format("[sys_c_p, sys_u_p, sys_u, sys_f]=Motor(24,b,J,R,L,1/343);",0, 142 * 10 ^ -7, 0.299, 82.3 * (10 ^ -6)));

            m_oSCilab.SendScilabJob("[Fpd]=getFpd();");
            m_oSCilab.readNamedMatrixOfDouble("Fpd");
        }
        void GetC1Controller(double KalmanGain, MotorModel m)
        {
            m_oSCilab.SendScilabJob("X =[C1.A C1.B ; C1.C C1.D];");
        }
    }
}