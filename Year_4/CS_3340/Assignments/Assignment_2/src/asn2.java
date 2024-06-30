// Made by: Jason Kwan
// Student Number: 251173792


import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class asn2 {
    static final int numLines = 72;
    static final int widthPerLine = 75;
    public static boolean[][] pluses = new boolean[numLines][widthPerLine];

    private static void setupArray(String fileName) {
        try {
            File myFile = new File(fileName);
            Scanner myReader = new Scanner(myFile);
            int currentLine = 0;
            while (myReader.hasNextLine()) {
                String currentLineData = myReader.nextLine();

                for (int i = 0; i < currentLineData.length(); i++) {
                    if (currentLineData.charAt(i) != '+') continue;

                    pluses[currentLine][i] = true;
                }
                currentLine += 1;
            }
            myReader.close();
        } catch (FileNotFoundException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
    }

    public static void printInputBinaryImage() {
        System.out.println("Displaying part 1.");

        for (int i = 0; i < pluses.length; i++) {
            for (int k = 0; k < pluses[i].length; k++) {
                boolean plus = pluses[i][k];

                if (plus) {
                    System.out.print("+");

                } else {
                    System.out.print(" ");
                }
            }
            System.out.println();
        }
    }

    private static void printOutSpecial(uandf mySet) {
        mySet.final_sets();

        // Print special characters
        for (int row = 0; row < pluses.length; row++) {
            for (int column = 0; column < pluses[row].length; column++) {
                if (!pluses[row][column]) {
                    System.out.print(" ");
                    continue;
                }

                // Find all where there are pluses
                int currentPlusLocation = row * widthPerLine + column + 1;
                int representative = mySet.find_set(currentPlusLocation);
                char charToRepresent = (char) (representative + 96);
                System.out.print(charToRepresent);
            }

            System.out.println();
        }
    }

    private static void partTwo(uandf mySet) {
        System.out.println("Displaying part 2.");

        for (int row = 0; row < pluses.length; row++) {
            for (int column = 0; column < pluses[row].length; column++) {
                // Skip to next spot if current spot has no plus
                if (!pluses[row][column]) continue;

                // Add 1 to keep numbers between 1 ... 5400
                int currentRepresentative = row * widthPerLine + column + 1;
                // Always make a new component for current spot
                mySet.make_set(currentRepresentative);

                // Check all 8 neighbours
                // If component is a neighbour, make that a component and union the two

                // check row above
                if (row > 0 && pluses[row - 1][column]) {
                    mySet.union_sets(currentRepresentative, (row - 1) * widthPerLine + column + 1);
                }

                // check top left
                if (row > 0 && column > 0 && pluses[row - 1][column - 1]) {
                    mySet.union_sets(currentRepresentative, (row - 1) * widthPerLine + (column - 1) + 1);
                }

                // check column to the left
                if (column > 0 && pluses[row][column - 1]) {
                    mySet.union_sets(currentRepresentative, row * widthPerLine + column);
                }

                // check top right
                if (column < widthPerLine - 1 && row > 0 && pluses[row - 1][column + 1]) {
                    mySet.union_sets(currentRepresentative, (row - 1) * widthPerLine + column + 2);
                }
            }
        }

        printOutSpecial(mySet);
    }

    private static void partThree(uandf mySet) {
        System.out.println("Displaying part 3.");

        final int numOfSets = mySet.final_sets();
        final int[] myList = new int[numOfSets + 1];

        for (int row = 0; row < pluses.length; row++) {
            for (int column = 0; column < pluses[row].length; column++) {
                if (!pluses[row][column]) {
                    continue;
                }

                // Find all where there are pluses
                int currentPlusLocation = row * widthPerLine + column + 1;
                // An integer between 1 to numOfSets
                int representative = mySet.find_set(currentPlusLocation);
                myList[representative]++;
            }
        }

        System.out.println("Component list sorted by size.");
        // 2d array of component sizes and their character as an int
        // 1st. Component size
        // 2nd. List character
        int[][] components = new int[myList.length][2];

        // Marked status of component
        boolean[] componentMarked = new boolean[myList.length];

        for (int i = 0; i < myList.length; i++) {
            components[i][0] = myList[i];
            components[i][1] = i;
        }

        int lastLargestComponentSize = -1;
        for (int i = 1; i < components.length; i++) {
            int largestComponentCharacter = -1;

            for (int k = 0; k < components.length; k++) {
                int componentSize = components[k][0];
                int componentCharacter = components[k][1];

                if (componentSize > lastLargestComponentSize && !componentMarked[componentCharacter]) {
                    lastLargestComponentSize = componentSize;
                    largestComponentCharacter = componentCharacter;
                }
            }

            char charToRepresent = (char) (largestComponentCharacter + 96);

            System.out.print(charToRepresent + ": ");
            System.out.println(lastLargestComponentSize);

            componentMarked[largestComponentCharacter] = true;
            lastLargestComponentSize = -1;
        }
    }


    private static void partFour(uandf mySet) {
        System.out.println("Displaying part 4.");

        final int numOfSets = mySet.final_sets();
        final int[] myList = new int[numOfSets + 1];

        for (int row = 0; row < pluses.length; row++) {
            for (int column = 0; column < pluses[row].length; column++) {
                if (!pluses[row][column]) {
                    continue;
                }

                // Find all where there are pluses
                int currentPlusLocation = row * widthPerLine + column + 1;
                // An integer between 1 to numOfSets
                int representative = mySet.find_set(currentPlusLocation);
                myList[representative]++;
            }
        }

        for (int row = 0; row < pluses.length; row++) {
            for (int column = 0; column < pluses[row].length; column++) {
                if (!pluses[row][column]) {
                    System.out.print(" ");
                    continue;
                }

                // Find all where there are pluses
                int currentPlusLocation = row * widthPerLine + column + 1;
                // An integer between 1 to numOfSets
                int representative = mySet.find_set(currentPlusLocation);
                if (myList[representative] > 2) {
                    char charToRepresent = (char) (representative + 96);
                    System.out.print(charToRepresent);
                } else {
                    System.out.print(" ");
                }
            }

            System.out.println();
        }
    }

    private static void partFive(uandf mySet) {
        System.out.println("Displaying part 5.");

        final int numOfSets = mySet.final_sets();
        final int[] myList = new int[numOfSets + 1];

        for (int row = 0; row < pluses.length; row++) {
            for (int column = 0; column < pluses[row].length; column++) {
                if (!pluses[row][column]) {
                    continue;
                }

                // Find all where there are pluses
                int currentPlusLocation = row * widthPerLine + column + 1;
                // An integer between 1 to numOfSets
                int representative = mySet.find_set(currentPlusLocation);
                myList[representative]++;
            }
        }

        for (int row = 0; row < pluses.length; row++) {
            for (int column = 0; column < pluses[row].length; column++) {
                if (!pluses[row][column]) {
                    System.out.print(" ");
                    continue;
                }

                // Find all where there are pluses
                int currentPlusLocation = row * widthPerLine + column + 1;
                // An integer between 1 to numOfSets
                int representative = mySet.find_set(currentPlusLocation);
                if (myList[representative] > 11) {
                    char charToRepresent = (char) (representative + 96);
                    System.out.print(charToRepresent);
                } else {
                    System.out.print(" ");
                }
            }

            System.out.println();
        }
    }
    public static void main(String[] args) {

        setupArray(args[0]);

        printInputBinaryImage();
        uandf mySet = new uandf(numLines * widthPerLine + 1);
        partTwo(mySet);
        partThree(mySet);
        partFour(mySet);
        partFive(mySet);
    }
}
