#ifndef SHOP_H
#define SHOP_H
#include "loginwindow.h"
#include "shopmainwindow.h"

class Shop{
public:
    Shop();
    ~Shop();
    void Login(void);
private:
    LoginWindow *m_logWindow;
    ShopMainWindow *m_shopWindow;
};

#endif // SHOP_H
