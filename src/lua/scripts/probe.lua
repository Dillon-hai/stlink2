-- stlink2/probe.lua

print("# stlink2 " .. stlink2.version())

devs = stlink2.probe()
for k,v in pairs(devs)
do
	dev = stlink2.open(v)
	print("serial: " .. dev:serial())
	print("name: " .. dev:name())
	print("version: " .. dev:version())
	print("voltage: " .. dev:voltage())
	print(string.format("coreid: 0x%08x", dev:coreid()))
	print(string.format("cpuid: 0x%08x", dev:cpuid()))
	print(string.format("chipid: 0x%08x", dev:chipid()))
	print(string.format("devid: 0x%03x", dev:devid()))
	print(string.format("flashSize: %d", dev:flashSize()))
	print(string.format("r0: 0x%08x", dev:readReg()))
end
