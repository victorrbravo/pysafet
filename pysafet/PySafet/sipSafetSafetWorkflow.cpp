/*
 * Interface wrapper code.
 *
 * Generated by SIP 4.19.7
 */

#include "sipAPISafet.h"

#line 275 "Safet.sip"
#include "../websafet/src/SafetWorkflow.h"
#line 12 "./sipSafetSafetWorkflow.cpp"

#line 27 "/usr/share/sip/PyQt4/QtCore/qstring.sip"
#include <qstring.h>
#line 16 "./sipSafetSafetWorkflow.cpp"
#line 336 "/usr/share/sip/PyQt4/QtXml/qdom.sip"
#include <qdom.h>
#line 19 "./sipSafetSafetWorkflow.cpp"
#line 26 "/usr/share/sip/PyQt4/QtCore/qcoreevent.sip"
#include <qcoreevent.h>
#line 22 "./sipSafetSafetWorkflow.cpp"
#line 368 "/usr/share/sip/PyQt4/QtCore/qcoreevent.sip"
#include <qcoreevent.h>
#line 25 "./sipSafetSafetWorkflow.cpp"
#line 351 "/usr/share/sip/PyQt4/QtCore/qcoreevent.sip"
#include <qcoreevent.h>
#line 28 "./sipSafetSafetWorkflow.cpp"
#line 29 "/usr/share/sip/PyQt4/QtCore/qobject.sip"
#include <qobject.h>
#line 31 "./sipSafetSafetWorkflow.cpp"
#line 265 "/usr/share/sip/PyQt4/QtCore/qvariant.sip"
#include <qvariant.h>
#line 34 "./sipSafetSafetWorkflow.cpp"
#line 28 "/usr/share/sip/PyQt4/QtCore/qlist.sip"
#include <qlist.h>
#line 37 "./sipSafetSafetWorkflow.cpp"
#line 32 "/usr/share/sip/PyQt4/QtCore/qbytearray.sip"
#include <qbytearray.h>
#line 40 "./sipSafetSafetWorkflow.cpp"
#line 26 "/usr/share/sip/PyQt4/QtCore/qnamespace.sip"
#include <qnamespace.h>
#line 43 "./sipSafetSafetWorkflow.cpp"
#line 115 "/usr/share/sip/PyQt4/QtCore/qlist.sip"
#include <qlist.h>
#line 46 "./sipSafetSafetWorkflow.cpp"
#line 26 "/usr/share/sip/PyQt4/QtCore/qthread.sip"
#include <qthread.h>
#line 49 "./sipSafetSafetWorkflow.cpp"
#line 26 "/usr/share/sip/PyQt4/QtCore/qregexp.sip"
#include <qregexp.h>
#line 52 "./sipSafetSafetWorkflow.cpp"
#line 26 "/usr/share/sip/PyQt4/QtCore/qobjectdefs.sip"
#include <qobjectdefs.h>
#line 55 "./sipSafetSafetWorkflow.cpp"


class sipSafetWorkflow : public  ::SafetWorkflow
{
public:
    sipSafetWorkflow();
    virtual ~sipSafetWorkflow();

    int qt_metacall(QMetaObject::Call,int,void **);
    void *qt_metacast(const char *);
    const QMetaObject *metaObject() const;

    /*
     * There is a public method for every protected method visible from
     * this class.
     */
     ::QObject* sipProtect_sender() const;
    int sipProtect_receivers(const char*) const;
    void sipProtectVirt_timerEvent(bool, ::QTimerEvent*);
    void sipProtectVirt_childEvent(bool, ::QChildEvent*);
    void sipProtectVirt_customEvent(bool, ::QEvent*);
    void sipProtectVirt_connectNotify(bool,const char*);
    void sipProtectVirt_disconnectNotify(bool,const char*);
    int sipProtect_senderSignalIndex() const;

