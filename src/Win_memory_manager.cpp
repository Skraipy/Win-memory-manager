#include <windows.h>  // Библиотека для работы с системными API Windows
#include <iostream>   // Ввод и вывод
#include <iomanip>    // Манипуляторы ввода-вывода (например, std::hex)
#include <locale>     // Локализация (установка русского языка)
#include <codecvt>    // Кодировки UTF-16
#include <fcntl.h>    // Установка режимов консоли
#include <io.h>       // Управление вводом/выводом
//---------------------------------------------------------------------------//
void GSI();//GetSystemInformation
void GMS();//GetMemoryStatus
void QMR();//QueryMemoryRegion
void RM(bool userDefined);//ReserveMemory
void AARM(bool userDefined);//AllocateAndReserveMemory
void WTM();//WriteToMemory
void PM();//ProtectMemory
void RM();//ReadMemory
//---------------------------------------------------------------------------//
int main() {
    int choice;
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    do {
        std::wcout << L"\nМеню программы:\n"
                   << L"1. Информация о системе\n"
                   << L"2. Статус виртуальной памяти\n"
                   << L"3. Проверить регион памяти\n"
                   << L"4. Зарезервировать память (Автоматический/Ручной ввод адреса)\n"
                   << L"5. Выделить и зарезервировать память (Автоматический/Ручной ввод адреса)\n"
                   << L"6. Записать данные в память\n"
                   << L"7. Установить защиту для памяти\n"
                   << L"8. Чтение данных с указанного адреса памяти\n"
                   << L"9. Выход\n"
                   << L"Выберите опцию: ";
        std::wcin >> choice;
        std::wcin.ignore();
        switch (choice) {
            case 1: GSI(); break;
            case 2: GMS(); break;
            case 3: QMR(); break;
            case 4: {
                std::wcout << L"1 - Авто, 2 - Ввести адрес: ";
                int opt;
                std::wcin >> opt;
                RM(opt == 2);
                break;
            }
            case 5: {
                std::wcout << L"1 - Авто, 2 - Ввести адрес: ";
                int opt;
                std::wcin >> opt;
                AARM(opt == 2);
                break;
            }
            case 6: WTM(); break;
            case 7: PM(); break;
            case 8: RM();break;  
            case 9: std::wcout << L"Выход...\n"; break;
            default: std::wcout << L"Неверный выбор!\n";
        }
    } while (choice != 9);
    return 0;
}
//---------------------------------------------------------------------------//
void GSI() {
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    std::wcout << L"\nИнформация о системе:\n"
               << L"Архитектура процессора: " << si.wProcessorArchitecture << L"\n"
               << L"Размер страницы: " << si.dwPageSize << L" байт\n"
               << L"Минимальный адрес: " << si.lpMinimumApplicationAddress << L"\n"
               << L"Максимальный адрес: " << si.lpMaximumApplicationAddress << L"\n";
}
//---------------------------------------------------------------------------//
void GMS() {
    MEMORYSTATUS memStatus;
    GlobalMemoryStatus(&memStatus);
    std::wcout << L"\nСтатус памяти:\n"
               << L"Всего физической памяти: " << memStatus.dwTotalPhys / (1024 * 1024) << L" МБ\n"
               << L"Доступно физической памяти: " << memStatus.dwAvailPhys / (1024 * 1024) << L" МБ\n"
               << L"Всего виртуальной памяти: " << memStatus.dwTotalVirtual / (1024 * 1024) << L" МБ\n"
               << L"Доступно виртуальной памяти: " << memStatus.dwAvailVirtual / (1024 * 1024) << L" МБ\n";
}
//---------------------------------------------------------------------------//
void QMR() {
    MEMORY_BASIC_INFORMATION mbi;
    void* address;
    std::wcout << L"Введите адрес памяти (HEX): ";
    std::wcin >> std::hex >> address;

    if (VirtualQuery(address, &mbi, sizeof(mbi))) {
        std::wcout << L"Размер региона: " << mbi.RegionSize << L" байт\n";

        // Определение состояния памяти
        std::wcout << L"Состояние: ";
        switch (mbi.State) {
            case MEM_COMMIT: std::wcout << L"Выделена (MEM_COMMIT)\n"; break;
            case MEM_RESERVE: std::wcout << L"Зарезервирована (MEM_RESERVE)\n"; break;
            case MEM_FREE: std::wcout << L"Свободна (MEM_FREE)\n"; break;
            default: std::wcout << L"Неизвестное\n";
        }
        // Определение типа памяти
        std::wcout << L"Тип: ";
        switch (mbi.Type) {
            case MEM_PRIVATE: std::wcout << L"Приватная (MEM_PRIVATE)\n"; break;
            case MEM_MAPPED: std::wcout << L"Отображена из файла (MEM_MAPPED)\n"; break;
            case MEM_IMAGE: std::wcout << L"Загружена из исполняемого файла (MEM_IMAGE)\n"; break;
            default: std::wcout << L"Неизвестный\n";
        }
    } else {
        std::wcout << L"Ошибка VirtualQuery!\n";
    }
}
//---------------------------------------------------------------------------//
void RM(bool userDefined) {
    void* address = nullptr;
    if (userDefined) {
        std::wcout << L"Введите начальный адрес (HEX): ";
        std::wcin >> std::hex >> address;
    }
    void* allocated = VirtualAlloc(address, 4096, MEM_RESERVE, PAGE_NOACCESS);
    if (allocated) {
        std::wcout << L"Память зарезервирована по адресу: " << allocated << L"\n";
    } else {
        std::wcout << L"Ошибка резервирования памяти!\n";
    }
}
//---------------------------------------------------------------------------//
void AARM(bool userDefined) {
    void* address = nullptr;
    if (userDefined) {
        std::wcout << L"Введите адрес (HEX): ";
        std::wcin >> std::hex >> address;
    }
    void* allocated = VirtualAlloc(address, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (allocated) {
        std::wcout << L"Память выделена и зарезервирована по адресу: " << allocated << L"\n";
    } else {
        std::wcout << L"Ошибка выделения памяти!\n";
    }
}
//---------------------------------------------------------------------------//
void WTM() {
    void* address;
    wchar_t data[256];
    std::wcout << L"Введите адрес (HEX): ";
    std::wcin >> std::hex >> address;
    std::wcin.ignore();  
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery(address, &mbi, sizeof(mbi)) == 0) {
        std::wcout << L"Ошибка VirtualQuery! Неверный адрес.\n";
        return;
    }
    if (!(mbi.State & MEM_COMMIT)) {
        std::wcout << L"Ошибка: Память не выделена! Используйте функцию выделения памяти.\n";
        return;
    }
    if (!(mbi.Protect & (PAGE_READWRITE | PAGE_EXECUTE_READWRITE))) {
        std::wcout << L"Ошибка: Память защищена от записи! Измените защиту памяти.\n";
        return;
    }
    std::wcout << L"Введите строку (максимум 255 символов): ";
    std::wcin.ignore();  // Игнорируем предыдущие символы в буфере
    std::wcin.getline(data, 256);  
    wcscpy((wchar_t*)address, data);
    std::wcout << L"Данные успешно записаны в память по адресу: " << address << L"\n";
}
//---------------------------------------------------------------------------//
void PM() {
    void* address;
    DWORD oldProtect;
    int protectType;
    std::wcout << L"Введите адрес региона памяти (HEX): ";
    std::wcin >> std::hex >> address;
    std::wcout << L"Выберите уровень защиты:\n"
               << L"1 - Только чтение\n"
               << L"2 - Чтение и запись\n"
               << L"3 - Выполнение\n"
               << L"4 - Нет доступа\n"
               << L"Выбор: ";
    std::wcin >> protectType;
    DWORD newProtect;
    switch (protectType) {
        case 1: newProtect = PAGE_READONLY; break;
        case 2: newProtect = PAGE_READWRITE; break;
        case 3: newProtect = PAGE_EXECUTE; break;
        case 4: newProtect = PAGE_NOACCESS; break;
        default: std::wcout << L"Неверный выбор!\n"; return;
    }
    if (VirtualProtect(address, 4096, newProtect, &oldProtect)) {
        std::wcout << L"Защита памяти установлена!\n";
    } else {
        std::wcout << L"Ошибка установки защиты!\n";
    }
}
//---------------------------------------------------------------------------//
void RM(){
    void* address;
    wchar_t checkData[256];
    std::wcout << L"Введите адрес для чтения данных(HEX): ";
    std::wcin >> std::hex >> address;
    std::wcin.ignore();  // Очистка буфера ввода после ввода адреса
    wcscpy(checkData, (wchar_t*)address);  // Считываем данные обратно
    if ((checkData[0])==L'\0')
    {
        std::wcout << L"Память пуста, попробуйте занести данные по этому адресу:"<<address<< L"\n";
    }
    else
    {
        std::wcout << L"Прочитанные данные из памяти: " << checkData << L"\n";
    }
}
//---------------------------------------------------------------------------//