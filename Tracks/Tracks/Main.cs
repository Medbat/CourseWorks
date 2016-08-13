
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace Tracks
{
    public class TrackFinder
    {
        // 0 - вода, 1 - земля
        public bool WaterOrGround;

        // изображение для вывода и обработки
        public Bitmap Map { get; private set; }

        // сохраняем путь для перезагрузки изображения
        private string filePath;

        // 0 (-2) - свободно, -1 (-1) - занято
        private int[,] IntMap;

        // конструкторы
        public TrackFinder()
        {
            
        }
        public TrackFinder(string filename)
        {
            OpenImage(filename);
        }
        // загрузка изображения
        public bool OpenImage(string filename)
        {
            filePath = filename;
            try
            {
                reloadImage();
            }
            catch (Exception)
            {
                return false;
            }
            Binarize(true);
            DeleteAllPoints();
            return true;
        }
        private void reloadImage()
        {
            var fs = new FileStream(filePath, FileMode.Open);
            try
            {
                Map = new Bitmap(fs);
            }
            catch (Exception)
            {
                throw;
            }
            finally
            {
                fs.Close();
            }
        }

        // очищаем карту от нарисованных путей через замену её на оригинал
        public void ClearMap()
        {
            DeleteAllPoints();
            if (Map == null)
                return;
            using (var fs = new FileStream(filePath, FileMode.Open)) {
                    Map = new Bitmap(fs);
            }
        }

        // описание графа
        public class MyPoint
        {
            public Point p;

            //private int group;
            public int Group { get; set; }

            public MyPoint(Point p)
            {
                this.p = p;
            }
        }
        // координаты точек
        public List<MyPoint> GraphNodes { get; private set; }
        // добавление/удаление точек на карте
        public bool AddPoint(Point p)
        {
            if (IntMap[p.X, p.Y] == -1) return false;
            GraphNodes.Add(new MyPoint(p));
            // добавляем точку с расширением матрицы графа
            int a = GraphWays == null ? 1 : GraphWays.GetLength(0) + 1;
            int b = GraphWays == null ? 1 : GraphWays.GetLength(1) + 1;
            var newGraphWays = new Way[a, b];
            for (int i = 0; i < a - 1; i++)
                for (int j = 0; j < b - 1; j++)
                    newGraphWays[i, j] = GraphWays[i, j];
            for (int i = 0; i < newGraphWays.GetLength(0); i++)
                newGraphWays[i, newGraphWays.GetLength(1) - 1] = new Way();
            for (int j = 0; j < newGraphWays.GetLength(1); j++)
                newGraphWays[newGraphWays.GetLength(0) - 1, j] = new Way();
            newGraphWays[newGraphWays.GetLength(0) - 1, newGraphWays.GetLength(0) - 1].status = WayStatus.Zero;
            GraphWays = newGraphWays;
            return true;
        }
        public void DeleteAllPoints()
        {
            GraphNodes = new List<MyPoint>();
            GraphWays = null;
        }

        public enum WayStatus { NotCalculated, Infinity, Exists, Zero, Selected }
        public class Way
        {
            public WayStatus status = WayStatus.NotCalculated;

            private Point[] _way;
            public Point[] way
            {
                get { return _way; }

                set
                {
                    if (value == null)
                        status = WayStatus.Infinity;
                    else
                    {
                        status = WayStatus.Exists;
                        _way = value;
                    }
                }
            }

            public int GetLength()
            {
                if (status == WayStatus.Infinity)
                    return Int32.MaxValue;
                if (status == WayStatus.Zero || status == WayStatus.NotCalculated)
                    return 0;
                return _way.GetLength(0);
            }
        }

        // ребра графа
        public Way[,] GraphWays;

        // поиск минимального остовного дерева алгоритмом Прима
        private void MinimumSpanningTree(int group)
        {
            var VminusU = new List<Verticle>(GraphNodes.Count);
            VminusU.AddRange(GraphNodes.Select((t, i) => new Verticle {index = i, point = t.p, group = t.Group}).
                Where(gn => gn.group == group));
            if (!VminusU.Any())
                return;
            var U = new List<Verticle>(GraphNodes.Count)
            {
                new Verticle {index = VminusU[0].index, point = VminusU[0].point}
            };
            VminusU.RemoveAt(0);

            while (VminusU.Any())
            {
                var minWay = new Way();
                int minIGW = 0, minJGW = 0, minJ = 0;
                for (var i = 0; i < U.Count(); i++)
                    for (var j = 0; j < VminusU.Count(); j++)
                        if (GraphWays[U[i].index, VminusU[j].index].status == WayStatus.Exists &&
                            (minWay.status == WayStatus.NotCalculated ||
                             GraphWays[U[i].index, VminusU[j].index].GetLength() < minWay.GetLength()))
                        {
                            minIGW = U[i].index;
                            minJGW = VminusU[j].index;
                            minJ = j;
                            minWay = GraphWays[U[i].index, VminusU[j].index];
                        }
                GraphWays[minIGW, minJGW].status = WayStatus.Selected;
                U.Add(new Verticle {index = VminusU[minJ].index, point = VminusU[minJ].point});
                VminusU.RemoveAt(minJ);
            }
        }
        // вспомогательная структура
        private struct Verticle
        {
            public Point point;
            public int index; // индекс вершины в GraphNodes
            public int group;
        }

        // волновой поиск
        private Point[] WaveSearch(Point start, Point goal)
        {
            // соседи
            Point[] neighBoughrs = {new Point(0,-1), new Point(0,1), new Point(1,0), new Point(-1,0)};
            
            // копируем карту для внесения изменений
            var tempMap = new int[IntMap.GetLength(0), IntMap.GetLength(1)];
            for (int i = 0; i < IntMap.GetLength(0); i++)
            {
                for (int j = 0; j < IntMap.GetLength(1); j++)
                {
                    if (IntMap[i, j] == 0)
                        tempMap[i, j] = -2;
                    else
                        tempMap[i, j] = -1;
                }
            }
            
            // изначально волна - 0. отмечаем ею стартовую точку
            int wave;
            tempMap[start.X, start.Y] = wave = 0;

            while (true)
            {
                bool enough = true;
                for (int i = 0; i < tempMap.GetLength(0); i++)
                    for (int j = 0; j < tempMap.GetLength(1); j++)
                        if (tempMap[i, j] == wave)
                            for (int k = 0; k < 4; k++)
                                if (i + neighBoughrs[k].X >= 0 && i + neighBoughrs[k].X < tempMap.GetLength(0) &&
                                    j + neighBoughrs[k].Y >= 0 && j + neighBoughrs[k].Y < tempMap.GetLength(1))
                                    if (tempMap[i + neighBoughrs[k].X, j + neighBoughrs[k].Y] == -2)
                                    {
                                        enough = false;
                                        tempMap[i + neighBoughrs[k].X, j + neighBoughrs[k].Y] = wave + 1;
                                    }
                wave++;
                if (enough)
                    break;
                if (tempMap[goal.X, goal.Y] != -2)
                    break;
            }
            // если путь не найден (его нет)
            if (tempMap[goal.X, goal.Y] == -2)
                return null;
            // восстановить путь
            int length = tempMap[goal.X, goal.Y];
            var lp = new List<Point>();
            var temp = new Point(goal.X, goal.Y);
            lp.Add(temp);
            for (var i = length - 1; i > 0; i--)
            {
                for (int k = 0; k < 4; k++)
                    if (temp.X + neighBoughrs[k].X >= 0 && temp.X + neighBoughrs[k].X < tempMap.GetLength(0) &&
                        temp.Y + neighBoughrs[k].Y >= 0 && temp.Y + neighBoughrs[k].Y < tempMap.GetLength(1))
                        if (tempMap[temp.X + neighBoughrs[k].X, temp.Y + neighBoughrs[k].Y] == i)
                        {
                            temp = new Point(temp.X + neighBoughrs[k].X, temp.Y + neighBoughrs[k].Y);
                            lp.Add(temp);
                            break;
                        }
            }
            lp.Add(start);
            return lp.ToArray();
        }

        // основной метод класса
        public void ToCount()
        {
            if (Map == null)
                return;
            reloadImage();
            if (GraphWays == null)
                return;
            for (int i = 0; i < GraphNodes.Count; i++)
            {
                GraphNodes[i].Group = i;
                for (int j = 0; j < GraphNodes.Count; j++)
                {
                    if (GraphWays[i, j].status == WayStatus.Selected)
                        GraphWays[i, j].status = WayStatus.Exists;
                }
            }
            var po = new ParallelOptions {MaxDegreeOfParallelism = Environment.ProcessorCount - 1};
            Parallel.For(0, GraphNodes.Count - 1, po, i => Parallel.For(i + 1, GraphNodes.Count, po, j =>
            {
                if (GraphWays[i, j].status == WayStatus.NotCalculated)
                    if (GraphNodes[i].p == GraphNodes[j].p)
                    {
                        GraphWays[i, j].status = WayStatus.Zero;
                        GraphWays[j, i].status = WayStatus.Zero;
                    }
                    else
                    {
                        GraphWays[i, j].way = WaveSearch(GraphNodes[i].p, GraphNodes[j].p);
                        GraphWays[j, i].way = GraphWays[i, j].way;
                        if (GraphWays[i, j].way != null)
                            GraphWays[i, j].status = GraphWays[j, i].status = WayStatus.Exists;
                        else
                            GraphWays[i, j].status = GraphWays[j, i].status = WayStatus.Infinity;
                    }
            }));

            // расставляем группы связности
            for (int i = 0; i < GraphNodes.Count - 1; i++)
                for (int j = i + 1; j < GraphNodes.Count; j++)
                {
                    if (GraphWays[i, j].status == WayStatus.Exists)
                        GraphNodes[j].Group = GraphNodes[i].Group;
                }

            for (int i = 0; i <= GraphNodes.OrderByDescending(gn => gn.Group).First().Group; i++)
                MinimumSpanningTree(i);

            foreach (var graphWay in GraphWays)
            {
                if (graphWay.status == WayStatus.Selected && graphWay.way != null)
                {
                    try
                    {
                        foreach (var point in graphWay.way)
                            Map.SetPixel(point.X, point.Y, Color.Red);
                    }
                    catch (Exception)
                    {
                    }
                }
            }
            
        }

        /// <summary>
        /// Бинаризация изображения с сохранением каждой точки карты в массив IntMap следующим образом:
        /// 0 - свободная точка;
        /// -1 - занятая (пройти нельзя).
        /// </summary>
        /// <param name="saveBinImage">Указать истину, если надо сохранить бинаризированное изображение 
        /// в файл "new.bmp", по умолчанию файл не сохраняется.</param>
        public unsafe void Binarize(bool saveBinImage = false)
        {
            int otsu = otsuThreshold();
            int width = Map.Width;
            int height = Map.Height;
            var ret = new int[width, height];
            BitmapData bd = Map.LockBits(new Rectangle(0, 0, width, height), ImageLockMode.ReadOnly,
                PixelFormat.Format24bppRgb);
            Bitmap output = new Bitmap(width, height, PixelFormat.Format24bppRgb);
            BitmapData outputbd = output.LockBits(new Rectangle(0, 0, width, height), ImageLockMode.WriteOnly,
                PixelFormat.Format24bppRgb);
            try
            {
                byte* curpos;
                byte* curposnew;
                for (int h = 0; h < height; h++)
                {
                    curpos = ((byte*)bd.Scan0) + h * bd.Stride;
                    curposnew = ((byte*)outputbd.Scan0) + h * bd.Stride;
                    for (int w = 0; w < width; w++)
                    {
                        int b = *(curpos++);
                        int g = *(curpos++);
                        int r = *(curpos++);

                        if ((r + b + g) / 3 < otsu && WaterOrGround || !WaterOrGround && r*r + g*g < b*b)
                        {
                            *(curposnew++) = 0;
                            *(curposnew++) = 0;
                            *(curposnew++) = 0;
                            ret[w, h] = -1;
                        }
                        else
                        {
                            *(curposnew++) = 255;
                            *(curposnew++) = 255;
                            *(curposnew++) = 255;
                            ret[w, h] = 0;
                        }
                    }
                }
            }
            finally
            {
                Map.UnlockBits(bd);
                output.UnlockBits(outputbd);
            }
            if (saveBinImage)
                output.Save(@"d:\new.bmp");
            IntMap = ret;

        }
        // вспомогательные методы
        public unsafe byte[,,] BitmapToByteArray()
        {
            var binaryMapByteArray = new byte[3, Map.Height, Map.Width];
            BitmapData bd = Map.LockBits(new Rectangle(0, 0, Map.Width, Map.Height), ImageLockMode.ReadOnly,
                PixelFormat.Format24bppRgb);
            try
            {
                for (int h = 0; h < Map.Height; h++)
                {
                    byte* curpos = ((byte*)bd.Scan0) + h * bd.Stride;
                    for (int w = 0; w < Map.Width; w++)
                    {
                        binaryMapByteArray[2, h, w] = *(curpos++);
                        binaryMapByteArray[1, h, w] = *(curpos++);
                        binaryMapByteArray[0, h, w] = *(curpos++);
                    }
                }
            }
            finally
            {
                Map.UnlockBits(bd);
            }
            return binaryMapByteArray;
        }
        private int otsuThreshold()
        {
            byte[,,] binaryMapByteArray = BitmapToByteArray();
            int min = rgb(binaryMapByteArray, 0, 0), max = rgb(binaryMapByteArray, 0, 0);

            int temp, temp1;

            /**** Построение гистограммы ****/
            /* Узнаем наибольший и наименьший полутон */
            for (int i = 0; i < Map.Width; i++)
            {
                for (int j = 0; j < Map.Height; j++)
                {
                    temp = rgb(binaryMapByteArray, i, j);
                    if (temp < min) min = temp;
                    if (temp > max) max = temp;
                }
            }

            int[] hist = new int[max - min + 1];

            for (int i = 0; i < hist.Length; i++)
                hist[i] = 0;

            /* Считаем сколько каких полутонов */
            for (int i = 0; i < Map.Width; i++)
                for (int j = 0; j < Map.Height; j++)
                    hist[rgb(binaryMapByteArray, i, j) - min]++;

            // Гистограмма построена

            int alpha, beta, threshold = 0;
            temp = temp1 = 0;
            alpha = beta = 0;
            /* Для расчета математического ожидания первого класса */
            for (int i = 0; i <= (max - min); i++)
            {
                temp += i * hist[i];
                temp1 += hist[i];
            }

            double sigma, maxSigma = -1;
            double w1, a;
            /* Основной цикл поиска порога
            Пробегаемся по всем полутонам для поиска такого, при котором внутриклассовая дисперсия минимальна */
            for (int i = 0; i < (max - min); i++)
            {
                alpha += i * hist[i];
                beta += hist[i];

                w1 = (double)beta / temp1;
                a = (double)alpha / beta - (double)(temp - alpha) / (temp1 - beta);
                sigma = w1 * (1 - w1) * a * a;

                if (sigma > maxSigma)
                {
                    maxSigma = sigma;
                    threshold = i;
                }
            }
            return threshold + min;
        }
        private int rgb(byte[, ,] mass, int j, int i)
        {
            return (mass[0, i, j] + mass[1, i, j] + mass[2, i, j]) / 3;
        }
    }
}