    /*
     * There is a protected method for every virtual method visible from
     * this class.
     */
protected:
    void disconnectNotify(const char*);
    void connectNotify(const char*);
    void customEvent( ::QEvent*);
    void childEvent( ::QChildEvent*);
    void timerEvent( ::QTimerEvent*);
    bool eventFilter( ::QObject*, ::QEvent*);
    bool event( ::QEvent*);

public:
    sipSimpleWrapper *sipPySelf;

private:
    sipSafetWorkflow(const sipSafetWorkflow &);
    sipSafetWorkflow &operator = (const sipSafetWorkflow &);

    char sipPyMethods[7];
};

sipSafetWorkflow::sipSafetWorkflow():  ::SafetWorkflow(), sipPySelf(0)
{
    memset(sipPyMethods, 0, sizeof (sipPyMethods));
}

sipSafetWorkflow::~sipSafetWorkflow()
{
    sipInstanceDestroyed(sipPySelf);
}

const QMetaObject *sipSafetWorkflow::metaObject() const
{
    return sip_Safet_qt_metaobject(sipPySelf,sipType_SafetWorkflow);
}

int sipSafetWorkflow::qt_metacall(QMetaObject::Call _c,int _id,void **_a)
{
    _id =  ::SafetWorkflow::qt_metacall(_c,_id,_a);

    if (_id >= 0)
        _id = sip_Safet_qt_metacall(sipPySelf,sipType_SafetWorkflow,_c,_id,_a);

    return _id;
}

void *sipSafetWorkflow::qt_metacast(const char *_clname)
{
    return (sip_Safet_qt_metacast(sipPySelf, sipType_SafetWorkflow, _clname)) ? this :  ::SafetWorkflow::qt_metacast(_clname);
}

void sipSafetWorkflow::disconnectNotify(const char*a0)
{
    sip_gilstate_t sipGILState;
    PyObject *sipMeth;

    sipMeth = sipIsPyMethod(&sipGILState,&sipPyMethods[0],sipPySelf,NULL,sipName_disconnectNotify);

    if (!sipMeth)
    {
         ::SafetWorkflow::disconnectNotify(a0);
        return;
    }

    extern void sipVH_Safet_0(sip_gilstate_t, sipVirtErrorHandlerFunc, sipSimpleWrapper *, PyObject *, const char*);

    sipVH_Safet_0(sipGILState, 0, sipPySelf, sipMeth, a0);
}

void sipSafetWorkflow::connectNotify(const char*a0)
{
    sip_gilstate_t sipGILState;
    PyObject *sipMeth;

    sipMeth = sipIsPyMethod(&sipGILState,&sipPyMethods[1],sipPySelf,NULL,sipName_connectNotify);

    if (!sipMeth)
    {
         ::SafetWorkflow::connectNotify(a0);
        return;
    }

    extern void sipVH_Safet_0(sip_gilstate_t, sipVirtErrorHandlerFunc, sipSimpleWrapper *, PyObject *, const char*);

    sipVH_Safet_0(sipGILState, 0, sipPySelf, sipMeth, a0);
}

void sipSafetWorkflow::customEvent( ::QEvent*a0)
{
    sip_gilstate_t sipGILState;
    PyObject *sipMeth;

    sipMeth = sipIsPyMethod(&sipGILState,&sipPyMethods[2],sipPySelf,NULL,sipName_customEvent);

    if (!sipMeth)
    {
         ::SafetWorkflow::customEvent(a0);
        return;
    }

    extern void sipVH_Safet_1(sip_gilstate_t, sipVirtErrorHandlerFunc, sipSimpleWrapper *, PyObject *,  ::QEvent*);

    sipVH_Safet_1(sipGILState, 0, sipPySelf, sipMeth, a0);
}

void sipSafetWorkflow::childEvent( ::QChildEvent*a0)
{
    sip_gilstate_t sipGILState;
    PyObject *sipMeth;

    sipMeth = sipIsPyMethod(&sipGILState,&sipPyMethods[3],sipPySelf,NULL,sipName_childEvent);

    if (!sipMeth)
    {
         ::SafetWorkflow::childEvent(a0);
        return;
    }

    extern void sipVH_Safet_2(sip_gilstate_t, sipVirtErrorHandlerFunc, sipSimpleWrapper *, PyObject *,  ::QChildEvent*);

    sipVH_Safet_2(sipGILState, 0, sipPySelf, sipMeth, a0);
}

