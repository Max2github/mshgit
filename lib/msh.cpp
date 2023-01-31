#include "msh.hpp"
#include "../include/alg.h"
#include "../include/msh.h"

msh::msh() {
    
}
msh::~msh() {
    msh_freeRessources();
}

void msh::Script(char * script) {
    msh_readScript(script);
}

// inline class
class msh::execute::execinfo : public msh_info {
    private:
        bool in_func_old;
    public:
        execinfo() {
            // *this = MSH_INFO_DEFAULT;
            this->wert = msh_Wert;
            this->info = MSH_CALLINFO_DEFAULT;
            this->stack = (FUNC_LOCAL_STACK *) NULL;
            this->refs = NULL;
            this->io = MSH_IO_DEFAULT;
            this->event = MSH_EVENTS_DEFAULT;

            this->in_func_old = false;
        }
        ~execinfo() {

        }

        // may not be needed!
        void enter_func(const char * name) {
            this->in_func_old = this->info.in_func;
            this->info.in_func = true;
            msh_func_deph_add_func(this, name);
        }
        void exit_func() {
            this->info.in_func = in_func_old;
            msh_func_depth_remove_last_func(this);
        }
};

msh::execute::execute() {
    this->msh = new execinfo();
    msh_exec_init(this->msh, true);
    
    this->info.msh = this->msh;
    this->var.msh = this->msh;
    this->func.msh = this->msh;
}
msh::execute::~execute() {
    msh_exec_free(this->msh);
    delete this->msh;
    this->msh = NULL;
}

void msh::execute::Line(const char * ln) { msh_readZeile(this->msh, ln); }

void msh::execute::Error(const char * msg) { return msh_error(this->msh, msg); }

unsigned int msh::execute::Info::Line() const { return this->msh->info.line; }
bool msh::execute::Info::InFunc() const { return this->msh->info.in_func; }
bool msh::execute::Info::InThread() const { return this->msh->info.in_thread; }

bool msh::execute::Var::Push(const char * name, const char * value) { return msh_var_push(this->msh, value, name); }
bool msh::execute::Var::Update(const char * name, const char * value) { return msh_var_updateByName(this->msh, value, name); }
bool msh::execute::Var::Update(unsigned int index, const char * value) { return msh_var_updateByIndex(this->msh, value, index); }
const char * msh::execute::Var::Get(const char * name) const { return msh_var_getByName(this->msh, name); }
const char * msh::execute::Var::Get(unsigned int index) const { return msh_var_getByIndex(this->msh, index); }
unsigned int msh::execute::Var::GetIndex(const char * name) const { return msh_var_getIndexByName(this->msh, name); }

void msh::execute::Func::Push(const char * name, const char * script) { /* does not do anything yet */ }
