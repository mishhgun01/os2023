#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

void ListDrives() {
    
    DWORD drives = GetLogicalDrives();
    cout << "List of drives:" << endl;
    for (char i = 'A'; i <= 'Z'; ++i) {
        if (drives & 1) {
            cout << i << ":\\";
            cout << endl;
        }
        drives >>= 1;
    }
}

void DiskInfo() {
    char drive;
    cout << "Enter disk name (C, for example): ";
    cin >> drive;
    string rootPath(":\\");
    rootPath.insert(0,1,drive);
    
    DWORD serialNumber, maxComponentLength, fileSystemFlags;
    char volumeName[MAX_PATH + 1];
	char SysNameBuffer[MAX_PATH +1];
    if (GetVolumeInformation(rootPath.c_str(), volumeName, sizeof(volumeName),
                             &serialNumber, &maxComponentLength, &fileSystemFlags, SysNameBuffer, sizeof(SysNameBuffer))) {
        ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;
        if (GetDiskFreeSpaceEx(rootPath.c_str(), &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes)) {

            cout << "Drive info " << rootPath << ":" << endl;
            
            DWORD driveType = GetDriveTypeA(rootPath.c_str());
            cout << "Drive type:";
            if (driveType == DRIVE_FIXED) {
                cout << " (HDD)" << endl;
            } else if (driveType == DRIVE_CDROM) {
                cout << " (CD/DVD-ROM)" << endl;
            }

            cout << "Tom name: " << volumeName << endl;
            cout << "Serial number: " << serialNumber << endl;
            cout << "Free space: " << (freeBytesAvailable.QuadPart / (1024 * 1024)) << " MB" << endl;
        }
    }
}

void CreateDirectory() {
    string dirName;
    cout << "Enter directory name: ";
    cin >> dirName;
    
    if (CreateDirectory(dirName.c_str(), NULL)) {
        cout << "Directory " << dirName << " created." << endl;
    } else {
        cout << "Error during creation. Error code: " << GetLastError() << endl;
    }
}

void RemoveDirectory() {
    string dirName;
    cout << "Enter directory name to remove: ";
    cin >> dirName;
    
    if (RemoveDirectory(dirName.c_str())) {
        cout << "Directory " << dirName << " deleted." << endl;
    } else {
        cout << "Error during deletion. Error code:: " << GetLastError() << endl;
    }
}

void CreateNewFile() {
    string input;
    cout << "Enter new file (D:\\folder\\file.txt)" << endl;
    cin >> input;
    HANDLE fileHandle = CreateFileA(
        input.c_str(),
        GENERIC_READ | GENERIC_WRITE, // Read and write access
            1,                            // No sharing
            nullptr,                      // No security attributes
            CREATE_NEW,                   // Create a new file only if it doesn't exist
            FILE_ATTRIBUTE_NORMAL,        // Normal file attributes
            nullptr
    );

    if (fileHandle == INVALID_HANDLE_VALUE) {
        cout << "Error during creating file. Error code: " << GetLastError() << endl;
    }

    cout << "File created" << endl;
    CloseHandle(fileHandle);
}

void CopyFiles() {
    string inputCopyable;
    string inputNewCopiedFile;
    cout << "Enter file to copy (D:\\folder\\file.txt)" << endl;
    cin >> inputCopyable;
    cout << "Enter new file (D:\\folder\\file_2.txt)" << endl;
    cin >> inputNewCopiedFile;

    if ( inputCopyable == inputNewCopiedFile) {
        cout << "Attempt to copy file in the same place" << endl;
    } else {
        BOOL success = CopyFileA(inputCopyable.c_str(), inputNewCopiedFile.c_str(), true);
        if (!success) {
            cout << "Error coping files. Error code: " << GetLastError() << endl;
        } else {
            cout << "Successfully copied files" << endl;
        }
    }
}

int main() {
    int choice;
    do {
        cout << "Menu:" << endl;
        cout << "1. Drivers list" << endl;
        cout << "2. Driver's info" << endl;
        cout << "3. Create directory" << endl;
        cout << "4. Delete directory" << endl;
        cout << "5. Create new file" << endl;
        cout << "6. Copy file" << endl;
        cout << "0. Exit" << endl;
        cout << "Choose operation: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                ListDrives();
                break;
            case 2:
                DiskInfo();
                break;
            case 3:
                CreateDirectory();
                break;
            case 4:
                RemoveDirectory();
                break;
            case 5:
                CreateNewFile();
                break;
            case 6:
                CopyFiles();
                break;
            case 0:
                cout << "Closing." << endl;
                break;
            default:
                cout << "Wrong answer. Try again. " << endl;
                break;
        }
    } while (choice != 0);
    
    return 0;
}
