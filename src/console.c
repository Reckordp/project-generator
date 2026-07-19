#include <windows.h>
#include <stdio.h>
#include <conio.h>

#include "project.h"

void coding() {
  const char *pilihan[] = {
    "Application", 
    "CustomSystem", 
    "Testing", 
    "Website"
  };
  int pos, prev, ulang, resp;
  char kb, jalur[32], masukan[32], subfolder[32];
  const char *tunjuk[4];
  HANDLE pegang;
  DWORD ditulis;
  rincianproses proses;

  printf("Pilih kategori dari project yang akan dibuat\n");
  printf("[ ] Application\n");
  printf("[ ] CustomSystem\n");
  printf("[ ] Testing\n");
  printf("[ ] Website\n");
  printf("Kasih bintang untuk kategori yang sesuai dengan project yang akan dibuat.");
  printf("\x1b[4F\x1b[1C");
  prev = 0;
  pos = 1;
  while((kb = _getch()) != 0x0d) {
    switch ((int)kb) {
    case 0x48:
      if(pos > 1) {
        printf("\x1b[1F\x1b[1C");
        pos--;
      }
      break;

    case 0x50:
      if(pos < 4) {
        printf("\x1b[1E\x1b[1C");
        pos++;
      }
      break;

    case 0x20:
      printf("*\x1b[1D");
      if(prev) {
        if(pos != prev) {
          if(pos < prev) {
            ulang = prev - pos;
            while(ulang--) printf("\x1b[1E\x1b[1C");
            printf(" ");
            ulang = prev - pos;
            while(ulang--) printf("\x1b[1F\x1b[1C");
          } else {
            ulang = pos - prev;
            while(ulang--) printf("\x1b[1F\x1b[1C");
            printf(" ");
            ulang = pos - prev;
            while(ulang--) printf("\x1b[1E\x1b[1C");
          }
        }
      }
      prev = pos;
      break;
    
    default:
      // printf("0x%x\n", kb);
      break;
    }
  }

  ulang = 6 - pos;
  while(ulang--) printf("\x1b[1E");
  printf("\n\n");

  jalur[0] = 0;
  strcat(jalur, PANGKALAN_CODING);
  strcat(jalur, "\\");
  strcat(jalur, pilihan[pos - 1]);
  if(GetFileAttributes(jalur) == INVALID_FILE_ATTRIBUTES) {
    printf("Directory doesnt exist.\n");
    printf("Generate folder ");
    resp = getchar();
    if(resp == 0x59 || resp == 0x79) {
      CreateDirectory(jalur, NULL);
    } else {
      printf("Couldnt find correct location.\n");
      return;
    }
  }

  pos = strlen(jalur);
  ulang = 1;
  while(ulang) {
    jalur[pos] = 0;
    printf("Ketik nama dari project: ");
    fgets(masukan, 32, stdin);
    resp = strlen(masukan);
    masukan[resp - 1] = 0;
    strcat(jalur, "\\");
    strcat(jalur, masukan);
    if(GetFileAttributes(jalur) == INVALID_FILE_ATTRIBUTES) {
      ulang = 0;
    } else {
      printf("Project already exist!\n\n");
    }
  }

  CreateDirectory(jalur, NULL);
  subfolder[0] = 0;
  pos = strlen(jalur);
  strcat(subfolder, jalur);
  strcat(subfolder, "\\include");
  CreateDirectory(subfolder, NULL);
  subfolder[pos] = 0;
  strcat(subfolder, "\\src");
  CreateDirectory(subfolder, NULL);
  strcat(jalur, "\\Rakefile");
  pegang = CreateFile(jalur, GENERIC_WRITE, 0, NULL, 
    CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  WriteFile(pegang, templateRakefile, 705, &ditulis, NULL);
  CloseHandle(pegang);

  jalur[pos] = 0;
  tunjuk[0] = jalur;
  proses.program = "code";
  proses.jumlahargs = 1;
  proses.argument = tunjuk;
  mulai_proses(&proses);
}

void repository() {
  char masukan[2048];
  printf("Target: ");
  fgets(masukan, 2048, stdin);
  masukan[strlen(masukan) - 1] = 0;
  mulai_clone(masukan);
}

int main(int argc, char const *argv[]) {
  char pilihan[32], kb;
  int pos, ulang;
  HANDLE hOut;
  DWORD dwMode;

  hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hOut == INVALID_HANDLE_VALUE) return 1;
  dwMode = 0;
  if (!GetConsoleMode(hOut, &dwMode)) return 1;
  dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(hOut, dwMode);

  printf(" ____  ____   ___      _ _____ ____ _____ \n");
  printf("|  _ \\|  _ \\ / _ \\    | | ____/ ___|_   _|\n");
  printf("| |_) | |_) | | | |_  | |  _|| |     | |  \n");
  printf("|  __/|  _ <| |_| | |_| | |__| |___  | |  \n");
  printf("|_|   |_| \\_\\\\___/ \\___/|_____\\____| |_|  \n\n");
  printf("  ____ _____ _   _ _____ ____      _  _____ ___  ____  \n");
  printf(" / ___| ____| \\ | | ____|  _ \\    / \\|_   _/ _ \\|  _ \\ \n");
  printf("| |  _|  _| |  \\| |  _| | |_) |  / _ \\ | || | | | |_) |\n");
  printf("| |_| | |___| |\\  | |___|  _ <  / ___ \\| || |_| |  _ < \n");
  printf(" \\____|_____|_| \\_|_____|_| \\_\\/_/   \\_\\_| \\___/|_| \\_\\\n\n\n");
  printf("MENU PROJECT GENERATOR\n");
  printf("1. Coding\n");
  printf("2. Repository\n");
  printf("Pilih menu sesuai kebutuhanmu\x1b[2F\x1b[2 q");
  pos = 1;
  while((kb = _getch()) != 0x0d) {
    switch ((int)kb) {
    case 0x48:
      if(pos == 2) {
        printf("\x1b[1F");
        pos--;
      }
      break;

    case 0x50:
      if(pos == 1) {
        printf("\x1b[1E");
        pos++;
      }
      break;
    
    default:
      // printf("0x%x\n", kb);
      break;
    }
  }

  ulang = 4 - pos;
  while(ulang--) printf("\x1b[1E");

  printf("\n\n");
  switch(pos) {
  case 1:
    coding();
    break;

  case 2:
    repository();
    break;
  }
  return 0;
}
