#include "coroutine.hpp"
#include <functional>

//*
void coro_function(boost::coroutines::asymmetric_coroutine<coro_packet*>::pull_type source, coro_func func) {
    coro_packet p;
    p.types.push_back(typeid(boost::coroutines::asymmetric_coroutine<coro_packet*>::pull_type));
    p.values.push_back((void*)&source);
    *source.get() = p;
    source();
    *current_coro->sent_packet = source.get();
    current_coro->first_call = false;
    func();
}
//*/

coroutine_t * current_coro;

bool operator==(coro_packet lhs, coro_packet rhs) {return lhs.types == rhs.types && lhs.values == rhs.values;}

coro_packet coroutine_t::resume(coro_packet p) {
    if (this->status == COROUTINE_STATUS_DEAD) return;
    coroutine_t * last_coro = current_coro;
    this->status = COROUTINE_STATUS_RUNNING;
    last_coro->status = COROUTINE_STATUS_NORMAL;
    current_coro = this;
    coro_packet * paddr = &p;
    sink(paddr);
    current_coro = last_coro;
    if (sink) this->status = COROUTINE_STATUS_SUSPENDED;
    else this->status = COROUTINE_STATUS_DEAD;
    current_coro->status = COROUTINE_STATUS_RUNNING;
    return *paddr;
}

coro_packet coroutine_t::yield(coro_packet p) {
    if (!first_call) **sent_packet = p;
    else sent_packet = (coro_packet**)malloc(sizeof(coro_packet**));
    first_call = false;
    (*source)();
    *sent_packet = source->get();
    return **sent_packet;
}

coroutine_t CoroutineAPI::create(coro_func func) {
    coroutine_t c;
    coro_packet p;
    c.sink = boost::coroutines::asymmetric_coroutine<coro_packet*>::push_type(std::bind(coro_function, std::placeholders::_1, func));
    coro_packet * paddr = &p;
    c.sink(paddr);
    if (*paddr == p || paddr->types.size() == 0) throw "Coroutine initializer returned nothing!";
    if (paddr->types[0] != typeid(boost::coroutines::asymmetric_coroutine<coro_packet*>::pull_type)) throw "Coroutine initializer returned incorrect type!";
    c.source = (boost::coroutines::asymmetric_coroutine<coro_packet*>::pull_type*)paddr->values[0];
    return c;
}

coroutine_t * CoroutineAPI::running() {return current_coro;}

coro_packet CoroutineAPI::yield(coro_packet p) {current_coro->yield(p);}

CoroutineAPI coroutine;