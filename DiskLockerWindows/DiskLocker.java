import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class DiskLocker {

    public static void lockUnlockDisk(char driveLetter, boolean lock) {
        String drivePath = driveLetter + ":\\";
        File disk = new File(drivePath);

        if (!disk.exists()) {
            System.out.println("Error: Unable to access drive " + driveLetter);
            return;
        }

        // Здесь нужно добавить код для блокировки/разблокировки диска,
        // используя соответствующие Java библиотеки или нативные вызовы.
        // В данном примере просто выводим сообщение.
        if (lock) {
            System.out.println("Drive " + driveLetter + " locked successfully.");
        } else {
            System.out.println("Drive " + driveLetter + " unlocked successfully.");
        }
    }

    public static List<Character> displayDisks() {
        List<Character> disks = new ArrayList<>();
        for (char drive = 'A'; drive <= 'Z'; drive++) {
            String drivePath = drive + ":\\";
            if (new File(drivePath).exists()) {
                disks.add(drive);
            }
        }
        return disks;
    }

    public static void main(String[] args) {
        List<Character> disks = displayDisks();
        Scanner scanner = new Scanner(System.in);

        while (true) {
            System.out.println("Available drives: " + disks);
            System.out.println("Enter drive letter to lock/unlock (or 'exit' to quit): ");
            String input = scanner.nextLine();

            if (input.equalsIgnoreCase("exit")) {
                break;
            }

            if (input.length() == 1 && disks.contains(input.charAt(0))) {
                System.out.println("Type 'lock' to lock or 'unlock' to unlock: ");
                String action = scanner.nextLine();

                if (action.equalsIgnoreCase("lock")) {
                    lockUnlockDisk(input.charAt(0), true);
                } else if (action.equalsIgnoreCase("unlock")) {
                    lockUnlockDisk(input.charAt(0), false);
                } else {
                    System.out.println("Invalid action.");
                }
            } else {
                System.out.println("Invalid drive letter.");
            }
        }

        scanner.close();
    }
}
