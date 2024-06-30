// Made by: Jason Kwan
// Student Number: 251173792

public class uandf {

    private final int[] parent;
    private final int[] finalRepresentatives;
    private final int[] rank;

    private int no_sets = 0;
    private boolean isFinalized = false;

    // (constructor) constructs a disjoint-set data type with n elements, 1, 2, . . . , n.
    public uandf(int n) {
        parent = new int[n];
        rank = new int[n];
        finalRepresentatives = new int[n];
    }

    // Creates a new set whose only member (and thus representative) is i
    public void make_set(int i) {
        if (isFinalized) return;

        parent[i] = i;
        rank[i] = 0;
    }

    // Returns the representative of the set containing i
    // NOTE: i MUST be in the set
    public int find_set(int i) {
        if (isFinalized) {
            int parentOfI = find_set_final(i);

            // Returns 1 to numOfSets
            return finalRepresentatives[parentOfI];
        }

        if (parent[i] != i) {
            // Path compression
            parent[i] = find_set(parent[i]);
        }

        return parent[i];
    }

    public int find_set_final(int i) {
        if (parent[i] != i) {
            // Path compression
            return find_set_final(parent[i]);
        }

        return parent[i];
    }

    // Unites the dynamic sets that contains i and j, respectively, into a
    // new set that is the union of these two sets.
    public void union_sets(int i, int j) {
        if (isFinalized) return;

        link_sets(find_set(i), find_set(j));
    }

    private void link_sets(int i, int j) {
        if (rank[i] > rank[j]) {
            parent[j] = i;
        } else if (rank[i] < rank[j]) {
            parent[i] = j;
        } else if (i != j) {
            parent[j] = i;
            rank[i] = rank[i] + 1;
        }
    }

    // Returns the total number of current sets, no_sets, and finalizes the
    // current sets: (i) make_set() and union_sets() will have no effect after this operation
    // and (ii) resets the representatives of the sets so that integers from 1 to no sets will
    // be used as representatives.
    public int final_sets() {
        if (isFinalized) return no_sets;

        // Step 1: Perform path compression on all nodes.
        for (int i = 1; i < parent.length; i++) {
            if (parent[i] != 0) {
                find_set(i);
            }
        }

        // Step 2: Setup final representatives (1 to no_sets)
        int currentRepresentative = 1;
        for (int i = 1; i < parent.length; i++) {
            // Don't care about empty nodes
            if (parent[i] == 0) continue;

            // This represents a plus sign. Find its parent
            int actualParent = find_set(parent[i]);
            if (finalRepresentatives[actualParent] == 0) {
                finalRepresentatives[actualParent] = currentRepresentative;
                currentRepresentative++;
                no_sets++;
            }
        }


        isFinalized = true;
        return no_sets;
    }
}
