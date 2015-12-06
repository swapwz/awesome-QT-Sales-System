#include "shop.h"

Shop::Shop()
{
    m_logWindow = new LoginWindow();
    m_shopWindow = new ShopMainWindow();
}
Shop::~Shop()
{
    delete m_logWindow;
    delete m_shopWindow;
}
void Shop::Login(void)
{
    m_logWindow->show();
    QObject::connect(m_logWindow, SIGNAL(loginValidate(int)), m_shopWindow, SLOT(login(int)));

    return ;
}
