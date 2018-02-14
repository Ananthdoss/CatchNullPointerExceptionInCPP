#if defined(USE_POSIX_SIGNAL) || defined(USE_POSIX_SIGACTION)
    #include <signal.h>
#endif


#include <sstream>
#include <exception>
#include <stdexcept>

//#include "except.h"

    //void register_for_os_exceptions();

    struct null_pointer_exception : public std::exception
    {
        const char* what() 
        {
            return "OS exception: null pointer!";
        }
    };

    struct division_by_zero_exception : public std::exception
    {
        const char* what() 
        {
            return "OS exception: division by zero!";
        }
    };




#if defined(USE_POSIX_SIGNAL) || defined(USE_POSIX_SIGACTION)
    const char* signalDescription(int sgn)
    {
        switch(sgn)
        {
            case SIGABRT: return "SIGABRT";
            case SIGFPE:  return "SIGFPE";
            case SIGILL:  return "SIGILL";
            case SIGINT:  return "SIGINT";
            case SIGSEGV: return "SIGSEGV";
            case SIGTERM: return "SIGTERM";
            default:      return "UNKNOWN";
        }
    }
#endif

#ifdef USE_POSIX_SIGACTION    
    void signalHandler(int sgn, siginfo_t *info, void *)
    {
        if (sgn == SIGSEGV && info->si_addr == 0)
        {
            throw null_pointer_exception();
        }

        if (sgn == SIGFPE && (info->si_code == FPE_INTDIV || info->si_code == FPE_FLTDIV))
        {
            throw division_by_zero_exception();
        }

        std::ostringstream os;
        os << "Signal caught: " << signalDescription(sgn) << "(" << sgn << ")";

        throw std::runtime_error(os.str().c_str());
    }
/*

    void register_for_os_exceptions()
    {
        struct sigaction act;

        act.sa_sigaction = signalHandler;
        sigemptyset(&act.sa_mask);
        act.sa_flags = SA_SIGINFO | SA_NODEFER;

        sigaction(SIGABRT, &act, NULL);
        sigaction(SIGFPE, &act, NULL);
        sigaction(SIGILL, &act, NULL);
        sigaction(SIGINT, &act, NULL);
        sigaction(SIGSEGV, &act, NULL);
        sigaction(SIGTERM, &act, NULL);
    } */
#endif


#ifdef USE_POSIX_SIGNAL
    void
    signalHandler(int sgn)
    {
        std::ostringstream os;
        os << "Signal caught: " << signalDescription(sgn) << "(" << sgn << ")";

        signal(sgn, signalHandler);

        throw std::runtime_error(os.str().c_str());
    }

    void except::register_for_os_exceptions()
    {
        signal(SIGABRT, signalHandler);
        signal(SIGFPE, signalHandler);
        signal(SIGILL, signalHandler);
        signal(SIGINT, signalHandler);
        signal(SIGSEGV, signalHandler);
        signal(SIGTERM, signalHandler);
    } 
#endif 

