#ifndef FDUTIL_HPP
#define FDUTIL_HPP

#define USE_IN_FDBUF_PROCESS 3

#include <tuple>
#include <initializer_list>
using std::initializer_list;
using std::pair;
using std::make_pair;
using std::tuple;
using std::make_tuple;
#if USE_IN_FDBUF_PROCESS == 2
#include <spawn.h>
#endif

#include "../cut.hpp"

class fdutil
{
    public:
        fdutil() = delete;
        //template<class Iter> static Iter block_on_all(Iter b, Iter e)
        //template<class Iter, class T> static Iter block_on_all(Iter b, Iter e, T& t)
        //template<class Iter> static Iter block_on_pointer_all(Iter b, Iter e)
        //template<class Iter, class T> static Iter block_on_pointer_all(Iter b, Iter e, T& t)
        //static pair<fdbuf, fdbuf> pipe()
        //template<String stringT> static tuple<fdbuf, fdbuf, fdbuf, pid_t> process(initializer_list<stringT> args, bool closein = false, bool closeout = false, bool closeerr = false)
        //template<String stringT> static fdbuf wget(stringT url)
        //template<class Iter> static tuple<fdbuf, fdbuf, fdbuf, pid_t> process(cut<Iter> args, bool closein = false, bool closeout = false, bool closeerr = false)
        //static tuple<fdbuf, fdbuf, fdbuf, pid_t> process(const char* const* argv, bool closein, bool closeout, bool closeerr);

        template<class Iter>
        static Iter block_on_all(Iter b, Iter e)
        {
            fd_set fds;
            FD_ZERO(&fds);
            int fdm = 0;
            for(Iter i = b; i != e; ++i)
            {
                FD_SET(i->get_fd(), &fds);
                fdm = std::max<int>(fdm, i->get_fd());
            }
            int r = select(fdm + 1, &fds, nullptr, nullptr, nullptr);
            if(r > 0)
            {
                Iter i = b;
                while(i != e && !FD_ISSET(i->get_fd(), &fds))
                    ++i;
                return i;
            }
            return e;
        }

        template<class Iter, class T>
        static Iter block_on_all(Iter b, Iter e, T& t)
        {
            fd_set fds;
            FD_ZERO(&fds);
            int fdm = 0;
            for(Iter i = b; i != e; ++i)
            {
                FD_SET(i->get_fd(), &fds);
                fdm = std::max<int>(fdm, i->get_fd());
            }
            {
                FD_SET(t.get_fd(), &fds);
                fdm = std::max<int>(fdm, t.get_fd());
            }
            int r = select(fdm + 1, &fds, nullptr, nullptr, nullptr);
            if(r > 0)
            {
                Iter i = b;
                while(i != e && !FD_ISSET(i->get_fd(), &fds))
                    ++i;
                return i;
            }
            return e;
        }

        template<class Iter>
        static Iter block_on_pointer_all(Iter b, Iter e)
        {
            fd_set fds;
            FD_ZERO(&fds);
            int fdm = 0;
            for(Iter i = b; i != e; ++i)
            {
                FD_SET((*i)->get_fd(), &fds);
                fdm = std::max<int>(fdm, (*i)->get_fd());
            }
            int r = select(fdm + 1, &fds, nullptr, nullptr, nullptr);
            if(r > 0)
            {
                Iter i = b;
                while(i != e && !FD_ISSET((*i)->get_fd(), &fds))
                    ++i;
                return i;
            }
            return e;
        }

        template<class Iter, class T>
        static Iter block_on_pointer_all(Iter b, Iter e, T& t)
        {
            fd_set fds;
            FD_ZERO(&fds);
            int fdm = 0;
            for(Iter i = b; i != e; ++i)
            {
                FD_SET((*i)->get_fd(), &fds);
                fdm = std::max<int>(fdm, (*i)->get_fd());
            }
            {
                FD_SET(t.get_fd(), &fds);
                fdm = std::max<int>(fdm, t.get_fd());
            }
            int r = select(fdm + 1, &fds, nullptr, nullptr, nullptr);
            if(r > 0)
            {
                Iter i = b;
                while(i != e && !FD_ISSET((*i)->get_fd(), &fds))
                    ++i;
                return i;
            }
            return e;
        }

        //first read, second write
        static pair<fdbuf, fdbuf> pipe()
        {
            int pipefd[2];
            ::pipe(pipefd);
            return make_pair(fdbuf(pipefd[0]), fdbuf(pipefd[1]));
        }
        template<String stringT>
        static tuple<fdbuf, fdbuf, fdbuf, pid_t> process(initializer_list<stringT> args, bool closein = false, bool closeout = false, bool closeerr = false)
        {
            const char* argv[args.size() + 1];
            typename std::initializer_list<stringT>::const_iterator it = args.begin();
            size_t i = 0;
            while(i < args.size() && it != args.end())
            {
                argv[i] = string_cstring(*it);
                ++it;
                ++i;
            }
            argv[i] = nullptr;
            return process(argv, closein, closeout, closeerr);
        }
        template<String stringT>
        static fdbuf wget(stringT url)
        {
            return get<1>(process({"wget", "-qO-", url}, true, false, true));
        }

