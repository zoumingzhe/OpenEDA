/* @file  qt_int.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_GUI_QTLINT_H_
#define EDI_GUI_QTLINT_H_

#include <QSocketNotifier>
#include <QTimer>
#include <map>
#include <tcl.h>

namespace open_edi {
namespace gui {

class QtFileHandler;

class QtNotifier : public QObject {
    Q_OBJECT

  public:
    typedef std::map<int, QtFileHandler*> HandlerMap;
    // the key Tcl Notifier functions
    static void  setTimer(Tcl_Time const* timePtr);
    static int   waitForEvent(Tcl_Time const* timePtr);
    static void  createFileHandler(int fd, int mask, Tcl_FileProc* proc, ClientData client_data);
    static void  deleteFileHandler(int fd);
    static void* initNotifier();
    static void  finalizeNotifier(ClientData client_data);
    static void  alertNotifier(ClientData client_data);
    static void  serviceModeHook(int mode);

    static QtNotifier* getInstance();

    static void registerTclNotifier();

  public slots:
    void readReady(int fd);
    void writeReady(int fd);
    void exception(int fd);
    void handleTimer();

  private:
    // singleton
    QtNotifier();
    ~QtNotifier() = default;
    HandlerMap         handlers_;
    QTimer*            timer_;
    static void        doTclCallback_(int type, int fd);
    static QtNotifier* notifier_;
};

class QtFileHandler : public QObject {
    Q_OBJECT

  public:
    QtFileHandler(QObject* parent, Tcl_FileProc* proc, ClientData client_data, int mask) : proc_(proc), clientData_(client_data), mask_(mask) {}
    ~QtFileHandler() { deleteSocketNotifier(); }
    void callbackFileHandler(int type, int fd);
    void bindSocketNotifier(QSocketNotifier* notifier);
    void deleteSocketNotifier();

  private:
    // function to call
    Tcl_FileProc* proc_;
    // extra data to supply
    ClientData clientData_;
    // types of activity supported
    int              mask_;
    QSocketNotifier* notifier_;
};

} // namespace gui
} // namespace open_edi
#endif