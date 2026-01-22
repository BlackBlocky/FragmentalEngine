#pragma once

#include <unordered_map>

#include <string>
#include <string_view>
#include <sstream>

#include <filesystem>

#include <functional>

#include <frag/Log.h>

namespace frag {

    /*
        The Package Interface is basically the core of every Package.
        Every Component, Asset, GameState or Scene belongs to an Package.
        Every string-key used in the Package is stored as a 64bit-hash to speed up lookups.
        If you should ever get a hash-collision (which is very unlikely), just change the name a bit :P

        Once a Package is setup, it cannot be changed anymore.
        A Package is read only after setup.

        Every String-Key will be renamed to packageName::keyName to avoid conflicts between Packages, so
        you dont have to worry about that.
    */
    class IPackage {
        public:
            virtual ~IPackage() = default;

            /*
                Things that should be done here
                - Register GameStates
                - Register Assets
                - Register Components
                - Register Scenes
            */
            virtual void setupPackage() = 0;
            // Call This after setupPackage to finish the package
            void finishPackage() {
                if (packageIsSetup) {
                    logWarn("Package {} is already setup, cannot finish it again.", packageName);
                    return;
                }
                if (!packageNameSet) {
                    logWarn("Package name is not set yet, cannot finish package.");
                    return;
                }
                
                packageHash = 0;
                packageAsString = toString();
                packageIsSetup = true;
            }

            // Setup Methods
            void rPackageName(std::string_view name) {
                if (packageIsSetup) {
                    logWarn("Package {} is already setup, cannot set package name to '{}'", packageName, name);
                    return;
                }
                if (packageNameSet) {
                    logWarn("Package name already set to '{}', cannot set it again to '{}'", packageName, name);
                    return;
                }

                packageName = "p:" + std::string(name);
                packageNameSet = true;
            }
            
            void rGameState(std::string_view name) {
                std::hash<std::string> stringHash;
                std::string nameAsString = std::string(name);
                registeredGameStates.insert({stringHash(nameAsString), nameAsString});
            };
            void rAsset(int type, std::string_view name, std::filesystem::path assetPath) {};

            class IComponent;
            template<typename T>
            void rComponent(std::string_view name, T componentClass) {
                if (packageIsSetup) {
                    logWarn("Package {} is already setup, cannot register component '{}'", packageName, name);
                    return;
                }

                static_assert(std::is_base_of<IPackage::IComponent, T>::value, "T must be derived from IPackage::IComponent"); // Change that later to the actual Component base class
            }

            // Get Methods
            std::string getPackageName() {
                return packageName;
            }
            std::uint64_t getPackageHash() {
                return 0;
            }
            std::string getPackageAsString() {
                if (!packageIsSetup) {
                    packageAsString = "Package not setup yet.";
                    return packageAsString;
                }

                return packageAsString;
            }


            


        private:
            std::string packageName;
            uint64_t packageHash;

            std::string packageAsString;

            bool packageIsSetup = false;
            bool packageNameSet = false;

            std::unordered_map<std::uint64_t, std::string> registeredGameStates; // Game States are stored in hashes

            struct assetInfo {
                int type;
                std::string name;
                std::filesystem::path path;
            };
            std::unordered_map<std::uint64_t, assetInfo> registeredAssets; // Assets-Names are stored in hashes



            // Methods

            /*
                Includes Formatting
            */
            std::string toString() {
                // Generate
                std::stringstream ss;
                ss << "Package: " << packageName << "\n";
                ss << "Package Hash: " << packageHash << "\n";
                ss << "GameStates: " << "\n";
                for (auto state : registeredGameStates) {
                    ss << "\t- Hash: " << state.first << "\n";
                    ss << "\t  Name: " << state.second << "\n";
                }
                ss << "Assets: " << "\n";
                for (auto asset : registeredAssets) {
                    ss << "\t- Hash: " << asset.first << "\n";
                    ss << "\t  Name: " << asset.second.name << "\n";
                    ss << "\t  Type: " << asset.second.type << "\n";
                    ss << "\t  Path: " << asset.second.path.string() << "\n";
                }

                return ss.str();
            }
    };

}