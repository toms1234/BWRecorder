/*****************************************************************************
* Model: bwgpsrecorder.qm
* File:  qmcode/qvdr.c
*
* This code has been generated by QM tool (see state-machine.com/qm).
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*****************************************************************************/
/* @(/3/21) ................................................................*/
#include "qp_port.h"
#include "qevents.h"
#include "bsp.h"
#include "utility.h"
#include "error.h"
#include "type.h"

Q_DEFINE_THIS_MODULE("qvdr.c")


/* Active object class -----------------------------------------------------*/
/* @(/1/20) ................................................................*/
typedef struct QVDRTag {
/* protected: */
    QActive super;
} QVDR;

/* protected: */
static QState QVDR_initial(QVDR * const me, QEvt const * const e);
static QState QVDR_state1(QVDR * const me, QEvt const * const e);



/* Local objects -----------------------------------------------------------*/
static QVDR l_QVDR; /* the single instance of the Table active object */

/* Global-scope objects ----------------------------------------------------*/
QActive * const AO_VDR = &l_QVDR.super; /* "opaque" AO pointer */

/*..........................................................................*/
/* @(/1/42) ................................................................*/
void QVDR_ctor(void) {
    QVDR *me = &l_QVDR;
    QActive_ctor(&me->super, Q_STATE_CAST(&QVDR_initial));
}
/* @(/1/20) ................................................................*/
/* @(/1/20/0) ..............................................................*/
/* @(/1/20/0/0) */
static QState QVDR_initial(QVDR * const me, QEvt const * const e) {
    return Q_TRAN(&QVDR_state1);
}
/* @(/1/20/0/1) ............................................................*/
static QState QVDR_state1(QVDR * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
