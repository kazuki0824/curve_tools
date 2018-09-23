using System;

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
}
