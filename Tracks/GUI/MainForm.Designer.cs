namespace GUI
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.MapImageBox = new System.Windows.Forms.PictureBox();
            this.ОткрытьФайлToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.выходToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.MenuStrip = new System.Windows.Forms.MenuStrip();
            this.рассчитатьToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.удалитьВсеТочкиToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.водаИлиГородToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            ((System.ComponentModel.ISupportInitialize)(this.MapImageBox)).BeginInit();
            this.MenuStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // MapImageBox
            // 
            this.MapImageBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MapImageBox.Location = new System.Drawing.Point(0, 24);
            this.MapImageBox.Name = "MapImageBox";
            this.MapImageBox.Size = new System.Drawing.Size(627, 226);
            this.MapImageBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.MapImageBox.TabIndex = 0;
            this.MapImageBox.TabStop = false;
            this.MapImageBox.Click += new System.EventHandler(this.MapImageBox_Click);
            // 
            // ОткрытьФайлToolStripMenuItem
            // 
            this.ОткрытьФайлToolStripMenuItem.Name = "ОткрытьФайлToolStripMenuItem";
            this.ОткрытьФайлToolStripMenuItem.Size = new System.Drawing.Size(98, 20);
            this.ОткрытьФайлToolStripMenuItem.Text = "Открыть файл";
            this.ОткрытьФайлToolStripMenuItem.Click += new System.EventHandler(this.ФайлToolStripMenuItem_Click);
            // 
            // выходToolStripMenuItem
            // 
            this.выходToolStripMenuItem.Name = "выходToolStripMenuItem";
            this.выходToolStripMenuItem.Size = new System.Drawing.Size(53, 20);
            this.выходToolStripMenuItem.Text = "Выход";
            this.выходToolStripMenuItem.Click += new System.EventHandler(this.выходToolStripMenuItem_Click);
            // 
            // MenuStrip
            // 
            this.MenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ОткрытьФайлToolStripMenuItem,
            this.рассчитатьToolStripMenuItem,
            this.удалитьВсеТочкиToolStripMenuItem,
            this.водаИлиГородToolStripMenuItem,
            this.выходToolStripMenuItem});
            this.MenuStrip.Location = new System.Drawing.Point(0, 0);
            this.MenuStrip.Name = "MenuStrip";
            this.MenuStrip.Size = new System.Drawing.Size(627, 24);
            this.MenuStrip.TabIndex = 2;
            this.MenuStrip.Text = "Меню";
            // 
            // рассчитатьToolStripMenuItem
            // 
            this.рассчитатьToolStripMenuItem.Font = new System.Drawing.Font("Calibri", 9.75F);
            this.рассчитатьToolStripMenuItem.ForeColor = System.Drawing.Color.Black;
            this.рассчитатьToolStripMenuItem.Name = "рассчитатьToolStripMenuItem";
            this.рассчитатьToolStripMenuItem.Size = new System.Drawing.Size(81, 20);
            this.рассчитатьToolStripMenuItem.Text = "Рассчитать";
            this.рассчитатьToolStripMenuItem.Click += new System.EventHandler(this.рассчитатьToolStripMenuItem_Click);
            // 
            // удалитьВсеТочкиToolStripMenuItem
            // 
            this.удалитьВсеТочкиToolStripMenuItem.Font = new System.Drawing.Font("Calibri", 9.75F);
            this.удалитьВсеТочкиToolStripMenuItem.ForeColor = System.Drawing.Color.Black;
            this.удалитьВсеТочкиToolStripMenuItem.Name = "удалитьВсеТочкиToolStripMenuItem";
            this.удалитьВсеТочкиToolStripMenuItem.Size = new System.Drawing.Size(98, 20);
            this.удалитьВсеТочкиToolStripMenuItem.Text = "Удалить точки";
            this.удалитьВсеТочкиToolStripMenuItem.Click += new System.EventHandler(this.удалитьВсеТочкиToolStripMenuItem_Click);
            // 
            // водаИлиГородToolStripMenuItem
            // 
            this.водаИлиГородToolStripMenuItem.Name = "водаИлиГородToolStripMenuItem";
            this.водаИлиГородToolStripMenuItem.Size = new System.Drawing.Size(144, 20);
            this.водаИлиГородToolStripMenuItem.Text = "Смена типа местности";
            this.водаИлиГородToolStripMenuItem.Click += new System.EventHandler(this.водаИлиГородToolStripMenuItem_Click);
            // 
            // MainForm
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.BackColor = System.Drawing.Color.SeaShell;
            this.ClientSize = new System.Drawing.Size(627, 250);
            this.Controls.Add(this.MapImageBox);
            this.Controls.Add(this.MenuStrip);
            this.MainMenuStrip = this.MenuStrip;
            this.Name = "MainForm";
            this.Text = "Track Finder";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.DragDrop += new System.Windows.Forms.DragEventHandler(this.MainForm_DragDrop);
            this.DragEnter += new System.Windows.Forms.DragEventHandler(this.MainForm_DragEnter);
            ((System.ComponentModel.ISupportInitialize)(this.MapImageBox)).EndInit();
            this.MenuStrip.ResumeLayout(false);
            this.MenuStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox MapImageBox;
        private System.Windows.Forms.ToolStripMenuItem ОткрытьФайлToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem выходToolStripMenuItem;
        private System.Windows.Forms.MenuStrip MenuStrip;
        private System.Windows.Forms.ToolStripMenuItem рассчитатьToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem удалитьВсеТочкиToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem водаИлиГородToolStripMenuItem;
    }
}