void sipSafetWorkflow::timerEvent( ::QTimerEvent*a0)
{
    sip_gilstate_t sipGILState;
    PyObject *sipMeth;

    sipMeth = sipIsPyMethod(&sipGILState,&sipPyMethods[4],sipPySelf,NULL,sipName_timerEvent);

    if (!sipMeth)
    {
         ::SafetWorkflow::timerEvent(a0);
        return;
    }

    extern void sipVH_Safet_3(sip_gilstate_t, sipVirtErrorHandlerFunc, sipSimpleWrapper *, PyObject *,  ::QTimerEvent*);

    sipVH_Safet_3(sipGILState, 0, sipPySelf, sipMeth, a0);
}

bool sipSafetWorkflow::eventFilter( ::QObject*a0, ::QEvent*a1)
{
    sip_gilstate_t sipGILState;
    PyObject *sipMeth;

    sipMeth = sipIsPyMethod(&sipGILState,&sipPyMethods[5],sipPySelf,NULL,sipName_eventFilter);

    if (!sipMeth)
        return  ::SafetWorkflow::eventFilter(a0,a1);

    extern bool sipVH_Safet_4(sip_gilstate_t, sipVirtErrorHandlerFunc, sipSimpleWrapper *, PyObject *,  ::QObject*, ::QEvent*);

    return sipVH_Safet_4(sipGILState, 0, sipPySelf, sipMeth, a0, a1);
}

bool sipSafetWorkflow::event( ::QEvent*a0)
{
    sip_gilstate_t sipGILState;
    PyObject *sipMeth;

    sipMeth = sipIsPyMethod(&sipGILState,&sipPyMethods[6],sipPySelf,NULL,sipName_event);

    if (!sipMeth)
        return  ::SafetWorkflow::event(a0);

    extern bool sipVH_Safet_5(sip_gilstate_t, sipVirtErrorHandlerFunc, sipSimpleWrapper *, PyObject *,  ::QEvent*);

    return sipVH_Safet_5(sipGILState, 0, sipPySelf, sipMeth, a0);
}

 ::QObject* sipSafetWorkflow::sipProtect_sender() const
{
    return  ::QObject::sender();
}

int sipSafetWorkflow::sipProtect_receivers(const char*a0) const
{
    return  ::QObject::receivers(a0);
}

void sipSafetWorkflow::sipProtectVirt_timerEvent(bool sipSelfWasArg, ::QTimerEvent*a0)
{
    (sipSelfWasArg ?  ::QObject::timerEvent(a0) : timerEvent(a0));
}

void sipSafetWorkflow::sipProtectVirt_childEvent(bool sipSelfWasArg, ::QChildEvent*a0)
{
    (sipSelfWasArg ?  ::QObject::childEvent(a0) : childEvent(a0));
}

void sipSafetWorkflow::sipProtectVirt_customEvent(bool sipSelfWasArg, ::QEvent*a0)
{
    (sipSelfWasArg ?  ::QObject::customEvent(a0) : customEvent(a0));
}

void sipSafetWorkflow::sipProtectVirt_connectNotify(bool sipSelfWasArg,const char*a0)
{
    (sipSelfWasArg ?  ::QObject::connectNotify(a0) : connectNotify(a0));
}

void sipSafetWorkflow::sipProtectVirt_disconnectNotify(bool sipSelfWasArg,const char*a0)
{
    (sipSelfWasArg ?  ::QObject::disconnectNotify(a0) : disconnectNotify(a0));
}

int sipSafetWorkflow::sipProtect_senderSignalIndex() const
{
    return  ::QObject::senderSignalIndex();
}


