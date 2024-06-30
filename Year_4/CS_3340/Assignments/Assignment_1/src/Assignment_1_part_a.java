// Jason Kwan
// 251173792

public class Assignment_1_part_a {
    static long computeNthLucasNumber(long n) {
        if (n == 0) {
            return 2;
        } else if (n == 1) {
            return 1;
        }

        return computeNthLucasNumber(n - 1) + computeNthLucasNumber(n - 2);
    }

    static void partA() {
        for (int i = 0; i <= 10; i++) {
            int nthNumber = i * 5;

            long number = computeNthLucasNumber(nthNumber);

            System.out.println(nthNumber + "th lukas number: " + number);
        }
    }

    public static void main(String[] args) {
        // 0 <= i <= 10
        partA();
    }
}
