#ifndef BASE_THREAD
#define BASE_THREAD

#include <thread>
#include <atomic>
#include <chrono>

namespace task {

    template<class... TArgs>
    class BaseThread {
        std::atomic<bool> m_run;
        std::thread m_th;

        void join() {
            if(m_th.joinable()) {
                m_th.join();
            }
        }

        protected:
        virtual void run(TArgs... args) = 0;

        auto isRunning() const {
            return m_run.load();
        }

        void msSleep(std::intmax_t ms) const {
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        }

        public:
        BaseThread() {
            m_run.store(false);
        }

        ~BaseThread() {
            stop();
        }

        void start(TArgs... args) {
            if(!m_th.joinable()) {
                m_run.store(true);
                m_th = std::thread([&](TArgs... args){ run(args...); }, args...);
            } else {
                throw std::runtime_error("Thread already running");
            }
        }

        void stop() {
            m_run.store(false);
            join();
        }

    };

}

#endif
