#pragma once

#include <string>
#include <format>
#include <iostream>
#include <string_view>
#include <sstream>

#include <memory>

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <chrono>

#include <unordered_map>

/*
    FragLogger - A simple asynchronous logger for the Fragmental Engine
    Uses a fixed-size stack to store log messages and a dedicated thread to process and output them.
    Features:
    - Thread-safe logging.
    - Asynchronous processing to avoid blocking the main thread.
    - Log-Levels (TRACE, DEBUG, INFO, WARNING, ERROR, FATAL)
    - Will only have an std::cout like performance impact, if there are more than STACK_SIZE logs buffered.
        You can increase this STACK_SIZE, but keep in mind that every Item needs about 
*/

namespace frag {

    enum class LogLevel {
        TRACE = 6,
        DEBUG = 5,
        INFO = 4,
        WARNING = 3,
        ERROR = 2,
        FATAL = 1,
        NONE = 0
    };
    
    class FragLogger {
        private:

            std::chrono::time_point<std::chrono::system_clock> wakeUpTime = std::chrono::system_clock::now();

            std::unordered_map<LogLevel, std::string> logLevelToString = {
                { LogLevel::TRACE, "TRACE" },
                { LogLevel::DEBUG, "DEBUG" },
                { LogLevel::INFO, "INFO" },
                { LogLevel::WARNING, "WARN" },
                { LogLevel::ERROR, "ERROR" },
                { LogLevel::FATAL, "FATAL" }
            };
            std::unordered_map<LogLevel, std::string> logLevelToColorCode = {
                { LogLevel::TRACE, "\e[0;30m" },   // White
                { LogLevel::DEBUG, "\e[0;35m" },   // Cyan
                { LogLevel::INFO, "\e[0;36m" },    // Green
                { LogLevel::WARNING, "\033[33m" },       // Yellow
                { LogLevel::ERROR, "\033[31m" },         // Red
                { LogLevel::FATAL, "\e[43m\e[1;31m" },   // Magenta
                { LogLevel::NONE, "\033[0m" }      // Reset
            };

            struct LogItem {
                bool alive = false;
                LogLevel level;
                std::unique_ptr<std::string> msg;
            };
            
            std::mutex writeIntoLoggerMutex;
            std::atomic<bool> loggerThreadRunning = false;
            size_t currentWriteIndex = 0;
            size_t currentReadIndex = 0; // Only used for keep track which index was lastly written.
            std::atomic<size_t> logsCounter = 0; // Counts how many logs have been written. Used for thread communication.

            static constexpr size_t STACK_SIZE = 100;
            LogItem logStack[STACK_SIZE];

            // This thread should only run ONCE. And there should NOT be running to instances of that thread at once. Just dont do it lol
            void loggingThread() {
                size_t lastLogCounter = 0;

                //std::cout << "Starting FragLogger Thread...\n";
                while (loggerThreadRunning) {
                    //std::this_thread::sleep_for(std::chrono::milliseconds(100));

                    if (logsCounter <= lastLogCounter) continue;; // Wait until logCounter inceases
                    lastLogCounter++;
                    
                    // Logging the message and resetting the log item
                    std::cout << *logStack[currentReadIndex].msg;
                    
                    logStack[currentReadIndex].alive = false;
                    logStack[currentReadIndex].msg.reset(); // Free the memory

                    currentReadIndex++;
                    if (currentReadIndex >= STACK_SIZE) currentReadIndex = 0;
                }
                //std::cout << "Stopping FragLogger Thread...\n";
            }

            std::string formatLogMessage(const std::string& msg, LogLevel level) {
                std::stringstream msgBuilder;

                if (colorizedOutput && colorWholeMessage) msgBuilder << logLevelToColorCode[level];
                
                // Timestamp
                auto timeSinceStart = std::chrono::system_clock::now().time_since_epoch() - wakeUpTime.time_since_epoch();
                int seconds = std::chrono::duration_cast<std::chrono::seconds>(timeSinceStart).count();
                int milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(timeSinceStart).count() % 1000;
                msgBuilder << seconds << "." << std::format("{:03}", milliseconds);

                // Log-Level Box
                msgBuilder << " : [";
                if (colorizedOutput && !colorWholeMessage) msgBuilder << logLevelToColorCode[level];
                msgBuilder << logLevelToString[level];
                if (colorizedOutput && !colorWholeMessage) msgBuilder << logLevelToColorCode[LogLevel::NONE];
                msgBuilder << "]";

                // Prefix

                // Message
                msgBuilder << " " << msg;
                if (colorizedOutput && colorWholeMessage) msgBuilder << logLevelToColorCode[LogLevel::NONE];
                msgBuilder << "\n";

                return msgBuilder.str();
            }

