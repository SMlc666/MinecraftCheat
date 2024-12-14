#include <iostream>
#include "dobby.h"
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include "LuaBridge/LuaBridge.h"
int myPrint(lua_State *L) {
  std::cout << "myPrint called" << std::endl;
  int n = lua_gettop(L); // 获取栈顶元素的数量
  for (int i = 1; i <= n; ++i) {
    std::cout << lua_tostring(L, i); // 将每个元素转换为字符串并输出
    if (i < n) {
      std::cout << "\t"; // 如果不是最后一个元素，输出制表符分隔
    }
  }
  std::cout << std::endl; // 输出换行符
  return 0;               // 返回值的数量为 0
}

int main() {
  void *handle = DobbySymbolResolver(nullptr, "luaB_print");
  std::cout << "handle:" << handle << std::endl;
  DobbyHook(handle, (void *)myPrint, nullptr);
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  int sb = 10;
  luabridge::getGlobalNamespace(L).beginNamespace("global").addVariable("sb", &sb).endNamespace();
  luaL_dostring(L, "print(global.sb)");
  sb = 20;
  luaL_dostring(L, "print(global.sb)");
  lua_register(L, "print", myPrint);
  luabridge::getGlobalNamespace(L).addFunction("myPrint", myPrint);
  luaL_dostring(L, "myPrint('hello', 'world')");
  lua_close(L);
  return 0;
}