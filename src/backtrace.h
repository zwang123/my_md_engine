#ifndef __BACKTRACE_H_INCLUDED
#define __BACKTRACE_H_INCLUDED
#include <execinfo.h>
#include <signal.h>
#include <string.h>

#include <iostream>
#include <cstdlib>
#include <stdexcept>

void my_terminate(void);

namespace {
    // invoke set_terminate as part of global constant initialization
    static const bool SET_TERMINATE = std::set_terminate(my_terminate);
}

// This structure mirrors the one found in /usr/include/asm/ucontext.h
typedef struct _sig_ucontext {
   unsigned long     uc_flags;
   struct ucontext   *uc_link;
   stack_t           uc_stack;
   struct sigcontext uc_mcontext;
   sigset_t          uc_sigmask;
} sig_ucontext_t;

void crit_err_hdlr(int sig_num, siginfo_t * info, void * ucontext) {
    sig_ucontext_t * uc = (sig_ucontext_t *)ucontext;

    // Get the address at the time the signal was raised from the EIP (x86)
#ifdef __x86_64__
    void * caller_address = (void *) uc->uc_mcontext.rip;
#else
    void * caller_address = (void *) uc->uc_mcontext.eip;
#endif

    std::cerr << "signal " << sig_num 
              << " (" << strsignal(sig_num) << "), address is " 
              << info->si_addr << " from " 
              << caller_address << std::endl;

    void * array[50];
    int size = backtrace(array, 50);

    std::cerr << __func__ << " backtrace returned " 
              << size << " frames\n\n";

    // overwrite sigaction with caller's address
    array[1] = caller_address;

    char ** messages = backtrace_symbols(array, size);

    // skip first stack frame (points here)
    for (int i = 1; i < size && messages != NULL; ++i) {
        std::cerr << "[bt]: (" << i << ") " << messages[i] << std::endl;
    }
    std::cerr << std::endl;

    free(messages);

    exit(EXIT_FAILURE);
}

void my_terminate() {
    static bool tried_throw = false;

    try {
        // try once to re-throw currently active exception
        //if (!tried_throw++) throw;
        auto tried_throw_bck = tried_throw;
        tried_throw = true;
        if (!tried_throw_bck) throw; // avoid warning
    }
    catch (const std::exception &e) {
        std::cerr << __func__ << " caught unhandled exception. what(): "
                  << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << __func__ << " caught unknown/unhandled exception." 
                  << std::endl;
    }

    void * array[50];
    int size = backtrace(array, 50);    

    std::cerr << __func__ << " backtrace returned " 
              << size << " frames\n\n";

    char ** messages = backtrace_symbols(array, size);

    for (int i = 0; i < size && messages != NULL; ++i) {
        std::cerr << "[bt]: (" << i << ") " << messages[i] << std::endl;
    }
    std::cerr << std::endl;

    free(messages);

    abort();
}
#endif // __BACKTRACE_H_INCLUDED
