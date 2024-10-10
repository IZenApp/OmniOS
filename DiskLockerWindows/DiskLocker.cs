using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Windows.Forms;

public class DiskLocker : Form
{
    private ComboBox comboBox;
    
    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern IntPtr CreateFile(string lpFileName, uint dwDesiredAccess, uint dwShareMode,
        IntPtr lpSecurityAttributes, uint dwCreationDisposition, uint dwFlagsAndAttributes, IntPtr hTemplateFile);

    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern bool DeviceIoControl(IntPtr hDevice, uint dwIoControlCode, IntPtr lpInBuffer,
        uint nInBufferSize, IntPtr lpOutBuffer, uint nOutBufferSize, out uint lpBytesReturned, IntPtr lpOverlapped);

    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern bool CloseHandle(IntPtr hObject);

    [DllImport("kernel32.dll")]
    private static extern uint GetLogicalDrives();

    private const uint FSCTL_LOCK_VOLUME = 0x00090018;
    private const uint FSCTL_UNLOCK_VOLUME = 0x0009001C;

    public DiskLocker()
    {
        this.Text = "Disk Locker";
        this.Size = new System.Drawing.Size(300, 200);

        comboBox = new ComboBox { DropDownStyle = ComboBoxStyle.DropDownList, Location = new System.Drawing.Point(20, 20) };
        this.Controls.Add(comboBox);
        
        Button lockButton = new Button { Text = "Lock Disk", Location = new System.Drawing.Point(20, 60) };
        lockButton.Click += (s, e) => LockUnlockDisk(comboBox.SelectedItem.ToString()[0], true);
        this.Controls.Add(lockButton);

        Button unlockButton = new Button { Text = "Unlock Disk", Location = new System.Drawing.Point(130, 60) };
        unlockButton.Click += (s, e) => LockUnlockDisk(comboBox.SelectedItem.ToString()[0], false);
        this.Controls.Add(unlockButton);

        Button lockAllButton = new Button { Text = "Lock All Disks", Location = new System.Drawing.Point(20, 100) };
        lockAllButton.Click += (s, e) => LockUnlockAllDisks(true);
        this.Controls.Add(lockAllButton);

        Button unlockAllButton = new Button { Text = "Unlock All Disks", Location = new System.Drawing.Point(130, 100) };
        unlockAllButton.Click += (s, e) => LockUnlockAllDisks(false);
        this.Controls.Add(unlockAllButton);

        DisplayDisks();
    }

    private void LockUnlockDisk(char driveLetter, bool lockDisk)
    {
        string drivePath = @"\\.\" + driveLetter + ":";
        IntPtr hDrive = CreateFile(drivePath, 0x80000000 | 0x40000000, 0x00000001 | 0x00000002, IntPtr.Zero,
            3, 0, IntPtr.Zero);

        if (hDrive == (IntPtr)(-1))
        {
            MessageBox.Show($"Error: Unable to open drive {driveLetter}");
            return;
        }

        uint bytesReturned;
        bool result;
        if (lockDisk)
        {
            result = DeviceIoControl(hDrive, FSCTL_LOCK_VOLUME, IntPtr.Zero, 0, IntPtr.Zero, 0, out bytesReturned, IntPtr.Zero);
        }
        else
        {
            result = DeviceIoControl(hDrive, FSCTL_UNLOCK_VOLUME, IntPtr.Zero, 0, IntPtr.Zero, 0, out bytesReturned, IntPtr.Zero);
        }

        MessageBox.Show(result
            ? $"Drive {driveLetter} {(lockDisk ? "locked" : "unlocked")} successfully."
            : $"Error: Unable to {(lockDisk ? "lock" : "unlock")} drive {driveLetter}");

        CloseHandle(hDrive);
    }

    private void LockUnlockAllDisks(bool lockDisk)
    {
        uint drives = GetLogicalDrives();
        for (int i = 0; i < 26; i++)
        {
            if ((drives & (1 << i)) != 0)
            {
                LockUnlockDisk((char)('A' + i), lockDisk);
            }
        }
    }

    private void DisplayDisks()
    {
        uint drives = GetLogicalDrives();
        for (int i = 0; i < 26; i++)
        {
            if ((drives & (1 << i)) != 0)
            {
                comboBox.Items.Add((char)('A' + i) + ":");
            }
        }

        if (comboBox.Items.Count > 0)
        {
            comboBox.SelectedIndex = 0; // Select the first disk by default
        }
    }

    [STAThread]
    public static void Main()
    {
        Application.EnableVisualStyles();
        Application.Run(new DiskLocker());
    }
}
