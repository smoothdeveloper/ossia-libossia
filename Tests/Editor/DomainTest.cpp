#include <QtTest>
#include "../ForwardDeclaration.h"
#include <iostream>

using namespace OSSIA;

class DomainTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    
    /*! test life cycle and accessors functions */
    void test_basic()
    {
        ;
    }
    
    /*! test edition functions */
    void test_edition()
    {
        ;
    }
};

QTEST_APPLESS_MAIN(DomainTest)

#include "DomainTest.moc"
