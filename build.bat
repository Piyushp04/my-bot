@echo off
g++ main.cpp ^
  -I"C:\Users\patha\OneDrive\Desktop\my bot\vcpkg\installed\x64-windows\include" ^
  -L"C:\Users\patha\OneDrive\Desktop\my bot\vcpkg\installed\x64-windows\lib" ^
  -lcurl -o llm_chat.exe
echo Build done!
pause