#ifndef MSH_HPP
#define MSH_HPP

#ifndef MSH_ALLOW_UNSAFE
    #define MSH_ALLOW_UNSAFE 0
#endif

class msh {
    public:
        // this is basically a namespace
        class var {
            public:
                static bool Push(const char * name, const char * value);
                static bool Update(const char * name, const char * value);
                static bool Update(unsigned int index, const char * value);

                static const char * Get(const char * name);
                static const char * Get(unsigned int index);
                static unsigned int GetIndex(const char * name);

                static int Count();
                static void PrintAll();
        };
        class execute {
            private:
                class execinfo; // defined in msh.cpp
                execinfo * msh;
            public:
                execute();
                ~execute();

                // execute
                void Line(const char *);
     
                /**
                 * @brief Executes a msh-script
                 * This functions creates and destroys its own msh::execute,
                 * that's why it's static.
                 * It's only also here, because I think people would expect it to be here.
                 */
                static void Script(char *);

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

                        const char * Get(const char * name) const;
                        const char * Get(unsigned int index) const;

                        int Count() const;
                        void PrintAll() const;
                };

                class Func {
                    private:
                        execinfo * msh;
                        friend class msh::execute;

                        // used for Enter and Exit
                        void * old_stack;
                        void * entered_stack;
                    private:
                        Func();
                    public :
                        
                        // proper function management
                        // define a function - does nothing for now
                        void Push(const char * name, const char * script);
                        // call a function
                        void Call(const char * name);

                        // "fake" function management
                        /**
                         * @brief Enter in a function state
                         * This does not define or call a function!!!
                         * It just prepare an environment, like in a function
                         */
                        bool Enter(const char * name = "fakefunc");
                        /**
                         * @brief Exits the current function state
                         * Only used, when this->Enter was used before!
                         * Otherwise this may cause trouble.
                         */
                        bool Exit();
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
         * This functions creates and destroys its own msh::execute,
         * that's why it's here.
         */
        static void Script(char *);
};

#endif