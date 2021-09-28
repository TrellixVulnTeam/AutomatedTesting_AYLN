#include "contanst.h"

Contanst::Contanst(QObject *parent) : QObject(parent)
{

}


QString Contanst::TestStatusInit()
{
    return "INIT";
}


QString Contanst::TestStatusFail()
{
    return "FAIL";
}


QString Contanst::TestStatusPass()
{
    return "PASS";
}


QString Contanst::TestStatusError(){

    return "Error";
}


