#include <iostream>
#include <Windows.h>
#include <dpapi.h>
#include <time.h>

int main()
{
    //std::cout << "Hello World!\n"; 

	/*
	DATA_BLOB input;
	DATA_BLOB inagain;
	DATA_BLOB output;
	BYTE data[] = "Hello World!!!!!!!!";
	input.pbData = data;
	input.cbData = sizeof(data);
	CryptProtectData(&input, NULL, NULL, NULL, NULL, CRYPTPROTECT_LOCAL_MACHINE, &output);
	CryptUnprotectData(&output, NULL, NULL, NULL, NULL, CRYPTPROTECT_LOCAL_MACHINE, &inagain);
	printf("%s\n\n", inagain.pbData);
	unsigned char Data [2000];
	HKEY hkey = HKEY_CLASSES_ROOT;
	DWORD X = 2000;
	LPDWORD size = &X;
	*/
	//LONG xx = RegGetValue(HKEY_CLASSES_ROOT,
	//L"Licenses\\41717607-F34E-432C-A138-A3CFD7E25CDA\\09278",
	//L"", RRF_RT_REG_BINARY, nullptr,(PVOID) Data, size);
	/*printf("return %d\n", xx);
	printf("size=%d\n", *size);
	for (int i=0; i < 200; i++) {
		printf("%x\n", Data[i]);
	}
	printf("\n%s\n", Data);
	output.pbData = Data;
	output.cbData = *size;
	CryptUnprotectData(&output, NULL, NULL, NULL, NULL, CRYPTPROTECT_LOCAL_MACHINE, &inagain);
	printf("size=%d\n", (inagain.cbData));
	for (int i = 0; i < (inagain.cbData); i++) {
		printf("%x\t", inagain.pbData[i]);
	}
	inagain.pbData[inagain.cbData - 14]--;
	printf("\n[%x]\n", inagain.pbData[inagain.cbData - 14]);
	CryptProtectData(&inagain, NULL, NULL, NULL, NULL, CRYPTPROTECT_LOCAL_MACHINE, &output);
	*/

	time_t sec, val = 1;
	struct tm* current_time;
	current_time = (struct tm*)malloc(sizeof(struct tm));
	// getting current time in seconds
	sec = time(0);
	// adding a month
	sec += (3600*24*30);
	localtime_s(current_time, &sec);
	DATA_BLOB cryptedKey = { 0 };
	DATA_BLOB decryptedKey;
	HKEY hkey;
	BYTE Data[2000];
	DWORD X = 2000;
	LPDWORD size = &X;
	//cryptedKey.pbData = (BYTE*) malloc(500);
	// obtaining registry key value
	// VSCE should be located at this position
	if (RegGetValue(HKEY_CLASSES_ROOT,
		L"Licenses\\41717607-F34E-432C-A138-A3CFD7E25CDA\\09278", L"",
		RRF_RT_REG_BINARY, nullptr, Data, size) == ERROR_SUCCESS) {
		//printf("Crypted key size: %d\n", *size);
		cryptedKey.pbData = Data;
		cryptedKey.cbData = *size;
		// decrypting key value
		CryptUnprotectData(&cryptedKey, NULL, NULL, NULL, NULL, CRYPTPROTECT_LOCAL_MACHINE, &decryptedKey);
		//printf("DeCrypted key size: %d\n", decryptedKey.cbData);
		int year;
		year = current_time->tm_year + 1900;
		decryptedKey.pbData[decryptedKey.cbData - 12] = current_time->tm_mday + 1;
		decryptedKey.pbData[decryptedKey.cbData - 14] = current_time->tm_mon + 1;
		// TODO: do this in a better way
		decryptedKey.pbData[decryptedKey.cbData - 15] = ((BYTE*)& year)[1];
		decryptedKey.pbData[decryptedKey.cbData - 16] = ((BYTE*)& year)[0];
		printf("New expiration date:\n %d/%d/%d\n", current_time->tm_mday, current_time->tm_mon + 1, year);
		// crypting the key value and replacing the old key value
		CryptProtectData(&decryptedKey, NULL, NULL, NULL, NULL, CRYPTPROTECT_LOCAL_MACHINE, &cryptedKey);
		if (RegOpenKeyExW(HKEY_CLASSES_ROOT, L"Licenses\\41717607-F34E-432C-A138-A3CFD7E25CDA\\09278", 0, KEY_SET_VALUE, &hkey) == ERROR_SUCCESS) {
			if (RegSetValueExW(hkey, L"", 0, REG_BINARY, cryptedKey.pbData, cryptedKey.cbData) == ERROR_SUCCESS) {
				printf("Key Value changed\n");
			}
			else {
				printf("Key not changed! Error : %d", (unsigned int)GetLastError());
			}
			RegCloseKey(hkey);
		}
		else {
			printf("Key Opening Failed!\n");
		}
	}
	else {
		printf("Failed to open Registry key!\n");
	}



	/*
	if (RegOpenKeyExW(HKEY_CLASSES_ROOT, L"Licenses\\41717607-F34E-432C-A138-A3CFD7E25CDA\\09278", 0, KEY_SET_VALUE, &hkey) == ERROR_SUCCESS) {
		if (RegSetValueExW(hkey, L"", 0, REG_BINARY, output.pbData, output.cbData) == ERROR_SUCCESS) {
			printf("change happened\n");
		}
		else {
			printf("Key not changed! Error : %d", (unsigned int)GetLastError());
		}
		RegCloseKey(hkey);
	}
	else {
		printf("Key Opening Failed!\n");
	}*/

}