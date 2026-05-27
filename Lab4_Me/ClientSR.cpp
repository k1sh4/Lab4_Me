#include <iostream>
#include "clock_h.h" // Цей файл згенерується автоматично з clock.idl

#pragma comment(lib, "rpcrt4.lib")

int main()
{
    std::cout << "Client started. Connecting to server...\n";
    RPC_STATUS status;
    RPC_WSTR szStringBinding = NULL;
    handle_t hBinding = NULL;

    // Формуємо рядок підключення (TCP/IP, localhost, порт 4545)
    status = RpcStringBindingCompose(
        NULL,
        (RPC_WSTR)L"ncacn_ip_tcp",
        (RPC_WSTR)L"127.0.0.1",
        (RPC_WSTR)L"4545",
        NULL,
        &szStringBinding);

    if (status) {
        std::cerr << "Error RpcStringBindingCompose: " << status << "\n";
        return status;
    }

    // Створюємо RPC-дескриптор (handle)
    status = RpcBindingFromStringBinding(szStringBinding, &hBinding);
    if (status) {
        std::cerr << "Error RpcBindingFromStringBinding: " << status << "\n";
        return status;
    }

    unsigned char time_buf[256];
    unsigned char date_buf[256];

    // Викликаємо віддалені процедури
    RpcTryExcept{
std::cout << "Sending GetTime() request...\n";
    GetTime(hBinding, time_buf);
        std::cout << "--> Server time: " << (char*)time_buf << "\n\n";

        std::cout << "Sending GetDate() request...\n";
        GetDate(hBinding, date_buf);
        std::cout << "--> Server date: " << (char*)date_buf << "\n"; }
        RpcExcept(1) {
        std::cerr << "Critical RPC error. Server is not responding. Code: " << RpcExceptionCode() << "\n";
    }
    RpcEndExcept

        // Очищуємо ресурси
        RpcStringFree(&szStringBinding);
    RpcBindingFree(&hBinding);

    std::cout << "\nWork finished.\n";
    system("pause");
    return 0;
}

// Функції виділення пам'яті для RPC (потрібні і на клієнті)
void* __RPC_USER midl_user_allocate(size_t size) { return malloc(size); }
void __RPC_USER midl_user_free(void* p) { free(p); }