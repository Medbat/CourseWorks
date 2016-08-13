using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GUI
{
    class WaveWaySearchNotMine
    {
        public Point[] Wave(Point start, Point finish, int[,] IntMap)
        {
            int[] dx = { 1, 0, -1, 0 };   // смещения, соответствующие соседям ячейки
            int[] dy = { 0, 1, 0, -1 };   // справа, снизу, слева и сверху
            int d;
            bool stop;
            int len; // длина пути
            int ii = IntMap.GetLength(0);
            int jj = IntMap.GetLength(1);
            /*int ii = 4;
            int jj = 4;
            int[,] grid = new int[,] { {-2,-2,-2, -1},{-1, -2,-1,-2} ,{-1, -2,-2,-2}, {-1,-1,-2, -2}};*/
            int[,] grid = new int[ii, jj];
            for (int i = 0; i < ii; i++)
            {
                for (int j = 0; j < jj; j++)
                {
                    if (IntMap[i, j] == 0)
                        grid[i, j] = -2;
                    else
                        grid[i, j] = -1;
                }
            }

            /*for (int i = 0; i < UPPER; i++)
            {
                
            }*/

            // распространение волны
            d = 0;
            grid[start.X, start.Y] = 0;            // стартовая ячейка помечена 0
            do
            {
                stop = true;               // предполагаем, что все свободные клетки уже помечены
                for (int x = 0; x < ii; x++)
                    for (int y = 0; y < jj; y++)
                        if (grid[x, y] == d)                         // ячейка (x, y) помечена числом d
                        {
                            for (int k = 0; k < 4; k++)                    // проходим по всем непомеченным соседям
                                if (y + dy[k] >= 0 && y + dy[k] < jj && x + dx[k] >= 0 && x + dx[k] < ii) // границы ключ
                                    if (grid[x + dx[k], y + dy[k]] == -2)//|| grid[x + dx[k], y + dy[k]] > d)
                                    {
                                        stop = false;                            // найдены непомеченные клетки
                                        grid[x + dx[k], y + dy[k]] = d + 1;      // распространяем волну
                                    }
                        }
                d++;
                /*for (int i = 0; i < ii; i++)
                {
                    for (int j = 0; j < jj; j++)
                    {
                        Console.Write(grid[i,j] +  "  ");
                    }
                    Console.WriteLine();
                }
                Console.WriteLine();*/
            } while (!stop || grid[finish.X, finish.Y] == -2);

            if (grid[finish.X, finish.Y] == -2) return null;  // путь не найден
            List<Point> lp = new List<Point>();
            // восстановление пути
            len = grid[finish.X, finish.Y];            // длина кратчайшего пути из (ax, ay) в (bx, by)
            int inx = finish.X;
            int iny = finish.Y;
            d = len;
            while (d > 0)
            {
                // записываем ячейку (x, y) в путь
                lp.Add(new Point(iny, inx));
                d--;
                for (int k = 0; k < 4; k++)
                    if (iny + dy[k] >= 0 && iny + dy[k] < jj && inx + dx[k] >= 0 && inx + dx[k] < ii)
                        if (grid[iny + dy[k], inx + dx[k]] == d)
                        {
                            inx += dx[k];
                            iny += dy[k];           // переходим в ячейку, которая на 1 ближе к старту
                            break;
                        }
            }
            lp.Add(start);
            return lp.ToArray();
        }

    }
}
