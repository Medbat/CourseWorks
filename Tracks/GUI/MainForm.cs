using System;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using Tracks;

namespace GUI
{
    public partial class MainForm : Form
    {
        private TrackFinder trackHandler;

        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            AllowDrop = true;
            trackHandler = new TrackFinder();
            водаИлиГородToolStripMenuItem.Text = @"Смена типа местности (земля)";
            trackHandler.WaterOrGround = true;
            //trackHandler.OpenImage(@"d:\Capture3.png");
            //MapImageBox.Image = trackHandler.Map;
        }

        private void выходToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void ФайлToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var openFileDialog1 = new OpenFileDialog
            {
                Filter = @"Файл изображения|*.bmp;*.jpg;*.png",
                Title = @"Выберите изображение"
            };

            if (openFileDialog1.ShowDialog() != DialogResult.OK) return;
            trackHandler.OpenImage(openFileDialog1.FileName);
            MapImageBox.Image = trackHandler.Map;
        }

        private void DrawPoints()
        {
            var gr = MapImageBox.CreateGraphics();
            if (trackHandler.GraphNodes == null || !trackHandler.GraphNodes.Any())
                return;
            foreach (var graphNode in trackHandler.GraphNodes)
            {
                gr.FillEllipse(Brushes.Red, new Rectangle(graphNode.p.X - 5,
                    graphNode.p.Y - 5, 10, 10));
            }
        }

        private void MapImageBox_Click(object sender, EventArgs e)
        {
            // если картинки нету, ничего не делать
            if (trackHandler.Map == null)
                return;
            Point click = new Point(MapImageBox.PointToClient(Cursor.Position).Y, MapImageBox.PointToClient(Cursor.Position).X);
            // если точка поставлена вне изображения
            if (click.Y >= trackHandler.Map.Width || click.X >= trackHandler.Map.Height ||
                click.Y < 0 || click.X < 0)
                return;
            if (trackHandler.AddPoint(new Point(click.Y, click.X)))
                DrawPoints();
        }

        private void MainForm_DragEnter(object sender, DragEventArgs e)
        {
            e.Effect = e.Data.GetDataPresent(DataFormats.FileDrop) ? DragDropEffects.Copy : DragDropEffects.None;
        }

        private void MainForm_DragDrop(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.None;
            if (!e.Data.GetDataPresent(DataFormats.FileDrop))
                return;
            if (!(trackHandler.OpenImage(((string[]) e.Data.GetData(DataFormats.FileDrop))[0])))
            {
                MapImageBox.Image = MapImageBox.ErrorImage;
                return;
            }
            MapImageBox.Image = trackHandler.Map;
            trackHandler.DeleteAllPoints();
        }

        private void рассчитатьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Enabled = false;
            trackHandler.ToCount();
            this.Enabled = true;
            MapImageBox.Image = trackHandler.Map;
            DrawPoints();
        }

        private void удалитьВсеТочкиToolStripMenuItem_Click(object sender, EventArgs e)
        {
            trackHandler.ClearMap();
            MapImageBox.Image = trackHandler.Map;
        }

        private void водаИлиГородToolStripMenuItem_Click(object sender, EventArgs e)
        {
            trackHandler.WaterOrGround = !trackHandler.WaterOrGround;
            водаИлиГородToolStripMenuItem.Text = trackHandler.WaterOrGround ? 
                "Смена типа местности (земля)" : "Смена типа местности (вода)";
        }
    }
}
