//#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <Winhttp.h>

#pragma comment(lib, "Winhttp.lib")

void CALLBACK AsyncCallback(HINTERNET hInternet, DWORD * dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInfoLength)
{

   if (dwInternetStatus == WINHTTP_CALLBACK_STATUS_SECURE_FAILURE)
   {
      char buffer[128] = { 0 };
      DWORD* details = (DWORD*)lpvStatusInformation;

      if ((*details)&WINHTTP_CALLBACK_STATUS_FLAG_CERT_REV_FAILED)
      {
         strcat_s(buffer, "CERT_REV_FAILED#");
      }
      if ((*details)&WINHTTP_CALLBACK_STATUS_FLAG_INVALID_CERT)
      {
         strcat_s(buffer, "INVALID_CERT#");
      }
      if ((*details)& WINHTTP_CALLBACK_STATUS_FLAG_CERT_REVOKED)
      {
         strcat_s(buffer, "CERT_REVOKED#");
      }
      if ((*details)& WINHTTP_CALLBACK_STATUS_FLAG_INVALID_CA)
      {
         strcat_s(buffer, "INVALID_CA#");
      }
      if ((*details)& WINHTTP_CALLBACK_STATUS_FLAG_CERT_CN_INVALID)
      {
         strcat_s(buffer, "CERT_CN_INVALID#");
      }
      if ((*details)& WINHTTP_CALLBACK_STATUS_FLAG_CERT_DATE_INVALID)
      {
         strcat_s(buffer, "CERT_DATE_INVALID#");
      }
      if ((*details)& WINHTTP_CALLBACK_STATUS_FLAG_CERT_WRONG_USAGE)
      {
         strcat_s(buffer, "CERT_WRONG_USAGE#");
      }
      if ((*details)& WINHTTP_CALLBACK_STATUS_FLAG_SECURITY_CHANNEL_ERROR)
      {
         strcat_s(buffer, "SECURITY_CHANNEL_ERROR#");
      }
      std::cout << buffer << std::endl;
   }
}


int main()
{
   DWORD dwSize = 0;
   DWORD dwDownloaded = 0;
   LPSTR pszOutBuffer;
   BOOL  bResults = FALSE;
   HINTERNET  hSession = NULL,
      hConnect = NULL,
      hRequest = NULL;

   // Use WinHttpOpen to obtain a session handle.
   hSession = WinHttpOpen(L"WinHTTP Example/1.0",WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

   // Specify an HTTP server.
   if (hSession)
   {
      BOOL win32Result{ FALSE };
      DWORD protocol = { WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_1};
      win32Result = WinHttpSetOption(hSession, WINHTTP_OPTION_SECURE_PROTOCOLS, &protocol, sizeof(protocol));
      if (FALSE == win32Result) {
         std::cout << "Can not set TLS 1.1 or TLS 1.2." << std::endl;
      }

      // Install the status callback function.
      WINHTTP_STATUS_CALLBACK isCallback = WinHttpSetStatusCallback(hSession, (WINHTTP_STATUS_CALLBACK)AsyncCallback, WINHTTP_CALLBACK_STATUS_SECURE_FAILURE, NULL);
     
     // hConnect = WinHttpConnect(hSession, L"www.revoked.badssl.com", INTERNET_DEFAULT_HTTPS_PORT, 0);
      hConnect = WinHttpConnect(hSession, L"api3.gameassists.co.uk", INTERNET_DEFAULT_HTTPS_PORT, 0);
   }


   // Create an HTTP request handle.
   if (hConnect)
   {
      hRequest = WinHttpOpenRequest(hConnect, L"POST", L"/casino/user/public/v1/accounts/login/real", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE); //last param INTERNET_DEFAULT_HTTPS_PORT?
   }
   // Send a request.
   if (hRequest)
   {
      //DWORD dwIgnoreFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_CERT_DATE_INVALID | SECURITY_FLAG_IGNORE_CERT_CN_INVALID |  SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE;
      //if (WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &dwIgnoreFlags, sizeof(dwIgnoreFlags)) == TRUE)
      //{
      //   printf("WinHttpSetOption ok\n");
      //}
 
      bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
   }

   // End the request.
   if (bResults)
      bResults = WinHttpReceiveResponse(hRequest, NULL);

   // ...

   // Report any errors.
   if (!bResults)
      printf("Error %d has occurred.\n", GetLastError());

   // Close any open handles.
   if (hRequest) WinHttpCloseHandle(hRequest);
   if (hConnect) WinHttpCloseHandle(hConnect);
   if (hSession) WinHttpCloseHandle(hSession);

   system("pause");
   return 0;
}