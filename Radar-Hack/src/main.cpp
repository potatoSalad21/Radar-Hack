#include "memory.h"
#include <iostream>
#include <thread>

namespace offsets
{
	// client
	constexpr ::std::ptrdiff_t dwLocalPlayer = 0xDEA98C;
	constexpr ::std::ptrdiff_t dwEntityList = 0x4DFFF7C;

	// entity
	constexpr ::std::ptrdiff_t m_bSpotted = 0x93D;
	constexpr ::std::ptrdiff_t m_iTeamNum = 0xF4;
}

int main()
{
	Memory mem { "csgo.exe" };
	std::cout << "[+] Process ID -> " << mem.getProcessId() << '\n';

	const auto client = mem.getModuleAddress("client.dll");
	std::cout << "[+] Client Dll -> 0x" << std::hex << client << std::dec << '\n';

	std::cout << "--------------------------------------------" << '\n';
	std::cout << "~ Starting The Cheat..." << '\n';

	// main hack loop
	while (true)
	{
		const auto localPlayer = mem.Read<uintptr_t>(client + offsets::dwLocalPlayer);
		const auto team = mem.Read<uintptr_t>(client + offsets::m_iTeamNum);

		for (int i = 1; i <= 64; ++i)
		{
			// iterating over entities (with the size of 0x10)
			const auto entity = mem.Read<uintptr_t>(client + offsets::dwEntityList + i * 0x10);

			if (mem.Read<uintptr_t>(entity + offsets::m_iTeamNum) == team)
				continue;

			mem.Write<bool>(entity + offsets::m_bSpotted, true);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return 0;
}
