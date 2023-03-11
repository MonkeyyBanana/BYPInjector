#include "includes.h"
#include "utils.h"

void quit(const std::string msg) {
	std::cout << msg << std::endl;
	system("pause");
	ExitProcess(0);
}

char* randomCharStr(int size) {
	char* out = (char*)malloc(sizeof(char) * size);
	static const char pool[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYabcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()-_=+[{]};:,<.>/?`~ " };
	srand((unsigned)time(NULL) * 7);
	for (int i = 0; i < size; ++i)
		out[i] = pool[rand() % (sizeof(pool) - 1)];
	return out;
}

int main() {
	srand((unsigned)time(NULL) * 11);
	SetConsoleTitleA(randomCharStr(7 + rand() % (64 - 7 + 1)));

	DWORD pid = Utils::getProcessID(Utils::antigibber("Y3Nnby5leGU=").c_str());

	if (!pid)
		quit(Utils::antigibber("IjQwNCBleGUgbm90IGZvdW5kIC1fLSI="));

	std::cout << "PID Hit! " << std::dec << pid << std::endl;

	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	uintptr_t module = Utils::getModuleBaseAddress(pid, Utils::antigibber("Y2xpZW50LmRsbA==").c_str());

	if (Utils::doFileExist(Utils::antigibber("bW9ua2V5LmRsbA==").c_str())) {
		if (!Utils::execBypass(handle)) {
			quit("We down");
		}

		if (Utils::checkOutABook(pid, Utils::antigibber("bW9ua2V5LmRsbA==").c_str())) {
			Utils::patch(handle);
			quit("We up");
		}
		else {
			Utils::patch(handle);
			quit("We def down");
		}
	}
	else {
		quit("Where file?");
	}

	return 0;
}