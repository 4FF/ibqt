#ifndef TESTIBQT_H
#define TESTIBQT_H

#include "ibqt.h"
#include <QObject>

class TestIbQt : public QObject
{
    Q_OBJECT
public:
    explicit TestIbQt(QObject *parent = 0);


    //orders
    void marketOrder(char dir, QByteArray stockName, QByteArray secType, int quantity);
    void limitOrder(char dir, QByteArray stockName, QByteArray secType, int quantity, double price);
    void limitOrder(char dir, QByteArray stockName, QByteArray secType, int quantity, double price, double sl,double tp);

    void BracketOrder(int parentOrderId, Order& parent, Order& takeProfit, Order& stopLoss, QByteArray action, double quantity, double limitPrice, double takeProfitLimitPrice, double stopLossPrice);


public slots:
    void run();

private slots:
    void onTwsConnected();
    void onManagedAccounts(const QByteArray & accountsList);
    void onContractDetails(int reqId, const ContractDetails & contractDetails);
    void onContractDetailsEnd(int reqId);
    void nextValidIdNew(OrderId Id);
private:
    IBQt ib;

};

#endif // TESTIBQT_H
