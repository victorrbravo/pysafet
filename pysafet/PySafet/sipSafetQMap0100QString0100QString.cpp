/*
 * Interface wrapper code.
 *
 * Generated by SIP 4.19.7
 */

#include "sipAPISafet.h"

#line 28 "/usr/share/sip/PyQt4/QtCore/qmap.sip"
#include <qmap.h>
#line 12 "./sipSafetQMap0100QString0100QString.cpp"

#line 68 "/usr/share/sip/PyQt4/QtCore/qstring.sip"
#include <qstring.h>
#line 16 "./sipSafetQMap0100QString0100QString.cpp"
#line 27 "/usr/share/sip/PyQt4/QtCore/qstring.sip"
#include <qstring.h>
#line 19 "./sipSafetQMap0100QString0100QString.cpp"


extern "C" {static void assign_QMap_0100QString_0100QString(void *, SIP_SSIZE_T, void *);}
static void assign_QMap_0100QString_0100QString(void *sipDst, SIP_SSIZE_T sipDstIdx, void *sipSrc)
{
    reinterpret_cast<QMap< ::QString, ::QString> *>(sipDst)[sipDstIdx] = *reinterpret_cast<QMap< ::QString, ::QString> *>(sipSrc);
}


extern "C" {static void *array_QMap_0100QString_0100QString(SIP_SSIZE_T);}
static void *array_QMap_0100QString_0100QString(SIP_SSIZE_T sipNrElem)
{
    return new QMap< ::QString, ::QString>[sipNrElem];
}


extern "C" {static void *copy_QMap_0100QString_0100QString(const void *, SIP_SSIZE_T);}
static void *copy_QMap_0100QString_0100QString(const void *sipSrc, SIP_SSIZE_T sipSrcIdx)
{
    return new QMap< ::QString, ::QString>(reinterpret_cast<const QMap< ::QString, ::QString> *>(sipSrc)[sipSrcIdx]);
}


/* Call the mapped type's destructor. */
extern "C" {static void release_QMap_0100QString_0100QString(void *, int);}
static void release_QMap_0100QString_0100QString(void *ptr, int)
{
    Py_BEGIN_ALLOW_THREADS
    delete reinterpret_cast<QMap< ::QString, ::QString> *>(ptr);
    Py_END_ALLOW_THREADS
}



extern "C" {static int convertTo_QMap_0100QString_0100QString(PyObject *, void **, int *, PyObject *);}
static int convertTo_QMap_0100QString_0100QString(PyObject *sipPy,void **sipCppPtrV,int *sipIsErr,PyObject *sipTransferObj)
{
    QMap< ::QString, ::QString> **sipCppPtr = reinterpret_cast<QMap< ::QString, ::QString> **>(sipCppPtrV);

#line 84 "/usr/share/sip/PyQt4/QtCore/qmap.sip"
    PyObject *t1obj, *t2obj;
    SIP_SSIZE_T i = 0;

    // Check the type if that is all that is required.
    if (sipIsErr == NULL)
    {
        if (!PyDict_Check(sipPy))
            return 0;

        while (PyDict_Next(sipPy, &i, &t1obj, &t2obj))
        {
            if (!sipCanConvertToType(t1obj, sipType_QString, SIP_NOT_NONE))
                return 0;

            if (!sipCanConvertToType(t2obj, sipType_QString, SIP_NOT_NONE))
                return 0;
        } 

        return 1;
    }

    QMap<QString, QString> *qm = new QMap<QString, QString>;
 
    while (PyDict_Next(sipPy, &i, &t1obj, &t2obj))
    {
        int state1, state2;

        QString *t1 = reinterpret_cast<QString *>(sipConvertToType(t1obj, sipType_QString, sipTransferObj, SIP_NOT_NONE, &state1, sipIsErr));
        QString *t2 = reinterpret_cast<QString *>(sipConvertToType(t2obj, sipType_QString, sipTransferObj, SIP_NOT_NONE, &state2, sipIsErr));
 
        if (*sipIsErr)
        {
            sipReleaseType(t1, sipType_QString, state1);
            sipReleaseType(t2, sipType_QString, state2);

            delete qm;
            return 0;
        }

        qm->insert(*t1, *t2);

        sipReleaseType(t1, sipType_QString, state1);
        sipReleaseType(t2, sipType_QString, state2);
    }
 
    *sipCppPtr = qm;
 
    return sipGetState(sipTransferObj);
#line 108 "./sipSafetQMap0100QString0100QString.cpp"
}


extern "C" {static PyObject *convertFrom_QMap_0100QString_0100QString(void *, PyObject *);}
static PyObject *convertFrom_QMap_0100QString_0100QString(void *sipCppV, PyObject *sipTransferObj)
{
   QMap< ::QString, ::QString> *sipCpp = reinterpret_cast<QMap< ::QString, ::QString> *>(sipCppV);

#line 32 "/usr/share/sip/PyQt4/QtCore/qmap.sip"
    // Create the dictionary.
    PyObject *d = PyDict_New();

    if (!d)
        return NULL;

    // Set the dictionary elements.
    QMap<QString, QString>::const_iterator i = sipCpp->constBegin();

    while (i != sipCpp->constEnd())
    {
        QString *t1 = new QString(i.key());
        QString *t2 = new QString(i.value());

        PyObject *t1obj = sipConvertFromNewType(t1, sipType_QString, sipTransferObj);
        PyObject *t2obj = sipConvertFromNewType(t2, sipType_QString, sipTransferObj);

        if (t1obj == NULL || t2obj == NULL || PyDict_SetItem(d, t1obj, t2obj) < 0)
        {
            Py_DECREF(d);

            if (t1obj)
            {
                Py_DECREF(t1obj);
            }
            else
            {
                delete t1;
            }

            if (t2obj)
            {
                Py_DECREF(t2obj);
            }
            else
            {
                delete t2;
            }

            return NULL;
        }

        Py_DECREF(t1obj);
        Py_DECREF(t2obj);

        ++i;
    }

    return d;
#line 167 "./sipSafetQMap0100QString0100QString.cpp"
}


sipMappedTypeDef sipTypeDef_Safet_QMap_0100QString_0100QString = {
    {
        -1,
        0,
        0,
        SIP_TYPE_MAPPED,
        sipNameNr_645,     /* QMap<QString,QString> */
        {0},
        0
    },
    {
        -1,
        {0, 0, 1},
        0, 0,
        0, 0,
        0, 0,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    },
    assign_QMap_0100QString_0100QString,
    array_QMap_0100QString_0100QString,
    copy_QMap_0100QString_0100QString,
    release_QMap_0100QString_0100QString,
    convertTo_QMap_0100QString_0100QString,
    convertFrom_QMap_0100QString_0100QString
};
