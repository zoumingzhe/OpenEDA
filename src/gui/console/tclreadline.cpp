 /* ================================================================== *
  * FILE: tclreadline.c
  * $Id: a81d2fca2303840fd4166b846c1babe8740f9f23 $
  * ---
  * tclreadline -- gnu readline for tcl
  * https://github.com/flightaware/tclreadline/
  * Copyright (c) 1998 - 2014, Johannes Zellner <johannes@zellner.org>
  * This software is copyright under the BSD license.
  * ================================================================== */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <tcl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined (READLINE_LIBRARY)
#  include <readline.h>
#  include <history.h>
#else
#  include <readline/readline.h>
#  include <readline/history.h>
#endif

#include "tclreadline.h"
#include <map>
#include <string>


/*
 * this prototype may be missing
 * in readline.h
 */
void rl_extend_line_buffer(int len);
extern int rl_display_fixed;
extern int _rl_completion_prefix_display_length;

namespace open_edi {
namespace gui {


#ifdef EXECUTING_MACRO_HACK
/**
 * this prototype may be private in readline's file `macro.c'.
 * We need it here to decide, if we should read more
 * characters from a macro. Dirty, but it should work.
 */
extern char* EXECUTING_MACRO_NAME;
#endif


static const char* tclrl_library = (const char*)NULL;
static const char* tclrl_version_str = TCLRL_VERSION_STR;
static const char* tclrl_patchlevel_str = TCLRL_PATCHLEVEL_STR;
static const char* tclrl_historypath_str = (const char*)NULL;



#define MALLOC(size) malloc(size)
#define FREE(ptr) (free(ptr), ptr = NULL)

enum {
    _CMD_SET = (1 << 0),
    _CMD_GET = (1 << 1)
};


typedef struct cmds_t {
    struct cmds_t* prev;
    char**         cmd;
    struct cmds_t* next;
} cmds_t;


// struct for muti-lines support
typedef struct mutilines_status {
    u_int32_t   rl_end;
    u_int32_t   rl_point;
    char        rl_line_buffer[0xff];
    char        prompt[0xff];
} mutilines_status;


#define MOVE_CURSOR_UP(count)      printf("\033[%dA",count)
#define MOVE_CURSOR_DOWN(count)    printf("\033[%dB",count)


#define ISWHITE(c) ((' ' == c) || ('\t' == c) || ('\n' == c))

/**
 * forward declarations
 */
static char*  stripleft(char* in);
static char*  stripright(char* in);
static char*  stripwhite(char* in);
static int    TclReadlineLineComplete(void);
static void   TclReadlineTerminate(int state);
static char*  TclReadlineQuote(char* text, char* quotechars);
static int    TclReadlineCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                             Tcl_Obj *CONST objv[]);
static void   TclReadlineReadHandler(ClientData clientData, int mask);
static void   TclReadlineLineCompleteHandler(char* ptr);
static int    TclReadlineInitialize(Tcl_Interp* interp, char* historyfile);
static int    blank_line(char* str);
static char** TclReadlineCompletion(char* text, int start, int end);
static char*  TclReadline0generator(char* text, int state);
static char*  TclReadlineKnownCommands(char* text, int state, int mode);
static int    TclReadlineParse(char** args, int maxargs, char* buf);

/**
 *  muti-line support  
 */

static void rebindKeyFunction(void);
static void restoreKeyFunction(void);
static void restoreLineStatus(void);
static void restoreRawLineBuffer(void);
static void redisplayCommands(void);
static void saveStatusToMap(uint32_t index, mutilines_status& status);
static void saveRawLineBuffer(void);
static void saveLineStatus(int end, int point, char* buf);
static void setLineStatus(mutilines_status& status);
static int  get_y_or_n (int for_pager);
static int  getCurrentLineIndex(void);
static int  keyUp (int count, int key);
static int  keyDown (int count, int key);
static int  KeyRubout (int, int);
static void mutilinesCompleteFunction (char ** matches, int len , int max);




enum {
    LINE_PENDING  = -1,
    LINE_EOF      = (1 << 8),
    LINE_COMPLETE = (1 << 9)
};

/**
 * global variables
 */
static int   tclrl_state                 = TCL_OK;
static char* tclrl_eof_string            = (char*) NULL;
static char* tclrl_custom_completer      = (char*) NULL;
static char* tclrl_last_line             = (char*) NULL;
static int   tclrl_use_builtin_completer = 1;
static int   tclrl_history_length        = -1;


Tcl_Interp*  tclrl_interp                = (Tcl_Interp*) NULL;

/**
 * muti-lines support variables
 */
static std::map<int,mutilines_status>       mutiline_map;
static char*                                raw_readline_buf            = nullptr;
static mutilines_status                     muti_lines_status;
static int                                  muti_lines_size             = 0;
static int                                  cursor_index                = 0;

static const char* tclrl_license =
"   Copyright (c) 1998 - 2000, Johannes Zellner <johannes@zellner.org>\n"
"   All rights reserved.\n"
"   \n"
"   Redistribution and use in source and binary forms, with or without\n"
"   modification, are permitted provided that the following conditions\n"
"   are met:\n"
"   \n"
"     * Redistributions of source code must retain the above copyright\n"
"       notice, this list of conditions and the following disclaimer.\n"
"     * Redistributions in binary form must reproduce the above copyright\n"
"       notice, this list of conditions and the following disclaimer in the\n"
"       documentation and/or other materials provided with the distribution.\n"
"     * Neither the name of Johannes Zellner nor the names of contributors\n"
"       to this software may be used to endorse or promote products derived\n"
"       from this software without specific prior written permission.\n"
"       \n"
"   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS\n"
"   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT\n"
"   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR\n"
"   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR\n"
"   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,\n"
"   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,\n"
"   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR\n"
"   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF\n"
"   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING\n"
"   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS\n"
"   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.";

static int  getCurrentLineIndex(void)
{
    return muti_lines_size - cursor_index;
}

static void saveStatusToMap(uint32_t index, mutilines_status& status)
{
    mutiline_map[index] = status;
}

static int keyUp (int count, int key)
{
    if(cursor_index < muti_lines_size)
    {
        //save the last line status 
        if (mutiline_map.find(muti_lines_size) == mutiline_map.end()) {
            memset(muti_lines_status.prompt,0,0xff);
            strcpy(muti_lines_status.prompt,Tcl_GetVar(tclrl_interp,"prompt2",TCL_GLOBAL_ONLY));
            saveStatusToMap(muti_lines_size,muti_lines_status);           
        }   
        MOVE_CURSOR_UP(1);
        cursor_index++;
        setLineStatus(mutiline_map[muti_lines_size - cursor_index]);
    }
    return 0;
}

static int keyDown (int count, int key)
{
    if(cursor_index > 0){   
        MOVE_CURSOR_DOWN(1);
        cursor_index--;
        setLineStatus(mutiline_map[muti_lines_size - cursor_index]);
    }
    return 0;
}

static int  KeyRubout (int count, int key)
{
    auto cur_index = getCurrentLineIndex();
    if (rl_end == 0 && 
        rl_point == rl_end && 
        (cur_index != muti_lines_size) &&
        (cur_index)
        ) {
        for(int i = cur_index; i < muti_lines_size; i++) {
            mutiline_map[i] = mutiline_map[i + 1];
            rl_line_buffer = mutiline_map[i].rl_line_buffer ;
            rl_end = strlen(rl_line_buffer);
            rl_point = rl_end;
            rl_refresh_line(rl_point,rl_end);
            MOVE_CURSOR_DOWN(1);
        }

        //clear the lastest line
        auto it =mutiline_map.find(muti_lines_size);
        if(it != mutiline_map.end()) {
            memset(mutiline_map[muti_lines_size].rl_line_buffer,0,0xff);
            rl_line_buffer = mutiline_map[muti_lines_size].rl_line_buffer;
            rl_set_prompt("");
            rl_refresh_line(rl_point,rl_end);            
            mutiline_map.erase(it);
        }

        //move back and redisplay
        muti_lines_size -= 1;
        cur_index = getCurrentLineIndex();
        cursor_index = muti_lines_size - cur_index;
        auto step =  cursor_index + 1;
        MOVE_CURSOR_UP(step);
        auto status = mutiline_map[cur_index];
        rl_set_prompt(status.prompt);
        rl_line_buffer = status.rl_line_buffer;
        rl_point = status.rl_point;
        rl_end = status.rl_end;
        rl_refresh_line(rl_point,rl_end);  
    }
    else
        return (rl_rubout (count, key));
    return 0;
}

static void saveRawLineBuffer(void)
{
    if(rl_line_buffer) raw_readline_buf = rl_line_buffer;
}

static void restoreRawLineBuffer(void)
{
    if(raw_readline_buf) {
        memset(raw_readline_buf,0,0xff);
        rl_line_buffer = raw_readline_buf;
    }   
}

static void setLineStatus(mutilines_status& status)
{
    rl_set_prompt(status.prompt);
    rl_line_buffer = status.rl_line_buffer ;
    rl_end = strlen(status.rl_line_buffer);
    rl_point = rl_end;
    rl_refresh_line(rl_point,rl_end);
}

static void saveLineStatus(int end, int point, char* buf)
{
    muti_lines_status.rl_end = end;
    muti_lines_status.rl_point = point;
    memset(muti_lines_status.rl_line_buffer,0,0xff);
    strcpy(muti_lines_status.rl_line_buffer,buf);
}


static void rebindKeyFunction(void)
{
#if defined (__MSDOS__)
  rl_bind_keyseq_in_map ("\033[0A",keyUp,rl_get_keymap());
  rl_bind_keyseq_in_map ("\033[0D",keyDown,rl_get_keymap());
#elif defined (__MINGW32__)
  rl_bind_keyseq_in_map ("\340H",keyUp,rl_get_keymap());
  rl_bind_keyseq_in_map ("\340P",keyDown,rl_get_keymap());
#else
  rl_bind_keyseq_in_map("\033[A",keyUp,rl_get_keymap());
  rl_bind_keyseq_in_map("\033[B",keyDown,rl_get_keymap());
#endif
  rl_bind_key_in_map(CTRL('H'),KeyRubout,rl_get_keymap());
  rl_bind_key_in_map(RUBOUT,KeyRubout,rl_get_keymap());
}
static void
restoreKeyFunction(void)
{
#if defined (__MSDOS__)
  rl_bind_keyseq_in_map ("\033[0A",rl_get_previous_history,rl_get_keymap());
  rl_bind_keyseq_in_map ("\033[0D",rl_get_next_history,rl_get_keymap());
#elif defined (__MINGW32__)
  rl_bind_keyseq_in_map ("\340H",rl_get_previous_history,rl_get_keymap());
  rl_bind_keyseq_in_map ("\340P",rl_get_next_history,rl_get_keymap());
#else
  rl_bind_keyseq_in_map("\033[A",rl_get_previous_history,rl_get_keymap());
  rl_bind_keyseq_in_map("\033[B",rl_get_next_history,rl_get_keymap());
#endif
  rl_bind_key_in_map(CTRL('H'),rl_rubout,rl_get_keymap());
  rl_bind_key_in_map(RUBOUT,rl_rubout,rl_get_keymap());
}

static void restoreLineStatus(void)
{
    if(muti_lines_size)
    {
        rl_completion_display_matches_hook = nullptr;
        //re-write the command line
        for (int i = cursor_index; i > 0; i--) {
            printf("%s%s\n",mutiline_map[muti_lines_size - i + 1].prompt, mutiline_map[muti_lines_size - i + 1].rl_line_buffer);
        }
        //reset status
        cursor_index = 0;
        restoreRawLineBuffer();
        rl_end = 0;
        rl_point = 0;
        mutiline_map.clear();
        restoreKeyFunction();
        muti_lines_size = 0;
    }
}


/* The user must press "y" or "n". Non-zero return means "y" pressed. */
static int get_y_or_n (int for_pager)
{
  int c;

  /* For now, disable pager in callback mode, until we later convert to state
     driven functions.  Have to wait until next major version to add new
     state definition, since it will change value of RL_STATE_DONE. */
#if defined (READLINE_CALLBACKS)
  if (RL_ISSTATE (RL_STATE_CALLBACK))
    return 1;
#endif

  for (;;)
    {
      RL_SETSTATE(RL_STATE_MOREINPUT);
      c = rl_read_key ();
      RL_UNSETSTATE(RL_STATE_MOREINPUT);

      if (c == 'y' || c == 'Y' || c == ' ')
	return (1);
      if (c == 'n' || c == 'N' || c == RUBOUT)
	return (0);
      if (for_pager && (c == NEWLINE || c == RETURN))
	return (2);
      if (for_pager && (c == 'q' || c == 'Q'))
	return (0);
      rl_ding ();
    }    
}

static void mutilinesCompleteFunction (char ** matches, int len , int max)
{
  /* If there are many items, then ask the user if she really wants to
     see them all. */
    if (rl_completion_query_items > 0 && len >= rl_completion_query_items)
    {
        rl_crlf ();
        fprintf (rl_outstream, "Display all %d possibilities? (y or n)", len);
        fflush (rl_outstream);
        if (get_y_or_n (0) == 0)
        {
        rl_crlf ();
        rl_forced_update_display ();
        rl_display_fixed = 1;
        return;
        }
    }
    rl_display_match_list (matches, len, max);

    for(auto &t : mutiline_map){
        if (mutiline_map.find(muti_lines_size) == mutiline_map.end()) {
            printf("%s%s\n",t.second.prompt, t.second.rl_line_buffer);
        }
        else {
            if (t.first != muti_lines_size) {
                 printf("%s%s\n",t.second.prompt, t.second.rl_line_buffer);
            }
        }              
    }
    rl_forced_update_display ();
    rl_display_fixed = 1;


    //reset cursor position
    auto current_line = getCurrentLineIndex();
    if (current_line != muti_lines_size)
    {
        cursor_index = muti_lines_size - current_line;
        auto move = cursor_index;
        // redraw lastline
        rl_line_buffer = mutiline_map[muti_lines_size].rl_line_buffer;
        rl_refresh_line(0,0);
        // move cursor back
        MOVE_CURSOR_UP(move);
        rl_line_buffer = mutiline_map[current_line].rl_line_buffer;
        rl_end = strlen(rl_line_buffer);
        rl_point = rl_end;
    }
}

static void  redisplayCommands(void)
{
    auto current_line = getCurrentLineIndex();

    saveStatusToMap(current_line,muti_lines_status);

    if(mutiline_map.find(muti_lines_size) != mutiline_map.end()) {
        for (int i = muti_lines_size; i > getCurrentLineIndex(); i--) {
            mutiline_map[i + 1] =  mutiline_map[i];
        }
    }
    auto prompt = Tcl_GetVar(tclrl_interp,"prompt2",TCL_GLOBAL_ONLY);
    

    //insert new  line to map
    mutilines_status tmp;
    memset(&tmp,0,sizeof(tmp));
    strcpy(tmp.prompt,prompt);
    current_line = getCurrentLineIndex()+1;
    saveStatusToMap(current_line,tmp);

    //overwrite commands
    rl_set_prompt(prompt);
    for(auto &t : mutiline_map) {
        if (t.first >= current_line) {
            rl_line_buffer = t.second.rl_line_buffer;
            rl_end = strlen(rl_line_buffer);
            rl_point = rl_end;
            rl_refresh_line(0,0);
            printf("\n");
        }
    }

    //reset cursor position
    cursor_index = muti_lines_size - current_line + 1;
    auto move = cursor_index+1;
    MOVE_CURSOR_UP(move);
    rl_line_buffer = mutiline_map[current_line].rl_line_buffer ;
    rl_end = strlen(rl_line_buffer);
    rl_point = rl_end;

}

static char* stripleft(char* in)
{
    char* ptr = in;
    while (*ptr && *ptr <= ' ')
        ptr++;
    if (in != ptr)
        memmove(in, ptr, strlen(ptr) + 1);
    return in;
}

static char* stripright(char* in)
{
    char* ptr;
    for (ptr = strchr(in, '\0') - 1; ptr >= in && *ptr <= ' '; ptr--)
        *ptr = '\0';
    return in;
}

static char* stripwhite(char* in)
{
    stripleft(in);
    stripright(in);
    return in;
}

static int TclReadlineLineComplete(void)
{
    return !(tclrl_state == LINE_PENDING);
}

static void TclReadlineTerminate(int state)
{
    tclrl_state = state;
    rl_callback_handler_remove();
}

static char* TclReadlineQuote(char* text, const char* quotechars)
{
    char* ptr;
    char* result_c;
    int i, len = strlen(quotechars);
    Tcl_DString result;

    Tcl_DStringInit(&result);
    for (ptr = text; ptr && *ptr; ptr++) {
        for (i = 0; i < len; i++) {
            if (quotechars[i] == *ptr) {
                Tcl_DStringAppend(&result, "\\", 1);
                break;
            }
        }
        Tcl_DStringAppend(&result, ptr, 1);
    }
    result_c = strdup(Tcl_DStringValue(&result));
    return result_c;
}

static int TclTestCmd(ClientData clientData, Tcl_Interp *interp, int objc,
               Tcl_Obj *CONST objv[])
{
    printf("test........");

    return TCL_OK;
}


static int TclReadlineCmd(ClientData clientData, Tcl_Interp *interp, int objc,
               Tcl_Obj *CONST objv[])
{
    int obj_idx, status,cmdlen;
    char* cmd;
    static bool first_time = true;

    static const char *subCmds[] = {
        "read", "initialize", "write", "add", "complete",
        "customcompleter", "builtincompleter", "eofchar",
        "reset-terminal", "bell", "text", "update",
        (char *) NULL
    };
    enum SubCmdIdx {
        TCLRL_READ, TCLRL_INITIALIZE, TCLRL_WRITE, TCLRL_ADD, TCLRL_COMPLETE,
        TCLRL_CUSTOMCOMPLETER, TCLRL_BUILTINCOMPLETER, TCLRL_EOFCHAR,
        TCLRL_RESET_TERMINAL, TCLRL_BELL, TCLRL_TEXT, TCLRL_UPDATE
    };
    
    Tcl_ResetResult(interp); /* clear the result space */

    if (objc < 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "option ?arg arg ...?");
        return TCL_ERROR;
    }

