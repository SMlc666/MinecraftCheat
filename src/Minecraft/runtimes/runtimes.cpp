#include "runtimes.hpp"
class ClientInstance;
static ClientInstance *instance = nullptr;
ClientInstance *runtimes::getClientInstance() {
  return instance;
}
void runtimes::setClientInstance(ClientInstance *minstance) {
  instance = minstance;
}