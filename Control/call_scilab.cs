using modeling;
using DotNetScilab;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Control
{
    class call_scilab
    {
        private static Scilab m_oSCilab;

        internal static void init()
        {
            Scilab.SetDllDirectory(@"C:\Program Files\scilab-6.0.0\bin");
            m_oSCilab = new Scilab(true);
            if (m_oSCilab.execScilabScript(System.AppDomain.CurrentDomain.BaseDirectory + "synth_core.sce") != 0)
            {
                //TODO:err
                throw new System.Exception();
            }
        }
        internal static Discrete_ss getConfig(MotorModel motor)
        {
           
            var cmd = string.Format("[sys_c_p, sys_u_p, sys_u, sys_f]=Motor(24,{0},{1},{2},{3},{4},{5});", 0, motor.Inertial_load, motor.K, motor.Resistance, motor.Inductance, 1.0 / motor.PwmFreq_Hz);
            m_oSCilab.SendScilabJob(cmd);

            m_oSCilab.SendScilabJob("[Fpd, f_u, Ap, Bp]=getFpd(2.0,1.0);");
            var F = m_oSCilab.readNamedMatrixOfDouble("Fpd");


        }

    }
}
