/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved                       
*
*************************************************************************** 
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* $FileName: httpd_mqx.c$
* $Version : 3.8.1.0$
* $Date    : Jul-27-2011$
*
* Comments:
*
*   HTTPD os specific part implementation.
*
*END************************************************************************/

#include "httpd_mqx.h"
#include "httpd.h"

#if HTTPDCFG_POLL_MODE
#include <select.h>
#endif

#define MAX(a, b)       (((a) > (b)) ? (a) : (b))

/** initialize httpd server with default configuration
 * \param root_dir < root directory structure
 * \param index_page < pointer to index page with absolut path
 * \return pointer to created server structure
 */
HTTPD_STRUCT* httpd_server_init(HTTPD_ROOT_DIR_STRUCT *root_dir, const char *index_page) {
    extern HTTPD_PARAMS_STRUCT* httpd_default_params(HTTPD_PARAMS_STRUCT *params);
    extern HTTPD_STRUCT* httpd_init(HTTPD_PARAMS_STRUCT *params);
    HTTPD_STRUCT *server = NULL;
    HTTPD_PARAMS_STRUCT *params;
    
    params = httpd_default_params(NULL);
    if (params) {
        params->root_dir = root_dir;
        params->index_page = (char*)index_page;
        server = httpd_init(params);
    }
    
    return server;
}

#if !HTTPDCFG_POLL_MODE     // httpd - sessions in task mode

/** session task
 */
static void httpd_session_task(pointer init_ptr, pointer creator) {
    extern HTTPD_SESSION_STRUCT* httpd_ses_alloc(HTTPD_STRUCT *server);
    extern void httpd_ses_poll(HTTPD_STRUCT *server, HTTPD_SESSION_STRUCT *session);
    HTTPD_SESSION_STRUCT *session;
    
    if (!init_ptr)
        goto ERROR;
    
    session = httpd_ses_alloc((HTTPD_STRUCT*)init_ptr);
    
    if (session) {
        RTCS_task_resume_creator(creator, RTCS_OK);
        httpd_ses_poll((HTTPD_STRUCT*)init_ptr, session);
    }
    else
        goto ERROR;
    
    return;
    
ERROR:
    RTCS_task_resume_creator(creator, RTCS_ERROR);
}

/** create task for session
 * \param server < pointer to server structure
 * \param stack < stack size for session task
 * \return
 */
static uint_32 httpd_ses_task_create(HTTPD_STRUCT *server) {
    uint_32 stack;
    
    stack = MAX(MAX(HTTPD_MAX_LEN, HTTPDCFG_MAX_BYTES_TO_SEND), HTTPDCFG_MAX_SCRIPT_LN) + 10 * sizeof(void*);
    stack = MAX(3 * stack, 2500);

    return RTCS_task_create("httpd session", 8, stack, httpd_session_task, server);
}

/** run httpd server - each session in separete task
 * \param server < pointer to server structure
 * \return zero if success
 */
int httpd_server_run(HTTPD_STRUCT *server) {
    _mqx_int ses_cnt = server->params->max_ses;
    
    while (server && ses_cnt) {
        if (httpd_ses_task_create(server) != RTCS_OK)
            break;
        
        ses_cnt--;
    }
    
    return (ses_cnt);
}

#else       // httpd - sessions in poll mode

/** httpd server poll
 * \param server < pointer to server structure
 * \param to < timeout for poll (max time for waiting event)
 */
void httpd_server_poll(HTTPD_STRUCT *server, _mqx_int to) {
    HTTPD_SESSION_STRUCT **session;
    int si, res;
    fd_set read_set, write_set;
    struct timeval tm;

    extern void httpd_ses_poll(HTTPD_STRUCT *server, HTTPD_SESSION_STRUCT *session);
    
    if (!server)
        return;
    
    session = server->session;

    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
        
    for (si = 0; si < server->params->max_ses; si++) {
        if (session[si]->valid == HTTPD_SESSION_VALID) {
            FD_SET(session[si]->sock, &read_set);
            FD_SET(session[si]->sock, &write_set);
        }
    }
    
    if (read_set.COUNT < server->params->max_ses)       // set listen socket if some session is not used
        FD_SET(server->sock, &read_set);

    tm.tv_sec = (to == -1) ? 0 : to;
    tm.tv_usec = 0;
        
    res = select(server->sock + 1, &read_set, &write_set, NULL, (to == 0) ? NULL : &tm);
        
    if (res > 0) {
        if (FD_ISSET(server->sock, &read_set)) {
            // new connection
            
            for (si = 0; si < server->params->max_ses; si++) {
                if (session[si] && session[si]->valid != HTTPD_SESSION_VALID) {
                    httpd_ses_poll(server, session[si]);
                    break;
                }
            }
        }
            
        for (si = 0; si < server->params->max_ses; si++) {
            if ((FD_ISSET(session[si]->sock, &read_set) || FD_ISSET(session[si]->sock, &write_set)) &&
                session[si]->valid == HTTPD_SESSION_VALID) {
                    
                httpd_ses_poll(server, session[si]);
            }
        }
    }
}

/** server task - httpd run in one task and poll each session
 */
static void httpd_server_task(pointer init_ptr, pointer creator) {
    extern void httpd_ses_poll(HTTPD_STRUCT *server, HTTPD_SESSION_STRUCT *session);
    
    HTTPD_STRUCT *server = (HTTPD_STRUCT*)init_ptr;
    
    if (!server)
        goto ERROR;
        
    RTCS_task_resume_creator(creator, RTCS_OK);

    while (1) {
        httpd_server_poll(server, 1);
    }
    
ERROR:
    RTCS_task_resume_creator(creator, (uint_32)RTCS_ERROR);
}

/** create server task - poll mode
 * \param server < pointer to server structure
 * \return
 */
static uint_32 httpd_server_task_create(HTTPD_STRUCT *server) {
    uint_32 stack;
    
    stack = MAX(MAX(HTTPD_MAX_LEN, HTTPDCFG_MAX_BYTES_TO_SEND), HTTPDCFG_MAX_SCRIPT_LN) + 10 * sizeof(void*);
    stack = MAX(3 * stack, 2500);
    
    return RTCS_task_create("httpd server", 8, stack, httpd_server_task, server);
}

/** run httpd server in poll mode
 * \param server < pointer to server structure
 * \return zero if success
 */
int httpd_server_run(HTTPD_STRUCT *server) {
    if (server && httpd_server_task_create(server) == RTCS_OK)
        return 0;
    
    return -1;
}

#endif
