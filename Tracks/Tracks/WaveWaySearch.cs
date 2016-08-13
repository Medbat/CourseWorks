using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tracks
{
    /*
    public class WayNode
    {
        // координаты точки
        public Point p;
        // используется для восстановления пути
        public WayNode cameFrom;
        // параметры точки
        public int h, // эвристическая оценка до цели
            g, // стоимость пути от начальной вершины
            f; // сумма

        public WayNode(Point p, WayNode cameFrom, int h, int g, int f)
        {
            this.p = p;
            this.cameFrom = cameFrom;
            this.h = h;
            this.g = g;
            this.f = f;
        }
    }

    class WaveWaySearch
    {
        // мой поиск пути A*
        // поиск пути
        [ObsoleteAttribute("Работает очень медленно, используйте WaveSearch")]
        public Point[] FindWay(Point start, Point goal)
        {
            List<WayNode> closedPoints = new List<WayNode>();
            WayNode startNode = new WayNode(start, null, HeuristicCostEstimate(start, goal), 0, HeuristicCostEstimate(start, goal));
            List<WayNode> openedPoints = new List<WayNode> { startNode };
            WayNode goalNode = new WayNode(goal, null, 0, 0, 0);
            while (openedPoints.Count != 0)
            {
                WayNode x = openedPoints.OrderBy(node => node.f).First();
                //Console.WriteLine(x.p.X + " " + x.p.Y);
                if (x.p == goalNode.p)
                {
                    //Console.WriteLine(closedPoints.Count);
                    return ReconstructPath(goalNode).ToArray();
                }
                openedPoints.Remove(x);
                closedPoints.Add(x);
                foreach (var neightbourNode in NeightbourNodes(x, goal))
                {
                    bool flag = false;
                    foreach (var closedPoint in closedPoints)
                    {
                        if (closedPoint.p == neightbourNode.p)
                        {
                            flag = true;
                            break;
                        }
                    }
                    if (flag)//(closedPoints.Count(n => n.p == neightbourNode.p) > 0)
                        continue;
                    int score = x.g + 1;
                    bool scoreIsBetter = false;

                    flag = true;
                    foreach (var openedPoint in openedPoints)
                    {
                        if (openedPoint.p == neightbourNode.p)
                            flag = false;
                    }
                    if (flag)//(openedPoints.Count(n => n.p == neightbourNode.p) == 0)
                    {
                        openedPoints.Add(neightbourNode);
                        scoreIsBetter = true;
                    }
                    else if (score < neightbourNode.g)
                        scoreIsBetter = true;
                    if (scoreIsBetter)
                    {
                        neightbourNode.cameFrom = x;
                        if (neightbourNode.p == goalNode.p)
                        {
                            //Console.WriteLine(closedPoints.Count);

                            goalNode.cameFrom = x;
                            return ReconstructPath(goalNode).ToArray();
                        }
                        neightbourNode.g = score;
                        neightbourNode.h = HeuristicCostEstimate(neightbourNode.p, goal);
                        neightbourNode.f = neightbourNode.g + neightbourNode.h;
                    }
                }
            }
            return null;
        }
        // вспомогательные методы
        private int HeuristicCostEstimate(Point start, Point goal)
        {
            return Math.Abs(start.X - goal.X) + Math.Abs(start.Y - goal.Y);
        }
        private WayNode[] NeightbourNodes(WayNode wn, Point goal)
        {
            Point[] neightbourNodes = 
            {
                new Point(wn.p.X - 1, wn.p.Y), 
                new Point(wn.p.X + 1, wn.p.Y), 
                new Point(wn.p.X, wn.p.Y - 1),
                new Point(wn.p.X, wn.p.Y + 1) 
            };
            List<WayNode> ret = new List<WayNode>();

            foreach (var neightbourNode in neightbourNodes)
                if (!(neightbourNode.X < 0 || neightbourNode.X >= IntMap.GetLength(0) ||
                      neightbourNode.Y < 0 || neightbourNode.Y >= IntMap.GetLength(1)))
                    if (IntMap[neightbourNode.X, neightbourNode.Y] != -1)
                        ret.Add(new WayNode(neightbourNode, wn,
                            HeuristicCostEstimate(neightbourNode, goal),
                            wn.g + 1,
                            HeuristicCostEstimate(neightbourNode, goal) + wn.g + 1));

            return ret.ToArray();
        }
        private IEnumerable<Point> ReconstructPath(WayNode goal)
        {
            WayNode currentNode = goal;
            while (currentNode != null)
            {
                yield return currentNode.p;
                currentNode = currentNode.cameFrom;
            }
        }
    }
    */
}
