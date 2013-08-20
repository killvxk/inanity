#include "../inanity-graphics.hpp"
#include "../inanity-dx11.hpp"
#include "../inanity-gl.hpp"
using namespace Inanity;
using namespace Inanity::Graphics;

int main(int argc, char** argv)
{
	ptr<System> system;
	String systemStr;
	if(argc >= 2)
		systemStr = argv[1];
	if(systemStr == "dx11")
	{
		system = NEW(Dx11System());
	}
	else if(systemStr == "gl")
	{
		system = NEW(GlSystem());
	}
	if(!system)
	{
		std::cout << "Usage: adapterstest <system>\nSupported systems: dx11, gl.\n";
		return 0;
	}

	const std::vector<ptr<Adapter> >& adapters = system->GetAdapters();
	for(size_t i = 0; i < adapters.size(); ++i)
	{
		ptr<Adapter> adapter = adapters[i];
		std::cout << "Adapter\n  ID=" << adapter->GetId() << "\n  Name=" << adapter->GetName() << "\n";
		const std::vector<ptr<Monitor> >& monitors = adapter->GetMonitors();
		for(size_t j = 0; j < monitors.size(); ++j)
		{
			ptr<Monitor> monitor = monitors[i];
			std::cout << "  Monitor\n    ID=" << monitor->GetId() << "\n    Name=" << monitor->GetName() << "\n";
			const std::vector<ptr<MonitorMode> >& modes = monitor->GetModes();
			for(size_t k = 0; k < modes.size(); ++k)
			{
				ptr<MonitorMode> mode = modes[k];
				std::cout << "    Mode " << mode->GetName() << "\n";
			}
		}
	}

	return 0;
}