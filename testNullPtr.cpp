#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <map>
#include <functional>

#include <exception>
    #include <signal.h>
#include <stdexcept>
void terminateHandler()
{
	if(std::current_exception()!=NULL)
	{
        try
        {
                throw;
        }
        catch (const std::exception& ex)
        {
                std::cout << "terminateHandler: check " << ex.what() << std::endl;
        }
        catch (...)
        {
                std::cout << "terminateHandler: Unknown exception!" << std::endl;
        }
}

        std::abort();
}


using namespace std ;
std::exception_ptr p;
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
/*
void readNullPointer()
{
 
       cout << "null ptr access " << endl	;
       int* p = NULL;
       std::cout << *p << std::endl;
//       cout << "null ptr access - caught " << endl	;
} */

void readNullPointer()
{
    try
    {
 
       cout << "null ptr access " << endl	;
       int* p = NULL;
       std::cout << *p << std::endl;
    }

    catch (const std::exception& ex)
    {
       cout << "null ptr access - caught " << endl	;
       std::cout << ex.what() << std::endl;
    } 
} 
//#if defined(USE_POSIX_SIGNAL) || defined(USE_POSIX_SIGACTION)
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
//#endif


    void
    signalHandler(int sgn)
    {
	std::cout << " signalHandler sgn" << std::endl ;
        std::ostringstream os;
        os << "Signal caught: " << signalDescription(sgn) << "(" << sgn << ")";

        signal(sgn, signalHandler);

        throw std::runtime_error(os.str().c_str());
    }

    void register_for_os_exceptions()
    {
        signal(SIGABRT, signalHandler);
        signal(SIGFPE, signalHandler);
        signal(SIGILL, signalHandler);
        signal(SIGINT, signalHandler);
        signal(SIGSEGV, signalHandler);
        signal(SIGTERM, signalHandler);
    } 

int main()
{
	register_for_os_exceptions();
	std::set_terminate(terminateHandler);
	readNullPointer();

	cout << "end of Main   "   << endl;

}