            std::thread loggerThread;

        public:
            FragLogger() {          
                loggerThreadRunning = true;
                
                loggerThread = std::thread(&FragLogger::loggingThread, this);

                std::cout << sizeof(LogItem) << "\n";
            }
            ~FragLogger() {
                std::cout << "Stopping logging thread, printing out last buffered messages...\n";

                loggerThreadRunning = false;
                loggerThread.join();

                // Make sure there are no logs left
                while (logStack[currentReadIndex].alive) {
                    logStack[currentReadIndex].alive = false;
                    std::cout << *logStack[currentReadIndex].msg;

                    currentReadIndex++;
                    if (currentReadIndex >= STACK_SIZE) currentReadIndex = 0;
                }
            }

            LogLevel logLevel = LogLevel::INFO;
            bool colorizedOutput = true;
            bool colorWholeMessage = false;

            template<typename... Args>
            void log(std::string& msg, LogLevel level) {
                {
                    std::lock_guard lock(writeIntoLoggerMutex);

                    // Just making sure we don't overwrite an alive log item (very unlikely)
                    while (logStack[currentWriteIndex].alive) {
                        //std::cout << "LOGGING NEEDED TO WAIT dqwdij2130121euß2103ß1923ß8u34üt3r+#t34+r023ir2089r230pr293r22\n";
                        std::this_thread::sleep_for(std::chrono::microseconds(1));
                    }

                    logStack[currentWriteIndex] = { false, LogLevel::INFO, std::make_unique<std::string>(formatLogMessage(msg, level)) };
                    logStack[currentWriteIndex].alive = true; // Only make it alive after everthing is written to avoid race conditions

                    currentWriteIndex++;
                    logsCounter++;

                    if (currentWriteIndex >= STACK_SIZE) currentWriteIndex = 0;
                }
            }
    };
    
    inline FragLogger fragLoggerInstance = FragLogger();

    template<typename... Args>
    inline void log(std::format_string<Args...> fmt, Args&&... args){
        if (fragLoggerInstance.logLevel < LogLevel::INFO) {
            return;
        }

        std::string result = std::format(fmt, std::forward<Args>(args)...);     
        fragLoggerInstance.log(result, LogLevel::INFO);
    }

    template<typename... Args>
    inline void logTrace(std::format_string<Args...> fmt, Args&&... args){
        if (fragLoggerInstance.logLevel < LogLevel::TRACE) {
            return;
        }

        std::string result = std::format(fmt, std::forward<Args>(args)...);     
        fragLoggerInstance.log(result, LogLevel::TRACE);
    }

    template<typename... Args>
    inline void logDebug(std::format_string<Args...> fmt, Args&&... args){
        if (fragLoggerInstance.logLevel < LogLevel::DEBUG) {
            return;
        }

        std::string result = std::format(fmt, std::forward<Args>(args)...);     
        fragLoggerInstance.log(result, LogLevel::DEBUG);
    }

    template<typename... Args>
    inline void logWarn(std::format_string<Args...> fmt, Args&&... args){
        if (fragLoggerInstance.logLevel < LogLevel::WARNING) {
            return;
        }
        
        std::string result = std::format(fmt, std::forward<Args>(args)...);     // geht nicht als string??!?!?!?!!??
        fragLoggerInstance.log(result, LogLevel::WARNING);
    }

    template<typename... Args>
    inline void logError(std::format_string<Args...> fmt, Args&&... args){
        if (fragLoggerInstance.logLevel < LogLevel::ERROR) {
            return;
        }

        std::string result = std::format(fmt, std::forward<Args>(args)...);     
        fragLoggerInstance.log(result, LogLevel::ERROR);
    }

    template<typename... Args>
    inline void logFatal(std::format_string<Args...> fmt, Args&&... args){
        if (fragLoggerInstance.logLevel < LogLevel::FATAL) {
            return;
        }

        std::string result = std::format(fmt, std::forward<Args>(args)...);     
        fragLoggerInstance.log(result, LogLevel::FATAL);
    }

    inline void logging_setLogLevel(LogLevel level) {
        fragLoggerInstance.logLevel = level;
    }
    inline void logging_enableColorizedOutput(bool enabled) {
        fragLoggerInstance.colorizedOutput = enabled;
    }
    inline void logging_enableColorWholeMessage(bool enabled) {
        fragLoggerInstance.colorWholeMessage = enabled;
    }

}