#pragma once
class ClientInstance;
namespace runtimes {
ClientInstance *getClientInstance();
void setClientInstance(ClientInstance *instance);
} // namespace runtimes