using System;
using System.Runtime.InteropServices;

class DiskLocker
{
    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern bool DeviceIoControl(
        IntPtr hDevice,
        uint dwIoControlCode,
        IntPtr lpInBuffer,
        uint nInBufferSize,
        IntPtr lpOutBuffer,
        uint nOutBufferSize,
        out uint lpBytesReturned,
        IntPtr lpOverlapped);

    private const uint FSCTL_LOCK_VOLUME = 0x00090018;
    private const uint FSCTL_UNLOCK_VOLUME = 0x0009001C;

    public static void LockVolume(string driveLetter)
    {
        IntPtr hDevice = CreateFile(
            $"\\\\.\\{driveLetter}:",
            FileAccess.GENERIC_READ | FileAccess.GENERIC_WRITE,
            FileShare.READ | FileShare.WRITE,
            IntPtr.Zero,
            CreationDisposition.OPEN_EXISTING,
            0,
            IntPtr.Zero);

        if (hDevice.ToInt32() == -1)
        {
            Console.WriteLine("Failed to open volume.");
            return;
        }

        uint bytesReturned;
        bool result = DeviceIoControl(
            hDevice,
            FSCTL_LOCK_VOLUME,
            IntPtr.Zero,
            0,
            IntPtr.Zero,
            0,
            out bytesReturned,
            IntPtr.Zero);

        if (result)
        {
            Console.WriteLine("Volume locked successfully.");
        }
        else
        {
            Console.WriteLine("Failed to lock volume.");
        }

        CloseHandle(hDevice);
    }

    public static void UnlockVolume(string driveLetter)
    {
        IntPtr hDevice = CreateFile(
            $"\\\\.\\{driveLetter}:",
            FileAccess.GENERIC_READ | FileAccess.GENERIC_WRITE,
            FileShare.READ | FileShare.WRITE,
            IntPtr.Zero,
            CreationDisposition.OPEN_EXISTING,
            0,
            IntPtr.Zero);

        if (hDevice.ToInt32() == -1)
        {
            Console.WriteLine("Failed to open volume.");
            return;
        }

        uint bytesReturned;
        bool result = DeviceIoControl(
            hDevice,
            FSCTL_UNLOCK_VOLUME,
            IntPtr.Zero,
            0,
            IntPtr.Zero,
            0,
            out bytesReturned,
            IntPtr.Zero);

        if (result)
        {
            Console.WriteLine("Volume unlocked successfully.");
        }
        else
        {
            Console.WriteLine("Failed to unlock volume.");
        }

        CloseHandle(hDevice);
    }

    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern IntPtr CreateFile(
        string lpFileName,
        FileAccess dwDesiredAccess,
        FileShare dwShareMode,
        IntPtr lpSecurityAttributes,
        CreationDisposition dwCreationDisposition,
        int dwFlagsAndAttributes,
        IntPtr hTemplateFile);

    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern bool CloseHandle(IntPtr hObject);

    [Flags]
    private enum FileAccess : uint
    {
        GENERIC_READ = 0x80000000,
        GENERIC_WRITE = 0x40000000
    }

    [Flags]
    private enum FileShare : uint
    {
        READ = 0x00000001,
        WRITE = 0x00000002
    }

    private enum CreationDisposition : uint
    {
        OPEN_EXISTING = 3
    }

    static void Main(string[] args)
    {
        if (args.Length != 2)
        {
            Console.WriteLine("Usage: DiskLocker <lock|unlock> <driveLetter>");
            return;
        }

        string action = args[0];
        string driveLetter = args[1];

        if (action == "lock")
        {
            LockVolume(driveLetter);
        }
        else if (action == "unlock")
        {
            UnlockVolume(driveLetter);
        }
        else
        {
            Console.WriteLine("Invalid action. Use 'lock' or 'unlock'.");
        }
    }
}