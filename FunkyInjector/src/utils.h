#ifndef UTILS_H
#define UTILS_H

namespace Utils {
	DWORD getProcessID(const char* processName) {
		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(PROCESSENTRY32);

		const auto snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		while (Process32Next(snapShot, &entry)) {
			_bstr_t tstr(entry.szExeFile);
			const char* exename = tstr;

			if (!strcmp(processName, exename)) {
				CloseHandle(snapShot);
				return entry.th32ProcessID;
			}
		}

		CloseHandle(snapShot);
		return NULL;
	}

	uintptr_t getModuleBaseAddress(DWORD pid, const char* moduleName) {
		HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
		if (snapShot != INVALID_HANDLE_VALUE) {
			MODULEENTRY32 entry;
			entry.dwSize = sizeof(MODULEENTRY32);

			while (Module32Next(snapShot, &entry)) {
				_bstr_t tstr(entry.szModule);
				const char* modname = tstr;

				if (!strcmp(moduleName, modname)) {
					CloseHandle(snapShot);
					return (uintptr_t)entry.modBaseAddr;
				}
			}
		}

		CloseHandle(snapShot);
		return NULL;
	}

	static std::string antigibber(const std::string& in) {

		std::string out;

		std::vector<int> T(256, -1);
		for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

		int val = 0, valb = -8;
		for (char c : in) {
			if (T[c] == -1) break;
			val = (val << 6) + T[c];
			valb += 6;
			if (valb >= 0) {
				out.push_back(char((val >> valb) & 0xFF));
				valb -= 8;
			}
		}

		return out;
	}

	bool doFileExist(const char* name) {
		if (FILE* file = fopen(name, "r")) {
			fclose(file);
			return true;
		}

		return false;
	}

	LPVOID ntSnipe = GetProcAddress(LoadLibraryW(L"ntdll"), "NtOpenFile");

	bool execBypass(HANDLE process) {
		if (ntSnipe) {
			char ogByte[5];
			memcpy(ogByte, ntSnipe, 5);

			if (WriteProcessMemory(process, ntSnipe, ogByte, 5, NULL)) {
				std::cout << "Damn ok?" << std::endl;
				return TRUE;
			}
		}

		return FALSE;
	}

	bool patch(HANDLE process) {
		if (ntSnipe) {
			char ogByte[5];
			memcpy(ogByte, ntSnipe, 5);
			WriteProcessMemory(process, ntSnipe, ogByte, 0, 0);
			return TRUE;
		}

		return FALSE;
	}

	bool checkOutABook(DWORD process, const char* booklocation) {
		if (!process)
			return false;

		char newbook[MAX_PATH];

		GetFullPathName(booklocation, MAX_PATH, newbook, 0);

		HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process);
		LPVOID allocMem = VirtualAllocEx(handle, NULL, sizeof(newbook), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		if (!WriteProcessMemory(handle, allocMem, newbook, sizeof(newbook), NULL))
			return FALSE;

		CreateRemoteThread(handle, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibrary, allocMem, 0, 0);

		if (handle)
			CloseHandle(handle);

		return TRUE;
	}
}

#endif