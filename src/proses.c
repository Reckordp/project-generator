#include <windows.h>
#include <stdio.h>

#include "project.h"

void mulai_proses(rincianproses *ket) {
  SECURITY_ATTRIBUTES attr;
  HANDLE pegang, baca, tulis;
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  DWORD dibaca;
  WINBOOL lembut;
  size_t butuh, kini, urutan, total, beda, panjang;
  char *perintah, *lokasi, *pilihan, *asal, *ujung, tahan[4096];
  const char *sumber;

  ZeroMemory(&attr, sizeof(SECURITY_ATTRIBUTES));
  ZeroMemory(&si, sizeof(STARTUPINFO));
  ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

  attr.nLength = sizeof(SECURITY_ATTRIBUTES);
  attr.bInheritHandle = TRUE;
  attr.lpSecurityDescriptor = NULL;

  CreatePipe(&baca, &tulis, &attr, 0);
  SetHandleInformation(baca, HANDLE_FLAG_INHERIT, 0);

  si.cb = sizeof(STARTUPINFO);
  si.hStdError = tulis;
  si.hStdOutput = tulis;
  si.dwFlags = STARTF_USESTDHANDLES;

  butuh = strlen(ket->program) + 6;
  perintah = malloc(butuh + 1);
  perintah[0] = 0;
  strcat(perintah, "where ");
  strcat(perintah, ket->program);
  lembut = CreateProcess(NULL, perintah, NULL, NULL, TRUE, 
    CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
  CloseHandle(tulis);
  ket->pid = pi.dwProcessId;
  butuh = 1;
  urutan = 1;
  kini = PANJANG_TAHAN_AWAL;
  perintah = realloc(perintah, kini);
  perintah[0] = 0x22;
  while(1) {
    lembut = ReadFile(baca, tahan, 4096, &dibaca, NULL);
    if(!lembut && dibaca == 0) break;
    tahan[dibaca] = 0;
    butuh += dibaca;
    if(kini < butuh + 2) {
      while(kini < butuh + 2) kini *= 2;
      perintah = realloc(perintah, kini);
    }
    strcpy(perintah + urutan, tahan);
    urutan += dibaca;
  }

  WaitForSingleObject(pi.hProcess, INFINITE);
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  CloseHandle(baca);

  asal = perintah;
  pilihan = NULL;
  while(!pilihan) {
    ujung = strchr(asal, 0x0a);
    if(!ujung) return;
    *ujung = 0;
    pilihan = strrchr(asal, 0x5c);
    if(!pilihan) return;
    lokasi = strchr(pilihan, 0x2e);
    if(!lokasi) {
      asal = ujung + 1;
      pilihan = NULL;
      continue;
    } 
    ujung--;
    lokasi++;
    if(strncmp(lokasi, "exe", 3) == 0) {
      pilihan = asal;
      *ujung = 0x22;
    } else if(strncmp(lokasi, "cmd", 3) == 0) {
      pilihan = asal;
      *ujung = 0x22;
    }
  }

  // lokasi = strchr(pilihan, 0x0a);
  // if(!lokasi) return;
  // lokasi--;
  // *lokasi = 0;
  panjang = strlen(pilihan) + 1;
  asal = malloc(panjang + 3);
  *asal = 0x22;
  strcpy(asal + 1, pilihan);
  lokasi = asal + panjang;
  // *lokasi++ = 0x22;
  *lokasi++ = 0x20;
  *lokasi++ = 0x00;
  panjang++;

  butuh = 0;
  for (size_t i = 0; i < ket->jumlahargs; i++) {
    butuh += strlen(ket->argument[i]) + 2;
  }

  beda = panjang;
  total = beda + butuh + ket->jumlahargs - 1;
  if(kini < total) {
    while(kini < total) kini *= 2;
    perintah = realloc(perintah, kini);
  }

  lokasi = perintah + beda;
  strcpy(perintah, asal);
  free(asal);
  
  for (size_t i = 0; i < ket->jumlahargs; i++) {
    sumber = ket->argument[i];
    butuh = strlen(sumber);
    *lokasi++ = 0x22;
    while(butuh--) *lokasi++ = *sumber++;
    *lokasi++ = 0x22;
    *lokasi++ = 0x20;
  }
  
  *lokasi++ = 0;
  ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
  ZeroMemory(&si, sizeof(STARTUPINFO));
  si.cb = sizeof(STARTUPINFO);
  lembut = CreateProcess(NULL, perintah, NULL, NULL, FALSE, 
    DETACHED_PROCESS | CREATE_NEW_PROCESS_GROUP, NULL, NULL, &si, &pi);
  WaitForSingleObject(pi.hProcess, INFINITE);
  ket->pid = pi.dwProcessId;
  CloseHandle(pi.hThread);
  CloseHandle(pi.hProcess);
  free(perintah);
}

void mulai_clone(const char *batch) {
  SECURITY_ATTRIBUTES attr;
  HANDLE pegang, baca, tulis;
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  DWORD dibaca;
  WINBOOL lembut;
  size_t butuh, kini, urutan, total, beda, panjang;
  char *perintah, *lokasi, *pilihan, *asal, *ujung, tahan[4096];
  const char *sumber;

  ZeroMemory(&attr, sizeof(SECURITY_ATTRIBUTES));
  ZeroMemory(&si, sizeof(STARTUPINFO));
  ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

  attr.nLength = sizeof(SECURITY_ATTRIBUTES);
  attr.bInheritHandle = TRUE;
  attr.lpSecurityDescriptor = NULL;

  CreatePipe(&baca, &tulis, &attr, 0);
  SetHandleInformation(baca, HANDLE_FLAG_INHERIT, 0);

  si.cb = sizeof(STARTUPINFO);
  si.hStdError = tulis;
  si.hStdOutput = tulis;
  si.dwFlags = STARTF_USESTDHANDLES;

  butuh = 3 + 6;
  perintah = malloc(butuh + 1);
  perintah[0] = 0;
  strcat(perintah, "where git");
  lembut = CreateProcess(NULL, perintah, NULL, NULL, TRUE, 
    CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
  CloseHandle(tulis);
  butuh = 1;
  urutan = 1;
  kini = PANJANG_TAHAN_AWAL;
  perintah = realloc(perintah, kini);
  perintah[0] = 0x22;
  while(1) {
    lembut = ReadFile(baca, tahan, 4096, &dibaca, NULL);
    if(!lembut && dibaca == 0) break;
    tahan[dibaca] = 0;
    butuh += dibaca;
    if(kini < butuh + 2) {
      while(kini < butuh + 2) kini *= 2;
      perintah = realloc(perintah, kini);
    }
    strcpy(perintah + urutan, tahan);
    urutan += dibaca;
  }

  WaitForSingleObject(pi.hProcess, INFINITE);
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  CloseHandle(baca);

  asal = perintah;
  pilihan = NULL;
  while(!pilihan) {
    ujung = strchr(asal, 0x0a);
    if(!ujung) return;
    *ujung = 0;
    pilihan = strrchr(asal, 0x5c);
    if(!pilihan) return;
    lokasi = strchr(pilihan, 0x2e);
    if(!lokasi) {
      asal = ujung + 1;
      pilihan = NULL;
      continue;
    } 
    ujung--;
    lokasi++;
    if(strncmp(lokasi, "exe", 3) == 0) {
      pilihan = asal;
      *ujung = 0x22;
    } else if(strncmp(lokasi, "cmd", 3) == 0) {
      pilihan = asal;
      *ujung = 0x22;
    }
  }

  // lokasi = strchr(pilihan, 0x0a);
  // if(!lokasi) return;
  // lokasi--;
  // *lokasi = 0;
  panjang = strlen(pilihan) + 1;
  asal = malloc(panjang + 3);
  *asal = 0x22;
  strcpy(asal + 1, pilihan);
  lokasi = asal + panjang;
  // *lokasi++ = 0x22;
  *lokasi++ = 0x20;
  *lokasi++ = 0x00;
  panjang++;

  butuh = 5 + strlen(batch);

  beda = panjang;
  total = beda + butuh + 2 - 1;
  if(kini < total) {
    while(kini < total) kini *= 2;
    perintah = realloc(perintah, kini);
  }

  lokasi = perintah + beda;
  strcpy(perintah, asal);
  free(asal);
  
  sumber = "clone";
  butuh = strlen(sumber);
  *lokasi++ = 0x22;
  while(butuh--) *lokasi++ = *sumber++;
  *lokasi++ = 0x22;
  *lokasi++ = 0x20;

  sumber = batch;
  butuh = strlen(sumber);
  *lokasi++ = 0x22;
  while(butuh--) *lokasi++ = *sumber++;
  sumber = ".git";
  butuh = strlen(sumber);
  if(strncmp(lokasi - 4, sumber, butuh) != 0) {
    while(butuh--) *lokasi++ = *sumber++;
  }
  *lokasi++ = 0x22;
  *lokasi++ = 0x20;

  sumber = PANGKALAN_CODING;
  butuh = strlen(sumber);
  *lokasi++ = 0x22;
  while(butuh--) *lokasi++ = *sumber++;
  sumber = "\\Repository\\";
  butuh = strlen(sumber);
  while(butuh--) *lokasi++ = *sumber++;
  asal = strrchr(batch, 0x2f) + 1;
  ujung = strchr(asal, 0x2e);
  if(!ujung) {
    ujung = strchr(asal, 0x00);
  }
  sumber = asal;
  butuh = ujung - sumber;
  while(butuh--) *lokasi++ = *sumber++;
  *lokasi++ = 0x22;
  *lokasi++ = 0x20;
  
  *lokasi++ = 0;
  ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
  ZeroMemory(&si, sizeof(STARTUPINFO));

  CreatePipe(&baca, &tulis, &attr, 0);
  SetHandleInformation(baca, HANDLE_FLAG_INHERIT, 0);

  si.cb = sizeof(STARTUPINFO);
  si.hStdError = tulis;
  si.hStdOutput = tulis;
  si.dwFlags = STARTF_USESTDHANDLES;

  lembut = CreateProcess(NULL, perintah, NULL, NULL, FALSE, 
    CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
  CloseHandle(tulis);
  while(1) {
    lembut = ReadFile(baca, tahan, 4096, &dibaca, NULL);
    if(!lembut && dibaca == 0) break;
    tahan[dibaca] = 0;
    printf("%s", tahan);
  }
  WaitForSingleObject(pi.hProcess, INFINITE);
  CloseHandle(pi.hThread);
  CloseHandle(pi.hProcess);
  CloseHandle(baca);
  free(perintah);
}