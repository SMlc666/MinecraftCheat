#include "gui.hpp"
GUI::GUI(std::shared_ptr<Module>& m_module)
    : module(m_module), first(MenuNames.at(m_module->getMenuType())) {
        
}