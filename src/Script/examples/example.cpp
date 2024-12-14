#include <iostream>
#include "dobby.h"
#include <sys/mman.h>
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "lobject.h"
#include "llimits.h"
}
l_sinline void (*old_ccall)(lua_State *L, StkId func, int nResults, l_uint32 inc);
l_sinline void new_ccall(lua_State *L, StkId func, int nResults, l_uint32 inc) {
  std::cout << "ccall called" << std::endl;
  old_ccall(L, func, nResults, inc);
}
int main() {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  void *handle = DobbySymbolResolver(nullptr, "ccall");
  std::cout << "handle: " << handle << std::endl;
  DobbyHook((void *)handle, (void *)old_ccall, (void **)&new_ccall);
  std::cout << "hooked ccall" << std::endl;
  const char *script = "print('Hello, Lua!')";

  luaL_dostring(L, script);
  lua_close(L);
  return 0;
}