extern "C" {static PyObject *meth_SafetWorkflow_sender(PyObject *, PyObject *, PyObject *);}
static PyObject *meth_SafetWorkflow_sender(PyObject *sipSelf, PyObject *sipArgs, PyObject *sipKwds)
{
    PyObject *sipParseErr = NULL;

    {
        const sipSafetWorkflow *sipCpp;

        if (sipParseKwdArgs(&sipParseErr, sipArgs, sipKwds, NULL, NULL, "B", &sipSelf, sipType_SafetWorkflow, &sipCpp))
        {
             ::QObject*sipRes = 0;

#line 678 "/usr/share/sip/PyQt4/QtCore/qobject.sip"
        typedef QObject *(*helper_func)(QObject *);
        
        static helper_func helper = 0;
        
        if (!helper)
        {
            helper = (helper_func)sipImportSymbol("qpycore_qobject_sender");
            Q_ASSERT(helper);
        }
        
        // sender() must be called without the GIL to avoid possible deadlocks between
        // the GIL and Qt's internal thread data mutex.
        
        Py_BEGIN_ALLOW_THREADS
        
        #if defined(SIP_PROTECTED_IS_PUBLIC)
        sipRes = sipCpp->sender();
        #else
        sipRes = sipCpp->sipProtect_sender();
        #endif
        
        Py_END_ALLOW_THREADS
        
        sipRes = helper(sipRes);
#line 332 "./sipSafetSafetWorkflow.cpp"

            return sipConvertFromType(sipRes,sipType_QObject,NULL);
        }
    }

    /* Raise an exception if the arguments couldn't be parsed. */
    sipNoMethod(sipParseErr, sipName_SafetWorkflow, sipName_sender, NULL);

    return NULL;
}


extern "C" {static PyObject *meth_SafetWorkflow_receivers(PyObject *, PyObject *, PyObject *);}
static PyObject *meth_SafetWorkflow_receivers(PyObject *sipSelf, PyObject *sipArgs, PyObject *sipKwds)
{
    PyObject *sipParseErr = NULL;

    {
        PyObject * a0;
        const sipSafetWorkflow *sipCpp;

        if (sipParseKwdArgs(&sipParseErr, sipArgs, sipKwds, NULL, NULL, "BP0", &sipSelf, sipType_SafetWorkflow, &sipCpp, &a0))
        {
            int sipRes = 0;
            sipErrorState sipError = sipErrorNone;

#line 706 "/usr/share/sip/PyQt4/QtCore/qobject.sip"
        // We need to take into account any proxies for Python signals. Import the
        // helpers if it hasn't already been done.
        typedef const char *(*pyqt4_get_signal_t)(PyObject *);
        typedef int (*receivers_helper_func)(QObject *, const char *, int);
        
        static pyqt4_get_signal_t pyqt4_get_signal = 0;
        static receivers_helper_func receivers_helper = 0;
        
        if (!pyqt4_get_signal)
        {
            pyqt4_get_signal = (pyqt4_get_signal_t)sipImportSymbol("pyqt4_get_signal");
            Q_ASSERT(pyqt4_get_signal);
        }
        
        if (!receivers_helper)
        {
            receivers_helper = (receivers_helper_func)sipImportSymbol("qpycore_qobject_receivers");
            Q_ASSERT(receivers_helper);
        }
        
        // PyQt5: Get rid of the const casts.
        const char *sig = pyqt4_get_signal(a0);
            
        if (sig)
        {
        #if defined(SIP_PROTECTED_IS_PUBLIC)
            sipRes = receivers_helper(const_cast<QObject *>(sipCpp), sig, sipCpp->receivers(sig));
        #else
            sipRes = receivers_helper(const_cast<QObject *>(static_cast<const QObject *>(sipCpp)), sig, sipCpp->sipProtect_receivers(sig));
        #endif
        }
        else
        {
            sipError = sipBadCallableArg(0, a0);
        }
#line 395 "./sipSafetSafetWorkflow.cpp"

            if (sipError == sipErrorFail)
                return 0;

            if (sipError == sipErrorNone)
            {
            return SIPLong_FromLong(sipRes);
            }

            sipAddException(sipError, &sipParseErr);
        }
    }

    /* Raise an exception if the arguments couldn't be parsed. */
    sipNoMethod(sipParseErr, sipName_SafetWorkflow, sipName_receivers, NULL);

    return NULL;
}


