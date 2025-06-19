#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <TlHelp32.h>


class Memory{
private:
  uintptr_t  processId = 0;
  HANDLE processHandle = nullptr;

public:


  // Constructor that finds the processid and opens the process handle

  Memory(const wchar_t* processName) noexcept{
    PROCESSENTRY32W processEntry = {};
    processEntry.dwSize = sizeof(PROCESSENTRY32W);

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hSnap == INVALID_HANDLE_VALUE)
      return;

    if(Process32FirstW(hSnap, &processEntry)){
      do{
        if(wcscmp(processEntry.szExeFile, processName) == 0){
          processId     = processEntry.th32ProcessID;
          processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
          break;
        }
      }while(Process32NextW(hSnap, &processEntry));
    }

    if(hSnap)
      CloseHandle(hSnap);
  }


  // Destructor that frees handles

  ~Memory(){
    if(processHandle)
      CloseHandle(processHandle);
  }


  // Function that finds the module address

  uintptr_t GetModuleBaseAddress(const wchar_t* moduleName) const noexcept{
    uintptr_t moduleAddress = 0;

    MODULEENTRY32W moduleEntry = {};
    moduleEntry.dwSize = sizeof(MODULEENTRY32W);

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
    if(hSnap == INVALID_HANDLE_VALUE)
      return 0;

    if(Module32FirstW(hSnap, &moduleEntry)){
      do{
        if(wcscmp(moduleEntry.szModule, moduleName) == 0){
          moduleAddress = reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr);
          break;
        }
      }while(Module32NextW(hSnap, &moduleEntry));
    }

    if(hSnap)
      CloseHandle(hSnap);

    return moduleAddress;
  }


  // Function that return the processId

  uintptr_t GetProcessId() const noexcept{
    return processId;
  }


  // Generic function that reads the value from a memory address

  template <typename T>
  T Read(const uintptr_t address) const noexcept{
    T value = {};
    ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address), &value, sizeof(T), nullptr);
    return value;
  }


  // Generic function that writes a value to a memory address

  template <typename T>
  void Write(const uintptr_t address, const T& value) const noexcept{
    WriteProcessMemory(processHandle, reinterpret_cast<LPVOID>(address), &value, sizeof(T), nullptr);
  }

};
