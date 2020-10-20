#include "testibqt.h"
#include <QtDebug>

TestIbQt::TestIbQt(QObject *parent) : QObject(parent)
{
    connect(&ib, SIGNAL(twsConnected()), this, SLOT(onTwsConnected()));
    connect(&ib, SIGNAL(managedAccounts(QByteArray)), this, SLOT(onManagedAccounts(QByteArray)));
    connect(&ib, SIGNAL(contractDetails(int,ContractDetails)), this, SLOT(onContractDetails(int,ContractDetails)));
    connect(&ib, SIGNAL(contractDetailsEnd(int)), this, SLOT(onContractDetailsEnd(int)));
    connect(&ib, SIGNAL(nextValidId(long)), this, SLOT(nextValidIdNew(long)));

}

void TestIbQt::marketOrder(char dir, QByteArray stockName, int quantity)
{
    qDebug() << "orderTest";
    Contract c;
    c.symbol = stockName;
    c.secType = QByteArray("STK");
    c.exchange = QByteArray("SMART");
    c.currency = QByteArray("USD");

    Order order;
    if(dir==1){
        order.action = "BUY";
    }else if(dir==-1){
        order.action = "SELL";
    }

      order.orderType = "MKT";
      order.totalQuantity = quantity;

    ib.placeOrder(ib.getOrderId(),c,order);

}

void TestIbQt::run()
{
    ib.connectToTWS();

}

void TestIbQt::onTwsConnected()
{
    qDebug() << "TWS Connected";
}

void TestIbQt::onManagedAccounts(const QByteArray &accountsList)
{
    qDebug() << "Managed Accounts:" << accountsList;
}

void TestIbQt::onContractDetails(int reqId, const ContractDetails &contractDetails)
{
    Q_UNUSED(reqId)
    qDebug() << "Sym:" << contractDetails.summary.symbol << "Exp:" << contractDetails.summary.expiry;
}

void TestIbQt::onContractDetailsEnd(int reqId)
{
    qDebug() << "onContractDetailsEnd:" << reqId;
}


void TestIbQt::nextValidIdNew(OrderId Id)
{
     ib.setOrderId(Id);
     qDebug() << "nextValidIdInitialization:" << Id;
}

