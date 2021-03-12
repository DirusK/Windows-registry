#include <stdio.h> 
#include <windows.h> 
#include <Winerror.h> 
#include <iostream>

using namespace std;

int main()
{
	setlocale(LC_ALL, "ru");
	HKEY hKey;
	LONG err;
	DWORD dwIndex = 0;
	DWORD subKeySize = 256;
	DWORD displaySize;
	WCHAR szSubKeyName[256];
	char szDisplayName[256];
	cout << "СПИСОК УСТАНОВЛЕННЫХ ПРОГРАММ НА КОМПЬЮТЕРЕ: \n\n";
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
		cout << "Не найдена папка Uninstall!" << endl;
		abort();
	}
	while ((err = RegEnumKeyEx(hKey, dwIndex, szSubKeyName, &subKeySize, NULL, NULL, NULL, NULL)) != ERROR_NO_MORE_ITEMS)
	{
		if (err != ERROR_SUCCESS) {
			continue;
		}
		HKEY hSubKey;
		if (RegOpenKeyEx(hKey, szSubKeyName, 0, KEY_READ, &hSubKey) != ERROR_SUCCESS) {
			continue;
		}
		// отображаем на экране название установленной программы
		displaySize = 256;
		if (RegQueryValueEx(hSubKey, TEXT("DisplayName"), NULL, NULL, (LPBYTE)&szDisplayName, &displaySize) == ERROR_SUCCESS) {
			for (int i = 0; i < displaySize - 1; i++) {
				cout << szDisplayName[i];
			}
			cout << endl;
		}
		subKeySize = 256;
		dwIndex++;
		RegCloseKey(hSubKey);
	}
	RegCloseKey(hKey);

	cout << "************************\n\nСкрываем вкладку \"Центр управления сетями и общим доступом\" во вкладке \"Сеть и Интернет\"..." << endl;
	HKEY h;
	char value = '0';
	if (err = RegOpenKeyEx(HKEY_CLASSES_ROOT, TEXT("CLSID\\{8E908FC9-BECC-40f6-915B-F4CA0E70D03D}"), 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &h) != ERROR_SUCCESS) {
		cout << "Раздел не найден! Ошибка " <<err<< endl;
		abort();
	}
	if (RegSetValueEx(h, TEXT("System.ControlPanel.Category"), NULL, REG_SZ, (LPBYTE)&value, sizeof(value)) != ERROR_SUCCESS) {
		cout<< "При записи значения произошла ошибка" << endl;
	}
	else {
		cout << "Операция успешно завершена" << endl;
	}
	RegCloseKey(h);
	return 1;
}