extern "C" {static PyObject *meth_SafetWorkflow_timerEvent(PyObject *, PyObject *, PyObject *);}
static PyObject *meth_SafetWorkflow_timerEvent(PyObject *sipSelf, PyObject *sipArgs, PyObject *sipKwds)
{
    PyObject *sipParseErr = NULL;
    bool sipSelfWasArg = (!sipSelf || sipIsDerivedClass((sipSimpleWrapper *)sipSelf));

    {
         ::QTimerEvent* a0;
        sipSafetWorkflow *sipCpp;

        if (sipParseKwdArgs(&sipParseErr, sipArgs, sipKwds, NULL, NULL, "BJ8", &sipSelf, sipType_SafetWorkflow, &sipCpp, sipType_QTimerEvent, &a0))
        {
            Py_BEGIN_ALLOW_THREADS
            sipCpp->sipProtectVirt_timerEvent(sipSelfWasArg,a0);
            Py_END_ALLOW_THREADS

            Py_INCREF(Py_None);
            return Py_None;
        }
    }

    /* Raise an exception if the arguments couldn't be parsed. */
    sipNoMethod(sipParseErr, sipName_SafetWorkflow, sipName_timerEvent, NULL);

    return NULL;
}


extern "C" {static PyObject *meth_SafetWorkflow_childEvent(PyObject *, PyObject *, PyObject *);}
static PyObject *meth_SafetWorkflow_childEvent(PyObject *sipSelf, PyObject *sipArgs, PyObject *sipKwds)
{
    PyObject *sipParseErr = NULL;
    bool sipSelfWasArg = (!sipSelf || sipIsDerivedClass((sipSimpleWrapper *)sipSelf));

    {
         ::QChildEvent* a0;
        sipSafetWorkflow *sipCpp;

        if (sipParseKwdArgs(&sipParseErr, sipArgs, sipKwds, NULL, NULL, "BJ8", &sipSelf, sipType_SafetWorkflow, &sipCpp, sipType_QChildEvent, &a0))
        {
            Py_BEGIN_ALLOW_THREADS
            sipCpp->sipProtectVirt_childEvent(sipSelfWasArg,a0);
            Py_END_ALLOW_THREADS

            Py_INCREF(Py_None);
            return Py_None;
        }
    }

    /* Raise an exception if the arguments couldn't be parsed. */
    sipNoMethod(sipParseErr, sipName_SafetWorkflow, sipName_childEvent, NULL);

    return NULL;
}


extern "C" {static PyObject *meth_SafetWorkflow_customEvent(PyObject *, PyObject *, PyObject *);}
static PyObject *meth_SafetWorkflow_customEvent(PyObject *sipSelf, PyObject *sipArgs, PyObject *sipKwds)
{
    PyObject *sipParseErr = NULL;
    bool sipSelfWasArg = (!sipSelf || sipIsDerivedClass((sipSimpleWrapper *)sipSelf));

    {
         ::QEvent* a0;
        sipSafetWorkflow *sipCpp;

        if (sipParseKwdArgs(&sipParseErr, sipArgs, sipKwds, NULL, NULL, "BJ8", &sipSelf, sipType_SafetWorkflow, &sipCpp, sipType_QEvent, &a0))
        {
            Py_BEGIN_ALLOW_THREADS
            sipCpp->sipProtectVirt_customEvent(sipSelfWasArg,a0);
            Py_END_ALLOW_THREADS

            Py_INCREF(Py_None);
            return Py_None;
        }
    }

    /* Raise an exception if the arguments couldn't be parsed. */
    sipNoMethod(sipParseErr, sipName_SafetWorkflow, sipName_customEvent, NULL);

    return NULL;
}


