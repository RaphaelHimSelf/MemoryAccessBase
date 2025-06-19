#include "Memory/memory.h"

int main(){

  //Memory mem(L"process.exe");                                                     --- Initialize the memory class

  //uintptr_t moduleAddress = mem.GetModuleBaseAddress(L"module.dll");              --- Get the module base address
  //uintptr_t addressPointer = mem.Read<uintptr_t>(moduleAddress + 0x000000);       --- Read a pointer from memory

  //uintptr_t addressToModify = addressPointer + 0x00;                              --- Add an offset to the address

  //int currentValue = mem.Read<int>(addressToModify);                              --- Read the address value
  //mem.Write<int>(addressToModify, 10);                                            --- Write the integer value (10) to the address

  return 0;
}
