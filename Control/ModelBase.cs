using System;
using System.Collections.Generic;

namespace modeling
{
    public class MotorModel
    {
        public double Inductance { get; set; }
        public double Resistance { get; set; }
        public double K { get; set; }
        public double Inertial_load { get; set; }
        public double Voltage { get; set; }
        public double PwmFreq_Hz { get; set; }
        
    }

    public class Discrete_ss
    {
        public List<double> A { get; set; }
        public List<double> B { get; set; }
        public List<double> C { get; set; }
        public List<double> F { get; set; }
    }
}
