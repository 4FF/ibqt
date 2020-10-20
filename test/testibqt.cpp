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

void TestIbQt::marketOrder(char dir, QByteArray stockName,QByteArray secType, int quantity)
{

    Contract c;
    c.symbol = stockName;
    c.secType = secType;
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
       order.transmit=1;

    ib.placeOrder(ib.getOrderId(),c,order);

     qDebug() << "marketOrder";

}

void TestIbQt::limitOrder(char dir, QByteArray stockName, QByteArray secType, int quantity, double price)
{
    Contract c;
    c.symbol = stockName;
    c.secType = secType;
    c.exchange = QByteArray("SMART");
    c.currency = QByteArray("USD");

    Order order;
    if(dir==1){
        order.action = "BUY";
    }else if(dir==-1){
        order.action = "SELL";
    }
      order.lmtPrice = price;
      order.orderType = "LMT";
      order.totalQuantity = quantity;
      order.transmit=1;

    ib.placeOrder(ib.getOrderId(),c,order);

     qDebug() << "marketOrder";


}

void TestIbQt::limitOrder(char dir, QByteArray stockName, QByteArray secType, int quantity, double price, double sl, double tp)
{
     Order parent;
     Order takeProfit;
     Order stopLoss;

     if(dir==1){
         parent.action = "BUY";
     }else if(dir==-1){
         parent.action = "SELL";
     }

     Contract c;
     c.symbol = stockName;
     c.secType = secType;
     c.exchange = QByteArray("SMART");
     c.currency = QByteArray("USD");


      BracketOrder(ib.getOrderId(),parent , takeProfit, stopLoss, parent.action, quantity, price,tp, sl);
      ib.placeOrder(parent.orderId, c, parent);
      ib.placeOrder(takeProfit.orderId, c, takeProfit);
      ib.placeOrder(stopLoss.orderId, c, stopLoss);

}

void TestIbQt::BracketOrder(int parentOrderId, Order &parent, Order &takeProfit, Order &stopLoss, QByteArray action, double quantity, double limitPrice, double takeProfitLimitPrice, double stopLossPrice)
{
    //This will be our main or "parent" order
     parent.orderId = parentOrderId;
     parent.action = action;
     parent.orderType = "LMT";
     parent.totalQuantity = quantity;
     parent.lmtPrice = limitPrice;
     //The parent and children orders will need this attribute set to false to prevent accidental executions.
     //The LAST CHILD will have it set to true,
     parent.transmit = false;
     takeProfit.orderId = parent.orderId + 1;
     takeProfit.action = (action == "BUY") ? "SELL" : "BUY";
     takeProfit.orderType = "LMT";
     takeProfit.totalQuantity = quantity;
     takeProfit.lmtPrice = takeProfitLimitPrice;
     takeProfit.parentId = parentOrderId;
     takeProfit.transmit = false;
     stopLoss.orderId = parent.orderId + 2;
     stopLoss.action = (action == "BUY") ? "SELL" : "BUY";
     stopLoss.orderType = "STP";
     //Stop trigger price
     stopLoss.auxPrice = stopLossPrice;
     stopLoss.totalQuantity = quantity;
     stopLoss.parentId = parentOrderId;
     //In this case, the low side order will be the last child being sent. Therefore, it needs to set this attribute to true
     //to activate all its predecessors
     stopLoss.transmit = true;
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

