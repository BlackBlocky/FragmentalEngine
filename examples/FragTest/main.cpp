#include <iostream>
#include <frag/Core.h>
#include <frag/Log.h>
#include <frag/package/IPackage.h>

#include <string>

#include <mutex>
#include <thread>
#include <chrono>

#include "testPackage.h"

int main() {
    //std::cout << "Heyyy\n" << sizeof(int) << " " << sizeof(std::mutex) << std::endl;


    frag::Core core;
    
    frag::logging_enableColorizedOutput(true);
    frag::logging_enableColorWholeMessage(false);
    frag::logging_setLogLevel(frag::LogLevel::TRACE);

    std::string lol = "Hi {}";
    
    frag::logTrace("Hello World! sdasdas {}", 2);
    frag::logDebug("Hello World! {}", 3);
    frag::log("Hello World! {}", 1);
    frag::logWarn("Hello World! {}", 5);
    frag::logError("Hello World! {}", 4);
    frag::logFatal("Hello World! {}", 5);

    int i = 0;

    while (false && i < 150)
    {
        frag::log("Log Nr {}", i);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        i++;
        //std::cin >> lol;
    }

    TestPackage testPackage;
    testPackage.setupPackage();
    std::cout << "Test: \n" << testPackage.getPackageAsString() << std::endl;

    //std::cin >> lol;

    std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Giving the logger at least a chance to start the logging thread xD
        
    return 0;
}