public class Edge {
    public int v1;
    public int v2;
    public int weight;

    public Edge(int _v1, int _v2, int _weight) {
        v1 = _v1;
        v2 = _v2;
        weight = _weight;
    }

    public void printEdgeInfo() {
//        System.out.print("Edge: " + v1 + "->" + v2 + " weight = " + weight );
        System.out.print("(" + v1 + "," + v2 + ") : " + weight);
    }
}