extern "C" {static PyObject *meth_SafetWorkflow_connectNotify(PyObject *, PyObject *, PyObject *);}
static PyObject *meth_SafetWorkflow_connectNotify(PyObject *sipSelf, PyObject *sipArgs, PyObject *sipKwds)
{
    PyObject *sipParseErr = NULL;
    bool sipSelfWasArg = (!sipSelf || sipIsDerivedClass((sipSimpleWrapper *)sipSelf));

    {
        PyObject * a0;
        sipSafetWorkflow *sipCpp;

        if (sipParseKwdArgs(&sipParseErr, sipArgs, sipKwds, NULL, NULL, "BP0", &sipSelf, sipType_SafetWorkflow, &sipCpp, &a0))
        {
            sipErrorState sipError = sipErrorNone;

#line 752 "/usr/share/sip/PyQt4/QtCore/qobject.sip"
        typedef const char *(*pyqt4_get_signal_t)(PyObject *);
        
        static pyqt4_get_signal_t pyqt4_get_signal = 0;
        
        if (!pyqt4_get_signal)
        {
            pyqt4_get_signal = (pyqt4_get_signal_t)sipImportSymbol("pyqt4_get_signal");
            Q_ASSERT(pyqt4_get_signal);
        }
        
        const char *sig = pyqt4_get_signal(a0);
            
        if (sig)
        {
        #if defined(SIP_PROTECTED_IS_PUBLIC)
            (sipSelfWasArg ? sipCpp->QObject::connectNotify(sig) : sipCpp->connectNotify(sig));
        #else
            sipCpp->sipProtectVirt_connectNotify(sipSelfWasArg, sig);
        #endif
        }
        else
        {
            sipError = sipBadCallableArg(0, a0);
        }
#line 539 "./sipSafetSafetWorkflow.cpp"

            if (sipError == sipErrorFail)
                return 0;

            if (sipError == sipErrorNone)
            {
            Py_INCREF(Py_None);
            return Py_None;
            }

            sipAddException(sipError, &sipParseErr);
        }
    }

    /* Raise an exception if the arguments couldn't be parsed. */
    sipNoMethod(sipParseErr, sipName_SafetWorkflow, sipName_connectNotify, NULL);

    return NULL;
}


extern "C" {static PyObject *meth_SafetWorkflow_disconnectNotify(PyObject *, PyObject *, PyObject *);}
static PyObject *meth_SafetWorkflow_disconnectNotify(PyObject *sipSelf, PyObject *sipArgs, PyObject *sipKwds)
{
    PyObject *sipParseErr = NULL;
    bool sipSelfWasArg = (!sipSelf || sipIsDerivedClass((sipSimpleWrapper *)sipSelf));

    {
        PyObject * a0;
        sipSafetWorkflow *sipCpp;

        if (sipParseKwdArgs(&sipParseErr, sipArgs, sipKwds, NULL, NULL, "BP0", &sipSelf, sipType_SafetWorkflow, &sipCpp, &a0))
        {
            sipErrorState sipError = sipErrorNone;

#line 785 "/usr/share/sip/PyQt4/QtCore/qobject.sip"
        typedef const char *(*pyqt4_get_signal_t)(PyObject *);
        
        static pyqt4_get_signal_t pyqt4_get_signal = 0;
        
        if (!pyqt4_get_signal)
        {
            pyqt4_get_signal = (pyqt4_get_signal_t)sipImportSymbol("pyqt4_get_signal");
            Q_ASSERT(pyqt4_get_signal);
        }
        
        const char *sig = pyqt4_get_signal(a0);
            
        if (sig)
        {
        #if defined(SIP_PROTECTED_IS_PUBLIC)
            (sipSelfWasArg ? sipCpp->QObject::disconnectNotify(sig) : sipCpp->disconnectNotify(sig));
        #else
            sipCpp->sipProtectVirt_disconnectNotify(sipSelfWasArg, sig);
        #endif
        }
        else
        {
            sipError = sipBadCallableArg(0, a0);
        }
#line 600 "./sipSafetSafetWorkflow.cpp"

            if (sipError == sipErrorFail)
                return 0;

            if (sipError == sipErrorNone)
            {
            Py_INCREF(Py_None);
            return Py_None;
            }

            sipAddException(sipError, &sipParseErr);
        }
    }

    /* Raise an exception if the arguments couldn't be parsed. */
    sipNoMethod(sipParseErr, sipName_SafetWorkflow, sipName_disconnectNotify, NULL);

    return NULL;
}


