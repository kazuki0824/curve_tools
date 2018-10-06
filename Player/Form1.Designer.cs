namespace Player
{
    partial class Form1
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージド リソースを破棄する場合は true を指定し、その他の場合は false を指定します。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.Label label1;
            System.Windows.Forms.Label label2;
            System.Windows.Forms.Label label3;
            System.Windows.Forms.Label label4;
            System.Windows.Forms.Label label6;
            System.Windows.Forms.Label label7;
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.propertyGrid1 = new System.Windows.Forms.PropertyGrid();
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.Jtxt = new System.Windows.Forms.TextBox();
            this.Ktxt = new System.Windows.Forms.TextBox();
            this.Rtxt = new System.Windows.Forms.TextBox();
            this.Ltxt = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.Freqtxt = new System.Windows.Forms.TextBox();
            this.Etxt = new System.Windows.Forms.TextBox();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            label1 = new System.Windows.Forms.Label();
            label2 = new System.Windows.Forms.Label();
            label3 = new System.Windows.Forms.Label();
            label4 = new System.Windows.Forms.Label();
            label6 = new System.Windows.Forms.Label();
            label7 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // propertyGrid1
            // 
            this.propertyGrid1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.propertyGrid1.Location = new System.Drawing.Point(516, 12);
            this.propertyGrid1.Name = "propertyGrid1";
            this.propertyGrid1.Size = new System.Drawing.Size(272, 337);
            this.propertyGrid1.TabIndex = 0;
            // 
            // trackBar1
            // 
            this.trackBar1.Location = new System.Drawing.Point(12, 23);
            this.trackBar1.Maximum = 314;
            this.trackBar1.Minimum = -314;
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(498, 56);
            this.trackBar1.TabIndex = 1;
            this.trackBar1.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(label1, 0, 1);
            this.tableLayoutPanel1.Controls.Add(label2, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.Jtxt, 1, 1);
            this.tableLayoutPanel1.Controls.Add(this.Ktxt, 1, 2);
            this.tableLayoutPanel1.Controls.Add(label3, 0, 3);
            this.tableLayoutPanel1.Controls.Add(label4, 0, 4);
            this.tableLayoutPanel1.Controls.Add(this.Rtxt, 1, 3);
            this.tableLayoutPanel1.Controls.Add(this.Ltxt, 1, 4);
            this.tableLayoutPanel1.Controls.Add(this.button1, 1, 6);
            this.tableLayoutPanel1.Controls.Add(label6, 0, 5);
            this.tableLayoutPanel1.Controls.Add(this.Freqtxt, 1, 5);
            this.tableLayoutPanel1.Controls.Add(label7, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.Etxt, 1, 0);
            this.tableLayoutPanel1.Location = new System.Drawing.Point(310, 135);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 7;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(200, 214);
            this.tableLayoutPanel1.TabIndex = 2;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new System.Drawing.Point(3, 28);
            label1.Name = "label1";
            label1.Size = new System.Drawing.Size(15, 15);
            label1.TabIndex = 0;
            label1.Text = "J";
            // 
            // label2
            // 
            label2.Location = new System.Drawing.Point(3, 56);
            label2.Name = "label2";
            label2.Size = new System.Drawing.Size(43, 15);
            label2.TabIndex = 0;
            label2.Text = "K";
            // 
            // Jtxt
            // 
            this.Jtxt.Location = new System.Drawing.Point(73, 31);
            this.Jtxt.Name = "Jtxt";
            this.Jtxt.Size = new System.Drawing.Size(100, 22);
            this.Jtxt.TabIndex = 1;
            // 
            // Ktxt
            // 
            this.Ktxt.Location = new System.Drawing.Point(73, 59);
            this.Ktxt.Name = "Ktxt";
            this.Ktxt.Size = new System.Drawing.Size(100, 22);
            this.Ktxt.TabIndex = 2;
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new System.Drawing.Point(3, 84);
            label3.Name = "label3";
            label3.Size = new System.Drawing.Size(16, 15);
            label3.TabIndex = 3;
            label3.Text = "R";
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new System.Drawing.Point(3, 112);
            label4.Name = "label4";
            label4.Size = new System.Drawing.Size(15, 15);
            label4.TabIndex = 4;
            label4.Text = "L";
            // 
            // Rtxt
            // 
            this.Rtxt.Location = new System.Drawing.Point(73, 87);
            this.Rtxt.Name = "Rtxt";
            this.Rtxt.Size = new System.Drawing.Size(100, 22);
            this.Rtxt.TabIndex = 5;
            // 
            // Ltxt
            // 
            this.Ltxt.Location = new System.Drawing.Point(73, 115);
            this.Ltxt.Name = "Ltxt";
            this.Ltxt.Size = new System.Drawing.Size(100, 22);
            this.Ltxt.TabIndex = 6;
            // 
            // button1
            // 
            this.button1.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.button1.Location = new System.Drawing.Point(97, 179);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 7;
            this.button1.Text = "Apply";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(65, 93);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(54, 15);
            this.label5.TabIndex = 3;
            this.label5.Text = "Speed: ";
            // 
            // label6
            // 
            label6.AutoSize = true;
            label6.Location = new System.Drawing.Point(3, 140);
            label6.Name = "label6";
            label6.Size = new System.Drawing.Size(64, 15);
            label6.TabIndex = 8;
            label6.Text = "Freq_kHz";
            // 
            // Freqtxt
            // 
            this.Freqtxt.Location = new System.Drawing.Point(73, 143);
            this.Freqtxt.Name = "Freqtxt";
            this.Freqtxt.Size = new System.Drawing.Size(100, 22);
            this.Freqtxt.TabIndex = 9;
            // 
            // label7
            // 
            label7.AutoSize = true;
            label7.Location = new System.Drawing.Point(3, 0);
            label7.Name = "label7";
            label7.Size = new System.Drawing.Size(15, 15);
            label7.TabIndex = 10;
            label7.Text = "E";
            // 
            // Etxt
            // 
            this.Etxt.Location = new System.Drawing.Point(73, 3);
            this.Etxt.Name = "Etxt";
            this.Etxt.Size = new System.Drawing.Size(100, 22);
            this.Etxt.TabIndex = 11;
            // 
            // checkBox1
            // 
            this.checkBox1.Appearance = System.Windows.Forms.Appearance.Button;
            this.checkBox1.Location = new System.Drawing.Point(68, 181);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(101, 63);
            this.checkBox1.TabIndex = 4;
            this.checkBox1.Text = "Link";
            this.checkBox1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.checkBox1.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AcceptButton = this.button1;
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 361);
            this.Controls.Add(this.checkBox1);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Controls.Add(this.trackBar1);
            this.Controls.Add(this.propertyGrid1);
            this.Name = "Form1";
            this.Text = "MotorLink";
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.PropertyGrid propertyGrid1;
        private System.Windows.Forms.TrackBar trackBar1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.TextBox Jtxt;
        private System.Windows.Forms.TextBox Ktxt;
        private System.Windows.Forms.TextBox Rtxt;
        private System.Windows.Forms.TextBox Ltxt;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox Freqtxt;
        private System.Windows.Forms.TextBox Etxt;
        private System.Windows.Forms.CheckBox checkBox1;
    }
}

