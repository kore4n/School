import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class asn3 {
    // Indices represent vertex.
    static boolean[] vertices_in_MST;
    static ArrayList<Edge> edges_in_MST;

    // Vertex number is the index, index 0 is unused
    // Indices 1-numberOfVertices are used.
    static ArrayList<ArrayList<Edge>> adjacencyList;

    // All max value by default
    // pi[u] = vertex parent of u
    static int[] pi;

    // keyEdges[u] = Minimum weight of an edge to a vertex in the MST to another vertex outside
    static int[] keyVertices;
    static Heap minHeapVertices;
    static int numberOfVertices;

    static Heap test;
    static int[] keyTestVertices;

    private static void generateMST(String inputFile) {
        try {
            setupHeapAndAdjacencyList(inputFile);
            printAdjacencyList();
            computeMST();
            printMST();
//            test();
        } catch (Exception e) {
            System.out.println(e);
        }
    }

    private static void test() {
        int n = 7;
        keyTestVertices = new int[n];

        keyTestVertices[1] = 26;
        keyTestVertices[2] = 20;
        keyTestVertices[3] = 43;
        keyTestVertices[4] = 11;
        keyTestVertices[5] = 7;
        keyTestVertices[6] = 5;
//        keyTestVertices[7] = 6;
//        keyTestVertices[8] = 7;

        System.out.println("key test vertices");
//        System.out.println(Arrays.toString(keyTestVertices));

        test = new Heap(keyTestVertices, n - 1);
        test.printHeap();

//        for (int i = 0; i < n - 1; i++) {
//            System.out.println("Deleting min");
//            System.out.println(test.min_key());
//            test.delete_min();
//            test.printHeap();
//        }
//        test.decrease_key(4, 1);
//        test.printHeap();

        System.out.println("6 in heap: " + test.in_heap(6));

        test.delete_min();
        test.printHeap();
        System.out.println("6 in heap: " + test.in_heap(6));
        System.out.println("2 in heap: " + test.in_heap(2));
        System.out.println("3 in heap: " + test.in_heap(3));
        System.out.println("4 in heap: " + test.in_heap(4));

//        while (!test.is_empty()) {
//            System.out.println("Deleting min");
//            System.out.println(test.min_key());
//            test.delete_min();
////            test.printHeap();
//        }
    }

    private static void setupHeapAndAdjacencyList(String inputFile) throws FileNotFoundException {
        File graph = new File(inputFile);

        Scanner myReader = new Scanner(graph);

        // Skip number of vertices line
        numberOfVertices = Integer.parseInt(myReader.nextLine());
        keyVertices = new int[numberOfVertices + 1];
        pi = new int[numberOfVertices + 1];
        vertices_in_MST = new boolean[numberOfVertices + 1];
        edges_in_MST = new ArrayList<>(numberOfVertices - 1);
        adjacencyList = new ArrayList<>(numberOfVertices + 1);
        for (int i = 0; i < numberOfVertices + 1; i++) {
            adjacencyList.add(new ArrayList<Edge>());
        }

        for (int i = 1; i < pi.length; i++) {
            keyVertices[i] = Integer.MAX_VALUE;
            pi[i] = -1;
        }

        // Vertex 1 is always root
        final int root = 1;
        keyVertices[root] = 0;
        pi[root] = 0;

        minHeapVertices = new Heap(keyVertices, numberOfVertices);

        while (myReader.hasNext()) {
            String currentLine = myReader.nextLine().trim();
            String[] words = currentLine.split("\\s+");
            int v1 = Integer.parseInt(words[0]);
            int v2 = Integer.parseInt(words[1]);
            int edgeWeight = Integer.parseInt(words[2]);
            Edge currentEdgeV1 = new Edge(v1, v2, edgeWeight);
            Edge currentEdgeV2 = new Edge(v2, v1, edgeWeight);

//            System.out.println(currentLine);
            adjacencyList.get(v1).add(currentEdgeV1);
            adjacencyList.get(v2).add(currentEdgeV2);
        }
    }

    private static void computeMST() {
        // Minheap edges should have 1 vertex in A[1], the first vertex
//        minHeapVertices.printHeap();

        while (!minHeapVertices.is_empty()) {

            int minVertexNumber = minHeapVertices.min_id();
            int minWeight = minHeapVertices.min_key();

            minHeapVertices.delete_min();

            Edge edgeToAdd = new Edge(pi[minVertexNumber], minVertexNumber, minWeight);
            edges_in_MST.add(edgeToAdd);
//            edgeToAdd.printEdgeInfo();

            // Update edges adjacent to current vertex.
            for (int i = 0; i < adjacencyList.get(minVertexNumber).size(); i++) {
                Edge adjacentEdge = adjacencyList.get(minVertexNumber).get(i);

                int adjacentVertex = adjacentEdge.v2;
                if (minHeapVertices.in_heap(adjacentVertex) && adjacentEdge.weight < keyVertices[adjacentVertex]) {
                    // Set parent of adjacent vertex to current vertex
                    pi[adjacentVertex] = minVertexNumber;

                    // Relax weight to adjacent vertex
                    minHeapVertices.decrease_key(adjacentVertex, adjacentEdge.weight);
                }
            }
        }
    }

    // Adjacency List
    private static void printAdjacencyList() {
        System.out.println("Adjacency List");
        for (int i = 1; i < adjacencyList.size(); i++) {
            System.out.print("Edges adjacent to vertex " + i + ": ");

            ArrayList<Edge> currentVertexList = adjacencyList.get(i);
            for (Edge edge : currentVertexList) {
                edge.printEdgeInfo();
                System.out.print(" | ");
            }
            System.out.println();
        }
    }

    private static void printMST() {
        System.out.println("Printing MST");
        System.out.println("Number of edges in MST: " + (edges_in_MST.size() - 1));
        int edgeNumber = 1;
        int finalWeight = 0;
        for (int i = 1; i < edges_in_MST.size(); i++) {
            Edge edge = edges_in_MST.get(i);
            System.out.print(edgeNumber + ": ");
            edge.printEdgeInfo();
            System.out.println();
            finalWeight += edge.weight;
            edgeNumber++;
        }
        System.out.println();
        System.out.println("Final weight of the MST: " + finalWeight);
    }

    public static void main(String[] args) {
        String inputFile = args[0];
        generateMST(inputFile);
    }
}
