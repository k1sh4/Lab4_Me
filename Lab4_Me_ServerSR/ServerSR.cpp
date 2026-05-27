#include <iostream>
#include <ctime>
#include "clock_h.h" // Цей файл згенерується автоматично з clock.idl

#pragma comment(lib, "rpcrt4.lib") // Підключаємо бібліотеку RPC
#pragma warning(disable: 4996)     // Вимикаємо попередження для старих функцій часу

RPC_STATUS CALLBACK SecurityCallback(RPC_IF_HANDLE /*hInterface*/, void* /*pBindingHandle*/)
{
    return RPC_S_OK; // Дозволяємо підключення всім
}

// Реалізація функції отримання часу
void GetTime(handle_t hBinding, unsigned char time_str[256]) {
    std::cout << "[Server] Client requested current time.\n";   
    time_t now = time(0);
    struct tm tstruct = *localtime(&now);
    strftime((char*)time_str, 256, "%X", &tstruct); // Приводимо до char* для strftime
}

// Реалізація функції отримання дати
void GetDate(handle_t hBinding, unsigned char date_str[256]) {
    std::cout << "[Server] Client requested current date.\n"; 
    time_t now = time(0);
    struct tm tstruct = *localtime(&now);
    strftime((char*)date_str, 256, "%Y-%m-%d", &tstruct); // Приводимо до char* для strftime
}


int main()
{
    std::cout << "Clock server started...!\n";
    RPC_STATUS status;

    // Використовуємо TCP/IP, порт 4545
    status = RpcServerUseProtseqEp(
        (RPC_WSTR)L"ncacn_ip_tcp",
        RPC_C_PROTSEQ_MAX_REQS_DEFAULT,
        (RPC_WSTR)L"4545",
        NULL);

    if (status) exit(status);

    // Реєструємо інтерфейс (зверни увагу на clock_v1_0_s_ifspec)
    status = RpcServerRegisterIf2(
        clock_v1_0_s_ifspec,
        NULL,
        NULL,
        RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH,
        RPC_C_LISTEN_MAX_CALLS_DEFAULT,
        (unsigned)-1,
        SecurityCallback);

    if (status) exit(status);

    std::cout << "Waiting for client connections...\n";
    status = RpcServerListen(1, RPC_C_LISTEN_MAX_CALLS_DEFAULT, FALSE);

    if (status) exit(status);
}

// Функції виділення пам'яті для RPC
void* __RPC_USER midl_user_allocate(size_t size) { return malloc(size); }
void __RPC_USER midl_user_free(void* p) { free(p); }