#include "msh.hpp"

#include "../include/alg.h"
#include "../include/msh.h"

msh::msh() {}
msh::~msh() { msh_freeRessources(); }

void msh::Script(char * script) { msh_readScript(script); }

// inline class
class msh::execute::execinfo : public msh_info {
    private:
        bool in_func_old;
    public:
        execinfo() {
            // *this = MSH_INFO_DEFAULT;
            this->wert = msh_Wert;
            this->info = (msh_callinfo) MSH_CALLINFO_DEFAULT;
            this->stack = (FUNC_LOCAL_STACK *) NULL;
            this->refs = NULL;
            this->io = (msh_io) MSH_IO_DEFAULT;
            this->event = (msh_events) MSH_EVENTS_DEFAULT;

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

// manage global vars

bool msh::var::Push(const char * name, const char * value) { return msh_var_push(NULL, value, name); }
bool msh::var::Update(const char * name, const char * value) { return msh_var_updateByName(NULL, value, name); }
bool msh::var::Update(unsigned int index, const char * value) { return msh_var_updateByIndex(NULL, value, index); }

const char * msh::var::Get(const char * name) { return msh_var_getByName(NULL, name); }
const char * msh::var::Get(unsigned int index) { return msh_var_getByIndex(NULL, index); }
unsigned int msh::var::GetIndex(const char * name) { return msh_var_getIndexByName(NULL, name); }

int msh::var::Count() {
    return VAR_WORTZAELER;
}
void msh::var::PrintAll() {
    for (int i = 0; i < VAR_WORTZAELER; i++) {
        printf("%d | %s | %s\n", i, VAR_NAMES[i], VAR_SPEICHER[i]);
    }
}

// execute

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
void msh::execute::Script(char * script) { msh_readScript(script); }

void msh::execute::Error(const char * msg) { return msh_error(this->msh, msg); }

unsigned int msh::execute::Info::Line() const { return this->msh->info.line; }
bool msh::execute::Info::InFunc() const { return this->msh->info.in_func; }
bool msh::execute::Info::InThread() const { return this->msh->info.in_thread; }

// Var

bool msh::execute::Var::Push(const char * name, const char * value) {
    if (this->msh->info.in_func) {
        msh_func_update_local_Var(name, value, this->msh->stack);
        return true;
    }
    return msh_var_push(this->msh, value, name);
}
bool msh::execute::Var::Update(const char * name, const char * value) {
    if (this->msh->info.in_func) {
        msh_func_update_local_Var(name, value, this->msh->stack);
        return true;
    }
    return msh_var_updateByName(this->msh, value, name);
}

const char * msh::execute::Var::Get(const char * name) const { return msh_var_getByName(this->msh, name); }
const char * msh::execute::Var::Get(unsigned int index) const { return msh_var_getByIndex(this->msh, index); }

int msh::execute::Var::Count() const {
    if (this->msh->info.in_func) {
        if (this->msh->stack == NULL) { return -1; }
        return s_arr_len(this->msh->stack->VAR_NAMES);
    }
    return msh::var::Count();
}
void msh::execute::Var::PrintAll() const {
    if (this->msh->info.in_func) {
        int nvars = this->Count();
        if (nvars == -1) { puts("No stack!"); }
        if (nvars == 0) { puts("No vars!"); }
        for (int i = 0; i < nvars; i++) {
            superstring name = s_arr_getEl(this->msh->stack->VAR_NAMES, i);
            superstring value = s_arr_getEl(this->msh->stack->VAR_SPEICHER, i);
            printf("%d | ", i); // s_printf("%SU | %SU \n", name, value); // bad - meory leaks
            s_print(name); printf(" | ");
            s_print(value); putchar('\n');
        }
    }
    return msh::var::PrintAll();
}

// Func

msh::execute::Func::Func() : entered_stack(NULL), old_stack(NULL) {}

void msh::execute::Func::Push(const char * name, const char * script) { /* does not do anything yet */ }
void msh::execute::Func::Call(const char * name) { return msh_func_call(this->msh, name); }

bool msh::execute::Func::Enter(const char * name) {
    if (this->entered_stack != NULL) {
        // does not support nested entering
        return false;
    }
    this->old_stack = (void *) this->msh->stack;
    this->msh->stack = new FUNC_LOCAL_STACK; // { NULL, NULL };
    // printf("stackp : %lld\n", (unsigned long long) this->msh->stack);
    this->msh->stack->VAR_NAMES = NULL;
    this->msh->stack->VAR_SPEICHER = NULL;
    this->msh->enter_func(name);

    this->entered_stack = (void *) this->msh->stack;
    return true;
}
bool msh::execute::Func::Exit() {
    if (this->entered_stack == NULL) {
        // needs to have a successfull this->Enter before
        return false;
    }
    this->msh->exit_func();

    s_arr_free(this->msh->stack->VAR_NAMES);
    s_arr_free(this->msh->stack->VAR_SPEICHER);
    delete this->msh->stack;

    this->msh->stack = (FUNC_LOCAL_STACK *) this->old_stack;
    // printf("stackp : %lld\n", (unsigned long long) this->msh->stack);
    this->entered_stack = NULL;
    this->old_stack = NULL;
    return true;
}