extern "C" {static PyObject *meth_SafetWorkflow_senderSignalIndex(PyObject *, PyObject *, PyObject *);}
static PyObject *meth_SafetWorkflow_senderSignalIndex(PyObject *sipSelf, PyObject *sipArgs, PyObject *sipKwds)
{
    PyObject *sipParseErr = NULL;

    {
        const sipSafetWorkflow *sipCpp;

        if (sipParseKwdArgs(&sipParseErr, sipArgs, sipKwds, NULL, NULL, "B", &sipSelf, sipType_SafetWorkflow, &sipCpp))
        {
            int sipRes;

            Py_BEGIN_ALLOW_THREADS
            sipRes = sipCpp->sipProtect_senderSignalIndex();
            Py_END_ALLOW_THREADS

            return SIPLong_FromLong(sipRes);
        }
    }

    /* Raise an exception if the arguments couldn't be parsed. */
    sipNoMethod(sipParseErr, sipName_SafetWorkflow, sipName_senderSignalIndex, NULL);

    return NULL;
}


extern "C" {static PyObject *meth_SafetWorkflow_listTasks(PyObject *, PyObject *);}
static PyObject *meth_SafetWorkflow_listTasks(PyObject *sipSelf, PyObject *sipArgs)
{
    PyObject *sipParseErr = NULL;

    {
        bool a0 = 0;
        const  ::QString& a1def = QString("\n");
        const  ::QString* a1 = &a1def;
        int a1State = 0;
         ::SafetWorkflow *sipCpp;

        if (sipParseArgs(&sipParseErr, sipArgs, "B|bJ1", &sipSelf, sipType_SafetWorkflow, &sipCpp, &a0, sipType_QString,&a1, &a1State))
        {
             ::QString*sipRes;

            Py_BEGIN_ALLOW_THREADS
            sipRes = new  ::QString(sipCpp->listTasks(a0,*a1));
            Py_END_ALLOW_THREADS
            sipReleaseType(const_cast< ::QString *>(a1),sipType_QString,a1State);

            return sipConvertFromNewType(sipRes,sipType_QString,NULL);
        }
    }

    /* Raise an exception if the arguments couldn't be parsed. */
    sipNoMethod(sipParseErr, sipName_SafetWorkflow, sipName_listTasks, NULL);

    return NULL;
}


/* Cast a pointer to a type somewhere in its inheritance hierarchy. */
extern "C" {static void *cast_SafetWorkflow(void *, const sipTypeDef *);}
static void *cast_SafetWorkflow(void *sipCppV, const sipTypeDef *targetType)
{
     ::SafetWorkflow *sipCpp = reinterpret_cast< ::SafetWorkflow *>(sipCppV);

    if (targetType == sipType_SafetXmlObject)
        return static_cast< ::SafetXmlObject *>(sipCpp);

    if (targetType == sipType_QObject)
        return static_cast< ::QObject *>(sipCpp);

    return sipCppV;
}


/* Call the instance's destructor. */
extern "C" {static void release_SafetWorkflow(void *, int);}
static void release_SafetWorkflow(void *sipCppV, int)
{
     ::SafetWorkflow *sipCpp = reinterpret_cast< ::SafetWorkflow *>(sipCppV);

    Py_BEGIN_ALLOW_THREADS

    if (QThread::currentThread() == sipCpp->thread())
        delete sipCpp;
    else
        sipCpp->deleteLater();

    Py_END_ALLOW_THREADS
}


extern "C" {static void dealloc_SafetWorkflow(sipSimpleWrapper *);}
static void dealloc_SafetWorkflow(sipSimpleWrapper *sipSelf)
{
    if (sipIsDerivedClass(sipSelf))
        reinterpret_cast<sipSafetWorkflow *>(sipGetAddress(sipSelf))->sipPySelf = NULL;

    if (sipIsOwnedByPython(sipSelf))
    {
        release_SafetWorkflow(sipGetAddress(sipSelf), sipIsDerivedClass(sipSelf));
    }
}


