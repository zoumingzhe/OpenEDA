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

class QtNotifier: public QObject
{
    Q_OBJECT

    public:
    typedef std::map<int, QtFileHandler*> HandlerMap;
    // the key Tcl Notifier functions
    static void setTimer(Tcl_Time const* timePtr);
    static int  waitForEvent(Tcl_Time const* timePtr);
    static void createFileHandler(int fd, int mask, Tcl_FileProc* proc, ClientData clientData);
    static void deleteFileHandler(int fd);
    static void* initNotifier();
    static void finalizeNotifier(ClientData clientData);
     static void alertNotifier(ClientData clientData);
    static void serviceModeHook(int mode);

    static QtNotifier* getInstance();

    static void registerTclNotifier();

    public slots:
    void readReady(int fd);
    void writeReady(int fd);
    void exception(int fd);
    void handle_timer();

    private:
    // singleton
    QtNotifier();    
    ~QtNotifier() = default;
    HandlerMap handlers_;
    QTimer* timer_;
    static void do_tcl_callback_(int type,int fd);                   
    static QtNotifier* notifier_;    

};

  class QtFileHandler: public QObject
  {
    Q_OBJECT

  public:
    QtFileHandler(QObject * parent, Tcl_FileProc* proc, ClientData clientData, int mask):proc_(proc), clientData_(clientData), mask_(mask){}
    ~QtFileHandler()
    {delete_socket_notifier(); }
    void callback_file_handler(int type, int fd);
    void bind_socket_notifier(QSocketNotifier* notifier);
    void delete_socket_notifier();

  private:
    // function to call
    Tcl_FileProc* proc_;     
    // extra data to supply 
    ClientData clientData_;   
    // types of activity supported
    int mask_;    
    QSocketNotifier* notifier_;            
  };

} // namespace gui
} // namespace open_edi
#endif