        template<class Iter>
        static tuple<fdbuf, fdbuf, fdbuf, pid_t> process(cut<Iter> args, bool closein = false, bool closeout = false, bool closeerr = false)
        {
            const size_t s = args.size();
            const char* argv[s + 1];
            Iter it = args.begin();
            size_t i = 0;
            while(i < s && it != args.end())
            {
                argv[i] = string_cstring(*it);
                ++it;
                ++i;
            }
            argv[i] = nullptr;
            return process(argv, closein, closeout, closeerr);
        }

#if USE_IN_FDBUF_PROCESS == 1
        static tuple<fdbuf, fdbuf, fdbuf, pid_t> process(const char* const* argv, bool closein, bool closeout, bool closeerr)
        {
            int infd[2];
            int outfd[2];
            int errfd[2];
            ::pipe(infd);
            ::pipe(outfd);
            ::pipe(errfd);
            pid_t pid = fork();
            if(pid == 0) //!pid
            {
                //child
                dup2(infd[0], STDIN_FILENO);
                dup2(outfd[1], STDOUT_FILENO);
                dup2(errfd[1], STDERR_FILENO);
                close(infd[0]);
                close(infd[1]);
                close(outfd[0]);
                close(outfd[1]);
                close(errfd[0]);
                close(errfd[1]);
                execvp(argv[0], const_cast<char* const*>(argv));
                //error has occured
                exit(1);
            }//mogelijke errorcheck pid < 0
            close(infd[0]);
            close(outfd[1]);
            close(errfd[1]);
            if(closein)
            {
                close(infd[1]);
                infd[1] = -1;
            }
            if(closeout)
            {
                close(outfd[0]);
                outfd[0] = -1;
            }
            if(closeerr)
            {
                close(errfd[0]);
                errfd[0] = -1;
            }
            return make_tuple(fdbuf(infd[1]), fdbuf(outfd[0]), fdbuf(errfd[0]), pid);
        }
#elif USE_IN_FDBUF_PROCESS == 2
        static tuple<fdbuf, fdbuf, fdbuf, pid_t> process(const char* const* argv, bool closein, bool closeout, bool closeerr)
        {
            int infd[2];
            int outfd[2];
            int errfd[2];
            if(::pipe(infd) | ::pipe(outfd) | ::pipe(errfd))
            {
                //error
            }
            posix_spawn_file_actions_t action;

            posix_spawn_file_actions_init(&action);
            posix_spawn_file_actions_addclose(&action, infd[1]);
            posix_spawn_file_actions_addclose(&action, outfd[0]);
            posix_spawn_file_actions_addclose(&action, errfd[0]);
            posix_spawn_file_actions_adddup2(&action, infd[0], STDIN_FILENO);
            posix_spawn_file_actions_adddup2(&action, outfd[1], STDOUT_FILENO);
            posix_spawn_file_actions_adddup2(&action, errfd[1], STDERR_FILENO);
            posix_spawn_file_actions_addclose(&action, infd[0]);
            posix_spawn_file_actions_addclose(&action, outfd[1]);
            posix_spawn_file_actions_addclose(&action, errfd[1]);
            pid_t pid;
            posix_spawnp(&pid, argv[0], &action, nullptr, const_cast<char* const*>(argv), nullptr);

            close(infd[0]);
            close(outfd[1]);
            close(errfd[1]);
            if(closein)
            {
                close(infd[1]);
                infd[1] = -1;
            }
            if(closeout)
            {
                close(outfd[0]);
                outfd[0] = -1;
            }
            if(closeerr)
            {
                close(errfd[0]);
                errfd[0] = -1;
            }
            return make_tuple(fdbuf(infd[1]), fdbuf(outfd[0]), fdbuf(errfd[0]), pid);
        }
#else
        static tuple<fdbuf, fdbuf, fdbuf, pid_t> process(const char* const* argv, bool closein, bool closeout, bool closeerr)
        {
            int infd[2];
            int outfd[2];
            int errfd[2];
            if(::pipe(infd) | ::pipe(outfd) | ::pipe(errfd))
            {}//error
            pid_t pid = vfork();
            if(pid == 0) //!pid
            {
                //child
                close(infd[1]);
                close(outfd[0]);
                close(errfd[0]);
                dup2(infd[0], STDIN_FILENO);
                dup2(outfd[1], STDOUT_FILENO);
                dup2(errfd[1], STDERR_FILENO);
                close(infd[0]);
                close(outfd[1]);
                close(errfd[1]);
                execvp(argv[0], const_cast<char* const*>(argv));
                //error has occured
                exit(1);
            }
            close(infd[0]);
            close(outfd[1]);
            close(errfd[1]);
            if(closein)
            {
                close(infd[1]);
                infd[1] = -1;
            }
            if(closeout)
            {
                close(outfd[0]);
                outfd[0] = -1;
            }
            if(closeerr)
            {
                close(errfd[0]);
                errfd[0] = -1;
            }
            return make_tuple(fdbuf(infd[1]), fdbuf(outfd[0]), fdbuf(errfd[0]), pid);
        }
#endif


};

#endif//FDUTIL_HPP
