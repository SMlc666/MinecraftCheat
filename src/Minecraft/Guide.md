## 指南
这是 MinecraftCheat 的指南。

### 函数分析查找方法

#### 1. Minecraft::Minecraft()
- **构造函数**：这是关于 Minecraft 类的构造函数。
- **查找方式**：通过IDA查找虚表的方式来查找这个函数，IDA通常会自动识别引用了虚表的函数。
- **注意事项**：构造函数一般作为引用了虚表的第一个函数出现在IDA中，因此可以直接找到这个函数的地址。
#### 2. Minecraft::update()
- **查找方式**: 在有符号的服务器版本中，我们可以在ida分析服务器的Minecraft::update()函数，可以发现在服务器的Minecraft::update()函数中，它使用了"disconnectionScreen.timeout"这个字符串，因此我们可以在客户端中搜索这个字符串，查找这个字符串被谁引用，就能找到Minecraft::update()函数的地址。
#### 3. ClientInstance::onStartJoinGame()