    status = Tcl_GetIndexFromObj
        (interp, objv[1], subCmds, "option", 0, (int *) &obj_idx);

    if (status != TCL_OK)
        return status;
    // printf("command %s\n", subCmds[obj_idx]);
    switch (obj_idx) {

        case TCLRL_READ:

            rl_callback_handler_install(
                       objc == 3 ? Tcl_GetStringFromObj(objv[2], 0)
                       : "% ", TclReadlineLineCompleteHandler);
            if(first_time)
            {
                first_time = false;
                saveRawLineBuffer();
            }
            Tcl_CreateFileHandler(0, TCL_READABLE,
                TclReadlineReadHandler, (ClientData) NULL);
 
            /**
             * Main Loop.
             * XXX each modification of the global variables
             *     which terminates the main loop must call
             *     rl_callback_handler_remove() to leave
             *     readline in a defined state.          XXX
             */
            tclrl_state = LINE_PENDING;

            while (!TclReadlineLineComplete()) {
#ifdef EXECUTING_MACRO_NAME
                /**
                 * check first, if more characters are
                 * available from _rl_executing_macro,
                 * because Tcl_DoOneEvent() will (naturally)
                 * not detect this `event'.
                 */
                if (EXECUTING_MACRO_NAME)
                    TclReadlineReadHandler((ClientData) NULL, TCL_READABLE);
                else
#endif
                    Tcl_DoOneEvent(TCL_ALL_EVENTS);

            }
            
            Tcl_DeleteFileHandler(0);

            switch (tclrl_state) {

                case LINE_COMPLETE:
                    return TCL_OK;
                    /* NOTREACHED */
                    break;

                case LINE_EOF:
                    if (tclrl_eof_string)
                    return Tcl_Eval(interp, tclrl_eof_string);
                    else
                    return TCL_OK;
                    /* NOTREACHED */
                    break;

                default:
                    return tclrl_state;
                    /* NOTREACHED */
                    break;
            }
            break;

        case TCLRL_INITIALIZE:
            if (3 != objc) {
                Tcl_WrongNumArgs(interp, 2, objv, "historyfile");
                return TCL_ERROR;
            } else {
                return TclReadlineInitialize(interp,
                                 Tcl_GetStringFromObj(objv[2], 0));
            }
            break;

        case TCLRL_WRITE:
            // if (3 != objc) {
            //     Tcl_WrongNumArgs(interp, 2, objv, "historyfile");
            //     return TCL_ERROR;
            // }  else if (write_history(Tcl_GetStringFromObj(objv[2], 0))) {
            //     Tcl_AppendResult(interp, "unable to write history to `",
            //         Tcl_GetStringFromObj(objv[2], 0), "'\n", (char*) NULL);
            //     return TCL_ERROR;
            // }
            // if (tclrl_history_length >= 0) {
            //     history_truncate_file(Tcl_GetStringFromObj(objv[2], 0),
            //                   tclrl_history_length);
            // }
            return TCL_OK;
            break;

        case TCLRL_ADD:
            if (3 != objc) {
                Tcl_WrongNumArgs(interp, 2, objv, "completerLine");
                return TCL_ERROR;
            } else if (TclReadlineKnownCommands(
                         Tcl_GetStringFromObj(objv[2], 0),
                         (int) 0, _CMD_SET)) {
                Tcl_AppendResult(interp, "unable to add command \"",
                    Tcl_GetStringFromObj(objv[2], 0), "\"\n", (char*) NULL);
            }
            break;

        case TCLRL_COMPLETE:                                  
            if (3 != objc) {
                Tcl_WrongNumArgs(interp, 2, objv, "line");
                return TCL_ERROR;
            } else 
            {
                cmd = Tcl_GetStringFromObj(objv[2], 0);
                if (Tcl_CommandComplete(cmd)) {
                    
                    cmdlen = strlen(cmd);
                    if(cmdlen) {
                        if( 0x5c == cmd[cmdlen-1] ) {
                            rl_completion_display_matches_hook = mutilinesCompleteFunction;
                            memset(muti_lines_status.prompt,0,0xff);
                            if(muti_lines_size == 0)
                            {
                                strcpy(muti_lines_status.prompt,Tcl_GetVar(tclrl_interp,"prompt",TCL_GLOBAL_ONLY)); 
                                rebindKeyFunction();
                            }
                            else{
                                strcpy(muti_lines_status.prompt,Tcl_GetVar(tclrl_interp,"prompt2",TCL_GLOBAL_ONLY));
                            }
                            if (getCurrentLineIndex() != muti_lines_size) {
                                redisplayCommands();
                            }
                            else{
                                saveStatusToMap(muti_lines_size,muti_lines_status);
                                restoreRawLineBuffer(); 
                            }
                            muti_lines_size++;                        
                            Tcl_AppendResult(interp, "0", (char*) NULL);
                        }
                        else {
                            restoreLineStatus();
                            Tcl_AppendResult(interp, "1", (char*) NULL);
                        } 
                    }
                    else {
                       restoreLineStatus();
                       Tcl_AppendResult(interp, "1", (char*) NULL);
                    }   
                } 
                else {
                    if(muti_lines_size) {
                        memset(muti_lines_status.prompt,0,0xff);
                        strcpy(muti_lines_status.prompt,Tcl_GetVar(tclrl_interp,"prompt2",TCL_GLOBAL_ONLY));
                        if (getCurrentLineIndex() != muti_lines_size) {
                            redisplayCommands();
                        }
                        else {
                            saveStatusToMap(muti_lines_size,muti_lines_status);
                            restoreRawLineBuffer(); 
                        }
                        muti_lines_size++;
                    }
                    Tcl_AppendResult(interp, "0", (char*) NULL);
                }
            }
            break;

        case TCLRL_CUSTOMCOMPLETER:
            if (objc > 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "?scriptCompleter?");
                return TCL_ERROR;
            } else if (3 == objc) {
                if (tclrl_custom_completer)
                    FREE(tclrl_custom_completer);
                if (!blank_line(Tcl_GetStringFromObj(objv[2], 0)))
                    tclrl_custom_completer =
                         stripwhite(strdup(Tcl_GetStringFromObj(objv[2], 0)));
            }
            Tcl_AppendResult(interp, tclrl_custom_completer, (char*) NULL);
            break;

