#ifndef MSH_HPP
#define MSH_HPP

class msh {
    public:
        class execute {
            private:
                class execinfo; // defined in msh.cpp
                execinfo * msh;
            public:
                execute();
                ~execute();

                // execute
                void Line(const char *);

                // other helpfull functions
                void Error(const char *);

                // sub classes

                // this is basically a namespace
                class Info {
                    private:
                        execinfo * msh;
                        friend class msh::execute;
                    public:

                        unsigned int Line() const;
                        bool InFunc() const;
                        bool InThread() const;
                };

                class Var {
                    private:
                        execinfo * msh;
                        friend class msh::execute;
                    public:
                        bool Push(const char * name, const char * value);
                        bool Update(const char * name, const char * value);
                        bool Update(unsigned int index, const char * value);

                        const char * Get(const char * name) const;
                        const char * Get(unsigned int index) const;
                        unsigned int GetIndex(const char * name) const;
                };

                class Func {
                    private:
                        execinfo * msh;
                        friend class msh::execute;
                    public:
                        void Push(const char * name, const char * script);
                        void Call(const char * name);
                };
            public:
                Info info;
                Var var;
                Func func;
        };
    public:
        msh();
        ~msh();
        
        /**
         * @brief Executes a msh-script
         * This functions creates and destroys its own msg::execute,
         * that's why it's here.
         */
        void Script(char *);
};

#endif