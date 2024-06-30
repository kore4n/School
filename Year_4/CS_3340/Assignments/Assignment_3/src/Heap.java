import java.util.Arrays;

public class Heap {
    // Array of keys
    private final int[] A;

    // Array of IDs (indices of keys)
    private final int[] H;

    // Number of keys
    private int n;

    private int mins_left = 0;

    public Heap(int[] keys, int n) {
        A = keys;
        H = new int[2 * n];
        this.n = n;

        mins_left = n;

        // Heapification
        // Setup IDs
        for (int i = 1; i <= n; i++) {
            H[i + n - 1] = i;
        }

//        System.out.println(Arrays.toString(H));

        for (int i = n - 1; i >= 1; i--) {
            if (A[H[2 * i]] < A[H[2 * i + 1]]) {
                H[i] = H[2 * i];
            } else {
                H[i] = H[2 * i + 1];
            }
        }
//        System.out.println(Arrays.toString(H));
    }

    // Returns true if the element with id is in the heap
    // ID is the index of the element
    public boolean in_heap(int id) {
        return H[id + n - 1] != 1;
    }

    // Returns true if heap is empty
    public boolean is_empty() {
//        return H[1] == 1;
//        return mins_left == 0;

        for (int i = 1; i < H.length; i++) {
            if (H[i] != 1) {
                return false;
            }
        }
        return true;
    }

    // Returns the minimum key of the heap
    public int min_key() {
//        if (mins_left == 1) {
//            for (int i = 1; i < H.length; i++) {
//                if (H[i] != 1) {
//
//                    return A[H[i]];
//                }
//            }
//        }

        return A[H[1]];
    }

    // Returns the id of the element with minimum key in the heap
    public int min_id() {
        return H[1];
    }

    // Returns the key of the element with id in the heap
    public int key(int id) {
        return A[id];
    }

    // Deletes the element with minimum key from the heap
    public void delete_min() {
        mins_left -= 1;

        A[H[1]] = Integer.MAX_VALUE;
        H[H[1] + n - 1] = 1;

//        printA();
//        printH();
        heapify();
    }

    // Sets the key of the element with id to new key if its current
    // key is greater than new key
    // Just reheapify everything
    public void decrease_key(int id, int new_key) {
        if (A[id] > new_key) {
            A[id] = new_key;
        }

        heapify();
    }

    private void heapify() {
//        int i = (H[1] + n - 1) / 2;
////        int i = n - 1;
//
//        System.out.println("value of i: " + i);
////        System.out.println(Arrays.toString(H));
//
//        while (i >= 1) {
//            if (A[H[2 * i]] < A[H[2 * i + 1]]) {
//                H[i] = H[2 * i];
//            }
//            else {
//                H[i] = H[2 * i + 1];
//            }
//            i = i / 2;
//        }

        for (int i = n - 1; i >= 1; i--) {
            if (A[H[2 * i]] < A[H[2 * i + 1]]) {
                H[i] = H[2 * i];
            } else {
                H[i] = H[2 * i + 1];
            }
        }
    }

    public void printA() {
        System.out.println("A: " + Arrays.toString(A));
    }

    public void printH() {
        System.out.println("H: " + Arrays.toString(H));
    }

    public void printHeap() {
        printA();
        printH();
    }
}
