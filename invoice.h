#ifndef INVOICE_H
#define INVOICE_H

class Invoice
{
public:
    Invoice();
    void run();
    virtual void message(int, int);
private:
    bool running;
};

#endif // INVOICE_H
