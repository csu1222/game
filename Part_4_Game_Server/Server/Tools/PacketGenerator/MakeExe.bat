pushd %~dp0
pyinstaller --onefile PacketGenerator.py
MOVE .\dist\PacketGenerator.exe .\GenPacket.exe
@RD /S /Q .\build
@RD /S /Q .\dist
DEL /S /Q .\PacketGenerator.spec 
PAUSE