        case TCLRL_BUILTINCOMPLETER:
            if (objc > 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "?boolean?");
                return TCL_ERROR;
            } else if (3 == objc) {
                int b_use = tclrl_use_builtin_completer;
                if (TCL_OK != Tcl_GetBoolean(interp,
                                 Tcl_GetStringFromObj(objv[2], 0),
                                 &b_use)) {
                    Tcl_AppendResult(interp,
                        "wrong # args: should be a boolean value.",
                        (char*) NULL);
                    return TCL_ERROR;
                } else {
                    tclrl_use_builtin_completer = b_use;
                }
            }
            Tcl_AppendResult(interp, tclrl_use_builtin_completer ? "1" : "0",
                (char*) NULL);
            break;

        case TCLRL_EOFCHAR:
            if (objc > 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "?script?");
                return TCL_ERROR;
            } else if (3 == objc) {
                if (tclrl_eof_string)
                    FREE(tclrl_eof_string);
                if (!blank_line(Tcl_GetStringFromObj(objv[2], 0)))
                    tclrl_eof_string =
                        stripwhite(strdup(Tcl_GetStringFromObj(objv[2], 0)));
            }
            Tcl_AppendResult(interp, tclrl_eof_string, (char*) NULL);
            break;

        case TCLRL_RESET_TERMINAL:
            /* TODO: add this to the completer */
            if (objc > 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "?terminal-name?");
                return TCL_ERROR;
            }
            if (3 == objc) {
                /*
                 * - tcl8.0 doesn't have Tcl_GetStringFromObj()
                 * - rl_reset_terminal() might be defined
                 *   to take no arguments. This might produce
                 *   a compiler warning.
                 */
                rl_reset_terminal(Tcl_GetStringFromObj(objv[2], 0));
#ifdef CLEANUP_AFER_SIGNAL
            } else {
                rl_cleanup_after_signal();
#endif
            }
            break;

        case TCLRL_BELL:
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, "");
                return TCL_ERROR;
            }


            /*
             * ring the terminal bell obeying the current
             * settings -- audible or visible.
             */

            rl_ding();
            break;

        case TCLRL_UPDATE:
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, "");
                return TCL_ERROR;
            }

            /* Update the input line */

            if (rl_line_buffer) {
                rl_forced_update_display();
            }

            break;


        case TCLRL_TEXT:
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, "");
                return TCL_ERROR;
            }

            /* Return the current input line */
            Tcl_SetObjResult(interp,
               Tcl_NewStringObj(rl_line_buffer ? rl_line_buffer : "", -1));
            break;

        default:
            goto BAD_COMMAND;
            /* NOTREACHED */
            break;
    }

    return TCL_OK;

