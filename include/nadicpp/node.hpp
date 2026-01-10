#ifndef NADI_NODE_HPP
#define NADI_NODE_HPP
#include <nadi/nadi.h>
#include <string>

// Platform-specific headers
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace nadicpp{
    // Function pointer types for DLL functions
    using init_pt = nadi_status (*)(nadi_node_handle* instance, nadi_receive_callback, void*, char*);
    using deinit_pt = nadi_status (*)(nadi_node_handle instance);
    using send_pt = nadi_status (*)(nadi_message* message, nadi_node_handle instance,unsigned int target_channel);
    using handle_events_pt = nadi_status (*)(nadi_node_handle);
    using descriptor_pt = nadi_status (*)(char*,size_t*);

    struct library{
        init_pt init = 0;
        deinit_pt deinit = 0;
        send_pt send = 0;
        handle_events_pt handle_events = 0;
        descriptor_pt descriptor = 0;
        #ifdef _WIN32
        HMODULE dll;
        #else
        void * dll;
        #endif
    };

    inline library load_node(std::string path) {
        library lib{};
    #ifdef _WIN32
        // Load DLL on Windows
        lib.dll = LoadLibraryA(path.c_str());
        if (!lib.dll) {
            //TODO handle error
            return lib;
        }
    #else
        // Load DLL on Linux/macOS
        lib.dll = dlopen(path.c_str(), RTLD_LAZY);
        if (!lib.dll) {
            //TODO handle error
            return false;
        }
    #endif

        // Load function pointers
    #ifdef _WIN32
        lib.init = reinterpret_cast<init_pt>(GetProcAddress(lib.dll, "nadi_init"));
        lib.deinit = reinterpret_cast<deinit_pt>(GetProcAddress(lib.dll, "nadi_deinit"));
        lib.send = reinterpret_cast<send_pt>(GetProcAddress(lib.dll, "nadi_send"));
        lib.handle_events = reinterpret_cast<handle_events_pt>(GetProcAddress(lib.dll, "nadi_handle_events"));
        lib.descriptor = reinterpret_cast<descriptor_pt>(GetProcAddress(lib.dll, "nadi_descriptor"));
    #else
        lib.init = reinterpret_cast<init_pt>(dlsym(lib.dll, "nadi_init"));
        lib.deinit = reinterpret_cast<deinit_pt>(dlsym(lib.dll, "nadi_deinit"));
        lib.send = reinterpret_cast<send_pt>(dlsym(lib.dll, "nadi_send"));
        lib.handle_events = reinterpret_cast<handle_events_pt>(dlsym(lib.dll, "nadi_handle_events"));
        lib.descriptor = reinterpret_cast<descriptor_pt>(dlsym(lib.dll, "nadi_descriptor"));
    #endif

        // Check if functions were loaded
        if (!lib.init || !lib.deinit || !lib.send || !lib.handle_events || !lib.descriptor) {
    #ifdef _WIN32
            //std::cerr << "Failed to load functions: " << GetLastError() << "\n";
    #else
            //std::cerr << "Failed to load functions: " << dlerror() << "\n";
    #endif
    #ifdef _WIN32
            FreeLibrary(lib.dll);
    #else
            dlclose(lib.dll);
    #endif
            lib.dll = 0;
        }

        return lib;
    }
}
#endif