/* @file  qt_int.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "qt_int.h"
#include <QCoreApplication>

namespace open_edi {
namespace gui {

// Singleton class implementation trick
QtNotifier* QtNotifier::getInstance() {
    if (!notifier_) {
        notifier_ = new QtNotifier();
    }
    return notifier_;
}

QtNotifier* QtNotifier::notifier_ = nullptr;

QtNotifier::QtNotifier() {
    timer_ = new QTimer(this);
    timer_->setSingleShot(true);
    QObject::connect(timer_, &QTimer::timeout,
                     this, &QtNotifier::handleTimer);
}

// Register notifer to tcl
void QtNotifier::registerTclNotifier() {
    Tcl_NotifierProcs notifier;
    notifier.createFileHandlerProc = createFileHandler;
    notifier.deleteFileHandlerProc = deleteFileHandler;
    notifier.setTimerProc          = setTimer;
    notifier.waitForEventProc      = waitForEvent;
    notifier.initNotifierProc      = initNotifier;
    notifier.finalizeNotifierProc  = finalizeNotifier;
    notifier.alertNotifierProc     = alertNotifier;
    notifier.serviceModeHookProc   = serviceModeHook;
    Tcl_SetNotifier(&notifier);
}

// create tcl file handler
void QtNotifier::createFileHandler(int fd, int mask, Tcl_FileProc* proc, ClientData client_data) {
    // find any existing handler
    HandlerMap::iterator old_handler_it = getInstance()->handlers_.find(fd);
    if (old_handler_it != getInstance()->handlers_.end()) {
        //delete immediately for efficency
        delete (old_handler_it->second);
        // old_handler_it->second->deleteLater();
        getInstance()->handlers_.erase(old_handler_it);
    }

    QtFileHandler*   hdlr = new QtFileHandler(getInstance(), proc, client_data, mask);
    QSocketNotifier* ntf;

    if (mask & TCL_READABLE) {
        ntf = new QSocketNotifier(fd, QSocketNotifier::Read);
        QObject::connect(ntf, &QSocketNotifier::activated,
                         getInstance(), &QtNotifier::readReady);
    }
    if (mask & TCL_WRITABLE) {
        ntf = new QSocketNotifier(fd, QSocketNotifier::Write);
        QObject::connect(ntf, &QSocketNotifier::activated,
                         getInstance(), &QtNotifier::writeReady);
    }

    if (mask & TCL_EXCEPTION) {
        ntf = new QSocketNotifier(fd, QSocketNotifier::Exception);
        QObject::connect(ntf, &QSocketNotifier::activated,
                         getInstance(), &QtNotifier::exception);
    }
    hdlr->bindSocketNotifier(ntf);
    getInstance()->handlers_.insert(std::make_pair(fd, hdlr));
}

// remove the handler according to file descriptor
void QtNotifier::deleteFileHandler(int fd) {
    HandlerMap::iterator old_handler_it = getInstance()->handlers_.find(fd);
    if (old_handler_it != getInstance()->handlers_.end()) {
        //delete immediately for efficency
        delete (old_handler_it->second);
        // old_handler_it->second->deleteLater();
        getInstance()->handlers_.erase(old_handler_it);
    }
}

// find and execute handler according to tcl type and file descriptor
void QtNotifier::doTclCallback_(int type, int fd) {
    // find the handler
    HandlerMap::const_iterator handler_it = getInstance()->handlers_.find(fd);

    if (handler_it == getInstance()->handlers_.end()) {
        printf("could not find a registered file handler for fd=%d\n", fd);
        return;
    }

    handler_it->second->callbackFileHandler(type, fd);
}

void QtNotifier::readReady(int fd) { doTclCallback_(TCL_READABLE, fd); }
void QtNotifier::writeReady(int fd) { doTclCallback_(TCL_WRITABLE, fd); }
void QtNotifier::exception(int fd) { doTclCallback_(TCL_EXCEPTION, fd); }

void QtNotifier::setTimer(Tcl_Time const* timePtr) {
    if (getInstance()->timer_->isActive()) {
        getInstance()->timer_->stop();
    }
    if (timePtr) {
        getInstance()->timer_->start(timePtr->sec * 1000 + timePtr->usec / 1000);
    }
}

void QtNotifier::handleTimer() {
    Tcl_ServiceAll();
}

int QtNotifier::waitForEvent(Tcl_Time const* timePtr) {

    int timeout;
    if (timePtr) {
        timeout = timePtr->sec * 1000 + timePtr->usec / 1000;
        if (timeout == 0) {
            if (!QCoreApplication::hasPendingEvents()) {
                // do not block , return directly
                return 0;
            }
        } else {
            setTimer(timePtr);
        }
    }
    // block until new events come
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents);
    QCoreApplication::sendPostedEvents(0, QEvent::DeferredDelete);
    return 1;
}

void* QtNotifier::initNotifier() { return 0; }
void  QtNotifier::finalizeNotifier(ClientData) {}
void  QtNotifier::alertNotifier(ClientData) {}

void QtNotifier::serviceModeHook(int) {}

void QtFileHandler::callbackFileHandler(int type,
                                          int fd) {
    if (!(mask_ & type)) {
        printf("signal type received (%d) for fd %d should not be active!", type, fd);
        return;
    }
    // execute proc
    (*proc_)(clientData_, type);
}

void QtFileHandler::bindSocketNotifier(QSocketNotifier* notifier) {
    notifier_ = notifier;
}

void QtFileHandler::deleteSocketNotifier() {
    delete notifier_;
}

} // namespace gui
} // namespace open_edi