import java.lang.reflect.Array;
import java.util.Arrays;

// Jason Kwan
// 251173792

public class Assignment_1_part_b {
    static int HOW_MANY_LUCAS_NUMBERS_TO_HOLD = 501;
    static int MAX_DIGITS_PER_LUCAS_NUMBER = 120;

    static int[][] pastLucasNumbers = new int[HOW_MANY_LUCAS_NUMBERS_TO_HOLD][MAX_DIGITS_PER_LUCAS_NUMBER];

    static int[] computeNthLucasNumber(int n) {
        if (n == 0) {
            int[] num = new int[MAX_DIGITS_PER_LUCAS_NUMBER];
            num[MAX_DIGITS_PER_LUCAS_NUMBER - 1] = 2;

            return num;
        } else if (n == 1) {
            int[] num = new int[MAX_DIGITS_PER_LUCAS_NUMBER];
            num[MAX_DIGITS_PER_LUCAS_NUMBER - 1] = 1;

            return num;
        }

        if (isCalculatedYet(n)) {
            return pastLucasNumbers[n];
        }

        // Not calculated yet. Save result.
        int[] currentLucasNumber = addNumbers(computeNthLucasNumber(n - 1), computeNthLucasNumber(n - 2));

        pastLucasNumbers[n] = currentLucasNumber;

        return currentLucasNumber;
    }

    // Determines if a lucas number is calculated yet
    // Parameters: whichLucasNumber -> 0-499 inclusive
    static boolean isCalculatedYet(int whichLucasNumber) {
        for (int i = 0; i < MAX_DIGITS_PER_LUCAS_NUMBER; i++) {
            if (pastLucasNumbers[whichLucasNumber][i] != 0)  {
                return true;
            }
        }

        return false;
    }

    static int[] addNumbers(int[] num1, int[] num2) {
        int[] sum = new int[MAX_DIGITS_PER_LUCAS_NUMBER];

        boolean carryOne = false;

        for (int i = MAX_DIGITS_PER_LUCAS_NUMBER - 1; i >= 0; i--) {

            int digitSum = num1[i] + num2[i];

            if (carryOne) {
                digitSum += 1;
                carryOne = false;
            }

            int currentDigit = digitSum % 10;

            sum[i] = currentDigit;

            if (digitSum > 9) {
                // Carry the digit over
                carryOne = true;
            }
        }

        return sum;
    }

    static void printLucasNumber(int[] arr) {
        boolean hasFoundFirstNonZero = false;

        for (int i = 0; i < arr.length; i++) {
            if (!hasFoundFirstNonZero) {
                if (arr[i] != 0) {
                    hasFoundFirstNonZero = true;
                    System.out.print(arr[i]);
                }

                // Continue. Don't want to print our digits if we haven't found the first non-zero digit yet.
                continue;
            }

            System.out.print(arr[i]);
        }
    }

    static void partB() {
        for (int i = 0; i <= 25; i++) {
            int nthNumber = i * 20;

            int[] lucasNumber = computeNthLucasNumber(nthNumber);

            System.out.print(nthNumber + "th lukas number: ");
            printLucasNumber(lucasNumber);
            System.out.println();
        }
    }

    public static void main(String[] args) {
        // 0 <= i <= 25
        partB();
    }
}