BAD_COMMAND:
    Tcl_AppendResult(interp,
        "wrong # args: should be \"readline option ?arg ...?\"",
        (char*) NULL);
    return TCL_ERROR;

}

static void TclReadlineReadHandler(ClientData clientData, int mask)
{
    if (mask & TCL_READABLE) {
#ifdef EXECUTING_MACRO_NAME
        do {
#endif
            saveLineStatus(rl_end,rl_point,rl_line_buffer);
            rl_callback_read_char();
            
#ifdef EXECUTING_MACRO_NAME
        /**
         * check, if we're inside a macro and
         * if so, read all macro characters
         * until the next eol.
         */
        } while (EXECUTING_MACRO_NAME && !TclReadlineLineComplete());
#endif
    }
}

static void TclReadlineLineCompleteHandler(char* ptr)
{

    Tcl_ResetResult(tclrl_interp); /* clear result space */

    if (!ptr) { /* <c-d> */

        TclReadlineTerminate(LINE_EOF);

    } else {

        /**
         * From version 0.9.3 upwards, all lines are
         * returned, even empty lines. (Only non-empty
         * lines are stuffed in readline's history.)
         * The calling script is responsible for handling
         * empty strings.
         */

        char* expansion = (char*) NULL;
        int status = history_expand(ptr, &expansion);

        if (status >= 2) {
            /* TODO: make this a valid tcl output */
            printf("%s\n", expansion);
            FREE(ptr);
            FREE(expansion);
            return;
        } else if (status <= -1) {
            Tcl_AppendResult
                (tclrl_interp, "error in history expansion: ", expansion, "\n", (char*) NULL);
                TclReadlineTerminate(TCL_ERROR);
            FREE(ptr);
            FREE(expansion);
            return;
        } else {
            if(muti_lines_size){
                for(auto &t : mutiline_map){              
                    Tcl_AppendResult(tclrl_interp, t.second.rl_line_buffer, (char*) NULL);
                    if(t.first != muti_lines_size)
                    Tcl_AppendResult(tclrl_interp, "\n", (char*) NULL);
                }
                auto it = mutiline_map.find(muti_lines_size);
                if (it == mutiline_map.end()) {
                    // Tcl_AppendResult(tclrl_interp, "\n", (char*) NULL);
                    Tcl_AppendResult(tclrl_interp, expansion, (char*) NULL); 
                }    
            }
            else{
                Tcl_AppendResult(tclrl_interp, expansion, (char*) NULL);   
            }    
        }

    #ifdef EXECUTING_MACRO_NAME
        /**
         * don't stuff macro lines
         * into readline's history.
         */
        if(!EXECUTING_MACRO_NAME) {
    #endif
            /**
             * don't stuff empty lines
             * into readline's history.
             * don't stuff twice the same
             * line into readline's history.
             */
            if (expansion && *expansion && (!tclrl_last_line ||
                    strcmp(tclrl_last_line, expansion))) {
                add_history(expansion);
            }
            if (tclrl_last_line)
                FREE(tclrl_last_line);
            tclrl_last_line = strdup(expansion);
    #ifdef EXECUTING_MACRO_NAME
        }
    #endif
        /**
         * tell the calling routines to terminate.
         */
        TclReadlineTerminate(LINE_COMPLETE);
        FREE(ptr);
        FREE(expansion);
    }
}
void TclsetHistoryPath(const char* historyPath)
{
     tclrl_historypath_str = historyPath;
     tclrl_library = historyPath;
}
int Tclreadline_SafeInit(Tcl_Interp *interp)
{
    return Tclreadline_Init(interp);
}

