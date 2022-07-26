﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace Player
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            propertyGrid1.SelectedObject = serialPort1;
            trackBar1_ValueChanged(this, null);

            chart1.Series["Rotation"].Points.AddXY(DateTime.Now.ToOADate(),1.22);
            System.Threading.Thread.Sleep(100);
            chart1.Series["Rotation"].Points.AddXY(DateTime.Now.ToOADate(), -1);
        }

        private void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            label5.Text = string.Format("{0}[rad/s]", (trackBar1.Value / 314.0));
        }
    }
}