extern "C" {static void *init_type_SafetWorkflow(sipSimpleWrapper *, PyObject *, PyObject *, PyObject **, PyObject **, PyObject **);}
static void *init_type_SafetWorkflow(sipSimpleWrapper *sipSelf, PyObject *sipArgs, PyObject *sipKwds, PyObject **sipUnused, PyObject **, PyObject **sipParseErr)
{
    sipSafetWorkflow *sipCpp = 0;

    {
        if (sipParseKwdArgs(sipParseErr, sipArgs, sipKwds, NULL, sipUnused, ""))
        {
            Py_BEGIN_ALLOW_THREADS
            sipCpp = new sipSafetWorkflow();
            Py_END_ALLOW_THREADS

            sipCpp->sipPySelf = sipSelf;

            return sipCpp;
        }
    }

    return NULL;
}


/* Define this type's super-types. */
static sipEncodedTypeDef supers_SafetWorkflow[] = {{9, 255, 1}};


static PyMethodDef methods_SafetWorkflow[] = {
    {SIP_MLNAME_CAST(sipName_childEvent), (PyCFunction)meth_SafetWorkflow_childEvent, METH_VARARGS|METH_KEYWORDS, NULL},
    {SIP_MLNAME_CAST(sipName_connectNotify), (PyCFunction)meth_SafetWorkflow_connectNotify, METH_VARARGS|METH_KEYWORDS, NULL},
    {SIP_MLNAME_CAST(sipName_customEvent), (PyCFunction)meth_SafetWorkflow_customEvent, METH_VARARGS|METH_KEYWORDS, NULL},
    {SIP_MLNAME_CAST(sipName_disconnectNotify), (PyCFunction)meth_SafetWorkflow_disconnectNotify, METH_VARARGS|METH_KEYWORDS, NULL},
    {SIP_MLNAME_CAST(sipName_listTasks), meth_SafetWorkflow_listTasks, METH_VARARGS, NULL},
    {SIP_MLNAME_CAST(sipName_receivers), (PyCFunction)meth_SafetWorkflow_receivers, METH_VARARGS|METH_KEYWORDS, NULL},
    {SIP_MLNAME_CAST(sipName_sender), (PyCFunction)meth_SafetWorkflow_sender, METH_VARARGS|METH_KEYWORDS, NULL},
    {SIP_MLNAME_CAST(sipName_senderSignalIndex), (PyCFunction)meth_SafetWorkflow_senderSignalIndex, METH_VARARGS|METH_KEYWORDS, NULL},
    {SIP_MLNAME_CAST(sipName_timerEvent), (PyCFunction)meth_SafetWorkflow_timerEvent, METH_VARARGS|METH_KEYWORDS, NULL}
};

static sipEnumMemberDef enummembers_SafetWorkflow[] = {
    {sipName_JSON, static_cast<int>( ::SafetWorkflow::JSON), 8},
    {sipName_SVG, static_cast<int>( ::SafetWorkflow::SVG), 8},
    {sipName_XML, static_cast<int>( ::SafetWorkflow::XML), 8},
};


static pyqt4ClassPluginDef plugin_SafetWorkflow = {
    & ::SafetWorkflow::staticMetaObject,
    0,
    0
};


sipClassTypeDef sipTypeDef_Safet_SafetWorkflow = {
    {
        -1,
        0,
        0,
        SIP_TYPE_SCC|SIP_TYPE_CLASS,
        sipNameNr_SafetWorkflow,
        {0},
        &plugin_SafetWorkflow
    },
    {
        sipNameNr_SafetWorkflow,
        {0, 0, 1},
        9, methods_SafetWorkflow,
        3, enummembers_SafetWorkflow,
        0, 0,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    0,
    -1,
    -1,
    supers_SafetWorkflow,
    0,
    init_type_SafetWorkflow,
    0,
    0,
#if PY_MAJOR_VERSION >= 3
    0,
    0,
#else
    0,
    0,
    0,
    0,
#endif
    dealloc_SafetWorkflow,
    0,
    0,
    0,
    release_SafetWorkflow,
    cast_SafetWorkflow,
    0,
    0,
    0,
    0,
    0,
    0
};
