#include <boost/coroutine/all.hpp>
#include <vector>
#include <typeinfo>

typedef void (*coro_func)();

enum coro_status {
    COROUTINE_STATUS_RUNNING,
    COROUTINE_STATUS_SUSPENDED,
    COROUTINE_STATUS_NORMAL,
    COROUTINE_STATUS_DEAD
};

typedef struct {
    std::vector<std::type_info> types;
    std::vector<void*> values;
} coro_packet;

class coroutine_t {
    friend class CoroutineAPI;
    friend void coro_function(boost::coroutines::asymmetric_coroutine<coro_packet*>::pull_type source, coro_func func);
    boost::coroutines::asymmetric_coroutine<coro_packet*>::push_type sink; // holds function
    boost::coroutines::asymmetric_coroutine<coro_packet*>::pull_type* source;
    coro_packet** sent_packet;
    bool first_call = true;
public:
    int status;
    coro_packet resume(coro_packet p);
    coro_packet yield(coro_packet p);
    coro_packet operator()(coro_packet p) {resume(p);}
};

class CoroutineAPI {
public:
    coroutine_t create(coro_func func);
    coro_packet resume(coroutine_t coro, coro_packet p = {}) {coro.resume(p);}
    coroutine_t * running();
    int status(coroutine_t coro) {return coro.status;}
    coroutine_t wrap(coro_func func) {return create(func);}
    coro_packet yield(coro_packet p = {});
};

extern CoroutineAPI coroutine;