int Tclreadline_Init(Tcl_Interp *interp)
{
    int status;
 #ifdef USE_TCL_STUBS
     Tcl_InitStubs(interp, "8.6", 0);
#endif
    Tcl_CreateObjCommand(interp, "::tclreadline::readline", TclReadlineCmd,
        (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateObjCommand(interp, "test_tk", TclTestCmd,
    (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    tclrl_interp = interp;
    if (TCL_OK != (status = Tcl_LinkVar(interp, "::tclreadline::historyLength",
            (char*) &tclrl_history_length, TCL_LINK_INT)))
        return status;
    if (TCL_OK != (status = Tcl_LinkVar(interp, "::tclreadline::mutilineIndex",
            (char*) &muti_lines_size, TCL_LINK_INT)))
        return status;
    if (TCL_OK != (status = Tcl_LinkVar(interp, "::tclreadline::currentLineIndex",
        (char*) &cursor_index, TCL_LINK_INT)))
    return status;

    if (TCL_OK != (status = Tcl_LinkVar(interp, "::tclreadline::library",
            (char*) &tclrl_library, TCL_LINK_STRING | TCL_LINK_READ_ONLY)))
        return status;
    if (TCL_OK != (status = Tcl_LinkVar(interp, "::tclreadline::version",
            (char*) &tclrl_version_str, TCL_LINK_STRING | TCL_LINK_READ_ONLY)))
        return status;
    if (TCL_OK != (status = Tcl_LinkVar(interp, "::tclreadline::patchLevel",
            (char*) &tclrl_patchlevel_str, TCL_LINK_STRING | TCL_LINK_READ_ONLY)))
        return status;
    if (TCL_OK != (status = Tcl_LinkVar(interp, "::tclreadline::license",
            (char*) &tclrl_license, TCL_LINK_STRING | TCL_LINK_READ_ONLY)))
        return status;

    if (TCL_OK != (status = Tcl_LinkVar(interp, "tclreadline_library",
            (char*) &tclrl_library, TCL_LINK_STRING | TCL_LINK_READ_ONLY)))
        return status;
    if (TCL_OK != (status = Tcl_LinkVar(interp, "tclreadline_version",
            (char*) &tclrl_version_str, TCL_LINK_STRING | TCL_LINK_READ_ONLY)))
        return status;
    if (TCL_OK != (status = Tcl_LinkVar(interp, "tclreadline_patchLevel",
            (char*) &tclrl_patchlevel_str, TCL_LINK_STRING | TCL_LINK_READ_ONLY)))
        return status;
    if (TCL_OK != (status = Tcl_LinkVar(interp, "::tclreadline::historyPath",
        (char*) &tclrl_historypath_str, TCL_LINK_STRING | TCL_LINK_READ_ONLY)))
    return status;

    return Tcl_PkgProvide(interp, "tclreadline", (char*)tclrl_version_str);
}

static int TclReadlineInitialize(Tcl_Interp* interp, char* historyfile)
{
    

    rl_readline_name = "tclreadline";
    /*    rl_special_prefixes = "${\"["; */
    rl_special_prefixes = "$";
    /**
     * default is " \t\n\"\\'`@$><=;|&{("
     * removed "(" <-- arrays
     * removed "{" <-- `${' variables
     * removed "<" <-- completion lists with < ... >
     * added "[]"
     * added "}"
     */
    /* 11.Sep rl_basic_word_break_characters = " \t\n\"\\@$}=;|&[]"; */
    /* besser (11. Sept) 2. (removed \") */
    /* rl_basic_word_break_characters = " \t\n\\@$}=;|&[]"; */
    /* besser (11. Sept) 3. (removed }) */
    rl_basic_word_break_characters = " \t\n\\@$=;|&[]";
#if 0
    rl_basic_quote_characters = "\"{"; /* XXX ??? XXX */
    rl_completer_quote_characters = "\"";
#endif
    /*
       rl_filename_quote_characters
       = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

       rl_filename_quoting_function
       = (CPFunction*) TclReadlineFilenameQuotingFunction;
     */
    /*
       rl_filename_quoting_desired = 1;
     */

    using_history();
    if (!tclrl_eof_string)
        tclrl_eof_string = strdup("puts {}; exit");

    /*
     * try to read historyfile in home
     * directory. If this failes, this
     * is *not* an error.
     */
    rl_attempted_completion_function = (rl_completion_func_t *) TclReadlineCompletion;
    // if (read_history(historyfile)) {
    //     if (write_history(historyfile)) {
    //         Tcl_AppendResult (interp, "warning: `",
    //             historyfile, "' is not writable.", (char*) NULL);
    //     }
    // }

    return TCL_OK;
}

static int blank_line(char* str)
{
    char* ptr;
    for (ptr = str; ptr && *ptr; ptr++) {
        if (!ISWHITE(*ptr))
            return 0;
    }
    return 1;
}

static char** TclReadlineCompletion(char* text, int start, int end)
{
    char** matches = (char**) NULL;
    int status;
    rl_completion_append_character = ' '; /* reset, just in case ... */

    if (text && ('!' == text[0]
            || (start && rl_line_buffer[start - 1] == '!' /* for '$' */))) {
        char* expansion = (char*) NULL;
        int oldlen = strlen(rl_line_buffer);
        status = history_expand(rl_line_buffer, &expansion);
        if (status >= 1) {
            rl_extend_line_buffer(strlen(expansion) + 1);
            strcpy(rl_line_buffer, expansion);
            rl_end = strlen(expansion);
            rl_point += strlen(expansion) - oldlen;
            FREE(expansion);
            /*
             * TODO:
             * because we return 0 == matches,
             * the filename completer will still beep.
             rl_inhibit_completion = 1;
             */
            return matches;
        }
        FREE(expansion);
    }

    if (tclrl_custom_completer) {
        char start_s[BUFSIZ], end_s[BUFSIZ];
        Tcl_Obj* obj;
        Tcl_Obj** objv;
        int objc;
        int state;
        char* quoted_text = TclReadlineQuote(text, "$[]{}\"");
        char* quoted_rl_line_buffer = TclReadlineQuote(rl_line_buffer, "$[]{}\"");
        sprintf(start_s, "%d", start);
        sprintf(end_s, "%d", end);
        Tcl_ResetResult(tclrl_interp); /* clear result space */
        state = Tcl_VarEval(tclrl_interp, tclrl_custom_completer,
            " \"", quoted_text, "\" ", start_s, " ", end_s,
            " \"", quoted_rl_line_buffer, "\"", (char*) NULL);
        if (TCL_OK != state) {
            Tcl_AppendResult (tclrl_interp, " `", tclrl_custom_completer,
                " \"", quoted_text, "\" ", start_s, " ", end_s,
                " \"", quoted_rl_line_buffer, "\"' failed.", (char*) NULL);
            TclReadlineTerminate(state);
            FREE(quoted_text);
            FREE(quoted_rl_line_buffer);
            return matches;
        }
        FREE(quoted_text);
        FREE(quoted_rl_line_buffer);
        obj = Tcl_GetObjResult(tclrl_interp);
        status = Tcl_ListObjGetElements(tclrl_interp, obj, &objc, &objv);
        if (TCL_OK != status)
            return matches;

        if (objc) {
            int i, length;
            matches = (char**) MALLOC(sizeof(char*) * (objc + 1));
            for (i = 0; i < objc; i++) {
                matches[i] = strdup(Tcl_GetStringFromObj(objv[i], &length));
                if (1 == objc && !strlen(matches[i])) {
                    FREE(matches[i]);
                    FREE(matches);
                    Tcl_ResetResult(tclrl_interp); /* clear result space */
                    return (char**) NULL;
                }
            }

            /**
             * this is a special one:
             * if the script returns exactly two arguments
             * and the second argument is the empty string,
             * the rl_completion_append_character is set
             * temporarily to NULL.
             */
            if (2 == objc && !strlen(matches[1])) {
                i--;
                FREE(matches[1]);
                rl_completion_append_character = '\0';
            }

            matches[i] = (char*) NULL; /* terminate */
        }
        Tcl_ResetResult(tclrl_interp); /* clear result space */
    }

    if (!matches && tclrl_use_builtin_completer) {
        matches = rl_completion_matches(text, (rl_compentry_func_t *)TclReadline0generator);
    }
    return matches;
}

static char*
TclReadline0generator(char* text, int state)
{
    return TclReadlineKnownCommands(text, state, _CMD_GET);
}

static char* TclReadlineKnownCommands(char* text, int state, int mode)
{
    static int len;
    static cmds_t *cmds = (cmds_t *) NULL, *cmds_new;
    char* tmp;
    char* args[256];
    int i, argc;
    char** name;

    char* local_line = (char*) NULL;
    int sub;


    switch (mode) {

        case _CMD_SET:

            cmds_new = (cmds_t *) MALLOC(sizeof(cmds_t));
            cmds_new->next = (cmds_t *) NULL;

            if (!cmds) {
                cmds = cmds_new;
                cmds->prev = cmds_new;
            } else {
                cmds->prev->next = cmds_new;
                cmds->prev = cmds_new;
            }

            tmp = strdup(text);
            argc = TclReadlineParse(args, sizeof(args), tmp);

            cmds_new->cmd = (char**) MALLOC(sizeof(char*) * (argc + 1));

            for (i = 0; i < argc; i++)
                cmds_new->cmd[i] = args[i];

            cmds_new->cmd[argc] = (char*) NULL;

            return (char*) NULL;
            /* NOTREACHED */
            break;


        case _CMD_GET:

            local_line = strdup(rl_line_buffer);
            sub = TclReadlineParse(args, sizeof(args), local_line);

            if (0 == sub || (1 == sub && '\0' != text[0])) {
                if (!state) {
                    cmds_new = cmds;
                    len = strlen(text);
                }
                while (cmds_new && (name = cmds_new->cmd)) {
                    cmds_new = cmds_new->next;
                    if (!strncmp(name[0], text, len))
                        return strdup(name[0]);
                }
                return (char*) NULL;
            } else {

                if (!state) {

                    cmds_new = cmds;
                    len = strlen(text);

                    while (cmds_new && (name = cmds_new->cmd)) {
                        if (!strcmp(name[0], args[0]))
                            break;
                        cmds_new = cmds_new->next;
                    }

                    if (!cmds_new)
                        return (char*) NULL;

                    for (i = 0; cmds_new->cmd[i]; i++) /* EMPTY */;

                    if (sub < i && !strncmp(cmds_new->cmd[sub], text, len))
                        return strdup(cmds_new->cmd[sub]);
                    else
                        return (char*) NULL;

                }
                else
                    return (char*) NULL;
            }

            /* NOTREACHED */
            break;


        default:
            return (char*) NULL;
            /* NOTREACHED */
            break;

    }
}

static int TclReadlineParse(char** args, int maxargs, char* buf)
{
    int nr = 0;

    while (*buf != '\0' && nr < maxargs - 1) {
        /*
         * Strip whitespace.  Use nulls, so
         * that the previous argument is terminated
         * automatically.
         */
        while (ISWHITE(*buf))
            *buf++ = '\0';

        if (!(*buf)) /* don't count the terminating NULL */
            break;

        *args++ = buf;
        nr++;

        while (('\0' != *buf) && !ISWHITE(*buf))
            buf++;
    }

    *args = NULL;
    return nr;
}

} // namespace gui
} // namespace open_edi

