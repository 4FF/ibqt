#include "testibqt.h"
#include <QtDebug>

TestIbQt::TestIbQt(QObject *parent) : QObject(parent)
{
    connect(&ib, SIGNAL(twsConnected()), this, SLOT(onTwsConnected()));
    connect(&ib, SIGNAL(managedAccounts(QByteArray)), this, SLOT(onManagedAccounts(QByteArray)));
    connect(&ib, SIGNAL(contractDetails(int,ContractDetails)), this, SLOT(onContractDetails(int,ContractDetails)));
    connect(&ib, SIGNAL(contractDetailsEnd(int)), this, SLOT(onContractDetailsEnd(int)));
}

void TestIbQt::run()
{
    ib.connectToTWS();
    reqContractDetails();


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

void TestIbQt::reqContractDetails()
{
    qDebug() << "orderTest";
    Contract c;
    c.symbol = QByteArray("AAPL");
    c.secType = QByteArray("STK");
    c.exchange = QByteArray("SMART");
    c.currency = QByteArray("USD");

    Order order;
      order.action = "BUY";
      order.orderType = "MKT";
      order.transmit = 1;
      order.totalQuantity = 100 ;


    ib.placeOrder(NEXT_VALID_ID,c,order);



}
